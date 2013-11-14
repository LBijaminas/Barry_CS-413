// From JamesM's tutorial

// Initializes the PIT and handles clock updates

#include "timer.h"
#include "isr.h"
#include "monitor.h"

#define PIT_FREQ 1193180

#define PIT_CHANNEL_0 0x40
#define PIT_CHANNEL_1 0x41
#define PIT_CHANNEL_2 0x42
#define PIT_CMD 0x43

u32int tick = 0;

static void timer_callback(registers_t regs){
    tick++;
    monitor_write("Tick: ");
    monitor_write_dec(tick);
    monitor_put('\n');
}

void init_timer(u32int frequency) {
    // register our timer callback.
    register_interrupt_handler(IRQ0, &timer_callback);

    // The value we send to the PIT is the value to divide it's input clock
    // (1193180 Hz) by, to get our required frequency. Important to note is
    // that the divisor must be small enough to fit into 16-bits.
    u32int divisor = PIT_FREQ / frequency;

    // Send the command byte.
    outb(PIT_CMD, 0x36);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    u8int l = (u8int)(divisor & 0xFF);
    u8int h = (u8int)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    outb(PIT_CHANNEL_0, l);
    outb(PIT_CHANNEL_0, h);
}
