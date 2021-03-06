#ifndef __VMI
#define __VMI

#define KWD_LQ_BREAK_RQ (0x0000)
#define KWD_SEMICOLON (0x0001)
#define KWD_PLING (0x0002)
#define KWD_LQ_BR_RQ (0x0003)
#define KWD_LQ_BR_DOT_ZERO_RQ (0x0004)
#define KWD_LQ_FARCALL_RQ (0x0005)
#define KWD_LQ_LITERAL_DOT_S_RQ (0x0006)
#define KWD_LQ_LITERAL_RQ (0x0007)
#define KWD_LQ_NOP_RQ (0x0008)
#define KWD_LQ_STACKRESET_RQ (0x0009)
#define KWD_AT (0x000a)
#define KWD_PLUS (0x000b)
#define KWD_GREATER_R (0x000c)
#define KWD_2_SLASH (0x000d)
#define KWD_CURSOR_PLING (0x000e)
#define KWD_KEYBOARD_AT (0x000f)
#define KWD_NAND (0x0010)
#define KWD_PAGE_PLING (0x0011)
#define KWD_PAGE_AT (0x0012)
#define KWD_R_GREATER (0x0013)
#define KWD_SCREEN_PLING (0x0014)
#define KWD_COUNT (21)

#define SYS_ADDR_LOAD (0x6000)

#ifdef MNEMONICS
static const char *_mnemonics[] = { 
"[break]",";","!","[br]","[br.zero]","[farcall]","[literal.s]","[literal]","[nop]","[stackreset]","@","+",">r","2/","cursor!","keyboard@","nand","page!","page@","r>","screen!"
};
#endif
#endif
