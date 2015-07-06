/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        配置文件处理机能                        */
/*           程序名        ：        ConfigFile.cpp                          */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "ConfigFile.h"

/******************************************************************************
    处理名        ：  本类的构造处理
    函数名        ：  构造函数
    参数          ：  (I) ：pName - 设定文件的文件名和路径
    返回值        ：  无
******************************************************************************/
HConfigFile::HConfigFile(p_char_t pFileName):BVodFile(pFileName){
    //NULL
}

/******************************************************************************
    处理名        ：  本类的析构处理
    函数名        ：  析构函数
    参数          ：  无
    返回值        ：  无
******************************************************************************/
HConfigFile::~HConfigFile(){
    //NULL
}

/******************************************************************************
    处理名        ：  从配置文件取得字符串的处理
    函数名        ：  GetConfInfoString()函数
    参数          ：  (I) ：pPrimaryKey   - 主键的名字
                      (I) ：pSecondaryKey - 副键的名字
                      (O) ：pValue        - 键值
    返回值        ：  =  0    ：正常
                      = -1    ：键不存在
******************************************************************************/
int32_t HConfigFile::GetConfInfoString(const p_char_t pPrimaryKey, 
                                       const p_char_t pSecondaryKey, 
                                             p_char_t pValue){
    char_t   chLineValue[120] = "\0"; /* 数据读入缓冲区                      */
    char_t   chSpecialTag     = '[';
    p_char_t pKey             = NULL; /* 键名保存缓冲区                      */
    p_char_t pKeyValue        = NULL; /* 键值保存缓冲区                      */

    /* 主键的名字、副键的名字或键值保存缓冲区为NULL                          */
    if ( (pPrimaryKey == NULL) && (pSecondaryKey == NULL) && (pValue == NULL) ){
        return -1;
    }
    /* 当前文件已经被打开                                                    */
    if ( !m_pBaseFile ){
        return -1;
    }
    fseek( m_pBaseFile, 0, SEEK_SET );
    /* 在当前的配置文件中通过主键进行检索                                    */
    while( !feof(m_pBaseFile) ){
        fgets( chLineValue, 120, m_pBaseFile );
        /* 当前取得的字符串不是主键                                          */
        if ( strncmp( pPrimaryKey, chLineValue, strlen(pPrimaryKey) ) != 0 ){
            continue;
        }
        /* 在当前配置文件中通过副键进行检索                                  */
        while( !feof(m_pBaseFile) ){
            fgets( chLineValue, 120, m_pBaseFile );
            /* 当前主键区域内没有指定的副键                                  */
            if ( strncmp( &chSpecialTag, chLineValue, sizeof(chSpecialTag) ) == 0 ){
                return -1;
            }
            /* 当前取得的字符串不是副键                                      */
            if ( strncmp( pSecondaryKey, chLineValue, strlen(pSecondaryKey) ) != 0 ){
                continue;
            }
            /* 通过当前的字符串取出键值                                      */
            pKey = strtok(chLineValue, "=");
            pKeyValue = strtok(0, "=");
            memcpy( pValue, pKeyValue, strlen(pKeyValue) - 1 );
            return 0;
        }
    }
    return -1;
}

/******************************************************************************
    处理名        ：  从配置文件取得整型数字的处理
    函数名        ：  GetConfInfoString()函数
    参数          ：  (I) ：pPrimaryKey   - 主键的名字
                      (I) ：pSecondaryKey - 副键的名字
                      (O) ：nValue        - 键值
    返回值        ：  =  0    ：正常
                      = -1    ：键不存在
******************************************************************************/
int32_t HConfigFile::GetConfInfoInt(const p_char_t pPrimaryKey, 
                                    const p_char_t pSecondaryKey, 
                                          int32_t& nValue){
    int32_t nRet              = -1;   /* 返回值                              */
    char_t  chLineValue[1024] = "\0"; /* 数据读入缓冲区                      */
    /* 通过配置文件取得字符串形式的键值                                      */
    nRet = GetConfInfoString( pPrimaryKey, pSecondaryKey, chLineValue );
    if ( nRet == 0 ){
        /* 将字符串格式的键值转换为整型格式的键值                            */
        nValue = atoi( chLineValue );
    }
    return nRet;
}

/******************************************************************************
    处理名        ：  从配置文件取得长整型数字的处理
    函数名        ：  GetConfInfoString()函数
    参数          ：  (I) ：pPrimaryKey   - 主键的名字
                      (I) ：pSecondaryKey - 副键的名字
                      (O) ：nValue        - 键值
    返回值        ：  =  0    ：正常
                      = -1    ：键不存在
******************************************************************************/
int32_t HConfigFile::GetConfInfoLong(const p_char_t pPrimaryKey, 
                                     const p_char_t pSecondaryKey, 
                                           int64_t& nValue){
    int32_t nRet              = -1;   /* 返回值                              */
    char_t  chLineValue[1024] = "\0"; /* 数据读入缓冲区                      */
    /* 通过配置文件取得字符串形式的键值                                      */
    nRet = GetConfInfoString( pPrimaryKey, pSecondaryKey, chLineValue );
    if ( nRet == 0 ){
        /* 将字符串格式的键值转换为长整型格式的键值                          */
        nValue = atol( chLineValue );
    }
    return nRet;
}

