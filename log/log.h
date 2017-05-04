#pragma once

#ifndef LOG_MAX_LEVEL
#define LOG_MAX_LEVEL	5

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_MAX_LEVEL
#endif

#ifdef __cplusplus
extern "C" {
#endif

#if LOG_LEVEL < LOG_MAX_LEVEL

#define LOG_ARGS const char *function, int line, const char *fmt, ...
typedef void (*log_func)(LOG_ARGS);

extern log_func _log_func[LOG_MAX_LEVEL];

int log_initialize(const char *uri);
int log_close();

#define log(level, fmt, ...) _log_func[level](__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#else

#define log_initialize(x)	0
#define log_close()			0

#define log(level, fmt, ...)

#endif

#if LOG_LEVEL <= 0

#define log_debug(fmt, ...) log(0, fmt, ##__VA_ARGS__)

#else

#define log_debug(fmt, ...)

#endif

#if LOG_LEVEL <= 1

#define log_info(fmt, ...) log(1, fmt, ##__VA_ARGS__)

#else

#define log_info(fmt, ...)

#endif

#if LOG_LEVEL <= 2

#define log_warning(fmt, ...) log(2, fmt, ##__VA_ARGS__)

#else

#define log_warning(fmt, ...)

#endif

#if LOG_LEVEL <= 3

#define log_critical(fmt, ...) log(3, fmt, ##__VA_ARGS__)

#else

#define log_critical(fmt, ...)

#endif

#if LOG_LEVEL <= 4

#define log_fatal(fmt, ...) log(4, fmt, ##__VA_ARGS__)

#else

#define log_fatal(fmt, ...)

#endif

#ifdef __cplusplus
}
#endif

#endif // LOG_MAX_LEVEL
