/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        SQLCondition.h                          */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#ifndef _SQLCONDITION_H
#define _SQLCONDITION_H

// 字典表字段TYPE定义
#define DATA_DICT_WEATHER_STATUS   "WEATHER_STATUS"
#define DATA_DICT_DUST_STATUS      "DUST_STATUS"
#define DATA_DICT_FOG_STATUS       "FOG_STATUS"
#define DATA_DICT_HARD_DISK_STATUS "HARD_DISK_STATUS"
#define DATA_DICT_VIDEO_STATUS     "VIDEO_STATUS"
#define DATA_DICT_ALERT_STATUS     "ALERT_STATUS"

#define SYSTEM_CONFIG_SYNC_STEP        "SC0101"
#define SYSTEM_CONFIG_HEART_STEP       "SC0102"
#define SYSTEM_CONFIG_NORMAL_INTERVAL  "SC0103"
#define SYSTEM_CONFIG_ALERT_INTERVAL   "SC0104"
#define SYSTEM_CONFIG_IMAGE_FOLDER     "SC0105"
#define SYSTEM_CONFIG_HEART_CHECK_STEP "SC0106"

// ▶▶▶气象数据组成结构——小时数据SQL文开始▶▶▶
// 插入小时数据到小时天气表
#define SQL_INSERT_ELEMENTS_HOUR_DATA "INSERT INTO tb_elements_hour_data (station_id, operation_time, upload_time, punctually_air_temperature, max_air_temperature, min_air_temperature, rainfall, punctually_air_pressure, max_air_pressure, min_air_pressure, wind_speed, wind_direction, humidity) "\
                                      "VALUES ('%.10s', '%s', NOW(), %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, %.2f, '%s', %.2f)"
// 删除已存在的小时气象数据
#define SQL_DELETE_ELEMENTS_HOUR_DATA "DELETE FROM tb_elements_hour_data WHERE operation_time >= date_format('%s', '%%Y-%%m-%%d %%H:00:00') AND operation_time <= date_format('%s', '%%Y-%%m-%%d %%H:59:59') AND station_id = '%s'"
// ◀◀◀气象数据组成结构——小时数据SQL文结束◀◀◀

// ▶▶▶气象数据组成结构——分钟数据SQL文开始▶▶▶
// 取得当前台站的分钟数据信息
#define SQL_QUERY_ELEMENTS_MINUTE_DATA  "SELECT COUNT(station_id) FROM tb_elements_minute_data WHERE station_id = '%.10s'"

// 插入分钟数据到分钟天气表
#define SQL_INSERT_ELEMENTS_MINUTE_DATA "INSERT INTO tb_elements_minute_data (station_id, operation_time, upload_time, air_temperature, rainfall, air_pressure, wind_speed, wind_direction, humidity) "\
                                        "VALUES ('%.10s', '%s', NOW(), %.2f, %.2f, %.2f, %.2f,  '%s', %.2f)"

// 更新分钟数据到分钟天气表
#define SQL_UPDATE_ELEMENTS_MINUTE_DATA "UPDATE tb_elements_minute_data SET operation_time='%s', upload_time=NOW(), air_temperature=%.2f, rainfall=%.2f, air_pressure=%.2f, wind_speed=%.2f, wind_direction='%s', humidity=%.2f WHERE station_id = '%.10s' "
// ◀◀◀气象数据组成结构——分钟数据SQL文结束◀◀◀

// ▶▶▶实时心跳记录处理开始▶▶▶
// 取得当前台站的实时心跳记录个数
#define SQL_QUERY_STATION_REALTIME_STATUS  "SELECT COUNT(station_id) FROM tb_station_realtime_status WHERE station_id = '%.10s' AND ABS(TIMESTAMPDIFF(SECOND, operation_time, STR_TO_DATE('%s','%%Y-%%m-%%d %%H:%%i:%%s'))) < (SELECT CAST(value AS DECIMAL) FROM tb_gb_system_config WHERE code = 'SC0106')"

// 插入实时心跳记录数据到实时心跳记录表
#define SQL_INSERT_STATION_REALTIME_STATUS "INSERT INTO tb_station_realtime_status (station_id, operation_time, upload_time, geo_field_video, office_video, hydrogen_video, harddisk_status, harddisk_capacity, harddisk_free_space) "\
                                           "VALUES ('%.10s', '%s', NOW(), '%s', '%s', '%s', '%s', %d, %d)"

// 更新实时心跳记录数据到实时心跳记录表
#define SQL_UPDATE_STATION_REALTIME_STATUS "UPDATE tb_station_realtime_status SET operation_time='%s', upload_time=NOW(), geo_field_video='%s', office_video='%s', hydrogen_video='%s', harddisk_status='%s', harddisk_capacity=%d, harddisk_free_space=%d WHERE station_id = '%.10s' "\
                                           " AND ABS(TIMESTAMPDIFF(SECOND, operation_time, STR_TO_DATE('%s','%%Y-%%m-%%d %%H:%%i:%%s'))) < (SELECT CAST(value AS DECIMAL) FROM tb_gb_system_config WHERE code = 'SC0106')"
// ◀◀◀实时心跳记录处理结束◀◀◀