/******************************************************************************
    处理名        ：  从配置文件取得列表Header的处理
    函数名        ：  GetConfInfoListHead()函数
    参数          ：  (I) ：pPrimaryKey   - 主键的名字
                      (I) ：pSecondaryKey - 副键的名字
    返回值        ：  =  0    ：当前文件里没有指定的键值
                      >= 1    ：当前文件里有指定的键值
******************************************************************************/
int64_t HConfigFile::GetConfInfoListHead(const p_char_t pPrimaryKey,
                                         const p_char_t pSecondaryKey){
    int64_t nRet                = 0;    /* 返回值                            */
    char_t     chLineValue[128] = "\0"; /* 文件读取缓冲区                    */
    char_t     chSpecialTag     = '[';
    p_char_t   pKey             = NULL; /* 键名保存缓冲区                    */
    p_char_t   pKeyValue        = NULL; /* 键值保存缓冲区                    */
    /* 主键或副键的缓冲区为NULL                                              */
    if ( (pPrimaryKey == NULL) && (pSecondaryKey == NULL) ){
        return nRet;
    }
    /* 当前文件处于打开状态                                                  */
    if ( !m_pBaseFile ){
        return nRet;
    }
    fseek( m_pBaseFile, 0, SEEK_SET );
    /* 在当前的配置文件中通过主键进行检索                                    */
    while( !feof(m_pBaseFile) ){
        fgets( chLineValue, 128, m_pBaseFile );
        /* 当前取得的字符串不是主键                                          */
        if ( strncmp( pPrimaryKey, chLineValue, strlen(pPrimaryKey) ) != 0 ){
            continue;
        }
        /* 在当前的配置文件中通过副键进行检索                                */
        while( !feof(m_pBaseFile) ){
            /* 在当前字符串中取出键值内容                                    */
            nRet = ftell(m_pBaseFile);
            fgets( chLineValue, 120, m_pBaseFile );
            /* 当前主键区域内没有指定的副键                                  */
            if ( strncmp( &chSpecialTag, chLineValue, sizeof(chSpecialTag) ) == 0 ){
                return -1;
            }
            /* 当前取得的字符串不是副键                                      */
            if ( strncmp( pSecondaryKey, chLineValue, strlen(pSecondaryKey) ) != 0 ){
                continue;
            }
            /* 通过当前的字符串取出键值                                      */
            pKey = strtok(chLineValue, "=");
            pKeyValue = strtok(0, "=");
            /* 取出列表字符串的位置                                          */
            nRet += strlen(pKey) + 1;
            break;
        }
        break;
    }
    return nRet;
}

/******************************************************************************
    处理名        ：  从配置文件取得列表内容的处理
    函数名        ：  GetConfInfoListNext()函数
    参数          ：  (I) ：nPos   - 通过GetConfInfoListHead()取得的列表Header
                      (O) ：nValue - 键值
    返回值        ：  =  0    ：当前没有下一个键值
                      >= 1    ：当前有下一个键值
    调用注意      ：  1，在调用本函数前，必须调用GetConfInfoListHead()函数；
                      2，本函数的返回值为"0"时，请不要使用通过参数nValue进行返回的键值。
******************************************************************************/
int64_t HConfigFile::GetConfInfoListNext(const int64_t nPos, int32_t& nValue){
    int64_t nRet                = 0;        /* 返回值                        */
    int16_t nStatus             = 0;
    char_t     chLineValue[10]  = "\0";     /* 文件读取缓冲区                */
    p_char_t   pKey             = NULL;     /* 键名保存缓冲区                */
    p_char_t   pKeyValue        = NULL;     /* 键值保存缓冲区                */
    /* 指定的列表数据异常                                                    */
    if ( nPos <= 0){
        return nRet;
    }
    /* 当前文件处于打开状态                                                  */
    if ( !m_pBaseFile ){
        return nRet;
    }
    nRet = nPos;
    fseek( m_pBaseFile, 0, SEEK_SET );
    fseek( m_pBaseFile, nPos, SEEK_SET );
    /* 在当前的配置文件中进行键值检索                                        */
    while( !feof(m_pBaseFile) ){
        memset(chLineValue, 0x00, sizeof(chLineValue));
        fread( chLineValue, 1, sizeof(chLineValue)-1, m_pBaseFile );
        /* 无效数据                                                          */
        if ( strlen( chLineValue ) <= 1 ) {
            nRet = 0;
            break;
        }
        /* 通过当前的字符串取出键值                                          */
        pKey = strtok(chLineValue, ",");
        pKeyValue = strtok(0, ",");
        nStatus = sscanf(pKey, "%d", &nValue);
        /* 取出列表字符串的位置                                              */
        if ( (nStatus <= 0) || (pKeyValue == NULL) ){
            nRet = 0;
            break;
        }
        nRet += strlen(pKey) + 1;
        break;
    }
    return nRet;
}

/******************************************************************************
    End
******************************************************************************/
