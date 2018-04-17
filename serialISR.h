#ifndef __SERIALISR_H
#define __SERIALISR_H
#define VALIDDATA 1
#include "Arduino.h"
#include "kinetis.h"
#include "core_pins.h"
#include "HardwareSerial.h"

void resetFailSafe();
void returnHeartBeat();
void prepareReceivingPackage();
void completeReceivingPackage();
void usartInit();
void sendChar(unsigned char c);
void sendBuffer(unsigned char* p, unsigned int length);
void sendStr(const char* p);
void sendStrWithNewLine(const char* p);
void sendNewLine();
void sendNum(unsigned int num);
void wifly_processer();

#endif
