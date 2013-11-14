// From JamesM's tutorial

// Initializes the PIT and handles clock updates

#include "timer.h"
#include "isr.h"
#include "monitor.h"

#define PIT_FREQ 1193180

//each channel is one of the frequency dividers
#define PIT_CHANNEL_0 0x40 //connected to PIC as IRQ0
#define PIT_CHANNEL_1 0x41 //barely ever used anymore may not even exist
#define PIT_CHANNEL_2 0x42 //used for speakers and other stuff

#define PIT_CMD 0x43

/*
 * Command Register- I/O Address 0x43
 * Bits 6-7: Select channel 00, 01, 10, 11 used in 8254 for read-back cmd only
 * Bits 4-5: Access mode
 *     00 = latch count value
 *     01 = lowbyte access only
 *     10 = highbyte access only
 *     11 = both bytes
 * Bits 1-3: Operating mode
 *     000 = interrupt on terminal count
 *     001 = hardware re-triggerable one-shot
 *     010 = rate generator
 *     011 = square wave generator
 *     100 = software triggered strobe
 *     101 = hardware triggered strobe
 *     110 = same as 010b
 *     111 = same as 011b
 * Bit 0: BCD/Binary mode (0 = 16 bit binary 1 = four digit BCD)
 */
#define PIT_OP_MODE 0x36

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
    outb(PIT_CMD, PIT_OP_MODE);

    // Divisor has to be sent byte-wise, so split here into upper/lower bytes.
    u8int l = (u8int)(divisor & 0xFF);
    u8int h = (u8int)( (divisor>>8) & 0xFF );

    // Send the frequency divisor.
    outb(PIT_CHANNEL_0, l);
    outb(PIT_CHANNEL_0, h);
}
