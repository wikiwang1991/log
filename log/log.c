#include "log.h"

static void log_null(LOG_ARGS) {}

log_func _log_func = log_null;

#ifdef WIN32

#include <Windows.h>

static HMODULE hDll = 0;

int log_initialize(const char *uri) {
	if (!hDll) hDll = LoadLibrary("log.dll");
	if (!hDll) return -1;
	void *p;
	p = GetProcAddress(hDll, "log_initialize_impl");
	int (*i)(const char *, log_func *func) = p;
	int ret;
	if (!i || (ret = i(uri, &_log_func)) < 0) {
		FreeLibrary(hDll);
		hDll = 0;
	}
	return ret;
}

int log_close() {
	if (!hDll) return 1;
	void *p;
	p = GetProcAddress(hDll, "log_close_impl");
	if (!p) return -1;
	int (*i)() = p;
	int ret = i();
	if (!ret) {
		FreeLibrary(hDll);
		hDll = 0;
	}
	return ret;
}

#else

#include <dlfcn.h>

static void *h = 0;

int log_initialize(const char *uri)
{
	if (!h) h = dlopen("./liblog.so", RTLD_LAZY);
	if (!h) return -1;
	void *p;
    p = dlsym(h, "log_initialize_impl");
	int (*i)(const char *, log_func *func) = p;
	int ret;
    if (!i || (ret = i(uri, &_log_func)) < 0) {
		dlclose(h);
		h = 0;
	}
	return ret;
}

int log_close()
{
	if (!h) return 1;
	void *p;
	p = dlsym(h, "log_close_impl");
	if (!p) return -1;
	int (*i)() = p;
	int ret = i();
	if (!ret) {
		dlclose(h);
		h = 0;
	}
	return ret;
}

#endif
