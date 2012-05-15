#include "ltp-log.h"

string asctime(struct tm* timenow) {
    ostringstream S;
    S << 1900 + timenow->tm_year;
    S << "/";
    S << 1 + timenow->tm_mon;
    S << "/";
    S << timenow->tm_mday;
    S << " ";
    S << timenow->tm_hour;
    S << ":";
    S << timenow->tm_min;
    S << ":";
    S << timenow->tm_sec;
    return S.str();
}

void write_log(int lvl, char *fmt, ...) {
    if (lvl == 0) {
        fprintf(stderr, "[DEBUG] ");
    } else if (lvl == 1) {
        fprintf(stderr, "[TRACE] ");
    } else if (lvl == 2) {
        fprintf(stderr, "[WARNING] ");
    } else if (lvl == 3) {
        fprintf(stderr, "[FATAL] ");
    } else {
        fprintf(stderr, "[UNKNOWN] ");
    }

    time_t now;
    struct tm *timenow;

    time(&now);
    timenow = localtime(&now);
    fprintf( stderr, "%s ", asctime(timenow).c_str());

    va_list list;
    va_start(list, fmt);
    vfprintf( stderr, fmt, list);
    va_end(list);
    fprintf( stderr, "\n");
}


