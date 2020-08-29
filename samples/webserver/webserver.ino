/**
  ******************************************************************************
  *
  * Xtase - fgalliat @Aug2020
  * 
  * WebServer
  * 
  * inspired by :
  * @author  Waveshare Team
  * @date    23-January-2018
  *
  ******************************************************************************
  */ 

/* Includes ------------------------------------------------------------------*/
#include "srvr.h" // Server functions

/* Entry point ----------------------------------------------------------------*/
void setup() 
{
    // Serial port initialization
    Serial.begin(115200);
    delay(10);

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
