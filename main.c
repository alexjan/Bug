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


unsigned char column,c;
  
/*******************************************************************************
 //flash lights on port C on dev board               for test applications 
    while(1) {
    PORTC=0x55;
    DelayMs(500);
    PORTC=0xAA;
    DelayMs(500);
    }
*******************************************************************************/ 
    OPTION_REG = 0b00001111;
    serial_setup();
    
    PORTC = 0;
    TRISC = 0b11011000;
    
//    DEnable = 1;
//    while(1) {
//		putlf;    
//		putst("Serial tester program for PIC16F876A by Girko Alexandr\n");
//		putst("Starting up serial @ 9600 baud, N,8,1, no flow control ...\n ");
//		DelayMs(1);
//    }
    
   

    while(1) {
	    DEnable = 0;
	    c = getch();
		switch (c){
			default:
				Red = false;
				Green = false;
				Yellow = false;
				break;
			case 'R':
				Red = true;
				Green = false;
				Yellow = false;
				break;
			case 'G':
				Red = false;
				Green = true;
				Yellow = false;
				break;
			case 'Y':
				Red = false;
				Green = false;
				Yellow = true;
		}	
		
		DEnable = 1;
		putch(c);
		while(!TRMT)CLRWDT(); 
    }
}

/*
void interrupt intfunc(void){
	if (RCIF){
		Reciever = RCREG;
	}
}
*/

