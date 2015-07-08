/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        WDS专用数据库处理类                     */
/*           程序名        ：        WDS_DBHelper.cpp                        */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "WDS_DBHelper.h"
#include "Common.h"
#include "SQLCondition.h"
#include "ObjectFactory.h"

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  MysqlHelper(string hosts, string userName, 
                                  string password, string dbName, 
                                  unsigned int port)
    参数          ：  * @param hosts:         Host IP address
                      * @param userName:      Login UserName
                      * @param password:      Login Password
                      * @param dbName:        Database Name
                      * @param port:          Host listen port number
    返回值        ：  无
******************************************************************************/
WDS_DBHelper::WDS_DBHelper(std::string hosts, std::string userName,
                           std::string password, std::string dbName, unsigned int port)
{
    IsConnected = false;
    this->setHosts(hosts);        // 设置主机IP地址
    this->setUserName(userName);  // 设置登录用户名
    this->setPassword(password);  // 设置登录密码
    this->setDBName(dbName);      // 设置数据库名
    this->setPort(port);          // 设置端口号
    m_bCreateByHandle = false;
}

WDS_DBHelper::WDS_DBHelper()
{
    m_bCreateByHandle = true;
    IsConnected       = true;
}
/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~WDS_DBHelper()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
WDS_DBHelper::~WDS_DBHelper(void)
{
    if (!m_bCreateByHandle)
        this ->destroyConnection();
}

/*
对数据库进行增加记录的操作
*/
/******************************************************************************
    处理名        ：  累加已完成通话数据到未出帐消费记录到数据库WDS-DB数据库
    函数名        ：  InsertCompWDSData()
    参数          ：  pInfo 已完成通话数据
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
/*
int WDS_DBHelper::InsertTDQuality(st_BillingInfo* pInfo, int& out_STATUS)
{
    printfs(1,"<WDS_DBHelper::InsertTDQuality()> Begin call InsertTDQuality()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }
    // 构建调用存储过程所需参数字符串
    char chCondition[sizeof(st_BillingInfo) + strlen(SQL_PROC_PARA_TRADE_QUALITY)] = "\0";

    sprintf(chCondition,
            SQL_PROC_PARA_TRADE_QUALITY,
            pInfo->chTradeID,
            pInfo->chCode,
            pInfo->nFinishCDR,
            pInfo->nFinishTime,
            pInfo->chFinishSrcIP,
            pInfo->nFinishValidTimeOmg,
            pInfo->dFinishCostOmg,
            pInfo->chFinishDstIP,
            pInfo->nFinishValidTimeTmg,
            pInfo->dFinishCostTmg,
            pInfo->nConnectedCDR,
            pInfo->nStartedCDR,
            pInfo->nPddTime);

    // 取得完整的调用存储过程字符串
    char chCallSPText[sizeof(st_BillingInfo) + strlen(SQL_PROC_PARA_TRADE_QUALITY) + strlen(CALL_PROC_SUBMIT_TRADE_QUALITY)] = "\0";
    sprintf(chCallSPText, CALL_PROC_SUBMIT_TRADE_QUALITY, chCondition);
    string sCallSPText = chCallSPText;
    printfs(2,"<WDS_DBHelper::InsertTDQuality()> call sql: %s", chCallSPText);

    // 执行调用存储过程函数
    unsigned int nRet = 0;
    nRet = mysql_real_query(&mySQLClient,sCallSPText.c_str(),
                         (unsigned int)strlen(sCallSPText.c_str()));// 执行查询
        mysql_query(&mySQLClient, SQL_QUERY_CALL_OUTPUT);
    if ( nRet ) {
        printfs(0,"<WDS_DBHelper::InsertTDQuality()> Error query from database: %s",
                          mysql_error(&mySQLClient));
        return 0;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(&mySQLClient);

    while( (row = mysql_fetch_row(res)) )
    {// 遍历结果集
        out_STATUS = 0;
        try{
            out_STATUS    = atoi(row[0]);
        }
        catch(...){
            printfs(0,"<WDS_DBHelper::GetCallTrackInfo()> Data error! ID: %d", atoi(row[0]));
            continue;
        }
    }
    mysql_free_result(res); //free result after you get the result

    printfs(1,"<WDS_DBHelper::InsertTDQuality()> Stop call InsertTDQuality()");
    return 1;
}
*/

