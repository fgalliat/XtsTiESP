/**
 * 
 * SDCard example
 * 
 * Xtase - fgalliat @Aug 2020
 * 
 * 
 * 
 * in SdFatConfig.h (of SdFatLib)
 *  If the symbol ENABLE_EXTENDED_TRANSFER_CLASS is nonzero, the class SdFatEX
 *  will be defined. If the symbol ENABLE_SOFTWARE_SPI_CLASS is also nonzero,
 *  the class SdFatSoftSpiEX will be defined.
 *  These classes used extended multi-block SD I/O for better performance.
 *  the SPI bus may not be shared with other devices in this mode.

 * 
 * 
 */

#include <SdFat.h>
SdFatSoftSpiEX<2, 4, 14> SD; // MISO, MOSI, SCK 
#define SD_CS 13

void setup() {
    Serial.begin(115200);

    while (!Serial) {
        delay(200);
    }
    

    if ( !SD.begin(SD_CS) ) {
        Serial.println("SD Failed to begin !");
        return;
    }
    Serial.println("SD Succeed to begin !");

    File f = SD.open("/hello.txt", FILE_WRITE);
    if ( !f ) { Serial.println("File W failed to open !"); return; }
    f.println("Hello world !");
    f.close();
    Serial.println("File Wrote !");

    f = SD.open("/hello.txt", FILE_READ);
    if ( !f ) { Serial.println("File R failed to open !"); return; }
    while( f.available() ) {
        Serial.write( f.read() );
    }
    f.close();
    Serial.println("File Read !");

    Serial.println(" = completed = ");
}

void loop() {
    delay(100);
}