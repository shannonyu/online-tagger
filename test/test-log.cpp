#include "ltp-log.h"

int main() {
    DEBUG_LOG("This log is for debug.");
    TRACE_LOG("This log is for trace.");
    WARNING_LOG("This log is a warning log.");
    FATAL_LOG("This log is a fatal log.");

    DEBUG_LOG("log with number [%d]", 10);
    return 0;
}
