/**
 * XtsTiESP - tiEsp.ino
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */


#include "hardware.h"

void setup() {
    bool ok = setupHardware();

    led(true);
    beep(440, 200);
    led(false);
    beep(220, 200);
}

void loop() {
    delay(1000);
    Serial.write('.');
}