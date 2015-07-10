/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        TCP处理机能                             */
/*           程序名        ：        TCPSocket.cpp                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/
#include "TCPSocket.h"

#include <netdb.h>
#include <unistd.h>
#include <netinet/tcp.h>
#include <iostream>
#include <assert.h>
#include <errno.h>
#include <sys/ioctl.h> 
#include <net/if.h> 
#include "ObjectFactory.h"

using std::cout;
using std::endl;

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  HTCPSocket()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
HTCPSocket::HTCPSocket(){
    m_bInitTCP = false;
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~HTCPSocket()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
HTCPSocket::~HTCPSocket(){
    if (m_bInitTCP) {
        Close();
    }
}

/******************************************************************************
    处理名        ：  当前TCPSocket是否已经关闭的判断处理
    函数名        ：  IsClosed()
    参数          ：  无
    返回值        ：  true  ： 当前TCPSocket已经关闭
                      false ： 当前TCPSocket没有关闭
******************************************************************************/
bool HTCPSocket::IsClosed(){
    return (!m_bInitTCP || m_nsocket_tcp <= 0);
}

/******************************************************************************
    处理名        ：  打开指定的TCPSocket
    函数名        ：  Open()
    参数          ：  无
    返回值        ：  1  ： TCPSocket打开成功
                      0  ： TCPSocket打开失败
******************************************************************************/
int HTCPSocket::Open(){
    if(m_bByIPv6) {
        return(((m_nsocket_tcp=socket(PF_INET6,SOCK_STREAM,0))<0)?0:1);
    } else {
        return(((m_nsocket_tcp=socket(PF_INET,SOCK_STREAM,0))<0)?0:1);
    }
}

/******************************************************************************
    处理名        ：  链接到指定的IP和Port
    函数名        ：  Connect(char* pIPAddr, int nPort)
    参数          ：  nPort   : 连接Port
                      pIPAddr : 连接地址
    返回值        ：  1  ： 连接成功
                      0  ： 连接失败
                     -1  ： 连接超时
******************************************************************************/
int HTCPSocket::Connect(char* pIPAddr, int nPort) {
    int Result;
    struct protoent *TCPent;
    int optval = 1;
    WriteLog(2, "[%s][%s]", "Connect", "Start");
    if(m_bInitTCP){return 0;}    //
    SetSocketAddr(pIPAddr, nPort);
    /* IPv6格式的IP地址和Port设定                                            */
    if(m_bByIPv6) { 
        Result = ::connect(m_nsocket_tcp, (sockaddr*)&m_SockAddr6, sizeof(m_SockAddr6));
    }
    /* IPv4格式的IP地址和Port设定                                            */
    else{
        Result = ::connect(m_nsocket_tcp, (sockaddr*)&m_SockAddr, sizeof(m_SockAddr));
    }
    if (Result < 0) {
        WriteLog(0, "Connect.[%s]", "connect");
            return 0;
    }
    // Set the TCP_NODELAY option(Client Side)
    TCPent = getprotobyname("tcp");
    if (TCPent == NULL) {
        WriteLog(0, "Connect[%s]", "getprotobyname");
        return -1;
    }
    // 只要系统缓冲区有数据就立刻发送
    if (setsockopt(m_nsocket_tcp,
                    TCPent->p_proto,
                    TCP_NODELAY,
                    (char *) &optval,
                    sizeof(optval))) {
        WriteLog(0, "Connect[%s]", "setsockopt TCP_NODELAY");
        return -1;
    }
    WriteLog(2, "Connect[%s]", "End.");
    return 1;
}

/******************************************************************************
    处理名        ：  对打开的连接进行监听
    函数名        ：  Listen(unsigned long howMuch)
    参数          ：  howMuch : 连接个数
    返回值        ：  0  ： 监听无内容
                      1  ： 监听有内容
******************************************************************************/
int HTCPSocket::Listen(unsigned long howMuch){
    if(m_bInitTCP){return 0;}
    WriteLog(2, "Listen[%s]", "Start");
    return ::listen(m_nsocket_tcp, howMuch) >= 0;
}

