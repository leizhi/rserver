/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        台站注册业务处理类                      */
/*           程序名        ：        WDSStationRegisterProcess.cpp           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "WDSStationRegisterProcess.h"
#include "WDS_DBHelper.h"
#include "ObjectFactory.h"
#include "SQLCondition.h"

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  CWDSStationRegisterProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSStationRegisterProcess::CWDSStationRegisterProcess(){
    //m_//pSysLogger           = NULL;
    m_pSendSocket          = NULL;
    m_pRecvSocket          = NULL;
    m_nFuncType            = -1;
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~CWDSStationRegisterProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSStationRegisterProcess::~CWDSStationRegisterProcess(){
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
void CWDSStationRegisterProcess::Do(){
    //m_//pSysLogger     = CObjectFactory::GetInstance()->GetSysLogger();
    //m_//pSysLogger->Add(1,"<CWDSStationRegisterProcess::Do()> WDSStationRegisterProcess start!");

    int nStatus = 0;
    int nRet = 0;
    char chSyncStep[DICT_VALUE_LENGTH + 1] = "\0";
    char chHeartStep[DICT_VALUE_LENGTH + 1] = "\0";
    // 取得小时气象信息的长度
    int nLength = sizeof(stStationRegister) - sizeof(stHeader);
    stStationRegister stationRegister;
    memset(&stationRegister,  0, sizeof(stationRegister));


    WDS_DBHelper* pWDSHelper = NULL;
    do{
        if (m_pRecvSocket == NULL){
            //m_//pSysLogger->Add(0,"<CWDSStationRegisterProcess::Do()> m_pRecvSocket == NULL");
            nStatus = 0;
            break;;
        }

        // 【业务处理】1. 取得端口传入的小时气象数据信息数据
        nRet = m_pRecvSocket->Receive((char*)(&stationRegister) + sizeof(stHeader), nLength);
        if(nRet == 0) {
            //m_//pSysLogger->Add(0,"<CWDSStationRegisterProcess::Do()> Receive packet time out");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        if(nRet == -1) {
            //m_//pSysLogger->Add(0, "<CWDSStationRegisterProcess::Do()> Receive packet failed");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        //m_//pSysLogger->Add(2, "<CWDSStationRegisterProcess::Do()> Receive struct info:cCurTime[%s], cStationID[%.10s]",
                         //    stationRegister.cCurTime,
                   //          stationRegister.cStationID);

        MYSQL* pMysqlConnection = CObjectFactory::GetInstance()->GetMySQLPool()->GetIdleMySql();
        if (pMysqlConnection == NULL){
            //m_//pSysLogger->Add(0,"<CWDSStationRegisterProcess::Do()> No enough mysql connections!");
            nStatus = 0;
            break;
        }
        pWDSHelper = new WDS_DBHelper();
        if (pWDSHelper == NULL){
            //m_//pSysLogger->Add(0,"<CWDSStationRegisterProcess::Do()> Can not connect to DB!");
            nStatus = 0;
            break;
        }
        pWDSHelper->SetMysqlHandle(pMysqlConnection);

        // step 1.业务处理，从DB取得当前台站的实时心跳记录数量
        int nCount = 0;
        nStatus = pWDSHelper->GetStationRealtimeStatusCount(stationRegister.cStationID, stationRegister.cCurTime, nCount);
        if (!nStatus){
            break;
        }
        stStationHeartBeat stationHeartBeat;
        memset(&stationHeartBeat,  0x00, sizeof(stationHeartBeat));

        memcpy(stationHeartBeat.cStationID, stationRegister.cStationID, STATION_ID_LENGTH);
        memcpy(stationHeartBeat.cCurTime, stationRegister.cCurTime, DATE_TIME_LENGTH);
        if (nCount != 0){
            // step 2.1业务处理，向DB更新当前台站的心跳信息
            nStatus = pWDSHelper->UpdateStationRealtimeStatus(&stationHeartBeat);
        }
        else{
            // step 2.2业务处理，向DB插入当前台站的心跳信息
            nStatus = pWDSHelper->InsertStationRealtimeStatus(&stationHeartBeat);
        }
        if (!nStatus){
            //m_//pSysLogger->Add(0,"<CWDSStationRegisterProcess::Do()> Station realtime status operation failed!");
            break;
        }

        // 业务处理，取得系统配置表中的同步时间间隔、心跳时间间隔
        nStatus = pWDSHelper->GetSysConfigValuebyCode(SYSTEM_CONFIG_SYNC_STEP, chSyncStep);
        if (!nStatus){
            //m_//pSysLogger->Add(0,"<CWDSStationRegisterProcess::Do()> GetSysConfigValuebyCode-SyncStep operation failed!");
            break;
        }
        nStatus = pWDSHelper->GetSysConfigValuebyCode(SYSTEM_CONFIG_HEART_STEP, chHeartStep);
        if (!nStatus){
            //m_//pSysLogger->Add(0,"<CWDSStationRegisterProcess::Do()> GetSysConfigValuebyCode-HeartStep operation failed!");
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
    answerInfo.nVerify1 = stationRegister.nVerify1;
    answerInfo.nStatus = nStatus;
    answerInfo.nVerify2 = stationRegister.nVerify2;

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
        // 业务处理 取得当前系统本地时间
        struct tm *pTime;
        time_t nowTime;
        time(&nowTime);
        pTime = gmtime(&nowTime);

        stStationRegisterAnswer stationRegisterAnswer;
        memset(&stationRegisterAnswer, 0, sizeof(stationRegisterAnswer));
        stationRegisterAnswer.nVerify1 = stationRegister.nVerify1;
        // 设置当前时间
        snprintf(stationRegisterAnswer.cCurTime, DATE_TIME_LENGTH, "%04d-%02d-%02d %02d:%02d:%02d",
            (1900+pTime->tm_year), (1+pTime->tm_mon), pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
        stationRegisterAnswer.nSyncStep = atoi(chSyncStep);
        stationRegisterAnswer.nHeartStep = atoi(chHeartStep);
        stationRegisterAnswer.nVerify2 = stationRegister.nVerify2;

        // 应答电文送信
        nRet = m_pRecvSocket->Send((char*)(&stationRegisterAnswer), sizeof(stationRegisterAnswer));
        if(nRet == 0) {
            //m_//pSysLogger->Add(0,"<CWDSStationRegisterProcess::Do()> Send status code time out");
        }
        if(nRet == -1) {
            //m_//pSysLogger->Add(0, "<CWDSStationRegisterProcess::Do()> Send status code failed");
        }
    }

    m_pRecvSocket->Close();

    //m_//pSysLogger->Add(1,"<CWDSStationRegisterProcess::Do()> WDSStationRegisterProcess end!");
}

/******************************************************************************
    End
******************************************************************************/
