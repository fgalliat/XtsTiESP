/**
 * XtsTiESP - tiEsp.ino
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */


#include "hardware.h"


#include "tilink.h"
#include "mediaplayer.h"

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


    if ( true ) {
        playAnime("/media/anime/DOOR");
        playAnime("/media/anime/FACEW");
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

    if ( true ) {
        if ( Serial.available() > 0 ) {
            static const int buffSize = 64;
            char msg[buffSize+1]; memset(msg, 0x00, buffSize+1);
            int read = Serial.readBytesUntil('\n', msg, buffSize);

            // test : $> echo "vid:/media/anime/FACEW" > /dev/ttyUSB0
            if ( strncmp("vid:", msg, 4) == 0 ) {
                bool ok = playAnime(&msg[4]);
                if (!ok) {
                    Serial.printf("Could not play '%s' \n", &msg[4]);
                }
            } else {
                Serial.print("I read : "); Serial.println(msg);
                Serial.println("------------");
            }

        } else {
            delay(100);
        }
    } else {
        delay(5000);
        Serial.write('.');
    }

}