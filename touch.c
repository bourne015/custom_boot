#include "s3c6410.h"

void puts(char *);

void touch_init(void)
{
	/*pclk:66Mhz, AD freq:2Mhz*/
	ADCCON = (1<<16) | (1<<14) | (32<<6);

	ADCDLY = 10000 & 0xffff;
	
	//ADCTSC = (0<<8) | (1<<7) | (1<<6) | (1<<4) | (0x3);
	ADCTSC = 0xd3;
	
}

void int_pen_up_down(void)
{
	unsigned char val;
	
	val = ADCUPDN;
	if (val & 0x01) {
		puts("down\n\r");
		ADCCON |= 0x01;
	} else if (val &0x02) {
		puts("up\n\r");
		ADCCON |= 0x01;
	}

	ADCCLRINTPNDNUP = 0x0;
}

void int_adc(void)
{
	unsigned long val;

	val = ADCDAT0;

	ADCCLRINT = 0x0;
}

void touch_test(void)
{
	touch_init();

	VIC1VECTADDR30 = int_pen_up_down;
	VIC1VECTADDR31 = int_adc;
	VIC1INTENABLE |= (1<<31) | (1<<30);
}
