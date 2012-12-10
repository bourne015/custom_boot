#include "s3c6410.h"
#define HCLK		133
#define nstoclk(ns)	(ns / (1000/HCLK) + 1)

void ddr_init(void)
{
	/*makes DRAM enter "config" state*/
	P1MEMCCMD = 0x04;
	
	/*set memory timing parameter, chip config , 
	  id config register,etc...
	*/
	P1REFRESH = nstoclk(7800); //7.8us
	P1CASLAT = (3 << 1);
	P1T_DQSS = 0X1;
	P1T_MRD = 0X2;
 	P1T_RAS = nstoclk(45);
	P1T_RC = nstoclk(68);
	P1T_RCD = nstoclk(23);//| ((nstoclk(23)-3) << 3);	
	P1T_RFC = nstoclk(80);//| ((nstoclk(80)-3)  << 5);	
	P1T_RP = nstoclk(23);//| ((nstoclk(23)-3) << 3);	
	P1T_RRD = nstoclk(15);
	P1T_WR = nstoclk(15);
	P1T_WTR = 0X7;
	P1T_XP = 0X2;
	P1T_XSR = nstoclk(120);
	P1T_ESR = nstoclk(120);
	
	/*mem config*/
	P1MEMCFG &= ~(0x3f);
	P1MEMCFG |= 0x2; 	//10 columns
	P1MEMCFG |= (1 << 4);	//13 rows , 256M

	P1MEMCFG &= ~(0x7 << 15);
	P1MEMCFG |= (1 << 16); //burst 4

	P1MEMCFG2 |= 0x5;

	P1MEMCFG2 &= ~(0x3 << 6);
	P1MEMCFG2 |= (1 << 6);  //32bit

	P1MEMCFG2 &= ~(0x7 << 8);
	P1MEMCFG2 |= (0x3 << 8);

	P1MEMCFG2 &= ~(0x3 << 11);
	P1MEMCFG2 |= (1 << 11);

	p1_chip_0_cfg |= 1 << 16;

	P1DIRECTCMD = 0xc0000; //nop
	P1DIRECTCMD = 0x000;   //prechargeAII
	P1DIRECTCMD = 0x40000; //autorefresh 
	P1DIRECTCMD = 0x40000; 
	P1DIRECTCMD = 0xa0000; //MRS
	P1DIRECTCMD = 0x80032; //EMRS
	
	MEM_SYS_CFG = 0X0;
	
	/*set DRAM to "ready" state*/
	P1MEMCCMD = 0x000;
	
	/*wait for "ready" state*/
	while ((P1MEMSTAT & 0X3) != 0X1);
}
