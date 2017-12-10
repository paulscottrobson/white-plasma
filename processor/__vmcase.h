case KWD_LQ_BREAK_RQ:/*** [break] ***/
    pc = (pc - 1) & 0xFFFF;;break;
case KWD_SEMICOLON:/*** ; ***/
    pc = PULLR(); if ((pc & 0x8000) == 0) { pc = pc | 0x8000; SETPAGE(PULLR()); };break;
case KWD_PLING:/*** ! ***/
    w1 = PULLD();w2 = PULLD();WRITE16(w1,w2);break;
case KWD_LQ_BR_RQ:/*** [br] ***/
    w1 = READ8(pc);pc = pc + 1; if (w1 & 0x80) w1 |= 0xFF00; pc = pc + w1;;break;
case KWD_LQ_BR_DOT_ZERO_RQ:/*** [br.zero] ***/
    w1 = READ8(pc);pc = pc + 1; if (PULLD() == 0) { if (w1 & 0x80) w1 |= 0xFF00; pc = pc + w1; };break;
case KWD_LQ_FARCALL_RQ:/*** [farcall] ***/
    ;;break;
case KWD_LQ_LITERAL_DOT_S_RQ:/*** [literal.s] ***/
    w1 = READ8(pc);pc = pc + 1;PUSHD(w1);;break;
case KWD_LQ_LITERAL_RQ:/*** [literal] ***/
    w1 = READ16(pc);pc = pc + 2;PUSHD(w1);;break;
case KWD_LQ_NOP_RQ:/*** [nop] ***/
    ;;break;
case KWD_LQ_STACKRESET_RQ:/*** [stackreset] ***/
    dSP = 0; rSP = 0;;break;
case KWD_AT:/*** @ ***/
    w1 = PULLD();PUSHD(READ16(w1));break;
case KWD_PLUS:/*** + ***/
    w1 = PULLD();w2 = PULLD();PUSHD(w1+w2);;break;
case KWD_GREATER_R:/*** >r ***/
    w1 = PULLD();PUSHR(w1);break;
case KWD_2_SLASH:/*** 2/ ***/
    w1 = PULLD();PUSHD((w1 >> 1) | (w1 & 0x8000));;break;
case KWD_CURSOR_PLING:/*** cursor! ***/
    HWISetCursor(PULLD());;break;
case KWD_KEYBOARD_AT:/*** keyboard@ ***/
    w1 = HWIGetKey();PUSHD(w1);;break;
case KWD_NAND:/*** nand ***/
    w1 = PULLD();w2 = PULLD();PUSHD((w1 & w2) ^ 0xFFFF);;break;
case KWD_PAGE_PLING:/*** page! ***/
    SETPAGE(PULLD());break;
case KWD_PAGE_AT:/*** page@ ***/
    PUSHD(GETPAGE());break;
case KWD_R_GREATER:/*** r> ***/
    w1 = PULLR();PUSHD(w1);;break;
case KWD_SCREEN_PLING:/*** screen! ***/
    w1 = PULLD();w2 = PULLD(); if (w1 >= 0 && w1 < 1024) { memory[SYS_DISPLAY_MEMORY+w1] = w2 & 0xFF; };break;
