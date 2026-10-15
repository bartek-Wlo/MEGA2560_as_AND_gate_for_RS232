#include "Arduino.h"
#include "AppCommunication.h"


bool stanUHF = false;
bool stan_HF = false;
bool stop_maszyny = false;

void softwareReset() {
  void(* resetToZero) (void) = 0;
  DEBUG_PRINTLN(F("Soft Reset..."));
  Serial.flush(); 
  delay(100);
  noInterrupts(); 
  resetToZero(); 
}

void okResponse(Stream& comSerial) {
  static uint8_t frame[8] = {0x55, 0x0F, 0xAA, 0x00, 0x00, 0x75, 0xCD, 0xCC};
  comSerial.write(frame, 8);
}

void UHFinit180() {
  static const char UHFinit[34][UHF_MESSAGE_LENGTH] = {
    {0x01, 0x00, 0x00, 0xF0, 0x18, 0x00, 0x00, 0x00}, // A
    {0x01, 0x00, 0x00, 0x07, 0x01, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x01, 0x07, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x02, 0x07, 0x01, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x05, 0x07, 0x64, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00},

    {0x01, 0x00, 0x00, 0xF0, 0x18, 0x00, 0x00, 0x00}, // B
    {0x01, 0x00, 0x0A, 0x01, 0x4C, 0x33, 0x1D, 0x00}, // 0xC4, 0x38
    {0x01, 0x00, 0x08, 0x01, 0x01, 0x01, 0x00, 0x00},
    {0x01, 0x00, 0x00, 0xF0, 0x27, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x00, 0xF0, 0x18, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x14, 0x01, 0xB4, 0x00, 0x00, 0x00}, // 0x64
    {0x01, 0x00, 0x06, 0x07, 0xB4, 0x00, 0x00, 0x00}, // 0x64 = 100, B4 = 180
    {0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00},

    {0x01, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00}, // C
    {0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x00, 0x09, 0xC0, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x01, 0x09, 0x40, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x11, 0x09, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x02, 0x09, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x03, 0x09, 0xF4, 0x40, 0x00, 0x00},
    {0x01, 0x00, 0x05, 0x09, 0x01, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00},

    {0x01, 0x00, 0x01, 0x0A, 0x06, 0x00, 0x00, 0x00}, // D
    {0x01, 0x00, 0x02, 0x0A, 0x01, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x03, 0x0A, 0x02, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x04, 0x0A, 0x06, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x05, 0x0A, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x06, 0x0A, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x07, 0x0A, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x08, 0x0A, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00}
  };
  for(int i=0; i<8; ++i) { UHF_Sensor.write((uint8_t)0x00);  delay(100); }
  for(int i=0; i<8; ++i) { UHF_Sensor.write(UHFinit[i], UHF_MESSAGE_LENGTH); delay(25);} delay(100);
  for(int i=8; i<16; ++i) { UHF_Sensor.write(UHFinit[i], UHF_MESSAGE_LENGTH); delay(25);} delay(100);
  for(int i=16; i<25; ++i) { UHF_Sensor.write(UHFinit[i], UHF_MESSAGE_LENGTH); delay(25);} delay(100);
  for(int i=25; i<34; ++i) { UHF_Sensor.write(UHFinit[i], UHF_MESSAGE_LENGTH); delay(25);} delay(100);

  static uint8_t frame[8] = {0x55, 0x0F, 0xEE, 0x00, 0x00, 0x62, 0xA8, 0xCC};
  AppPort.write(frame, 8);
}

