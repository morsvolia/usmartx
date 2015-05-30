
#define STATUS_REG R2
#define GIE 8

	.global INT_Disable
	.func	INT_Disable
INT_Disable:
	;BIC.W	#GIE,STATUS_REG
	mov.w		STATUS_REG,R15
INT_Disable_real:
	DINT
	nop
	RET
	.endfunc

	.global	INT_Enable
	.func	INT_Enable
INT_Enable:
	;BIS.W	#GIE,STATUS_REG
	EINT
	nop
	RET
	.endfunc

	.global	INT_Restore
	.func	INT_Restore
INT_Restore:
	and.w	#GIE,R15
	jz	INT_Disable_real
	jmp	INT_Enable
	.endfunc
