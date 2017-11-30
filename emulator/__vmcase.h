case KWD_LQ_HALT_RQ:/*** [halt] ***/
    pc = (pc - 2) & 0xFFFF;break;
case KWD_AT:/*** @ ***/
    w1 = PULLD();PUSHD(READ16(w1));break;
case KWD_C_AT:/*** c@ ***/
    w1 = PULLD();PUSHD(READ8(w1));break;
case KWD_PLING:/*** ! ***/
    w1 = PULLD();w2 = PULLD();WRITE16(w1,w2);break;
case KWD_C_PLING:/*** c! ***/
    w1 = PULLD();w2 = PULLD();WRITE8(w1,w2);break;
case KWD_PAGE_PLING:/*** page! ***/
    SETPAGE(PULLD());break;
case KWD_GREATER_R:/*** >r ***/
    w1 = PULLD();PUSHR(w1);break;
case KWD_R_GREATER:/*** r> ***/
    w1 = PULLR();PUSHD(w1);;break;
case KWD_SEMICOLON:/*** ; ***/
    pc = PULLR(); if ((pc & 0x8000) == 0) { pc = pc | 0x8000; SETPAGE(PULLR()); };break;
case KWD_LQ_R_GREATER_RQ:/*** [r>] ***/
    w1 = PULLR(); if ((w1 & 0x8000) == 0) ERROR("[>R] triggered on far call"); PUSHD(w1);;break;
case KWD_LQ_FARCALL_RQ:/*** [farcall] ***/
    w1 = READ16(pc);pc = pc + 2; PUSHR(GETPAGE()); PUSHR(pc & 0x7FFF); SETPAGE(w1 & 0x1F); pc = ((w1 & 0xFFE0) >> 2) | 0xC000;break;
case KWD_LQ_NOP_RQ:/*** [nop] ***/
    ;;break;
case KWD_LQ_LIT_RQ:/*** [lit] ***/
    w1 = READ16(pc);pc = pc + 2;PUSHD(w1);;break;
case KWD_LQ_BZ_RQ:/*** [bz] ***/
    w1 = READ16(pc);pc = pc + 2; if (PULLD() == 0) pc = (pc + w1) & 0xFFFF;;break;
case KWD_PLUS:/*** + ***/
    w1 = PULLD();w2 = PULLD();PUSHD(w1+w2);;break;
case KWD_NAND:/*** nand ***/
    w1 = PULLD();w2 = PULLD();PUSHD((w1 & w2) ^ 0xFFFF);;break;
case KWD_2_SLASH:/*** 2/ ***/
    w1 = PULLD();w1 = (w1 >> 1) | (w1 & 0x8000);PUSHD(w1);break;
case KWD_0_EQUALS:/*** 0= ***/
    w1 = PULLD();w1 = (w1 == 0) ? 0xFFFF:0x0000;PUSHD(w1);break;
case KWD_LQ_SYSINFO_RQ:/*** [sysinfo] ***/
    PUSHD(SYS_DICTIONARY_BASE_ADDRESS);;break;
case KWD_CURSOR_PLING:/*** cursor! ***/
    HWISetCursor(PULLD());;break;
case KWD_SCREEN_PLING:/*** screen! ***/
    w1 = PULLD();w2 = PULLD(); if (w1 >= 0 && w1 < 1024) { memory[SYS_DISPLAY_MEMORY+w1] = w2 & 0xFF; };break;
case KWD_KEYBOARD_AT:/*** keyboard@ ***/
    w1 = HWIGetKey();PUSHD(w1);;break;
case KWD_BLOCK_AT:/*** block@ ***/
    ;;break;
case KWD_LQ_STACKRESET_RQ:/*** [stackreset] ***/
    rSP = dSP = 0;;break;
case KWD_BLOCK_PLING:/*** block! ***/
    ;;break;
