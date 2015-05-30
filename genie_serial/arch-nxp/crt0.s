        .global main                    // int main(void)

        .global _stack                  // top of stack

// Stack Sizes
        .set  UND_STACK_SIZE, 0x00000004
        .set  ABT_STACK_SIZE, 0x00000004
        .set  FIQ_STACK_SIZE, 0x00000004
        .set  IRQ_STACK_SIZE, 0X00000080
        .set  SVC_STACK_SIZE, 0x00000004

// Standard definitions of Mode bits and Interrupt (I & F) flags in PSRs
        .set  MODE_USR, 0x10            // User Mode
        .set  MODE_FIQ, 0x11            // FIQ Mode
        .set  MODE_IRQ, 0x12            // IRQ Mode
        .set  MODE_SVC, 0x13            // Supervisor Mode
        .set  MODE_ABT, 0x17            // Abort Mode
        .set  MODE_UND, 0x1B            // Undefined Mode
        .set  MODE_SYS, 0x1F            // System Mode

        .equ  I_BIT, 0x80               // when I bit is set, IRQ is disabled
        .equ  F_BIT, 0x40               // when F bit is set, FIQ is disabled

        .text
        .code 32
        .align 2

        .global startup
        .func   startup
startup:

// Runtime Interrupt Vectors
// -------------------------
        b     reset                    // reset - _start
        ldr   pc,_undf                  // undefined - _undf
        ldr   pc,_swi                   // SWI - _swi
        ldr   pc,_pabt                  // program abort - _pabt
        ldr   pc,_dabt                  // data abort - _dabt
        nop                             // reserved
        ldr   pc,[pc,#-0xFF0]           // IRQ - read the VIC
        ldr   pc,_fiq                   // FIQ - _fiq

// Use this group for development
_undf:  .word __undf                    // undefined
_swi:   .word __swi                     // SWI
_pabt:  .word __pabt                    // program abort
_dabt:  .word __dabt                    // data abort
_irq:   .word __irq                     // IRQ
_fiq:   .word __fiq                     // FIQ

__undf: b     .                         // undefined
__swi:  b     .                         // SWI
__pabt: b     .                         // program abort
__dabt: b     .                         // data abort
__irq:  b     .                         // IRQ
__fiq:  b     .                         // FIQ
        .size startup, . - startup
        .endfunc



// Setup the operating mode & stack.
// ---------------------------------
        .global reset
        .func   reset
reset:

// Initialize Interrupt System
// - Set stack location for each mode
// - Leave in System Mode with Interrupts Disabled
// -----------------------------------------------
        ldr   r0,=_stack
        msr   CPSR_c,#MODE_UND|I_BIT|F_BIT // Undefined Instruction Mode
        mov   sp,r0
        sub   r0,r0,#UND_STACK_SIZE
        msr   CPSR_c,#MODE_ABT|I_BIT|F_BIT // Abort Mode
        mov   sp,r0
        sub   r0,r0,#ABT_STACK_SIZE
        msr   CPSR_c,#MODE_FIQ|I_BIT|F_BIT // FIQ Mode
        mov   sp,r0
        sub   r0,r0,#FIQ_STACK_SIZE
        msr   CPSR_c,#MODE_IRQ|I_BIT|F_BIT // IRQ Mode
        mov   sp,r0
        sub   r0,r0,#IRQ_STACK_SIZE
        msr   CPSR_c,#MODE_SVC|I_BIT|F_BIT // Supervisor Mode
        mov   sp,r0
        sub   r0,r0,#SVC_STACK_SIZE
        msr   CPSR_c,#MODE_SYS|I_BIT|F_BIT // System Mode
        mov   sp,r0


// Call main program: main(0)
// --------------------------
        mov   r0,#0                     // no arguments (argc = 0)
        mov   r1,r0
        mov   r2,r0
        mov   fp,r0                     // null frame pointer
        mov   r7,r0                     // null frame pointer for thumb
        ldr   r10,=main
        mov   lr,pc
        bx    r10                       // enter main()

/* "global object"-dtors are never called and it should not be 
   needed since there is no OS to exit to. */
   
        .size   reset, . - reset
        .endfunc

        .end
