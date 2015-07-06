/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        VodEntity.h                             */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "VodUserType.h"
#include "VodErrorMessage.h"

#ifndef _VODENTITY_H
#define _VODENTITY_H

#define DIRECTION_N "N"
#define DIRECTION_NNE "NNE"
#define DIRECTION_NE "NE"
#define DIRECTION_ENE "ENE"
#define DIRECTION_E "E"
#define DIRECTION_ESE "ESE"
#define DIRECTION_SE "SE"
#define DIRECTION_SSE "SSE"
#define DIRECTION_S "S"
#define DIRECTION_SSW "SSW"
#define DIRECTION_SW "SW"
#define DIRECTION_WSW "WSW"
#define DIRECTION_W "W"
#define DIRECTION_WNW "WNW"
#define DIRECTION_NW "NW"
#define DIRECTION_NNW "NNW"


/* IP地址长度                                                                */
#define IPADDRESS_LENGTH         40
/* 时间格式长度                                                              */
#define DATE_TIME_LENGTH         20
/* 最大文件名长度                                                            */
#define MAX_FILEPATH_LENGTH      256
/* 站点编号长度                                                              */
#define STATION_ID_LENGTH        10
/* 方向格式长度                                                              */
#define DIRECTION_LENGTH         4
/* 数据字典表CODE宽度                                                        */
#define DICT_CODE_LENGTH         6
/* 数据字典表TYPE宽度                                                        */
#define DICT_TYPE_LENGTH         100
/* 数据字典表VALUE宽度                                                       */
#define DICT_VALUE_LENGTH        255

/* 特殊值定义                                                                */
/* 无效的能见度                                                              */
#define S_VISIBILITY             21755
/* 无效的开始/结束时间                                                       */
#define S_GRAY_TIME              77:88:00
/* 不明确的情况下显示时间值                                                  */
#define S_DISABLED_TIME          00:00:00

/* 情报识别Code定义                                                          */
/* 气象数据组成结构——小时数据                                              */
#define INFO_TYPE_ELEMENTS_HOUR_DATA               1
/* 气象数据组成结构——分钟数据                                              */
#define INFO_TYPE_ELEMENTS_MINUTE_DATA             2
/* 台站监控静态图像——月数据                                                */
#define INFO_TYPE_STATION_MONTH_IMAGE              3
/* 实时重大天气数据                                                          */
#define INFO_TYPE_REALTIME_WEATHER_REPORT          4
/* RMS1001设备注册                                                           */
#define INFO_TYPE_RMS1001_STATION_REGISTER         8
/* RMS1001监控告警信息报告                                                   */
#define INFO_TYPE_RMS1001_REALTIME_ALERT           9
/* RMS1001心跳信息上传                                                       */
#define INFO_TYPE_RMS1001_REALTIME_STATUS          10
/* 5001设备注册                                                              */
#define INFO_TYPE_AUTO_STATION_REGISTER            11
/* 5001控制信息请求                                                          */
#define INFO_TYPE_AUTO_STATION_CONTROL_REQUEST     12
/* 5001控制信息应答                                                          */
#define INFO_TYPE_AUTO_STATION_CONTROL_ANSWER      13
/* 5001静态图像上传                                                          */
#define INFO_TYPE_AUTO_STATION_IMAGE               14
/* 5001报警信息上传                                                          */
#define INFO_TYPE_AUTO_STATION_ALERT               15
/* 5001心跳信息上传                                                          */
#define INFO_TYPE_AUTO_STATION_REALTIME_STATUS     16
/* 5001设备注册应答                                                          */
#define INFO_TYPE_AUTO_STATION_REGISTER_ANSWER     17
/* 5001设备同步时间取得请求                                                  */
#define INFO_TYPE_AUTO_STATION_SYNCTIME_REQUEST    18
/* 5001设备同步时间取得应答                                                  */
#define INFO_TYPE_AUTO_STATION_SYNCTIME_ANSWER     19
/* RMS1001设备注册应答                                                       */
#define INFO_TYPE_RMS1001_STATION_REGISTER_ANSWER  20
/* RMS1001设备同步时间取得请求                                               */
#define INFO_TYPE_RMS1001_STATION_SYNCTIME_REQUEST 21
/* RMS1001设备同步时间取得应答                                               */
#define INFO_TYPE_RMS1001_STATION_SYNCTIME_ANSWER  22

/* 电文头的电文格式                                                          */
typedef struct {
    int    nVerify1;     // 验证码，用于判断数据是否正确，暂定为0x23456789
    int    nLength;      // Header部+Data部的总长度
    int    nInfoType;    // 参见系统通讯设计说明书.doc
    int    nVerify2;     // 验证码，用于判断数据是否正确，暂定为0x23456789
}stHeader;