void sendBufferToPC(Stream& comSerial) {
  uint8_t packedBytes[8] = {0};

  for (int i = 0; i < 64; i++) {
    if (ringbufferUHFhistoric[i] == 1) {
      packedBytes[i / 8] |= (1 << (i % 8));
    }
  }

  uint16_t crc;
  uint8_t frame[8];

  // 1. Ramka Dystansu (55 BB [D] [D] [D] CRC CC)
  frame[0] = 0x55; frame[1] = 0xBB;
  frame[2] = (uint8_t)Distance;
  frame[3] = (uint8_t)Distance;
  frame[4] = (uint8_t)Distance;
  crc = calculate_crc16_mcrf4xx(&frame[1], 4); // CRC z bajtów 1-4
  frame[5] = crc & 0xFF; frame[6] = crc >> 8; frame[7] = 0xCC;
  comSerial.write(frame, 8);
  delay(10); // Małe opóźnienie dla stabilności

  // 2. Ramka Bufora 1 (55 42 [B0] [B1] [B2] CRC CC)
  frame[1] = 0x42;
  frame[2] = packedBytes[0]; frame[3] = packedBytes[1]; frame[4] = packedBytes[2];
  crc = calculate_crc16_mcrf4xx(&frame[1], 4);
  frame[5] = crc & 0xFF; frame[6] = crc >> 8; frame[7] = 0xCC;
  comSerial.write(frame, 8);
  delay(10);

  // 3. Ramka Bufora 2 (55 DB [B3] [B4] [B5] CRC CC)
  frame[1] = 0xDB;
  frame[2] = packedBytes[3]; frame[3] = packedBytes[4]; frame[4] = packedBytes[5];
  crc = calculate_crc16_mcrf4xx(&frame[1], 4);
  frame[5] = crc & 0xFF; frame[6] = crc >> 8; frame[7] = 0xCC;
  comSerial.write(frame, 8);
  delay(10);

  // 4. Ramka Bufora 3 (Finał) (55 33 [B6] [B7] [Dist] CRC CC)
  frame[1] = 0x33;
  frame[2] = packedBytes[6]; frame[3] = packedBytes[7]; frame[4] = (uint8_t)Distance;
  crc = calculate_crc16_mcrf4xx(&frame[1], 4);
  frame[5] = crc & 0xFF; frame[6] = crc >> 8; frame[7] = 0xCC;
  comSerial.write(frame, 8);
}


static uint8_t tempRecvBuffer[8];
static bool gotPart1 = false;
static bool gotPart2 = false;

