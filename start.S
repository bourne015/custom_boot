.global _start

_start:
	/*peripheral port setup*/
	ldr	r0, =0x70000000
	orr	r0, r0, #0x13
	mcr	p15, 0, r0, c15, c2, 4

	/*disable watchdog*/
	ldr	r0, =0x7E004000 
	mov	r1, #0x0
	str	r1, [r0]

	/*lowlevel init*/
	ldr	sp, =8*1024
	bl	clock_init
	bl	uart_init
	bl	ddr_init
	
	/*copy code to ram*/
	adr	r0, _start
	ldr	r1, =_start
	ldr	r2, =bss_start
	sub	r2, r2, r1
	cmp	r0, r1
	beq	clean_bss
	
	bl	nand2ddr
#	bl	test
	cmp	r0, #0
	beq	error

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
	ldr	lr, =error
	ldr	pc, =do_bootm_linux

error:
	b	error
