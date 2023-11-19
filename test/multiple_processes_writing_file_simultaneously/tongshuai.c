#include "tongshuai.h"
#include <stdlib.h>
#include <stdarg.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>

void tongshuaiLogRaw(int level, const char *msg) {
    const char *c = ".-*#";
    FILE *fp;
    char buf[64];
    if (level < TONGSHUAI_LOGMSG_LEVEL) return;

    fp = fopen(TONGSHUAI_LOGFILE,"a");
    if (!fp) return;
    int off;
    struct timeval tv;

    gettimeofday(&tv,NULL);
    off = strftime(buf,sizeof(buf),"%d %b %H:%M:%S.",   (&tv.tv_sec));
    snprintf(buf+off,sizeof(buf)-off,"%03d",(int)tv.tv_usec/1000);
    fprintf(fp,"[%d] %s %c %s\n",(int)getpid(),buf,c[level],msg);
    fflush(fp);

    fclose(fp);
}

/* 仿照redis写的日志函数 */
void tongshuaiLog(int level, const char *fmt, ...) {
    va_list ap;
    char msg[TONGSHUAI_MAX_LOGMSG_LEN];

    if ((level&0xff) < TONGSHUAI_LOGMSG_LEVEL) return;

    va_start(ap, fmt);
    vsnprintf(msg, sizeof(msg), fmt, ap);
    va_end(ap);

    tongshuaiLogRaw(level,msg);
}