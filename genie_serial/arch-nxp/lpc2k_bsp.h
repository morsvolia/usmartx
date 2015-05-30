#ifndef _LPC2K_H
#define _LPC2K_H

#define __arch_putb(v,a) (*(volatile unsigned char *)(v)=(a))
#define __arch_putl(v,a) (*(volatile unsigned int *)(v)=(a))
#define __arch_getb(v)	(*(volatile unsigned char *)(v))
#define __arch_getl(v)	(*(volatile unsigned int *)(v))

#ifndef NULL
#define NULL ((void *)0)
#endif

typedef volatile unsigned char uint8t;
typedef volatile unsigned short uint16t;
typedef volatile unsigned int uint32t;

#ifndef _BV
#define _BV(n) (1 << (n))
#endif

/* arch. depend functions */

struct lpc2k_vector{
	uint32t	IRQstatus;	
	uint32t	FIQstatus;
	uint32t	RAWIntr;
	uint32t	IntrSelect;	// select which interrupt classify as IRQ or FIQ
	uint32t	IntrEnable;	// enable classified IRQ or FIQ interrupt,RW
	uint32t	IntrEnClear;	// clear classified IRQ or FIQ interrupt, write-only
	uint32t	SoftIntr;
	uint32t	SoftIntrClear;
	uint32t	ProtectEnable;
	void (*VectorAddr)(void);
	void (*NonVectorAddr)(void);
	void (*VectorAddrSlot[16])(void);
	uint32t	VectorCtrl[16];
};
// BV constant for vic
enum VICBV{WDT=0, ARMCORE0=2, ARMCORE1, TIMER0, TIMER1, UART0, UART1,
	   PWM0, I2C, SPI0, SPI1, SSP=11, PLL, RTC, EINT0, EINT1, EINT2, EINT3,
	   ADC, CANCOMMON, CAN1TX, CAN2TX, CAN3TX, CAN4TX, CAN1RX=26, CAN2RX, CAN3RX, CAN4RX};
#define VICVectorCtrlIRQSlotEn 5
/* to assign interrupt source to vector slot , equal VectorCtrl[n] to one of VICBV ORed with _BV(VICVectorCtrlIRQSlotEn) */

extern struct lpc2k_vector *lpc2k_vintr;
void lpc2k_vic_init(void);
// Both IRQ & FIQ operation
void disableAllIntr(void);
void enableAllIntr(void);
// Only IRQ operation
void disableVectorIntr(void);
void enableVectorIntr(void);
void IRQ_hookup_slot(int slotnum,void (*funcptr)(void),enum VICBV intr);

struct lpc2k_tmr{
    uint32t	IntrReg;
    uint32t	TmrCtrlReg;
    uint32t	TmrCntr;
    uint32t	Prescaler;
    uint32t	PrescalerCntr;
    uint32t	MatchCtrlReg;
    uint32t	MatchReg[4];
    uint32t	CaptureCtrlReg;
    uint32t	CaptureReg[4];	// read only others is read-write capable
    uint32t	ExtMatchReg;
};
// BV constant for timer struct
enum tmrIR{ MR0IF, MR1IF, MR2IF, MR3IF, CR0IF, CR1IF, CR2IF, CR3IF};
enum tmrTCR{TCEN, TCRST};
enum tmrMCR{MR0IE, MR0RST, MR0STOP, 
	 MR1IE, MR1RST, MR1STOP, 
	 MR2IE, MR2RST, MR2STOP,
	 MR3IE, MR3RST, MR3STOP};
enum tmrCCR{CCAP_0RSNG, CCAP_0FLNG, CCAP_0IE,
	 CCAP_1RSNG, CCAP_1FLNG, CCAP_1IE,
	 CCAP_2RSNG, CCAP_2FLNG, CCAP_2IE,
	 CCAP_3RSNG, CCAP_3FLNG, CCAP_3IE};