/******************************************************************************
    处理名        ：  生成本地连接Socket
    函数名        ：  Accept(HTCPSocket *pServer)
    参数          ：  pServer : 本地连接Socket的指针
    返回值        ： -1  ： 连接异常
                      0  ： 连接超时
                      1  ： 连接正常返回
******************************************************************************/
int HTCPSocket::Accept(HTCPSocket *pServer){
    struct protoent *TCPent;
    
    int    optval = 1;
    socklen_t    l;
    
    if(m_bInitTCP){return -1;}
    
    if(pServer == NULL){return -1;}
    
    if(pServer->m_nsocket_tcp <= 0) {
        return -1;
    }

    WriteLog(2, "Accept[%s]", "Start");
    if(m_bByIPv6) {
        l = sizeof(m_SockAddr6);
        m_nsocket_tcp = ::accept(pServer->m_nsocket_tcp, (sockaddr*)&m_SockAddr6, &l);
    } else {
        l = sizeof(m_SockAddr);
        m_nsocket_tcp = ::accept(pServer->m_nsocket_tcp, (sockaddr*)&m_SockAddr, &l);
    }

    if (m_nsocket_tcp == -1) {
        if( errno == EAGAIN ) {
            WriteLog(0, "Accept[connection timeout])");
            return 0;
        } else {
            WriteLog(0, "Accept[connection error]");
            return -1;
        }
    }

    // Set the TCP_NODELAY option(Server Side)
    TCPent = getprotobyname("tcp");
    if (TCPent == NULL) {
        WriteLog(0, "Accept[%s]", "getprotobyname");
        return 0;
    }
    if (setsockopt(m_nsocket_tcp,
                        TCPent->p_proto,
                        TCP_NODELAY,
                        (char *) &optval,
                        sizeof(optval))) {
        WriteLog(0, "Accept[%s]", "setsockopt TCP_NODELAY");
        return 0;
    }
    m_bInitTCP = true;
    WriteLog(2, "Accept[%s]", "End");
    return 1;
}

/******************************************************************************
    处理名        ：  绑定连接完成的Socket
    函数名        ：  Bind(char* pIPAddr, int nPort)
    参数          ：  nPort   : 连接Port
                      pIPAddr : 连接地址
    返回值        ：  0  ： 绑定失败
                      1  ： 绑定成功
******************************************************************************/
int HTCPSocket::Bind(char* pIPAddr, int nPort) {
    long Length, Result;
    int  loop = 1;
    if(m_bInitTCP){return 0;}
    
    //WriteLog(2, "Bind[%s]", "Start");
    
    SetSocketAddr(pIPAddr, nPort);
    if(setsockopt(m_nsocket_tcp, SOL_SOCKET, SO_REUSEADDR, 
                  &loop, sizeof(loop)) < 0)
    { 
        WriteLog(0, "setsockopt:SO_REUSEADDR");
        return 0;
    }
    if(m_bByIPv6) {
        Length = sizeof(m_SockAddr6);
        Result = ::bind(m_nsocket_tcp, (sockaddr *)&m_SockAddr6, Length);
    } else {
        Length = sizeof(m_SockAddr);
        Result = ::bind(m_nsocket_tcp, (sockaddr *)&m_SockAddr, Length);
    }
    if(Result < 0) {
        WriteLog(0, "[%s]", "bind");
        return 0;
    }
    //WriteLog(2, "Bind[%s]", "End");
    return 1;
}

