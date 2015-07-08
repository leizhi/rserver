/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        命令行控制机能                          */
/*           程序名        ：        ChannelItem.cpp                         */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "ChannelItem.h"
#include "ObjectFactory.h"
#include "ProcessFactory.h"

/******************************************************************************
    处理名        ：  本类的构造处理
    函数名        ：  CChannelItem()
    参数          ：  (I) ：pConfigInfo - 频道配置信息
    返回值        ：  无
******************************************************************************/
CChannelItem::CChannelItem(int nSocketPort)
: m_nSocketPort(nSocketPort){
    //m_//pSysLogger = CObjectFactory::GetInstance()->GetSysLogger();

    m_pSocketMonitor       = NULL;    /* 端口监视器                          */
    m_pSocketMonitorThread = NULL;    /* 端口监视器子线程                    */
    m_bStarted             = false;   /* 启动状态标志                        */

    if(m_nSocketPort > 0) {
        m_bCanStart = true;           /* 可否启动标志                        */
    } else {
        m_bCanStart = false;
        printfs(0, "<CChannelItem>Socket port error! Socket port[%d]", m_nSocketPort);
    }
}

/******************************************************************************
    处理名        ：  本类的析构处理
    函数名        ：  ~CChannelItem()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CChannelItem::~CChannelItem(){
    ReleaseThread();
    ReleaseProcess();
}

/******************************************************************************
    处理名        ：  启动处理
    函数名        ：  Start()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
int CChannelItem::Start(){
    printfs(1, "<CChannelItem>Start command received!");
    if(m_bStarted) {
        return 0;
    }
    /* 生成各个子线程的处理类                                                */
    if(CreateProcess() < 0) {
        m_bCanStart = false;
        return -1;
    } 
    /* 生成各个子线程                                                        */
    ThreadCreate();
    /* 启动各个子线程                                                        */
    ThreadStart();
    m_bStarted = true;
    return 0;
}

/******************************************************************************
    处理名        ：  停止处理(本机能暂时未使用)
    函数名        ：  Join()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
int CChannelItem::Join(){
    if ( m_pSocketMonitorThread ) {
        /* 1. 等待子线程停止处理                                             */
        m_pSocketMonitorThread->Join();
        printfs(1, "<CChannelItem>Join channel Thread complete!");
        m_bStarted = false;
    }
    return 0;
}

/******************************************************************************
    处理名        ：  停止处理
    函数名        ：  Stop()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
int CChannelItem::Stop(){
    printfs(1, "<CChannelItem>Stop command received!");
    ThreadStop();
    m_bStarted = false;
    return 0;
}

/******************************************************************************
    处理名        ：  创建处理类
    函数名        ：  CreateProcess()
    参数          ：  无
    返回值        ：   = 0 : 创建处理类成功
                      <> 0 : 创建处理类失败
******************************************************************************/
int CChannelItem::CreateProcess() {
    int nRet = 0;

    CProcessFactory* pFactoryInstance  = NULL;   /* 对象实例工厂             */
    pFactoryInstance = CProcessFactory::GetInstance();
    /* 创建端口监控处理类                                                    */
    if (!m_pSocketMonitor){
        m_pSocketMonitor = (CSocketMonitorProcess*)(pFactoryInstance->GetProcessModule(PRC_TYPE_SOCKET_MONITOR));
        printfs(1, "<CChannelItem>Process create succeed!");
    }
    if (m_pSocketMonitor == NULL){
        nRet = -1;
    }
    /* 向端口监控处理类追加频道配置信息                                      */
    m_pSocketMonitor->SetChannelPort(m_nSocketPort);

    printf("创建处理类 %d\n", nRet);

    return nRet;
}

/******************************************************************************
    处理名        ：  创建子线程处理
    函数名        ：  ThreadCreate()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
void CChannelItem::ThreadCreate(){
    if(m_pSocketMonitor != NULL) {
        m_pSocketMonitorThread = new HProcessThread( (IProcess*)m_pSocketMonitor );
    }
    printfs(1, "<CChannelItem>Create channel monitor process succeed! ");
}

/******************************************************************************
    处理名        ：  启动子线程处理
    函数名        ：  ThreadStart()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
void CChannelItem::ThreadStart(){
    if ( m_pSocketMonitorThread ){
        m_pSocketMonitorThread->Create();
    }
    printfs(1, "<CChannelItem>Create channel monitor thread succeed!");
}

/******************************************************************************
    处理名        ：  停止子线程处理(本机能暂时未使用)
    函数名        ：  ThreadJoin()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
int CChannelItem::ThreadJoin(){
    return 0;
}

/******************************************************************************
    处理名        ：  停止子线程处理
    函数名        ：  ThreadStop()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
void CChannelItem::ThreadStop(){
    if ( m_pSocketMonitorThread ) {
        /* 1. 终止线程中对端口的监控操作，关闭Socket                         */
        m_pSocketMonitor->TerminateProcess();
        /* 2. 停止子线程处理                                                 */
        m_pSocketMonitorThread->Stop();
        printfs(1, "<CChannelItem>Stop channel Thread succeed!");
    }
}

/******************************************************************************
    处理名        ：  释放子线程处理
    函数名        ：  ReleaseThread()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
void CChannelItem::ReleaseThread(){
    if ( m_pSocketMonitorThread != NULL ){
        delete m_pSocketMonitorThread;
        m_pSocketMonitorThread = NULL;
    }
}

/******************************************************************************
    处理名        ：  释放处理过程
    函数名        ：  ReleaseProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
void CChannelItem::ReleaseProcess(){
    if(m_pSocketMonitor != NULL) {
        m_pSocketMonitor = NULL;
    }
}

/******************************************************************************
    End
******************************************************************************/
