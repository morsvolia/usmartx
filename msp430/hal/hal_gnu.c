#include <in430.h>

__inline__ unsigned int INT_Disable(void){
  unsigned int imask;
  __asm__ __volatile__("mov r2,%0": "=r" ((unsigned int)imask) :);
  _disable_interrupts();
  return (imask & (unsigned int)0x0008);
}

__inline__ void INT_Enable(void){
  _enable_interrupts();
}

__inline__ void INT_Restore(unsigned int imask){
  if(imask == 0)
    _enable_interrupts();
  else
    _disable_interrupts();
}
