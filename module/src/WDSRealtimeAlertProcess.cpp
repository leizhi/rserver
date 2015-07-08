/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        台站实时报警业务处理类                  */
/*           程序名        ：        WDSRealtimeAlertProcess.cpp             */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "WDSRealtimeAlertProcess.h"
#include "WDS_DBHelper.h"
#include "ObjectFactory.h"
#include "SQLCondition.h"

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  CWDSRealtimeAlertProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSRealtimeAlertProcess::CWDSRealtimeAlertProcess(){
    m_pSendSocket          = NULL;
    m_pRecvSocket          = NULL;
    m_nFuncType            = -1;
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~CWDSRealtimeAlertProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSRealtimeAlertProcess::~CWDSRealtimeAlertProcess(){
    m_pRecvSocket = NULL;
    if(m_pSendSocket != NULL) {
        if (!m_pSendSocket->IsClosed()){
            m_pSendSocket->Close();
        }
        delete m_pSendSocket;
        m_pSendSocket = NULL;
    }
}

/******************************************************************************
    处理名        ：  业务处理
    函数名        ：  Do()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
void CWDSRealtimeAlertProcess::Do(){
    printfs(1,"<CWDSRealtimeAlertProcess::Do()> WDSRealtimeAlertProcess start!");

    int nStatus = 0;
    int nRet = 0;
    stStationAlert stationAlert;
    memset(&stationAlert,  0, sizeof(stationAlert));

    WDS_DBHelper* pWDSHelper = NULL;
    do{
        if (m_pRecvSocket == NULL){
            printfs(0,"<CWDSRealtimeAlertProcess::Do()> m_pRecvSocket == NULL");
            nStatus = 0;
            break;
        }

        // 取得小时气象信息的长度
        int nLength = sizeof(stStationAlert) - sizeof(stHeader);

        // 【业务处理】1. 取得端口传入的小时气象数据信息数据
        nRet = m_pRecvSocket->Receive((char*)(&stationAlert) + sizeof(stHeader), nLength);
        if(nRet == 0) {
            printfs(0,"<CWDSRealtimeAlertProcess::Do()> Receive packet time out");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        if(nRet == -1) {
            printfs(0, "<CWDSRealtimeAlertProcess::Do()> Receive packet failed");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        printfs(2, "<CWDSRealtimeAlertProcess::Do()> Receive struct info:cCurTime[%s], cStationID[%.10s], cAlert[%s], alertCharacter[No.8][%#x]",
                             stationAlert.cCurTime,
                             stationAlert.cStationID,
                             stationAlert.cAlert,
                             stationAlert.cAlert[8]);

        // 业务处理，数据包校验，判断当前包是否为报警包
        if (stationAlert.cAlert[1] != 0x08){
            printfs(0,"<CWDSRealtimeAlertProcess::Do()> Packet type error, not an alert packet! Received type:[%#x]", stationAlert.cAlert[1]);
            nStatus = 0;
            break;
        }

        MYSQL* pMysqlConnection = CObjectFactory::GetInstance()->GetMySQLPool()->GetIdleMySql();
        if (pMysqlConnection == NULL){
            printfs(0,"<CWDSRealtimeAlertProcess::Do()> No enough mysql connections!");
            nStatus = 0;
            break;
        }
        pWDSHelper = new WDS_DBHelper();
        if (pWDSHelper == NULL){
            printfs(0,"<CWDSRealtimeAlertProcess::Do()> Can not connect to DB!");
            nStatus = 0;
            break;
        }
        pWDSHelper->SetMysqlHandle(pMysqlConnection);

        // step 1.业务处理，从DB取得当前台站的实时报警记录数量
        int nCount = 0;

        nStatus = pWDSHelper->GetStationRealtimeAlertCount(stationAlert.cStationID, m_nFuncType, nCount);
        if (!nStatus){
            printfs(0,"<CWDSRealtimeAlertProcess::Do()> GetStationRealtimeAlertCount() operation failed!");
            break;
        }
        int nNealyAlert[8] = {0,0,0,0,0,0,0,0};
        if (nCount != 0){
            // step 2.1.1业务处理，取得最近一条实时报警状态信息
            nStatus = pWDSHelper->GetStationNealyAlertStatus(stationAlert.cStationID, m_nFuncType, nNealyAlert);
            if (!nStatus){
                printfs(0,"<CWDSRealtimeAlertProcess::Do()> GetStationNealyAlertStatus() operation failed!");
                break;
            }
            // step 2.1.2业务处理，结合最近的实时报警信息，创建一条最新报警
            nStatus = pWDSHelper->InsertRealtimeAlert(&stationAlert, m_nFuncType, nNealyAlert);
            if (!nStatus){
                printfs(0,"<CWDSRealtimeAlertProcess::Do()> InsertRealtimeAlert() operation failed!");
                break;
            }
        }
        else{
            // step 2.1.2业务处理，创建一条最新报警
            nStatus = pWDSHelper->InsertRealtimeAlert(&stationAlert, m_nFuncType, nNealyAlert);
            if (!nStatus){
                printfs(0,"<CWDSRealtimeAlertProcess::Do()> InsertRealtimeAlert() operation failed!");
                break;
            }
        }

        if (!nStatus){
            printfs(0,"<CWDSRealtimeAlertProcess::Do()> Database operation failed!");
            break;
        }

    }
    while(0);
    if (pWDSHelper){
        CObjectFactory::GetInstance()->GetMySQLPool()->SetIdleMysql(pWDSHelper->GetMysqlHandle());
        delete pWDSHelper;
        pWDSHelper = NULL;
    }

    stAnswer answerInfo;
    memset(&answerInfo, 0, sizeof(answerInfo));
    answerInfo.nVerify1 = stationAlert.nVerify1;
    answerInfo.nStatus = nStatus;
    answerInfo.nVerify2 = stationAlert.nVerify2;

    // 应答电文送信
    nRet = m_pRecvSocket->Send((char*)(&answerInfo), sizeof(answerInfo));
    if(nRet == 0) {
        printfs(0,"<CWDSRealtimeAlertProcess::Do()> Send status code time out");
    }
    if(nRet == -1) {
        printfs(0, "<CWDSRealtimeAlertProcess::Do()> Send status code failed");
    }

    m_pRecvSocket->Close();

    printfs(1,"<CWDSRealtimeAlertProcess::Do()> WDSRealtimeAlertProcess end!");
}

/******************************************************************************
    End
******************************************************************************/
