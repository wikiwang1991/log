#pragma once

#ifdef ENABLE_LOG

#define LOG_ARGS const char *function, int line, const char *fmt, ...
typedef void (*log_func)(LOG_ARGS);

#ifdef __cplusplus
extern "C" {
#endif

extern log_func _log_debug_0_0;
extern log_func _log_info_0_0;
extern log_func _log_warning_0_0;
extern log_func _log_critical_0_0;
extern log_func _log_fatal_0_0;

int log_initialize(const char *file);
void log_close();

#ifdef __cplusplus
}
#endif

#define log_debug(fmt, ...) _log_debug_0_0(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) _log_info_0_0(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) _log_warning_0_0(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_critical(fmt, ...) _log_critical_0_0(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...) _log_fatal_0_0(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)

#else

#define log_initialize(x)
#define log_close()

#define log_debug(fmt, ...)
#define log_info(fmt, ...)
#define log_warning(fmt, ...)
#define log_critical(fmt, ...)
#define log_fatal(fmt, ...)

#endif
