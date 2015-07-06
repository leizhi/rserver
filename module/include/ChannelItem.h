/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        命令行控制机能                          */
/*           程序名        ：        ChannelItem.h                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#ifndef _CCHANNELITEM_H_
#define _CCHANNELITEM_H_

/*****************************************************************************
    系统Include
******************************************************************************/
#include     <stdio.h>
#include     <string.h>
#include     <iostream>

#include <pthread.h>
#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
/******************************************************************************
    用户Include
******************************************************************************/
#include    "log.h"
#include    "ProcessThread.h"

/* 添加频道端口处理器                                                        */
// 端口监视器处理类
#include    "SocketMonitorProcess.h"
class CChannelItem {
public:
    CChannelItem(int nPort);
    ~CChannelItem();
    int  Start();
    int  Stop();
    int  Join();
    bool CanStart()         {return m_bCanStart;}
    bool IsStart()          {return m_bStarted; }
private:
    int                    m_nSocketPort;
    CSocketMonitorProcess* m_pSocketMonitor;
    HProcessThread*        m_pSocketMonitorThread;
    //CLog*                  m_pSysLogger;

    int     Init();
    int     CreateProcess();
    void    ThreadCreate();
    void    ThreadStart();
    int     ThreadJoin();
    void    ThreadStop();
    void    ReleaseThread();
    void    ReleaseProcess();
    bool    m_bCanStart;
    bool    m_bStarted;
};

#endif /*_CCHANNELITEM_H_*/

/*****************************************************************************
    End
*****************************************************************************/
