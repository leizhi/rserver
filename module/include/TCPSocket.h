/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        TCPSocket.h                             */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "Socket.h"
#include "log.h"

#if !defined(_HTCPSOCKET_H)
#define _HTCPSOCKET_H

#define LOCALHOST_LISTEN_IP "0.0.0.0"
#define MAX_LESTENCOUNT = 5

class HTCPSocket : public BSocket{
private:
    long    m_nsocket_tcp;
    bool    m_bInitTCP;
    bool    m_bByIPv6;
    struct  sockaddr_in  m_SockAddr;
    struct  sockaddr_in6 m_SockAddr6;


    int  Open();
    int  Connect(char* pIPAddr, int nPort);
    int  Listen(unsigned long howMuch=5);
    int  Bind(char* pIPAddr, int nPort);
    int  CheckIPAddr(char* pIPAddr);
    void SetSocketAddr(char* pIPAddr, int nPort);
    void TimeoutTCP(int nSec, bool isRecive);
public :
    HTCPSocket();
    virtual ~HTCPSocket();
    virtual void Close();
    bool    IsClosed();

    bool CreateSendSocket(char* pOutIPAddr, int nOutPort);
    bool CreateSendSocket(char* pOutIPAddr, int nOutPort, int nTimeout);
    bool CreateReceiveSocket(int nInPort);
    bool CreateReceiveSocket(int nInPort, int nTimeout);
    bool CreateReceiveSocket(char* pInIPAddr, int nInPort);
    bool CreateReceiveSocket(char* pInIPAddr, int nInPort, int nTimeout);
    int Accept(HTCPSocket *serveur);
    unsigned int get_m_nsocket_tcp();

    virtual int Send(const char *, size_t);
    virtual int Receive(void *Buffer, long Size);
};

#endif

/******************************************************************************
    End
******************************************************************************/
