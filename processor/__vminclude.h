#ifndef __VMI
#define __VMI

#define KWD_SEMICOLON (0x8000)
#define KWD_PLING (0x8001)
#define KWD_QUESTION (0x8002)
#define KWD_LQ_BR_DOT_ZERO_RQ (0x8003)
#define KWD_LQ_BR_DOT_POS_RQ (0x8004)
#define KWD_LQ_BR_RQ (0x8005)
#define KWD_LQ_CLR_DOT_DS_RQ (0x8006)
#define KWD_LQ_CLR_DOT_RS_RQ (0x8007)
#define KWD_LQ_HALT_RQ (0x8008)
#define KWD_LQ_LITERAL_RQ (0x8009)
#define KWD_LQ_NOP_RQ (0x800a)
#define KWD_LQ_R_GREATER_RQ (0x800b)
#define KWD_LQ_SYSINFO_RQ (0x800c)
#define KWD_AT (0x800d)
#define KWD_PLUS (0x800e)
#define KWD_GREATER_R (0x800f)
#define KWD_2_SLASH (0x8010)
#define KWD_C_PLING (0x8011)
#define KWD_C_AT (0x8012)
#define KWD_CURSOR_PLING (0x8013)
#define KWD_KEYBOARD_AT (0x8014)
#define KWD_NAND (0x8015)
#define KWD_PAGE_PLING (0x8016)
#define KWD_R_GREATER (0x8017)
#define KWD_SCREEN_PLING (0x8018)
#define KWD_COUNT (25)

#ifdef MNEMONICS
static const char *_mnemonics[] = { 
";","!","?","[br.zero]","[br.pos]","[br]","[clr.ds]","[clr.rs]","[halt]","[literal]","[nop]","[r>]","[sysinfo]","@","+",">r","2/","c!","c@","cursor!","keyboard@","nand","page!","r>","screen!"
};
#endif
#endif
