/**
 * XtsTiESP - tilink.cpp
 * 
 * link to Ti92 via XtsTiLink_gh project (own project too)
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */

#include "hardware.h"
#include "tilink.h"

Stream* serialPtr = NULL;

void ti_setSerial(Stream* _serialPtr) {
    serialPtr = _serialPtr;
}


bool _ti_readError() {
    if ( serialPtr->available() <= 0 ) {
        return false;
    }

    if ( serialPtr->peek() == 'E' ) {
        serialPtr->read();
        if ( serialPtr->peek() == ':' ) {
            serialPtr->read();
            Serial.write("E:Ti:");
            int ch;
            while( serialPtr->available() ) {
                ch = serialPtr->read();
                Serial.write( ch );
                if ( ch == '\n' ) { break; }
            }
        }
    }

    return true;
}

bool _ti_readInfo() {
    if ( serialPtr->available() <= 0 ) {
        return false;
    }

    if ( serialPtr->peek() == 'I' ) {
        serialPtr->read();
        if ( serialPtr->peek() == ':' ) {
            serialPtr->read();
            Serial.write("I:Ti:");
            int ch;
            while( serialPtr->available() ) {
                ch = serialPtr->read();
                Serial.write( ch );
                if ( ch == '\n' ) { break; }
            }
        }
    }

    return true;
}




// >> \SW
void ti_wakeUpCalc() {
    serialPtr->print("\\SW");
    while( _ti_readError() ) {
        delay(100);
    }
}

// launch then wait for
// K 0 0 0
// >> \SL
bool ti_launchProgram() {
    serialPtr->print("\\SL");
    // check if errors readed 'E:..'
    while( _ti_readError() ) {
        delay(100);
    }
    return true;
}

// >> NYI ?
bool ti_strikeEsc() {
    return false;
}

/** 240 x 128 bits => 3840 bytes */
// >> \Ss .... [3840/256] [3840%256] [ 3840 bytes of bitmap ] 
bool ti_screenDump(uint8_t* destBuffer) {
    return false;
}

// >> \SR
void ti_resetLink() {
    serialPtr->print("\\SR");
    delay(500);
}

// \SK
bool ti_sendTermProgram() {
    serialPtr->print("\\SK");

    // read 5 lines w/ "I:..."

    // check if errors readed 'E:..'
    return true;
}

// just read stream & waitfor K Y X X
// beware of "I:..." info
// beware of "E:..." errors
int ti_readKey(char &ch) {
    while( _ti_readError() ) {
        delay(100);
    }

    // -1 for no key
    // 0 for no spe key 0 (when prg starts)
    // else keyCode

    return 0;
}

