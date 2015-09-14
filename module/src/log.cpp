#include "log.h"

char nows[20]="\0";

char * getNow(){

    time_t timep;
    struct tm *p;
    time(&timep);
    p=gmtime(&timep);

    strftime(nows,20,"%F %T",p);

    return nows;
}

void printfs(unsigned int nlevel, const char * fmt, ...){

	int dlevel = 1;
	if(dlevel==1){
        char buf[2048]    = "\0";
        char logbuf[4096] = "\0";
        char cType[10]    = "\0";

        if(nlevel == 1) {
            strcpy(cType,"INFO");
        }
        else if(nlevel == 2) {
            strcpy(cType,"DEBUG");
        }
        else if(nlevel == 3) {
             strcpy(cType,"ALL");
        }else{
            nlevel = 0;
            strcpy(cType,"ERROR");
        }
        
        va_list va_alist;
        
        va_start(va_alist, fmt);

        vsnprintf(buf, sizeof(buf), fmt, va_alist);

        va_end (va_alist);

	const char * filename = "rserver.log";

	int flength = -1;
	FILE* fp = fopen( filename, "r" );

	if(fp!=NULL){
		fseek(fp, 0L, SEEK_END );
		flength = ftell(fp);
		fclose(fp);
	}

	ofstream *of =NULL;

	if(flength<1024*1024){
		of = new ofstream(filename,ios::app);
	}else{
		of = new ofstream(filename,ios::out);
	}

	time_t t = time(0);
	char times[64]={0};

	strftime(times, sizeof(times), "%Y/%m/%d %X:",localtime(&t));

	sprintf(logbuf, "%s [%s] %s", times, cType, buf);

	*of<<logbuf<<endl;

	printf("%s\n", logbuf);

	(*of).close();

	if(of != NULL){
		delete of;
		of = NULL;
	}
    }
}
