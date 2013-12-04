#include "paging.h"

/*
* A bitset of frames - used or free
*/
u32int* frames;
u32int frame_count;

extern u32int p_address;

page_directory_t* kernel_directory;
page_directory_t* current_directory;

/*
* Macros are considered outdated so I've implemented them here as functions.
* Instead of using 8 * 4 I just use the result of that to save an operation.
*/
u32int index_from_bit(u32int a)
{
        return a / 32;
}

u32int offset_from_bit(u32int a)
{
        return a % 32;
}

/*
* Set a bit in the frames bitset
*/
static void set_frame(u32int frame_addr)
{
        u32int frame = frame_addr/0x1000;
        u32int idx = index_from_bit(frame);
        u32int off = offset_from_bit(frame);
        frames[idx] |= (0x1 << off);
}
/*
* Clear a bit in the frames bitset
*/
static void clear_frame(u32int frame_addr)
{
        u32int frame = frame_addr/0x1000;
        u32int idx = index_from_bit(frame);
        u32int off = offset_from_bit(frame);
        frames[idx] &= ~(0x1 << off);
}

/*
* Test if a bit is set
*/
static u32int test_frame(u32int frame_addr)
{
        u32int frame = frame_addr/0x1000;
        u32int idx = index_from_bit(frame);
        u32int off = offset_from_bit(frame);
        frames[idx] & (0x1 << off);
}

/*
* Find first free frame
*/
static u32int first_frame()
{
        u32int i, j;
        for(i = 0; i < index_from_bit(frame_count); i++){
                if(frames[i] != 0xFFFFFFFF){
                        for(j = 0; j < 32; j++){
                                u32int test = 0x1 << j;
                                if(!(frames[i] & test)){
                                        return i * 32 + j;
                                }
                        }
                }
        }
}

/*
* Allocate a frame
*/
void alloc_frame(page_t* page, int is_kernel, int is_writeable)
{
        if(page->frame != 0)
                return; //This frame has already been allocated
        
        u32int idx = first_frame();
        if(idx == (u32int) -1)
                PANIC("No free frames!");
        
        set_frame(idx*0x1000);
        page->present = 1;
        if(is_writeable)
                page->rw = 1;
        else
                page->rw = 0;
        if(is_kernel)
                page->user = 0;
        else
                page->user = 1;
        page->frame = idx;
}

/*
* Deallocate a frame
*/
void free_frame(page_t* page)
{
        u32int frame = page->frame;
        if(!frame)
                return; //We never had a frame in the first place
        clear_frame(frame);
        page->frame = 0x0;
}

void initialize_paging()
{
	//The size of physical memory. For now let's just assume it's 16MB
	u32int mem_end_page = 0x1000000;

	frame_count = mem_end_page / 0x1000;
	frames = (u32int*) kmalloc(index_from_bit(frame_count));
	memset(frames, 0, index_from_bit(frame_count));

	//Create a page directory
	kernel_directory = (page_directory_t*) kmalloc_aligned(sizeof(page_directory_t));
	memset(kernel_directory, 0, sizeof(page_directory_t));
	current_directory = kernel_directory;
	
	/*
	 * We need to identity map (phys addr = virt addr) from 0x0 to the end of
	 * used memory, so we can access this transparently as if paging wasn't
	 * enabled. NOTE that we use a while loop here deliberately. Inside the
	 * loop body we actually change p_address by calling kmalloc(). A
	 * while loop causes this to be computed on-the-fly rather than once at
	 * the start.
	 */ 
	int i = 0;
	while(i < p_address){
		//Kernel code is readable but not writeable from userspace.
		alloc_frame(get_page(i, 1, kernel_directory), 0, 0);
		i += 0x1000;
	}
	register_interrupt_handler(14, page_fault);

	switch_page_directory(kernel_directory);
}

void switch_page_directory(page_directory_t* dir)
{
	current_directory = dir;
	asm volatile("mov %0, %%cr3":: "r"(&dir->tablesPhysical));
	u32int cr0;
	asm volatile("mov %%cr0, %0": "=r"(cr0));
	cr0 |= 80000000;
	asm volatile("mov %0, %%cr0":: "r"(cr0));
}

page_t* get_page(u32int address, int make, page_directory_t* dir)
{
	address /= 0x1000;
	u32int table_idx = address / 1024;
	if(dir->tables[table_idx]){
		return &dir->tables[table_idx]->pages[address%1024];
	}else if(make){
		u32int tmp;
		dir->tables[table_idx] = (page_table_t*)kmalloc_aligned_returned(sizeof(page_table_t), &tmp);
		memset(dir->tables[table_idx], 0, 0x1000);
		dir->tablesPhysical[table_idx] = tmp | 0x7; //PRESENT, RW, US.
		return &dir->tables[table_idx]->pages[address%1024];
	}else{
		return 0;
	}
}

void page_fault(registers_t regs)
{
	u32int faulting_address;
	asm volatile("mov %%cr2, %0" : "=r" (faulting_address));

	int present = !(regs.err_code & 0x1); //page not present
	int rw = regs.err_code & 0x2; //Write operation?
	int us = regs.err_code & 0x4; //Processor in user mode?
	int reserved = regs.err_code & 0x8; //overwritten cpu reserved bits (entry)
	int id = regs.err_code & 0x10; //caused by instruction fetch?

	//Output error message
	monitor_write("Page fault! ( ");
	if(present)
		monitor_write("missing ");
	if(rw)
		monitor_write("read-only ");
	if(us)
		monitor_write("user-mode ");
	if(reserved)
		monitor_write("reserved ");
	monitor_write(") at 0x");
	monitor_write_hex(faulting_address);
	monitor_write("\n");
	PANIC("Page fault");
}
