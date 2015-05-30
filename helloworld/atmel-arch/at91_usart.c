/*   
 *  at91_usart.c
 *  this file implement usart driver on AT91X40 
 *  for Atmel AT91 serial port comunication
 *
 *  Bugs report:  li ming  ( lmcs00@mails.tsinghua.edu.cn )
 *
 */
#include "at91.h"
#include "at91_usart.h"
#include "at91_init.h"

static volatile struct atmel_usart_regs *usarts[AT91_USART_CNT] = {
	(volatile struct atmel_usart_regs *) AT91_USART0_BASE,
	(volatile struct atmel_usart_regs *) AT91_USART1_BASE
};

static unsigned long calcCD(unsigned long br)
{
	return (UART_CLOCK / br);
}

static void uart_init(volatile struct atmel_usart_regs *uart)
{
	/* Reset the USART */
	uart->cr = US_TXDIS | US_RXDIS | US_RSTTX | US_RSTRX;
	/* clear Rx receive and Tx sent counters */
	uart->rcr = 0;
	uart->tcr = 0;
	
	/* Set the serial port into a safe sane state */
	uart->mr = US_USCLKS(0) | US_CLK0 | US_CHMODE(0) | US_NBSTOP(0) |
		    US_PAR(4) | US_CHRL(3);

	uart->brgr = calcCD(9600);

	uart->rtor = 20;			// timeout = value * 4 *bit period
	uart->ttgr = 0;				// no guard time
	uart->rcr = 0;
	uart->rpr = 0;
	uart->tcr = 0;
	uart->tpr = 0;
}

static void _INLINE_ tx_enable(volatile struct atmel_usart_regs *uart)
{
	uart->ier = US_TXEMPTY;
}

static void _INLINE_ rx_enable(volatile struct atmel_usart_regs *uart)
{
	uart->ier = US_ENDRX | US_TIMEOUT;
}

static void _INLINE_ tx_start(volatile struct atmel_usart_regs *uart, int ints)
{
	if (ints) {
		tx_enable(uart);
	}
	uart->cr = US_TXEN;
}

static void _INLINE_ rx_start(volatile struct atmel_usart_regs *uart, int ints)
{
	uart->cr = US_RXEN | US_STTO;
	uart->rtor = 20;
	if (ints) {
		rx_enable(uart);
	}
}

static void _INLINE_ rx_disable(volatile struct atmel_usart_regs *uart)
{
	uart->idr = US_ENDRX | US_TIMEOUT;
}

static void _INLINE_ rx_stop(volatile struct atmel_usart_regs *uart)
{
	rx_disable(uart);
	uart->rtor = 0;
	uart->rcr = 0;
	uart->cr = US_RXDIS;
}

static void start_rx(volatile struct atmel_usart_regs *uart, char *rx_buf, int len )
{
	rx_stop(uart);

 	uart->rpr = (unsigned long) rx_buf;
 	uart->rcr = (unsigned long) len;
 	rx_start(uart, 1);
}

static void xmit_string(volatile struct atmel_usart_regs *uart, char *p, int len)
{
	uart->tcr = 0;
	uart->tpr = (unsigned long) p;
	uart->tcr = (unsigned long) len;
	tx_start(uart, 1);
}

static void xmit_char(volatile struct atmel_usart_regs *uart, char ch)
{
	xmit_string(uart, &ch, 1);
}

void at91_init_usart()
{
	uart_init( usarts[0] );	
}

void at91_uart_send_string( char *p, int len )
{
	xmit_string( usarts[0], p, len);
}

void at91_uart_put_char( char ch )
{
	xmit_char( usarts[0], ch );
}

static void _io_read( long * paddr, long * pvalue )
{
	*pvalue = *paddr;
	return;	
}

void at91_uart_get_char( char *ch )
{
	unsigned long status;

	long tmp;
	start_rx( usarts[0], ch, 1 );
	do
	{
		// must read 0xfffff100, status register to clear all flags !!!
		_io_read( (long*)0xfffff100, &tmp );
		_io_read( (long*)0xfffff108, &tmp );
		status = usarts[0]->rcr;
	}
	while (status > 0);
}