/* 通用应答的电文格式                                                        */
typedef struct {
    int    nVerify1;    // 验证码，用于判断数据是否正确，暂定为0x23456789
    int    nStatus;     // 0：处理失败；1：处理成功
    int    nVerify2;    // 验证码，用于判断数据是否正确，暂定为0x23456789
}stAnswer;

/* 台站设备注册                                                              */
typedef struct {
    stHeader header;       // 电文头

    int     nVerify1;      // 验证码，用于判断数据是否正确，暂定为0x23456789
    char    cCurTime[DATE_TIME_LENGTH];
                           // 当前时间
    char    cStationID[STATION_ID_LENGTH];
                           // 站点编号
    char    cRev1[2];      // 预留位1
    int     nVerify2;      // 验证码，用于判断数据是否正确，暂定为0x23456789
}stStationRegister;

/* 台站设备注册应答                                                          */
typedef struct {
//    stHeader header;       // 电文头

    int     nVerify1;      // 验证码，用于判断数据是否正确，暂定为0x23456789
    char    cCurTime[DATE_TIME_LENGTH];
                           // 当前时间
    int     nSyncStep;     // 同步时间间隔(单位：分钟)(当同步时间设置为0时，
                           // 不进行自动的同步处理(范围：60～1440))
    int     nHeartStep;    // 心跳时间间隔(单位：秒)
    int     nVerify2;      // 验证码，用于判断数据是否正确，暂定为0x23456789
}stStationRegisterAnswer;

/* 台站监控报警信息报告                                                      */
typedef struct {
    stHeader header;       // 电文头

    int    nVerify1;       // 验证码，用于判断数据是否正确，暂定为0x23456789
    char   cCurTime[DATE_TIME_LENGTH];
                           // 当前时间
    char   cStationID[STATION_ID_LENGTH];
                           // 站点编号
    char   cRev1[2];       // 预留位1
    char   cAlert[12];     // 报警实时状态
    int    nVerify2;       // 验证码，用于判断数据是否正确，暂定为0x23456789
}stStationAlert;

/* 台站心跳信息上传                                                          */
typedef struct {
    stHeader header;        // 电文头

    int    nVerify1;        // 验证码，用于判断数据是否正确，暂定为0x23456789
    char   cCurTime[DATE_TIME_LENGTH];
                            // 当前时间
    char   cStationID[STATION_ID_LENGTH];
                            // 站点编号
    char   cChannelStatus;  // 通道视频(0X00000000)(0:无视频；1:有视频)
    char   cRev1;           // 预留位1
    int    nHDstatus;       // 硬盘状态(0:无硬盘；1:有硬盘未格式化；2：有硬盘已格式化)
    int    nHDCapacity;     // GByte单位
    int    nFreeSpace;      // MByte单位
    int    nVerify2;        // 验证码，用于判断数据是否正确，暂定为0x23456789
}stStationHeartBeat;

/* 台站系统同步时间取得请求                                                  */
typedef struct {
    stHeader header;        // 电文头

    int     nVerify1;       // 验证码，用于判断数据是否正确，暂定为0x23456789
    char    cCurTime[DATE_TIME_LENGTH];
                            // 当前时间
    char    cStationID[STATION_ID_LENGTH];
                            // 站点编号
    char    cRev1[2];       // 预留位1
    int     nVerify2;       // 验证码，用于判断数据是否正确，暂定为0x23456789
}stSyncTimeRequest;

/* 台站系统同步时间下发                                                      */
typedef struct {
//    stHeader header;        // 电文头

    int     nVerify1;       // 验证码，用于判断数据是否正确，暂定为0x23456789
    char    cCurTime[DATE_TIME_LENGTH];
                            // 当前时间
    int     nVerify2;       // 验证码，用于判断数据是否正确，暂定为0x23456789
}stSyncTimeAnswer;

/* 5001控制信息请求[INFO_TYPE_AUTO_STATION_CONTROL_REQUEST]                  */
typedef struct {
    stHeader header;       // 电文头

    int     nVerify1;      // 验证码，用于判断数据是否正确，暂定为0x23456789
    char    cCurTime[DATE_TIME_LENGTH];
                           // 当前时间
    char    cStationID[STATION_ID_LENGTH];
                           // 站点编号
    char    cRev1[2];      // 预留位1
    int     nVerify2;      // 验证码，用于判断数据是否正确，暂定为0x23456789
}stAutoStationControlRequest;

