// From JamesM's tutorial

#include "monitor.h"
#include "descriptor_tables.h"
#include "timer.h"

int main(){
    // setup stack protector
     __stack_chk_guard_setup();

    // Initialise all the ISRs and segmentation
    init_descriptor_tables();

    monitor_clear();


    monitor_write("Working!\n");
    monitor_write_hex(0x123456);

    // Throw some interrupts to ensure interrupt handler is working
   /* asm volatile("int $0x3");
    asm volatile("int $0x4");

    asm volatile("sti");
    init_timer(50);*/

    return 0;
}
