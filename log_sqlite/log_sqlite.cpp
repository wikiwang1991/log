#include <log.h>
#include "sqlite3.h"
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <mutex>
#include <thread>
#include <sstream>

#ifdef WIN32
#include <Windows.h>
#endif

static constexpr size_t BUFFER_SIZE = 4096;

static std::mutex mutex;

static long reference_count = 0;
static sqlite3 *sqlite3_t;
static sqlite3_stmt *sqlite3_stmt_t;

static inline void lock()
{ mutex.lock(); }

static inline void unlock()
{ mutex.unlock(); }

static inline void log_impl_header(const char *time, int level, void *object, const char *function, int line) {
	sqlite3_bind_text(sqlite3_stmt_t, 1, time, -1, SQLITE_STATIC);
	std::stringstream s;
	s << std::this_thread::get_id();
	sqlite3_int64 tid;
	s >> tid;
	sqlite3_bind_int64(sqlite3_stmt_t, 2, tid);
	sqlite3_bind_int(sqlite3_stmt_t, 3, level);
	sqlite3_bind_int64(sqlite3_stmt_t, 4, (sqlite3_int64)object);
	sqlite3_bind_text(sqlite3_stmt_t, 5, function, -1, SQLITE_STATIC);
	sqlite3_bind_int(sqlite3_stmt_t, 6, line);
}

static inline void log_impl_step() {
	sqlite3_step(sqlite3_stmt_t);
	sqlite3_reset(sqlite3_stmt_t);
}

static inline void log_impl_with_msg(int level, void *object, const char *function, int line,
                                     const char *fmt, va_list vl) {
	timespec ts;
	timespec_get(&ts, TIME_UTC);
	tm *t = gmtime(&ts.tv_sec);
	char time[32];
	sprintf(time, "%d-%02d-%02d %02d:%02d:%02d.%09d\t",
			t->tm_year + 1900, t->tm_mon + 1,
			t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, ts.tv_nsec);
	char msg[BUFFER_SIZE];
	vsprintf(msg, fmt, vl);
	lock();
	log_impl_header(time, level, object, function, line);
	sqlite3_bind_text(sqlite3_stmt_t, 7, msg, -1, SQLITE_STATIC);
	log_impl_step();
	unlock();
}

static inline void log_impl_without_msg(int level, void *object, const char *function, int line) {
	timespec ts;
	timespec_get(&ts, TIME_UTC);
	tm *t = gmtime(&ts.tv_sec);
	char time[32];
	sprintf(time, "%d-%02d-%02d %02d:%02d:%02d.%09d\t",
			t->tm_year + 1900, t->tm_mon + 1,
			t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, ts.tv_nsec);
	lock();
	log_impl_header(time, level, object, function, line);
	sqlite3_bind_null(sqlite3_stmt_t, 7);
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

int log_initialize_impl(const char *uri, log_func *func) {
	lock();
	if (reference_count) goto noerr;
	char buffer[BUFFER_SIZE];
	if (uri) strcpy(buffer, uri);
	else {
#ifdef WIN32
		DWORD len;
		len = GetModuleFileName(0, buffer, BUFFER_SIZE);
		if (!len) return -1;
#else
		ssize_t len = readlink("/proc/self/exe", buffer, BUFFER_SIZE);
		if (len <= 0) return -1;
		pthread_mutex_init(&mutex, 0);
#endif
		int pos_slash, pos_point = len;
		for (int i = 0; i < (int)len; ++i)
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

	int ret = sqlite3_open(buffer, &sqlite3_t);
	if (ret != SQLITE_OK) goto over;
	sqlite3_exec(sqlite3_t, "pragma journal_mode = wal;", 0, 0, 0);
	sqlite3_exec(sqlite3_t, "pragma synchronous = normal;", 0, 0, 0);
	time_t t;
	time(&t);
	struct tm *ti = gmtime(&t);
	char table[32];
	sprintf(table, "[%d-%02d-%02d %02d:%02d:%02d]",
		ti->tm_year + 1900, ti->tm_mon + 1,
		ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);
	sprintf(buffer, "create table %s(time text,"
		"thread int, level tinyint, object int, function text, line int, message text);",
		table);
	ret = sqlite3_exec(sqlite3_t, buffer, 0, 0, 0);
	if (ret != SQLITE_OK) goto err;
	sprintf(buffer, "insert into %s(time, thread, level, object, function, line, message) "
		"values(?, ?, ?, ?, ?, ?, ?);", table);
	ret = sqlite3_prepare(sqlite3_t, buffer, -1, &sqlite3_stmt_t, 0);
	if (ret != SQLITE_OK) goto err;
	*func = log_impl;
noerr:
	ret = ++reference_count;
	goto over;
err:
	sqlite3_close(sqlite3_t);
over:
	unlock();
	return ret;
}

int log_close_impl() {
	lock();
	long rc = --reference_count;
	if (!rc) {
		sqlite3_finalize(sqlite3_stmt_t);
		sqlite3_close(sqlite3_t);
	}
	unlock();
	return rc;
}
