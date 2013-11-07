// From JamesM's tutorial

#ifndef MONITOR_H
#define MONITOR_H

#include "common.h"

// Write a single character out to the screen
void monitor_put(char c);

// Clear the screen
void monitor_clear();

// Output a null-terminated ASCII string to the monitor
void monitor_write(char *c);

// write hex function
void monitor_write_hex(u32int);
#endif
