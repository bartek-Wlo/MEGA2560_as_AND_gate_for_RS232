#include "HexFun.h"

byte hexToByte(char hexChar) {
    if (hexChar >= '0' && hexChar <= '9') return hexChar - '0';
    if (hexChar >= 'A' && hexChar <= 'F') return hexChar - 'A' + 10;
    if (hexChar >= 'a' && hexChar <= 'f') return hexChar - 'a' + 10;
    return 0;
}





/* Do wysyłania tekstu zawierającego oryginalną wartość do debug */
#ifdef DEBUG_DISPLAY
  void printHex(byte data) {
      if (data < 0x10) Serial.print("0");
      Serial.print(data, HEX);
      Serial.print(" ");
  }
#else
  void printHex(byte data) { }
#endif


/* Do wysyłania poleceń w HEX do aplikacji */
void sendHexByte(Stream &port, uint8_t data) {
    port.write(data);
} // sendHexByte(AppSerial, 0x3A);

void sendHexBuffer(Stream &port, const uint8_t *buffer, size_t length) {
    port.write(buffer, length);
} // sendHexBuffer(AppSerial, sd.buffer, sd.mesLength);

void reportToControlApplication(Stream &port, uint8_t* response) {
  static uint8_t length = 4;
  append_crc16_to_array(response, length);
  port.write(0x55);
  port.write(response, length + 2);
  port.write(0xCC);
}







//  CRC-16/MCRF4XX
uint16_t calculate_crc16_mcrf4xx(uint8_t* data, size_t length) {
    uint16_t crc = 0xFFFF; // Wartość początkowa

    for (size_t i = 0; i < length; ++i) {
        crc ^= data[i]; 
        for (int j = 0; j < 8; ++j) {
            if (crc & 0x0001) {
                crc = (crc >> 1) ^ 0x8408; // Wielomian 0x1021 w wersji odbitej
            } else {
                crc >>= 1;
            }
        }
    }
    return crc;
}


/**
 * Funkcja dodająca CRC na koniec tablicy
 * @param tab - tablica z danymi (musi mieć miejsce na 2 dodatkowe bajty!)
 * @param dataLength - długość samych danych przed dodaniem CRC
 */
void append_crc16_to_array(uint8_t tab[], size_t dataLength) {
    uint16_t crc = calculate_crc16_mcrf4xx(tab, dataLength);

    // MCRF4XX wysyła LSB pierwszy
    tab[dataLength]     = (uint8_t)(crc & 0xFF);         // LSB
    tab[dataLength + 1] = (uint8_t)((crc >> 8) & 0xFF);  // MSB
}
