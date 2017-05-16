#include "log.h"

static void log_null(LOG_ARGS) {}

log_func _log_func[LOG_MAX_LEVEL] =
{ log_null, log_null, log_null, log_null, log_null };

#ifdef WIN32

#include <Windows.h>

static HMODULE hDll = 0;

int log_initialize(const char *uri) {
	if (!hDll) hDll = LoadLibrary("log.dll");
	if (!hDll) return -1;
	void *p;
	p = GetProcAddress(hDll, "log_initialize");
	int (*i)(const char *, log_func *func_table) = p;
	int ret;
	if (!i || (ret = i(uri, _log_func)) < 0) {
		FreeLibrary(hDll);
		hDll = 0;
	}
	return ret;
}

int log_close() {
	if (!hDll) return 1;
	void *p;
	p = GetProcAddress(hDll, "log_close");
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

static void *h;

int log_initialize(const char *uri)
{
	h = dlopen("liblog.so", RTLD_LAZY);
	if (!h) return -1;
	void *p;
	p = dlsym(h, "log_initialize");
	int (*i)(const char *) = p;
	int ret;
	if (!i || (ret = i(uri))) {
		dlclose(h);
		return ret;
	}
	p = dlsym(h, "log_fatal");
	if (p) _log_fatal_0_0 = p;
	p = dlsym(h, "log_critical");
	if (p) _log_critical_0_0 = p;
	p = dlsym(h, "log_warning");
	if (p) _log_warning_0_0 = p;
	p = dlsym(h, "log_info");
	if (p) _log_info_0_0 = p;
	p = dlsym(h, "log_debug");
	if (p) _log_debug_0_0 = p;
	return 0;
}

void log_close()
{
	void *p;
	p = dlsym(h, "log_close");
	if (!p) return;
	void (*i)() = p;
	i();
	dlclose(h);
}

#endif
