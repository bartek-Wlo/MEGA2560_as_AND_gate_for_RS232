#include "SNIF.h"
extern SoftwareSerial Serial_4;
static bool workAsSnifer = false; // else work as AND gate, Tryb pracy programu
static bool allowWrite = false; // Gdy true - dane przesłane do Serial0 zostają wysłane przez MEGA2560 do wszystich pozostałych Serial 1-4
const byte turnToSnif[4] = {0x55, 0x33, 0x0F, 0xCC}; // SNIF
const byte turnToAndG[4] = {0x55, 0x33, 0xF0, 0xCC}; // AND
const byte writeModON[4] = {0x55, 0x33, 0xEE, 0xCC}; // Włącza allowWrite

 
void handleSerialPort_HF(Stream& readingSerial, Stream& writingSerial, serialData& sd, const char* SerialNum) {
  if (readingSerial.available()) {
    if ((  sd.mesLength!=0  )&&(  millis()-sd.mesTime > FRAME_TIMEOUT  )) {
      sd.mesLength = 0;
      Serial.print("\nHF Serial "); Serial.print(SerialNum); Serial.print(" TIME OUT !");
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
  if (readingSerial.available()) {
    if((  sd.mesLength!=0  )&&(  millis()-sd.mesTime > FRAME_TIMEOUT  )) {
      Serial.print("\nUHF Serial "); Serial.print(SerialNum); Serial.print(" TIME OUT ! ");
      Serial.print(millis()-sd.mesTime); Serial.print(" [ms]: "); for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  
    if(sd.mesLength < UHF_MESSAGE_LENGTH) {
      if(sd.mesLength == 0) sd.mesTime = millis();
      sd.buffer[sd.mesLength++] = readingSerial.read();
    } 
    if(sd.mesLength >= UHF_MESSAGE_LENGTH) {
      writingSerial.write(sd.buffer, sd.mesLength);
      Serial.print("\n"); Serial.print(SerialNum); Serial.print(": ");
      for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  }
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
        if (memcmp(ordered, turnToSnif, 4) == 0) {
          allowWrite=false;  workAsSnifer=true;
          Serial.println("\n--- MEGA2560 ---------------  SNIFER MODE  ---");
        } else if (memcmp(ordered, turnToAndG, 4) == 0) {
          allowWrite=false;  workAsSnifer=false;
          Serial.println("\n--- MEGA2560 --------------- AND gate MODE ---");
        } else if (memcmp(ordered, writeModON, 4) == 0) {
          allowWrite = !allowWrite;
          if(allowWrite) Serial.println("\n--- MEGA2560 --------------- Write MODE ON ---");
          else Serial.println("\n--- MEGA2560 --------------- Write MODE OFF---");
        }
      }
    } //else  if (hexCharTem == '\n' || hexCharTem == '\r') 55 33 CC 0F/F0
  }
}
