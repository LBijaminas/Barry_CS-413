/*
 * common.h -- some global typedefs and functions
 */

#ifndef _COMMON_H_
#define _COMMON_H_

typedef unsigned int u32int;
typedef int s32int;
typedef unsigned short u16int;
typedef short s16int;
typedef unsigned char u8int;
typedef char s8int;

void byte_out(u16int port, u8int val);
u8int byte_in(u16int port);
u16int word_in(u16int port);

#endif /*_COMMON_H_*/
