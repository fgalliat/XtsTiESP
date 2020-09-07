/**
 * XtsTiESP - hardware.h
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */

#include <Arduino.h>
#include "globals.h"

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