#include <stdio.h>

// 日志最大长度1024
#define TONGSHUAI_MAX_LOGMSG_LEN    1024
#define TONGSHUAI_LOGMSG_LEVEL      0
#define TONGSHUAI_LOGFILE           "tongshuailog.txt"

#ifdef __GNUC__
void tongshuaiLog(int level, const char *fmt, ...)
    __attribute__((format(printf, 2, 3)));
#else
void tongshuaiLog(int level, const char *fmt, ...);
#endif

