/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        ProcessThread.cpp                       */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

/*****************************************************************************
    系统 include
******************************************************************************/
#include <stdio.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
using namespace std;

/*****************************************************************************
    用户 include
******************************************************************************/
#include "ProcessThread.h"
#include "ObjectFactory.h"

/******************************************************************************
    处理名        ：  main process
    函数名        ：  void* StartRoutine(void* pThread)
    参数          ：  (I)    ：pThread -  current object's address
******************************************************************************/
void* HProcessThread::StartRoutine(void* pThread){
    pthread_detach(pthread_self());
    //CLog* //pSysLogger = CObjectFactory::GetInstance()->GetSysLogger();
    HProcessThread* pThis = (HProcessThread*)pThread;
    try{
        pThis->SetRunningState(true);
        printfs(1, "HProcessThread::StartRoutine Start Process!");
        if (pThis->m_pProcess == NULL){
            printfs(0, "No process in this thread!");
        }
        pThis->m_pProcess->Do();
        pThis->SetRunningState(false);
    }
    catch(exception e){
        printfs(0, "HProcessThread::StartRoutine StartRoutine:  Exception occured!!");
    }
    pthread_exit(0);
}

/******************************************************************************
    处理名        ：  initilaze method
    函数名        ：  void InitWork()
******************************************************************************/
void HProcessThread::InitWork() {
}

/******************************************************************************
    处理名        ：  create a new process
    函数名        ：  void Create() 
******************************************************************************/
void HProcessThread::Create()
{
    //CLog* //pSysLogger = CObjectFactory::GetInstance()->GetSysLogger();
    try{
        // Do some initialisations before starting the real job
        InitWork();
        // Now launch the thread
        // Set the thread attributes. It is important for the thread to be created
        // joinable unless the instance of the class could be destroyed before the
        // end of the execution of Do 
        pthread_attr_t sAttr;
        pthread_attr_init(&sAttr);
        pthread_attr_setdetachstate(&sAttr, PTHREAD_CREATE_JOINABLE);
        int iRc = pthread_create(&m_tId, NULL, HProcessThread::StartRoutine, this);
        printf("HProcessThread::Create 0x%08x %d\n",&m_tId,iRc);
        pthread_attr_destroy(&sAttr);
        if(iRc) {
            printfs(0, "pthread_create failure! ");
            return;
        }
    }catch(exception e){
        printfs(0, "pthread_create failure! exception!");
        return;
    }
}

/******************************************************************************
    处理名        ：  To stop the thread, Clean all the resource
    函数名        ：  void Stop()
******************************************************************************/
void HProcessThread::Stop() {
    //CLog* //pSysLogger = CObjectFactory::GetInstance()->GetSysLogger();
    int nRc = pthread_cancel( m_tId );
    if ( nRc < 0 ) {
        printfs(1, "HProcessThread::Stop(%s)..................", GetName());
    }
    m_bIsRunning = false;
}

void HProcessThread::Join() {
    //CLog* //pSysLogger = CObjectFactory::GetInstance()->GetSysLogger();
    int nRc = pthread_join( m_tId, NULL );
    if ( nRc < 0 ) {
        printfs(1, "HProcessThread::~HProcessThread(%s)..................", GetName());
    }
    m_bIsRunning = false;
}

/******************************************************************************
    处理名        ：  To clean all the resource
    函数名        ：  void CleanWork()
******************************************************************************/
void HProcessThread::CleanWork() {
}

/******************************************************************************
    处理名        ：  Set thread's name
    函数名        ：  void SetName(p_char_t pchName) 
    参数          ：  (I)    ：pchName - the thread's name
******************************************************************************/
void HProcessThread::SetName(p_char_t pchName) {
    strncpy(m_chThreadName, pchName, sizeof(m_chThreadName));
}

/******************************************************************************
    处理名        ：  构造函数
    函数名        ：  HProcessThread()
******************************************************************************/
HProcessThread::HProcessThread() {
    m_pProcess   = NULL;
    m_bIsRunning = false;
}

/******************************************************************************
    处理名        ：  构造函数
    函数名        ：  HProcessThread(IProcess* pro)
    参数          ：  (I)    ：pro - appointed processer
******************************************************************************/
HProcessThread::HProcessThread(IProcess* pProcess) {
    m_pProcess   = pProcess;
    m_bIsRunning = false;
}

/******************************************************************************
    处理名        ：  析构函数
    函数名        ：  ~HProcessThread()
******************************************************************************/
HProcessThread::~HProcessThread() {
    if ( !m_pProcess ){
        delete m_pProcess;
        m_pProcess = NULL;
    }
}

/*****************************************************************************
    End
*****************************************************************************/
