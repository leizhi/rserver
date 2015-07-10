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

    CProcessFactory*    pFactoryInstance = NULL; /* 处理模块实例工厂         */
    CObjectFactory*     pObjectInstance  = NULL; /* 对象实例工厂             */

    //CLog* //pSysLogger = NULL;
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
    //pSysLogger = pObjectInstance->GetSysLogger();
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

    if ( pChannelItem != NULL ){
        delete pChannelItem;
        pChannelItem = NULL;
    }

    char s[1024];
    fgets(s,1024,stdin);
    while(strcmp(s,"exit\n")!=0){
        fgets(s,1024,stdin);
        //printf("%s",s);
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
