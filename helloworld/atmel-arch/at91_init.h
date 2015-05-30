/*   
 *  at91_init.h  
 *  this file defines init process on AT91X40 
 *  for Atmel AT91 initialization, includes AIC, TC, USART ...
 *
 *  Bugs report:     li ming  ( lmcs00@mails.tsinghua.edu.cn )
 *  Last modified:   2003-02-02 
 *
 */

#ifndef __AT91_INIT_H
#define __AT91_INIT_H

/* always include this file for at91 peripherals */
#include "at91.h"


#include "at91_aic.h"
#include "at91_tc.h"
#include "at91_usart.h"


void at91_init( void );
void install_irqhandler(void * user_isr);

//yangye 2003-2-14
int request_irq(int irq_num, void * usr_isr);

void at91_mask_irq(unsigned int irq);
void at91_unmask_irq(unsigned int irq);
void at91_mask_ack_irq(unsigned int irq);


#endif __AT91_INIT_H
