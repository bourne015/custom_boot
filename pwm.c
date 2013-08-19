#include "s3c6410.h"

void pwm_init(void)
{
	/*just pwm 0*/
	GPFCON &= ~(0x3<<28);
	GPFCON |= (0x2<<28);

	/*timer = PCLK/(presvaler+1)/(divider)
	*	= 66/(1+1)/1
	*/
	TCFG0 & = ~(0xff);
	TCFG0 |= 0x1;
	TCFG1 &= ~(0xf);

	/*auto reloader*/
	TCON &= ~(0x1f);
	TCON |= (1<<3)

	TCNTB0 = 33000;
	TCMPB0 = 33000;
}

void pwm_start(void)
{
	TCON0 |= (0x1<<1);
	TCON0 |= 0x1;
	TCON0 &= ~(0x1<<1);
}

void pwm_stop(void)
{
	TCON0 &= ~(0x1);
}
