/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        配置文件处理机能                        */
/*           程序名        ：        SystemConfigInfo.h                      */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

//#pragma   warning(disable:4786) 

#include "VodUserType.h"
#include "VodEntity.h"

#if !defined(_SYSTEMCONFIGINFO_H)
#define _SYSTEMCONFIGINFO_H

/* 配置文件的主键定义                                                        */
#define CFG_MCS_CONFIG_MODE     "[MCS_CONFIG]"
#define CFG_RSS_CONFIG_MODE     "[RSS_CONFIG]"
#define CFG_CDR_DB_CONFIG_MODE  "[CDR_DB_CONFIG]"
#define CFG_TDM_CONFIG_MODE     "[TDM_CONFIG]"
#define CFG_WDS_CONFIG_MODE     "[WDS_CONFIG]"

/* WDS-DB服务器的IP地址                                                      */
#define CFG_WDS_DB_IP           "WDS_DB_IP"
/* WDS-DB服务器的连接端口号                                                  */
#define CFG_WDS_DB_PORT         "WDS_DB_PORT"
/* WDS-DB服务器的服务名                                                      */
#define CFG_WDS_DB_NAME         "WDS_DB_NAME"
/* WDS-DB数据库的登录用户名                                                  */
#define CFG_WDS_DB_USER         "WDS_DB_USER"
/* WDS-DB数据库的登录密码                                                    */
#define CFG_WDS_DB_PASS         "WDS_DB_PASS"

/* 交易管理前台service监视端口                                               */
#define CFG_RECV_PORT           "RECV_PORT"
/* 日志文件的保存路径                                                        */
#define CFG_LOG_FOLDER          "LOG_FOLDER_PATH"
/* 日志输出的级别控制                                                        */
#define CFG_LOG_LEVEL           "LOG_LEVEL"
/* 端口收信的超时时间                                                        */
#define CFG_SOCKET_WATE_TIME    "SOCKET_WATE_TIME"

/* 系统最大子线程数                                                          */
#define CFG_MAX_SUB_THREAD      "MAX_SUB_THREAD"
/* 系统可开启的最大Socket连接数                                              */
#define CFG_MAX_SOCKET          "MAX_SOCKET"
/* 系统图像文件夹路径                                                        */
#define CFG_IMAGE_PATH          "IMAGE_PATH"

/* 系统最大Socket连接数                                                      */
#define CONFIG_FILE_PATH        "../config/system_config.ini"

class CSystemConfigInfo{
private:
    char_t    m_chCdrDBIPAddr[IPADDRESS_LENGTH];
                                     /* CDR-DB服务器的IP地址                 */

    int32_t   m_nCdrDBPort;          /* CDR-DB服务器的连接端口号             */
    char_t    m_chCdrDBName[256];    /* CDR-DB服务器的服务名                 */
    char_t    m_chCdrDBUser[256];    /* CDR-DB数据库的登录用户名             */
    char_t    m_chCdrDBPasswd[256];  /* CDR-DB数据库的登录密码               */

    int32_t   m_nRecvPort;           /* 交易管理前台service监视端口          */
    int64_t   m_nSocketWiteTime;     /* 端口的连接超时时间                   */
    int32_t   m_nMaxSubThread;       /* 系统最大子线程数                     */
    int32_t   m_nMaxSocket;          /* 系统可开启的最大Socket连接数         */

    int32_t   m_nLogLevel;           /* 日志输出的级别控制                   */
                                     /* 0: 输出Error级别的日志               */
                                     /* 1: 输出Error, Info级别的日志         */
                                     /* 2: 输出Error, Info, Debug级别的日志  */
                                     /* 3: 输出全部日志                      */
    char_t    m_chLogFilePath[MAX_FILEPATH_LENGTH];
                                     /* 日志文件的保存路径                   */
    char_t    m_chImgFilePath[MAX_FILEPATH_LENGTH];
                                     /* 图像文件的保存路径                   */

public:
    /* 构造函数和析构函数                                                    */
    CSystemConfigInfo();
    ~CSystemConfigInfo();

    /* CDR-DB服务器的连接端口号的Set・Get函数                                 */
    int32_t  GetCdrDBPort()                    { return m_nCdrDBPort;         }
    void SetCdrDBPort(int32_t nPort)           { m_nCdrDBPort = nPort;        }

    /* 端口的连接超时时间的Set・Get函数                                       */
    int64_t  GetSocketWaitTime()               { return m_nSocketWiteTime;    }
    void SetSocketWaitTime(int64_t nTime)      { m_nSocketWiteTime = nTime;   }

    /* 系统最大子线程数的Set・Get函数                                         */
    int32_t  GetMaxSubThread()                 { return m_nMaxSubThread;      }
    void SetMaxSubThread(int32_t nValue)       { m_nMaxSubThread = nValue;    }

    /* 系统可开启的最大Socket连接数Set・Get函数                               */
    int32_t  GetMaxSocket()                    { return m_nMaxSocket;         }
    void SetMaxSocket(int32_t nValue)          { m_nMaxSocket = nValue;       }

    /* CDR-DB服务器的IP地址的Set・Get函数                                     */
    p_char_t GetCdrDBIPAddress()              { return m_chCdrDBIPAddr;       }
    void  SetCdrDBIPAddress(const p_char_t pIPAddr);

    /* CDR-DB服务器的服务名的Set・Get函数                                     */
    p_char_t GetCdrDBName()                   { return m_chCdrDBName;         }
    void  SetCdrDBName(const p_char_t pName);

    /* CDR-DB数据库的登录用户名的Set・Get函数                                 */
    p_char_t GetCdrDBUser()                   { return m_chCdrDBUser;         }
    void  SetCdrDBUser(const p_char_t pUser);

    /* CDR-DB数据库的登录密码的Set・Get函数                                   */
    p_char_t GetCdrDBPasswd()                 { return m_chCdrDBPasswd;       }
    void  SetCdrDBPasswd(const p_char_t pPasswd);

    /* 日志文件的保存路径的Set・Get函数                                       */
    p_char_t GetLogPath()                     { return m_chLogFilePath;       }
    void  SetLogPath(const p_char_t pPath);

    /* 图像数据文件的保存路径的Set・Get函数                                   */
    p_char_t GetImgPath()                     { return m_chImgFilePath;       }
    void  SetImgPath(const p_char_t pPath);

    /* 交易管理前台service监视端口的Set・Get函数                              */
    int32_t  GetRecvPort()                     { return m_nRecvPort;          }
    void SetRecvPort(int32_t nPort)            { m_nRecvPort = nPort;         }

    /* 日志输出的级别控制的Set・Get函数                                       */
    int32_t  GetLogLevel()                     { return m_nLogLevel;          }
    void SetLogLevel(int32_t nLevel)           { m_nLogLevel = nLevel;        }
};

#endif // _SYSTEMCONFIGINFO_H

/******************************************************************************
    End
******************************************************************************/
