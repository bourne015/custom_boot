#include "s3c6410.h"

.global clock_init
clock_init:
	/*set the clock time*/
	ldr	r0, =APLL_LOCK
	ldr	r1, =0xffff
	str	r1, [r0]

	str	r1, [r0, #0x4]	/*MPLL_CLOCK*/
	str	r1, [r0, #0x8]	/*EPLL_CLOCK*/

	/*set sync mode*/
	ldr	r0, =OTHERS
	ldr	r1, [r0]
	bic	r1, #0xc0
	str	r1, [r0]
	
	/*wait for sync mode*/
loop1:
	ldr	r0, =OTHERS
	ldr	r1, [r0]
	and	r1, #0xf00
	cmp	r1, #0x00
	bne	loop1

	ldr	r0, =CLK_DIV0
	ldr	r1, =DIV0_VAL
	str	r1, [r0]
	
	ldr	r0, =APLL_CON
	ldr	r1, =APLL_VAL
	str	r1, [r0]
	
	ldr	r0, =MPLL_CON
	str	r1, [r0]

	/*set the clk_src to choose pll but not xttpll*/
	ldr	r0, =CLK_SRC
	ldr	r1, =0x03
	str	r1, [r0]
	
	mov	pc, lr
