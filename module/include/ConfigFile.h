/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        ConfigFile.h                            */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#if !defined(_CONFIGFILE_H)
#define _CONFIGFILE_H

#include <string.h>
#include <stdlib.h>
#include "VodUserType.h"
#include "VodFile.h"

#define FILE_CHANNEL_CONFIG "../config/mcs_channel_config.ini"
#define FILE_SYSTEM_CONFIG "../config/mcs_system_config.ini"

class HConfigFile : public BVodFile{

public:
    /* 构造函数和析构函数                                                    */
    HConfigFile(p_char_t pFileName);
    ~HConfigFile();

    /* 取得配置文件中指定的主键内容的函数                                    */
    int32_t GetConfInfoString(const p_char_t pPrimaryKey,
                              const p_char_t pSecondaryKey,
                              p_char_t pValue);
    int32_t GetConfInfoInt(const p_char_t pPrimaryKey,
                           const p_char_t pSecondaryKey,
                           int32_t& nValue);
    int32_t GetConfInfoLong(const p_char_t pPrimaryKey,
                            const p_char_t pSecondaryKey,
                            int64_t& nValue);
    int64_t GetConfInfoListHead(const p_char_t pPrimaryKey,
                                const p_char_t pSecondaryKey);
    int64_t GetConfInfoListNext(const int64_t nPos,
                                int32_t& nValue);
};

#endif

/******************************************************************************
    End
******************************************************************************/
