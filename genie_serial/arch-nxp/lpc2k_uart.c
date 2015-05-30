#include "lpc2k_bsp.h"
#include "circbuffer.h"

extern void uart0_isr(void);

struct lpc2k_uart *lpc2k_usart0=(struct lpc2k_uart *)0xE000C000;
struct circBuffer circRxBuf,circTxBuf;


void lpc2k_uart_init(void)
{
 unsigned char tmp;

	circBuffer_init(&circTxBuf);
	circBuffer_init(&circRxBuf);

	lpc2k_usart0->IntrEn=0;	// disable all uart interrupt
	tmp=lpc2k_usart0->IntrIdReg; 
	tmp=lpc2k_usart0->RecvBufReg;
	tmp=lpc2k_usart0->LineStatusReg;

	lpc2k_usart0->LineCtrlReg = UART_PROTOCOL_8N1; // select line protocol 8N1
	lpc2k_usart0->FifoCtrlReg = _BV(FIFOEN)| _BV(RXRST) |_BV(TXRST) | RXINTR_TRIGGER8; // select 8 byte rx triggerable fifo & reset rx,tx fifo
	// setup band rate for usart0
	
	// setup tx empty & rx data available interrupts
	IRQ_hookup_slot(1,uart0_isr,UART0);

	lpc2k_usart0->IntrEn = _BV(RBRIE) | _BV(THRIE);
}


void lpc2k_uart_get_char(char *ch)
{
 char tmp;
  if(circRxBuf.len > 0)
  {
	tmp = circBuffer_get(&circRxBuf);
	*ch = tmp;
  }
  else
    *ch ='\0';
}
void lpc2k_uart_put_char(char ch)
{
	circBuffer_put(&circTxBuf,ch);

}

void lpc2k_uart_send_string(char *strz,int len)
{
 int cntr;
 char *ptr;
	 cntr=len;
	 ptr=strz;
 
	 while(cntr-- > 0)
    	{
 	 	circBuffer_put(&circTxBuf,*ptr);
	 	ptr++;
    	}
}

