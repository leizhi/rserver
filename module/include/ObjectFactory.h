/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        ObjectFactory.h                         */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#ifndef _COBJECTFACTORY_H_
#define _COBJECTFACTORY_H_

/*****************************************************************************
    用户 inlcude
******************************************************************************/
#include    "VodUserType.h"
#include    "Mutex.h"
#include    "log.h"
#include    "ConfigFile.h"
#include    "SystemConfigInfo.h"
//#include    "SocketThread.h"
//#include    "ObjectContainer.h"
#include    "WDS_DBHelper.h"
#include    "MySQLPool.h"

#include <map>
#include <iostream>

using namespace std;
  
class CObjectFactory {
public:
    CObjectFactory();                                        /*  */
    ~CObjectFactory();                                       /*  */

    static CObjectFactory*        GetInstance();             /*  */

    CSystemConfigInfo*            GetSystemConfigInfo();     /*  */
    //CLog*                  GetSysLogger();            /*  */

    //CObjectContainer<HSocketThread>* GetThreadContainer();   /*  */

    char* GetChannelConfFile() { return m_channelConfFile; }
    char* GetSystemConfFile() { return m_systemConfFile; }
    void  SetChannelConfFile(char* pFile);
    void  SetSystemConfFile(char* pFile);
    WDS_DBHelper* GetWDSDBHelper();
    CMySQLPool* GetMySQLPool();

    int ChangeDirection(char* pDirection);

private:
    static CObjectFactory* m_pInstance;                      /*  */
    static CMutex m_sMutex;                                  /*  */

    //CObjectContainer<HSocketThread>* m_pObjectContainer;     /*  */

    CSystemConfigInfo*            m_pSystemConfig;
    //CLog*                  m_pSysLogger;
    HConfigFile*                  m_pConfigFile;
    WDS_DBHelper*                 m_pWDSDBHelper;
    CMySQLPool*                   m_pMySQLPool;

    char               m_channelConfFile[256];
    char               m_systemConfFile[256];
    std::map<std::string, std::string> m_mapDirection;

private:
    void InitDirectionMap();
};

#endif /*COBJECTFACTORY_H_*/

/*****************************************************************************
    End
*****************************************************************************/
