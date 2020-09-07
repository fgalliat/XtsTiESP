/**
 * XtsTiESP - hardware.cpp
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */

#include "hardware.h"

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

bool setupHardware() {
    Serial.begin(115200);

    _setupLed();
    
    _setupBuzzer();
    
    _setupBtn();

    return true;
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

