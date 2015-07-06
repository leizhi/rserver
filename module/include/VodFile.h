/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通机能                                */
/*           程序名        ：        VodFile.h                               */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/


#if !defined(_VODFILE_H)
#define _VODFILE_H
/******************************************************************************
    系统include
******************************************************************************/
#include <stdio.h>
/******************************************************************************
    用户include
******************************************************************************/
#include "VodUserType.h"

#define     FILE_READ_MODE         "rb"
#define     FILE_WRITE_MODE        "ab+"

class BVodFile {
protected:
    FILE*    m_pBaseFile;       /* 系统文件句柄                              */
    p_char_t m_pFileName;       /* 文件名和文件路径                          */
private:
    BVodFile();
public:
    /* 构造函数和系统函数                                                    */
    BVodFile(p_char_t pFileName);
    ~BVodFile();
    /* 文件操作函数                                                          */
    int32_t  Open(p_char_t pMode);
    int32_t  Close();
    int64_t  Read(p_char_t pBuffer, int64_t nSize);
    int64_t  Write(p_char_t pBuffer, int64_t nSize);
    int32_t  Seek(int64_t lOffset,int32_t nMode);
    uint64_t GetPos();
    int32_t  EndFile();
};
#endif  //_VODFILE_H
/******************************************************************************
    End
******************************************************************************/
