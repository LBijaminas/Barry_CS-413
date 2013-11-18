// From JamesM's tutorials

#ifndef COMMON_H
#define COMMON_H
// let's define what NULL is
#define NULL 0

/*
 * PANIC macro returns error on __LINE__ of __FILE__
 */
#define PANIC(a) panic(a, __LINE__, __FILE__) 

// Type defs so I wouldn't need to rewrite multi-word
// variable types
typedef unsigned int   u32int;
typedef          int   s32int;
typedef unsigned short u16int;
typedef          short s16int;
typedef unsigned char  u8int;
typedef          char  s8int;

// Functions mapping directly to assembly
// to read or write bytes and words
void outb(u16int, u8int);
u8int inb(u16int);
u16int inw(u16int);
void memcpy(u8int *, const u8int *, u32int);
void memset(u8int *, u8int, u32int);
int strcmp(char *, char *);
char *strcpy(char *, const char *);
char *strcat(char *, const char *);
#endif
