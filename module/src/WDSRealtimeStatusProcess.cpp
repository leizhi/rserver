/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        台站实时状态业务处理类                  */
/*           程序名        ：        WDSRealtimeStatusProcess.cpp            */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "WDSRealtimeStatusProcess.h"
#include "WDS_DBHelper.h"
#include "ObjectFactory.h"
#include "SQLCondition.h"

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  CWDSRealtimeStatusProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSRealtimeStatusProcess::CWDSRealtimeStatusProcess(){
    //m_//pSysLogger           = NULL;
    m_pSendSocket          = NULL;
    m_pRecvSocket          = NULL;
    m_nFuncType            = -1;
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~CWDSRealtimeStatusProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSRealtimeStatusProcess::~CWDSRealtimeStatusProcess(){
    //m_//pSysLogger  = NULL;
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
void CWDSRealtimeStatusProcess::Do(){
    //m_//pSysLogger     = CObjectFactory::GetInstance()->GetSysLogger();
    //m_//pSysLogger->Add(1,"<CWDSRealtimeStatusProcess::Do()> WDSRealtimeStatusProcess start!");

    int nStatus = 0;
    int nRet = 0;

    WDS_DBHelper* pWDSHelper = NULL;
    do{
        if (m_pRecvSocket == NULL){
            //m_//pSysLogger->Add(0,"<CWDSRealtimeStatusProcess::Do()> m_pRecvSocket == NULL");
            nStatus = 0;
            break;
        }

        // 取得小时气象信息的长度
        int nLength = sizeof(stStationHeartBeat) - sizeof(stHeader);
        stStationHeartBeat stationHeartBeat;
        memset(&stationHeartBeat,  0, sizeof(stationHeartBeat));

        // 【业务处理】1. 取得端口传入的小时气象数据信息数据
        nRet = m_pRecvSocket->Receive((char*)(&stationHeartBeat) + sizeof(stHeader), nLength);
        if(nRet == 0) {
            //m_//pSysLogger->Add(0,"<CWDSRealtimeStatusProcess::Do()> Receive packet time out");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        if(nRet == -1) {
            //m_//pSysLogger->Add(0, "<CWDSRealtimeStatusProcess::Do()> Receive packet failed");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        /*m_//pSysLogger->Add(2, "<CWDSRealtimeStatusProcess::Do()> Receive struct info:cCurTime[%s], cStationID[%.10s], cChannelStatus[%#x], nHDstatus[%d], nHDCapacity[%d], nFreeSpace[%d]",
                             stationHeartBeat.cCurTime,
                             stationHeartBeat.cStationID,
                             stationHeartBeat.cChannelStatus,
                             stationHeartBeat.nHDstatus,
                             stationHeartBeat.nHDCapacity,
                             stationHeartBeat.nFreeSpace);
*/
        // step 1.业务处理，从DB取得当前台站的实时心跳记录数量
        int nCount = 0;

        MYSQL* pMysqlConnection = CObjectFactory::GetInstance()->GetMySQLPool()->GetIdleMySql();
        if (pMysqlConnection == NULL){
            //m_//pSysLogger->Add(0,"<CWDSRealtimeStatusProcess::Do()> No enough mysql connections!");
            nStatus = 0;
            break;
        }
        pWDSHelper = new WDS_DBHelper();
        if (pWDSHelper == NULL){
            //m_//pSysLogger->Add(0,"<CWDSRealtimeStatusProcess::Do()> Can not connect to DB!");
            nStatus = 0;
            break;
        }
        pWDSHelper->SetMysqlHandle(pMysqlConnection);

        nStatus = pWDSHelper->GetStationRealtimeStatusCount(stationHeartBeat.cStationID, stationHeartBeat.cCurTime, nCount);
        if (!nStatus){
            break;
        }
        if (nCount != 0){
            // step 2.1业务处理，向DB更新当前台站的心跳信息
            nStatus = pWDSHelper->UpdateStationRealtimeStatus(&stationHeartBeat);
        }
        else{
            // step 2.2业务处理，向DB插入当前台站的心跳信息
            nStatus = pWDSHelper->InsertStationRealtimeStatus(&stationHeartBeat);
        }

        if (!nStatus){
            //m_//pSysLogger->Add(0,"<CWDSRealtimeStatusProcess::Do()> Procdess station realtime status failed!");
            break;
        }

        stAnswer answerInfo;
        memset(&answerInfo, 0, sizeof(answerInfo));
        answerInfo.nVerify1 = stationHeartBeat.nVerify1;
        answerInfo.nStatus = nStatus;
        answerInfo.nVerify2 = stationHeartBeat.nVerify2;

        // 应答电文送信
        nRet = m_pRecvSocket->Send((char*)(&answerInfo), sizeof(answerInfo));
        if(nRet == 0) {
            //m_//pSysLogger->Add(0,"<CWDSRealtimeStatusProcess::Do()> Send status code time out");
        }
        if(nRet == -1) {
            //m_//pSysLogger->Add(0, "<CWDSRealtimeStatusProcess::Do()> Send status code failed");
        }
    }
    while(0);
    if (pWDSHelper){
        CObjectFactory::GetInstance()->GetMySQLPool()->SetIdleMysql(pWDSHelper->GetMysqlHandle());
        delete pWDSHelper;
        pWDSHelper = NULL;
    }

    m_pRecvSocket->Close();

    //m_//pSysLogger->Add(1,"<CWDSRealtimeStatusProcess::Do()> WDSRealtimeStatusProcess end!");
}

/******************************************************************************
    End
******************************************************************************/
