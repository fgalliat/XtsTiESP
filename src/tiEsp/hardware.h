/**
 * XtsTiESP - hardware.h
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */

#include <Arduino.h>
#include "globals.h"

#include <SPI.h>
#include <TFT_eSPI.h>
// in : TFT_eSPI User_Setup.h
#define TFT_MISO 19
#define TFT_MOSI 23
#define TFT_SCLK 18
#define TFT_CS   5  // as for YAEL layout
#define TFT_DC    15  // as for YAEL layout
#define TFT_RST  -1  // Set TFT_RST to -1 if display RESET is connected to ESP32 board RST

extern TFT_eSPI tft;


// ==== Custom SPI SdCard ==== 
// beware if "WiFi.h" is called after : need to redeclare file access flags 
// else can't CREATE new file !!!
#include <SdFat.h>
#define SD_CS 13
#define SD_MISO 2
#define SD_MOSI 4
#define SD_SCK 14


// ==== General GPIO ==== 
#define LED 27

#define BUZZER 12
#define BUZ_channel 0
#define BUZ_resolution 8


// labeled RXD but dones't receives serial datas on my model
#define BTN 10

// ==== Spe UARTs ====
// Spe mapping for SerialX1 because legacy Serial1 doesn't RX
// 5v protected SerialX

// legacy RX 1 doesn't work
#define RX1 16
#define TX1 9
// static HardwareSerial SerialX1(1);

// connected to XtsTiLink ProMicro 5v
#define TXx 33
#define RXx 32
// static HardwareSerial SerialX(2); // 5v protected


// ==== Functions =====

bool setupHardware();

void buzz_tone(int freq);
void buzz_noTone();
void beep(int freq, int duration);

void led(bool on);


File sd_openFile(char* filename, bool read=true, bool append=false);
bool sd_exists(char* filename);

void tft_cls();