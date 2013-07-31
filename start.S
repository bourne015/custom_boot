.global _start

_start:
	/*set the CPU to SVC32 mode*/
	mrs	r0, cpsr
	bic	r0, r0, #0x3f
	orr	r0, r0, #0xd3
	msr	cpsr, r0

	/*peripheral port setup*/
	ldr	r0, =0x70000000
	orr	r0, r0, #0x13
	mcr	p15, 0, r0, c15, c2, 4

	/*lowlevel: clock, uart, nand*/
	bl	lowlevel_init
	ldr	sp, =8*1024
#	bl	led_run
	bl	ddr_init
	
	/*copy code to ram*/
	adr	r0, _start
	ldr	r1, =_start
	ldr	r2, =bss_start
	sub	r2, r2, r1
	cmp	r0, r1
	beq	clean_bss
	
#	bl	nand_read
	bl	led_run
#	cmp	r0, #0
#	beq	error

clean_bss:
	ldr	r0, =bss_start
	ldr	r1, =bss_end
	ldr	r2, =0x0
	cmp	r0, r1
	beq	after_copy
bss_loop:
	str	r1, [r0], #4
	cmp	r0, r1
	bne	bss_loop

after_copy:
#	ldr	lr, =error
	ldr	pc, =start_armboot

###error:
#	b	error
