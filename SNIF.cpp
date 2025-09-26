#include "SNIF.h"
extern SoftwareSerial Serial_4;
static bool workAsSnifer = false; // else work as AND gate, Tryb pracy programu
static bool allowWrite = false; // Gdy true - dane przesłane do Serial0 zostają wysłane przez MEGA2560 do wszystich pozostałych Serial 1-4
static bool distanceIsDeclaring = false;
const byte turnToSnif[4] = {0x55, 0x33, 0x0F, 0xCC}; // SNIF
const byte turnToAndG[4] = {0x55, 0x33, 0xF0, 0xCC}; // AND
const byte writeModON[4] = {0x55, 0x33, 0xEE, 0xCC}; // Włącza allowWrite
const byte DecDistance[4] = {0x55, 0x33, 0xAA, 0xCC}; // Deklarowanie odległości
const byte stopDeclaring[4] = {0x55, 0x33, 0xFF, 0xCC}; // Koniec deklarowania

 
void handleSerialPort_HF(Stream& readingSerial, Stream& writingSerial, serialData& sd, const char* SerialNum) {
  if (readingSerial.available()) {
    if ((  sd.mesLength!=0  )&&(  millis()-sd.mesTime > FRAME_TIMEOUT  )) {
      Serial.print("\nHF Serial "); Serial.print(SerialNum); Serial.print(": TIME OUT !");
      Serial.print(millis()-sd.mesTime); Serial.print(" [ms] Message length: "); printHex(sd.buffer[0]);
      sd.mesLength = 0;
    }
    if (sd.mesLength == 0) {
      sd.mesLength = readingSerial.read();
      if (sd.mesLength >= BUFFER_SIZE) sd.mesLength = BUFFER_SIZE - 1;
      sd.buffer[0] = sd.mesLength;
      sd.mesTime = millis();
    }
    if ((  sd.mesLength!=0  )&&(  readingSerial.available() >= sd.mesLength-1  )) {
      readingSerial.readBytes(sd.buffer +1, sd.mesLength -1);
      writingSerial.write(sd.buffer, sd.mesLength);
      Serial.print("\n"); Serial.print(SerialNum); Serial.print(": ");
      for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  }
}

void handleSerialPort_UHF(Stream& readingSerial, Stream& writingSerial, serialData& sd, const char* SerialNum) {
  static const char UHFinit0[UHF_MESSAGE_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  if (readingSerial.available()) {
    if((  sd.mesLength!=0  )&&(  millis()-sd.mesTime > FRAME_UHF_TIMEOUT  )) {
      if(memcmp(sd.buffer, UHFinit0, sd.mesLength) == 0) writingSerial.write(UHFinit0, UHF_MESSAGE_LENGTH);
      Serial.print("\nUHF Serial "); Serial.print(SerialNum); Serial.print(": TIME OUT ! ");
      Serial.print(millis()-sd.mesTime); Serial.print(" [ms]: "); for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  
    if(sd.mesLength < UHF_MESSAGE_LENGTH) {
      sd.mesTime = millis();
      sd.buffer[sd.mesLength++] = readingSerial.read();
    } 
    if(sd.mesLength >= UHF_MESSAGE_LENGTH) send_to_UHF(readingSerial, writingSerial, sd, SerialNum);
  }
}

void send_to_UHF(Stream& readingSerial, Stream& writingSerial, serialData& sd, const char* SerialNum) {
  writingSerial.write(sd.buffer, sd.mesLength);
  Serial.print("\n"); Serial.print(SerialNum); Serial.print(": ");
  for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
  sd.mesLength = 0;
}


void handleSerialPortCommunication(HardwareSerial& comSerial) {
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
