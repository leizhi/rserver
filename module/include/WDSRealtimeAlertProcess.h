/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        实时报警业务处理类                      */
/*           程序名        ：        WDSRealtimeAlertProcess.h               */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/
#if !defined(_WDSREALTIMEALERTPROCESS_H)
#define _WDSREALTIMEALERTPROCESS_H
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
/******************************************************************************/

using namespace std;

class CWDSRealtimeAlertProcess : public IProcess {

private:
    HTCPSocket*      m_pSendSocket;
    HTCPSocket*      m_pRecvSocket;
    int              m_nFuncType;

public:
    virtual void Do();
    CWDSRealtimeAlertProcess();
    ~CWDSRealtimeAlertProcess();

    void SetRecvInfo(HTCPSocket* pRecvSocket, int nType)
    {
        m_pRecvSocket = pRecvSocket;
        m_nFuncType = nType;
    }

};
#endif  //_WDSREALTIMEALERTPROCESS_H

/*****************************************************************************
    End
*****************************************************************************/
