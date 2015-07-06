/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        VodErrorMessage.h                       */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#ifndef _VOD_ERROR_MESSAGE_H
#define _VOD_ERROR_MESSAGE_H

#define ERR_SOCKET_CREATE_FAILED      "%s(%d)Socket创建失败。"
#define ERR_SOCKET_BIND_FAILED        "%sSocket绑定失败。"
#define ERR_SOCKET_CONNECT_FAILED     "与%s(%s:%d)的连接失败。"
#define ERR_SOCKET_CONNECT_TIMEOUT    "与%s的连接超时。"
#define ERR_SOCKET_SEND_FAILED        "向%s送信失败。"
#define ERR_SOCKET_SEND_TIMEOUT       "向%s送信超时。"
#define ERR_SOCKET_RECV_FAILED        "从%s接收数据失败。"
#define ERR_SOCKET_RECV_TIMEOUT       "从%s接收数据超时。"

#endif // _VOD_ERROR_MESSAGE_H

/*****************************************************************************
    End
*****************************************************************************/
