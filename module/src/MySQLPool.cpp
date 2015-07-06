#include "MySQLPool.h"
#include "ObjectFactory.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CMySQLPool::CMySQLPool()
{

}

CMySQLPool::CMySQLPool(const char *host, const char *user, const char *password, const char *db, unsigned int port/* =3306 */)
{
    //m_//pSysLogger     = CObjectFactory::GetInstance()->GetSysLogger();
    strcpy(m_host, host);
    strcpy(m_user, user);
    strcpy(m_password, password);
    strcpy(m_db, db);
    m_port = port;
}

CMySQLPool::~CMySQLPool()
{
    for (ITER_CONNECTION_HANDLE_LIST iter=m_lsBusyList.begin(); iter != m_lsBusyList.end(); iter++){
        mysql_close((*iter));
    }

    for (ITER_CONNECTION_HANDLE_LIST iter=m_lsIdleList.begin(); iter != m_lsIdleList.end(); iter++){
        mysql_close((*iter));
    }
}

bool CMySQLPool::ConnectDB(int nCount)
{
    //同时打开CONNECTION_NUM个连接
    try{
        for (int i=0; i<nCount; ++i){
            MYSQL *pMySql = mysql_init((MYSQL*)NULL);
            if (pMySql != NULL){
                my_bool auto_reconnect = 1;
                mysql_options(pMySql, MYSQL_OPT_RECONNECT, &auto_reconnect);
                if (!mysql_real_connect(pMySql,m_host,m_user,m_password,m_db,m_port,NULL,CLIENT_MULTI_STATEMENTS|CLIENT_INTERACTIVE)){
                    //m_//pSysLogger->Add(0,"<CMySQLPool::ConnectDB()> Error connection to database: %s!", mysql_error(pMySql));
                    return false;
                }
                mysql_set_character_set(pMySql, "utf8");
                //m_//pSysLogger->Add(2,"<CMySQLPool::ConnectDB()> push pMySql[%p]!", pMySql);
                m_lsIdleList.push_back(pMySql);
            }
        }
    }
    catch (...){
        //m_//pSysLogger->Add(0,"<CMySQLPool::ConnectDB()> Unknow error occured while connection to database!");
        return false;
    }
    return true;
}

MYSQL* CMySQLPool::GetIdleMySql()
{
    MYSQL* pMySql = NULL;
    sMutex.Lock();
    if (m_lsIdleList.size() > 0){
        pMySql = m_lsIdleList.front();
        m_lsIdleList.pop_front();
        m_lsBusyList.push_back(pMySql);
    }else{
        pMySql = NULL;
    }
    sMutex.UnLock();

    //m_//pSysLogger->Add(2,"<CMySQLPool::GetIdleMySql()> return pMySql:[%p]!", pMySql);
    return pMySql;
}

void CMySQLPool::SetIdleMysql(MYSQL* pMySql)
{
    if (pMySql == NULL){
        return;
    }
    //m_//pSysLogger->Add(2,"<CMySQLPool::SetIdleMysql()> input pMySql:[%p]!", pMySql);
    sMutex.Lock();
    m_lsBusyList.remove(pMySql);
    m_lsIdleList.push_back(pMySql);
    sMutex.UnLock();
}

