# ESP32 Mini (HW665)

ILI 9341 SPI / SdCard SPI (not shared)

- SD - ok

  ```c++
  #include <SDFat.h>
  
  // in SdFatConfig.h (of SdFatLib)
  //  If the symbol ENABLE_EXTENDED_TRANSFER_CLASS is nonzero, the class SdFatEX
  //  will be defined. If the symbol ENABLE_SOFTWARE_SPI_CLASS is also nonzero,
  //  the class SdFatSoftSpiEX will be defined.
  //  These classes used extended multi-block SD I/O for better performance.
  //  the SPI bus may not be shared with other devices in this mode.
  
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

- TFT_eSPI User_Setup.h

- ```c++
  #define TFT_MISO 19
  #define TFT_MOSI 23
  #define TFT_SCLK 18
  //#define TFT_CS   15  // Chip select control pin
  //#define TFT_CS   14  // as for M5Stack emulation
  #define TFT_CS   5  // as for YAEL layout
  
  //#define TFT_DC    2  // Data Command control pin
  //#define TFT_DC    27  // as for M5Stack emulation
  #define TFT_DC    15  // as for YAEL layout
  
  //#define TFT_RST   4  // Reset pin (could connect to RST pin)
  #define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST
  ```




- Serial UARTs layout

  ```c++
  // legacy RX 1 doesn't work
  #define RX1 16
  #define TX1 9
  HardwareSerial SerialX1(1);
  
  // connected to XtsTiLink ProMicro 5v
  #define TXx 33
  #define RXx 32
  HardwareSerial SerialX(2); // 5v protected
  ```

- Layout (DevKit R1) (inspired from YAEL project) - **just as an example**

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

  