/**
 * XtsTiESP - tilink.h
 * 
 * link to Ti92 via XtsTiLink_gh project (own project too)
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */

void ti_setSerial(Stream* serialPtr);

// >> \SW
void ti_wakeUpCalc();

// launch then wait for
// K 0 0 0
// >> \SL
bool ti_launchProgram();

// >> NYI ?
bool ti_strikeEsc();

/** 240 x 128 bits => 3840 bytes */
// >> \Ss .... [3840/256] [3840%256] [ 3840 bytes of bitmap ] 
bool ti_screenDump(uint8_t* destBuffer);

// >> \SR
void ti_resetLink();

// \SK
bool ti_sendTermProgram();

// just read stream & waitfor K Y X X
// beware of "I:..." info
// beware of "E:..." errors
int ti_readKey(char &ch);

