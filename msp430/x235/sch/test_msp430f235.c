#include <msp430.h>
#include <usmartx.h>


static void 
__interrupt(TIMERB0_VECTOR) tmrb_isr(void){
  if((TBIV & TBIV_TBIFG) == TBIV_TBIFG)
    uSMARTX_Tick();
}

TSK_CREATE(led1_tcb);
TSK_CREATE(led2_tcb);

STATUS led1_handler(STATUS ucEvent){
  P1OUT ^= 0xAA;
  
  TSK_Sleep(TSK_Self(),100);
  return SYS_OK;
}

STATUS led2_handler(STATUS ucEvent){
  P1OUT ^= 0x55;

  TSK_Sleep(TSK_Self(),100);
  return SYS_OK;
}

static task_entry_t tbl_usmartask[]={
  {&led1_handler,&led1_tcb,1,"LED1_TSK"},
  {&led2_handler,&led2_tcb,1,"LED2_TSK"},
  {0,0,0,""}};
  


void setup_timerb(void){
  TBCTL = 0; // stop timer b
  TBCCR0  = 0x04;
  TBCCTL0 = CAP; // disable capture mode
  TBCCTL1 = CAP; // disable capture mode
  TBCCTL2 = CAP; // disable capture mode
  //TimerB 8bit     ACLK       /8     upto CCR0  
  TBCTL =  CNTL_3 + TBSSEL_1 + ID_3 + MC_1     + TBIE;
}

void setup_gpio(void){
  P1DIR = 0xFF; // all is output
  P1REN = 0; // disable pull up/down R
  P1SEL = 0; // use as I/O pin
  P1OUT = 0; // all at low level
}

void setup_clk_src(void){
  BCSCTL1 = CAL_BC1_16MHZ;
  BCSCTL2 = 0;
  DCOCTL = CAL_DCO_16MHZ;  
}

int main(void){

  WDTCTL = WDTPW + WDTHOLD; // stop watchdog

  setup_clk_src();
  setup_gpio();
  setup_timerb();

  uSMARTX_Init(tbl_usmartask);

  INT_Enable();

  while(1){
    if(uSMARTX_Scheduler()==SYS_ERROR)
      break;
  }
  
  return 0;
}
