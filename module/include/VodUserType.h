/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        VodUserType.h                           */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "system_include.h"

#if !defined(_VOIP_H)
#define _VOIP_H

//#ifndef _DEBUG
    #define _DEBUG
//#endif

#define CHANNELCOUNT    10

#define MAX_FNAME       256    /* 文件名和文件路径的最大值                   */

//Contioner num
#define MAXCAPACITY     5

/* Type model indepent typedefs */

#ifndef bool_t
typedef bool  bool_t;
#endif

#ifndef byte_t
typedef char  byte_t;
#endif
#ifndef p_byte_t
typedef char* p_byte_t;
#endif


#ifndef ubyte_t
typedef unsigned char  ubyte_t;
#endif
#ifndef p_ubyte_t
typedef unsigned char* p_ubyte_t;
#endif

#ifndef char_t
typedef char  char_t;
#endif
#ifndef p_char_t
typedef char* p_char_t;
#endif

#ifndef uchar_t
typedef unsigned char  uchar_t;
#endif
#ifndef p_uchar_t
typedef unsigned char* p_uchar_t;
#endif

//#ifndef int8_t
//typedef                char    int8_t;
//#endif
#ifndef uint8_t
typedef unsigned char  uint8_t;
#endif

#ifndef int16_t
typedef short          int16_t;
#endif
#ifndef    uint16_t
typedef unsigned short uint16_t;
#endif


#ifndef int32_t
typedef int int32_t;
#endif
#ifndef uint32_t
typedef unsigned int uint32_t;
#endif

#ifndef float32_t
typedef float float32_t;
#endif

typedef signed long int    int64_t;

typedef unsigned long int  uint64_t;

#define U16_AT(p)       ( (((uint16_t)((uchar_t*)&(p))[0]) << 8)                \
                          | (((uint16_t)((uchar_t*)&(p))[1])) )
#define U32_AT(p)       ( (((uint32_t)((uchar_t*)&(p))[0]) << 24)               \
                          | (((uint32_t)((uchar_t*)&(p))[1]) << 16)             \
                          | (((uint32_t)((uchar_t*)&(p))[2]) << 8)              \
                          | (((uint32_t)((uchar_t*)&(p))[3])) )

// Macros used to write a packet

#define SET_U16_TO(p, n)        ((uchar_t*)&(p))[0] = (n) >> 8;              \
                                ((uchar_t*)&(p))[1] = (n);
#define SET_U32_TO(p, n)        ((uchar_t*)&(p))[0] = (n) >> 24;             \
                                ((uchar_t*)&(p))[1] = (n) >> 16;             \
                                ((uchar_t*)&(p))[2] = (n) >> 8;              \
                                ((uchar_t*)&(p))[3] = (n);

#define SEM_READ_LIST               0
#define SEM_WRITE_LIST              1

#define IPVERSION                   4

#define _LOC_CONFIG_PATH_          "./"

#ifdef DEBUG
#define ASSERT(Var)                                                            \
if (!(Var))                                                                    \
    printf("ASSERT ERROR in %s line %d (code checked: %s)\n",                  \
           __FILE__, __LINE__, #Var);
#else
#define ASSERT(Var)
#endif

#endif  //_VOIP_H

/*****************************************************************************
    End
*****************************************************************************/
