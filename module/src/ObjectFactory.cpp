/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        ObjectFactory.cpp                       */
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
#include "ObjectFactory.h"

CMutex CObjectFactory::m_sMutex;
CObjectFactory* CObjectFactory::m_pInstance = NULL;
/******************************************************************************
    处理名        ：  本类的构造处理
    函数名        ：  CObjectFactory()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CObjectFactory::CObjectFactory(){
    memset(m_channelConfFile, 0x00, 256);
    memset(m_systemConfFile, 0x00, 256);
    //m_pObjectContainer = NULL;
    m_pSystemConfig    = NULL;
    m_pConfigFile      = NULL;

    m_pWDSDBHelper     = NULL;
    m_pMySQLPool       = NULL;
    InitDirectionMap();
}

/******************************************************************************
    处理名        ：  本类的析构处理
    函数名        ：  ~CObjectFactory()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CObjectFactory::~CObjectFactory(){
    //if(m_pObjectContainer){
    //    delete m_pObjectContainer;
    //    m_pObjectContainer = NULL;
    //}
    if(m_pSystemConfig){
        delete m_pSystemConfig;
        m_pSystemConfig = NULL;
    }
    if(m_pConfigFile){
        delete m_pConfigFile;
        m_pConfigFile = NULL;
    }
    if(m_pWDSDBHelper){
        delete m_pWDSDBHelper;
        m_pWDSDBHelper = NULL;
    }
    if(m_pMySQLPool){
        delete m_pMySQLPool;
        m_pMySQLPool = NULL;
    }
}

void CObjectFactory::InitDirectionMap()
{
    m_mapDirection["N"] = "N";
    m_mapDirection["S"] = "S";
    m_mapDirection["W"] = "W";
    m_mapDirection["E"] = "E";
    m_mapDirection["NNE"] = "NNE";
    m_mapDirection["NEN"] = "NNE";
    m_mapDirection["ENN"] = "NNE";
    m_mapDirection["NE"] = "NE";
    m_mapDirection["EN"] = "NE";
    m_mapDirection["ENE"] = "ENE";
    m_mapDirection["EEN"] = "ENE";
    m_mapDirection["NEE"] = "ENE";
    m_mapDirection["ESE"] = "ESE";
    m_mapDirection["EES"] = "ESE";
    m_mapDirection["SEE"] = "ESE";
    m_mapDirection["SE"] = "SE";
    m_mapDirection["ES"] = "SE";
    m_mapDirection["SSE"] = "SSE";
    m_mapDirection["SES"] = "SSE";
    m_mapDirection["ESS"] = "SSE";
    m_mapDirection["SSW"] = "SSW";
    m_mapDirection["SWS"] = "SSW";
    m_mapDirection["WSS"] = "SSW";
    m_mapDirection["SW"] = "SW";
    m_mapDirection["WS"] = "SW";
    m_mapDirection["WSW"] = "WSW";
    m_mapDirection["WWS"] = "WSW";
    m_mapDirection["SWW"] = "WSW";
    m_mapDirection["WNW"] = "WNW";
    m_mapDirection["WWN"] = "WNW";
    m_mapDirection["NWW"] = "WNW";
    m_mapDirection["NW"] = "NW";
    m_mapDirection["WN"] = "NW";
    m_mapDirection["NNW"] = "NNW";
    m_mapDirection["NWN"] = "NNW";
    m_mapDirection["WNN"] = "NNW";

}

int CObjectFactory::ChangeDirection(char* pDirection)
{
    map<std::string,std::string>::iterator directionIterator = m_mapDirection.find(pDirection);
    if (directionIterator == m_mapDirection.end()){
        return 0;
    }
    strcpy(pDirection, directionIterator->second.c_str());
    return 1;
}

/******************************************************************************
    处理名        ：  ThreadContainer的生成方法
    函数名        ：  GetThreadContainer()
    参数          ：  无
    返回值        ：  ThreadContainer
******************************************************************************/
/*CObjectContainer<HSocketThread>* CObjectFactory::GetThreadContainer(){
    if (!m_pObjectContainer){
        if (!m_pSystemConfig){
            return NULL;
        }
        int nThreadCount = m_pSystemConfig->GetMaxSubThread();
        m_sMutex.TryLock();
        m_pObjectContainer = new CObjectContainer<HSocketThread>(nThreadCount);
        m_sMutex.UnLock();
    }

    return m_pObjectContainer;
}
*/
/******************************************************************************
    处理名        ：  取得系统配置信息
    函数名        ：  GetSystemConfigInfo()
    参数          ：  无
    返回值        ：  系统配置信息
******************************************************************************/
CSystemConfigInfo* CObjectFactory::GetSystemConfigInfo() {
    if (!m_pSystemConfig){
        m_sMutex.TryLock();
        if (!m_pConfigFile){
            m_pConfigFile = new HConfigFile(m_systemConfFile);
            if( m_pConfigFile == NULL ){
                return NULL;
            }
            if(m_pConfigFile->Open("r+") != 0){
                return NULL;
            }
        }
        char_t    chCdrDBIPAddr[IPADDRESS_LENGTH]      = "\0";
                                                 /* CDR-DB服务器的IP地址     */
        int32_t   nCdrDBPort             = 0;    /* CDR-DB服务器的连接端口号 */
        char_t    chCdrDBName[256]       = "\0"; /* CDR-DB服务器的服务名     */
        char_t    chCdrDBUser[256]       = "\0"; /* CDR-DB数据库的登录用户名 */
        char_t    chCdrDBPassword[256]   = "\0"; /* CDR-DB数据库的登录密码   */

        int32_t   nSocketPort     = 0;           /* 交易管理前台service监视端口  */
        int64_t   nSocketWaitTime = 0;           /* 端口的连接超时时间       */
        int32_t   nMaxSubThread   = 0;           /* 系统最大子线程数         */
        int32_t   nMaxSocket      = 0;           /* 系统可开启的最大Socket连接数 */
        char_t    chLogFilePath[MAX_FILEPATH_LENGTH]  = "\0";
                                                 /* 日志文件的保存路径       */
        char_t    chImgFilePath[MAX_FILEPATH_LENGTH]  = "\0";
                                                 /* 图像数据文件的保存路径   */
        int32_t   nLogLevel     = 0;             /* 日志输出的级别控制       */
        /* 通过系统配置文件取得配置信息                                      */
        m_pConfigFile->GetConfInfoString(CFG_WDS_CONFIG_MODE, CFG_WDS_DB_IP        , chCdrDBIPAddr);
        m_pConfigFile->GetConfInfoInt   (CFG_WDS_CONFIG_MODE, CFG_WDS_DB_PORT      , nCdrDBPort);
        m_pConfigFile->GetConfInfoString(CFG_WDS_CONFIG_MODE, CFG_WDS_DB_NAME      , chCdrDBName);
        m_pConfigFile->GetConfInfoString(CFG_WDS_CONFIG_MODE, CFG_WDS_DB_USER      , chCdrDBUser);
        m_pConfigFile->GetConfInfoString(CFG_WDS_CONFIG_MODE, CFG_WDS_DB_PASS      , chCdrDBPassword);
        m_pConfigFile->GetConfInfoLong  (CFG_WDS_CONFIG_MODE, CFG_SOCKET_WATE_TIME , nSocketWaitTime);
        m_pConfigFile->GetConfInfoInt   (CFG_WDS_CONFIG_MODE, CFG_MAX_SUB_THREAD   , nMaxSubThread);
        m_pConfigFile->GetConfInfoInt   (CFG_WDS_CONFIG_MODE, CFG_MAX_SOCKET       , nMaxSocket);
        m_pConfigFile->GetConfInfoInt   (CFG_WDS_CONFIG_MODE, CFG_RECV_PORT        , nSocketPort);
        m_pConfigFile->GetConfInfoInt   (CFG_WDS_CONFIG_MODE, CFG_LOG_LEVEL        , nLogLevel);
        m_pConfigFile->GetConfInfoString(CFG_WDS_CONFIG_MODE, CFG_LOG_FOLDER       , chLogFilePath);
        m_pConfigFile->GetConfInfoString(CFG_WDS_CONFIG_MODE, CFG_IMAGE_PATH       , chImgFilePath);
        /* 将取出的配置信息放入配置信息类                                    */
        m_pSystemConfig = new CSystemConfigInfo;
        m_pSystemConfig->SetCdrDBIPAddress(chCdrDBIPAddr);
        m_pSystemConfig->SetCdrDBPort(nCdrDBPort);
        m_pSystemConfig->SetCdrDBName(chCdrDBName);
        m_pSystemConfig->SetCdrDBUser(chCdrDBUser);
        m_pSystemConfig->SetCdrDBPasswd(chCdrDBPassword);
        m_pSystemConfig->SetSocketWaitTime(nSocketWaitTime);
        m_pSystemConfig->SetMaxSubThread(nMaxSubThread);
        m_pSystemConfig->SetMaxSocket(nMaxSocket);
        m_pSystemConfig->SetRecvPort(nSocketPort);
        m_pSystemConfig->SetLogPath(chLogFilePath);
        m_pSystemConfig->SetLogLevel(nLogLevel);
        m_pSystemConfig->SetImgPath(chImgFilePath);
        if (m_pConfigFile) {
            m_pConfigFile->Close();
            delete m_pConfigFile;
            m_pConfigFile = NULL;
        }
        m_sMutex.UnLock();
    }
    return m_pSystemConfig;
}

