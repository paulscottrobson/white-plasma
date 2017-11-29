; **************************************************************************************
;
;							File of core machine code words
;
;	These are all based around the two pages after the runtime start page. The codes
;	are doubled, and this is an index into this page. Because the code is doubled
; 	all words must be even aligned.
;	
; **************************************************************************************

; **************************************************************************************
;
;								This is the start point. 
;
; **************************************************************************************

	org 		runTimeStart+100h 					; run time routines go here.
hardCodeStart:
	jr 			hardCodeStart 						; 0 stops CPU

; **************************************************************************************
;
;					Push a short constant on the stack, 0-255
;	
; **************************************************************************************

HCC_ShortConstant:									; [[lit:s]]
	xor 		a
	jr 			__HCC_LoadLH
	nop

; **************************************************************************************
;
;			  Push a 16 bit constant on the stack (high, low order)
;	
; **************************************************************************************

HCC_LongConstant:									; [[lit:l]]
	ld 			a,(de) 								; high byte
	inc 		de
__HCC_LoadLH:
	ld 			h,a
	ld 			a,(de) 								; low byte
	inc 		de
	ld 			l,a
__HCC_PushHLExit	
	push 		hl
	nop
__HCC_Nop:											; [[nop]]	
	jp 			(ix)

; **************************************************************************************
;
;						Duplicate the top of the stack.
;
; **************************************************************************************

HCC_Dup:											; [[dup]]
	pop			hl
	push 		hl
	jr 			__HCC_PushHLExit

; **************************************************************************************
;
;							Drop the top of the stack
;
; **************************************************************************************

HCC_Drop:											; [[drop]]
	pop			hl
	jp 			(ix)
	nop

; **************************************************************************************
;
;					Swap the top two items on the stack.
;
; **************************************************************************************

HCC_Swap:											; [[swap]]
	exx
	pop			de
	pop 		hl
__HCC_PushDEHLExx:	
	push 		de
__HCC_PushHLExx:	
	push 		hl
__HCC_Exx:
	exx
	jp 			(ix)
;
;	Drop the 2nd item on the stack
;
HCC_Nip:											; [[nip]]
	exx
	pop			hl
	pop 		de
	jr 			__HCC_PushHLExx
	nop
;
;	Duplicate the 2nd item on the stack 			
;
HCC_Over:											; [[over]]
	exx
	pop 		hl
	pop 		de
	push 		hl
	jr 			__HCC_PushDEHLExx
;
;	16 bit read.
;	
HCC_Read16:											; [[@]]
	pop 		hl
	ld 			a,(hl)
	inc 		hl
	ld 			h,(hl)
	ld 			l,a
	jr 			__HCC_PushHLExit
	nop
;
;	8 bit read.
;
HCC_Read8:											; [[c@]]
	pop			hl
	ld 			l,(hl)
	ld 			h,0
	jr 			__HCC_PushHLExit

; **************************************************************************************
;
;										16 bit write
;
; **************************************************************************************

HCC_Write16:										; [[!]]
	exx
	pop 		hl
	pop 		de
__HCC_WriteDEToHLExx:	
	ld 			(hl),e
	inc 		hl
	ld 			(hl),d
	jr 			__HCC_Exx

; **************************************************************************************
;
;								8 bit write
;
; **************************************************************************************

HCC_Write8:											; [[c!]]
	exx
	pop			hl
	pop 		de
	ld 			(hl),e
	jr 			__HCC_Exx

; **************************************************************************************
;
;							 16 bit add to memory
;
; **************************************************************************************

HCC_Add16Bit:										; [[+!]]
	exx 	
	pop 		hl 									; HL = address
	pop 		de 									; DE = data
	ld 			c,(hl)								; BC = address contents
	inc 		hl
	ld 			b,(hl)
	dec			hl
	ex 			de,hl 								; DE = address, HL = data
	add 		hl,bc 								; DE = address, HL = result
	ex 			de,hl 								; HL = address, DE = result
	jr 			__HCC_WriteDEToHLExx

; **************************************************************************************
;
;								16 bit logical not 
;
; **************************************************************************************

HCC_Minus:											; [[-]]	why Chuck ?
	pop			hl
	ld 			a,h
	cpl
	ld 			h,a
	ld 			a,l
	cpl
	ld 			l,a
	jr 			__HCC_PushHLExit
	nop

; **************************************************************************************
;
;								16 bit negation
;
; **************************************************************************************

HCC_Negate:											; [[negate]]
HCC_0Minus:											; [[0-]]
	exx
	pop 		de
	xor 		a
	ld 			h,a
	ld 			l,a
__HCCSubPushHLExx:
	sbc 		hl,de
	jr 			__HCC_PushHLExx
	nop

; **************************************************************************************
;
;							16 bit subtraction
;
; **************************************************************************************

