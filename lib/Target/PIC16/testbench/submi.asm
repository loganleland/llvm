list p=16f877
__CONFIG 0x3F39


INDF equ 0x00
FSR  equ 0x04
F    equ 0x1
W    equ 0x0
SP   equ 0x32
FP   equ 0x31
GPR10 equ 0x30
GPR9 equ 0x29
GPR8 equ 0x28
GPR7 equ 0x27
GPR6 equ 0x26
GPR5 equ 0x25
GPR4 equ 0x24
GPR3 equ 0x23
GPR2 equ 0x22
GPR1 equ 0x21
GPR0 equ 0x20


org 0x000
nop

init:
  movlw 0x33
  movwf SP
  movwf FP

premain:
  call main
  quicksand: goto quicksand

main:                                   ; @main
; BB#0:                                 ; %entry
	incf	SP, F
	movf	SP, W
	movwf	FSR
	movf	FP, W
	movwf	INDF
	incf	SP, F
	movf	SP, W
	movwf	FP
	decf	FP, F
	decf	FP, F
	movf	SP, W
	addlw	0x2
	movwf	SP
	movf FP,W
	addlw 0x2
	movwf FSR
	movlw 0x0
	movwf INDF
	movf FP,W
	addlw 0x3
	movwf FSR
	movlw 0x2a
	movwf INDF
	movf FP,W
	addlw 0x4
	movwf FSR
	movlw 0x20
	movwf INDF
	movf FP,W
	addlw 0x3
	movwf FSR
	movf INDF,W
	addlw -0x20
	movwf INDF
	movlw 0x0
	movwf GPR4
	incf	FP, W
	movwf	FSR
	movf	INDF, W
	movwf	FP
	decf	SP, F
	decf	SP, F
	movf	SP, W
	addlw	-0x4
	movwf	SP
	return


end
