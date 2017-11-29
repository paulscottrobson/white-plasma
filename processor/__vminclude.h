#ifndef __VMI
#define __VMI

#define KWD_LQ_HALT_RQ (0x8000)
#define KWD_AT (0x8001)
#define KWD_C_AT (0x8002)
#define KWD_PLING (0x8003)
#define KWD_C_PLING (0x8004)
#define KWD_F_AT (0x8005)
#define KWD_F_PLING (0x8006)
#define KWD_PAGE_PLING (0x8007)
#define KWD_GREATER_R (0x8008)
#define KWD_R_GREATER (0x8009)
#define KWD_SEMICOLON (0x800a)
#define KWD_LQ_R_GREATER_RQ (0x800b)
#define KWD_LQ_FARCALL_RQ (0x800c)
#define KWD_LQ_NOP_RQ (0x800d)
#define KWD_LQ_LIT_RQ (0x800e)
#define KWD_LQ_BZERO_RQ (0x800f)
#define KWD_PLUS (0x8010)
#define KWD_NAND (0x8011)
#define KWD_2_SLASH (0x8012)
#define KWD_0_EQUALS (0x8013)
#define KWD_LQ_SYSINFO_RQ (0x8014)
#define KWD_CURSOR_PLING (0x8015)
#define KWD_SCREEN_PLING (0x8016)
#define KWD_KEYBOARD_AT (0x8017)
#define KWD_BLOCK_AT (0x8018)
#define KWD_LQ_STACKRESET_RQ (0x8019)
#define KWD_BLOCK_PLING (0x801a)
#define KWD_COUNT (27)

#ifdef MNEMONICS
static const char *_mnemonics[] = { 
"[halt]","@","c@","!","c!","f@","f!","page!",">r","r>",";","[r>]","[farcall]","[nop]","[lit]","[bzero]","+","nand","2/","0=","[sysinfo]","cursor!","screen!","keyboard@","block@","[stackreset]","block!"
};
#endif
#endif
