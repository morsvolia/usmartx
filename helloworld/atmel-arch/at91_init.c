/*   
 *  at91_init.h  
 *  this file defines init process on AT91X40 
 *  for Atmel AT91 initialization, includes AIC, TC, USART ...
 *
 *  Bugs report:     li ming  ( lmcs00@mails.tsinghua.edu.cn )
 *  Last modified:   2003-02-02 
 *
 */
 
#include "at91_init.h"

#include "skyeye_stdio.h"

//yangye 2003-2-14
//add svr[32] to save ISR entrys
unsigned long svr[32];
#ifndef NULL
#define NULL (void *)0
#endif

void do_irq( void );

void at91_mask_irq(unsigned int irq)
{
	unsigned long mask = 1 << (irq);
	__arch_putl(mask, AIC_IDCR);		// AIC Interrupt Disable Command Register
}	

void at91_unmask_irq(unsigned int irq)
{
	unsigned long mask = 1 << (irq);
	__arch_putl(mask, AIC_IECR);		// AIC Interrupt Enable Command Register
}

void at91_mask_ack_irq(unsigned int irq)
{
	at91_mask_irq(irq);
        __arch_putl(0, AIC_EOICR);     /* value=don't care */
}

void at91_init()
{
	// here Must put at91_init_usart() at the very beginning of init !
	at91_init_usart();
	at91_init_aic();	
	at91_init_timer();
}

//yangye
//use this function to install your OSISR as the IRQ mode handler
//and your OSISR must call do_irq to deal with different irqs
void install_irqhandler(void * isr)
{
	/* ARM irq exception vector addr is 0x00000018  */
	unsigned int * irq_vec_addr = ( unsigned int * ) 0x18;
	/* this is isr entry address, could be another address like 0x3c, 0x58... */
	unsigned int * isr_entry_addr = ( unsigned int * ) 0x38;

	unsigned int instruction;
	
	/* set the ISR entry at 0x38  */
	*isr_entry_addr = (unsigned int)isr;
	
	/* make an instruction: it is machine-code for "ldr  pc, [pc, #(38-18-8)]"  */
	instruction = ((unsigned int) isr_entry_addr  - (unsigned int)irq_vec_addr - 0x08) | 0xe59ff000;
	
	/* set this instruction at 0x18  */
	*irq_vec_addr = instruction;	
}

void do_irq(void)
{
	int irq_num;
	void (*isr)(void);
	//chy 2003-02-16 : IVR could have some irqs at the same time, so should check it untile IVR=0 	
	//irqnum is the highest irq number 
label_again:
	irq_num = __arch_getl(AIC_IVR);
	

	if(irq_num!=0){
		//close this interrupt
		at91_mask_irq(irq_num);
		
		isr = (void *)svr[irq_num];
		if( isr != 0 )
			isr();

		if(irq_num == KERNEL_TIMER_IRQ_NUM){
			//do some extra work for skyeye timer interrupt
			register volatile struct at91_timers* tt = (struct at91_timers*) (AT91_TC_BASE);
			register volatile struct at91_timer_channel* tc = &tt->chans[KERNEL_TIMER].ch;
			/* clear TC Status Register to continue interrupt */
			unsigned long tmp = tc->sr; // only read status register, must do!
			tmp = tmp;			// just use it to avoid compiler warnings!
		}
		//reenable this interrupt
		at91_unmask_irq(irq_num);
		 /* clear AIC : indicate the end of interrupts */
		__arch_putl(irq_num,AIC_EOICR);  // only write AIC_EOICR
	        //chy 2003-02-16 : IVR could have some irqs at the same time, so should check it untile IVR=0 	
	        goto label_again;
	   }
}


int request_irq(int irq_num, void * usr_isr)
{
	if(irq_num>0 && irq_num < 32){
		svr[irq_num] = (unsigned long)usr_isr;
		return 1;
	}else{
		//wrong irq_num
		return -1;
	}
}