/* 5001控制信息应答[INFO_TYPE_AUTO_STATION_CONTROL_ANSWER]                   */
typedef struct {
//    stHeader header;       // 电文头

    int     nVerify1;      // 验证码，用于判断数据是否正确，暂定为0x23456789
    char    cCurTime[DATE_TIME_LENGTH];
                           // 当前时间
    char    cStationID[STATION_ID_LENGTH];
                           // 站点编号
    char    cRev1[2];      // 预留位1
    short   sNormalShoot;  // 正常抓拍时间间隔(单位:秒)
    short   sAlertShoot;   // 报警抓拍时间间隔(单位:秒)
    int     nVerify2;      // 验证码，用于判断数据是否正确，暂定为0x23456789
}stAutoStationControlAnswer;

/* 气象数据组成结构——小时数据[INFO_TYPE_ELEMENTS_HOUR_DATA]                */
typedef struct {
    stHeader header;       // 电文头

    int     nVerify1;       // 验证码，用于判断数据是否正确，暂定为0x23456789
    char    cCurTime[DATE_TIME_LENGTH];
                            // 当前时间
    char    cStationID[STATION_ID_LENGTH];
                            // 站点编号
    char    cRev1[2];       // 预留位1
    float   fCurTemp;       // 本小时正点温度(单位:℃)
    float   fMaxTemp;       // 本小时最高温度(单位:℃)
    float   fMinTemp;       // 本小时最低温度(单位:℃)
    float   fRainfall;      // 雨量(单位:mm)
    float   fCurAP;         // 本小时正点气压(单位:HPa)
    float   fMaxAP;         // 本小时最高气压(单位:HPa)
    float   fMinAP;         // 本小时最低气压(单位:HPa)
    float   fWindVelocity;  // 风速(单位:m/s)
    char    cWindDirection[DIRECTION_LENGTH];
                            // 风向(使用16方向标示，如南南西：NNW)
    float   fHumidity;      // 湿度(单位:%)
    int     nVerify2;       // 验证码，用于判断数据是否正确，暂定为0x23456789
}stElementsHourData;

/* 气象数据组成结构——分钟数据[INFO_TYPE_ELEMENTS_MINUTE_DATA]              */
typedef struct {
    stHeader header;       // 电文头

    int     nVerify1;       // 验证码，用于判断数据是否正确，暂定为0x23456789
    char    cCurTime[DATE_TIME_LENGTH];
                            // 当前时间
    char    cStationID[STATION_ID_LENGTH];
                            // 站点编号
    char    cRev1[2];       // 预留位1
    float   fCurTemp;       // 气温(单位:℃)
    float   fRainfall;      // 雨量(单位:mm)
    char    cWindDirection[DIRECTION_LENGTH];
                            // 风向(使用16方向标示，如南南西：NNW)
    float   fWindVelocity;  // 风速(单位:m/s)
    float   fCurAP;         // 气压(单位:HPa)
    float   fHumidity;      // 湿度(单位:%)
    int     nVerify2;       // 验证码，用于判断数据是否正确，暂定为0x23456789
}stElementsMinuteData;

/* 台站监控静态图像                                                          */
typedef struct {
    stHeader header;       // 电文头

    int     nVerify1;      // 验证码，用于判断数据是否正确，暂定为0x23456789
    char    cCurTime[DATE_TIME_LENGTH];
                           // 当前时间
    char    cStationID[STATION_ID_LENGTH];
                           // 站点编号
    char    cRev1[2];      // 预留位1
    char    cDirection[DIRECTION_LENGTH];
                           // 方向(使用16方向标示，如南南西：NNW)
    int     nPicSize;      // 图像大小
    int     nVerify2;      // 验证码，用于判断数据是否正确，暂定为0x23456789
}stStationImage;

#define WEATHER_REPORT_BEGIN 0
#define WEATHER_REPORT_STOP 1

#define WEATHER_STATUS_DUST 3
#define WEATHER_STATUS_FOG  4

/* 实时重大天气数据[INFO_TYPE_REALTIME_WEATHER_REPORT]                       */
typedef struct {
    stHeader header;      // 电文头

    int     nVerify1;      // 验证码，用于判断数据是否正确，暂定为0x23456789
    char    cCurTime[DATE_TIME_LENGTH];
                           // 当前时间
    char    cStationID[STATION_ID_LENGTH];
                           // 站点编号
    char    cRev1[2];      // 预留位1
    char    cTime[DATE_TIME_LENGTH];
                           // 开始/结束时间
    int     nFlag;         // 0：开始；1：结束
    int     nVisibility;   // 能见度(单位:米)(当事件为沙尘、雾时，此字段有效)
    int     nEventNo;      // 事件编号(0:雨、1:雪、2:雷暴、3:沙尘、4:雾、5:冰雹、6:霾、7:浮尘)
    char    cEventComment[64]; // 事件描述
    int     nVerify2;      // 验证码，用于判断数据是否正确，暂定为0x23456789
}stRealtimeWeatherReport;

#endif // _VODENTITY_H

/*****************************************************************************
    End
*****************************************************************************/
