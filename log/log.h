#pragma once

#ifndef LOG_MAX_LEVEL
#define LOG_MAX_LEVEL	5

#define LOG_ARGS int level, void *object, const char *function, int line, const char *fmt, ...
typedef void (*log_func)(LOG_ARGS);

#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_MAX_LEVEL
#endif

#if LOG_LEVEL < LOG_MAX_LEVEL

#ifdef __cplusplus
extern "C" {
#endif

extern log_func _log_func;

int log_initialize(const char *uri);
int log_close();

#ifdef __cplusplus
}
#endif

#define logg(level, fmt, ...) _log_func(level, 0, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define logm(level, fmt, ...) _log_func(level, this, __FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#else

#define log_initialize(x)	0
#define log_close()			0

#define logg(level, fmt, ...)
#define logm(level, fmt, ...)

#endif

#if LOG_LEVEL <= 0

#define logg_debug(fmt, ...) logg(0, fmt, ##__VA_ARGS__)
#define logm_debug(fmt, ...) logm(0, fmt, ##__VA_ARGS__)

#else

#define logg_debug(fmt, ...)
#define logm_debug(fmt, ...)

#endif

#if LOG_LEVEL <= 1

#define logg_info(fmt, ...) logg(1, fmt, ##__VA_ARGS__)
#define logm_info(fmt, ...) logm(1, fmt, ##__VA_ARGS__)

#else

#define logg_info(fmt, ...)
#define logm_info(fmt, ...)

#endif

#if LOG_LEVEL <= 2

#define logg_warning(fmt, ...) logg(2, fmt, ##__VA_ARGS__)
#define logm_warning(fmt, ...) logm(2, fmt, ##__VA_ARGS__)

#else

#define logg_warning(fmt, ...)
#define logm_warning(fmt, ...)

#endif

#if LOG_LEVEL <= 3

#define logg_critical(fmt, ...) logg(3, fmt, ##__VA_ARGS__)
#define logm_critical(fmt, ...) logm(3, fmt, ##__VA_ARGS__)

#else

#define logg_critical(fmt, ...)
#define logm_critical(fmt, ...)

#endif

#if LOG_LEVEL <= 4

#define logg_fatal(fmt, ...) logg(4, fmt, ##__VA_ARGS__)
#define logm_fatal(fmt, ...) logm(4, fmt, ##__VA_ARGS__)

#else

#define logg_fatal(fmt, ...)
#define logm_fatal(fmt, ...)

#endif

#endif // LOG_MAX_LEVEL
