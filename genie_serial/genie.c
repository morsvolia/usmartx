#include "usmartx.h"
#include "semaphore.h"
#include "lpc2k_bsp.h"
#include "circbuffer.h"

extern struct circBuffer circRxBuf,circTxBuf;

SEM_CREATE(semLpc2k_uart,1,1);

TSK_CREATE(tskUart_in);

STATUS tskTxTest(STATUS event)
{

    if(circTxBuf.len == 0)
        circBuffer_put(&circTxBuf,'A');
    
    TSK_Sleep(TSK_Self(),300);
    return SYS_OK;
}

task_entry_t xtask_Table[]={{&tskTxTest,&tskUart_in,1},
			    {0,0,0}};
int main(void)
{
	lpc2k_init();
	
	uSMARTX_Init( xtask_Table);
	
	INT_Enable();

	while(1)
	{
	    if(uSMARTX_Scheduler() == SYS_ERROR)
		break;
	}

	return 0; // Code never reach
}

void __attribute__ ((interrupt("IRQ"))) tmr0_isr(void)
{
 	if((lpc2k_tc0->IntrReg & _BV(MR0IF)) > 0)
	{
  		uSMARTX_Tick();
		lpc2k_tc0_clear_ir(MR0IF);
		lpc2k_vintr->VectorAddr = NULL;
	}
	
}

void __attribute__((interrupt("IRQ"))) uart0_isr(void)
{
 int cntr;
	if((lpc2k_usart0->LineStatusReg & _BV(RECVRDY)) > 0)
	{
	 // read in 8 byte data
	 
	 lpc2k_usart0->LineCtrlReg &= ~_BV(DLAB);
	 for(cntr=0; cntr<8; cntr++)
	 {
		circBuffer_put(&circRxBuf,lpc2k_usart0->RecvBufReg);
	 }
	}
	if((lpc2k_usart0->LineStatusReg & _BV(THREMP)) > 0)
	{
	 // send out 16 byte data
	 lpc2k_usart0->LineCtrlReg &= ~_BV(DLAB);
	 cntr=16;
	 while((circTxBuf.len > 0) && (cntr-- > 0))
	 {
		lpc2k_usart0->XmitHoldReg = circBuffer_get(&circTxBuf);
	 }

	}
}
