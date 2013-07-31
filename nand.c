#include "s3c6410.h"

void nand_reset(void)
{
	unsigned long val;
	/*enable chip select*/
	val = SYSREG_R(NFCONT);
	val &= ~(1 << 1);
	SYSREG_W(NFCONT, val);

	/*reset*/
	SYSREG_W(NFCMMD, 0xff);

	/*wait nand ready to operw*/
	while ((SYSREG_R(NFSTAT) & 0x1) == 0);
	
	/*disenable chip select*/
	val = SYSREG_R(NFCONT);
	val |= (1 << 1);
	SYSREG_W(NFCONT, val);
}

void nand_send_addr(unsigned int addr)
{
	/*1st cycle a0~a7*/
	SYSREG_W(NFADDR, (addr&0xff));
	/*2nd cycle a8~a12*/
	SYSREG_W(NFADDR, ((addr >> 7) & 0X1F));
	/*3rd cycle a13~a20*/
	SYSREG_W(NFADDR, ((addr >> 12) & 0xff));
	/*4th cycle a21~a28*/
	SYSREG_W(NFADDR, ((addr >> 20) & 0xff));
	/*5th cycle a29~a31*/
	SYSREG_W(NFADDR, ((addr >> 28) & 0x07));
}

int 
nand_read(unsigned int nand_start, unsigned int ddr_start, unsigned int len)
{
	unsigned int addr = nand_start;
	int count = 0, i = 0;
	unsigned char *dest = (unsigned char *)ddr_start;
	unsigned char data = 0;
	unsigned long val; //regiser value

	val = SYSREG_R(NFCONT);
	val &= ~(1 << 1);
	SYSREG_W(NFCONT, val);

	while (count < len) {
		SYSREG_W(NFCMMD, 0X00);
		nand_send_addr(addr);
		SYSREG_W(NFCMMD, 0X30);

		while ((SYSREG_R(NFSTAT) & 0x1) == 0);

		for (i = 0; i < 4096 && count < len; i++) {
			data = SYSREG_R(NFDATA);
			/*for the 1st 4*4K, just used 2K each*/
			if(addr < 16384) {
				if(i < 2048)
					dest[count++] = data;
			} else
				dest[count++] = data;
		}
		addr += 4096;
	}
	
	val = SYSREG_R(NFCONT);
	val |= (1 << 1);
	SYSREG_W(NFCONT, val);

	return 0;
}
