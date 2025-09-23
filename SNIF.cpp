#include "SNIF.h"
extern SoftwareSerial Serial_4;
 
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
  static bool FirstChar = true;
  static char hexChar1, hexChar2;
  if (comSerial.available()) {
    char hexCharTem = comSerial.read();
    if(isHexadecimalDigit(hexCharTem)) {
      if(FirstChar) { 
        hexChar1 = hexCharTem; 
        FirstChar = false; comSerial.write(hexChar1);
      } else /*SecondChar*/ { 
        hexChar2 = hexCharTem; 
        FirstChar = true; comSerial.write(hexChar2);
      
        byte value = hexToByte(hexChar1) * 16 + hexToByte(hexChar2);  
        
        Serial1.write(value); 
        Serial2.write(value);
        Serial3.write(value);
        Serial_4.write(value);
      }
    } //else  if (hexCharTem == '\n' || hexCharTem == '\r')
  }
}
