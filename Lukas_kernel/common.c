// From JamesM's tutorials

#include "common.h"

void outb(u16int port, u8int value) {
    // Write a byte out to the specified port
    asm volatile ("outb %1, %0" : : "dN" (port), "a" (value));
}

u8int inb(u16int port) {
    // Read a byte from the specified port
    u8int ret;
    asm volatile("inb %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

u16int inw(u16int port) {
    // Read a word from the specified port
    u16int ret;
    asm volatile ("inw %1, %0" : "=a" (ret) : "dN" (port));
    return ret;
}

/*
// Standard C library functions
void memcpy(u8int *dest, const u8int *src, u32int len) {
    // Copy bytes
    const u8int *sp = (const u8int *)src; // source
    u8int *dp = (u8int *)dest; //destination
    for(; len != 0; len--) *dp++ = *sp++;
}

void memset(u8int *dest, u8int val, u32int len) {
    // Write value of length len to the destination
    u8int *temp = (u8int *)dest;
    for ( ; len != 0; len--) *temp++ = val;
}

int strcmp(char *str1, char *str2){
    // Compare two strings. Should return -1 if
    // str1 < str2, 0 if they are equal or 1 otherwise.
    int i;
    int failed = 0;
    for(i = 0; str1[i] != '\0' && str2[i] != '\0'; i++) {
        if(str1[i] != str2[i]){
            failed = 1;
            break;
        }
     }
    // the loop could've existed if one of the strings ended,
    // so this checks if that didn't happen
    if((str1[i] == '\0' && str2[i] != '\0') || (str1[i] != '\0' && str2[i] == '\0'))
        failed = 1;
    return failed;
}

char *strcpy(char *dest, const char *src) {
    // Copy the NULL-terminated string src into dest, and
    // return dest.
    do {
      *dest++ = *src++;
    } while (*src != 0);
}

char *strcat(char *dest, const char *src){
    // Concatenate the NULL-terminated string src onto
    // the end of dest, and return dest.
    while (*dest != 0){
        *dest = *dest++;
    }

    do{
        *dest++ = *src++;
    } while (*src != 0);
    return dest;
}
*/
/*
    The two functions below are supposed to handle
    stack error messages.
*//*
void *__stack_chk_guard = NULL;

void __stack_chk_guard_setup() {
    unsigned char *p;
    p = (unsigned char *) &__stack_chk_guard;

    /* If you have the ability to generate random numbers in your kernel then use them,
       otherwise for 32-bit code:
    *p =  0x00000aff;
}

// Needs to be done
void __attribute__((noreturn)) __stack_chk_fail(){
    /* put your panic function or similar in here
    unsigned char * vid = (unsigned char *)0xB8000;
    vid[1] = 7;
    for(;;)
    vid[0]++;
}
*/
