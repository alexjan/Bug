/*

Designed by AlexJan 2013 to Marth 27 version 1.00

Microprocessor: 	Microchip PIC16F876A

Compiled with:		Hi Tech PICC v9.83 

Overall goal: 		serial port tester @ 9600 baud

Notes: to save memory, you have to comment out a function or else it will be included
even if its never called

*/

#include	<pic.h>
#include	<conio.h>
#include	<stdlib.h>
#include 	<stdio.h>
#include	<ctype.h>

#include	"always.h"
#include 	"clk_freq.h"      //defines clock speed for micro in MHz, ie: 3.6864Mhz
#include	"delay.h"
#include	"serial.h"

#define 	Yellow RC0
#define 	Red RC1
#define 	Green RC2
#define		DEnable RC5
#define 	Transmite 1
#define 	Reader 0


    __CONFIG (WDTE_ON & FOSC_HS & PWRTE_ON & BOREN_OFF & LVP_OFF & CPD_OFF);

//unsigned char Reciever = 0; 
/*******************************************************************************
 *                                                                             *
 *                        B E G I N  M A I N                                   *
 *                             PROGRAM                                         *
 *                                                                             *
 ******************************************************************************/
void main(void) {


    unsigned char position,column,number,c,out;
    unsigned short long buffer,temp;
    OPTION_REG = 0b00001111;
    serial_setup();
    PORTC = 0;
    DEnable = 1;
    buffer = 0;
	putlf;    
	putst("Serial tester program for PIC16F876A by Girko Alexandr\n");
   	putst("Starting up serial @ 9600 baud, N,8,1, no flow control ...\n");
	position = 0;
    buffer = 0;
    putst("0000.00\n^\n");
	while(!TRMT)CLRWDT();
    while(1) {
	    DEnable = 0;
	    c = getch();
	    DEnable = 1;
	    number = c - '0';
	    if(number < 10){
    	    if(!position--)position = 5;
    	    temp = (unsigned short long)0x0F << (4 * position);
    	    temp = ~temp;  
    	    buffer &= temp;
    	    buffer |= (unsigned short long)number << (4 * position); 
    	    out = buffer >> 16;
    	    putchhex(out);
    	    out = buffer >> 8;
    	    putchhex(out);
    	    putch('.');
    	    out = buffer & 0xFF;
    	    putchhex(out);
    	    putlf;
    	    switch(position){
        	    case 1:
        	        putst("      ^");
        	        break;
        	    case 2:
        	        putst("     ^");
        	        break;
        	    case 3:
        	        putst("   ^");
        	        break;
        	    case 4:
        	        putst("  ^");
        	        break;
        	    case 5:
        	        putst(" ^");
        	        break;
        	    case 0:
        	        putst("^");
        	        break;
        	}
        	putlf;
    		while(!TRMT)CLRWDT(); 
    	} else if (c == 'r' || c == 'R' ) {
    	    position = 0;
    	    buffer = 0;
            putst("0000.00\n^\n");
    	    while(!TRMT)CLRWDT(); 
    	}    	
    }
}

