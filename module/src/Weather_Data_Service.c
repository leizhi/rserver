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

#include "stdio.h"
#include "string.h"
#include "unistd.h"
#include "getopt.h"

#include "log.h"
#include "conf.h"

//#include "ProcessFactory.h"
//#include "ChannelItem.h"
//#include "ProcessThread.h"
//#include "SystemConfigInfo.h"

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
    /* 命令行参数解析                                                        */
    int c = 0;
    int nCount = 0;
    while ((c = getopt(argc, argv, "c:s:h")) != EOF)
    {
        switch(c)
        {
            case 'c':
                //pObjectInstance->SetChannelConfFile(optarg);
                nCount++;
                break;
            case 's':
                //pObjectInstance->SetSystemConfFile(optarg);
                init_system_conf();

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
        printf("\nUsage : Weather_Data_service [OPTION]\n%s%s%s",
                "\n-c\twds_channel_config.ini file path",
                "\n-s\twds_system_config.ini file path",
                "\n-h\tprint this helper\n");
        return 0;
    }

    //CSystemConfigInfo* pConfigInfo = pObjectInstance->GetSystemConfigInfo();
    printfs(1, "Start Weather_Data_service Application!");

    /* 启动命令行处理线程                                                    */
    printfs(1, "Start pChannelItem->Create()!");
    //pChannelItem->Start();
    printfs(1, "Start pChannelItem->Join()!");
   // pChannelItem->Join();
    printfs(1, "End pChannelItem->Join()!");

    char s[1024];
    fgets(s,1024,stdin);
    while(strcmp(s,"exit\n")!=0){
        fgets(s,1024,stdin);
        //printf("%s",s);
    }

    //pthread_exit(NULL);
    return 0;
}
/******************************************************************************
    End
******************************************************************************/

