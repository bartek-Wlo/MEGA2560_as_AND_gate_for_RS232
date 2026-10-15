#ifndef AND_H
#define AND_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "HexFun.h"
#include "config.h"

extern const char zapytanie_HF[zapytanie_HF_LENGTH];
extern const char zapytanie_UHF[UHF_MESSAGE_LENGTH];
extern bool stanUHF;
extern bool stan_HF;
extern bool stop_maszyny;
void resetValuesForConnectionTEstCheck();

void handleSerialPort_HF_CZUJNIK( HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum);
void decideForCompleteMessage_HF(HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum);
// void BAD__HF_CZUJNIK(HardwareSerial& readingSerial);

void handleSerialPort_UHF_CZUJNIK(HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum);
void BAD_UHF_CZUJNIK(HardwareSerial& readingSerial);

void handleSerialPort_HF_PC(HardwareSerial& readingSerial, HardwareSerial& writing_UHF_Serial, HardwareSerial& writing_HF_Serial, serialData& sd, const char* SerialNum);

void czujnikiANDgate(HardwareSerial& writingSerial, HardwareSerial& writing_HF_Serial,  Stream &AppSerial);
void printRingBuffer(bool* buffer, unsigned int size, unsigned int currentPos, unsigned int distance);

#endif