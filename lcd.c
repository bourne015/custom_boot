#include "s3c6410.h"
#include "font_8x8.h"

#define VBPD	2
#define VFPD	2
#define VSPW	10

#define HBPD	2	
#define HFPD	2
#define HSPW	40

#define LINEVAL 271
#define HOZVAL	479

#define LEFTTOP_X 0
#define LEFTTOP_Y 0
#define RIGHTBOT_X 491
#define RIGHTBOT_Y 271

void puts(char *);
void putc(char);
char getc(void);

volatile static unsigned long lcd_buffer[LINEVAL+1][HOZVAL+1];

void lcd_init(void)
{
	GPICON = 0xaaaaaaaa;
	GPIPUD = 0x00000000;
	GPJCON = 0xaaaaaa;
	GPJPUD = 0x00000000;

	GPFCON &= ~(0x3 << 28);
	GPFCON |= (0x1 << 28);

	GPECON &= ~(0xf);
	GPECON |= (0x1);

	MIFPCON &= ~(0x1 << 3);

	SPCON &= ~(0x3);
	SPCON |= 0x1;

	VIDCON0 &= ~((0x3<<26) | (0x3<<17) | (0xff<<6) | (0x3<<2));
	VIDCON0 |= (2<<6) | (1<<4) | (0x3);

	VIDCON1 |= ((0x1 << 5) | (3 << 6));
	VIDCON1 &= ~(0x1 << 7);

	VIDTCON0 = ((VBPD << 16) | (VFPD << 8) | VSPW);
	VIDTCON1 = ((HBPD << 16) | (HFPD << 8) | HSPW);
	VIDTCON2 = ((LINEVAL << 11) | HOZVAL);

	WINCON0 &= ~(0xf << 2);
	WINCON0 |= (0xb << 2);

	VIDOSD0A = ((LEFTTOP_X << 11) | LEFTTOP_Y);

	VIDOSD0B = (((RIGHTBOT_X) << 11) | RIGHTBOT_Y);

	VIDOSD0C = ((LINEVAL+1) * (HOZVAL+1)) & 0xffffff;

	VIDW00ADD0B0 = (unsigned long)lcd_buffer;

	VIDW00ADD1B0 = (((HOZVAL+1)*4)*(LINEVAL+1)+
			(unsigned long)lcd_buffer) & 0xffffff;

	VIDW00ADD2 = (HOZVAL+1)*4;

	DITHMODE = (1<<5) | (1<<3) | (1<<1)|(0x1);
}

void backlight_enable(void)
{
	GPFDAT |= (1<<14);
}

void backlight_disable(void)
{
	GPFDAT &= ~(1<<14);
}


void lcd_on(void)
{
	GPEDAT |= (1);
}

void lcd_off(void)
{
	GPEDAT &= ~(1);
}

void display_on(void)
{
	VIDCON0 |= (0x3);
	WINCON0 |= (0x1);
}

void display_off(void)
{
	VIDCON0 &= ~(0x3);
	WINCON0 &= ~(0x1);
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
	backlight_disable();
	lcd_off();
	/*display controller */
	display_off();
}

void full_color(const unsigned long color)
{
	int x, y;

	for (y = 0; y <= LINEVAL; y++) {
		for (x = 0; x <= HOZVAL; x++) {
			lcd_buffer[y][x] = color;
		}
	}
}


void draw(int x, int y, const unsigned long data)
{
	if ((x < 479) && (y < 272))
		lcd_buffer[y][x] = data;
}

void lcd_putc(int x, int y, const unsigned char c)
{
	int i, j;
	unsigned char line_dot;
	int num = (c-32)*8;
	
	for (i = 0; i < 8; i++) {
		line_dot = fontdata_8x8[num+i] & 0xff;
		for (j = 0; j < 8; j++) {
			if (line_dot & (0x80 >> j))
				draw(x+j, y+i, 0xff0000);
			else
				draw(x+j, y+i, 0xffffff);
		}
	}
}

void handle(char cho)
{
	char c;

	switch (cho) {
	case '1':
		puts("full red");
		full_color(0xdd0000);
		break;
	case '2':
		puts("full green");
		full_color(0x00dd00);
		break;
	case '3':
		puts("full blue");
		full_color(0x0000dd);
		break;
	case '6':
		backlight_enable();
		break;
	case '7':
		backlight_disable();
		break;
	case '8':
		display_on();
		break;
	case '9':
		display_off();
		break;
	case '0':
		full_color(0x0);
		break;
	case 'a':
		puts("\n\rinput char:");
		c = getc();
		lcd_putc(100, 100, c);
		break;
	default:
		puts("wrong choice!\n\r");
		break;
	}
}
void menu(void)
{
	char cho;
	while (1) {
		puts("\n\r");
		puts("\t   TEST LCD\n\r");
		puts("\t1. full screen red\n\r");
		puts("\t2. full screen green\n\r");
		puts("\t3. full screen blue\n\r");
		puts("\t6. backlight on\n\r");
		puts("\t7. backlight off\n\r");
		puts("\t8. display on\n\r");
		puts("\t9. display off\n\r");
		puts("\t0. clean screen\n\r");
		puts("\ta. putc in screen\n\r");
		puts("your chooice: ");
		cho = getc();
		handle(cho);
		putc(cho);
	}
}
void lcd_test(void)
{
	lcd_init();
	lcd_enable();
	menu();
}
