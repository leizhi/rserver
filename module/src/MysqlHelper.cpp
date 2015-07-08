/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        Mysql数据库处理类                       */
/*           程序名        ：        MysqlHelper.cpp                         */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "MysqlHelper.h"
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
MysqlHelper::MysqlHelper(string hosts, string userName, string password, 
                         string dbName, unsigned int port)
{
    IsConnected = false;
    this->setHosts(hosts);        // 设置主机IP地址
    this->setUserName(userName);  // 设置登录用户名
    this->setPassword(password);  // 设置登录密码
    this->setDBName(dbName);      // 设置数据库名
    this->setPort(port);          // 设置端口号
    m_bCreateByHandle = false;
    m_pSQLClient = NULL;
}

MysqlHelper::MysqlHelper()
{
    m_bCreateByHandle = true;
    IsConnected       = true;
    m_pSQLClient      = NULL;
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~MysqlHelper()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
MysqlHelper::~MysqlHelper()
{
    if (!m_bCreateByHandle)
        this ->destroyConnection();
}

/******************************************************************************
    处理名        ：  设置数据库名称
    函数名        ：  setDBName()
    参数          ：  string dbName 数据库名
    返回值        ：  无
******************************************************************************/
void MysqlHelper::setDBName(string dbName)
{
    if ( dbName.empty() )
    {// 用户没有指定数据库名
        printfs(1,"<MysqlHelper::setDBName()> DBName is null! Used default value: mysql");
        this ->DBNAME = new char[5];
        strcpy(this ->DBNAME, "mysql");
    }
    else
    {
        this ->DBNAME = new char[dbName.length()];
        strcpy(this ->DBNAME, dbName.c_str());
    }
}

/******************************************************************************
    处理名        ：  设置数据库IP地址
    函数名        ：  setHosts(string hosts)
    参数          ：  string hosts 数据库IP地址
    返回值        ：  无
******************************************************************************/
void MysqlHelper::setHosts(string hosts)
{
    if ( hosts.empty() )
    {// 用户没有指定数据库IP地址    
        printfs(1,"<MysqlHelper::setHosts()> Hosts is null! Used default value: localhost");
        this ->HOSTS = new char[9];
        strcpy(this ->HOSTS, "localhost");
    }
    else
    {
        this ->HOSTS = new char[hosts.length()];
        strcpy(this ->HOSTS, hosts.c_str());
    }
}

/******************************************************************************
    处理名        ：  设置数据库登陆密码
    函数名        ：  setPassword()
    参数          ：  string password 登陆密码
    返回值        ：  无
******************************************************************************/
void MysqlHelper::setPassword(string password)
{// 用户没有指定密码
    if ( password.empty() )
    {
        printfs(1,"<MysqlHelper::setPassword()> Password is null! Used default value: ");
        this ->PASSWORD = new char[1];
        strcpy(this ->PASSWORD, "");
    }
    else
    {
        this ->PASSWORD = new char[password.length()];
        strcpy(this ->PASSWORD, password.c_str());
    }
}

/******************************************************************************
    处理名        ：  设置端口号
    函数名        ：  setPort()
    参数          ：  unsigned int port 端口号
    返回值        ：  无
******************************************************************************/
void MysqlHelper::setPort(unsigned int port)
{// 用户没有指定端口号，使用默认端口号
    if ( !port )
    {
        printfs(1,"<MysqlHelper::setPort()> Port is null! Used default value: 0");
        this ->DEFAULTPORT = 0;
    }
    else
    {
        this ->DEFAULTPORT = port;
    }
}

/******************************************************************************
    处理名        ：  设置登陆用户名
    函数名        ：  setUserName()
    参数          ：  string userName 登陆用户名
    返回值        ：  无
******************************************************************************/
void MysqlHelper::setUserName(string userName)
{// 用户没有指定登录用户名
    if ( userName.empty() )
    {
        printfs(1,"<MysqlHelper::setUserName()> UserName is null! Used default value: root");
        this ->USERNAME = new char[4];
        strcpy(this ->USERNAME, "root");
    }
    else
    {
        this ->USERNAME = new char[userName.length()];
        strcpy(this ->USERNAME, userName.c_str());
    }
}

/******************************************************************************
    处理名        ：  初始化并连接数据库连接
    函数名        ：  initConnection()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
bool MysqlHelper::initConnection()
{
    if ( IsConnected )
    {// 已经连接到服务器
        printfs(0,"<MysqlHelper::initConnection()> Allready connected to server!");
        return false;
    }
    m_pSQLClient = mysql_init((MYSQL*)NULL);// 初始化相关对象
    my_bool auto_reconnect = 1;
    mysql_options(m_pSQLClient, MYSQL_OPT_RECONNECT, &auto_reconnect);
    if ( !mysql_real_connect( m_pSQLClient, HOSTS, USERNAME, PASSWORD, DBNAME,
                              DEFAULTPORT, NULL, CLIENT_MULTI_STATEMENTS|CLIENT_INTERACTIVE) )
    {// 连接到服务器
        printfs(0,"<MysqlHelper::initConnection()> Error connection to database: %s!", mysql_error(m_pSQLClient));
        return false;
    }
    // 设置插入数据的编码格式为“utf8”
    mysql_set_character_set(m_pSQLClient, "utf8");
    IsConnected = true;// 修改连接标识

    return true;
}

/******************************************************************************
    处理名        ：  执行指定SQL文
    函数名        ：  runSQLCommand()
    参数          ：  string sql 指定SQL文
    返回值        ：  true:执行成功  false:执行失败
******************************************************************************/
bool MysqlHelper::runSQLCommand(string sql)
{
    if ( !IsConnected )
    {// 没有连接到服务器
        printfs(0,"<MysqlHelper::runSQLCommand()> Not connect to database!");
        return false;
    }
    if ( sql.empty() )
    {// SQL语句为空
        printfs(0,"<MysqlHelper::runSQLCommand()> SQL is null!");
        return false;
    }

    MYSQL_RES *res;

    unsigned int i = 0;
    //unsigned int i,j = 0;
    try{
        mysql_ping(m_pSQLClient);

        i = mysql_real_query(m_pSQLClient,sql.c_str(),(unsigned int)strlen(sql.c_str()));// 执行查询
        if ( i )
        {
            printfs(0,"<MysqlHelper::runSQLCommand()> Error query from database: %s", mysql_error(m_pSQLClient));
            //std::cout << "Error query from database: %s\n" << mysql_error(&mySQLClient) << std::endl;
            return false;
        }
        
        do
        {/* Process all results */
            //printf("total affected rows: %lld\n", mysql_affected_rows(&mySQLClient));
            if (!(res= mysql_store_result(m_pSQLClient)))
                break;
            mysql_free_result(res);
        } while (!mysql_next_result(m_pSQLClient));
    }
    catch(...){
        printfs(0,"<MysqlHelper::runSQLCommand()> An unknown error occured during query : %s", sql.c_str());
        return false;
    }

    return true;
}

/******************************************************************************
    处理名        ：  返回查询结果
    函数名        ：  getResult()
    参数          ：  无
    返回值        ：  查询结果集
******************************************************************************/
vector< vector<string> > MysqlHelper::getResult()
{
    return resultList;
}

/******************************************************************************
    处理名        ：  关闭与数据库的连接
    函数名        ：  destroyConnection()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
void MysqlHelper::destroyConnection()
{
    mysql_close(m_pSQLClient);
    this ->IsConnected = false;
}

/******************************************************************************
    处理名        ：  当前连接状态
    函数名        ：  getConnectionStatus()
    参数          ：  无
    返回值        ：  true: 连接中  false:连接中断
******************************************************************************/
bool MysqlHelper::getConnectionStatus()
{
    return IsConnected;
}
