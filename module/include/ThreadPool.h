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
#include "stdio.h"
#include "stdlib.h"
#include "unistd.h"
#include "pthread.h"
#include "string.h"
#include "time.h"
#include "string.h"
#include "sys/types.h"
#include "stdarg.h"
#include "assert.h"

#ifndef THREADPOOL_H

#define THREADPOOL_H

#define MAX_POOL 10

/* 
*线程池里所有运行和等待的任务都是一个CThread_worker 
*由于所有任务都在链表里，所以是一个链表结构 
*/  
typedef struct worker  
{
	//回调函数，任务运行时会调用此函数，注意也可声明成其它形式
	void *(*process) (const void *arg);  
	void *arg;/*回调函数的参数*/
	struct worker *next;
} CThread_worker;
  
/*线程池结构*/  
typedef struct 
{  
     pthread_mutex_t queue_lock;  
     pthread_cond_t queue_ready;  
  
    /*链表结构，线程池中所有等待任务*/  
     CThread_worker *queue_head;
  
    /*是否销毁线程池*/  
    int shutdown;  
     pthread_t *threadid;

    /*线程池中允许的活动线程数目*/  
    int max_thread_num;  

    /*当前等待队列的任务数目*/  
    int cur_queue_size;  
} CThread_pool;  

static CThread_pool *pool = NULL;

int pool_add_worker(void *(*process) (void *arg), void *arg);

void *thread_routine (void *arg);

//初始化线程池
void pool_init (int max_thread_num);

//向线程池中加入任务
int pool_add_worker (void *(*process) (const void *arg), void *arg);

//销毁线程池，等待队列中的任务不会再被执行，但是正在运行的线程会一直 
//把任务运行完后再退出
int pool_destroy();

//可用线程数
int pool_free();

#endif

/******************************************************************************
    End
******************************************************************************/
