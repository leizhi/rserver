/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        图像上传业务处理类                      */
/*           程序名        ：        WDSStationImageProcess.h                */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/
#if !defined(_WDSSTATIONIMAGEPROCESS_H)
#define _WDSSTATIONIMAGEPROCESS_H
/*****************************************************************************
    系统Include
******************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <bitset> 
#include <iostream>
#include <vector>
/*****************************************************************************
    用户Include
******************************************************************************/
#include    "VodUserType.h"
#include    "Process.h"
#include    "TCPSocket.h"
#include    "VodEntity.h"
#include    "log.h"
#include    "ObjectFactory.h"
#include    "VodErrorMessage.h"
#include    "VodFile.h"
/******************************************************************************/

using namespace std;

class CWDSStationImageProcess : public IProcess {

private:
    //CLog*            m_pSysLogger;
    HTCPSocket*      m_pSendSocket;
    HTCPSocket*      m_pRecvSocket;
    int              m_nFuncType;
    BVodFile*        m_pFile;

public:
    virtual void Do();
    CWDSStationImageProcess();
    ~CWDSStationImageProcess();

    void SetRecvInfo(HTCPSocket* pRecvSocket, int nType)
    {
        m_pRecvSocket = pRecvSocket;
        m_nFuncType = nType;
    }

};
#endif  //_WDSSTATIONIMAGEPROCESS_H

/*****************************************************************************
    End
*****************************************************************************/
