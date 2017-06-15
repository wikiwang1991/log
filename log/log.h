#pragma once

#ifndef LOG_MAX_LEVEL
#define LOG_MAX_LEVEL	5

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_MAX_LEVEL
#endif

#if LOG_LEVEL < LOG_MAX_LEVEL

#define LOG_ARGS int level, void *object, const char *function, int line, const char *fmt, ...
typedef void (*log_func)(LOG_ARGS);

#ifdef __cplusplus
extern "C" {
#endif

extern log_func _log_func;

int log_initialize(const char *uri);
int log_close();

#ifdef __cplusplus
}
#endif

#define log(level, fmt, ...) _log_func(level, 0, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define logm(level, fmt, ...) _log_func(level, this, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#else

#define log_initialize(x)	0
#define log_close()			0

#define log(level, fmt, ...)
#define logm(level, fmt, ...)

#endif

#if LOG_LEVEL <= 0

#define log_debug(fmt, ...) log(0, fmt, ##__VA_ARGS__)
#define logm_debug(fmt, ...) logm(0, fmt, ##__VA_ARGS__)

#else

#define log_debug(fmt, ...)
#define logm_debug(fmt, ...)

#endif

#if LOG_LEVEL <= 1

#define log_info(fmt, ...) log(1, fmt, ##__VA_ARGS__)
#define logm_info(fmt, ...) logm(1, fmt, ##__VA_ARGS__)

#else

#define log_info(fmt, ...)
#define logm_info(fmt, ...)

#endif

#if LOG_LEVEL <= 2

#define log_warning(fmt, ...) log(2, fmt, ##__VA_ARGS__)
#define logm_warning(fmt, ...) logm(2, fmt, ##__VA_ARGS__)

#else

#define log_warning(fmt, ...)
#define logm_warning(fmt, ...)

#endif

#if LOG_LEVEL <= 3

#define log_critical(fmt, ...) log(3, fmt, ##__VA_ARGS__)
#define logm_critical(fmt, ...) logm(3, fmt, ##__VA_ARGS__)

#else

#define log_critical(fmt, ...)
#define logm_critical(fmt, ...)

#endif

#if LOG_LEVEL <= 4

#define log_fatal(fmt, ...) log(4, fmt, ##__VA_ARGS__)
#define logm_fatal(fmt, ...) logm(4, fmt, ##__VA_ARGS__)

#else

#define log_fatal(fmt, ...)
#define logm_fatal(fmt, ...)

#endif

#endif // LOG_MAX_LEVEL
