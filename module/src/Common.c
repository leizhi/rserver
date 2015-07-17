/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        共通处理                                */
/*           程序名        ：        Common.cpp                              */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "Common.h"
#include <stdio.h>
#include <sys/stat.h>
#include <string.h>
#include <unistd.h>

#include "VodEntity.h"

int CheckRealtimeStatus(char cStatusSrc, char cCompareDest)
{
    return ((cStatusSrc & cCompareDest) == 0x00) ? 0 : 1;
}

int IsAlertChanged(char cStatusSrc, char cCompareDest, int nOldAlert)
{
    int nRet = CheckRealtimeStatus(cStatusSrc, cCompareDest);
    if ((nRet == 0) && (nOldAlert == 0))
        return 0;
    else if ((nRet == 0) && (nOldAlert == 1))
        return 0;
    else if ((nRet == 1) && (nOldAlert == 0))
        return 1;
    else if ((nRet == 1) && (nOldAlert == 1))
        return 0;
    else
        return 0;
}

bool CreateFolder(char* pFolder)
{
    char pPath[MAX_FILEPATH_LENGTH] = "\0";
    memcpy(pPath, pFolder, sizeof(pPath));
    char* pPosition = pPath;
    while (pPosition = strchr(++pPosition, '/')) {
        *pPosition = '\0';

        if (access(pPath, 0) != -1) {
            *pPosition = '/';
            continue;
        }

        int nRet = mkdir(pPath, 0777);
        if (nRet != 0){
            return false;
        }

        *pPosition = '/';
    }

    return true;
}

/******************************************************************************
    End
******************************************************************************/