/******************************************************************************
    处理名        ：  插入一条5001实时监控报警信息到数据库
    函数名        ：  InsertRMS5001RealtimeAlert()
    参数          ：  pInfo 5001实时监控报警信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::InsertRMS5001RealtimeAlert(stStationAlert* pInfo)
{
    printfs(1,"<WDS_DBHelper::InsertRMS5001RealtimeAlert()> Begin call InsertRMS5001RealtimeAlert()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }

    printfs(1,"<WDS_DBHelper::InsertRMS5001RealtimeAlert()> Stop call InsertRMS5001RealtimeAlert()");
    return 1;
}

/******************************************************************************
    处理名        ：  插入一条1001实时监控报警信息到数据库
    函数名        ：  InsertRMS1001RealtimeAlert()
    参数          ：  pInfo 1001实时监控报警信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::InsertRMS1001RealtimeAlert(stStationAlert* pInfo)
{
    printfs(1,"<WDS_DBHelper::InsertRMS1001RealtimeAlert()> Begin call InsertRMS1001RealtimeAlert()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }

    printfs(1,"<WDS_DBHelper::InsertRMS1001RealtimeAlert()> Stop call InsertRMS1001RealtimeAlert()");
    return 1;
}

/******************************************************************************
    处理名        ：  插入一条实时监控报警信息到数据库
    函数名        ：  InsertRMSRealtimeAlert()
    参数          ：  pInfo 实时监控报警信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::InsertRealtimeAlert(stStationAlert* pInfo, int nType, int *pStatus)
{
    printfs(1,"<WDS_DBHelper::InsertRMSRealtimeAlert()> Begin call InsertRMSRealtimeAlert()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }

    // 构建调用SQL语句所需参数字符串
    char chCondition[sizeof(stStationHeartBeat) + strlen(SQL_INSERT_1001_REALTIME_ALERT) + 8*sizeof(int) + 1] = "\0";
    if (nType == INFO_TYPE_RMS1001_REALTIME_ALERT){
        sprintf(chCondition,
                SQL_INSERT_1001_REALTIME_ALERT,
                pInfo->cStationID,
                pInfo->cCurTime,
                CheckRealtimeStatus(pInfo->cAlert[8], 0x80),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x40),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x20),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x10),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x08),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x04),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x02),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x01),
                IsAlertChanged(pInfo->cAlert[8], 0x80, pStatus[0]),
                IsAlertChanged(pInfo->cAlert[8], 0x40, pStatus[1]),
                IsAlertChanged(pInfo->cAlert[8], 0x20, pStatus[2]),
                IsAlertChanged(pInfo->cAlert[8], 0x10, pStatus[3]),
                IsAlertChanged(pInfo->cAlert[8], 0x08, pStatus[4]),
                IsAlertChanged(pInfo->cAlert[8], 0x04, pStatus[5]),
                IsAlertChanged(pInfo->cAlert[8], 0x02, pStatus[6]),
                IsAlertChanged(pInfo->cAlert[8], 0x01, pStatus[7]));
    } else {
        sprintf(chCondition,
                SQL_INSERT_5001_REALTIME_ALERT,
                pInfo->cStationID,
                pInfo->cCurTime,
                CheckRealtimeStatus(pInfo->cAlert[8], 0x80),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x40),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x20),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x10),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x08),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x04),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x02),
                CheckRealtimeStatus(pInfo->cAlert[8], 0x01),
                IsAlertChanged(pInfo->cAlert[8], 0x80, pStatus[0]),
                IsAlertChanged(pInfo->cAlert[8], 0x40, pStatus[1]),
                IsAlertChanged(pInfo->cAlert[8], 0x20, pStatus[2]),
                IsAlertChanged(pInfo->cAlert[8], 0x10, pStatus[3]),
                IsAlertChanged(pInfo->cAlert[8], 0x08, pStatus[4]),
                IsAlertChanged(pInfo->cAlert[8], 0x04, pStatus[5]),
                IsAlertChanged(pInfo->cAlert[8], 0x02, pStatus[6]),
                IsAlertChanged(pInfo->cAlert[8], 0x01, pStatus[7]));
    }

    string sSqlText = chCondition;
    printfs(2,"<WDS_DBHelper::InsertRMSRealtimeAlert()> Insert sql: %s", chCondition);
    // 执行调用SQL函数
    bool bRet = runSQLCommand(sSqlText);
    if (!bRet){
        printfs(0,"<WDS_DBHelper::InsertRMSRealtimeAlert()> Insert failed, sql: %s", chCondition);
    }

    printfs(1,"<WDS_DBHelper::InsertRMSRealtimeAlert()> Stop call InsertRMSRealtimeAlert()");
    return bRet;
}

/******************************************************************************
    处理名        ：  插入一条台站实时心跳信息到数据库
    函数名        ：  InsertStationRealtimeStatus()
    参数          ：  pInfo 台站实时心跳信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::InsertStationRealtimeStatus(stStationHeartBeat* pInfo)
{
    printfs(1,"<WDS_DBHelper::InsertStationRealtimeStatus()> Begin call InsertStationRealtimeStatus()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }

    // 取得硬盘状态字典CODE
    char chHDStatus[DICT_CODE_LENGTH + 1] = "\0";
    if(!GetDictCodebyValue(chHDStatus, DATA_DICT_HARD_DISK_STATUS, pInfo->nHDstatus)){
        printfs(0,"<WDS_DBHelper::InsertStationRealtimeStatus()> No dict code found! HD type:%d", pInfo->nHDstatus);
        return false;
    }

    // 取得测场视频状态字典CODE
    char chGeoFieldStatus[DICT_CODE_LENGTH + 1] = "\0";
    if(!GetDictCodebyValue(chGeoFieldStatus, DATA_DICT_VIDEO_STATUS, CheckRealtimeStatus(pInfo->cChannelStatus, 0x80))){
        printfs(0,"<WDS_DBHelper::InsertStationRealtimeStatus()> No dict code found! cChannelStatus:%x", pInfo->cChannelStatus);
        return false;
    }

    // 取得值班室视频状态字典CODE
    char chOfficeStatus[DICT_CODE_LENGTH + 1] = "\0";
    if(!GetDictCodebyValue(chOfficeStatus, DATA_DICT_VIDEO_STATUS, CheckRealtimeStatus(pInfo->cChannelStatus, 0x40))){
        printfs(0,"<WDS_DBHelper::InsertStationRealtimeStatus()> No dict code found! cChannelStatus:%x", pInfo->cChannelStatus);
        return false;
    }

    // 取得制氢室视频状态字典CODE
    char chHydrogenStatus[DICT_CODE_LENGTH + 1] = "\0";
    if(!GetDictCodebyValue(chHydrogenStatus, DATA_DICT_VIDEO_STATUS, CheckRealtimeStatus(pInfo->cChannelStatus, 0x20))){
        printfs(0,"<WDS_DBHelper::InsertStationRealtimeStatus()> No dict code found! cChannelStatus:%x", pInfo->cChannelStatus);
        return false;
    }

    // 构建调用SQL语句所需参数字符串
    char chCondition[sizeof(stStationHeartBeat) + strlen(SQL_INSERT_STATION_REALTIME_STATUS) + 3 * DICT_CODE_LENGTH + 1] = "\0";
    sprintf(chCondition,
            SQL_INSERT_STATION_REALTIME_STATUS,
            pInfo->cStationID,
            pInfo->cCurTime,
            chGeoFieldStatus,
            chOfficeStatus,
            chHydrogenStatus,
            chHDStatus,
            pInfo->nHDCapacity,
            pInfo->nFreeSpace);

    string sSqlText = chCondition;
    printfs(2,"<WDS_DBHelper::InsertStationRealtimeStatus()> Insert sql: %s", chCondition);
    // 执行调用SQL函数
    bool bRet = runSQLCommand(sSqlText);
    if (!bRet){
        printfs(0,"<WDS_DBHelper::InsertStationRealtimeStatus()> Insert failed, sql: %s", chCondition);
    }

    printfs(1,"<WDS_DBHelper::InsertStationRealtimeStatus()> Stop call InsertStationRealtimeStatus()");
    return bRet;
}

/******************************************************************************
    处理名        ：  插入一条气象要素——小时数据信息到数据库
    函数名        ：  InsertElementsHourData()
    参数          ：  pInfo 气象要素——小时数据信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::InsertElementsHourData(stElementsHourData* pInfo)
{
    printfs(1,"<WDS_DBHelper::InsertElementsHourData()> Begin call InsertElementsHourData()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }

    // 构建调用SQL语句所需参数字符串
    char chCondition[sizeof(stElementsHourData) + strlen(SQL_INSERT_ELEMENTS_HOUR_DATA) + 1] = "\0";
    sprintf(chCondition,
            SQL_INSERT_ELEMENTS_HOUR_DATA,
            pInfo->cStationID,
            pInfo->cCurTime,
            pInfo->fCurTemp,
            pInfo->fMaxTemp,
            pInfo->fMinTemp,
            pInfo->fRainfall,
            pInfo->fCurAP,
            pInfo->fMaxAP,
            pInfo->fMinAP,
            pInfo->fWindVelocity,
            pInfo->cWindDirection,
            pInfo->fHumidity);

    string sSqlText = chCondition;
    printfs(2,"<WDS_DBHelper::InsertElementsHourData()> Insert sql: %s", chCondition);
    // 执行调用SQL函数
    bool bRet = runSQLCommand(sSqlText);
    if (!bRet){
        printfs(0,"<WDS_DBHelper::InsertElementsHourData()> Insert failed, sql: %s", chCondition);
    }

    printfs(1,"<WDS_DBHelper::InsertElementsHourData()> Stop call InsertElementsHourData()");
    return bRet;
}

/******************************************************************************
    处理名        ：  删除一条气象要素——小时数据信息到数据库
    函数名        ：  DeleteElementsHourData()
    参数          ：  pInfo 气象要素——小时数据信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::DeleteElementsHourData(stElementsHourData* pInfo)
{
    printfs(1,"<WDS_DBHelper::DeleteElementsHourData()> Begin call DeleteElementsHourData()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }

    // 构建调用SQL语句所需参数字符串
    char chCondition[sizeof(stElementsHourData) + strlen(SQL_DELETE_ELEMENTS_HOUR_DATA) + 1] = "\0";
    sprintf(chCondition,
            SQL_DELETE_ELEMENTS_HOUR_DATA,
            pInfo->cCurTime,
            pInfo->cCurTime,
            pInfo->cStationID);

    string sSqlText = chCondition;
    printfs(2,"<WDS_DBHelper::DeleteElementsHourData()> Delete sql: %s", chCondition);
    // 执行调用SQL函数
    bool bRet = runSQLCommand(sSqlText);
    if (!bRet){
        printfs(0,"<WDS_DBHelper::DeleteElementsHourData()> Delete failed, sql: %s", chCondition);
    }

    printfs(1,"<WDS_DBHelper::DeleteElementsHourData()> Stop call DeleteElementsHourData()");
    return bRet;
}

/******************************************************************************
    处理名        ：  插入一条气象要素——分钟数据信息到数据库
    函数名        ：  InsertElementsMinuteData()
    参数          ：  pInfo 气象要素——分钟数据信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::InsertElementsMinuteData(stElementsMinuteData* pInfo)
{
    printfs(1,"<WDS_DBHelper::InsertElementsMinuteData()> Begin call InsertElementsMinuteData()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }

    // 构建调用SQL语句所需参数字符串
    char chCondition[sizeof(stElementsMinuteData) + strlen(SQL_INSERT_ELEMENTS_MINUTE_DATA) + 1] = "\0";
    sprintf(chCondition,
            SQL_INSERT_ELEMENTS_MINUTE_DATA,
            pInfo->cStationID,
            pInfo->cCurTime,
            pInfo->fCurTemp,
            pInfo->fRainfall,
            pInfo->fCurAP,
            pInfo->fWindVelocity,
            pInfo->cWindDirection,
            pInfo->fHumidity);

    string sSqlText = chCondition;
    printfs(2,"<WDS_DBHelper::InsertElementsMinuteData()> Insert sql: %s", chCondition);
    // 执行调用SQL函数
    bool bRet = runSQLCommand(sSqlText);
    if (!bRet){
        printfs(0,"<WDS_DBHelper::InsertElementsMinuteData()> Insert failed, sql: %s", chCondition);
    }

    printfs(1,"<WDS_DBHelper::InsertElementsMinuteData()> Stop call InsertElementsMinuteData()");
    return bRet;
}

/******************************************************************************
    处理名        ：  插入一条实时重大天气数据信息到数据库
    函数名        ：  InsertElementsHourData()
    参数          ：  pInfo 实时重大天气数据信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::InsertRealtimeWeatherReport(stRealtimeWeatherReport* pInfo)
{
    printfs(1,"<WDS_DBHelper::InsertRealtimeWeatherReport()> Begin call InsertRealtimeWeatherReport()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }

    char chEventNo[DICT_CODE_LENGTH + 1] = "\0";
    char chVisibility[DICT_CODE_LENGTH + 1] = "\0";
    if(!GetDictCodebyValue(chEventNo, DATA_DICT_WEATHER_STATUS, pInfo->nEventNo)){
        printfs(0,"<WDS_DBHelper::InsertRealtimeWeatherReport()> No dict code found! event_no:%d", pInfo->nEventNo);
        return false;
    }

    if(pInfo->nEventNo == WEATHER_STATUS_DUST){
        if(!GetDictCodebyRange(chVisibility, DATA_DICT_DUST_STATUS, pInfo->nVisibility)){
            printfs(0,"<WDS_DBHelper::InsertRealtimeWeatherReport()> No dict code found! event_no:%d", pInfo->nVisibility);
            return false;
        }
    }
    else if(pInfo->nEventNo == WEATHER_STATUS_FOG){
        if(!GetDictCodebyRange(chVisibility, DATA_DICT_FOG_STATUS, pInfo->nVisibility)){
            printfs(0,"<WDS_DBHelper::InsertRealtimeWeatherReport()> No dict code found! event_no:%d", pInfo->nVisibility);
            return false;
        }
    }
    char cTempTime[20] = "\0";
    int nUnknownTime = IsUnknownTime(pInfo->cTime, cTempTime);
    if(!nUnknownTime){
        memcpy(cTempTime, pInfo->cTime, DATE_TIME_LENGTH);
    }

    // 构建调用SQL语句所需参数字符串
    char chCondition[sizeof(stRealtimeWeatherReport) + strlen(SQL_INSERT_REALTIME_WEATHER) + 1] = "\0";
    sprintf(chCondition,
            SQL_INSERT_REALTIME_WEATHER,
            pInfo->cStationID,
            pInfo->cCurTime,
            cTempTime,
            chVisibility,
            chEventNo,
            pInfo->cEventComment,
            nUnknownTime);

    string sSqlText = chCondition;
    printfs(2,"<WDS_DBHelper::InsertElementsMinuteData()> Insert sql: %s", chCondition);
    // 执行调用SQL函数
    bool bRet = runSQLCommand(sSqlText);
    if (!bRet){
        printfs(0,"<WDS_DBHelper::InsertElementsMinuteData()> Insert failed, sql: %s", chCondition);
    }

    printfs(1,"<WDS_DBHelper::InsertRealtimeWeatherReport()> Stop call InsertRealtimeWeatherReport()");
    return bRet;
}

/******************************************************************************
    处理名        ：  插入一条台站监控静态图像信息到数据库
    函数名        ：  InsertElementsHourData()
    参数          ：  pInfo 台站监控静态图像信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::InsertStationImage(stStationImage* pInfo, long lId)
{
    printfs(1,"<WDS_DBHelper::InsertStationImage()> Begin call InsertStationImage()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }

    // 构建调用SQL语句所需参数字符串
    char chCondition[sizeof(stStationImage) + strlen(SQL_INSERT_STATION_IMAGE) + 1] = "\0";
    sprintf(chCondition,
            SQL_INSERT_STATION_IMAGE,
            pInfo->cStationID,
            lId,
            pInfo->cCurTime,
            pInfo->cDirection);

    string sSqlText = chCondition;
    printfs(2,"<WDS_DBHelper::InsertStationImage()> Insert sql: %s", chCondition);
    // 执行调用SQL函数
    bool bRet = runSQLCommand(sSqlText);
    if (!bRet){
        printfs(0,"<WDS_DBHelper::InsertStationImage()> Insert failed, sql: %s", chCondition);
    }

    printfs(1,"<WDS_DBHelper::InsertStationImage()> Stop call InsertStationImage()");
    return bRet;
}

/******************************************************************************
    处理名        ：  保存静态图像资源信息到tb_gb_file_info表
    函数名        ：  InsertImageFile()
    参数          ：  pInfo 静态图像资源信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::InsertImageFile(char* pFilePath, char* pFileName, char* pFileOriginalName, char* pTime)
{
    printfs(1,"<WDS_DBHelper::InsertImageFile()> Begin call InsertImageFile()");

    // 构建调用SQL语句所需参数字符串
    char chCondition[3 * MAX_FILEPATH_LENGTH + DATE_TIME_LENGTH + 1] = "\0";
    sprintf(chCondition,
            SQL_INSERT_IMAGE_FILE,
            pFilePath,
            pFileName,
            pFileOriginalName,
            pTime);

    string sSqlText = chCondition;
    printfs(2,"<WDS_DBHelper::InsertImageFile()> Insert sql: %s", chCondition);
    // 执行调用SQL函数
    bool bRet = runSQLCommand(sSqlText);
    if (!bRet){
        printfs(0,"<WDS_DBHelper::InsertImageFile()> Insert failed, sql: %s", chCondition);
    }

    printfs(1,"<WDS_DBHelper::InsertImageFile()> Stop call InsertImageFile()");
    return bRet;
}

/*
对数据库进行更新记录的操作
*/
/******************************************************************************
    处理名        ：  更新台站心跳信息到数据库
    函数名        ：  UpdateStationRealtimeStatus()
    参数          ：  pInfo 台站心跳信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::UpdateStationRealtimeStatus(stStationHeartBeat* pInfo)
{
    printfs(1,"<WDS_DBHelper::UpdateStationRealtimeStatus()> Begin call UpdateStationRealtimeStatus()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }

    // 取得硬盘状态字典CODE
    char chHDStatus[DICT_CODE_LENGTH + 1] = "\0";
    if(!GetDictCodebyValue(chHDStatus, DATA_DICT_HARD_DISK_STATUS, pInfo->nHDstatus)){
        printfs(0,"<WDS_DBHelper::UpdateStationRealtimeStatus()> No dict code found! HD type:%d", pInfo->nHDstatus);
        return false;
    }

    // 取得测场视频状态字典CODE
    char chGeoFieldStatus[DICT_CODE_LENGTH + 1] = "\0";
    if(!GetDictCodebyValue(chGeoFieldStatus, DATA_DICT_VIDEO_STATUS, CheckRealtimeStatus(pInfo->cChannelStatus, 0x80))){
        printfs(0,"<WDS_DBHelper::UpdateStationRealtimeStatus()> No dict code found! cChannelStatus:%x", pInfo->cChannelStatus);
        return false;
    }

    // 取得值班室视频状态字典CODE
    char chOfficeStatus[DICT_CODE_LENGTH + 1] = "\0";
    if(!GetDictCodebyValue(chOfficeStatus, DATA_DICT_VIDEO_STATUS, CheckRealtimeStatus(pInfo->cChannelStatus, 0x40))){
        printfs(0,"<WDS_DBHelper::UpdateStationRealtimeStatus()> No dict code found! cChannelStatus:%x", pInfo->cChannelStatus);
        return false;
    }

    // 取得制氢室视频状态字典CODE
    char chHydrogenStatus[DICT_CODE_LENGTH + 1] = "\0";
    if(!GetDictCodebyValue(chHydrogenStatus, DATA_DICT_VIDEO_STATUS, CheckRealtimeStatus(pInfo->cChannelStatus, 0x20))){
        printfs(0,"<WDS_DBHelper::UpdateStationRealtimeStatus()> No dict code found! cChannelStatus:%x", pInfo->cChannelStatus);
        return false;
    }

    // 构建调用SQL语句所需参数字符串
    char chCondition[sizeof(stStationHeartBeat) + strlen(SQL_UPDATE_STATION_REALTIME_STATUS) + 3 * DICT_CODE_LENGTH + 1] = "\0";
    sprintf(chCondition,
            SQL_UPDATE_STATION_REALTIME_STATUS,
            pInfo->cCurTime,
            chGeoFieldStatus,
            chOfficeStatus,
            chHydrogenStatus,
            chHDStatus,
            pInfo->nHDCapacity,
            pInfo->nFreeSpace,
            pInfo->cStationID,
            pInfo->cCurTime);

    string sSqlText = chCondition;
    printfs(2,"<WDS_DBHelper::UpdateStationRealtimeStatus()> Update sql: %s", chCondition);
    // 执行调用SQL函数
    bool bRet = runSQLCommand(sSqlText);
    if (!bRet){
        printfs(0,"<WDS_DBHelper::UpdateStationRealtimeStatus()> Insert failed, sql: %s", chCondition);
    }

    printfs(1,"<WDS_DBHelper::UpdateStationRealtimeStatus()> Stop call UpdateStationRealtimeStatus()");
    return bRet;
}

/******************************************************************************
    处理名        ：  更新一条气象要素——分钟数据信息到数据库
    函数名        ：  UpdateElementsMinuteData()
    参数          ：  pInfo 气象要素——分钟数据信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::UpdateElementsMinuteData(stElementsMinuteData* pInfo)
{
    printfs(1,"<WDS_DBHelper::UpdateElementsMinuteData()> Begin call UpdateElementsMinuteData()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }

    // 构建调用SQL语句所需参数字符串
    char chCondition[sizeof(stElementsMinuteData) + strlen(SQL_UPDATE_ELEMENTS_MINUTE_DATA) + 1] = "\0";
    sprintf(chCondition,
            SQL_UPDATE_ELEMENTS_MINUTE_DATA,
            pInfo->cCurTime,
            pInfo->fCurTemp,
            pInfo->fRainfall,
            pInfo->fCurAP,
            pInfo->fWindVelocity,
            pInfo->cWindDirection,
            pInfo->fHumidity,
            pInfo->cStationID);

    string sSqlText = chCondition;
    printfs(2,"<WDS_DBHelper::UpdateElementsMinuteData()> Update sql: %s", chCondition);
    // 执行调用SQL函数
    bool bRet = runSQLCommand(sSqlText);
    if (!bRet){
        printfs(0,"<WDS_DBHelper::UpdateElementsMinuteData()> Insert failed, sql: %s", chCondition);
    }

    printfs(1,"<WDS_DBHelper::UpdateElementsMinuteData()> Stop call UpdateElementsMinuteData()");
    return bRet;
}

/******************************************************************************
    处理名        ：  更新实时重大天气数据信息到数据库
    函数名        ：  UpdateRealtimeWeatherReport()
    参数          ：  pInfo 实时重大天气数据信息
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::UpdateRealtimeWeatherReport(stRealtimeWeatherReport* pInfo, int nId)
{
    printfs(1,"<WDS_DBHelper::UpdateRealtimeWeatherReport()> Begin call UpdateRealtimeWeatherReport()");
    // 判断参数正确性
    if( NULL == pInfo){
        return 0;
    }
    char cTempTime[20] = "\0";
    int nUnknownTime = IsUnknownTime(pInfo->cTime, cTempTime);
    if(!nUnknownTime){
        memcpy(cTempTime, pInfo->cTime, DATE_TIME_LENGTH);
    }

    // 构建调用SQL语句所需参数字符串
    char chCondition[sizeof(stRealtimeWeatherReport) + strlen(SQL_UPDATE_REALTIME_WEATHER) + 1] = "\0";
    sprintf(chCondition,
            SQL_UPDATE_REALTIME_WEATHER,
            cTempTime,
            nUnknownTime,
            nId);

    string sSqlText = chCondition;
    printfs(2,"<WDS_DBHelper::UpdateRealtimeWeatherReport()> Update sql: %s", chCondition);
    // 执行调用SQL函数
    bool bRet = runSQLCommand(sSqlText);
    if (!bRet){
        printfs(0,"<WDS_DBHelper::UpdateRealtimeWeatherReport()> Insert failed, sql: %s", chCondition);
    }

    printfs(1,"<WDS_DBHelper::UpdateRealtimeWeatherReport()> Stop call UpdateRealtimeWeatherReport()");
    return bRet;
}

/*
对数据库进行查询记录的操作
*/
/******************************************************************************
    处理名        ：  根据指定SQL文从数据库获得分钟数据个数
    函数名        ：  GetElementsMinuteDataCount()
    参数          ：  nCount 分钟数据个数
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::GetElementsMinuteDataCount(char* chCondition, int& nCount)
{
    printfs(1,"<WDS_DBHelper::GetElementsMinuteDataCount()> Begin call GetElementsMinuteDataCount()");
    char chQueryText[strlen(SQL_QUERY_ELEMENTS_MINUTE_DATA) + STATION_ID_LENGTH + 1] = "\0";
    sprintf(chQueryText, SQL_QUERY_ELEMENTS_MINUTE_DATA, chCondition);

    string sQeryText = chQueryText;
    // 执行查询函数
    unsigned int nRet = 0;
    printfs(2,"<WDS_DBHelper::GetElementsMinuteDataCount()> Query sql: %s", chQueryText);
    nRet = mysql_real_query(m_pSQLClient,sQeryText.c_str(),
                         (unsigned int)strlen(sQeryText.c_str()));// 执行查询
    if ( nRet ) {
        printfs(0,"<WDS_DBHelper::GetElementsMinuteDataCount()> Error query from database: %s",mysql_error(m_pSQLClient));
        return false;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(m_pSQLClient);

    while( (row = mysql_fetch_row(res)) )
    {// 遍历结果集
        nCount = 0;
        try{
            nCount    = atoi(row[0]);
        }
        catch(...){
            printfs(0,"<WDS_DBHelper::GetElementsMinuteDataCount()> Data query error!");
            continue;
        }
    }
    mysql_free_result(res); //free result after you get the result

    printfs(1,"<WDS_DBHelper::GetElementsMinuteDataCount()> Stop call GetElementsMinuteDataCount()");
    return true;
}

/******************************************************************************
    处理名        ：  根据指定SQL文从数据库获得指定台站的报警个数
    函数名        ：  GetStationRealtimeAlertCount()
    参数          ：  nCount 报警个数
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::GetStationRealtimeAlertCount(char* chCondition, int nType, int& nCount)
{
    printfs(1,"<WDS_DBHelper::GetStationRealtimeAlertCount()> Begin call GetStationRealtimeAlertCount()");
    char chQueryText[strlen(SQL_QUERY_1001_ALERT_COUNT) + STATION_ID_LENGTH + 1] = "\0";
    if (nType == INFO_TYPE_RMS1001_REALTIME_ALERT){
        sprintf(chQueryText, SQL_QUERY_1001_ALERT_COUNT, chCondition);
    } else {
        sprintf(chQueryText, SQL_QUERY_5001_ALERT_COUNT, chCondition);
    }

    string sQeryText = chQueryText;
    // 执行查询函数
    unsigned int nRet = 0;
    printfs(2,"<WDS_DBHelper::GetStationRealtimeAlertCount()> Query sql: %s", chQueryText);
    nRet = mysql_real_query(m_pSQLClient,sQeryText.c_str(),
                         (unsigned int)strlen(sQeryText.c_str()));// 执行查询
    if ( nRet ) {
        printfs(0,"<WDS_DBHelper::GetStationRealtimeAlertCount()> Error query from database: %s",mysql_error(m_pSQLClient));
        return false;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(m_pSQLClient);

    while( (row = mysql_fetch_row(res)) )
    {// 遍历结果集
        nCount = 0;
        try{
            nCount    = atoi(row[0]);
        }
        catch(...){
            printfs(0,"<WDS_DBHelper::GetStationRealtimeAlertCount()> Data query error!");
            continue;
        }
    }
    mysql_free_result(res); //free result after you get the result

    printfs(1,"<WDS_DBHelper::GetStationRealtimeAlertCount()> Stop call GetStationRealtimeAlertCount()");
    return true;
}

/******************************************************************************
    处理名        ：  根据指定SQL文从数据库获得指定台站的最近一次报警状态
    函数名        ：  GetStationNealyAlertStatus()
    参数          ：  pStatus 最近一次报警状态
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::GetStationNealyAlertStatus(char* chCondition, int nType, int *pStatus)
{
    if (pStatus == NULL){
        return false;
    }
    memset(pStatus, 0, 8 * sizeof(int));
    printfs(1,"<WDS_DBHelper::GetStationNealyAlertStatus()> Begin call GetStationNealyAlertStatus()");
    char chQueryText[strlen(SQL_QUERY_1001_NEALY_ALARM) + STATION_ID_LENGTH + 1] = "\0";
    if (nType == INFO_TYPE_RMS1001_REALTIME_ALERT){
        sprintf(chQueryText, SQL_QUERY_1001_NEALY_ALARM, chCondition);
    } else {
        sprintf(chQueryText, SQL_QUERY_5001_NEALY_ALARM, chCondition);
    }

    string sQeryText = chQueryText;
    // 执行查询函数
    unsigned int nRet = 0;
    printfs(2,"<WDS_DBHelper::GetStationNealyAlertStatus()> Query sql: %s", chQueryText);
    nRet = mysql_real_query(m_pSQLClient,sQeryText.c_str(),
                         (unsigned int)strlen(sQeryText.c_str()));// 执行查询
    if ( nRet ) {
        printfs(0,"<WDS_DBHelper::GetStationNealyAlertStatus()> Error query from database: %s",mysql_error(m_pSQLClient));
        return false;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(m_pSQLClient);

    while( (row = mysql_fetch_row(res)) )
    {// 遍历结果集
        try{
            pStatus[0] = atoi(row[0]);
            pStatus[1] = atoi(row[1]);
            pStatus[2] = atoi(row[2]);
            pStatus[3] = atoi(row[3]);
            pStatus[4] = atoi(row[4]);
            pStatus[5] = atoi(row[5]);
            pStatus[6] = atoi(row[6]);
            pStatus[7] = atoi(row[7]);
        }
        catch(...){
            printfs(0,"<WDS_DBHelper::GetStationNealyAlertStatus()> Data query error!");
            continue;
        }
    }
    mysql_free_result(res); //free result after you get the result

    printfs(1,"<WDS_DBHelper::GetStationNealyAlertStatus()> Stop call GetStationNealyAlertStatus()");
    return true;
}

/******************************************************************************
    处理名        ：  根据指定SQL文从数据库获得指定台站的实时心跳记录数量
    函数名        ：  GetStationRealtimeStatusCount()
    参数          ：  nCount 分钟数据个数
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::GetStationRealtimeStatusCount(char* chCondition1, char* chCondition2, int& nCount)
{
    printfs(1,"<WDS_DBHelper::GetStationRealtimeStatusCount()> Begin call GetStationRealtimeStatusCount()");
    char chQueryText[strlen(SQL_QUERY_STATION_REALTIME_STATUS) + STATION_ID_LENGTH + DATE_TIME_LENGTH + 1] = "\0";
    sprintf(chQueryText, SQL_QUERY_STATION_REALTIME_STATUS, chCondition1, chCondition2);

    string sQeryText = chQueryText;
    // 执行查询函数
    unsigned int nRet = 0;
    printfs(2,"<WDS_DBHelper::GetStationRealtimeStatusCount()> Query sql: %s", chQueryText);
    nRet = mysql_real_query(m_pSQLClient,sQeryText.c_str(),
                         (unsigned int)strlen(sQeryText.c_str()));// 执行查询
    if ( nRet ) {
        printfs(0,"<WDS_DBHelper::GetStationRealtimeStatusCount()> Error query from database: %s",mysql_error(m_pSQLClient));
        return false;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(m_pSQLClient);

    while( (row = mysql_fetch_row(res)) )
    {// 遍历结果集
        nCount = 0;
        try{
            nCount    = atoi(row[0]);
        }
        catch(...){
            printfs(0,"<WDS_DBHelper::GetStationRealtimeStatusCount()> Data query error!");
            continue;
        }
    }
    mysql_free_result(res); //free result after you get the result

    printfs(1,"<WDS_DBHelper::GetStationRealtimeStatusCount()> Stop call GetStationRealtimeStatusCount()");
    return true;
}

/******************************************************************************
    处理名        ：  根据指定SQL文从数据库获得实时重大天气数据信息ID
    函数名        ：  GetMaxWeatherReportID()
    参数          ：  nId 实时重大天气数据信息ID
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::GetMaxWeatherReportID(stRealtimeWeatherReport* pInfo, int& nId)
{
    printfs(1,"<WDS_DBHelper::GetMaxWeatherReportID()> Begin call GetMaxWeatherReportID()");
    char chQueryText[strlen(SQL_QUERY_REALTIME_WEATHER) + sizeof(stRealtimeWeatherReport) + 1] = "\0";
    char chCode[DICT_CODE_LENGTH+1] = "\0";
    if(!GetDictCodebyValue(chCode, DATA_DICT_WEATHER_STATUS, pInfo->nEventNo)){
        printfs(0,"<WDS_DBHelper::GetMaxWeatherReportID()> No dict code found! event_no:%d", pInfo->nEventNo);
        return false;
    }

    sprintf(chQueryText, 
            SQL_QUERY_REALTIME_WEATHER, 
            pInfo->cStationID, 
            chCode);

    string sQeryText = chQueryText;
    // 执行查询函数
    unsigned int nRet = 0;
    printfs(2,"<WDS_DBHelper::GetMaxWeatherReportID()> Query sql: %s", chQueryText);
    nRet = mysql_real_query(m_pSQLClient,sQeryText.c_str(),
                         (unsigned int)sQeryText.size());// 执行查询
    if ( nRet ) {
        printfs(0,"<WDS_DBHelper::GetMaxWeatherReportID()> Error query from database: %s",mysql_error(m_pSQLClient));
        return false;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(m_pSQLClient);

    while( (row = mysql_fetch_row(res)) )
    {// 遍历结果集
        try{
            if (row[0] != NULL){
                nId    = atoi(row[0]);
            }
        }
        catch(...){
            printfs(0,"<WDS_DBHelper::GetMaxWeatherReportID()> Data query error!");
            continue;
        }
    }
    mysql_free_result(res); //free result after you get the result

    printfs(1,"<WDS_DBHelper::GetMaxWeatherReportID()> Stop call GetMaxWeatherReportID()");
    return true;
}

/******************************************************************************
    处理名        ：  根据指定SQL文从数据库获得tb_gb_file_info表中的指定图像的ID
    函数名        ：  GetImageFileID()
    参数          ：  lId 图像的ID
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::GetImageFileID(char* pFileName, char* pFilePath, long& lId)
{
    printfs(1,"<WDS_DBHelper::GetImageFileID()> Begin call GetImageFileID()");
    char chQueryText[strlen(SQL_QUERY_IMAGE_FILE_ID) + 2 * MAX_FILEPATH_LENGTH + 1] = "\0";
    sprintf(chQueryText, 
            SQL_QUERY_IMAGE_FILE_ID, 
            pFileName, 
            pFilePath);

    string sQeryText = chQueryText;
    // 执行查询函数
    unsigned int nRet = 0;
    printfs(2,"<WDS_DBHelper::GetImageFileID()> Query sql: %s", chQueryText);
    nRet = mysql_real_query(m_pSQLClient,sQeryText.c_str(),
                         (unsigned int)sQeryText.size());// 执行查询
    if ( nRet ) {
        printfs(0,"<WDS_DBHelper::GetImageFileID()> Error query from database: %s",mysql_error(m_pSQLClient));
        return false;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(m_pSQLClient);

    while( (row = mysql_fetch_row(res)) )
    {// 遍历结果集
        try{
            if (row[0] != NULL){
                lId    = atol(row[0]);
            }
        }
        catch(...){
            printfs(0,"<WDS_DBHelper::GetImageFileID()> Data query error!");
            continue;
        }
    }
    mysql_free_result(res); //free result after you get the result

    printfs(1,"<WDS_DBHelper::GetImageFileID()> Stop call GetImageFileID()");
    return true;
}

/*
对数据库进行删除记录的操作
*/

