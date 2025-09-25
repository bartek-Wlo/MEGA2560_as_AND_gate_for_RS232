#ifndef SNIF_H
#define SNIF_H

#include <Arduino.h> 
#include <SoftwareSerial.h>
#include "HexFun.h"
extern bool workAsSnifer; // else work as AND gate

void handleSerialPort_HF (Stream& readingSerial, Stream& writingSerial, serialData& sd, const char* SerialNum);
void handleSerialPort_UHF(Stream& readingSerial, Stream& writingSerial, serialData& sd, const char* SerialNum);
void send_to_UHF(Stream& readingSerial, Stream& writingSerial, serialData& sd, const char* SerialNum);
void handleSerialPortCommunication(HardwareSerial& comSerial);

#endif