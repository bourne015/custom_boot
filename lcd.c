#define sysreg_r(reg) (*(volatile unsigned long *)reg)
#define sysreg_w(reg, val) (*(volatile unsigned long *)reg = val)

#define GPBCON	0x7f008020
#define GPBDAT	0x7f008024
#define GPICON	0x7f008100
#define GPJCON	0x7f008120
#define GPECON	0x7f008080
#define GPEDAT	0x7f008084
#define GPFCON	0x7f0080a0
#define GPFDAT	0x7f0080a4
#define GPPCON  0x7f008160

#define SPCON	0x7f0081a0
#define MIFPCON	0x7410800c

#define VIDCON0		0x77100000
#define VIDCON1		0x77100004
#define VIDCON2		0x77100008
#define VIDTCON0	0x77100010
#define VIDTCON1	0x77100014
#define VIDTCON2	0x77100018
#define WINCON0		0x77100020
#define VIDOSD0A	0x77100040
#define VIDOSD0B	0x77100044
#define VIDOSD0C	0x77100048
#define VIDW00ADD0B0	0x771000a0
#define VIDW00ADD1B0	0x771000d0
#define VIDW00ADD2	0x77100100

#define VBPD	1
#define VFPD	1
#define VSPW	10

#define HBPD	2	
#define HFPD	2
#define HSPW	41

#define LINEVAL 271
#define HOZVAL	479

#define LEFTTOP_X 0
#define LEFTTOP_Y 0
#define RIGHTBOT_X 491
#define RIGHTBOT_Y 271

#define FRAME_BUFFER 0x58000000

void puts(char *);

void lcd_init(void)
{
	unsigned long val;

	sysreg_w(GPJCON, 0xaaaaaaa);
	sysreg_w(GPICON, 0xaaaaaaaa);

	val = sysreg_r(GPFCON);
	val &= ~(0x3 << 28);
	val |= (0x1 << 28);
	sysreg_w(GPFCON, val);

	val = sysreg_r(GPECON);
	val &= ~(0xf);
	val |= (0x1);
	sysreg_w(GPECON, val);

	val = sysreg_r(SPCON);
	val &= ~(0x3);
	val |= 0x1;
	sysreg_w(SPCON, val);

	val = sysreg_r(VIDCON0);
	val &= ~((0x3 << 26) | (0x3 << 17) | (0xff << 6) |
		(0x3 << 2) | (0x3));
	
	val |= (0x1 << 4) | (7 << 6);
	sysreg_w(VIDCON0, val);

	val = sysreg_r(MIFPCON);
	val &= ~(0x1 << 3);
	sysreg_w(MIFPCON, val);

	val = sysreg_r(VIDCON1);
	val |= ((0x1 << 5) | (0x1 << 6));
	val &= ~((0x1 << 4) | (0x1 << 7));
	sysreg_w(VIDCON1, val);

	val = (VSPW | (VFPD << 8) | (VBPD << 16));
	sysreg_w(VIDTCON0, val);

	val = (HSPW | (HFPD << 8) | (HBPD << 16));
	sysreg_w(VIDTCON1, val);

	val = (HOZVAL | (LINEVAL << 11));
	sysreg_w(VIDTCON2, val);

	val = sysreg_r(WINCON0);
	val &= ~(0xf << 2);
	val |= (0xb << 2) | 0x1;

	val = (LEFTTOP_Y | (LEFTTOP_X << 11));
	sysreg_w(VIDOSD0A, val);

	val = ((RIGHTBOT_Y+1) | ((RIGHTBOT_X+1) << 11));
	sysreg_w(VIDOSD0B, val);

	val = (LINEVAL+1) * (HOZVAL+1);
	sysreg_w(VIDOSD0C, val);

	val = FRAME_BUFFER;
	sysreg_w(VIDW00ADD0B0, val);

	val = (FRAME_BUFFER & 0xffffff) + ((HOZVAL+1) * (LINEVAL+1));
	//val = (((HOZVAL+1)*4) * (LINEVAL+1)) & 0xffffff;
	sysreg_w(VIDW00ADD1B0, val);

	val = HOZVAL+1;
	sysreg_w(VIDW00ADD2, val);
}

void backlight_enable(void)
{
	unsigned long val;
	val = sysreg_r(GPFDAT);
	val |= (1<<14);
	sysreg_w(GPFDAT, val);
}

void backlight_disable(void)
{
	unsigned long val;
	val = sysreg_r(GPFDAT);
	val &= ~(1<<14);
	sysreg_w(GPFDAT, val);
}


void lcd_on(void)
{
	unsigned long val;
	/* 等待10 frame */
	val = sysreg_r(GPEDAT);
	val |= (1);
	sysreg_w(GPEDAT, val);
}

void lcd_off(void)
{
	unsigned long val;
	val = sysreg_r(GPEDAT);
	val &= ~(1 << 0);
	sysreg_w(GPEDAT, val);
}

void display_on(void)
{
	unsigned long val;

	val = sysreg_r(VIDCON0);
	val |= (0x3);
	sysreg_w(VIDCON0, val);

	val = sysreg_r(WINCON0);
	val |= (0x1);
	sysreg_w(WINCON0, val);
}

void display_off(void)
{
	unsigned long val;

	val = sysreg_r(VIDCON0);
	val &= ~(0x3);
	sysreg_w(VIDCON0, val);

	val = sysreg_r(VIDCON0);
	val &= ~(0x1);
	sysreg_w(WINCON0, val);
}

void lcd_enable(void)
{
	lcd_on();
	backlight_enable();
	/*display controller */
	display_on();
}

void lcd_disable(void)
{
/* 关闭背光 */
/* 关闭LCD本身 */
/* 关闭display controller */
}

void display_red(void)
{
	volatile unsigned long **p = (volatile unsigned long *)FRAME_BUFFER;
	int x, y;
	//int cnt = 0;
	unsigned long colors[] = {0xff0000, 0x00ff00, 0x0000ff, 0, 0xffffff};
	//unsigned long colors[] = {0x10ddff, 0x00ff00, 0x0000ff, 0, 0xffffff};
	static int color_idx = 0;

	for (y = 1; y <= LINEVAL; y++) {
		for (x = 1; x <= HOZVAL; x++) {
			//p[cnt++] =colors[color_idx] ; /* red */
			p[y][x] =colors[color_idx] ; /* blue */
		}
	}
}


void lcd_test(void)
{
	puts("\n\r");
	puts("\ttest lcd\n\r");

	lcd_init();
	lcd_enable();
	display_red();
}
