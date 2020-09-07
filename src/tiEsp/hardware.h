/**
 * XtsTiESP - hardware.h
 * 
 * Xtase - fgalliat @Sep 2020
 * 
 */

#include <Arduino.h>
#include "globals.h"

#define LED 27

#define BUZZER 12
#define BUZ_channel 0
#define BUZ_resolution 8


// labeled RXD but dones't receives serial datas on my model
#define BTN 10


bool setupHardware();

void buzz_tone(int freq);
void buzz_noTone();
void beep(int freq, int duration);

void led(bool on);
