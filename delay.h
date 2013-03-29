

#ifndef __DELAY_H
#define __DELAY_H

#include "clk_freq.h"
/*****************************************************
 *               Defination for FOSC = 20MHz         *
 *****************************************************/
#define dly200n asm("nop")
#define dly400n dly200n;dly200n
#define dly2u dly400n;dly400n;dly400n;dly400n;dly400n
#define DelayDivisor 1
#define WaitFor1Us asm("nop")
#define Jumpback asm("goto $ - 3")
/*****************************************************
 *               End of defination                   *
 *****************************************************/
extern unsigned char delayus_variable;

#define DelayUs(x) { \
			delayus_variable=(unsigned char)(x/DelayDivisor); \
			WaitFor1Us;  \
			asm("decfsz _delayus_variable,f"); \
			Jumpback;  \
                        asm ("nop"); \
                   }


#define LOOP_CYCLES_CHAR	9							//how many cycles per loop, optimizations on
#define timeout_char_us(x)	(long)(((x)/LOOP_CYCLES_CHAR)*(PIC_CLK/1000000/4))

#define LOOP_CYCLES_INT		2						//how many cycles per loop, optimizations on
#define timeout_int_us(x)	(long)(((x)/LOOP_CYCLES_INT)*(PIC_CLK/1000000/4))

//if lo byte is zero, faster initialization by 1 instrucion
#define timeout_int_lobyte_zero_us(x)	(long)(((x)/LOOP_CYCLES_INT)*(PIC_CLK/4.0)&0xFF00)

//function prototypes

void DelayBigUs(unsigned int cnt);
void DelayMs(unsigned char cnt);
void DelayMs_interrupt(unsigned char cnt);
void DelayBigMs(unsigned int cnt);
void DelayS(unsigned char cnt);

#endif


