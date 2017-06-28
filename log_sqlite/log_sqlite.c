#include <log.h>
#include "sqlite3.h"
#include <time.h>
#include <stdio.h>
#include <string.h>

#define URI_BUFFER_SIZE 1024

#ifdef WIN32

#include <windows.h>

CRITICAL_SECTION cs;

#else

#include <pthread.h>
#include <unistd.h>

pthread_mutex_t mutex;

#endif

static long reference_count = 0;
static sqlite3 *sqlite3_t;
static sqlite3_stmt *sqlite3_stmt_t;

static inline void lock() {
#ifdef WIN32
	EnterCriticalSection(&cs);
#else
	pthread_mutex_lock(&mutex);
#endif
}

static inline void unlock() {
#ifdef WIN32
	LeaveCriticalSection(&cs);
#else
	pthread_mutex_unlock(&mutex);
#endif
}

static inline void log_impl_header(int level, void *object, const char *function, int line) {
	sqlite3_bind_int(sqlite3_stmt_t, 1, GetCurrentThreadId());
	sqlite3_bind_int(sqlite3_stmt_t, 2, level);
	sqlite3_bind_int64(sqlite3_stmt_t, 3, object);
	sqlite3_bind_text(sqlite3_stmt_t, 4, function, -1, SQLITE_STATIC);
	sqlite3_bind_int(sqlite3_stmt_t, 5, line);
}

static inline void log_impl_step() {
	sqlite3_step(sqlite3_stmt_t);
	sqlite3_reset(sqlite3_stmt_t);
}

static inline void log_impl_with_msg(int level, void *object, const char *function, int line,
                                     const char *fmt, va_list vl) {
	char msg[URI_BUFFER_SIZE];
	vsprintf(msg, fmt, vl);
	lock();
	log_impl_header(level, object, function, line);
	sqlite3_bind_text(sqlite3_stmt_t, 6, msg, -1, SQLITE_STATIC);
	log_impl_step();
	unlock();
}

static inline void log_impl_without_msg(int level, void *object, const char *function, int line) {
	lock();
	log_impl_header(level, object, function, line);
	sqlite3_bind_null(sqlite3_stmt_t, 6);
	log_impl_step();
	unlock();
}

static void log_impl(LOG_ARGS) {
	if (fmt) {
		va_list vl;
		va_start(vl, fmt);
		log_impl_with_msg(level, object, function, line, fmt, vl);
	} else log_impl_without_msg(level, object, function, line);
}

int log_initialize(const char *uri, log_func *func) {
	
	if (reference_count) goto noerr;
	char buffer[URI_BUFFER_SIZE];
	if (uri) strcpy(buffer, uri);
	else {
#ifdef WIN32
		DWORD len;
		len = GetModuleFileName(0, buffer, URI_BUFFER_SIZE);
		if (!len) return -1;
#else
		ssize_t len = readlink("/proc/self/exe", buffer, URI_BUFFER_SIZE);
		if (len <= 0) return -1;
		pthread_mutex_init(&mutex, 0);
#endif
		int pos_slash, pos_point = len;
		for (int i = 0; i < len; ++i)
			switch (buffer[i]) {
			case '\\':
			case '/':
				pos_slash = i;
				break;
			case '.':
				pos_point = i;
				break;
			}
		int name_length = pos_point - pos_slash - 1;
		memmove(buffer, buffer + pos_slash + 1, name_length);
		strcpy(buffer + name_length, ".log.sqlite3");
	}
#ifdef WIN32
	InitializeCriticalSection(&cs);
#else
	pthread_mutex_init(&mutex, 0);
#endif
	int ret = sqlite3_open(buffer, &sqlite3_t);
	if (ret != SQLITE_OK) goto err;
	sqlite3_exec(sqlite3_t, "pragma journal_mode = wal;", 0, 0, 0);
	sqlite3_exec(sqlite3_t, "pragma synchronous = normal;", 0, 0, 0);
	time_t t;
	time(&t);
	struct tm *ti = localtime(&t);
	char table[32];
	sprintf(table, "[%d-%02d-%02d %02d:%02d:%02d]",
		ti->tm_year + 1900, ti->tm_mon + 1,
		ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);
	sprintf(buffer, "create table %s(time datetime default"
		"(strftime('%%Y-%%m-%%d %%H:%%M:%%f', 'now')), "
		"thread int, level tinyint, object int, function text, line int, message text);",
		table);
	ret = sqlite3_exec(sqlite3_t, buffer, 0, 0, 0);
	if (ret != SQLITE_OK) goto err;
	sprintf(buffer, "insert into %s(thread, level, object, function, line, message) "
		"values(?, ?, ?, ?, ?, ?);", table);
	ret = sqlite3_prepare(sqlite3_t, buffer, -1, &sqlite3_stmt_t, 0);
	if (ret != SQLITE_OK) goto err;
	*func = log_impl;
noerr:

	return InterlockedIncrement(&reference_count);

err:
	sqlite3_close(sqlite3_t);
	return ret;
}

int log_close() {
	long rc;

	rc = InterlockedDecrement(&reference_count);

	if (!rc) {
		lock();
		sqlite3_finalize(sqlite3_stmt_t);
		sqlite3_close(sqlite3_t);
		unlock();
#ifdef WIN32
		DeleteCriticalSection(&cs);
#else
		pthread_mutex_destroy(&mutex);
#endif
	}
	return rc;
}
