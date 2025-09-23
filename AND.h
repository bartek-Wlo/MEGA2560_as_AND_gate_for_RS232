#ifndef AND_H
#define AND_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "HexFun.h"

extern unsigned int LiczbaOdpowiedziUHF; // Liczba Odpowiedzi Od wysłania zapytania o antene.
extern bool inicjalizacja;

void handleSerialPort_HF_CZUJNIK( HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum);
void handleSerialPort_UHF_CZUJNIK(HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum);
void handleSerialPort_HF_PC(HardwareSerial& readingSerial, HardwareSerial& writing_UHF_Serial, HardwareSerial& writing_HF_Serial, serialData& sd, const char* SerialNum);
void UHF_init(HardwareSerial& readingSerial, HardwareSerial& writing_UHF_Serial, HardwareSerial& writing_HF_Serial, serialData& sd);

#endif