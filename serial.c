#include <pic.h>
#define	CLRWDT()	asm("clrwdt")
#define false 0
#define FALSE 0
#define true 1
#define TRUE 1
#define FOSC PIC_CLK      			// <<< clock freq (ie: 4000000 Hz)
#define BAUD 19200         			// <<< in bits per second (ie: 19200bps)
#define SPBRG_SET 64        			// for FOSC 20MHz
#define BRGH_SET 1          			// for error 0.16


void serial_setup(void)
{
	SPBRG=SPBRG_SET;
	BRGH=BRGH_SET;	  				//data rate for sending/receiving
	SYNC=0;						//asynchronous
	SPEN=1;						//enable serial port pins
	CREN=1;						//enable reception
	SREN=0;						//no effect
	TXIE=0;						//disable tx interrupts
	RCIE=0;						//disable rx interrupts
	TX9=0;						//8-bit transmission
	RX9=0;						//8-bit reception
	TXEN=0;						//reset transmitter
	TXEN=1;						//enable the transmitter
}

unsigned char dummy;

#define clear_usart_errors_inline	\
if (OERR)		\
{																	\
	TXEN=0;		\
	TXEN=1;		\
	CREN=0;		\
	CREN=1;		\
}																	\
if (FERR)               \
{																	\
	dummy=RCREG;	\
	TXEN=0;		\
	TXEN=1;		\
}

//writes a character to the serial port
void putch(unsigned char c)
{
	while(!TXIF)			//set when register is empty
	{
		clear_usart_errors_inline;
		CLRWDT();
	}
	TXREG=c;
}

//gets a character from the serial port without timeout
unsigned char getch(void)
{
	while(!RCIF){
		CLRWDT();
		clear_usart_errors_inline;
	}
	return RCREG;
}
/*
unsigned char getch_available(void) {
  if (RCIF) return true;
  return false;
}
*/
void clear_buffer(void) {
  clear_usart_errors_inline;
}

/*
unsigned char getch_timeout(void)
{
	unsigned char i;
	unsigned int timeout_int;

	// retrieve one byte with a timeout
	for (i=2;i!=0;i--)
	{
		timeout_int=timeout_int_us(240000);

		while (hibyte(timeout_int)!=0) //only check the msb of the int for being 0, it saves space, see always.h for macro
		{
			CLRWDT();
			timeout_int--;
			if (RCIF)
			{
				return RCREG;
			}
		}
	}
	return 0;
}
*/

void clear_usart_errors(void) {
	clear_usart_errors_inline;
}



/*
writes a character to the serial port in hex
if serial lines are disconnected, there are no errors
*/

void putchhex(unsigned char c) {
	unsigned char temp;

	// transmits in hex

	temp=c;

	c=(c >> 4);
	if (c<10) c+=48; else c+=55;
	putch(c);

	c=temp;

	c=(c & 0x0F);
	if (c<10) c+=48; else c+=55;
	putch(c);
}

void putinthex(unsigned int c) {
	#define ramuint(x)						(*((unsigned int *) (x)))
	#define ramuint_hibyte(x)			(*(((unsigned char *)&x)+1))
	#define ramuint_lobyte(x)			(*(((unsigned char *)&x)+0))
	#define ramuchar(x)						(*((unsigned char *) (x)))

	putchhex(ramuint_hibyte(c));
	putchhex(ramuint_lobyte(c));

	#undef ramuint(x)
	#undef ramuint_hibyte(x)
	#undef ramuint_lobyte(x)
	#undef ramuchar(x)
}

//if there has been a previous timeout error from getch_timeout, this returns TRUE
unsigned char usart_timeout(void)
{
//	return usart_timeout_error;
	return FALSE;
}

/*
writes a character to the serial port in decimal
if serial lines are disconnected, there are no errors
*/
void putchdec(unsigned char c)
{
	unsigned char temp;

	temp=c;
	//hundreds
	if ((c/100)>0) putch((c/100)+'0');
	c-=(c/100)*100;

	//tens
	if (((temp/10)>0) || ((temp/100)>0)) putch((c/10)+'0');
	c-=(c/10)*10;

	//ones
	putch((c/1)+'0');
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


