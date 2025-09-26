#ifndef HEXFUN_H
#define HEXFUN_H

#include <Arduino.h>

#define BUFFER_SIZE 128
#define FRAME_TIMEOUT 100    // [ms]
#define FRAME_UHF_TIMEOUT 10 // [ms]
#define UHF_MESSAGE_LENGTH 8 // Oczekiwana długość wiadomości UHF w [bajtach]
inline static unsigned int Distance = 8;

struct serialData {
  unsigned mesLength = 0;
  unsigned long mesTime = 0;
  unsigned char buffer[BUFFER_SIZE];
};

inline byte hexToByte(char hexChar) {
    if (hexChar >= '0' && hexChar <= '9') return hexChar - '0';
    if (hexChar >= 'A' && hexChar <= 'F') return hexChar - 'A' + 10;
    if (hexChar >= 'a' && hexChar <= 'f') return hexChar - 'a' + 10;
    return 0;
}

inline void printHex(byte data) {
    if (data < 0x10) Serial.print("0");
    Serial.print(data, HEX);
    Serial.print(" ");
}

#endif