void handleSerialPortCommunication(Stream& comSerial, HardwareSerial& readingSerial_PC) {
  static bool msgUHF = false, msgHF = false, msgPCUHF = false, msgPCHF = false;
  static bool distanceIsDeclaring = false;

  static const int COM_ERROR_LENGTH = 7;
  static const char error_comunication[COM_ERROR_LENGTH] = {0x07, 0x00, 0xB0, 0x83, 0x00, 0xB2, 0x2C};
  
  static const byte ANSWER_OK_BUFFOR_RECEIVE[8] = {0x55, 0x0F, 0x42, 0x00, 0x00, 0x16, 0x02, 0xCC};
  static const byte ANSWER_COMUNICATION[8]      = {0x55, 0x0F, 0x33, 0xAA, 0xAA, 0xCD, 0x80, 0xCC};
  static const byte CMD_RESPONCE_CHECK[8] = {0x55, 0x77, 0x77, 0x77, 0x77, 0x6C, 0xB8, 0xCC};
  static const byte CMD_STOP[8]           = {0x55, 0x22, 0x22, 0x22, 0x22, 0x14, 0x11, 0xCC};
  static const byte CMD_SNIFFER[8]        = {0x55, 0x0F, 0x0F, 0x0F, 0x0F, 0x20, 0x80, 0xCC};
  static const byte CMD_AND_GATE[8]       = {0x55, 0xF0, 0xF0, 0xF0, 0xF0, 0xB9, 0x73, 0xCC};

  static const byte CMD_UHF_ON[8]         = {0x55, 0xEE, 0x0F, 0xFF, 0xFF, 0x96, 0xA3, 0xCC};
  static const byte CMD_UHF_OFF[8]        = {0x55, 0xEE, 0x0F, 0x00, 0x00, 0x2E, 0x53, 0xCC};
  static const byte CMD_HF_ON[8]          = {0x55, 0xEE, 0x00, 0xFF, 0xFF, 0x51, 0xE9, 0xCC};
  static const byte CMD_HF_OFF[8]         = {0x55, 0xEE, 0x00, 0x00, 0x00, 0xE9, 0x19, 0xCC};
  static const byte CMD_PCUHF_ON[8]       = {0x55, 0xEE, 0xFF, 0xFF, 0xFF, 0xA2, 0x2F, 0xCC};
  static const byte CMD_PCUHF_OFF[8]      = {0x55, 0xEE, 0xFF, 0x00, 0x00, 0x1A, 0xDF, 0xCC};
  static const byte CMD_PCHF_ON[8]        = {0x55, 0xEE, 0xF0, 0xFF, 0xFF, 0x65, 0x65, 0xCC};
  static const byte CMD_PCHF_OFF[8]       = {0x55, 0xEE, 0xF0, 0x00, 0x00, 0xDD, 0x95, 0xCC};

  static const byte CMD_INIT_PC[8]        = {0x55, 0x66, 0x0F, 0x0F, 0x0F, 0xA7, 0xE0, 0xCC};
  static const byte CMD_INIT_MEGA[8]      = {0x55, 0x66, 0xF0, 0xF0, 0xF0, 0xEC, 0xD6, 0xCC};
  static const byte CMD_RST_UHF[8]        = {0x55, 0x66, 0xAA, 0xAA, 0xAA, 0x2D, 0xF5, 0xCC};
  static const byte CMD_RST_SNIF[8]       = {0x55, 0x66, 0x33, 0x33, 0x33, 0x47, 0x27, 0xCC};
  
  static byte buffer[8] = {0x00}; 
  static byte ordered[8]; 
  static int pos = 0; 

  while (comSerial.available()) {
    byte value = comSerial.read();

    if(msgUHF)   Serial3.write(value);
    if(msgHF)    Serial1.write(value);
    if(msgPCUHF) Serial_4.write(value);
    if(msgPCHF)  Serial2.write(value);
    Serial.write(value);

    buffer[pos] = value;
    pos = (pos + 1) & 7; // Modulo 8

    // SZYBKI FILTR: Sprawdzamy czy najstarszy to: 0x55) i najnowszy to: 0xCC 
    if (buffer[pos] != 0x55 || value != 0xCC) { continue; }

    for (int i = 0; i < 8; ++i) { ordered[i] = buffer[(pos + i) & 7]; }


    uint16_t calcCRC = calculate_crc16_mcrf4xx(&ordered[1], 4);  /* CRC CHECK */
    uint16_t recvCRC = ordered[5] | (ordered[6] << 8);
    if (calcCRC != recvCRC) { DEBUG_PRINTLN(F("CRC Error in AppCommunication")); continue; }

    switch (ordered[1]) {
      case 0x22:
          if      (memcmp(ordered + 2, CMD_STOP + 2, 5) == 0)      { msgUHF=true;    DEBUG_PRINTLN(F("\n-> MSG STOP!!"));  readingSerial_PC.write(error_comunication,  COM_ERROR_LENGTH);  stop_maszyny=true;}
          break;
      case 0xEE:
          if      (memcmp(ordered + 2, CMD_UHF_ON + 2, 5) == 0)    { msgUHF=true;    DEBUG_PRINTLN(F("\n-> MSG UHF: ON")); }
          else if (memcmp(ordered + 2, CMD_UHF_OFF + 2, 5) == 0)   { msgUHF=false;   DEBUG_PRINTLN(F("\n-> MSG UHF: OFF")); }
          else if (memcmp(ordered + 2, CMD_HF_ON + 2, 5) == 0)     { msgHF=true;     DEBUG_PRINTLN(F("\n-> MSG HF: ON")); }
          else if (memcmp(ordered + 2, CMD_HF_OFF + 2, 5) == 0)    { msgHF=false;    DEBUG_PRINTLN(F("\n-> MSG HF: OFF")); }
          else if (memcmp(ordered + 2, CMD_PCUHF_ON + 2, 5) == 0)  { msgPCUHF=true;  DEBUG_PRINTLN(F("\n-> MSG PC-UHF: ON")); }
          else if (memcmp(ordered + 2, CMD_PCUHF_OFF + 2, 5) == 0) { msgPCUHF=false; DEBUG_PRINTLN(F("\n-> MSG PC-UHF: OFF")); }
          else if (memcmp(ordered + 2, CMD_PCHF_ON + 2, 5) == 0)   { msgPCHF=true;   DEBUG_PRINTLN(F("\n-> MSG PC-HF: ON")); }
          else if (memcmp(ordered + 2, CMD_PCHF_OFF + 2, 5) == 0)  { msgPCHF=false;  DEBUG_PRINTLN(F("\n-> MSG PC-HF: OFF")); }
          break;
      case 0x66:
          if      (memcmp(ordered + 2, CMD_INIT_PC + 2, 5) == 0)   { DEBUG_PRINTLN(F("\n-> INIT PC->MEGA"));   gotPart1 = false; gotPart2 = false; comSerial.write(ANSWER_OK_BUFFOR_RECEIVE, 8);}
          else if (memcmp(ordered + 2, CMD_INIT_MEGA + 2, 5) == 0) { DEBUG_PRINTLN(F("\n-> INIT MEGA->PC"));   sendBufferToPC(comSerial);}
          else if (memcmp(ordered + 2, CMD_RST_UHF + 2, 5) == 0)   { DEBUG_PRINTLN(F("\n-> RESTART UHF"));     UHFinit180();}
          else if (memcmp(ordered + 2, CMD_RST_SNIF + 2, 5) == 0)  { DEBUG_PRINTLN(F("\n-> RESTART SNIFFER")); softwareReset();}
          break;
      case 0x0F: // SNIFFER
          if (memcmp(ordered + 2, CMD_SNIFFER + 2, 5) == 0) {
              workAsSnifer = true;
              DEBUG_PRINTLN(F("\n--- MODE: SNIFFER ---"));
          }
          break;
      case 0xF0: // AND GATE
          if (memcmp(ordered + 2, CMD_AND_GATE + 2, 5) == 0) {
              workAsSnifer = false;
              DEBUG_PRINTLN(F("\n--- MODE: AND GATE ---"));
          }
          break;
      case 0xAA:
          // Format: 55 AA [VAL] [VAL] [VAL] [CRC_L] [CRC_H] CC
          // Indeksy: 0  1    2     3     4      5       6     7
          if (ordered[2] == ordered[3] && ordered[3] == ordered[4]) {
             Distance = ordered[2];
             DEBUG_PRINT(F("\n-> SET DISTANCE: ")); DEBUG_PRINTLN(Distance);
          }
          break;
      case 0x77:
          if(ordered[2]==0x77 && ordered[3]==0x77 && ordered[4]==0x77) {
            comSerial.write(ANSWER_COMUNICATION, 8);
            
            uint8_t frame[8]; 
            frame[0] = 0x55; 
            frame[1] = 0x0F; 
            frame[2] = 0x33;
            frame[3] = stanUHF ? 0x00 : 0xFF;
            frame[4] = stan_HF  ? 0x00 : 0xFF;
            uint16_t crc = calculate_crc16_mcrf4xx(&frame[1], 4);
            frame[5] = (uint8_t)(crc & 0xFF);
            frame[6] = (uint8_t)((crc >> 8) & 0xFF);
            frame[7] = 0xCC;
            DEBUG_PRINT(F("\n-> Stan połączenia: UHF - ")); DEBUG_PRINT(stanUHF?"Good":"Bad"); DEBUG_PRINT(" = "); DEBUG_PRINT(frame[3]); DEBUG_PRINT(F(" HF - ")); DEBUG_PRINT(stan_HF?"Good":"Bad"); DEBUG_PRINT(" = "); DEBUG_PRINTLN(frame[4]);
            comSerial.write(frame, 8);

            stanUHF = false;
            stan_HF = false;
            HF_Sensor.write(zapytanie_HF, zapytanie_HF_LENGTH);
            UHF_Sensor.write(zapytanie_UHF, UHF_MESSAGE_LENGTH);
          }
          break;
      case 0x42: // Buffer Part 1
          BufferPart_1(ordered);
          break;
      case 0xDB: // Buffer Part 2
          BufferPart_2(ordered);
          break;
      case 0x33: // Buffer Part 3 + Finalize
          BufferPart_3(comSerial, ordered);
          break;
    }
  }
}