// ▶▶▶1001台站实时报警处理开始▶▶▶
// 取得当前1001台站的实时实时报警记录个数
#define SQL_QUERY_1001_ALERT_COUNT  "SELECT COUNT(station_id) FROM tb_rms1001_realtime_alert WHERE station_id = '%.10s'"

// 取得当前1001台站的最近实时报警信息
#define SQL_QUERY_1001_NEALY_ALARM "SELECT BIN(passive_device0), BIN(passive_device1), BIN(passive_device2), BIN(passive_device3), BIN(passive_device4), BIN(passive_device5), BIN(passive_device6), BIN(passive_device7) from tb_rms1001_realtime_alert WHERE station_id = '%.10s' ORDER BY upload_time DESC LIMIT 1"

// 插入新的1001台站报警信息
#define SQL_INSERT_1001_REALTIME_ALERT "INSERT INTO tb_rms1001_realtime_alert (station_id, operation_time, upload_time, passive_device0, passive_device1, passive_device2, passive_device3, passive_device4, passive_device5, passive_device6, passive_device7, alert_device0, alert_device1, alert_device2, alert_device3, alert_device4, alert_device5, alert_device6, alert_device7) "\
                                       "VALUES ('%.10s', '%s', NOW(), %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)"
// ◀◀◀1001台站实时报警处理结束◀◀◀

// ▶▶▶5001台站实时报警处理开始▶▶▶
// 取得当前1001台站的实时实时报警记录个数
#define SQL_QUERY_5001_ALERT_COUNT  "SELECT COUNT(station_id) FROM tb_rms5001_realtime_alert WHERE station_id = '%.10s'"

// 取得当前5001台站的最近实时报警信息
#define SQL_QUERY_5001_NEALY_ALARM "SELECT BIN(passive_device0), BIN(passive_device1), BIN(passive_device2), BIN(passive_device3), BIN(passive_device4), BIN(passive_device5), BIN(passive_device6), BIN(passive_device7) from tb_rms5001_realtime_alert WHERE station_id = '%.10s' ORDER BY upload_time DESC LIMIT 1"

// 插入新的5001台站报警信息
#define SQL_INSERT_5001_REALTIME_ALERT "INSERT INTO tb_rms5001_realtime_alert (station_id, operation_time, upload_time, passive_device0, passive_device1, passive_device2, passive_device3, passive_device4, passive_device5, passive_device6, passive_device7, alert_device0, alert_device1, alert_device2, alert_device3, alert_device4, alert_device5, alert_device6, alert_device7) "\
                                       "VALUES ('%.10s', '%s', NOW(), %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d, %d)"
// ◀◀◀5001台站实时报警处理结束◀◀◀

// ▶▶▶实时重大天气情报SQL文开始▶▶▶
// 取得最符合当前实时重大天气情报的开始记录ID
#define SQL_QUERY_REALTIME_WEATHER      "SELECT id, MAX(begin_time) FROM tb_realtime_weather_report WHERE station_id = '%.10s' AND event_no = '%.6s' AND end_time IS NULL"

// 插入实时重大天气到实时重大天气情况表
#define SQL_INSERT_REALTIME_WEATHER     "INSERT INTO tb_realtime_weather_report (station_id, operation_time, upload_time, begin_time, visibility, event_no, event_comment, begin_time_unknown) "\
                                        "VALUES ('%.10s', '%s', NOW(), '%s', '%s', '%.6s', '%s', %d)"

// 更新实时重大天气到实时重大天气情况表
#define SQL_UPDATE_REALTIME_WEATHER     "UPDATE tb_realtime_weather_report SET end_time='%s', upload_time=NOW(), end_time_unknown=%d WHERE id = %d"\
// ◀◀◀实时重大天气情报SQL文结束◀◀◀

// ▶▶▶台站监控静态图像SQL文开始▶▶▶
// 取得tb_gb_file_info表中的指定图像的ID
#define SQL_QUERY_IMAGE_FILE_ID      "SELECT id FROM tb_gb_file_info WHERE file_name = '%s' AND file_path = '%s'"

// 保存静态图像资源信息到tb_gb_file_info表
#define SQL_INSERT_IMAGE_FILE        "INSERT INTO tb_gb_file_info (file_path, file_name, original_file_name, upload_time) "\
                                     "VALUES ('%s', '%s', '%s', '%s')"

// 保存台站图像关联信息到tb_station_image表
#define SQL_INSERT_STATION_IMAGE     "INSERT INTO tb_station_image (station_id, file_id, operation_time, upload_time, direction) "\
                                     "VALUES ('%.10s', %ld, '%s', NOW(), '%s')"
// ◀◀◀台站监控静态图像SQL文结束◀◀◀

// ▶▶▶字典表SQL文开始▶▶▶
// 通过取值范围确定CODE内容
#define SQL_QUERY_DATA_DICT_BY_RANGE    "SELECT code, value FROM tb_gb_data_dict WHERE type = '%s'"

// 通过具体值确定CODE内容
#define SQL_QUERY_DATA_DICT_BY_VALUE    "SELECT code FROM tb_gb_data_dict WHERE type = '%s' AND value='%d'"

// 通过CODE确定具体值内容
#define SQL_QUERY_SYSTEM_CONFIG_BY_VALUE    "SELECT value FROM tb_gb_system_config WHERE code='%6s'"
// ◀◀◀字典表SQL文结束◀◀◀

#endif

/******************************************************************************
    End
******************************************************************************/

