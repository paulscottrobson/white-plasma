{ ************************************************************************* }
{ ***																	*** }
{ ***								Group 1 Words						*** }
{ ***																	*** }
{ ************************************************************************* }

{ *** Basic Constants $8000 is used for the sign bit. *** }
{ Note, 0 -1 1 2 3 could be implemented as keywords. }

: -1 		$FFFF ;
: $8000 	32768 ;
: 256 		$100 ;
	
{ *** Basic stack operators. Same functionality as normal FORTH *** }

: dup		[[sysinfo]] ! [[sysinfo]] @ [[sysinfo]] @ ;
: drop 		[[sysinfo]] ! ;
: swap 		[[sysinfo]] ! >r [[sysinfo]] @ r> ;
: over 		>r dup r> swap ;
: nip 		swap drop ;

{ *** Basic binary bitwise arithmetic operators  *** }

: invert	dup nand ;
: and 		nand invert ;
: or 		invert swap invert nand ;
: xor 		over over invert and >r swap invert and r> or ;

{ *** Byte accessors *** }

: c@ 		@ $00FF and ;
: c! 		swap $FF and over @ $FF00 and or swap ! ;

{ *** Add to memory *** }

: +! 		dup [[sysinfo]] ! @ + [[sysinfo]] @ ! ;

{ *** Simple unary arithmetic *** }

: 1+ 		1 + ;
: 1- 		-1 + ;
: 2+ 		2 + ;
: 2- 		$FFFE + ;
: 2* 		dup + ;
: negate	invert 1+ ;

{ *** Subtraction operator (see documents) *** }

: - 		negate + ;

{ *** Basic comparison operators *** }

: 0=	 	if 0 else -1 then ;
: 0< 		$8000 and if -1 else 0 then ;
: -dup 		dup if dup then ;

{ *** variable helper *** }

: [variable]	r> ;

{ *** Multiply helper *** }			

{ Note: if you are implementing multiply in code, then you don't need this }

variable multResult private

: *+ 		over 1 and 						{ get lowest bit of 2nd value }
			if dup multResult +! then 		{ if non zero add 1st value to count }			
			2* swap 2/ swap ;				{ shift values }


{******* CHANGELOG *******}

{ 10-12-17	: 	First complete and tested version }

: _main $ABCD

0 multResult !
8 9 *+ *+ *+ *+ 
drop drop multResult @

$FEDC [break]