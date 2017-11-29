; This is where your program starts in RAM.

	org 8000h

runTimeStart equ 8100h								; must be odd page so core on even page

	org 		runTimeStart + 40h
	di 	
;
;	Set up registers.
;	
	ld 			bc,runTimeStart + 040h 				; BC is predecrementing stack for return addresses
	ld 			sp,runTimeStart + 0FEh 				; SP is the data stack.
	ld 			hl,0FFFFh
	push 		hl
	push 		hl
	ld 			de,testCode 						; DE is the instruction pointer.
	ld 			ix,mainInterpreterLoop				; IX is used to jump to next instruction
;
;	Main Loop.
;	
mainInterpreterLoop:
	ld 			a,(de)								; get next code.
	inc 		de
	add 		a,a 								; shift left into L
	ld 			l,a
	jr 			c,callRoutine						; if bit 7 was set it's a call address.
	ld 			h,hardCodeStart/256 				; set up the H register
	jp 			(hl)								; otherwise go there.
;
;	Call a routine. L already contains byte 0 << 1
;
callRoutine:
	ld 			a,(de) 								; get high byte into H
	inc 		de
	ld 			h,a 
	ld 			a,d 								; push DE on return stack
	dec 		bc
	ld 			(bc),a
	ld 			a,e
	dec 		bc
	ld 			(bc),a
	ex 			de,hl 								; DE is target address.
	jr 			mainInterpreterLoop

	include 	corewords.asm

SetTopPage:
	ret

	org 		8200h
testCode:
	db 			(HCC_LongConstant-hardCodeStart)/2,0C1h,0Fh
	db 			(HCC_One-hardCodeStart)/2
	db 			(HCC_GreaterEqualZero-hardCodeStart)/2
	db 			(HCC_Zero-hardCodeStart)/2
	db 			(HCC_GreaterEqualZero-hardCodeStart)/2
	db 			(HCC_Minus1-hardCodeStart)/2
	db 			(HCC_GreaterEqualZero-hardCodeStart)/2
	db 			(HCC_LongConstant-hardCodeStart)/2,0ABh,0CDh
	db 			0,0

end runTimeStart+40h
	
