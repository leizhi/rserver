AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS=-I../include -I/usr/include/mysql

AM_LDFLAGS=-L/usr/lib64/mysql  -lmysqlclient -lpthread -O3 -Wall

bin_PROGRAMS = Weather_Data_Service

Weather_Data_Service_SOURCES = WDSSyncTimeProcess.cpp TCPSocket.cpp  WDSHourDataProcess.cpp MySQLPool.cpp WDSStationImageProcess.cpp ConfigFile.cpp ProcessThread.cpp ObjectFactory.cpp WDSRealtimeStatusProcess.cpp log.cpp MysqlHelper.cpp PacketAnalysisProcess.cpp WDSStationConfigProcess.cpp SystemConfigInfo.cpp WDSStationRegisterProcess.cpp VodFile.cpp WDSMinuteDataProcess.cpp WDS_DBHelper.cpp Weather_Data_Service.cpp Thread.cpp Mutex.cpp SocketMonitorProcess.cpp WDSWeatherReportProcess.cpp Process.cpp WDSRealtimeAlertProcess.cpp ChannelItem.cpp ProcessFactory.cpp Common.cpp ThreadPool.cpp 
