// From JamesM's tutorial
#ifndef KERNEL_HEAP_H
#define KERNEL_HEAP_H

#include "common.h"

// Functions for the kernel heap

// most elaborate of the functions
u32int kmalloc_internal(u32int, int, u32int *);

// page aligned kernel allocation
u32int kmalloc_aligned(u32int);

// not aligned but address is returned in second parameter
u32int kmalloc_returned(u32int, u32int *);

// page aligned and address is returned in the second parameter
u32int kmalloc_aligned_returned(u32int, u32int *);

// very generic allocation
u32int kmalloc(u32int);

#endif
