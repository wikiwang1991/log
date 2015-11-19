#include "../log/log.h"
//#include "../test_dll/test_dll.h"

int main()
{
	int i = 0;
	log_initialize();
//	test_dll();

	log_debug("Hello debug %d\n", i++);
	log_info("Hello info %d\n", i++);
	log_warning("Hello warning %d\n", i++);
	log_critical("Hello critical %d\n", i++);
	log_fatal("Hello fatal %d\n", i);

//	test_dll();

	log_close();

	return 0;
}