HCC_Subtract:										; [[--]]
	exx
	pop 		de
	pop	 		hl
	xor 		a
	jr 			__HCCSubPushHLExx

; **************************************************************************************
;
;							Arithmetic shift left
;	
; **************************************************************************************

HCC_2Times:											; [[2*]]
	pop			hl
	add 		hl,hl
	jr 			__HCC_PushHLExit

; **************************************************************************************
;
;							Arithmetic shift right
;	
; **************************************************************************************

HCC_2Divide:										; [[2/]]
	pop 		hl
	sra 		h
	rr 			l
__HCC_PushHLExit2:	
	jr 			__HCC_PushHLExit
	nop

; **************************************************************************************
;
;							Check if equal to zero.
;	
; **************************************************************************************

HCC_EqualZero:										; [[0=]]
	pop 		hl
	ld 			a,h
	or 			l
	jr 			z,HCC_True
	nop
;
;	Push zero and False
;
HCC_False:											; [[false]]
HCC_Zero:											; [[0]]
	ld 			hl,0
	jr 			__HCC_PushHLExit2
	nop

; **************************************************************************************
;
;								  Push -1 and True
;	
; **************************************************************************************

HCC_True:											; [[true]]
HCC_Minus1:											; [[-1]]
HCC_1Minus:											; [[1-]]
	ld 			hl,0FFFFh
	jr 			__HCC_PushHLExit2
	nop

; **************************************************************************************
;
;										Push 1.
;
; **************************************************************************************

HCC_One:
	ld 			hl,1 								; [[1]]
	jr 			__HCC_PushHLExit2
	nop

; **************************************************************************************
;
;									Check if negative
;
; **************************************************************************************

HCC_LessZero:										; [[<0]]
	pop 		hl
	bit 		7,h
	jr 			nz,HCC_True
	jr 			HCC_False
	nop

; **************************************************************************************
;
;						Check if greater than or equal to zero.
;
; **************************************************************************************

HCC_GreaterEqualZero:								; [[>=0]]
	pop 		hl
	bit 		7,h
	jr 			z,HCC_True
	jr 			HCC_False
	nop

; **************************************************************************************
;
;										16 bit and
;
; **************************************************************************************

HCC_And:											; [[and]]
	exx
	pop 		de
	pop 		hl
	ld 			a,h
	and 		d
	ld 			h,a
	ld 			a,l
	and 		e
__HCC_AToLPushHLAndExit
	ld  		l,a
	push 		hl
	exx
	jp 			(ix)
	nop

; **************************************************************************************
;
;						16 bit xor (note, or is exclusive or)
;
; **************************************************************************************

HCC_Xor:											; [[xor]]
HCC_Or:												; [[or]]
	exx
	pop 		de
	pop 		hl
	ld 			a,h
	xor 		d
	ld 			h,a
	ld 			a,l
	xor 		e
	jr 			__HCC_AToLPushHLAndExit

; **************************************************************************************
;
;							16 bit or (note, +or is or)
;
; **************************************************************************************

HCC_PlusOr:											; [[+or]]
HCC_Ior:											; [[ior]]
	exx
	pop 		de
	pop 		hl
	ld 			a,h
	or 			d
	ld 			h,a
	ld 			a,l
	or 			e
	jr 			__HCC_AToLPushHLAndExit

; **************************************************************************************
;
;								Subroutine Return.
;	
; **************************************************************************************

HCC_Return:											; [[;]]
	ld 			a,(bc)		
	ld 			e,a 								; pop address to DE
	inc 		bc
	ld 			a,(bc)
	ld 			d,a
	inc 		bc
	bit 		7,d 								; if bit 7 set no return.
	jr 			nz,__HCC_NoPageReturn
	ld 			a,(bc) 								; restore page number.
	inc 		bc
	inc 		bc
	call 		SetTopPage 							; go back to that page.
	set 		7,d 								; correct return address
__HCC_NoPageReturn:
	jp 			(ix)

; **************************************************************************************
;
;							Data Stack to Return stack 						
;
; **************************************************************************************

HCC_DSToRS:									; [[>r]]
	pop 		hl 
	ld 			a,h
	dec 		bc
	ld 			(bc),a
	ld 			a,l
	dec 		bc 
	ld 			(bc),a
	jp 			(ix)
	nop

; **************************************************************************************
;
;							 Return stack to Data stack
;
; **************************************************************************************

HCC_RSToDS:									; [[r>]]
	ld 			a,(bc)
	ld 			l,a
	inc 		bc
	ld 			a,(bc)
	ld 			h,a
	inc 		bc
	jp 			__HCC_PushHLExit2
	nop


;	<branch short/nonzero/positive>
; 	<next>
;	0< 0>= 0= 
; 	<far call>