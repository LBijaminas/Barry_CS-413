#include "monitor.h"

int kernel_main()
{
	monitor_clear();
	monitor_write("Hello, World!");
	return 0xDEADBABA;	
}
