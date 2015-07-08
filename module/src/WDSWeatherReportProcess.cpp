/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        实时重大天气业务处理类                  */
/*           程序名        ：        WDSWeatherReportProcess.cpp             */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "WDSWeatherReportProcess.h"
#include "WDS_DBHelper.h"
#include "ObjectFactory.h"

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  CWDSWeatherReportProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSWeatherReportProcess::CWDSWeatherReportProcess(){
    //m_//pSysLogger           = NULL;
    m_pSendSocket          = NULL;
    m_pRecvSocket          = NULL;
    m_nFuncType            = -1;
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~CWDSWeatherReportProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSWeatherReportProcess::~CWDSWeatherReportProcess(){
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
void CWDSWeatherReportProcess::Do(){
    //m_//pSysLogger     = CObjectFactory::GetInstance()->GetSysLogger();
    printfs(1,"<CWDSWeatherReportProcess::Do()> WDSWeatherReportProcess start!");

    int nStatus = 0;
    int nRet = 0;

    // 取得小时气象信息的长度
    int nLength = sizeof(stRealtimeWeatherReport) - sizeof(stHeader);
    stRealtimeWeatherReport realtimeWeatherReport;
    memset(&realtimeWeatherReport,  0, sizeof(realtimeWeatherReport));

    WDS_DBHelper* pWDSHelper = NULL;
    do{
        if (m_pRecvSocket == NULL){
            printfs(0,"<CWDSWeatherReportProcess::Do()> m_pRecvSocket == NULL");
            nStatus = 0;
            break;;
        }

        // 【业务处理】1. 取得端口传入的小时气象数据信息数据
        nRet = m_pRecvSocket->Receive((char*)(&realtimeWeatherReport) + sizeof(stHeader), nLength);
        if(nRet == 0) {
            printfs(0,"<CWDSWeatherReportProcess::Do()> Receive packet time out");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        if(nRet == -1) {
            printfs(0, "<CWDSWeatherReportProcess::Do()> Receive packet failed");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        printfs(2, "<CWDSWeatherReportProcess::Do()> Receive struct info:cCurTime[%s], cStationID[%.10s], cTime[%s], nFlag[%d], nVisibility[%d], nEventNo[%d], cEventComment[%s]",
                             realtimeWeatherReport.cCurTime,
                             realtimeWeatherReport.cStationID,
                             realtimeWeatherReport.cTime,
                             realtimeWeatherReport.nFlag,
                             realtimeWeatherReport.nVisibility,
                             realtimeWeatherReport.nEventNo,
                             realtimeWeatherReport.cEventComment);

        MYSQL* pMysqlConnection = CObjectFactory::GetInstance()->GetMySQLPool()->GetIdleMySql();
        if (pMysqlConnection == NULL){
            printfs(0,"<CWDSWeatherReportProcess::Do()> No enough mysql connections!");
            nStatus = 0;
            break;
        }
        pWDSHelper = new WDS_DBHelper();
        if (pWDSHelper == NULL){
            printfs(0,"<CWDSWeatherReportProcess::Do()> Can not connect to DB!");
            nStatus = 0;
            break;
        }
        pWDSHelper->SetMysqlHandle(pMysqlConnection);

        // 业务处理
        if (realtimeWeatherReport.nFlag == WEATHER_REPORT_BEGIN){
            // step 1.1业务处理，实时重大天气开始，从DB取得最近的一条同类型且未结束的重大气象数据ID
            int nID = -1;
            nStatus = pWDSHelper->GetMaxWeatherReportID(&realtimeWeatherReport, nID);
            if (!nStatus){
                break;
            }
            if (nID != -1){
                // step 1.1.1业务处理，有未结束的同类型重大天气数据，结束该条重大天气数据
                nStatus = pWDSHelper->UpdateRealtimeWeatherReport(&realtimeWeatherReport, nID);
                if (!nStatus){
                    printfs(0,"<CWDSWeatherReportProcess::Do()> UpdateRealtimeWeatherReport() failed!");
                    break;
                }
                printfs(1, "<CWDSWeatherReportProcess::Do()> Find begin weather that does not stoped, station_id:%s,current_time:%s,weather_type:%d",
                                     realtimeWeatherReport.cStationID,
                                     realtimeWeatherReport.cCurTime,
                                     realtimeWeatherReport.nEventNo);
            }
            // step 1.1.2业务处理，向DB插入实时重大气象数据
            nStatus = pWDSHelper->InsertRealtimeWeatherReport(&realtimeWeatherReport);
        }
        else if(realtimeWeatherReport.nFlag == WEATHER_REPORT_STOP){
            // step 1.2业务处理，实时重大天气结束，从DB取得最近的一条同类型且未结束的重大气象数据ID
            int nID = -1;
            nStatus = pWDSHelper->GetMaxWeatherReportID(&realtimeWeatherReport, nID);
            if (!nStatus){
                printfs(0,"<CWDSWeatherReportProcess::Do()> GetMaxWeatherReportID() failed!");
                break;
            }
            if (nID != -1){
                // step 1.2.1业务处理，找到同类型重大天气数据，更新结束时间
                nStatus = pWDSHelper->UpdateRealtimeWeatherReport(&realtimeWeatherReport, nID);
                if (!nStatus){
                    printfs(0,"<CWDSWeatherReportProcess::Do()> UpdateRealtimeWeatherReport() failed!");
                    break;
                }
            }
            else{
                // step 1.2.2业务处理，没有同类型重大天气数据，处理结束
                printfs(0, "<CWDSWeatherReportProcess::Do()> No nearly begin weather type, station_id:%s,current_time:%s,weather_type:%d",
                                     realtimeWeatherReport.cStationID,
                                     realtimeWeatherReport.cCurTime,
                                     realtimeWeatherReport.nEventNo);
            }
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
    answerInfo.nVerify1 = realtimeWeatherReport.nVerify1;
    answerInfo.nStatus = nStatus;
    answerInfo.nVerify2 = realtimeWeatherReport.nVerify2;

    // 应答电文送信
    nRet = m_pRecvSocket->Send((char*)(&answerInfo), sizeof(answerInfo));
    if(nRet == 0) {
        printfs(0,"<CWDSWeatherReportProcess::Do()> Send status code time out");
    }
    if(nRet == -1) {
        printfs(0, "<CWDSWeatherReportProcess::Do()> Send status code failed");
    }
    m_pRecvSocket->Close();

    printfs(1,"<CWDSWeatherReportProcess::Do()> WDSWeatherReportProcess end!");
}

/******************************************************************************
    End
******************************************************************************/
