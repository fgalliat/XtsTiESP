/**
 * XtsTiESP - hardware.cpp
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */

#include "hardware.h"

/*
 * in SdFatConfig.h (of SdFatLib)
 *  If the symbol ENABLE_EXTENDED_TRANSFER_CLASS is nonzero, the class SdFatEX
 *  will be defined. If the symbol ENABLE_SOFTWARE_SPI_CLASS is also nonzero,
 *  the class SdFatSoftSpiEX will be defined.
 *  These classes used extended multi-block SD I/O for better performance.
 *  the SPI bus may not be shared with other devices in this mode.
 */
SdFatSoftSpiEX<SD_MISO, SD_MOSI, SD_SCK> SD; // MISO, MOSI, SCK 
bool _setupSd() {
    if ( !SD.begin(SD_CS) ) {
        Serial.println("SD Failed to begin !");
        return false;
    }
    return true;
}


void _setupLed() {
    pinMode( LED, OUTPUT );
    digitalWrite( LED, LOW );
}

void _setupBuzzer() {
    pinMode( BUZZER, OUTPUT );
    digitalWrite( BUZZER, LOW );
    int freq = 2000;
    ledcSetup(BUZ_channel, freq, BUZ_resolution);
    ledcAttachPin(BUZZER, BUZ_channel);
}

void _setupBtn() {
    pinMode( BTN, INPUT_PULLUP );
}

/*
if use regulare ./make && ./send.sh
 Backtrace: 0x40080f75:0x3ffb1e70 0x400d1f35:0x3ffb1e90 0x400d2332:0x3ffb1eb0 0x4000
 0x40080f75: __pinMode at /home/fgalliat/.arduino15/packages/esp32/hardware/esp32/1.0.4/cores/esp32/esp32-hal-gpio.c line 115
 0x400d1f35: uartAttachRx at /home/fgalliat/.arduino15/packages/esp32/hardware/esp32/1.0.4/cores/esp32/esp32-hal-uart.c line 149
 0x400d2332: uartBegin at /home/fgalliat/.arduino15/packages/esp32/hardware/esp32/1.0.4/cores/esp32/esp32-hal-uart.c line 214
*/

HardwareSerial SerialX1(1);
HardwareSerial SerialX(2); // 5v protected - to XtsTiLink MCU

void _setupUarts() {
   // beware w/ make.sh (need spe version) -- else boot crash loop
   SerialX1.begin(115200,SERIAL_8N1,RX1,TX1); // UART 1

   SerialX.begin(115200,SERIAL_8N1,RXx,TXx);  // UART x (5v protected)
}


bool setupHardware() {
    bool ok = true;
    Serial.begin(115200);

    _setupUarts();

    _setupLed();
    
    _setupBuzzer();
    
    _setupBtn();

    ok &= _setupSd();

    return ok;
}

// ======== Buzzer ================
void buzz_tone(int freq) {
    int vol = 125;
    vol = 200;
    ledcWrite(BUZ_channel, vol); // volume
    ledcWriteTone(BUZ_channel, freq); // freq
}

void buzz_noTone() {
    ledcWrite(BUZ_channel, 0); // volume  
}

void beep(int freq, int duration) {
    buzz_tone(freq);
    delay(duration);
    buzz_noTone();
}

// ======== Led ================
void led(bool on) {
    digitalWrite( LED, on ? HIGH : LOW );
}

// ======== Btn ================
bool isBtnPressed() {
    return digitalRead( BTN ) == LOW;
}

// ======== SdCard ================
File sd_openFile(char* filename, bool read, bool append) {
    if ( !read && !append ) {
        if ( SD.exists(filename) ) {
            SD.remove(filename);
        }
    }
    File f = SD.open( filename, read ? FILE_READ : FILE_WRITE );
    return f;
}

