// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		sys_processor.cpp
//		Purpose:	Processor Emulation.
//		Created:	26th October 2015
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include "sys_processor.h"
#include "hardware.h"
#include "__vminclude.h"

// *******************************************************************************************************************************
//												Main Memory/CPU
// *******************************************************************************************************************************

#define RAMSIZE 	(49152 + 10 * 16384)										// RAM 48k + 10x16k pages.

static BYTE8 memory[RAMSIZE];													// RAM memory 

static BYTE8 rSP,dSP;															// SP (next free +ve)
static WORD16 returnStack[32],dataStack[32];									// Stacks
static WORD16 pc;																// Program Counter
static WORD16 page;																// Currently mapped page
static WORD16 cycles;
static WORD16 testFlags;														// Flag status.

// *******************************************************************************************************************************
//											  Paged memory code
// *******************************************************************************************************************************

#define READ16(a) 		__read16(a)								// Apply to current page
#define READ8(a) 		__read8(a)
#define WRITE16(a,d) 	__write16(a,d)
#define WRITE8(a,d)		__write8(a,d)

#define SETPAGE(n)		page = (n)
#define GETPAGE()		(page)

static LONG32 inline __addressMap(WORD16 address) {
	if (address < 0xC000) return address;
	return (address & 0x3FFF)+0xC000+page*0x4000;
}

static WORD16 inline __read16(WORD16 address) {
	LONG32 addr = __addressMap(address);
	return memory[addr] + (memory[addr+1] << 8);
}

static void inline __write16(WORD16 address,WORD16 data) {
	LONG32 addr = __addressMap(address);
	memory[addr] = data & 0xFF;
	memory[addr+1] = data >> 8;
}

static BYTE8 inline __read8(WORD16 address) {
	LONG32 addr = __addressMap(address);
	return memory[addr];
}

static void inline __write8(WORD16 address,BYTE8 data) {
	LONG32 addr = __addressMap(address);
	memory[addr] = data;
}

// *******************************************************************************************************************************
//												 Error method
// *******************************************************************************************************************************

#define ERROR(msg) 	__error(msg)

static void __error(const char *msg) {
	fprintf(stderr,"Error %s at %04x\n",msg,pc);
	exit(0);
}

// *******************************************************************************************************************************
//												Processor Reset
// *******************************************************************************************************************************

void CPUReset(void) {

	for (int i = 0x100;i < 0x1000;i++) memory[i] = rand();
	rSP = dSP = 0;testFlags = 0;
	page = READ8(SYS_DICTIONARY_BASE_ADDRESS+9);
	pc = READ16(SYS_DICTIONARY_BASE_ADDRESS+10);
	cycles = 0;
	HWIReset();
}

// *******************************************************************************************************************************
//													 Execute a single phase.
// *******************************************************************************************************************************

#include <stdlib.h>
#include <stdio.h>

#define TOS() 		dataStack[dSP-1]
#define PULLD() 	dataStack[--dSP]
#define PUSHD(n) 	dataStack[dSP++] = (n)
#define PULLR() 	returnStack[--rSP]
#define PUSHR(n) 	returnStack[rSP++] = (n)

BYTE8 CPUExecuteInstruction(void) {
	WORD16 w1,w2;
	cycles++;
	WORD16 opcode = READ16(pc);
	if (opcode == 0) { HWIEndFrame();return FRAME_RATE; }
	pc = (pc + 2) & 0xFFFF;

	if ((opcode & 0x8000) != 0) {											// Core word
		switch(opcode) {
			#include "__vmcase.h"
		}	
	} else {																// Direct call.
		PUSHR(pc);
		pc = opcode|0x8000;
	}
	rSP = rSP & 31;dSP = dSP & 31;											// Fix up stacks.
	if (cycles < CYCLES_PER_FRAME) return 0;								// Frame in progress, return 0.
	cycles -= CYCLES_PER_FRAME;												// Adjust cycle counter
	HWIEndFrame();															// Hardware stuff.
	return FRAME_RATE;														// Return the frame rate for sync speed.
}

#ifdef INCLUDE_DEBUGGING_SUPPORT

// *******************************************************************************************************************************
//										 Get the step over breakpoint value
// *******************************************************************************************************************************

WORD16 CPUGetStepOverBreakpoint(void) {
	if ((READ16(pc) & 0xC000) != 0x8000) return pc + 2;
	return 0;
}

// *******************************************************************************************************************************
//										Run continuously till breakpoints / halt.
// *******************************************************************************************************************************

BYTE8 CPUExecute(WORD16 break1,WORD16 break2) {
	BYTE8 rate = 0;
	while(1) {
		rate = CPUExecuteInstruction();												// Execute one instruction phase.
		if (rate != 0) {															// If end of frame, return rate.
			return rate;													
		}
		if (pc == break1 || pc == break2) return 0;
	} 																				// Until hit a breakpoint or HLT.
}

// *******************************************************************************************************************************
//												Load a binary file into RAM
// *******************************************************************************************************************************

void CPULoadBinary(char *fileName) {
	FILE *f = fopen(fileName,"rb");
	fread(memory+SYS_DICTIONARY_BASE_ADDRESS,RAMSIZE,1,f);
	fclose(f);
}

// *******************************************************************************************************************************
//												Gets a pointer to RAM memory
// *******************************************************************************************************************************
	
BYTE8 CPURead(WORD16 address) {
	return READ8(address);
}

// *******************************************************************************************************************************
//											Retrieve a snapshot of the processor
// *******************************************************************************************************************************

static CPUSTATUS s;																	// Status area

CPUSTATUS *CPUGetStatus(void) {
	s.dataStack = dataStack;
	s.returnStack = returnStack;
	s.dsp = dSP;
	s.rsp = rSP;
	s.pc = pc;
	s.page = page;
	s.cycles = cycles;
	s.zFlag = (testFlags == 0);
	s.sFlag = (testFlags & 0x8000) != 0;
	return &s;
}
#endif
