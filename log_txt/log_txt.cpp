#include <log.h>
#include <fstream>
#include <time.h>
#include <stdio.h>
#include <string.h>
#include <mutex>
#include <thread>
#include <cstdarg>

#ifdef _WINDOWS
#include <Windows.h>
#else
#include <unistd.h>
extern "C" {
int log_initialize_impl(const char *uri, log_func *func);
int log_close_impl();
}
#endif

static constexpr size_t BUFFER_SIZE = 4096;

static std::mutex mutex;

static long reference_count = 0;
static std::ofstream f;

static inline void lock()
{ mutex.lock(); }

static inline void unlock()
{ mutex.unlock(); }

static inline void log_impl_header(const char *time, int level, void *object, const char *function, int line)
{ f << time << std::this_thread::get_id() << '\t' << level << '\t' << object << '\t' << function << '\t' << line; }

static inline void log_impl_step()
{ f << std::endl; }

static inline void log_impl_with_msg(int level, void *object, const char *function, int line,
									 const char *fmt, va_list vl) {
	timespec ts;
	timespec_get(&ts, TIME_UTC);
	tm *t = gmtime(&ts.tv_sec);
	char time[32];
	sprintf(time, "%d-%02d-%02d %02d:%02d:%02d.%09ld\t",
			t->tm_year + 1900, t->tm_mon + 1,
			t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, ts.tv_nsec);
	char msg[BUFFER_SIZE];
	vsprintf(msg, fmt, vl);
	lock();
	log_impl_header(time, level, object, function, line);
	f << '\t' << msg;
	log_impl_step();
	unlock();
}

static inline void log_impl_without_msg(int level, void *object, const char *function, int line) {
	timespec ts;
	timespec_get(&ts, TIME_UTC);
	tm *t = gmtime(&ts.tv_sec);
	char time[32];
	sprintf(time, "%d-%02d-%02d %02d:%02d:%02d.%09ld\t",
			t->tm_year + 1900, t->tm_mon + 1,
			t->tm_mday, t->tm_hour, t->tm_min, t->tm_sec, ts.tv_nsec);
	lock();
	log_impl_header(time, level, object, function, line);
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
	struct tm *ti;
	int ret = -1;
	lock();
	if (reference_count) goto noerr;
	char buffer[BUFFER_SIZE];
	size_t name_length;
	if (uri) {
		name_length = strlen(uri);
		memcpy(buffer, uri, name_length);
	} else {
#ifdef _WINDOWS
		DWORD len;
		len = GetModuleFileNameA(0, buffer, BUFFER_SIZE);
		if (!len) return -1;
#else
		ssize_t len = readlink("/proc/self/exe", buffer, BUFFER_SIZE);
		if (len <= 0) return -1;
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
		name_length = pos_point - pos_slash - 1;
		memmove(buffer, buffer + pos_slash + 1, name_length);
	}

	time_t t;
	time(&t);
	ti = gmtime(&t);
	sprintf(buffer + name_length, " [%d-%02d-%02d %02d-%02d-%02d].txt",
			ti->tm_year + 1900, ti->tm_mon + 1,
			ti->tm_mday, ti->tm_hour, ti->tm_min, ti->tm_sec);
	
	f.open(buffer);
	if (!f.is_open()) goto over;

	f << "time\tthread\tlevel\tobject\tfunction\tline\tmessage" << std::endl;

	*func = log_impl;
noerr:
	ret = ++reference_count;
over:
	unlock();
	return ret;
}

int log_close_impl() {
	lock();
	long rc = --reference_count;
	if (!rc) { f.close(); }
	unlock();
	return rc;
}
