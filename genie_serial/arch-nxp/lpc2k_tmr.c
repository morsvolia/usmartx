#include "lpc2k_bsp.h"

struct lpc2k_tmr *lpc2k_tc0=(struct lpc2k_tmr *)0xE0004000;
struct lpc2k_tmr *lpc2k_tc1=(struct lpc2k_Tmr *)0xE0008000;

extern void tmr0_isr(void);

void lpc2k_timer_init(void)
{
 lpc2k_tc0->Prescaler = 1;
 lpc2k_tc0->MatchReg[0] = 30;
 lpc2k_tc0->MatchCtrlReg = _BV(MR0IE)| _BV(MR0RST);
 IRQ_hookup_slot(0,tmr0_isr,TIMER0);
 lpc2k_tc0->TmrCtrlReg = _BV(TCEN);
}

