#include "lpc2114.h"
#include "lpc2k_bsp.h"
void lpc2k_init(void)
{
	MAM_MAMMAP = 1; // interrupt vectors retain at flash area
	PCB_PINSEL0 = 5; // set Port 0.0 & 0.1 to Uart TX & RX
	lpc2k_uart_init();
	lpc2k_vic_init();
	lpc2k_timer_init();
}

