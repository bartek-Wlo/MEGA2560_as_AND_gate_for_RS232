#ifndef HEXFUN_H
#define HEXFUN_H

#include <Arduino.h>
#include <SoftwareSerial.h>
#include "config.h"

extern unsigned int Distance;  /* Liczba anten od czujnika UHF do HF włącznie z antenami pod tymi czujnikami */
extern SoftwareSerial Serial_4;
extern bool workAsSnifer; // else work as AND gate

struct serialData {
  unsigned mesLength = 0;
  unsigned long mesTime = 0;
  uint8_t buffer[BUFFER_SIZE];
};



byte hexToByte(char hexChar);

/* Do wysyłania tekstu zawierającego oryginalną wartość do debug */
void printHex(byte data);

/* Do wysyłania poleceń w HEX do aplikacji */
void sendHexByte(Stream &port, uint8_t data);
void sendHexBuffer(Stream &port, const uint8_t *buffer, size_t length);
void reportToControlApplication(Stream &port, uint8_t* response);


//  CRC-16/MCRF4XX
uint16_t calculate_crc16_mcrf4xx(uint8_t* data, size_t length);
void append_crc16_to_array(uint8_t tab[], size_t dataLength);

#endif