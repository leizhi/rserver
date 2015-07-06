/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        WDS专用DB操作机能                       */
/*           程序名        ：        WDS_DBHelper.h                          */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#ifndef TDM_DBHELPER_H_
#define TDM_DBHELPER_H_

#include "MysqlHelper.h"
#include "VodEntity.h"
#include "log.h"
class WDS_DBHelper : public MysqlHelper
{
private:
    //CLog*    m_pSysLogger;

    int      IsUnknownTime(char* pTime, char* pTempTime);
public:
    WDS_DBHelper(std::string hosts, std::string userName, std::string password, 
                 std::string dbName, unsigned int port);
    WDS_DBHelper();
    ~WDS_DBHelper();

    void SetMysqlHandle(MYSQL* pMysql) {m_pSQLClient = pMysql;}
    MYSQL* GetMysqlHandle(){return m_pSQLClient;}

    /*
    对数据库进行增加记录的操作
    */
    // 插入一条5001实时监控报警信息到数据库
    int InsertRMS5001RealtimeAlert(stStationAlert* pInfo);

    // 插入一条1001实时监控报警信息到数据库
    int InsertRMS1001RealtimeAlert(stStationAlert* pInfo);

    // 插入一条实时监控报警信息到数据库
    int InsertRealtimeAlert(stStationAlert* pInfo, int nType, int *pStatus);

    // 插入一条台站实时心跳信息到数据库
    int InsertStationRealtimeStatus(stStationHeartBeat* pInfo);

    // 插入一条气象要素——小时数据信息到数据库
    int InsertElementsHourData(stElementsHourData* pInfo);

    // 插入一条气象要素——分钟数据信息到数据库
    int InsertElementsMinuteData(stElementsMinuteData* pInfo);

    // 插入一条实时重大天气数据信息到数据库
    int InsertRealtimeWeatherReport(stRealtimeWeatherReport* pInfo);

    // 插入一条台站监控静态图像信息到数据库
    int InsertStationImage(stStationImage* pInfo, long lId);

    // 保存静态图像资源信息到tb_gb_file_info表
    int InsertImageFile(char* pFilePath, char* pFileName, char* pFileOriginalName, char* pTime);

    /*
    对数据库进行查询记录的操作
    */
    // 根据指定SQL文从数据库获得分钟数据个数
    int GetElementsMinuteDataCount(char* chCondition, int& nCount);

    // 根据指定SQL文从数据库获得指定台站的报警个数
    int GetStationRealtimeAlertCount(char* chCondition, int nType, int& nCount);

    // 根据指定SQL文从数据库获得指定台站的最近一次报警状态
    int GetStationNealyAlertStatus(char* chCondition, int nType, int *pStatus);

    // 根据指定SQL文从数据库获得指定台站的实时心跳记录数量
    int GetStationRealtimeStatusCount(char* chCondition1, char* chCondition2, int& nCount);

    // 根据指定SQL文从数据库获得实时重大天气数据信息ID
    int GetMaxWeatherReportID(stRealtimeWeatherReport* pInfo, int& nId);

    // 根据指定SQL文从数据库获得tb_gb_file_info表中的指定图像的ID
    int GetImageFileID(char* pFileName, char* pFilePath, long& lId);

    /*
    对数据库进行更新记录的操作
    */
    // 更新台站心跳信息到数据库
    int UpdateStationRealtimeStatus(stStationHeartBeat* pInfo);

    // 更新一条气象要素——分钟数据信息到数据库
    int UpdateElementsMinuteData(stElementsMinuteData* pInfo);

    // 更新实时重大天气数据信息到数据库
    int UpdateRealtimeWeatherReport(stRealtimeWeatherReport* pInfo, int nId);

    /*
    对数据库进行删除记录的操作
    */
    int DeleteElementsHourData(stElementsHourData* pInfo);

    /*
    共通操作处理
    */
    // 通过取值范围确定CODE内容
    int GetDictCodebyRange(char* chCode, char* chType, int nValue);
    // 通过具体值确定CODE内容
    int GetDictCodebyValue(char* chCode, char* chType, int nValue);
    // 通过CODE确定具体值内容
    int GetSysConfigValuebyCode(char* chCode, char* chValue);

};

#endif /* TDM_DBHELPER_H_ */ 

/*****************************************************************************
    End
*****************************************************************************/
