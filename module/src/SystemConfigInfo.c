/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        配置文件处理机能                        */
/*           程序名        ：        SystemConfigInfo.cpp                    */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include <string.h>
#include "SystemConfigInfo.h"

/******************************************************************************
    处理名        ：  本类的构造处理
    函数名        ：  构造函数
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CSystemConfigInfo::CSystemConfigInfo(){
    m_nCdrDBPort       = -1;              /* CDR-DB服务器的连接端口号        */
    m_nRecvPort        = 0;               /* 交易管理前台service监视端口     */
    m_nLogLevel        = 0;               /* 日志输出的级别控制              */
    m_nSocketWiteTime  = 0;               /* 端口的连接超时时间              */
    m_nMaxSubThread    = 10;              /* 系统最大子线程数                */
    m_nMaxSocket       = 10;              /* 系统可开启的最大Socket连接数    */
    strcpy( m_chCdrDBIPAddr , "0.0.0.0"); /* CDR-DB服务器的IP地址            */
    strcpy( m_chCdrDBName   , "\0");      /* CDR-DB服务器的服务名            */
    strcpy( m_chCdrDBUser   , "\0");      /* CDR-DB数据库的登录用户名        */
    strcpy( m_chCdrDBPasswd , "\0");      /* CDR-DB数据库的登录密码          */
    strcpy( m_chLogFilePath , "\0");      /* 日志文件的保存路径              */
    strcpy( m_chImgFilePath , "\0");      /* 图像数据文件的保存路径          */
}

/******************************************************************************
    处理名        ：  本类的析构处理
    函数名        ：  析构函数
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CSystemConfigInfo::~CSystemConfigInfo(){
}

/******************************************************************************
    处理名        ：  设定CDR-DB服务器的IP地址
    函数名        ：  SetCdrDBIPAddress()
    参数          ：  (I) ：pIPAddr - CDR-DB服务器的IP地址
    返回值        ：  无
******************************************************************************/
void CSystemConfigInfo::SetCdrDBIPAddress(const p_char_t pIPAddr){
    if ( NULL != pIPAddr ) {
        memcpy(m_chCdrDBIPAddr, pIPAddr, IPADDRESS_LENGTH);
    }
}

/******************************************************************************
    处理名        ：  设定CDR-DB服务器的服务名
    函数名        ：  SetCdrDBName()
    参数          ：  (I) ：pName - CDR-DB服务器的服务名
    返回值        ：  无
******************************************************************************/
void CSystemConfigInfo::SetCdrDBName(const p_char_t pName){
    if ( NULL != pName ) {
        memcpy(m_chCdrDBName, pName, 256);
    }
}

/******************************************************************************
    处理名        ：  设定CDR-DB数据库的登录用户名
    函数名        ：  SetCdrDBUser()
    参数          ：  (I) ：pUser - CDR-DB数据库的登录用户名
    返回值        ：  无
******************************************************************************/
void CSystemConfigInfo::SetCdrDBUser(const p_char_t pUser){
    if ( NULL != pUser ) {
        memcpy(m_chCdrDBUser, pUser, 256);
    }
}

/******************************************************************************
    处理名        ：  设定CDR-DB数据库的登录密码
    函数名        ：  SetDBPassword()
    参数          ：  (I) ：pPassword - CDR-DB数据库的登录密码
    返回值        ：  无
******************************************************************************/
void CSystemConfigInfo::SetCdrDBPasswd(const p_char_t pPasswd){
    if ( NULL != pPasswd ) {
        memcpy(m_chCdrDBPasswd, pPasswd, 256);
    }
}

/******************************************************************************
    处理名        ：  设定日志文件的保存路径
    函数名        ：  SetLogPath()
    参数          ：  (I) ：pPath - 日志文件的保存路径
    返回值        ：  无
******************************************************************************/
void CSystemConfigInfo::SetLogPath(const p_char_t pPath){
    if ( NULL != pPath ) {
        memcpy(m_chLogFilePath, pPath, MAX_FILEPATH_LENGTH);
    }
}

/******************************************************************************
    处理名        ：  设定图像数据文件的保存路径
    函数名        ：  SetImgPath()
    参数          ：  (I) ：pPath - 图像数据文件的保存路径
    返回值        ：  无
******************************************************************************/
void CSystemConfigInfo::SetImgPath(const p_char_t pPath){
    if ( NULL != pPath ) {
        memcpy(m_chImgFilePath, pPath, MAX_FILEPATH_LENGTH);
    }
}

/******************************************************************************
    End
******************************************************************************/
