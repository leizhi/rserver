/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        时间同步业务处理类                      */
/*           程序名        ：        WDSSyncTimeProcess.cpp                  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "WDSSyncTimeProcess.h"
#include "WDS_DBHelper.h"
#include "ObjectFactory.h"

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  CWDSSyncTimeProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSSyncTimeProcess::CWDSSyncTimeProcess(){
    //m_//pSysLogger           = NULL;
    m_pSendSocket          = NULL;
    m_pRecvSocket          = NULL;
    m_nFuncType            = -1;
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~CWDSSyncTimeProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSSyncTimeProcess::~CWDSSyncTimeProcess(){
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
void CWDSSyncTimeProcess::Do(){
    //m_//pSysLogger     = CObjectFactory::GetInstance()->GetSysLogger();
    //m_//pSysLogger->Add(1,"<CWDSSyncTimeProcess::Do()> WDSSyncTimeProcess start!");

    int nRet = 0;
    int nStatus = 0;
    // 取得设备同步信息包的长度
    int nLength = sizeof(stSyncTimeRequest) - sizeof(stHeader);
    stSyncTimeRequest syncTimeRequest;
    memset(&syncTimeRequest,  0, sizeof(syncTimeRequest));

    do{
        if (m_pRecvSocket == NULL){
            //m_//pSysLogger->Add(0,"<CWDSSyncTimeProcess::Do()> m_pRecvSocket == NULL");
            break;;
        }

        // 【业务处理】1. 取得端口传入的小时气象数据信息数据
        nRet = m_pRecvSocket->Receive((char*)(&syncTimeRequest) + sizeof(stHeader), nLength);
        if(nRet == 0) {
            //m_//pSysLogger->Add(0,"<CWDSSyncTimeProcess::Do()> Receive packet time out");
            m_pRecvSocket->Close();
            break;
        }
        if(nRet == -1) {
            //m_//pSysLogger->Add(0, "<CWDSSyncTimeProcess::Do()> Receive packet failed");
            m_pRecvSocket->Close();
            break;
        }
        //m_//pSysLogger->Add(2, "<CWDSSyncTimeProcess::Do()> Receive struct info:cCurTime[%s], cStationID[%.10s]",
         //                    syncTimeRequest.cCurTime,
         //                    syncTimeRequest.cStationID);

        nStatus = 1;
    }
    while(0);

    // 发送标准应答
    stAnswer answerInfo;
    memset(&answerInfo, 0, sizeof(answerInfo));
    answerInfo.nVerify1 = syncTimeRequest.nVerify1;
    answerInfo.nStatus = nStatus;
    answerInfo.nVerify2 = syncTimeRequest.nVerify2;

    // 应答电文送信
    nRet = m_pRecvSocket->Send((char*)(&answerInfo), sizeof(answerInfo));
    if(nRet == 0) {
        //m_//pSysLogger->Add(0,"<CWDSStationRegisterProcess::Do()> Send status code time out");
    }
    if(nRet == -1) {
        //m_//pSysLogger->Add(0, "<CWDSStationRegisterProcess::Do()> Send status code failed");
    }

    // 发送同步时间应答
    if(nStatus) {
        // 业务处理 取得当前系统本地时间
        struct tm *pTime;
        time_t nowTime;
        time(&nowTime);
        pTime = localtime(&nowTime);

        stSyncTimeAnswer syncTimeAnswer;
        memset(&syncTimeAnswer, 0, sizeof(syncTimeAnswer));
        // 设置当前时间
        snprintf(syncTimeAnswer.cCurTime, DATE_TIME_LENGTH, "%04d-%02d-%02d %02d:%02d:%02d",
            (1900+pTime->tm_year), (1+pTime->tm_mon), pTime->tm_mday, pTime->tm_hour, pTime->tm_min, pTime->tm_sec);
        syncTimeAnswer.nVerify1 = syncTimeRequest.nVerify1;
        syncTimeAnswer.nVerify2 = syncTimeRequest.nVerify2;

        // 应答电文送信
        nRet = m_pRecvSocket->Send((char*)(&syncTimeAnswer), sizeof(syncTimeAnswer));
        if(nRet == 0) {
            //m_//pSysLogger->Add(0,"<CWDSStationRegisterProcess::Do()> Send status code time out");
        }
        if(nRet == -1) {
            //m_//pSysLogger->Add(0, "<CWDSStationRegisterProcess::Do()> Send status code failed");
        }
    }

    m_pRecvSocket->Close();

    //m_//pSysLogger->Add(1,"<CWDSSyncTimeProcess::Do()> WDSSyncTimeProcess end!");
}

/******************************************************************************
    End
******************************************************************************/
