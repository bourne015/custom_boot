#include "s3c6410.h"

.global lowlevel_init
lowlevel_init:
	mov	r12, lr
	
	/*disable watchdog*/	
	ldr	r0, =0x7e000000
	orr	r0, r0, #4000
	mov	r1, #0
	str	r1, [r0]

	bl	clock_init
	bl	uart_init
	bl	nand_init

	mov	lr, r12
	mov	pc, lr

clock_init:
        /*set the clock time*/
        ldr     r0, =APLL_LOCK
        ldr     r1, =0xffff
        str     r1, [r0]

        str     r1, [r0, #0x4]  /*MPLL_CLOCK*/
        str     r1, [r0, #0x8]  /*EPLL_CLOCK*/

        /*set sync mode*/
        ldr     r0, =OTHERS
        ldr     r1, [r0]
        bic     r1, #0xc0
        str     r1, [r0]

wait_for_async:
        ldr     r0, =OTHERS
        ldr     r1, [r0]
        and     r1, #0xf00
	cmp     r1, #0x00
        bne     wait_for_async

        ldr     r0, =CLK_DIV0
        ldr     r1, =DIV0_VAL
        str     r1, [r0]

        ldr     r0, =APLL_CON
        ldr     r1, =APLL_VAL
        str     r1, [r0]

        ldr     r0, =MPLL_CON
        str     r1, [r0]

        /*set the clk_src to choose pll but not xttpll*/
        ldr     r0, =CLK_SRC
        ldr     r1, =0x03
        str     r1, [r0]

        mov     pc, lr

uart_init:
	/*set gpio to uart mode*/
	ldr	r0, =GPACON
	ldr	r1, [r0]
	bic	r1, #0xff
	orr	r1, r1, #0x22
	str	r1, [r0]

	/*set the frame*/
	ldr	r0, =ULCON0
	ldr	r1, =0x03
	str	r1, [r0]

	/*set to interrupt mode and PCLK*/
	ldr	r0, =UCON0
	ldr	r1, =0x805
	str	r1, [r0]

	/*enable FIFO*/
	ldr	r0, =UFCON0
	ldr	r1, =0x01
	str	r1, [r0]

	/*disable ACF*/
	ldr	r0, =UMCON0
	ldr	r1, =0x01
	str	r1, [r0]

	/*boaud rate: 115200*/
	ldr	r0, =UBRDIV0
	ldr	r1, =35
	str	r1, [r0]
	ldr	r0, =UDIVSLOT0
	ldr	r1, =0x01
	str	r1, [r0]
	
	mov	pc, lr


nand_init:
	ldr	r0, =MEM_SYS_CFG
	ldr	r1, [r0]
	bic	r1, r1, #0x2
	str	r1, [r0]

	ldr	r0, =NFCONF
	ldr	r1, [r0]
	ldr	r2, =0x10007770
	bic	r1, r1, r2
	orr	r1, r1, #0x210
	str	r1, [r0]

	/*enable nand controller*/
	ldr	r0, =NFCONT
	ldr	r1, =0x01
	str	r1, [r0]

	/*disable lock*/
	ldr	r1, [r0]
	bic	r1, #0x10000
	str	r1, [r0]

	/*
	 *nand reset
	 *enable chip select
	 */
	ldr	r0, =NFCONT
	ldr	r1, [r0]
	bic	r1, #0x2
	ldr	r1, [r0]

	/*reset*/
	ldr	r0, =NFCMMD
	ldr	r1, =0x0ff
	str	r1, [r0]
	
wait_ready:
	ldr	r0, =NFSTAT
	cmp	r1, #0x01
	bne	wait_ready

disable_chip:
	ldr	r0, =NFCONT
	ldr	r1, [r0]
	orr	r1, r1, #0x2
	str	r1, [r0]

	mov	pc, lr
