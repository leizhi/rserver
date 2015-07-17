AUTOMAKE_OPTIONS = foreign

AM_CPPFLAGS=-I../include -I/usr/include/mysql

AM_LDFLAGS=-L/usr/lib64/mysql  -lmysqlclient -lpthread -O3 -Wall

bin_PROGRAMS = Weather_Data_Service

Weather_Data_Service_SOURCES = Weather_Data_Service.c conf.c log.c

#Weather_Data_Service_SOURCES = WDSSyncTimeProcess.c TCPSocket.c  WDSHourDataProcess.c MySQLPool.c WDSStationImageProcess.c ConfigFile.c ProcessThread.c ObjectFactory.c WDSRealtimeStatusProcess.c log.c MysqlHelper.c PacketAnalysisProcess.c WDSStationConfigProcess.c SystemConfigInfo.c WDSStationRegisterProcess.c VodFile.c WDSMinuteDataProcess.c WDS_DBHelper.c Weather_Data_Service.c Thread.c Mutex.c SocketMonitorProcess.c WDSWeatherReportProcess.c Process.c WDSRealtimeAlertProcess.c ChannelItem.c ProcessFactory.c Common.c ThreadPool.c 
