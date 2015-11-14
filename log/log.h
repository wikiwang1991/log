#pragma once

#define LOG_ARGS const char *function, int line, const char *fmt, ...
typedef void (*log_func)(LOG_ARGS);

#ifdef __cplusplus
extern "C" {
#endif

extern log_func _log_debug;
extern log_func _log_info;
extern log_func _log_warning;
extern log_func _log_critical;
extern log_func _log_fatal;

void log_initialize();
void log_close();

#ifdef __cplusplus
}
#endif

#define log_debug(fmt, ...) _log_debug(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_info(fmt, ...) _log_info(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_warning(fmt, ...) _log_warning(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_critical(fmt, ...) _log_critical(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
#define log_fatal(fmt, ...) _log_fatal(__FUNCTION__, __LINE__, fmt, ##__VA_ARGS__)
