#include "s3c6410.h"

void clock_init(void)
{
	APLL_LOCK = 0xffff;
	MPLL_LOCK = 0xffff;
	EPLL_LOCK = 0xffff;
	
	/*async mode*/
	OTHERS &= ~0xc0;
	while ((OTHERS & 0xf00) != 0);

	CLK_DIV0 = (ARM_RATIO) | (MPLL_RATIO << 4) | (HCLK_RATIO << 8) |
			(HCLKX2_RATIO << 9) | (PCLK_RATIO << 12);	

	/*APLL = 532 Mhz*/
	APLL_CON = APLL_VAL;
	MPLL_CON = MPLL_VAL;
	CLK_SRC = 0x3;
}