/*
共通操作处理
*/
/******************************************************************************
    处理名        ：  通过取值范围确定CODE内容
    函数名        ：  GetDictCodebyRange()
    参数          ：  chCode CODE内容
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::GetDictCodebyRange(char* chCode, char* chType, int nValue)
{
    if(chCode == NULL){
        return false;
    }
    printfs(1,"<WDS_DBHelper::GetDictCodebyRange()> Begin call GetDictCodebyRange()");
    char chQueryText[strlen(SQL_QUERY_DATA_DICT_BY_RANGE) + DICT_CODE_LENGTH + 1] = "\0";
    sprintf(chQueryText, SQL_QUERY_DATA_DICT_BY_RANGE, chType);

    string sQeryText = chQueryText;
    // 执行查询函数
    unsigned int nRet = 0;
    printfs(2,"<WDS_DBHelper::GetDictCodebyRange()> Query sql: %s", chQueryText);
    nRet = mysql_real_query(m_pSQLClient,sQeryText.c_str(),
                         (unsigned int)strlen(sQeryText.c_str()));// 执行查询
    if ( nRet ) {
        printfs(0,"<WDS_DBHelper::GetDictCodebyRange()> Error query from database: %s",mysql_error(m_pSQLClient));
        return false;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(m_pSQLClient);
    char tempValue[32] = "\0";
    int nMaxRange = 0;
    int nMinRange = 0;

    while( (row = mysql_fetch_row(res)) )
    {// 遍历结果集
        nMaxRange = 0;
        nMinRange = 0;
        try{
            memcpy(tempValue, row[1], 32);
            sscanf(tempValue, "%d,%d", &nMinRange, &nMaxRange);
            // 未知能见度
            if((nMinRange == nMaxRange) && (nMinRange == nValue)){
                memcpy(chCode, row[0], DICT_CODE_LENGTH);
                break;
            }
            if((nValue >= nMinRange) && (nValue < nMaxRange)){
                memcpy(chCode, row[0], DICT_CODE_LENGTH);
                break;
            }
        }
        catch(...){
            printfs(0,"<WDS_DBHelper::GetDictCodebyRange()> Data query error!");
            continue;
        }
    }
    mysql_free_result(res); //free result after you get the result

    printfs(1,"<WDS_DBHelper::GetDictCodebyRange()> Stop call GetDictCodebyRange()");
    return true;
}

/******************************************************************************
    处理名        ：  通过具体值确定CODE内容
    函数名        ：  GetDictCodebyValue()
    参数          ：  chCode CODE内容
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::GetDictCodebyValue(char* chCode, char* chType, int nValue)
{
    if(chCode == NULL){
        return false;
    }
    printfs(1,"<WDS_DBHelper::GetDictCodebyValue()> Begin call GetDictCodebyValue()");
    char chQueryText[strlen(SQL_QUERY_DATA_DICT_BY_VALUE) + DICT_TYPE_LENGTH + sizeof(nValue) + 1] = "\0";

    sprintf(chQueryText, SQL_QUERY_DATA_DICT_BY_VALUE, chType, nValue);

    string sQeryText = chQueryText;
    // 执行查询函数
    unsigned int nRet = 0;
    printfs(2,"<WDS_DBHelper::GetDictCodebyValue()> Query sql: %s", chQueryText);
    nRet = mysql_real_query(m_pSQLClient,sQeryText.c_str(),
                         (unsigned int)strlen(sQeryText.c_str()));// 执行查询
    if ( nRet ) {
        printfs(0,"<WDS_DBHelper::GetDictCodebyValue()> Error query from database: %s",mysql_error(m_pSQLClient));
        return false;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(m_pSQLClient);

    while( (row = mysql_fetch_row(res)) )
    {// 遍历结果集
        try{
            memcpy(chCode, row[0], DICT_CODE_LENGTH);
        }
        catch(...){
            printfs(0,"<WDS_DBHelper::GetDictCodebyValue()> Data query error!");
            continue;
        }
    }
    mysql_free_result(res); //free result after you get the result

    printfs(1,"<WDS_DBHelper::GetDictCodebyValue()> Stop call GetDictCodebyValue()");
    return true;
}

/******************************************************************************
    处理名        ：  通过CODE确定具体值内容
    函数名        ：  GetSysConfigValuebyCode()
    参数          ：  chCode CODE内容
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
int WDS_DBHelper::GetSysConfigValuebyCode(char* chCode, char* chValue)
{
    if(chValue == NULL){
        return false;
    }
    printfs(1,"<WDS_DBHelper::GetSysConfigValuebyCode()> Begin call GetSysConfigValuebyCode()");
    char chQueryText[strlen(SQL_QUERY_SYSTEM_CONFIG_BY_VALUE) + DICT_CODE_LENGTH + 1] = "\0";
    sprintf(chQueryText, SQL_QUERY_SYSTEM_CONFIG_BY_VALUE, chCode);

    string sQeryText = chQueryText;
    // 执行查询函数
    unsigned int nRet = 0;
    printfs(2,"<WDS_DBHelper::GetSysConfigValuebyCode()> Query sql: %s", chQueryText);
    nRet = mysql_real_query(m_pSQLClient,sQeryText.c_str(),
                         (unsigned int)strlen(sQeryText.c_str()));// 执行查询
    if ( nRet ) {
        printfs(0,"<WDS_DBHelper::GetSysConfigValuebyCode()> Error query from database: %s",
mysql_error(m_pSQLClient));
        return false;
    }
    MYSQL_RES *res;
    MYSQL_ROW row;
    res = mysql_store_result(m_pSQLClient);

    while( (row = mysql_fetch_row(res)) )
    {// 遍历结果集
        try{
            memcpy(chValue, row[0], DICT_VALUE_LENGTH);
        }
        catch(...){
            printfs(0,"<WDS_DBHelper::GetSysConfigValuebyCode()> Data query error!");
            continue;
        }
    }
    mysql_free_result(res); //free result after you get the result

    printfs(1,"<WDS_DBHelper::GetSysConfigValuebyCode()> Stop call GetSysConfigValuebyCode()");
    return true;
}

/******************************************************************************
    处理名        ：  通过CODE确定具体值内容
    函数名        ：  GetSysConfigValuebyCode()
    参数          ：  
    返回值        ：  1:未知时间  0:不是未知时间
******************************************************************************/
int WDS_DBHelper::IsUnknownTime(char* pTime, char* pTempTime)
{
    printfs(2,"<WDS_DBHelper::IsUnknownTime()> Input time :%s", pTime);
    int nYear   = 0;
    int nMonth  = 0;
    int nDay    = 0;
    int nHour   = 0;
    int nMinute = 0;
    int nSecond = 0;
    sscanf(pTime, "%d-%d-%d %d:%d:%d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);
    printfs(2,"<WDS_DBHelper::IsUnknownTime()> Input time :Year[%d], Month[%d], Day[%d], Hour[%d], Minute[%d], Second[%d]", nYear, nMonth, nDay, nHour, nMinute, nSecond);
    if ((nHour == 77) && (nMinute == 88)){
        sprintf(pTempTime, "%04d-%02d-%02d %02d:%02d:%02d", nYear, nMonth, nDay, 0, 0, 0);
        return 1;
    }
    return 0;
}
