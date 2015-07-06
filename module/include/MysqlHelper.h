/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        Mysql操作机能                           */
/*           程序名        ：        MysqlHelper.h                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#ifndef MYSQLHELPER_H_
#define MYSQLHELPER_H_

#include <mysql.h>

#include <string>
#include <iostream>
#include <vector>
#include <list>

#include <string.h>

#include "log.h"

using namespace std;

class MysqlHelper
{
public:
    /*
     * Init MySQL
     * @param hosts:         Host IP address
     * @param userName:      Login UserName
     * @param password:      Login Password
     * @param dbName:        Database Name
     * @param port:          Host listen port number
     */
    MysqlHelper(std::string hosts, std::string userName, std::string password, 
                std::string dbName, unsigned int port);
    MysqlHelper();
    ~MysqlHelper();
    bool initConnection();
    /*    
     * Making query from database
     * @param mysql: MySQL Object
     * @param sql:   Running SQL command
     */
    bool runSQLCommand(std::string sql);
    /**
     * Destroy MySQL object
     * @param mysql MySQL object
     */
    void destroyConnection();
    bool getConnectionStatus();
    vector< vector<string> > getResult();
protected:
    void setUserName(std::string userName);
    void setHosts(std::string hosts);
    void setPassword(std::string password);
    void setDBName(std::string dbName);
    void setPort(unsigned int port);

protected:
    bool IsConnected;
    vector< vector<string> > resultList;

    MYSQL* m_pSQLClient;
    bool   m_bCreateByHandle;
private:
    unsigned int DEFAULTPORT;
    char * HOSTS;
    char * USERNAME;
    char * PASSWORD;
    char * DBNAME;
    //CLog * m_pSysLogger;
};

#endif /* MYSQLHELPER_H_ */ 

/*****************************************************************************
    End
*****************************************************************************/