/******************************************************************************
    处理名        ：  关闭连接
    函数名        ：  Close()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
void HTCPSocket::Close(){
    WriteLog(2, "Close()");
    if (m_nsocket_tcp > 0) {
        ::shutdown(m_nsocket_tcp, 2);
        if(::close(m_nsocket_tcp) < 0) {
            WriteLog(0, "Close()");
        }
        m_bInitTCP = false;
    }
}

/******************************************************************************
    处理名        ：  检查指定的IP地址格式
    函数名        ：  CheckIPAddr(char* pIPAddr)
    参数          ：  pIPAddr : 检查地址
    返回值        ： -1  ： 检查失败
                      0  ： 指定的IP地址格式为IPv4格式
                      1  ： 指定的IP地址格式为IPv6格式
******************************************************************************/
int HTCPSocket::CheckIPAddr(char* pIPAddr)
{
    if(pIPAddr == NULL) {
        return -1;
    }
    char pNewIPAddr[120] = "\0";
    /* IP地址的格式是否为IPv6的判断                                          */
    if( inet_pton(AF_INET6, pIPAddr, pNewIPAddr) == 1 ) {
        m_bByIPv6 = true;
        return 1;
    }
    /* IP地址的格式是否为IPv4的判断                                          */
    else if( inet_pton(AF_INET, pIPAddr, pNewIPAddr) == 1 ) {
        m_bByIPv6 = false;
        return 0;
    }
    else {
        WriteLog(0, "CheckIPAddr[%s]", "inet_pton");
        return -1;    /* IP地址错误 */
    }
}

/******************************************************************************
    处理名        ：  设置端口地址
    函数名        ：  SetSocketAddr(char* pIPAddr, int nPort)
    参数          ：  nPort   : 连接Port
                      pIPAddr : 连接地址
    返回值        ： 无
******************************************************************************/
void HTCPSocket::SetSocketAddr(char* pIPAddr, int nPort) {
    printf("m_bByIPv6: %d\n",m_bByIPv6);
    
    if(m_bByIPv6) { /* IPv6格式的端口情报设定 */
        bzero(&m_SockAddr6, sizeof(m_SockAddr6));
        m_SockAddr6.sin6_family = PF_INET6;     // IPv6
        m_SockAddr6.sin6_port = htons(nPort);  // IPv6
        inet_pton(AF_INET6, pIPAddr, &m_SockAddr6.sin6_addr);  // IPv6
    } else {    /* IPv4格式的端口情报设定 */
        bzero(&m_SockAddr, sizeof(m_SockAddr));
        m_SockAddr.sin_family = PF_INET;      // IPv4
        m_SockAddr.sin_port = htons(nPort);  // IPv4
        m_SockAddr.sin_addr.s_addr = inet_addr(pIPAddr);  // IPv4
    }
}

/******************************************************************************
    处理名        ：  端口监控和网络数据包送信Socket生成处理
    函数名        ：  CreateSendSocket(char* pOutIPAddr, int nOutPort)
    参数          ：  nPort   : 连接Port
                      pIPAddr : 连接地址
    返回值        ： true  ： 成功
                     false ： 失败
******************************************************************************/
bool HTCPSocket::CreateSendSocket(char* pOutIPAddr, int nOutPort) {
    return CreateSendSocket(pOutIPAddr, nOutPort, 0);
}

/******************************************************************************
    处理名        ：  端口监控和网络数据包送信Socket生成处理
    函数名        ：  CreateSendSocket(char* pOutIPAddr, int nOutPort, int nTimeout)
    参数          ：  nPort    : 连接Port
                      pIPAddr  : 连接地址
                      nTimeout : 超时时间
    返回值        ： true  ： 成功
                     false ： 失败
******************************************************************************/
bool HTCPSocket::CreateSendSocket(char* pOutIPAddr, 
                                  int nOutPort,
                                  int nTimeout) {
    if(m_bInitTCP){return false;}
    WriteLog(2, "CreateSendSocket[Port:%d][IP:%s]", nOutPort, pOutIPAddr);
    
    if(CheckIPAddr(pOutIPAddr) < 0) {
        WriteLog(0, "[%s][%s][%d]", "CheckIPAddr()", pOutIPAddr, nOutPort);
        return false;
    }
    if (!Open()) {
        WriteLog(0, "[%s]", "Open()");
        return false;
    }
    if (!Connect(pOutIPAddr, nOutPort)) {
        WriteLog(0, "[%s][%s][%d]", "Connect()", pOutIPAddr, nOutPort);
        return false;
    }
    TimeoutTCP(nTimeout, false);
    m_bInitTCP = true;
    WriteLog(2, "CreateSendSocket[%s]", "End");
    return true;
}

