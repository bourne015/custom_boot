#include "s3c6410.h"

void pwm_init(int light_val)
{
	/*just time 0*/
	GPFCON &= ~(0x3<<28);
	GPFCON |= (0x2<<28);

	/*timer = PCLK/(presvaler+1)/(divider)
	*	= 66/(1+1)/1
	*/
	TCFG0 &= ~(0xff);
	TCFG0 |= 0x1;
	TCFG1 &= ~(0xf);

	/*auto reloader*/
//	TCON &= ~(0x1f);
	TCON |= (1<<3);

	TCNTB0 = 33000;
	if (light_val) {
		TCMPB0 = 33000/light_val;
	} else{
		TCMPB0 = 33000;
	}

	pwm_start();
}

void pwm_start(void)
{
	/*invert on*/
	TCON |= (0x1<<1);
	/*start timer0*/
	TCON |= 0x1;
	/*clean manual update bit*/
	TCON &= ~(0x1<<1);
}

void pwm_stop(void)
{
	TCON &= ~(0x1);
}

int backlight_change(char c)
{
	static int cnt = 5;
	int light_val;

	if (c == '+') {
		light_val = 100/cnt;
		pwm_init(light_val);
		cnt += 5;
		if (cnt > 100)
			cnt = 100;
	} else if (c == '-') {
		light_val = 100/cnt;
		pwm_init(light_val);
		cnt -= 5;
		if (cnt < 5)
			cnt = 5;
	} else {
		printf("Usage: input '+' or '-'");
		return -1;
	}

	return 0;
}
