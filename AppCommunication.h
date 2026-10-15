#ifndef APPCOMMUNICATION_H
#define APPCOMMUNICATION_H

#include <Arduino.h> 
#include <avr/wdt.h>
#include <SoftwareSerial.h>
#include "HexFun.h"
#include "config.h"

extern const char zapytanie_HF[zapytanie_HF_LENGTH];
extern const char zapytanie_UHF[UHF_MESSAGE_LENGTH];
extern bool stanUHF;
extern bool stan_HF;
extern bool stop_maszyny;
extern void resetValuesForConnectionTEstCheck();

void softwareReset();
void okResponse(Stream& comSerial);
void UHFinit180();

extern bool ringbufferUHFhistoric[64];
void sendBufferToPC(Stream& comSerial); 

void handleSerialPortCommunication(Stream& comSerial, HardwareSerial& readingSerial_PC);
void debugCommunication(HardwareSerial& comSerial);

void BufferPart_1(const uint8_t* ordered);
void BufferPart_2(const uint8_t* ordered);
void BufferPart_3(Stream& comSerial, const uint8_t* ordered);

#endif