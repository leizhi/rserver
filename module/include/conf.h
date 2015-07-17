#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "assert.h"
#include "errno.h"

#ifndef INCLUDE_CONF_H

#define INCLUDE_CONF_H

#define KEYVALLEN 100

//  
typedef struct _channel_conf  
{
    char db_ip[17];
    unsigned int db_port;
    char db_name[32];
    char db_user[32];
    char db_pass[32];
    unsigned int recv_port;
    char log_path[64];
    unsigned int log_on;
    unsigned int threads;
} channel_conf;

//系统配置文件
typedef struct
{
    char db_ip[17];
    unsigned int db_port;
    char db_name[32];
    char db_user[32];
    char db_pass[32];
    unsigned int recv_port;
    char log_path[64];
    unsigned int log_on;
    unsigned int threads;
}_system_conf;

static _system_conf system_conf;

void init_system_conf();

//   删除左边的空格
char * l_trim(char * szOutput, const char *szInput);

//   删除右边的空格
char *r_trim(char *szOutput, const char *szInput);

//   删除两边的空格
char * a_trim(char * szOutput, const char * szInput);

//   删除两边的空格
int GetProfileString(char *profile, char *AppName, char *KeyName, char *KeyVal );

#endif