void BufferPart_1(const uint8_t* ordered) {
  tempRecvBuffer[0] = ordered[2];
  tempRecvBuffer[1] = ordered[3];
  tempRecvBuffer[2] = ordered[4];
  gotPart1 = true;
  DEBUG_PRINTLN(F("-> Got Buff Part 1"));
}
void BufferPart_2(const uint8_t* ordered) {
  tempRecvBuffer[3] = ordered[2];
  tempRecvBuffer[4] = ordered[3];
  tempRecvBuffer[5] = ordered[4];
  gotPart2 = true;
  DEBUG_PRINTLN(F("-> Got Buff Part 2"));
}
void BufferPart_3(Stream& comSerial, const uint8_t* ordered) {
  if (gotPart1 && gotPart2) {
      if (ordered[4] == (uint8_t)Distance) {  // czy dystans w ramce zgadza się z globalnym
          tempRecvBuffer[6] = ordered[2];
          tempRecvBuffer[7] = ordered[3];
          
          // ROZPAKOWANIE BITÓW DO BUFORA PRODUKCYJNEGO
          for (int i = 0; i < 64; i++) {
              uint8_t currentByte = tempRecvBuffer[i / 8];
              bool isSet = (currentByte & (1 << (i % 8))) != 0;
              ringbufferUHFhistoric[i] = isSet ? 1 : 0;  // Jeśli 1 -> Good, 0 -> Bad
          }

          DEBUG_PRINTLN(F("-> Buffer LOADED Successfully!"));
          
          // Potwierdzenie sukcesu do PC
          uint8_t resp[] = {0x55, 0x0F, 0x42, 0x00, 0x00, 0x00, 0x00, 0xCC};
          uint16_t c = calculate_crc16_mcrf4xx(&resp[1], 4);
          resp[5] = c & 0xFF; resp[6] = c >> 8;
          comSerial.write(resp, 8);

      } else {
          DEBUG_PRINTLN(F("-> Buffer Load ERROR: Distance Mismatch"));
          // Potwierdzenie błędu (FF FF)
          uint8_t resp[] = {0x55, 0x0F, 0x42, 0xFF, 0xFF, 0x00, 0x00, 0xCC};
          uint16_t c = calculate_crc16_mcrf4xx(&resp[1], 4);
          resp[5] = c & 0xFF; resp[6] = c >> 8;
          comSerial.write(resp, 8);
      }
  }
  gotPart1 = false; // Reset
  gotPart2 = false;
}


















