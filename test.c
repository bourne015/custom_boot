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

void key0_4_irq(void)
{
        int i;
        GPMCON &= ~(0xffff);
        GPMCON |= 0x1111;

        for (i = 0; i < 6; i++) {
                if (EINT0PEND & (1 << i)) {
                        if (GPNDAT & (1 << i))
                                GPMDAT &= ~(1 << i);
                        else
                                GPMDAT |= (1 << i);
                }
        }
}

void key5_6_irq(void)
{
        int i = 0;

        GPMCON &= ~(0xffff);
        GPMCON |= 0x1111;

        for (i = 4; i < 6; i ++) {
                if (EINT0PEND & (1<<i)) {
                        if (GPNDAT & (1<<i))
                                GPMDAT = 0xf;
                        else
                                GPMDAT = 0x0;
                }
        }
}

void key_irq_init()
{
        /*set GPIO to interrupt mode*/
        GPNCON &= ~(0xfff);
        GPNCON |= (0xaaa);

        /*set signal method,both edge*/
        EINT0CON0 &= ~(0xfff);
        EINT0CON0 |= 0x777;

        /*enable*/
        EINT0MASK &= ~(0x3f);

        /*enable VIC0(eint0-5)*/
        VIC0INTENABLE |= 0x3;

        VIC0VECTADDR0 = key0_4_irq;
        VIC0VECTADDR1 = key5_6_irq;
}
/*
void led(unsigned int x)
{
	GPMCON &= ~0xffff;
	GPMCON |= 0x1111;

	GPMDAT = x;
}
*/
