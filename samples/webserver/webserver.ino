/**
  ******************************************************************************
  *
  * Xtase - fgalliat @Aug2020
  * 
  * WebServer
  * 
  * SdCard supoport
  * in SdFatConfig.h (of SdFatLib)
  *  If the symbol ENABLE_EXTENDED_TRANSFER_CLASS is nonzero, the class SdFatEX
  *  will be defined. If the symbol ENABLE_SOFTWARE_SPI_CLASS is also nonzero,
  *  the class SdFatSoftSpiEX will be defined.
  *  These classes used extended multi-block SD I/O for better performance.
  *  the SPI bus may not be shared with other devices in this mode.
  * 
  * 
  * 
  * inspired by :
  * @author  Waveshare Team
  * @date    23-January-2018
  *
  ******************************************************************************
  */ 

#define SD_SUPPORT 1 

#if SD_SUPPORT
#include <SdFat.h>
SdFatSoftSpiEX<2, 4, 14> SD; // MISO, MOSI, SCK 
#define SD_CS 13

void setupSd() {
  if ( !SD.begin(SD_CS) ) {
        Serial.println("SD Failed to begin !");
        return;
    }
    Serial.println("SD Succeed to begin !");
}
#else
void setupSd() {
  Serial.println("NO SD support !");
}
#endif

/* Includes ------------------------------------------------------------------*/
#include "srvr.h" // Server functions

/* Entry point ----------------------------------------------------------------*/
void setup() 
{
    // Serial port initialization
    Serial.begin(115200);
    delay(10);

    // SDCard initialization
    setupSd();

    // Server initialization
    Srvr__setup();

    // Initialization is complete
    Serial.print("\r\nOk!\r\n");
}

/* The main loop -------------------------------------------------------------*/
void loop() 
{
    // The server state observation
    Srvr__loop();
}