/******************************************************************************
    处理名        ：  端口监控和网络数据包受信Socket生成处理
    函数名        ：  CreateReceiveSocket(int nInPort)
    参数          ：  nInPort  : 连接Port
    返回值        ： true  ： 成功
                     false ： 失败
******************************************************************************/
bool HTCPSocket::CreateReceiveSocket(int nInPort) {
    return CreateReceiveSocket(LOCALHOST_LISTEN_IP, nInPort, 0);
}

/******************************************************************************
    处理名        ：  端口监控和网络数据包受信Socket生成处理
    函数名        ：  CreateReceiveSocket(int nInPort, int nTimeout)
    参数          ：  nInPort  : 连接Port
                      nTimeout : 超时时间
    返回值        ： true  ： 成功
                     false ： 失败
******************************************************************************/
bool HTCPSocket::CreateReceiveSocket(int nInPort, int nTimeout) {
    return CreateReceiveSocket(LOCALHOST_LISTEN_IP, nInPort, nTimeout);
}

/******************************************************************************
    处理名        ：  端口监控和网络数据包受信Socket生成处理
    函数名        ：  CreateReceiveSocket(char* pInIPAddr, int nInPort)
    参数          ：  nInPort   : 连接Port
                      pInIPAddr : 连接IP地址
    返回值        ： true  ： 成功
                     false ： 失败
******************************************************************************/
bool HTCPSocket::CreateReceiveSocket(char* pInIPAddr, int nInPort) {
    return CreateReceiveSocket(pInIPAddr, nInPort, 0);
}

/******************************************************************************
    处理名        ：  端口监控和网络数据包受信Socket生成处理
    函数名        ：  CreateReceiveSocket(char* pInIPAddr, int nInPort, int nTimeout)
    参数          ：  nInPort   : 连接Port
                      pInIPAddr : 连接IP地址
                      nTimeout  : 超时时间
    返回值        ： true  ： 成功
                     false ： 失败
******************************************************************************/
bool HTCPSocket::CreateReceiveSocket(char* pInIPAddr, int nInPort, int nTimeout) {
    
    printf("CreateReceiveSocket m_bInitTCP:%d pInIPAddr:%s nInPort:%d nTimeout:%d\n",m_bInitTCP,pInIPAddr,nInPort,nTimeout);
    
    if(m_bInitTCP){return false;}
    
    WriteLog(2, "CreateReceiveSocket[Port:%d][IP:%s]", nInPort, pInIPAddr);
    if(CheckIPAddr(pInIPAddr) < 0) {
        WriteLog(0, "[%s]", "CheckIPAddr()");
        return false;
    }
    printf("CheckIPAddr()\n");
    
    if (!Open()) {
        WriteLog(0, "[%s]", "Open()");
        return false;
    }
    printf("Open()\n");
    if (!Bind(pInIPAddr, nInPort)) {
        WriteLog(0, "[%s]", "Bind()");
        return false;
    }
    printf("Bind()\n");
    TimeoutTCP(nTimeout, true);
    if (!Listen(5)) {
        WriteLog(0, "[%s]", "Listen()");
        return false;
    }
    printf("Listen()\n");
    
    WriteLog(2, "CreateReceiveSocket[%s]", "End");
    m_bInitTCP = true;
    return true;
}

