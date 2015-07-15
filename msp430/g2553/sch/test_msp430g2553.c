#include <msp430.h>
#include <usmartx.h>


#pragma vector=TIMER1_A0_VECTOR
__interrupt void tmra_isr(void){
    uSMARTX_Tick();
}

TSK_CREATE(led1_tcb);
TSK_CREATE(led2_tcb);

STATUS led1_handler(STATUS ucEvent){
  P1OUT ^= BIT0;
  
  TSK_Sleep(TSK_Self(),100);
  return SYS_OK;
}

STATUS led2_handler(STATUS ucEvent){
  P1OUT ^= BIT6;

  TSK_Sleep(TSK_Self(),100);
  return SYS_OK;
}

static task_entry_t tbl_usmartask[]={
  {&led1_handler,&led1_tcb,1,"LED1_TSK"},
  {&led2_handler,&led2_tcb,1,"LED2_TSK"},
  {0,0,0,""}};
  


void setup_timer1(void){
  TA1CTL = 0; // stop timer a1
  TA1CCR0  = 0x04;
  TA1CCTL0 = CCIE; // disable capture mode
  TA1CCTL1 = CAP; // disable capture mode
  TA1CCTL2 = CAP; // disable capture mode
  //TimerA1 ACLK       /8     upto CCR0  
  TA1CTL =  TASSEL_1 + ID_3 + MC_1;
}

void setup_gpio(void){
  P1DIR = 0xFF; // all is output
  P1REN = 0; // disable pull up/down R
  P1SEL = 0; // use as I/O pin
  P1OUT = 0; // all at low level
}

void setup_clk_src(void){
  BCSCTL1 = CALBC1_16MHZ;
  BCSCTL2 = 0;
  DCOCTL = CALDCO_16MHZ;  
}

int main(void){

  WDTCTL = WDTPW + WDTHOLD; // stop watchdog

  setup_clk_src();
  setup_gpio();
  setup_timer1();

  uSMARTX_Init(tbl_usmartask);

  INT_Enable();

  while(1){
    if(uSMARTX_Scheduler()==SYS_ERROR)
      break;
  }
  
  return 0;
}
