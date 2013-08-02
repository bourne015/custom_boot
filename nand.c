#include "s3c6410.h"

void nand_reset(void)
{
	/*enable chip select*/
	NFCONT &= ~(1 << 1);

	/*reset*/
	NFCMMD = 0xff;

	/*wait nand ready to operw*/
	while ((NFSTAT & 0x1) == 0);
	
	/*disenable chip select*/
	NFCONT |= (1 << 1);
}

void nand_init(void)
{
	MEM_SYS_CFG &= ~(1 << 1);
	NFCONF &= ~((1 << 30) | (0x7 << 12) | (0x7 << 8) | (0x7 << 4));
	NFCONF |= ((TWRPH1 << 4) | (TWRPH0 << 8) | (TACLS << 12));

	/*enable nand controller*/
	NFCONT |= 0x1;

	/*no lock*/
	NFCONT &= ~(1 << 16);

	nand_reset();
}

void nand_send_addr(unsigned int addr)
{
	/*1st cycle a0~a7*/
	NFADDR = (addr & 0xff);
	/*2nd cycle a8~a12*/
	NFADDR = ((addr >> 7) & 0x1f);
	/*3rd cycle a13~a20*/
	NFADDR = ((addr >> 12) & 0xff);
	/*4th cycle a21~a28*/
	NFADDR = ((addr >> 20) & 0xff);
	/*5th cycle a29~a31*/
	NFADDR = ((addr >> 28) & 0x7);
}

int 
nand_read(unsigned int nand_start, unsigned int ddr_start, unsigned int len)
{
	unsigned int addr = nand_start;
	int count = 0, i = 0;
	unsigned char *dest = (unsigned char *)ddr_start;
	unsigned char data = 0;

	NFCONT &= ~(1 << 1);

	while (count < len) {
		NFCMMD = 0x00;
		nand_send_addr(addr);
		NFCMMD = 0x30;
		while ((NFSTAT & 0x1) == 0);

		for (i = 0; i < 4096 && count < len; i++) {
			data = NFDATA;
			dest[count++] = data;
		}
		addr += 4096;
	}

	NFCONT |= (1 << 1);
	
	return 0;
}

void 
nand2ddr(unsigned int nand, unsigned int ddr, unsigned int len)
{
	nand_init();
	nand_read(nand, ddr, len);
}
