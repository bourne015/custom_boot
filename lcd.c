#define SYSREG_R(reg) (*(volatile unsigned long *)reg)
#define SYSREG_W(reg, val) (*(volatile unsigned long *)reg = val)

#define GPBCON	0x7f008020
#define GPBDAT	0x7f008024
#define GPICON	0x7f008100
#define GPJCON	0x7f008120
#define GPECON	0x7f008080
#define GPEDAT	0x7f008080
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
#define WINCON0		0x77100024
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
#define RIGHTBOT_X 271
#define RIGHTBOT_Y 479

#define FRAME_BUFFER 0x57000000

void puts(char *);

void lcd_init(void)
{
	unsigned long val;

	SYSREG_W(GPJCON, 0xaaaaaa);
	SYSREG_W(GPICON, 0xaaaaaaaa);

	val = SYSREG_R(GPFCON);
	val &= ~(0x3 << 28);
	val |= (0x1 << 28);
	SYSREG_W(GPFCON, val);

	val = SYSREG_R(GPECON);
	val &= ~(0xf);
	val |= (0x1);
	SYSREG_W(GPECON, val);

	val = SYSREG_R(GPBCON);
	val &= ~(0xf << 24);
	val |= (0x1 << 24);
	SYSREG_W(GPECON, val);

	val = SYSREG_R(SPCON);
	val &= ~(0x3);
	val |= 0x1;
	SYSREG_W(SPCON, val);

	val = SYSREG_R(VIDCON0);
	val &= ~((0x3 << 26) | (0x3 << 17) | (0xff << 6) |
		(0x3 << 2) | (0x3));
	
	val |= (0x1 << 4);
	SYSREG_W(VIDCON0, val);

	SYSREG_W(MIFPCON, 0x0);

	val = SYSREG_R(VIDCON1);
	val &= ~((0x1 << 5) | (0x1) << 6);
	val |= (0x1 << 4) | (0x1 << 7);
	SYSREG_W(VIDCON1, val);

	val = (VSPW | (VFPD << 8) | (VBPD << 16));
	SYSREG_W(VIDTCON0, val);

	val = (HSPW | (HFPD << 8) | (HBPD << 16));
	SYSREG_W(VIDTCON1, val);

	val = (HOZVAL | (LINEVAL << 11));
	SYSREG_W(VIDTCON2, val);

	val = SYSREG_R(WINCON0);
	val &= ~(0xf << 2);
	val |= (0xf << 2) | 0x1;

	val = (LEFTTOP_Y | (LEFTTOP_X << 11));
	SYSREG_W(VIDOSD0A, val);

	val = (RIGHTBOT_Y | (RIGHTBOT_X << 11));
	SYSREG_W(VIDOSD0B, val);

	val = LINEVAL * HOZVAL;
	SYSREG_W(VIDOSD0C, val);

	val = FRAME_BUFFER;
	SYSREG_W(VIDW00ADD0B0, val);

	val = FRAME_BUFFER + ((HOZVAL+1) * (LINEVAL+1));
	SYSREG_W(VIDW00ADD1B0, val);

	val = HOZVAL+1;
	SYSREG_W(VIDW00ADD2, val);
}

void backlight_enable(void)
{
	unsigned long val;
	val = SYSREG_R(GPFDAT);
	val |= (1<<14);
	SYSREG_W(GPFDAT, val);
}

void backlight_disable(void)
{
	unsigned long val;
	val = SYSREG_R(GPFDAT);
	val &= ~(1<<14);
	SYSREG_W(GPFDAT, val);
}


void lcd_on(void)
{
	unsigned long val;
	/* 等待10 frame */
	val = SYSREG_R(GPEDAT);
	val |= (1);
	SYSREG_W(GPEDAT, val);

	val = SYSREG_R(GPBDAT);
	val |= (1 << 6);
	SYSREG_W(GPBDAT, val);
}

void lcd_off(void)
{
	unsigned long val;
	val = SYSREG_R(GPEDAT);
	val &= ~(1 << 0);
	SYSREG_W(GPEDAT, val);
}

void displaycon_on(void)
{
	unsigned long val;

	val = SYSREG_R(VIDCON0);
	val |= (0x3);
	SYSREG_W(VIDCON0, val);

	val = SYSREG_R(VIDCON0);
	val |= (0x1);
	SYSREG_W(WINCON0, val);
}

void displaycon_off(void)
{
	unsigned long val;

	val = SYSREG_R(VIDCON0);
	val &= ~(0x3);
	SYSREG_W(VIDCON0, val);

	val = SYSREG_R(VIDCON0);
	val &= ~(0x1);
	SYSREG_W(WINCON0, val);
}

void lcd_enable(void)
{
	/* 使能LCD本身 */
	lcd_on();

	/* 打开背光 */
	backlight_enable();

	/* 使能display controller */
	displaycon_on();
}

void lcd_disable(void)
{
/* 关闭背光 */
/* 关闭LCD本身 */
/* 关闭display controller */
}
/*
void draw_line(void)
{
	DrawLine(0,0, 0,271, 0xff0000);
	DrawLine(0,0, 479,0, 0x00ff00);
	DrawLine(0,0, 479,271, 0x0000ff);
	DrawLine(0,271, 479,0, 0x00ffff);
	DrawLine(0,271, 479,271, 0xff00ff);
	DrawLine(479,271, 479,0, 0xffff00);
	DrawLine(0,136, 479,136, 0x0123ff);
	DrawLine(240,0, 240,271, 0x0567ff);
}
*/
void display_red(void)
{
	volatile unsigned long *p = (volatile unsigned long *)FRAME_BUFFER;
	int x, y;
	int cnt = 0;
	unsigned long colors[] = {0xff0000, 0x00ff00, 0x0000ff, 0, 0xffffff};
	static int color_idx = 0;

	for (y = 0; y <= LINEVAL; y++) {
		for (x = 0; x <= HOZVAL; x++) {
			p[cnt++] =colors[color_idx] ; /* red */
		}
	}

	color_idx++;
	if (color_idx == 5)
		color_idx = 0;
}


void lcd_test(void)
{
	puts("\n\r");
	puts("\ttest lcd\n\r");

	lcd_init();
	lcd_enable();
	display_red();
}
