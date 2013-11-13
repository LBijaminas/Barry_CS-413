#ifndef PAGING_H
#define PAGING_H

#include "common.h"
#include "isr.h"

typedef struct page{
    u32int present: 1; // whether page is present in memory
    u32int rw: 1;      // set it read-write
    u32int user: 1;    // 0 - kernel mode, 1 - user mode
    u32int accessed: 1;// whether it has been accesssed since last refresh
    u32int written: 1; // whether the page has been written to since last refresh
    u32int reserved: 7;// unused and reserved bits
    u32int frame: 20;  // frame address
} page_t;

// data structure for page tab;e
typedef struct page_table{
    page_t pages[1024];
} page_table_t;

// data structure for page directory
typedef struct page_directory{
    page_table_t *tables[1024]; // array of pointers to page tables
    u32int tablesPhysical[1024]; // physical locations of tables array values to load in CR3 register
    u32int physicalAddress; // the physical address of tablesPhysical; matters when kheap allocated and the directory
                            // is in different location in memory
} page_directory_t;

// initializes the environment
void init_paging();

//loads specified page dir to CR3
void switch_page_dir(page_directory_t *new);

// gets pointer to the page required
page_t *get_page(u32int addr, int make, page_directory_t *dir);

// page fault handler
void page_fault(registers_t regs);
#endif // PAGING_H
