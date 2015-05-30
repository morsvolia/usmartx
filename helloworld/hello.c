#include "skyeye-stdio/skyeye_stdio.h"
#include "usmartx.h"

TSK_CREATE(TSK_UART);

STATUS TSK_Uart_handler(STATUS ucEvent)
{
	skyeye_putc('%');
	skyeye_puts("In other world...");

	TSK_Sleep(TSK_Self(),200);
	return SYS_OK;
}

task_entry_t usmartask_table[]={{&TSK_Uart_handler,&TSK_UART,1},{0,0,0}};

int main(void)
{
	
	
	uSMARTX_Init(usmartask_table);
	INT_Enable();

	while(1)
	{
		if(uSMARTX_Scheduler() == SYS_ERROR)
			break;
	}

	return 0;
}
