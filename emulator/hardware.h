// *******************************************************************************************************************************
// *******************************************************************************************************************************
//
//		Name:		hardware.h
//		Purpose:	Hardware Interface (header)
//		Date:		1st October 2017
//		Author:		Paul Robson (paul@robsons.org.uk)
//
// *******************************************************************************************************************************
// *******************************************************************************************************************************

#ifndef _HARDWARE_H
#define _HARDWARE_H
	
void HWIReset(void);
int HWIProcessKey(int key,int runMode);
BYTE8 HWIGetKey();
void HWISetCursor(WORD16 pos);
WORD16 HWIGetCursor(void);
void  HWIEndFrame(void);
#endif
