#ifndef AND_H
#define AND_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "HexFun.h"

extern unsigned int LiczbaOdpowiedziUHF; // Liczba Odpowiedzi Od wysłania zapytania o antene.
extern bool inicjalizacja;

void handleSerialPort_HF_CZUJNIK(Stream& readingSerial, Stream& writingSerial, serialData& sd, const char* SerialNum);
void handleSerialPort_UHF_CZUJNIK(Stream& readingSerial, Stream& writingSerial, serialData& sd, const char* SerialNum);
void handleSerialPort_HF_PC(Stream& readingSerial, Stream& writingSerial, serialData& sd, const char* SerialNum);
void UHF_init(Stream& readingSerial, Stream& writingSerial, serialData& sd);

#endif