void debugCommunication(HardwareSerial& comSerial) {
static bool allowWrite = false; // Gdy true - dane przesłane do Serial0 zostają wysłane przez MEGA2560 do wszystich pozostałych Serial 1-4
static bool distanceIsDeclaring = false;
const byte turnToSnif[4] = {0x55, 0x33, 0x0F, 0xCC}; // SNIF
const byte turnToAndG[4] = {0x55, 0x33, 0xF0, 0xCC}; // AND
const byte writeModON[4] = {0x55, 0x33, 0xEE, 0xCC}; // Włącza allowWrite
const byte DecDistance[4] = {0x55, 0x33, 0xAA, 0xCC}; // Deklarowanie odległości
const byte stopDeclaring[4] = {0x55, 0x33, 0xFF, 0xCC}; // Koniec deklarowania
const byte restartMEGA[4] = {0x55, 0x33, 0x00, 0xCC}; // Koniec deklarowania

  static byte buffer[4] = {0x00}, ordered[4]; static int pos = 0; /*ring buffer*/
  static bool FirstChar = true;
  static char hexChar1, hexChar2;
  if (comSerial.available()) {
    char hexCharTem = comSerial.read();
    if(isHexadecimalDigit(hexCharTem)) {
      if(FirstChar) { 
        hexChar1 = hexCharTem; 
        FirstChar = false; 
      } else /*SecondChar*/ { 
        hexChar2 = hexCharTem; 
        FirstChar = true; 
      
        byte value = hexToByte(hexChar1) * 16 + hexToByte(hexChar2);  
        
        Serial.print("\n00: "); printHex(value);
        if(allowWrite) {
          Serial.print(" sent to all.");
          Serial1.write(value); 
          Serial2.write(value);
          Serial3.write(value);
          Serial_4.write(value);
        }

        buffer[pos] = value;
        pos = (pos + 1) % 4;
        for (int i = 0; i < 4; ++i) { ordered[i] = buffer[(pos + i) % 4]; }
        if(distanceIsDeclaring) {
          if(ordered[0]==ordered[1] && ordered[0]==ordered[2] && ordered[0]==ordered[3]) Distance = ordered[0];
          else if (memcmp(ordered, stopDeclaring, 4) == 0)  {
            distanceIsDeclaring = false; Serial.print("\n--- MEGA2560 --------------- STOP Declaring Distance --- D = "); Serial.println(Distance);
          }
        } else if (memcmp(ordered, turnToSnif, 4) == 0) {
          allowWrite=false;  workAsSnifer=true;
          Serial.println("\n--- MEGA2560 ---------------  SNIFER MODE  ---");
        } else if (memcmp(ordered, turnToAndG, 4) == 0) {
          allowWrite=false;  workAsSnifer=false;
          Serial.println("\n--- MEGA2560 --------------- AND gate MODE ---");
        } else if (memcmp(ordered, restartMEGA, 4) == 0) {
          Serial.println("\n!!! MEGA2560 --------------- RESTART !!!");
          softwareReset();
          Serial.println("\n!!! MEGA2560 --------------- RESTART !!!");
        } else if (memcmp(ordered, writeModON, 4) == 0) {
          allowWrite = !allowWrite;
          if(allowWrite) Serial.println("\n--- MEGA2560 --------------- Write MODE ON ---");
          else Serial.println("\n--- MEGA2560 --------------- Write MODE OFF---");
        } else if (memcmp(ordered, DecDistance, 4) == 0) { 
          distanceIsDeclaring = true; Serial.println("\n--- MEGA2560 --------------- Declaring Distance ---");
        }
      }
    } //else  if (hexCharTem == '\n' || hexCharTem == '\r')
  }
}






