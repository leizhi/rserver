/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        端口监控类                              */
/*           程序名        ：        SocketMonitorProcess.h                  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-04-02          Create                        */
/*                                                                           */
/*****************************************************************************/
#if !defined(_SOCKETMONITORPROCESS_H)
#define _SOCKETMONITORPROCESS_H
/*****************************************************************************
    系统Include
******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <bitset> 
#include <iostream>
#include <vector>
/*****************************************************************************
    用户Include
******************************************************************************/
#include    "VodUserType.h"
#include    "Process.h"
#include    "TCPSocket.h"
#include    "VodEntity.h"
#include    "log.h"
#include    "ObjectFactory.h"
#include    "VodErrorMessage.h"
/*****************************************************************************/

using namespace std;

void *clientprocess(const void *m_pTCPSocket);

class CSocketMonitorProcess : public IProcess {

private:
    bool                   m_bBreakFlag;
    int                    m_nSocketPort;
    HTCPSocket*            m_pRecvSocket;
    HTCPSocket             m_monitorSocket;
    int                    m_nFuncType;

public:
    virtual void Do();
    CSocketMonitorProcess();
    ~CSocketMonitorProcess();

    int TerminateProcess();

    void SetChannelPort(int nPort){ m_nSocketPort = nPort; }

    void SetRecvInfo(HTCPSocket* pRecvSocket, int nType)
    {
        m_pRecvSocket = pRecvSocket;
        m_nFuncType = nType;
    }
};
#endif  //_SOCKETMONITORPROCESS_H

/*****************************************************************************
    End
*****************************************************************************/
