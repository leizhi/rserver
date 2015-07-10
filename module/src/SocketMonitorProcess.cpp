/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        端口监控处理类                          */
/*           程序名        ：        SocketMonitorProcess.cpp                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "SocketMonitorProcess.h"
#include "ObjectFactory.h"
#include "PacketAnalysisProcess.h"
#include "ThreadPool.h"

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  CSocketMonitorProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CSocketMonitorProcess::CSocketMonitorProcess(){
    //CObjectFactory* pInstance = CObjectFactory::GetInstance();
    m_pRecvSocket             = NULL;
    m_bBreakFlag              = false;
    m_nSocketPort             = -1;
    m_nFuncType               = -1;
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~CSocketMonitorProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CSocketMonitorProcess::~CSocketMonitorProcess(){
    m_pRecvSocket = NULL;

}

//下面是测试代码  
void *clientprocess(const void *m_pTCPSocket)
{
    try{
	IProcess* m_pProcess = NULL;
	printfs(1, "m_pTCPSocket 0x%04x",m_pTCPSocket);
	m_pProcess = (IProcess*)(new CPacketAnalysisProcess((HTCPSocket*)m_pTCPSocket));
	printfs(1, "m_pProcess 0x%04x",m_pProcess);
	m_pProcess->Do();
	//delete m_pTCPSocket;
	
	delete m_pProcess;
	m_pProcess = NULL;
    }catch(exception e){
    	printfs(1, "myprocess exception");
    }
    return NULL;
}

/******************************************************************************
    处理名        ：  业务处理
    函数名        ：  Do()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
void CSocketMonitorProcess::Do(){
    if (m_nSocketPort == -1){
        printfs(0, "<CSocketMonitorProcess>Channel port error!");
        return;
    }

    printfs(1, "Socket monitor process started! Port.[%d]", m_nSocketPort);

    //HSocketThread*    pThread    = NULL;

    try{
        // 1.开始Socket监控
        if(m_monitorSocket.CreateReceiveSocket( m_nSocketPort ) == 0){
            printfs(0, "Socket monitor create failed Port.[%d]", m_nSocketPort);
            return;
        }

        int             nRet       = 0;
        //int             nCount     = 0;
        char            chName[20] = "\0";
                
        //CObjectFactory* pInstance  = CObjectFactory::GetInstance();
        
        //CObjectContainer<HSocketThread>* pContainer = pInstance->GetThreadContainer();

        /*if (pContainer == NULL){
            printfs(0, "Create thread container failed !");
            return;
        }*/

	pool_init(25);//线程池中最多三个活动线程
	//int i=0;
        while(1){
            // 2.有新的Socket连接到达
            m_pRecvSocket = new HTCPSocket();
            nRet = m_pRecvSocket->Accept(&m_monitorSocket);

            printfs(1, "<CSocketMonitorProcess::Do()> New socket arrived!");

            if( nRet != 1 ) {
                printfs(0, "[%s] [Accept failed]", "CSocketMonitorProcess::Do()");
                continue;
            }

            printfs(2, "<CSocketMonitorProcess::Do()> Socket Accepted!");
            // 3.从Thread container中取得空闲的线程
            //pThread = pContainer->GetObject();

            printfs(2, "<CSocketMonitorProcess::Do()> Thread Getted!");
            // 4.判断Thread指针是否为空
            // 4.1如果指针为空，则说明线程容器中的线程总数已经达到系统上限，
            // 写入这个指针的日志信息，并重新开始Socket监控
            /*if (pThread == NULL){
                printfs(0, "Thread container full!");
                continue;
            }*/

            // 4.2.1指针不为空，分配新的Thread处理这个Socket
            //pThread->Reset();

            printfs(2, "<CSocketMonitorProcess::Do()> Thread Resetted!");

            // 4.2.2重置Thread中的Socket信息
            //pThread->SetTCPSocket(m_pRecvSocket);
            //sprintf(chName, "Thread.%d", pContainer->GetObjectID());
            //pThread->SetName(chName);

            //printf("pThread Create: %s\n",chName);

            printfs(2, "<CSocketMonitorProcess::Do()> RecvSocket has been set to thread!");
            // 4.2.3重新启动这个Thread
            //pThread->Create();
            pool_add_worker(clientprocess, m_pRecvSocket);
            //i++;
            printfs(2, "<CSocketMonitorProcess::Do()> Thread has been created!");
            //nCount++;
        }
    }
    catch(...){
        /*if (pThread != NULL)
        {
            pThread->Reset();
            pThread = NULL;
        }*/
        printfs(0, "[CSocketMonitorProcess::Do()] [Exception occured]");
    }
	//等待所有任务完成
	sleep(1);
	//销毁线程池
	pool_destroy();  

	if (!m_monitorSocket.IsClosed()){
		m_monitorSocket.Close();
	}
}

/******************************************************************************
    处理名        ：  终止当前线程的端口监控操作及子线程操作
    函数名        ：  TerminateProcess()
    参数          ：  无
    返回值        ：  = 0 : 成功
                      < 0 : 失败
******************************************************************************/
int CSocketMonitorProcess::TerminateProcess(){

    return 0;
}

/******************************************************************************
    End
******************************************************************************/
