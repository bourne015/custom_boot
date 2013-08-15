#include "s3c6410.h"

void puts(char *);

void stylus_up_down(void)
{
	if ((ADCDAT0 & (1<<15))) {
		//stylus up
		full_color(0x00ddff);
		//detect stylus down
		ADCTSC = 0xd3;
	} else {
		//stylus down
		//wait up
		//ADCTSC = 0x1d3;
		
		//set to auto mode,disable pull-up
		ADCTSC = (1<<2) | (1<<3);
		full_color(0xffdd00);

		//start adc convert
		ADCCON |= 0x01;
	}

	ADCCLRINTPNDNUP = 0x0;
}

void stylus_adc(void)
{
	volatile unsigned long x, y;

	x = ADCDAT0 & 0x3ff;
	y = ADCDAT1 & 0x3ff;
	//wait stylus up
	ADCTSC = 0x1d3;

	ADCCLRINT = 0x0;
}

void touch_init(void)
{
	/*pclk:66Mhz, AD freq:2Mhz*/
	ADCCON = (1<<16) | (1<<14) | (32<<6);

	ADCDLY = 10000 & 0xffff;
	
	//enable updn and adc irq,and irq_handler
	VIC1VECTADDR30 = stylus_up_down;
	VIC1VECTADDR31 = stylus_adc;
	VIC1INTENABLE |= (1<<31) | (1<<30);

	//ADCTSC = (0<<8) | (1<<7) | (1<<6) | (1<<4) | (0x3);
	ADCTSC = 0xd3;
}

void touch_test(void)
{
	touch_init();
	key_irq_init();	
}
