#include "lpc2k_bsp.h"
#define IRQ_BIT 0x00000080
#define FIQ_BIT 0x00000040

struct lpc2k_vector *lpc2k_vintr=(struct lpc2k_vector *)0xFFFFF000;
void lpc2k_vic_init(void)
{
 unsigned char slotno;
 lpc2k_vintr->IntrEnClear = _BV(WDT) |_BV(ARMCORE0) | _BV(ARMCORE1) | _BV(TIMER0) | \
 			  _BV(TIMER1) | _BV(UART0) | _BV(UART1) | _BV(PWM0) | \
			  _BV(I2C) | _BV(SPI0) | _BV(SPI1) | _BV(PLL) | _BV(RTC) | \
			  _BV(EINT0) | _BV(EINT1) | _BV(EINT2) | _BV(EINT3) | _BV(ADC) | \
			  _BV(CANCOMMON) | _BV(CAN1TX) | _BV(CAN2TX) | _BV(CAN3TX) | _BV(CAN4TX) | \
			  _BV(CAN1RX) | _BV(CAN2RX) | _BV(CAN3RX) | _BV(CAN4RX);

 lpc2k_vintr->SoftIntrClear = _BV(WDT) |_BV(ARMCORE0) | _BV(ARMCORE1) | _BV(TIMER0) | \
 			  _BV(TIMER1) | _BV(UART0) | _BV(UART1) | _BV(PWM0) | \
			  _BV(I2C) | _BV(SPI0) | _BV(SPI1) | _BV(PLL) | _BV(RTC) | \
			  _BV(EINT0) | _BV(EINT1) | _BV(EINT2) | _BV(EINT3) | _BV(ADC) | \
			  _BV(CANCOMMON) | _BV(CAN1TX) | _BV(CAN2TX) | _BV(CAN3TX) | _BV(CAN4TX) | \
			  _BV(CAN1RX) | _BV(CAN2RX) | _BV(CAN3RX) | _BV(CAN4RX);
 for(slotno=0;slotno < 16;slotno++)
 	lpc2k_vintr->VectorCtrl[slotno]=0;
}
/* hookup interrupt to IRQ slot */
void IRQ_hookup_slot(int slotnum, void (*funcptr)(void),enum VICBV intr)
{
 slotnum &= 0xF;
 lpc2k_vintr->VectorAddrSlot[slotnum]=funcptr;
 lpc2k_vintr->VectorCtrl[slotnum]=(uint32t)(intr | _BV(VICVectorCtrlIRQSlotEn));
 lpc2k_vintr->IntrEnable |= _BV(intr);
}

void disableAllIntr(void)
{
 unsigned long retval;
	asm volatile("mrs %0,cpsr" :"=r"(retval):);
	retval |= (IRQ_BIT | FIQ_BIT); // stop both IRQ and FIQ
	asm volatile("msr cpsr,%0" ::"r"(retval));
}

void enableAllIntr(void)
{
 unsigned long retval;
	asm volatile("mrs %0,cpsr" :"=r"(retval):);
	retval &= ~(IRQ_BIT | FIQ_BIT); // enable both IRQ and FIQ
	asm volatile("msr cpsr,%0" ::"r"(retval));
}


void disableVectorIntr(void)
{
 unsigned long retval;
	asm volatile("mrs %0,cpsr" :"=r"(retval):);
	retval |= IRQ_BIT; // stop IRQ only
	asm volatile("msr cpsr,%0" ::"r"(retval));
}

void enableVectorIntr(void)
{
 unsigned long retval;
	asm volatile("mrs %0,cpsr" :"=r"(retval):);
	retval &= ~(IRQ_BIT); // enable IRQ only
	asm volatile("msr cpsr,%0" ::"r"(retval));
}
