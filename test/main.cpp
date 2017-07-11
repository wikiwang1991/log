#include <log.h>

void f()
{
	for (int i = 0; i < 100; ++i) {
		logg_debug("Hello debug %d", i);
		logg_info("Hello info %d", i);
		logg_warning("Hello warning %d", i);
		logg_critical("Hello critical %d", i);
		logg_fatal("Hello fatal %d", i);
	}
}

int main()
{
	log_initialize(0);

	f();

	log_close();

	return 0;
}