enum tmrEMR{EM0EN, EM1EN, EM2EN, EM3EN, EM0CR=4, EM1CR=6, EM2CR=8, EM3CR=10};
#define EMCR_NONE 0
#define EMCR_LO 1 // select external match pin to output direction first
#define EMCR_HI 2
#define EMCR_TOGGLE 3
/* set output to external match pin, use ExtMatchReg to assign _BV(EMnEN) | (EMCR_x << EMnCR) */

extern struct lpc2k_tmr *lpc2k_tc0;
extern struct lpc2k_tmr *lpc2k_tc1;
static inline void lpc2k_tc0_clear_ir(enum tmrIR flag)
{
 lpc2k_tc0->IntrReg |=(uint32t) _BV(flag);
}
static inline void lpc2k_tc1_clear_ir(enum tmrIR flag)
{
 lpc2k_tc1->IntrReg |=(uint32t) _BV(flag);
}
void lpc2k_timer_init(void);

struct lpc2k_uart{
	union{
		uint8t RecvBufReg; // readonly
		uint8t XmitHoldReg; //writeonly
		uint8t DivsorLatchLsb;
	};
	uint8t _pad0[3];
	union{
		uint8t IntrEn;
		uint8t DivsorLatchMsb;
	};
	uint8t _pad1[3];
	union{
		uint8t IntrIdReg; //readonly
		uint8t FifoCtrlReg; // writeonly
	};
	uint8t _pad2[3];
	uint8t LineCtrlReg;
	uint8t _pad3[3];
	uint8t ModemCtrlReg; // only for UART1
	uint8t _pad4[3];
	uint8t LineStatusReg;//readonly
	uint8t _pad5[3];
	uint8t ModemStatusReg; //only for UART1
	uint8t _pad6[3];
	uint32t _pad7;
};
enum uartIE{RBRIE, THRIE, RXLNSTIE,MODEMSTIE,CTSIE=7};
enum uartFCR{FIFOEN,RXRST,TXRST};
#define RXINTR_TRIGGER1 0
#define RXINTR_TRIGGER4 (1 << 6)
#define RXINTR_TRIGGER8 (2 << 6)
#define RXINTR_TRIGGER14 (3 << 6)

enum uartIIRNIBBLE{IDMODEMSTATUS=0,IDNONE=1,IDTHRE=2,IDRXDATAVA=4,IDLINERR=6,IDCHARTIMOUT=12};

enum uartLCR{WORDLEN0,WORDLEN1,STOPBIT0,STOPBIT1,PAREN,PARSEL0,PARSEL1,BREAK,DLAB};
enum uartLSR{RECVRDY,OVRRUNERR,PARERR,FRAMERR,BREAKINTR,THREMP,TXEMP,RXFIROERR};
enum uartMCR{DTRCTRL,RTSCTRL,LOOPBACK=4,RTSEN=6,CTSEN=7};
enum uartMSR{DELTACTS,DELTADSR,EDGERI,DELTADCD,CTSSTATE,DSRSTATE,RISTATE,DCDSTATE};

#define UART_PROTOCOL_8N1 _BV(WORDLEN0) | _BV(WORDLEN1)
#define UART_PROTOCOL_7N1 _BV(WORDLEN1)
#define UART_PROTOCOL_8N2 _BV(WORDLEN0) | _BV(WORDLEN1) | _BV(STOPBIT1)
#define UART_PROTOCOL_7N2 _BV(WORDLEN1) | _BV(STOPBIT1)
#define UART_PROTOCOL_8E1 _BV(WORDLEN0) | _BV(WORDLEN1) | _BV(PAREN) | _BV(PARSEL0)
#define UART_PROTOCOL_8O1 _BV(WORDLEN0) | _BV(WORLDLEN1) | _BV(PAREN)

extern struct lpc2k_uart *lpc2k_usart0;
void lpc2k_uart_init(void);
void lpc2k_uart_get_char(char *ch);
void lpc2k_uart_put_char(char ch);
void lpc2k_uart_send_string(char *strz,int len);

void lpc2k_init(void);

#endif
