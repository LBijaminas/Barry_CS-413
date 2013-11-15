// From JamesM's tutorial

#include "kernel_heap.h"

extern u32int end; // this is defined in linker script

u32int p_address = (u32int) &end; // placement address

u32int kmalloc_internal(u32int size, int align, u32int *address){
    /*
        Size is the amount of space to be allocated on the heap
        Align is a flag whether the page should be aligned or not
        Address acts as a flag, as well as provides a variable, if provided,
        to return the new page address
    */
    if (align && (p_address & 0xFFF)){
        // align is needed

        // ensures that it is on the page bound
        p_address &= 0xFFFFF000;

        // skips over a page, in case the previous page has been used
        p_address += 0x1000;

    }
    if (address){
        // address needs to be set
        *address = p_address;
    }

    u32int temp = p_address;
    p_address += size;
    return temp; // returns the address, but i'm not sure why it doesn't return a pointer
}

u32int kmalloc_aligned(u32int size){
    return kmalloc_internal(size, 1, 0);
}

u32int kmalloc_returned(u32int size, u32int *address){
    return kmalloc_internal(size, 0, address);
}

u32int kmalloc_aligned_returned(u32int size, u32int *address){
    return kmalloc_internal(size, 1, address);
}

u32int kmalloc(u32int size){
    // the most simple allocation, just to add the value on the heap
    return kmalloc_internal(size, 0, 0);
}
