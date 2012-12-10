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
	unsigned long key = 0xeeeeee;

	GPMCON &= ~0xffff;
	GPMCON |= 0x1111;
	while (i--) {
		key = key >> 1;
		GPMDAT = key;
		delay2();
	}
}

void led(unsigned int x)
{
	GPMCON &= ~0xffff;
	GPMCON |= 0x1111;

	GPMDAT = x;
}

void test(void)
{
	puts("\n\rcustom uboot for ok6410\n\r");
}
