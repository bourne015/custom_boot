#include "s3c6410.h"

/*
char getchar(void)
{
	while ((UFSTAT0 & 0x7f) == 0);
	
	return URXH0;
}
*/
void putc(char c)
{
	while (SYSREG_R(UFSTAT0) & (1 << 14));

	SYSREG_W(UTXH0,c);
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
