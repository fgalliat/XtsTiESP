# ESP32 Mini (HW665)

ILI 9341 SPI / SdCard SPI (not shared)

- SD

  ```c++
  #include <SDFat.h>
  SdFatSoftSpiEX<2, 4, 14> SD; // MISO, MOSI, SCK  
  ```

- Screen

  ```
  #include "SPI.h"
  #include <TFT_eSPI.h> // Hardware-specific library for ESP8266
  TFT_eSPI tft = TFT_eSPI();
  
  #define OWN_SPI_CS   5
  #define OWN_SPI_DC   15
  #define OWN_SPI_MOSI 23
  #define OWN_SPI_CLK  18
  #define OWN_SPI_MISO 19
  
  #define TFT_CS OWN_SPI_CS
  //#define TS_CS 26
  #define SD_CS 13 // SD chip select
  ```

  

- Layout (DevKit R1)

  ```
  //                ___________
  //            3.3|           |VIN
  //            GND|           |GND
  //  TFT_DC    D15|           |D13 SD_CS
  //  SD_MISO   D02|           |D12 BUZZER - certified
  //  SD_MOSI   D04|           |D14 SD_CLK
  //  Sub_MCU   RX2|           |D27 LED
  //  Sub_MCU   TX2|           |D26 TS_CS
  //  TFT_CS    D05|           |D25 ~~
  //  TFT_CLK   D18|           |D33 TX1 (aux ExtPort)
  //  TFT_MISO  D19|           |D32 RX1 (aux ExtPort)
  //            D21|           |D35 SubMCU /READY \__ Cf spec: INPUT ONLY
  //            RX0|           |D34 MP3 /BUSY     /
  //            TX0|           |VN
  //            D22|           |VP
  //  TFT_MOSI  D23|___________|EN /RESET
  ```

  