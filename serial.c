#include <pic.h>
#include "delay.h"
#define	CLRWDT()	asm("clrwdt")
#define false 0
#define FALSE 0
#define true 1
#define TRUE 1
#define FOSC PIC_CLK      			// <<< clock freq (ie: 4000000 Hz)
// #define BAUD 9600         			// <<< in bits per second (ie: 9600bps)
#define SPBRG_SET 129        			// for FOSC 20MHz
#define BRGH_SET 1          			// for error 0.16
#define DEnable  RC5


void serial_setup(void){
	SPBRG = SPBRG_SET;
	BRGH = BRGH_SET;	  				//data rate for sending/receiving
	SYNC = 0;						//asynchronous
	SPEN = 1;						//enable serial port pins
	CREN = 1;						//enable reception
	SREN = 0;						//no effect
	TXIE = 0;						//disable tx interrupts
	RCIE = 0;						//disable rx interrupts
	TX9 = 0;						//8-bit transmission
	RX9 = 0;						//8-bit reception
	TXEN = 0;						//reset transmitter
	TXEN = 1;						//enable the transmitter
}

//unsigned char dummy;

#define clear_usart_errors_inline	\
if (OERR)				\
{					\
    unsigned char temp;			\
    temp = RCREG;                       \
    CREN=0;				\
    CREN=1;				\
}					\
else if (FERR)                          \
{                                       \
    CREN=0;				\
    CREN=1;                             \
}

void putch(unsigned char c) {
    //DEnable = 1;		
    while(!TXIF) CLRWDT();
    TXREG = c;
}

unsigned char getch(void){
    while(!RCIF){
        clear_usart_errors_inline;
        CLRWDT();
    }
    return RCREG;
}

unsigned char getch_available(void) {
    if (RCIF) return true;
    return false;
}

void clear_buffer(void) {
  clear_usart_errors_inline;
}

void clear_usart_errors(void) {
	clear_usart_errors_inline;
}

void putchhex(unsigned char c) {
	unsigned char temp;

	// transmits in hex

	temp = c;

	c = (c >> 4);
	if (c < 10) c += 48; else c += 55;
	putch(c);

	c = temp;

	c = (c & 0x0F);
	if (c < 10) c += 48; else c += 55;
	putch(c);
}

void putinthex(unsigned int c) {
	#define ramuint(x) (*((unsigned int *) (x)))
	#define ramuint_hibyte(x) (*(((unsigned char *)&x)+1))
	#define ramuint_lobyte(x) (*(((unsigned char *)&x)+0))
	#define ramuchar(x) (*((unsigned char *) (x)))

	putchhex(ramuint_hibyte(c));
	putchhex(ramuint_lobyte(c));

	#undef ramuint(x)
	#undef ramuint_hibyte(x)
	#undef ramuint_lobyte(x)
	#undef ramuchar(x)
}


void putchdec(unsigned char c)
{
	unsigned char temp;

	temp = c;
	//hundreds
	if ((temp / 100) > 0) putch((temp / 100) + '0');
	temp -= (temp / 100) * 100;

	//tens
	if ((temp / 10) > 0 ) putch((temp / 10)+'0');
	temp -= (temp / 10) * 10;

	//ones
	putch((temp / 1)+'0');
}

void putst(register const char *str)
{
	while((*str)!=0)
	{
		putch(*str);
    if (*str==13) putch(10);
    if (*str==10) putch(13);
		str++;
	}
}

