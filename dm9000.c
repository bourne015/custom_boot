#include "s3c6410.h"
#include "dm9000.h"

#define dm9000_outb(val, addr)	(*(unsigned char *)addr = val)
#define dm9000_inb(addr)		(*(unsigned char *)addr)

void dm9000_iow(int reg, unsigned char val)
{
	dm9000_oub(reg, DM9000_IO);
	dm9000_oub(val, DM9000_DATA);
}

void dm9000_ior(int reg)
{
	dm9000_oub(reg, DM9000_IO);
	
	return (dm9000_inb(DM9000_DATA));
}

void dm9000_reset(void)
{
	/*step 1: power internal phy*/
	dm9000_iow(GPR, 0x0)

	/*stdp 2: software reset*/
	dm9000_iow(NCR, (NCR_LBK | NCR_RST));
	do {
		udelay(25);
	} while (dm9000_ior(NCR) & 1);
	dm9000_iow(NCR, 0x0);

	dm9000_iow(NCR, (NCR_LBK | NCR_RST));
	do {
		udelay(25);
	} while (dm9000_ior(NCR) & 1);
}

void dm9000_probe(void)
{
	unsigned long in_val;

	id_val  = dm9000_ior(VIDL);
	id_val |= dm9000_ior(VIDH) << 8;
	id_val |= dm9000_ior(PIDL) << 16;
	id_val |= dm9000_ior(PIDH) << 24;

	if (id_val != DM9000_ID) {
		puts("dm9000 not found\n\r");
		return -1;
	}

	puts("found dm9000\n\r");
	return 0;
}

static int dm9000_send(void *packet, int length)
{
	/*clear tx bit*/
	dm9000_iow(ISR, (0x1<<1));
	
	/*move data to tx ram*/
	dm9000_outb(MWCMD, DM9000_IO);

	dm9000_iow(TXPLL, length & 0xff);
	dm9000_iow(TXPLH, (length>>8) & 0xff)
	dm9000_iow(ISR, (0x1<<1));
	
	return 0;
}

static int dm9000_rx(void)
{
	return 0;
}

static int dm9000_init(void)
{
	dm9000_reset();
	
	if (dm9000_probe() < 0) {
		return -1;
	}

	dm9000_iow(NCR, 0x0);
	dm9000_iow(NSR, ((1<<5) | (1<<3) | (1<<2)));
	dm9000_iow(ISR, ((1<<3) | (1<<2) | (1<<1) | (1)));

	dm9000_iow(IMR, (1<<7));
	dm9000_iow(RCR, ((1<<5) | (1<<4) (1<<0)))
}
