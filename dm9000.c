#include <stdio.h>
#include "s3c6410.h"
#include "dm9000.h"
#include <common.h>

#define dm9000_outb(val, addr)	(*(unsigned char *)(addr) = (val))
#define dm9000_inb(addr)	(*(unsigned char *)(addr))
#define dm9000_outw(val, addr)	(*(u16 *)(addr) = (val))
#define dm9000_inw(addr)	(*(u16 *)(addr))

void dm9000_iow(int reg, unsigned char val)
{
	dm9000_outb(reg, DM9000_IO);
	dm9000_outb(val, DM9000_DATA);
}

static unsigned char dm9000_ior(int reg)
{
	dm9000_outb(reg, DM9000_IO);
	
	return (dm9000_inb(DM9000_DATA));
}

void dm9000_reset(void)
{
	/*step 1: power internal phy*/
	dm9000_iow(GPR, 0x0);

	/*stdp 2: software reset*/
	dm9000_iow(NCR, ((1<<1) | (1<<0)));
	do {
		mdelay(1);
	} while (dm9000_ior(NCR) & 1);
	dm9000_iow(NCR, 0x0);

	dm9000_iow(NCR, ((1<<1) | (1<<0)));
	do {
		mdelay(1);
	} while (dm9000_ior(NCR) & 1);
}

static int dm9000_probe(void)
{
	unsigned long id_val;
	int i;
	
	/*try multiple times, codes from kernel*/
	for (i = 0; i < 8; i++) {
		id_val  = dm9000_ior(VIDL);
		id_val |= dm9000_ior(VIDH) << 8;
		id_val |= dm9000_ior(PIDL) << 16;
		id_val |= dm9000_ior(PIDH) << 24;

		if (id_val == DM9000_ID)
			break;
		printf("read wrong id %lu, ty again\n\r", id_val);
	}
	
	if (id_val != DM9000_ID) {
		printf("wrong id %lu\n\r", id_val);
		return -1;
	}

	return 0;
}

static int dm9000_send(volatile void *packet, int length)
{
	int i = 0, count;

	/*clear tx bit*/
	dm9000_iow(ISR, (0x1<<1));
	
	/*prapare to move data to tx ram*/
	dm9000_outb(MWCMD, DM9000_IO);
	
	/*push data to the tx-fifo*/
	count = (length + 1)>>2;
	for (i = 0; i < count; i++)
		dm9000_outw(((u16 *)packet)[i], DM9000_DATA);
	
	dm9000_iow(TCR, 0x1);
	/*set tx length*/
	dm9000_iow(TXPLL, length & 0xff);
	dm9000_iow(TXPLH, (length>>8) & 0xff);
	
	/*wait for end of transmission*/
	while (!(dm9000_ior(NSR) & ((1<<3) | (1<<2))));

	dm9000_iow(ISR, (0x1<<1));

	return 0;
}

static int dm9000_rx(volatile void *packet)
{
	int rx_len, rx_status, tmplen;
	int rx;
	int i = 0;
	
	/*check if we received packet*/
	if (!(dm9000_ior(ISR) & 0x1))
		return 0;
	
	/*clear latched status*/
	dm9000_iow(ISR, 0x1);

	/*dummy read, read out last data 
	 *of read command
	 */
	dm9000_ior(MRCMDX);

	rx = dm9000_inb(DM9000_DATA) & 0x3;

	if (rx != 0x1)
		return 0;
		
	rx_status = dm9000_inw(DM9000_DATA);
	rx_len = dm9000_inw(DM9000_DATA);

	tmplen = (rx_len + 1) >> 1;
	for (i = 0; i <  tmplen; i++)
		((u16 *)packet)[i] = dm9000_inw(DM9000_DATA);	

	return 0;
}

static int dm9000_init(void)
{
	int i = 0;

	dm9000_reset();
	
	if (dm9000_probe() < 0) {
		return -1;
	}

	dm9000_iow(NCR, 0x0);
	/*clear tx status*/
	dm9000_iow(NSR, ((1<<5) | (1<<3) | (1<<2)));
	/*clear interrupt status*/
	dm9000_iow(ISR, ((1<<3) | (1<<2) | (1<<1) | (1)));
	/*
	* enable sram point return to start, when
	* the point address are over the sram size
	*/
	dm9000_iow(IMR, (1<<7));
	/*rx enable*/
	dm9000_iow(RCR, ((1<<5) | (1<<4) | (1<<0)));

	/*read mac address*/
	for (i = 0; i < 6; i++) {
		printf("mac addr: %c ", dm9000_ior(PAR+i));
	}
}
