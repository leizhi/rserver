/*****************************************************************************/
/*                                                                           */
/*           系统名        ：        内蒙气象局后台服务                      */
/*           客户名        ：        内蒙气象局                              */
/*           机能名        ：        静态图片业务处理类                      */
/*           程序名        ：        WDSStationImageProcess.cpp              */
/*                                                                           */
/*****************************************************************************/
/*                                                                           */
/*                VERSION    DATE      BY      CHANGE/COMMENT                */
/*---------------------------------------------------------------------------*/
/*                V1.00      09-11-11          Create                        */
/*                                                                           */
/*****************************************************************************/

#include "WDSStationImageProcess.h"
#include "WDS_DBHelper.h"
#include "ObjectFactory.h"
#include "Common.h"

/******************************************************************************
    处理名        ：  类的构造，类的成员变量的初始化处理
    函数名        ：  CWDSStationImageProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSStationImageProcess::CWDSStationImageProcess(){
    m_pSendSocket          = NULL;
    m_pRecvSocket          = NULL;
    m_nFuncType            = -1;
    m_pFile                = NULL;
}

/******************************************************************************
    处理名        ：  类的析构处理
    函数名        ：  ~CWDSStationImageProcess()
    参数          ：  无
    返回值        ：  无
******************************************************************************/
CWDSStationImageProcess::~CWDSStationImageProcess(){
    if(m_pFile != NULL) {
        m_pFile->Close();
        delete m_pFile;
        m_pFile = NULL;
    }
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
void CWDSStationImageProcess::Do(){
    printfs(1,"<CWDSStationImageProcess::Do()> WDSStationImageProcess start!");

    int nStatus = 0;
    int nRet = 0;

    if (NULL != m_pFile){
        m_pFile->Close();
        delete m_pFile;
        m_pFile = NULL;
    }

    // 取得信息的长度
    int nLength = sizeof(stStationImage) - sizeof(stHeader);
    stStationImage stationImage;
    memset(&stationImage,  0, sizeof(stationImage));
    char* pImage = NULL;

    WDS_DBHelper* pWDSHelper = NULL;
    do{
        if (m_pRecvSocket == NULL){
            printfs(0,"<CWDSrealtimeWeatherReportProcess::Do()> m_pRecvSocket == NULL");
            break;;
        }

        // step 1. 取得端口传入的信息数据
        nRet = m_pRecvSocket->Receive((char*)(&stationImage) + sizeof(stHeader), nLength);
        if(nRet == 0) {
            printfs(0,"<CWDSStationImageProcess::Do()> Receive packet time out");
            m_pRecvSocket->Close();
            break;
        }
        if(nRet == -1) {
            printfs(0, "<CWDSStationImageProcess::Do()> Receive packet failed");
            m_pRecvSocket->Close();
            break;
        }
        /*m_printfs(2, "<CWDSStationImageProcess::Do()> Receive struct info:cCurTime[%s], cStationID[%.10s], cDirection[%s], nPicSize[%d]",
                             stationImage.cCurTime,
                             stationImage.cStationID,
                             stationImage.cDirection,
                             stationImage.nPicSize);
*/
        // 修改方向为标准方向
        nStatus = CObjectFactory::GetInstance()->ChangeDirection(stationImage.cDirection);
        if (!nStatus){
            printfs(0,"<CWDSStationImageProcess::Do()> Wind direction error! input wind direct [%s]", stationImage.cDirection);
            break;
        }

        // step 2. 取得端口传入的图像数据
        pImage = new char[stationImage.nPicSize];
        nRet = m_pRecvSocket->Receive(pImage, stationImage.nPicSize);
        if(nRet == 0) {
            printfs(0,"<CWDSStationImageProcess::Do()> Receive packet time out");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }
        if(nRet == -1) {
            printfs(0, "<CWDSStationImageProcess::Do()> Receive packet failed");
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }

        char* pImgPath = NULL;
        char pFilePath[MAX_FILEPATH_LENGTH] = "\0";
        char pFileName[MAX_FILEPATH_LENGTH] = "\0";
        char pFile[MAX_FILEPATH_LENGTH] = "\0";
        long lId = -1;
        // 1.业务处理，向本机目录保存图像
        pImgPath = CObjectFactory::GetInstance()->GetSystemConfigInfo()->GetImgPath();
        int year = 0;
        int month = 0;
        int day = 0;
        int hour = 0;
        int minute = 0;
        int second = 0;
        sscanf(stationImage.cCurTime, "%d-%d-%d %d:%d:%d", &year, &month, &day, &hour, &minute, &second);

        // 生成文件路径("/weather_station/station_image/台站号/年(4位)月(2位)日(2位)/")
        // 例如：/weather_station/station_image/57078/20100107/
        sprintf( pFilePath, "%s%s/%04d%02d%02d/", pImgPath, stationImage.cStationID, year, month, day);
        // 创建目录结构
        if (!CreateFolder(pFilePath)){
            printfs(0, "<CWDSStationImageProcess::Do()> Create station image folder failed folder:[%s]", pFilePath);
            m_pRecvSocket->Close();
            nStatus = 0;
            break;
        }

        sprintf(pFileName, "%.10s_%04d%02d%02d%02d%02d%02d_%s.jpg", stationImage.cStationID, year, month, day, hour, minute, second, stationImage.cDirection);

        sprintf( pFile, "%s%s", pFilePath, pFileName );
        m_pFile = new BVodFile(pFile);
        printfs(2, "<CWDSStationImageProcess::Do()> Ready to create image file[%s]", pFile );

        int32_t nRc = m_pFile->Open(FILE_WRITE_MODE);
        if(nRc != 0) {
            delete m_pFile;
            m_pFile = NULL;
            nStatus = 0;
            break;
        }
//        m_pFile->Seek(0, SEEK_BEGIN);
        m_pFile->Write(pImage, stationImage.nPicSize);

        m_pFile->Close();
        delete m_pFile;
        m_pFile = NULL;

        MYSQL* pMysqlConnection = CObjectFactory::GetInstance()->GetMySQLPool()->GetIdleMySql();
        if (pMysqlConnection == NULL){
            printfs(0,"<CWDSStationImageProcess::Do()> No enough mysql connections!");
            nStatus = 0;
            break;
        }
        pWDSHelper = new WDS_DBHelper();
        if (pWDSHelper == NULL){
            printfs(0,"<CWDSStationImageProcess::Do()> Can not connect to DB!");
            nStatus = 0;
            break;
        }
        pWDSHelper->SetMysqlHandle(pMysqlConnection);

        // 2.业务处理，向DB的tb_gb_file_info表插入图像数据
        nStatus = pWDSHelper->InsertImageFile(pFilePath, pFileName, pFileName, stationImage.cCurTime);
        if (!nStatus){
            printfs(0,"<CWDSStationImageProcess::Do()> InsertImageFile() operation failed!");
            break;
        }

        // 3.业务处理，从DB中取得tb_gb_file_info表中的刚才插入的图片文件的ID
        nStatus = pWDSHelper->GetImageFileID(pFileName, pFilePath, lId);
        if (!nStatus){
            printfs(0,"<CWDSStationImageProcess::Do()> GetImageFileID() operation failed!");
            break;
        }

        // 4.业务处理，向DB的tb_station_image表插入台站图像信息
        nStatus = pWDSHelper->InsertStationImage(&stationImage, lId);
        if (!nStatus){
            printfs(0,"<CWDSStationImageProcess::Do()> InsertStationImage() operation failed!");
            break;
        }
    }
    while(0);
    if (pWDSHelper){
        CObjectFactory::GetInstance()->GetMySQLPool()->SetIdleMysql(pWDSHelper->GetMysqlHandle());
        delete pWDSHelper;
        pWDSHelper = NULL;
    }

    if (pImage){
        delete [] pImage;
        pImage = NULL;
    }

    stAnswer answerInfo;
    memset(&answerInfo, 0, sizeof(answerInfo));
    answerInfo.nVerify1 = stationImage.nVerify1;
    answerInfo.nStatus = nStatus;
    answerInfo.nVerify2 = stationImage.nVerify2;

    // 应答电文送信
    nRet = m_pRecvSocket->Send((char*)(&answerInfo), sizeof(answerInfo));
    if(nRet == 0) {
        printfs(0,"<CWDSStationImageProcess::Do()> Send status code time out");
    }
    if(nRet == -1) {
        printfs(0, "<CWDSStationImageProcess::Do()> Send status code failed");
    }
    m_pRecvSocket->Close();

    printfs(1,"<CWDSStationImageProcess::Do()> WDSStationImageProcess end!");
}

/******************************************************************************
    End
******************************************************************************/
