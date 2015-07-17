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

#include "stdlib.h"
#include "errno.h"
#include "arpa/inet.h"
#include "netinet/in.h"

#include "sys/types.h"
#include "sys/socket.h"
#include "sys/wait.h"
#include "sys/time.h"
#include "sys/types.h"

#include "log.h"
#include "conf.h"

//#include "ProcessFactory.h"
//#include "ChannelItem.h"
//#include "ProcessThread.h"
//#include "SystemConfigInfo.h"

#define MAXBUF 1024

typedef struct CLIENT {
	int fd;
	struct sockaddr_in addr;	
}CLIENT;

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
                init_channel_conf();
                nCount++;
                break;
            case 's':
                //pObjectInstance->SetSystemConfFile(optarg);
                init_system_conf();

                nCount++;
                break;
            case 'h':
                printfs(1,"\nUsage : Weather_Data_service [OPTION]\n%s%s%s",
                "\n-c\twds_channel_config.ini file path",
                "\n-s\twds_system_config.ini file path",
                "\n-h\tprint this helper\n");
                break;
            default:
                break;
        }
    }
    if (nCount == 0){
        printfs(1,"\nUsage : Weather_Data_service [OPTION]\n%s%s%s",
                "\n-c\twds_channel_config.ini file path",
                "\n-s\twds_system_config.ini file path",
                "\n-h\tprint this helper\n");
        return 0;
    }

    //CSystemConfigInfo* pConfigInfo = pObjectInstance->GetSystemConfigInfo();
    printfs(1, "Start Weather_Data_service Application!");

    //服务端套接字
	int i,n,maxi = -1;
	int nready;
	int slisten,sockfd,maxfd=-1,connectfd;
	
	unsigned int myport,lisnum; 

	struct sockaddr_in  my_addr,addr;
	struct timeval tv;
	
	socklen_t len;
	fd_set rset,allset;	
	
	char buf[MAXBUF + 1];
	CLIENT client[FD_SETSIZE];

	myport = 10086;
	lisnum = FD_SETSIZE;

	if((slisten = socket(AF_INET,SOCK_STREAM,0)) == -1)
	{
		perror("socket");
		exit(1);
	}
	printfs(1, "AF_INET!");

	bzero(&my_addr,sizeof(my_addr));
	my_addr.sin_family = AF_INET;
	my_addr.sin_port = htons(myport);
	my_addr.sin_addr.s_addr = INADDR_ANY;

	if(bind(slisten, (struct sockaddr *)&my_addr, sizeof(my_addr)) == -1) {
        	perror("bind");
        	exit(1);
        }
	printfs(1, "bind!");
	
	if (listen(slisten, lisnum) == -1) {
        	perror("listen");
        	exit(1);
    	}

	printfs(1, "listen!");
	
	for(i=0;i<FD_SETSIZE;i++)
	{
		client[i].fd = -1;
	}

	FD_ZERO(&allset);
	FD_SET(slisten, &allset);  
	maxfd = slisten;

/*
struct tcp_info info; 
int len=sizeof(info); 
getsockopt(sock, IPPROTO_TCP, TCP_INFO, &info, (socklen_t *)&len); 
if((info.tcpi_state==TCP_ESTABLISHED))
	
else
	
*/
	printfs(1,"Waiting for connections and data...");
	while (1)
	{		
		rset = allset;        	

		tv.tv_sec = 1;
		tv.tv_usec = 0;

		nready = select(maxfd + 1, &rset, NULL, NULL, &tv);

		if(nready == 0)
			continue;

		if(nready < 0)
		{
			printfs(1,"select failed!");
			break;
		}

		if(FD_ISSET(slisten,&rset)) // new connection
		{			
			len = sizeof(struct sockaddr);
			if((connectfd = accept(slisten,
				(struct sockaddr*)&addr,&len)) == -1)
			{
				perror("accept() error");
				continue;
			}
			for(i=0;i<FD_SETSIZE;i++)
			{
				if(client[i].fd < 0)
				{
					client[i].fd = connectfd;
					client[i].addr = addr; 				
					printfs(1,"Yout got a connection from %s.",inet_ntoa(client[i].addr.sin_addr));
					break;
				}
			}
			if(i == FD_SETSIZE)			
				printfs(1,"too many connections");
			FD_SET(connectfd,&allset);
			if(connectfd > maxfd)
				maxfd = connectfd;
			if(i > maxi)
				maxi = i;
		}
		else
		{			
			for(i=0;i<=maxi;i++)
			{			
				if((sockfd = client[i].fd)<0)
					continue;
				if(FD_ISSET(sockfd,&rset))
				{
					bzero(buf,MAXBUF + 1);

					if((n = recv(sockfd,buf,MAXBUF,0)) > 0)
					{
						printfs(1,"received data:%s from %s",buf,inet_ntoa(client[i].addr.sin_addr));
					}
					else
					{
					    printfs(1,"disconnected by client!");
						close(sockfd);
						FD_CLR(sockfd,&allset);
						client[i].fd = -1;
					}
				}
			}
		}
	}
	close(slisten);

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
        //printfs(1,"%s",s);
    }

    //pthread_exit(NULL);
    return 0;
}
/******************************************************************************
    End
******************************************************************************/

