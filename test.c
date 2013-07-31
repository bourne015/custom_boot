#include "s3c6410.h"
#include "common.h"

void delay2(void)
{
	volatile int i = 30000;
	while (i--);
}

void led_run(void)
{
	int i = 16;
	unsigned long data;
	unsigned long key = 0xeeeeee;
	
	data = SYSREG_R(GPMCON);
	data &= ~0xffff;
	data |= 0x1111;
	SYSREG_W(GPMDAT, data);

	while (i--) {
		SYSREG_W(GPMDAT, key);
		key = key >> 1;
		delay2();
	}
}

void led(unsigned int x)
{
	unsigned long data;

	data = SYSREG_R(GPMCON);
	data &= ~0xffff;
	data |= 0x1111;
	SYSREG_W(GPMDAT, data);

	SYSREG_W(GPMDAT, x);
}
/*
void test(void)
{
	puts("\n\rcustom uboot for ok6410\n\r");
}
*/
