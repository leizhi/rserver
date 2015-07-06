/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        ProcessFactory.h                        */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#if !defined(_CPROCESSFACTORY_H)
#define _CPROCESSFACTORY_H

/*****************************************************************************
    用户 include
******************************************************************************/
#include    "VodUserType.h"
#include    "Mutex.h"

#include    "PacketAnalysisProcess.h"
#include    "SocketMonitorProcess.h"
#include    "WDSHourDataProcess.h"
#include    "WDSMinuteDataProcess.h"
#include    "WDSWeatherReportProcess.h"
#include    "WDSSyncTimeProcess.h"
#include    "WDSStationRegisterProcess.h"
#include    "WDSStationConfigProcess.h"
#include    "WDSRealtimeStatusProcess.h"
#include    "WDSRealtimeAlertProcess.h"
#include    "WDSStationImageProcess.h"

/* Process类型定义                                                           */
/* 命令行处理类                                                              */
#define    PRC_TYPE_COMMAND                      1
/* 端口监控处理类                                                            */
#define    PRC_TYPE_SOCKET_MONITOR               2
/* 包解析处理类                                                              */
#define    PRC_TYPE_PACKET_ANALYSIS              3

/* 气象数据组成结构——小时数据                                              */
#define    PRC_TYPE_ELEMENTS_HOUR_DATA           4
/* 气象数据组成结构——分钟数据                                              */
#define    PRC_TYPE_ELEMENTS_MINUTE_DATA         5
/* 台站监控静态图像                                                          */
#define    PRC_TYPE_STATION_IMAGE                6
/* 实时重大天气数据                                                          */
#define    PRC_TYPE_REALTIME_WEATHER_REPORT      7
/* 台站设备注册                                                              */
#define    PRC_TYPE_STATION_REGISTER             8
/* 台站报警信息上传                                                          */
#define    PRC_TYPE_STATION_ALERT                9
/* 台站心跳信息上传                                                          */
#define    PRC_TYPE_STATION_REALTIME_STATUS      10
/* 5001控制信息请求                                                          */
#define    PRC_TYPE_AUTO_STATION_CONTROL_REQUEST 11
/* 台站设备同步时间取得请求                                                  */
#define    PRC_TYPE_SYNCTIME_REQUEST             12

class CProcessFactory {
public:
    IProcess*                     GetProcessModule(int32_t nModuleType);

    static CProcessFactory*       GetInstance();
    CProcessFactory();
    ~CProcessFactory();
private:
    static CProcessFactory*  m_pInstance;
    static CMutex            m_sMutex;
};

#endif  //_CPROCESSFACTORY_H

/*****************************************************************************
    End
******************************************************************************/