/******************************************************************************
    处理名        ：  送信处理
    函数名        ：  Send(const char *buffer, size_t size)
    参数          ：  buffer   : 送信数据缓冲区
                      size     : 送信字节数
    返回值        ： -1 ： 送信失败
                      0 ： 送信失败-超时
                      1 ： 成功
******************************************************************************/
int HTCPSocket::Send(const char *buffer, size_t size){
    long total_sent = 0;
    long expected_size = size;
    if(!m_bInitTCP){return -1;}
    WriteLog(2, "Send: Beginning to send TCP message...(Size %ld)", size);
    while (total_sent < expected_size) {
        int sent = ::send(m_nsocket_tcp, buffer + total_sent,
                          expected_size - total_sent, 0);
        if (sent <= 0) {
            if( errno == EAGAIN ) {
                WriteLog(0, "[%s]", "Timeout");
                return 0;
            } else {
                WriteLog(0, "[%s]", "send");
                return -1;
            }
        }
        total_sent += sent;
    }
    WriteLog(2, "Sent %ld bytes out of %ld.", 
             total_sent, expected_size);
    return 1;
}

/******************************************************************************
    处理名        ：  受信处理
    函数名        ：  Receive(void *buffer, unsigned long size)
    参数          ：  buffer   : 受信数据缓冲区
                      size     : 受信字节数
    返回值        ： -1 ： 受信失败
                      0 ： 受信失败-超时
                      1 ： 成功
******************************************************************************/
int HTCPSocket::Receive(void *buffer, long size) {
    if(!m_bInitTCP){return -1;}
    long nReceived = 0;
    long RBLength = 0;
    WriteLog(2, "Beginning to receive TCP message...(Size %ld)", size);
    while (RBLength < size)
    {
        nReceived = recv(m_nsocket_tcp, 
                        (char *) buffer + RBLength, size - RBLength, 0);
        if (nReceived <= 0) {
            if( errno == EAGAIN ) {
                WriteLog(0, "[%s]", "Receive Timeout");
                return 0;
            } else {
                WriteLog(0, "[%s]", "recv");
                return -1;
            }
        }
        RBLength += nReceived;
        if (nReceived <= 0) {
            if( errno == EAGAIN ) {
                WriteLog(0, "[%s]", "Receive Timeout");
                return 0;
            } else {
                WriteLog(0, "[%s]", "recv");
                return -1;
            }
        }
    }
    WriteLog(2, "Received %ld bytes out of %ld.", RBLength, size);
    return 1;
}

/******************************************************************************
    处理名        ：  设定TCP送信超时时间
    函数名        ：  TimeoutTCP(int nSec, bool isRecive)
    参数          ：  nSec     : 超时时间
                      isRecive : 是否接收状态
    返回值        ： 无
******************************************************************************/
void HTCPSocket::TimeoutTCP(int nSec, bool isRecive){
    if(nSec <= 0) {
        return;
    }
    struct timeval timeo;
    socklen_t len = sizeof(timeo);
    timeo.tv_sec  = nSec;
    timeo.tv_usec = 0;
    if(isRecive) {
        /* Socket送信缓冲区的Size设定                                        */
        setsockopt(m_nsocket_tcp, SOL_SOCKET, SO_RCVTIMEO, &timeo, len);
    } else {
        /* Socket送信缓冲区的Size设定                                        */
        setsockopt(m_nsocket_tcp, SOL_SOCKET, SO_SNDTIMEO, &timeo, len);
    }
}

/******************************************************************************
    处理名        ：  按照fmt参数的内容进行日志输出处理
    函数名        ：  Add (unsigned int nlevel, const char * fmt, ...)
    参数          ：  (I) : nlevel 执行日志的级别(警告/情报/调试/全部)
                      (I) : fmt    日志输出的内容
    返回值        ：  无
******************************************************************************/
void HTCPSocket::WriteLog(int nlevel, const char * fmt, ...) {
}
/******************************************************************************
    End
******************************************************************************/

