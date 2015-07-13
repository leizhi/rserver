/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        包解析业务处理类                        */
/*           程序名        ：        PacketAnalysisProcess.cpp               */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-04-02          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "PacketAnalysisProcess.h"
#include "ObjectFactory.h"
#include "ProcessFactory.h"
#include "VodEntity.h"

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  CPacketAnalysisProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CPacketAnalysisProcess::CPacketAnalysisProcess(HTCPSocket* pRecvSocket)
: m_pRecvSocket(pRecvSocket){
    m_pSendSocket          = NULL;
    m_nFuncType            = -1;
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~CPacketAnalysisProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CPacketAnalysisProcess::~CPacketAnalysisProcess(){

    if(m_pRecvSocket != NULL) {
    	printfs(1, " close m_pRecvSocket 0x%04x",m_pRecvSocket);
        m_pRecvSocket->Close();
        delete m_pRecvSocket;
        m_pRecvSocket = NULL;
    }
    
    if(m_pSendSocket != NULL) {
        printfs(1, " close m_pSendSocket 0x%04x",m_pSendSocket);
        m_pSendSocket->Close();
        delete m_pSendSocket;
        m_pSendSocket = NULL;
    }

    if(m_pProcess != NULL) {
        printfs(1, " close m_pSendSocket 0x%04x",m_pProcess);
        delete m_pProcess;
        m_pProcess = NULL;
    }
}

/******************************************************************************
    处理名        ：  业务处理
    函数名        ：  Do()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
void CPacketAnalysisProcess::Do(){
    printfs(1,"<CPacketAnalysisProcess::Do()> Analysis packet process start!");

    int nProcessType = 0;
    int nRet         = 0;
    stHeader         headerInfo;
    memset(&headerInfo, 0x00, sizeof(stHeader));

    /* Socket端口包解析                                                  */
    printfs(1, "m_pRecvSocket:0x%04x",m_pRecvSocket);

    nRet = m_pRecvSocket->Receive(&headerInfo, sizeof(stHeader));
    if(nRet == 0) {
        printfs(0,"<CPacketAnalysisProcess::Do()> Receive packet time out");
        m_pRecvSocket->Close();
        return;
    }

    if(nRet == -1) {
        printfs(0, "<CPacketAnalysisProcess::Do()> Receive packet failed");
        m_pRecvSocket->Close();
        return;
    }

    switch(headerInfo.nInfoType){
/* 气象数据组成结构——小时数据                                              */
        case INFO_TYPE_ELEMENTS_HOUR_DATA:
        {
            printfs(2, "<CPacketAnalysisProcess::Do()> Elements hour data!");
            nProcessType = PRC_TYPE_ELEMENTS_HOUR_DATA;
            break;
        }
/* 气象数据组成结构——分钟数据                                              */
        case INFO_TYPE_ELEMENTS_MINUTE_DATA:
        {
            printfs(2, "<CPacketAnalysisProcess::Do()> Elements minute data!");
            nProcessType = PRC_TYPE_ELEMENTS_MINUTE_DATA;
            break;
        }
/* 台站监控静态图像——月数据                                                */
/* 5001静态图像上传                                                          */
        case INFO_TYPE_STATION_MONTH_IMAGE:
        case INFO_TYPE_AUTO_STATION_IMAGE:
        {
            printfs(2, "<CPacketAnalysisProcess::Do()> Station image!");
            nProcessType = PRC_TYPE_STATION_IMAGE;
            break;
        }
/* 实时重大天气数据                                                          */
        case INFO_TYPE_REALTIME_WEATHER_REPORT:
        {
            printfs(2, "<CPacketAnalysisProcess::Do()> Realtime weather report!");
            nProcessType = PRC_TYPE_REALTIME_WEATHER_REPORT;
            break;
        }
/* RMS1001设备注册                                                           */
/* 5001设备注册                                                              */
        case INFO_TYPE_RMS1001_STATION_REGISTER:
        case INFO_TYPE_AUTO_STATION_REGISTER:
        {
            printfs(2, "<CPacketAnalysisProcess::Do()> Station register!");
            nProcessType = PRC_TYPE_STATION_REGISTER;
            break;
        }
/* RMS1001监控告警信息报告                                                   */
/* 5001报警信息上传                                                          */
        case INFO_TYPE_RMS1001_REALTIME_ALERT:
        case INFO_TYPE_AUTO_STATION_ALERT:
        {
            printfs(2, "<CPacketAnalysisProcess::Do()> Station alert!");
            nProcessType = PRC_TYPE_STATION_ALERT;
            break;
        }
/* RMS1001心跳信息上传                                                       */
/* 5001心跳信息上传                                                          */
        case INFO_TYPE_RMS1001_REALTIME_STATUS:
        case INFO_TYPE_AUTO_STATION_REALTIME_STATUS:
        {
            printfs(2, "<CPacketAnalysisProcess::Do()> Station realtime status!");
            nProcessType = PRC_TYPE_STATION_REALTIME_STATUS;
            break;
        }
/* 5001控制信息请求                                                          */
        case INFO_TYPE_AUTO_STATION_CONTROL_REQUEST:
        {
            printfs(2, "<CPacketAnalysisProcess::Do()> Auto-station control request!");
            nProcessType = PRC_TYPE_AUTO_STATION_CONTROL_REQUEST;
            break;
        }
/* 5001设备同步时间取得请求                                                  */
/* RMS1001设备同步时间取得请求                                               */
        case INFO_TYPE_AUTO_STATION_SYNCTIME_REQUEST:
        case INFO_TYPE_RMS1001_STATION_SYNCTIME_REQUEST:
        {
            printfs(2, "<CPacketAnalysisProcess::Do()> Station sync-time request!");
            nProcessType = PRC_TYPE_SYNCTIME_REQUEST;
            break;
        }
        default:
            printfs(1,"<CPacketAnalysisProcess::Do()> Undefined cInfoType! cInfoType:[%c]", headerInfo.nInfoType);
            break;
    }

    /* 取得对应的操作Process                                             */
    m_pProcess = CProcessFactory::GetInstance()->GetProcessModule(nProcessType);
    if (!m_pProcess){
        printfs(0,"<CPacketAnalysisProcess::Do()> No such process! processType=%d", nProcessType);
        return;
    }
    m_pProcess->SetRecvInfo(m_pRecvSocket, headerInfo.nInfoType);
    m_pProcess->Do();

    printfs(1,"<CPacketAnalysisProcess::Do()> analysis packet process end!");
}

/******************************************************************************
    End
******************************************************************************/
