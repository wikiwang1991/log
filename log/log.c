#include "log.h"

static void log_null(LOG_ARGS) {}

log_func _log_debug = log_null;
log_func _log_info = log_null;
log_func _log_warning = log_null;
log_func _log_critical = log_null;
log_func _log_fatal = log_null;

#ifdef WIN32
#include <Windows.h>

static HMODULE hDll;

void log_initialize()
{
	hDll = LoadLibrary("log.dll");
	if (!hDll) return;
	void *p;
	p = GetProcAddress(hDll, "log_initialize");
	int (*i)() = p;
	if (!i || i()) {
		FreeLibrary(hDll);
		return;
	}
	p = GetProcAddress(hDll, "log_debug");
	if (p) _log_debug = p;
	p = GetProcAddress(hDll, "log_info");
	if (p) _log_info = p;
	p = GetProcAddress(hDll, "log_warning");
	if (p) _log_warning = p;
	p = GetProcAddress(hDll, "log_critical");
	if (p) _log_critical = p;
	p = GetProcAddress(hDll, "log_fatal");
	if (p) _log_fatal = p;
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

void log_initialize()
{
	h = dlopen("liblog.so", RTLD_LAZY);
	if (!h) return;
	void *p;
	p = dlsym(h, "log_initialize");
	int (*i)() = p;
	if (!i || i()) {
		dlclose(h);
		return;
	}
	p = dlsym(h, "log_debug");
	if (p) _log_debug = p;
	p = dlsym(h, "log_info");
	if (p) _log_info = p;
	p = dlsym(h, "log_warning");
	if (p) _log_warning = p;
	p = dlsym(h, "log_critical");
	if (p) _log_critical = p;
	p = dlsym(h, "log_fatal");
	if (p) _log_fatal = p;
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
