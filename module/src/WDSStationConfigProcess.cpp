/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        台站配置信息业务处理类                  */
/*           程序名        ：        WDSStationConfigProcess.cpp             */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "WDSStationConfigProcess.h"
#include "WDS_DBHelper.h"
#include "ObjectFactory.h"
#include "SQLCondition.h"

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  CWDSStationConfigProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSStationConfigProcess::CWDSStationConfigProcess(){
    //m_//pSysLogger           = NULL;
    m_pSendSocket          = NULL;
    m_pRecvSocket          = NULL;
    m_nFuncType            = -1;
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~CWDSStationConfigProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSStationConfigProcess::~CWDSStationConfigProcess(){
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
void CWDSStationConfigProcess::Do(){
    //m_//pSysLogger     = CObjectFactory::GetInstance()->GetSysLogger();
    //m_//pSysLogger->Add(1,"<CWDSStationConfigProcess::Do()> WDSStationConfigProcess start!");

    int nStatus = 0;
    int nRet = 0;
    // 取得信息的长度
    int nLength = sizeof(stAutoStationControlRequest) - sizeof(stHeader);
    stAutoStationControlRequest autoStationControlRequest;
    memset(&autoStationControlRequest,  0, sizeof(autoStationControlRequest));

    char chNormalInterval[DICT_VALUE_LENGTH + 1] = "\0";
    char chAlertInterval[DICT_VALUE_LENGTH + 1] = "\0";

    WDS_DBHelper* pWDSHelper = NULL;
    do{
        if (m_pRecvSocket == NULL){
            //m_//pSysLogger->Add(0,"<CWDSStationConfigProcess::Do()> m_pRecvSocket == NULL");
            nStatus = 0;
            break;;
        }

        // 【业务处理】1. 取得端口传入的数据
        nRet = m_pRecvSocket->Receive((char*)(&autoStationControlRequest) + sizeof(stHeader), nLength);
        if(nRet == 0) {
            //m_//pSysLogger->Add(0,"<CWDSStationConfigProcess::Do()> Receive packet time out");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        if(nRet == -1) {
            //m_//pSysLogger->Add(0, "<CWDSStationConfigProcess::Do()> Receive packet failed");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        //m_//pSysLogger->Add(2, "<CWDSStationConfigProcess::Do()> Receive struct info:cCurTime[%s], cStationID[%.10s]",
        //                     autoStationControlRequest.cCurTime,
        //                     autoStationControlRequest.cStationID);

        MYSQL* pMysqlConnection = CObjectFactory::GetInstance()->GetMySQLPool()->GetIdleMySql();
        if (pMysqlConnection == NULL){
            //m_//pSysLogger->Add(0,"<CWDSStationConfigProcess::Do()> No enough mysql connections!");
            nStatus = 0;
            break;
        }
        pWDSHelper = new WDS_DBHelper();
        if (pWDSHelper == NULL){
            //m_//pSysLogger->Add(0,"<CWDSStationConfigProcess::Do()> Can not connect to DB!");
            nStatus = 0;
            break;
        }
        pWDSHelper->SetMysqlHandle(pMysqlConnection);

        // 业务处理，取得系统配置表中的正常抓拍时间间隔、报警抓拍时间间隔
        nStatus = pWDSHelper->GetSysConfigValuebyCode(SYSTEM_CONFIG_NORMAL_INTERVAL, chNormalInterval);
        if (!nStatus){
            //m_//pSysLogger->Add(0,"<CWDSStationConfigProcess::Do()> GetSysConfigValuebyCode-NormalInterval operation failed!");
            break;
        }
        nStatus = pWDSHelper->GetSysConfigValuebyCode(SYSTEM_CONFIG_ALERT_INTERVAL, chAlertInterval);
        if (!nStatus){
            //m_//pSysLogger->Add(0,"<CWDSStationConfigProcess::Do()> GetSysConfigValuebyCode-AlertInterval operation failed!");
            break;
        }
    }
    while(0);
    if (pWDSHelper){
        CObjectFactory::GetInstance()->GetMySQLPool()->SetIdleMysql(pWDSHelper->GetMysqlHandle());
        delete pWDSHelper;
        pWDSHelper = NULL;
    }

    // 发送标准应答
    stAnswer answerInfo;
    memset(&answerInfo, 0, sizeof(answerInfo));
    answerInfo.nVerify1 = autoStationControlRequest.nVerify1;
    answerInfo.nStatus = nStatus;
    answerInfo.nVerify2 = autoStationControlRequest.nVerify2;

    // 应答电文送信
    nRet = m_pRecvSocket->Send((char*)(&answerInfo), sizeof(answerInfo));
    if(nRet == 0) {
        //m_//pSysLogger->Add(0,"<CWDSStationRegisterProcess::Do()> Send status code time out");
    }
    if(nRet == -1) {
        //m_//pSysLogger->Add(0, "<CWDSStationRegisterProcess::Do()> Send status code failed");
    }

    // 发送系统配置应答
    if(nStatus) {
        stAutoStationControlAnswer autoStationControlAnswer;
        memset(&autoStationControlAnswer, 0, sizeof(autoStationControlAnswer));
        autoStationControlAnswer.nVerify1 = autoStationControlRequest.nVerify1;
        memcpy(autoStationControlAnswer.cCurTime, autoStationControlRequest.cCurTime, DATE_TIME_LENGTH);
        memcpy(autoStationControlAnswer.cStationID, autoStationControlRequest.cStationID, STATION_ID_LENGTH);
        autoStationControlAnswer.nVerify2 = autoStationControlRequest.nVerify2;
        autoStationControlAnswer.sNormalShoot = (short)atoi(chNormalInterval);
        autoStationControlAnswer.sAlertShoot = (short)atoi(chAlertInterval);

        // 应答电文送信
        nRet = m_pRecvSocket->Send((char*)(&autoStationControlAnswer), sizeof(autoStationControlAnswer));
        if(nRet == 0) {
            //m_//pSysLogger->Add(0,"<CWDSStationConfigProcess::Do()> Send status code time out");
        }
        if(nRet == -1) {
            //m_//pSysLogger->Add(0, "<CWDSStationConfigProcess::Do()> Send status code failed");
        }
    }

    m_pRecvSocket->Close();

    //m_//pSysLogger->Add(1,"<CWDSStationConfigProcess::Do()> WDSStationConfigProcess end!");
}

/******************************************************************************
    End
******************************************************************************/
