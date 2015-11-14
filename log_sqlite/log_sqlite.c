#include "sqlite3.h"
#include <time.h>
#include <stdio.h>

#define LOG_ARGS const char *function, int line, const char *fmt, ...
#define BUFFER_SIZE 1024

#ifdef WIN32
#include <windows.h>

#define inline __inline

CRITICAL_SECTION cs;
#else
#endif

static int reference_count = 0;
static sqlite3 *sqlite3_t;
static sqlite3_stmt *sqlite3_stmt_t;

int log_initialize()
{
	if (reference_count) goto noerr;
	char buffer[BUFFER_SIZE];
#ifdef WIN32
	InitializeCriticalSection(&cs);
	if (!GetModuleFileNameA(0, buffer, BUFFER_SIZE))
		return -1;
#else
#endif
	int pos_slash, pos_point;
	for (int i = 0; i < BUFFER_SIZE; ++i) {
		switch (buffer[i]) {
		case '\\':
			pos_slash = i;
			break;
		case '.':
			pos_point = i;
			break;
		case '\0':
			goto tag_break;
		}
		continue;
	tag_break:
		break;
	}
	int name_length = pos_point - pos_slash - 1;
	memmove(buffer, buffer + pos_slash + 1, name_length);
	strcpy(buffer + name_length, ".log.db");
	int ret = sqlite3_open(buffer, &sqlite3_t);
	if (ret != SQLITE_OK) goto err;
	sqlite3_exec(sqlite3_t, "pragma journal_mode = wal;", 0, 0, 0);
	sqlite3_exec(sqlite3_t, "pragma synchronous = normal;", 0, 0, 0);
	time_t t;
	time(&t);
	struct tm *ti = localtime(&t);
	char table[32];
	sprintf(table, "[%d:%d:%d %d/%d/%d]",
			ti->tm_hour, ti->tm_min, ti->tm_sec,
			ti->tm_mon + 1, ti->tm_mday, ti->tm_year + 1900);
	sprintf(buffer, "create table %s(time timestamp default current_timestamp, \
			level tinyint, function text, line int, msg text);", table);
	ret = sqlite3_exec(sqlite3_t, buffer, 0, 0, 0);
	if (ret != SQLITE_OK) goto err;
	sprintf(buffer, "insert into %s(level, function, line, msg) values(?, ?, ?, ?);", table);
	ret = sqlite3_prepare(sqlite3_t, buffer, -1, &sqlite3_stmt_t, 0);
	if (ret != SQLITE_OK) goto err;
noerr:
	++reference_count;
	return 0;
err:
	sqlite3_close(sqlite3_t);
	return ret;
}

void log_close()
{
	if (--reference_count) return;
	sqlite3_finalize(sqlite3_stmt_t);
	sqlite3_close(sqlite3_t);
}

static inline void lock()
{
#ifdef WIN32
	EnterCriticalSection(&cs);
#else
#endif
}

static inline void unlock()
{
#ifdef WIN32
	LeaveCriticalSection(&cs);
#else
#endif
}

static inline void log_impl_with_msg(int level, const char *function, int line, const char *fmt, va_list vl)
{
	char msg[BUFFER_SIZE];
	vsprintf(msg, fmt, vl);
	lock();
	sqlite3_bind_int(sqlite3_stmt_t, 1, level);
	sqlite3_bind_text(sqlite3_stmt_t, 2, function, -1, SQLITE_STATIC);
	sqlite3_bind_int(sqlite3_stmt_t, 3, line);
	sqlite3_bind_text(sqlite3_stmt_t, 4, msg, -1, SQLITE_STATIC);
	sqlite3_step(sqlite3_stmt_t);
	sqlite3_reset(sqlite3_stmt_t);
	unlock();
}

static inline void log_impl_without_msg(int level, const char *function, int line)
{
	lock();
	sqlite3_bind_int(sqlite3_stmt_t, 1, level);
	sqlite3_bind_text(sqlite3_stmt_t, 2, function, -1, SQLITE_STATIC);
	sqlite3_bind_int(sqlite3_stmt_t, 3, line);
	sqlite3_bind_null(sqlite3_stmt_t, 4);
	sqlite3_step(sqlite3_stmt_t);
	sqlite3_reset(sqlite3_stmt_t);
	unlock();
}

void log_debug(LOG_ARGS)
{
	const int level = 0;
	if (fmt) {
		va_list vl;
		va_start(vl, fmt);
		log_impl_with_msg(level, function, line, fmt, vl);
	} else log_impl_without_msg(level, function, line);
}

void log_info(LOG_ARGS)
{
	const int level = 1;
	if (fmt) {
		va_list vl;
		va_start(vl, fmt);
		log_impl_with_msg(level, function, line, fmt, vl);
	} else log_impl_without_msg(level, function, line);
}

void log_warning(LOG_ARGS)
{
	const int level = 2;
	if (fmt) {
		va_list vl;
		va_start(vl, fmt);
		log_impl_with_msg(level, function, line, fmt, vl);
	} else log_impl_without_msg(level, function, line);
}

void log_critical(LOG_ARGS)
{
	const int level = 3;
	if (fmt) {
		va_list vl;
		va_start(vl, fmt);
		log_impl_with_msg(level, function, line, fmt, vl);
	} else log_impl_without_msg(level, function, line);
}

void log_fatal(LOG_ARGS)
{
	const int level = 4;
	if (fmt) {
		va_list vl;
		va_start(vl, fmt);
		log_impl_with_msg(level, function, line, fmt, vl);
	} else log_impl_without_msg(level, function, line);
}
