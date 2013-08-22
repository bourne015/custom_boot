#include "s3c6410.h"

/*when 'time' increase '1', it's 100us*/
volatile unsigned long time = 0;

void mdelay(unsigned long delay_val)
{
	unsigned long val = delay_val*10;

	val += time;
	while (time != val);
}

void timer2_irq(void)
{
	time++;
	VIC0ADDRESS = 0;
	TINT_STAT |= (1<<7);
}

void timer2_start(void)
{
	TCON |= (1<<13);
	TCON |= (1<<12);
	TCON &= ~(1<<13);

	TINT_STAT |= (1<<2);
}

void timer2_stop(void)
{
	TCON &= ~(1<<12);
	TINT_STAT &= ~(1<<2);
}

void timer2_init(void)
{
	/*pclk/2/1=33250000*/
	TCFG0 &= ~(0xff<<8);
	TCFG0 |= (1<<8);
	
	TCFG1 &= ~(0xff<<8);

	/*auto reloader*/
	TCON |= (1<<15);

	/*0.1ms*/
	TCNTB2 = 33250;

	VIC0VECTADDR25 = timer2_irq;
	VIC0INTENABLE |= (1<<25);

	timer2_start();
}
