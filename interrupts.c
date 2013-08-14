#include "s3c6410.h"

void enable_interrupts(void)
{
	unsigned long temp;
	__asm__ __volatile__("mrs %0, cpsr\n"
			     "bic %0, %0, #0x80\n"
			     "msr cpsr_c, %0"
			     : "=r" (temp)
			     :
			     : "memory");
}

void do_irq(void)
{
	void (*irq_isr)(void);

	if (VIC0IRQSTATUS) {
		irq_isr = VIC0ADDRESS;
		irq_isr();
		
		VIC0ADDRESS = 0;
	} else if (VIC1IRQSTATUS) {
		irq_isr = VIC1ADDRESS;
		irq_isr();

		VIC0ADDRESS = 0;
	}
}
