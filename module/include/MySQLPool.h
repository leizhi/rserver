#ifndef MYSQLPOOL_H_
#define MYSQLPOOL_H_

#include <mysql.h>
#include <list>
#include "Mutex.h"
#include "log.h"

typedef std::list<MYSQL *> CONNECTION_HANDLE_LIST;
typedef std::list<MYSQL *>::iterator ITER_CONNECTION_HANDLE_LIST;

#define CONNECTION_NUM 10 //同时打开的连接数

class CMySQLPool
{
    public:
    CMySQLPool();
    CMySQLPool(const char *host, const char *user, const char *password, const char *db, unsigned int port=3306);
    virtual ~CMySQLPool();

    public:
    bool ConnectDB(int nCount = CONNECTION_NUM);      //连接数据库
    MYSQL* GetIdleMySql();     //提取一个空闲句柄供使用
    void SetIdleMysql(MYSQL* pMySql);  //从使用队列中释放一个使用完毕的句柄，插入到空闲队列

    public:
    //创建两个队列
    CONNECTION_HANDLE_LIST m_lsBusyList;                //正在使用的连接句柄
    CONNECTION_HANDLE_LIST m_lsIdleList;                //未使用的连接句柄

    public:
    char m_host[20];   //主机
    char m_user[20];   //用户名
    char m_password[20];  //密码
    char m_db[20];    //数据库名
    unsigned int m_port;  //端口

    private:
    CMutex sMutex;
};
#endif /* MYSQLPOOL_H_ */ 

