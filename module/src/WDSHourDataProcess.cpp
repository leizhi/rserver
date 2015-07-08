/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        六要素小时数据业务处理类                */
/*           程序名        ：        WDSHourDataProcess.cpp                  */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "WDSHourDataProcess.h"
#include "WDS_DBHelper.h"
#include "ObjectFactory.h"

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  CWDSHourDataProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSHourDataProcess::CWDSHourDataProcess(){
    //m_//pSysLogger           = NULL;
    m_pSendSocket          = NULL;
    m_pRecvSocket          = NULL;
    m_nFuncType            = -1;
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~CWDSHourDataProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSHourDataProcess::~CWDSHourDataProcess(){
    //m_//pSysLogger  = NULL;
    m_pRecvSocket = NULL;
    if(m_pSendSocket != NULL) {
        if (!m_pSendSocket->IsClosed()){
            m_pSendSocket->Close();
        }
        delete m_pSendSocket;
        m_pSendSocket = NULL;
    }
}

/******************************************************************************
    处理名        ：  业务处理
    函数名        ：  Do()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
void CWDSHourDataProcess::Do(){
    //m_//pSysLogger     = CObjectFactory::GetInstance()->GetSysLogger();
    printfs(1,"<CWDSHourDataProcess::Do()> WDSHourDataProcess start!");

    int nStatus = 0;
    int nRet = 0;

    // 取得小时气象信息的长度
    int nLength = sizeof(stElementsHourData) - sizeof(stHeader);
    stElementsHourData elementsHourData;
    memset(&elementsHourData,  0, sizeof(elementsHourData));

    WDS_DBHelper* pWDSHelper = NULL;
    do{
        if (m_pRecvSocket == NULL){
            printfs(0,"<CWDSHourDataProcess::Do()> m_pRecvSocket == NULL");
            nStatus = 0;
            break;;
        }

        // 【业务处理】1. 取得端口传入的小时气象数据信息数据
        nRet = m_pRecvSocket->Receive((char*)(&elementsHourData) + sizeof(stHeader), nLength);
        if(nRet == 0) {
            printfs(0,"<CWDSHourDataProcess::Do()> Receive packet time out");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        if(nRet == -1) {
            printfs(0, "<CWDSHourDataProcess::Do()> Receive packet failed");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        /*m_printfs(2, "<CWDSHourDataProcess::Do()> Receive struct info:cCurTime[%s], cStationID[%.10s], fCurTemp[%f], fMaxTemp[%f], fMinTemp[%f], fRainfall[%f], fCurAP[%f], fMaxAP[%f], fMinAP[%f], fWindVelocity[%f], cWindDirection[%.4s], fHumidity[%f]",
                             elementsHourData.cCurTime,
                             elementsHourData.cStationID,
                             elementsHourData.fCurTemp,
                             elementsHourData.fMaxTemp,
                             elementsHourData.fMinTemp,
                             elementsHourData.fRainfall,
                             elementsHourData.fCurAP,
                             elementsHourData.fMaxAP,
                             elementsHourData.fMinAP,
                             elementsHourData.fWindVelocity,
                             elementsHourData.cWindDirection,
                             elementsHourData.fHumidity);
*/
        // 修改方向为标准方向
        nStatus = CObjectFactory::GetInstance()->ChangeDirection(elementsHourData.cWindDirection);
        if (!nStatus){
            printfs(0,"<CWDSHourDataProcess::Do()> Wind direction error! input wind direct [%s]", elementsHourData.cWindDirection);
            break;
        }

        MYSQL* pMysqlConnection = CObjectFactory::GetInstance()->GetMySQLPool()->GetIdleMySql();
        if (pMysqlConnection == NULL){
            printfs(0,"<CWDSHourDataProcess::Do()> No enough mysql connections!");
            nStatus = 0;
            break;
        }
        pWDSHelper = new WDS_DBHelper();
        if (pWDSHelper == NULL){
            printfs(0,"<CWDSHourDataProcess::Do()> Can not connect to DB!");
            nStatus = 0;
            break;
        }
        pWDSHelper->SetMysqlHandle(pMysqlConnection);

        // 业务处理，删除已存在的小时气象数据
        nStatus = pWDSHelper->DeleteElementsHourData(&elementsHourData);
        if (!nStatus){
            printfs(0,"<CWDSHourDataProcess::Do()> DeleteElementsHourData() operation failed!");
            break;
        }

        // 业务处理，向DB插入小时气象数据
        nStatus = pWDSHelper->InsertElementsHourData(&elementsHourData);
        if (!nStatus){
            printfs(0,"<CWDSHourDataProcess::Do()> InsertElementsHourData() operation failed!");
            break;
        }
    }
    while(0);
    if (pWDSHelper){
        CObjectFactory::GetInstance()->GetMySQLPool()->SetIdleMysql(pWDSHelper->GetMysqlHandle());
        delete pWDSHelper;
        pWDSHelper = NULL;
    }

    stAnswer answerInfo;
    memset(&answerInfo, 0, sizeof(answerInfo));
    answerInfo.nVerify1 = elementsHourData.nVerify1;
    answerInfo.nStatus = nStatus;
    answerInfo.nVerify2 = elementsHourData.nVerify1;

    // 应答电文送信
    nRet = m_pRecvSocket->Send((char*)(&answerInfo), sizeof(answerInfo));
    if(nRet == 0) {
        printfs(0,"<CWDSHourDataProcess::Do()> Send status code time out");
    }
    if(nRet == -1) {
        printfs(0, "<CWDSHourDataProcess::Do()> Send status code failed");
    }
    m_pRecvSocket->Close();

    printfs(1,"<CWDSHourDataProcess::Do()> WDSHourDataProcess end!");
}

/******************************************************************************
    End
******************************************************************************/