/******************************************************************************
    处理名        ：  取得CObjectFactory类的唯一实例
    函数名        ：  GetInstance()
    参数          ：  无
    返回值        ：  CObjectFactory类的唯一实例
******************************************************************************/
CObjectFactory* CObjectFactory::GetInstance(){
    if (NULL == m_pInstance) {
        m_sMutex.TryLock();
        m_pInstance = new CObjectFactory();
        m_sMutex.UnLock();
    }
    return m_pInstance;
}

void CObjectFactory::SetChannelConfFile(char* pFile){
    if ( NULL != pFile ) {
        memcpy(m_channelConfFile, pFile, 256);
    }
}

void CObjectFactory::SetSystemConfFile(char* pFile){
    if ( NULL != pFile ) {
        memcpy(m_systemConfFile, pFile, 256);
    }
}

/******************************************************************************
    处理名        ：  创建并建立TPSDB的连接
    函数名        ：  GetTPSDBHelper()
    参数          ：  无
    返回值        ：  CDR_DBHelper
******************************************************************************/
WDS_DBHelper* CObjectFactory::GetWDSDBHelper(){
    m_sMutex.Lock();
    if (!m_pWDSDBHelper){
        if (!m_pSystemConfig){
            return NULL;
        }

        m_pWDSDBHelper = new WDS_DBHelper(m_pSystemConfig->GetCdrDBIPAddress(),
                                          m_pSystemConfig->GetCdrDBUser(),
                                          m_pSystemConfig->GetCdrDBPasswd(),
                                          m_pSystemConfig->GetCdrDBName(),
                                          m_pSystemConfig->GetCdrDBPort());
        if( (!m_pWDSDBHelper) || (!m_pWDSDBHelper->initConnection())){
            delete m_pWDSDBHelper;
            m_pWDSDBHelper = NULL;
        }
    }

    m_sMutex.UnLock();
    return m_pWDSDBHelper;
}

/******************************************************************************
    处理名        ：  创建并建立TPSDB的连接
    函数名        ：  GetMySQLPool()
    参数          ：  无
    返回值        ：  CMySQLPool
******************************************************************************/
CMySQLPool* CObjectFactory::GetMySQLPool(){

    m_sMutex.Lock();
    if (!m_pMySQLPool){
        if (!m_pSystemConfig){
            return NULL;
        }

        m_pMySQLPool = new CMySQLPool(m_pSystemConfig->GetCdrDBIPAddress(),
                                          m_pSystemConfig->GetCdrDBUser(),
                                          m_pSystemConfig->GetCdrDBPasswd(),
                                          m_pSystemConfig->GetCdrDBName(),
                                          m_pSystemConfig->GetCdrDBPort());
        if( (!m_pMySQLPool) || (!m_pMySQLPool->ConnectDB(m_pSystemConfig->GetMaxSubThread()))){
            delete m_pMySQLPool;
            m_pMySQLPool = NULL;
        }
    }
    m_sMutex.UnLock();

    return m_pMySQLPool;
}

/******************************************************************************
    End
******************************************************************************/
