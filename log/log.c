#include "log.h"

static void log_null(LOG_ARGS) {}

log_func _log_debug_0_0 = log_null;
log_func _log_info_0_0 = log_null;
log_func _log_warning_0_0 = log_null;
log_func _log_critical_0_0 = log_null;
log_func _log_fatal_0_0 = log_null;

#ifdef WIN32
#include <Windows.h>

static HMODULE hDll;

int log_initialize(const char *file)
{
	hDll = LoadLibrary("log.dll");
	if (!hDll) return -1;
	void *p;
	p = GetProcAddress(hDll, "log_initialize");
	int (*i)(const char *) = p;
	int ret;
	if (!i || (ret = i(file))) {
		FreeLibrary(hDll);
		return ret;
	}
	p = GetProcAddress(hDll, "log_fatal");
	if (p) _log_fatal_0_0 = p;
	p = GetProcAddress(hDll, "log_critical");
	if (p) _log_critical_0_0 = p;
	p = GetProcAddress(hDll, "log_warning");
	if (p) _log_warning_0_0 = p;
	p = GetProcAddress(hDll, "log_info");
	if (p) _log_info_0_0 = p;
	p = GetProcAddress(hDll, "log_debug");
	if (p) _log_debug_0_0 = p;
	return 0;
}

void log_close()
{
	if (!hDll) return;
	void *p;
	p = GetProcAddress(hDll, "log_close");
	if (!p) return;
	void (*i)() = p;
	i();
	FreeLibrary(hDll);
}
#else
#include <dlfcn.h>

static void *h;

int log_initialize(const char *file)
{
	h = dlopen("liblog.so", RTLD_LAZY);
	if (!h) return -1;
	void *p;
	p = dlsym(h, "log_initialize");
	int (*i)(const char *) = p;
	int ret;
	if (!i || (ret = i(file))) {
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
