/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        主进程机能                              */
/*           程序名        ：        Weather_Data_service.cpp                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <getopt.h>

#include "ProcessFactory.h"
#include "ChannelItem.h"
#include "ProcessThread.h"
#include "SystemConfigInfo.h"

//#include <signal.h>

void SigRoutine(int iSignNum)
{
    printfs(0,"capture signal number:%d\n",iSignNum);
}

/******************************************************************************
    处理名        ：  Main处理
    函数名        ：  main(int argc, char* argv[])
    参数          ：  (I)    ：argc - 命令行参数个数
                      (I)    ：argv - 命令行参数列表
    返回值        ：  =  0   ：正常结束
                      <> 0   ：异常结束
******************************************************************************/
int main(int argc, char* argv[])
{
/*
  if(signal(SIGHUP,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGHUP"); 
  }
  if(signal(SIGINT,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGINT"); 
  } 
  if(signal(SIGQUIT,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGQUIT"); 
  } 
  if(signal(SIGILL,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGILL"); 
  } 
  if(signal(SIGTRAP,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGTRAP"); 
  } 
  if(signal(SIGBUS,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGBUS"); 
  } 
  if(signal(SIGFPE,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGFPE"); 
  } 
  if(signal(SIGKILL,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGKILL"); 
  } 
  if(signal(SIGUSR1,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGUSR1"); 
  } 
  if(signal(SIGSEGV,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGSEGV"); 
  } 
  if(signal(SIGUSR2,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGUSR2"); 
  } 
  if(signal(SIGPIPE,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGPIPE"); 
  } 
  if(signal(SIGALRM,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGALRM"); 
  } 
  if(signal(SIGTERM,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGTERM"); 
  }  
  if(signal(SIGSTKFLT,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for  SIGSTKFLT"); 
  } 
  if(signal(SIGCHLD,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGCHLD"); 
  } 
  if(signal(SIGCONT,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGCONT"); 
  } 
  if(signal(SIGSTOP,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGSTOP"); 
  } 
  if(signal(SIGTSTP,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGTSTP"); 
  } 
  if(signal(SIGTTIN,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGTTIN"); 
  } 
  if(signal(SIGTTOU,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGTTOU"); 
  } 
  if(signal(SIGURG,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGURG"); 
  } 
  if(signal(SIGXCPU,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGXCPU"); 
  } 
  if(signal(SIGXFSZ,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGXFSZ"); 
  } 
  if(signal(SIGVTALRM,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGVTALRM"); 
  } 
  if(signal(SIGPROF,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGPROF"); 
  } 
  if(signal(SIGWINCH,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGWINCH"); 
  } 
  if(signal(SIGIO,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGIO"); 
  } 
  if(signal(SIGPWR,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGPWR"); 
  } 
  if(signal(SIGSYS,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGSYS"); 
  } 
  if(signal(SIGRTMIN,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGRTMIN"); 
  } 
  if(signal(SIGRTMIN+1,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGRTMIN+1"); 
  } 
  if(signal(SIGRTMIN+2,SigRoutine) == SIG_ERR) 
  { 
      printfs(0,"coundn't register signal handler for SIGRTMIN+2"); 
  } 
  */
    CProcessFactory*    pFactoryInstance = NULL; /* 处理模块实例工厂         */
    CObjectFactory*     pObjectInstance  = NULL; /* 对象实例工厂             */

    /* 取得日志操作模块                                                      */
    pObjectInstance  = CObjectFactory::GetInstance();
    CChannelItem* pChannelItem = NULL;
    /* 命令行参数解析                                                        */
    int c = 0;
    int nCount = 0;
    while ((c = getopt(argc, argv, "c:s:h")) != EOF)
    {
        switch(c)
        {
            case 'c':
                pObjectInstance->SetChannelConfFile(optarg);
                nCount++;
                break;
            case 's':
                pObjectInstance->SetSystemConfFile(optarg);
                nCount++;
                break;
            case 'h':
                printf("\nUsage : Weather_Data_service [OPTION]\n%s%s%s",
                "\n-c\twds_channel_config.ini file path",
                "\n-s\twds_system_config.ini file path",
                "\n-h\tprint this helper\n");
                break;
            default:
                break;
        }
    }
    if (nCount == 0){
        if ( pObjectInstance != NULL ){
            delete pObjectInstance;
            pObjectInstance = NULL;
        }
        printf("\nUsage : Weather_Data_service [OPTION]\n%s%s%s",
                "\n-c\twds_channel_config.ini file path",
                "\n-s\twds_system_config.ini file path",
                "\n-h\tprint this helper\n");
        return 0;
    }

    CSystemConfigInfo* pConfigInfo = pObjectInstance->GetSystemConfigInfo();
    printfs(1, "Start Weather_Data_service Application!");

    /* 取得命令行处理模块                                                    */
    pFactoryInstance = CProcessFactory::GetInstance();
    pChannelItem = new CChannelItem(pConfigInfo->GetRecvPort());
    /* 启动命令行处理线程                                                    */
    try{
        printfs(1, "Start pChannelItem->Create()!");
        pChannelItem->Start();
        printfs(1, "Start pChannelItem->Join()!");
        pChannelItem->Join();
        printfs(1, "End pChannelItem->Join()!");
    }
    catch(...){
        pChannelItem->Stop();
        printfs(1, "Exception occured!!");
    }

    char s[1024];
    fgets(s,1024,stdin);
    while(strcmp(s,"exit\n")!=0){
        fgets(s,1024,stdin);
        //printf("%s",s);
    }

    if ( pChannelItem != NULL ){
        delete pChannelItem;
        pChannelItem = NULL;
    }
    
    printfs(1, "Exit Weather_Data_service Application!");
    pObjectInstance = CObjectFactory::GetInstance();
    if ( pObjectInstance != NULL ){
        delete pObjectInstance;
        pObjectInstance = NULL;
    }

    if ( pFactoryInstance != NULL ){
        delete pFactoryInstance;
        pFactoryInstance = NULL;
    }

    //pthread_exit(NULL);

    return 0;
}
/******************************************************************************
    End
******************************************************************************/

