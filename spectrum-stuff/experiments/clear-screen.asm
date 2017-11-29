; This is where your program starts in RAM.
org 8000h

addr equ 081h

start:
; Clear the screen to black.
    ld a,4         ; White ink (7), black paper (0), bright (64).
    ld (23693),a    ; Set our screen colours.
    xor a           ; Load accumulator with zero.
    call 8859       ; Set permanent border colours.
    call 3503       ; Clear the screen, open channel 2.

    ld 		hl,08000h
    di
	ld 		a,h
	ld 		i,a
setim2:
	ld 		(hl),addr
	inc 	hl
	ld 		a,l
	or 		a
	jr 		nz,setim2
	ld 		(hl),addr
	im		2
	ei
wait:
	;call 	loop
	jr 		wait

	org addr * 257

loop:
	di
	push af
	push hl
    ld 	hl,4000h
loop2:
    inc (hl)
    inc h
    ld a,h
    cp 48h
    jr nz,loop2
    pop hl
    pop af
    ei
    reti

end start

