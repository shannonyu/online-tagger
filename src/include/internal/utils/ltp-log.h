#ifndef __LTP_LOG_H__
#define __LTP_LOG_H__

#include <iostream>
#include <sstream>
#include <ctime>
#include <stdarg.h>
#include <cstdio>
using namespace std;

#define LTP_LOG_DEBUG   0
#define LTP_LOG_TRACE   1
#define LTP_LOG_WARNING 2
#define LTP_LOG_FATAL   3

void write_log(int lvl, char *fmt, ...);

#define debug_log(msg) do { \
    write_log(LTP_LOG_DEBUG, "%s", msg); \
} while (0);

#define trace_log(msg) do { \
    write_log(LTP_LOG_TRACE, "%s", msg); \
} while (0);

#define warning_log(msg) do { \
    write_log(LTP_LOG_WARNING, "%s:%d:%s(): %s", __FILE__, __LINE__, __FUNCTION__, msg); \
} while (0);

#define fatal_log(msg) do { \
    write_log(LTP_LOG_FATAL, "%s:%d:%s(): %s", __FILE__, __LINE__, __FUNCTION__, msg); \
} while (0);

#endif  // end for __LTP_LOG_H__

