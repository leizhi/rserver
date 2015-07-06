/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        Socket.h                                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <errno.h>

#include <set>

#if !defined(_BSOCKET_H)
#define _BSOCKET_H

class BSocket
{
public:
    virtual ~BSocket() {};

    virtual int Send(const char *, size_t) = 0 ;
    virtual int Receive(void *Buffer, long Size) = 0 ;
    virtual void Close() = 0 ;
};

#endif

/******************************************************************************
    End
******************************************************************************/
