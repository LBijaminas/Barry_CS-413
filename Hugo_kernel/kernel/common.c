#include "common.h"

void byte_out(u16int port, u8int val)
{
	asm volatile("outb %1, %0" : : "dN" (port), "a" (val));
}

u8int byte_in(u16int port)
{
	u8int ret;
	asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}

u16int word_in(u16int port)
{
	u16int ret;
	asm volatile("inw %1, %0" : "=a" (ret) : "dN" (port));
	return ret;
}
