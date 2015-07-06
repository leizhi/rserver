/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        日志文件操作机能                        */
/*           程序名        ：        log.h                                   */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/
#pragma once

#include "time.h"
#include "stdio.h"
#include "string.h"
#include "sys/types.h"
#include "stdarg.h"
#include <iostream>
#include <fstream>

using namespace std;

#ifndef INCLUDE_LOG_H

#define INCLUDE_LOG_H

//extern char logdtnows[20];
    
char * getNow();

void printfs(unsigned int nlevel, const char * fmt, ...);

#endif

/******************************************************************************
    End
******************************************************************************/
