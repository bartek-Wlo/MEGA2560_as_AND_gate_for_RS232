#ifndef CONFIG_H
#define CONFIG_H

// #define DEBUG_DISPLAY
// #define CRC16_MCRF4XX_TEST


#define BUFFER_SIZE 128
#define FRAME_TIMEOUT 100     // [ms]
#define FRAME_UHF_TIMEOUT 20  // [ms]
#define UHF_MESSAGE_LENGTH 8  // Oczekiwana długość wiadomości UHF w [bajtach]
#define zapytanie_HF_LENGTH 7 // Długość zapytania wiadomości HF 0x07
#define DEFAULT_DISTANCE 6    // Liczba anten pomiędzy czyjnikami + 2  będące pod tymi antenami

#define BOUND_UHF 115200  
#define BOUND_HF 38400 


#ifdef DEBUG_DISPLAY
  #define DEBUG_PRINT(x)    Serial.print(x)
  #define DEBUG_PRINTLN(x)  Serial.println(x)
#else
  #define DEBUG_PRINT(x)
  #define DEBUG_PRINTLN(x)
#endif

extern HardwareSerial& PC_USB;
extern HardwareSerial& HF_Sensor;
extern HardwareSerial& HF_PC;
extern HardwareSerial& UHF_Sensor;
extern SoftwareSerial& UHF_PC;
extern SoftwareSerial& AppPort;

#endif



  // #ifdef CRC16_MCRF4XX_TEST
  //   uint8_t message[17] = {0x11, 0x00, 0xB0, 0x00, 0x01, 0x03, 0x00, 0xE0, 0x16, 0x80, 0xFF, 0x08, 0x00, 0x4F, 0xBC, 0x00, 0x00}; 
  //   size_t dataSize = 15;
  //   append_crc16_to_array(message, dataSize);
  //   Serial.print("Wiadomosc + CRC-16/MCRF4XX: ");                  /*Test poprawnie działajacego CRC-16/MCRF4XX*/
  //   for (size_t i = 0; i < dataSize + 2; ++i) {
  //     printHex(message[i]);
  //   }
  // #endif