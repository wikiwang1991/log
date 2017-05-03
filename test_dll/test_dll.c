#include "test_dll.h"
#include "../log/log.h"

void test_dll()
{
	log_initialize(0);

	log_debug("test_dll");

	log_close();
}
