/**
 * XtsTiESP - tiEsp.ino
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */


#include "hardware.h"


#include "tilink.h"

void setup() {
    bool ok = setupHardware();

    led(true);
    beep(440, 200);
    led(false);
    beep(220, 200);

    if ( !ok ) {
        Serial.println("Some hardware failed !");
        led(true);
    }

}

void loop() {
    // FIXME : check that sd is OK before ...
    if ( false ) {
        led(true);
        File f = sd_openFile("/hello.txt");
        if ( !f ) {
            Serial.println("Failed to open file");
        } else {
            Serial.println("Able to open file");
            while( f.available() ) {
                Serial.write( f.read() ); 
            }
            f.close();
        }
        led(false);
    }

    delay(5000);
    Serial.write('.');
}