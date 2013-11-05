/*
 * monitor.h -- defines routines for interacting with the monitor
 */

#ifndef _MONITOR_H_
#define _MONITOR_H_

#include "common.h"

void monitor_putc(char c);
void monitor_clear();
void monitor_puts(char *c);
#endif /*_MONITOR_H_*/
