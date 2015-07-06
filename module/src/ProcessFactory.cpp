/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        日志文件处理机能                        */
/*           程序名        ：        ProcessFactory.cpp                      */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************
    用户Include
******************************************************************************/
#include "ProcessFactory.h"

CProcessFactory* CProcessFactory::m_pInstance = NULL;
CMutex CProcessFactory::m_sMutex;
/******************************************************************************
    处理名        ：  Process实例生成方法
    函数名        ：  GetInstance()
    参数          ：  无
    返回值        ：  Process实例
******************************************************************************/
IProcess* CProcessFactory::GetProcessModule(int32_t nModuleType){

    IProcess* pProcess = NULL;
    switch (nModuleType){
        /* 端口监控处理类                                                    */
        case PRC_TYPE_SOCKET_MONITOR:
            pProcess = (IProcess*)(new CSocketMonitorProcess());
            break;
        /* 端口监控处理类                                                    */
//        case PRC_TYPE_PACKET_ANALYSIS:
//            pProcess = (IProcess*)(new CPacketAnalysisProcess());
//            break;
/* 气象数据组成结构——小时数据                                              */
        case PRC_TYPE_ELEMENTS_HOUR_DATA:
            pProcess = (IProcess*)(new CWDSHourDataProcess());
            break;
/* 气象数据组成结构——分钟数据                                              */
        case PRC_TYPE_ELEMENTS_MINUTE_DATA:
            pProcess = (IProcess*)(new CWDSMinuteDataProcess());
            break;
/* 台站监控静态图像                                                          */
        case PRC_TYPE_STATION_IMAGE:
            pProcess = (IProcess*)(new CWDSStationImageProcess());
            break;
/* 实时重大天气数据                                                          */
        case PRC_TYPE_REALTIME_WEATHER_REPORT:
            pProcess = (IProcess*)(new CWDSWeatherReportProcess());
            break;
/* 台站设备注册                                                              */
        case PRC_TYPE_STATION_REGISTER:
            pProcess = (IProcess*)(new CWDSStationRegisterProcess());
            break;
/* 台站报警信息上传                                                          */
        case PRC_TYPE_STATION_ALERT:
            pProcess = (IProcess*)(new CWDSRealtimeAlertProcess());
            break;
/* 台站心跳信息上传                                                          */
        case PRC_TYPE_STATION_REALTIME_STATUS:
            pProcess = (IProcess*)(new CWDSRealtimeStatusProcess());
            break;
/* 5001控制信息请求                                                          */
        case PRC_TYPE_AUTO_STATION_CONTROL_REQUEST:
            pProcess = (IProcess*)(new CWDSStationConfigProcess());
            break;
/* 台站设备同步时间取得请求                                                  */
        case PRC_TYPE_SYNCTIME_REQUEST:
            pProcess = (IProcess*)(new CWDSSyncTimeProcess());
            break;
        default:
            break;
    }

    return pProcess;
}

/******************************************************************************
    处理名        ：  CProcessFactory实例生成方法
    函数名        ：  GetInstance()
    参数          ：  无
    返回值        ：  CProcessFactory对象实例
******************************************************************************/
CProcessFactory* CProcessFactory::GetInstance() {
    m_sMutex.Lock();
    if (NULL == m_pInstance) {
        m_pInstance = new CProcessFactory();
    }
    m_sMutex.UnLock();
    return m_pInstance;
}

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  CProcessFactory()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CProcessFactory::CProcessFactory() {
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~HTCPSocket()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CProcessFactory::~CProcessFactory() {
}
/******************************************************************************
    End
******************************************************************************/
