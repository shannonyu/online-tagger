#ifndef __LTP_LOG_H__
#define __LTP_LOG_H__

/*
 * Default log level
 */
#define LTP_LOG_DEBUG   0
#define LTP_LOG_TRACE   1
#define LTP_LOG_WARNING 2
#define LTP_LOG_FATAL   3

/*
 * Function for writing log
 *
 *  @param[in]  lvl the log level
 *  @param[in]  fmt the format string
 *  @param[in]  ... the variable parameter
 */
void write_log(int lvl, char *fmt, ...);

#define DEBUG_LOG(msg, ...) do { \
    write_log(LTP_LOG_DEBUG, msg, ##__VA_ARGS__); \
} while (0);

#define TRACE_LOG(msg, ...) do { \
    write_log(LTP_LOG_TRACE, msg, ##__VA_ARGS__); \
} while (0);

#define WARNING_LOG(msg, ...) do { \
    write_log(LTP_LOG_WARNING, "%s:line %d:%s(): " msg, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
} while (0);

#define FATAL_LOG(msg, ...) do { \
    write_log(LTP_LOG_FATAL, "%s:line %d:%s(): " msg, __FILE__, __LINE__, __FUNCTION__, ##__VA_ARGS__); \
} while (0);

#endif  // end for __LTP_LOG_H__

