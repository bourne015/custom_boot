#include "s3c6410.h"

void uart_init(void)
{
	/*set to uart mode*/
	GPACON &= ~0xff;
	GPACON |= 0x22;

	/*set the frame*/
	ULCON0 = 0x03;

	/*set to interrupt mode and PCLK*/
	UCON0 = 0x805;
	
	/*enable FIFO*/
	UFCON0 = 0x01;
	
	/*don't use ACF*/
	UMCON0 = 0x01;
	
	/*boaud rate
	DIV_VAL = (PCLK / (bps * 16)) -1
		= (66500000 / (115200 * 16)) - 1
		= 35.08
	*/
	UBRDIV0 = 35;
	UDIVSLOT0 = 0X1;
}
/*
char getchar(void)
{
	while ((UFSTAT0 & 0x7f) == 0);
	
	return URXH0;
}
*/
void putc(char c)
{
	while (UFSTAT0 & (1 << 14));

	UTXH0 = c;
}

void puts(char *str)
{
	while (*str) {
		putc(*str++);
	}
}

void putmem(unsigned int val)
{
	int i = 0, j= 0;

	puts("0x");
	for (i = 0; i < 8; i++) {
		j = (val >> ((7-i)*4)) & 0xf;
		if ((j >= 0) && (j <= 9))
			putc('0' + j);
		else
			putc('A' + j - 0xa);
	}
}
