// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		hardware.cpp
//		Purpose:	Hardware Interface
//		Date:		1st October 2017
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include "sys_processor.h"
#include "hardware.h"
#include "gfx.h"																

static BYTE8 currentKey = 0;
static BYTE8 cursorPos = 0;

// *******************************************************************************************************************************
//										Hardware Reset
// *******************************************************************************************************************************

void HWIReset(void) {
	currentKey = 0;
}

// *******************************************************************************************************************************
//											End of frame code.
// *******************************************************************************************************************************

void HWIEndFrame() {
}

// *******************************************************************************************************************************
//									Get and set cursor position
// *******************************************************************************************************************************

void HWISetCursor(BYTE8 pos) {
	cursorPos = pos;
}
BYTE8 HWIGetCursor(void) {
	return cursorPos;
}

// *******************************************************************************************************************************
//									Get & Clear any pending key press
// *******************************************************************************************************************************

BYTE8 HWIGetKey() {
	BYTE8 key = currentKey;
	currentKey = 0;
	return key;
}

// *******************************************************************************************************************************
//										Intercept keyboard processing
// *******************************************************************************************************************************

int HWIProcessKey(int key,int runMode) {
	if (key <= 0) return key;
	int aKey = GFXToASCII(key,1);
	if (aKey >= 32 && aKey < 96) currentKey = aKey;
	if (aKey >= 'a' && aKey <= 'z') currentKey = aKey - 32;
	if (aKey == 13 || aKey == 8) currentKey = aKey;
	/*
	if (key == GFXKEY_F2) currentKey = 16;
	if (key == GFXKEY_F3) currentKey = 17;
	if (key == GFXKEY_F4) currentKey = 18;
	if (key == GFXKEY_F5) currentKey = 19;
	*/
	if (key == GFXKEY_UP) currentKey = 20;
	if (key == GFXKEY_DOWN) currentKey = 21;
	if (key == GFXKEY_LEFT) currentKey = 22;
	if (key == GFXKEY_RIGHT) currentKey = 23;
	//printf("%d %d %d\n",aKey,key,currentKey);
	return key;
}

