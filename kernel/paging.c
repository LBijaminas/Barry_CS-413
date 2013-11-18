/*
* A bitset of frames - used or free
*/
u32int* frames;
u32int frame_count;

extern u32int placement_address;

#define INDEX_FROM_BIT(a) (a/(8*4))
#define OFFSET_FROM_BIT(a) (a%(8*4))

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
