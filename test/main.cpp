#include "../log/log.h"
//#include "../test_dll/test_dll.h"

#include <thread>

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
	int i = 0;
	log_initialize(0);
//	test_dll();

	constexpr size_t threads = 8;
	std::thread t[threads];

	for (size_t i = 0; i < threads; ++i)
	{
		t[i] = std::thread(f);
	}

	for (size_t i = 0; i < threads; ++i)
	{
		t[i].join();
	}

//	test_dll();

	log_close();

	return 0;
}
