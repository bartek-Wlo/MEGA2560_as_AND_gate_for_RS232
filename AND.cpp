#include "AND.h"

static unsigned int LiczbaOdpowiedziUHF = 0; // Liczba Odpowiedzi Od wysłania zapytania o antene.
static bool inicjalizacja = false;

void handleSerialPort_HF_CZUJNIK(HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum) {
}

void handleSerialPort_UHF_CZUJNIK(HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum) {
  static const int GoodResL = 17, badResL = 6;
  static const char GoodRes[GoodResL] = {0x11, 0x00, 0xB0, 0x00, 0x01, 0x03, 0x00, 0xE0, 0x16, 0x80, 0xFF, 0x08, 0x00, 0x50, 0x24, 0x80, 0x75};
  static const char ErrorRes[badResL] = {0x06, 0x00, 0xB0, 0x81, 0x54, 0xE7};
  static const char badRes[badResL] = {0x06, 0x00, 0xB0, 0x01, 0x5C, 0x63};
  if (readingSerial.available()) {
    if((  sd.mesLength!=0  )&&(  millis()-sd.mesTime > FRAME_TIMEOUT  )) {
    /*writingSerial.write(sd.buffer, sd.mesLength); JAK BY CZASEM JEDNAK BYŁY INNEJ DŁUGOŚCI NIŻ 8
      Serial.print("\n"); Serial.print(SerialNum); Serial.print(": ");
      for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}*/
      sd.mesLength = 0;
      Serial.print("\nUHF Serial "); Serial.print(SerialNum); Serial.print(" TIME OUT !");
      Serial.print(millis()); Serial.print("-"); Serial.print(sd.mesTime);
    }
  
    if(sd.mesLength < UHF_MESSAGE_LENGTH) {
      if(sd.mesLength == 0) sd.mesTime = millis();
      sd.buffer[sd.mesLength++] = readingSerial.read();
    } 
    if(sd.mesLength >= UHF_MESSAGE_LENGTH) {

      if(inicjalizacja==false) {
        ++LiczbaOdpowiedziUHF;  
        if(LiczbaOdpowiedziUHF == 3) {
          if(sd.buffer[1]==0x12)       {writingSerial.write(GoodRes, GoodResL);Serial.print("\nAntenaGood ");}
          else if (sd.buffer[1]==0x00) {writingSerial.write(badRes, badResL);  Serial.print("\n>>>>>>>>>>>>>>>>>>>>>>>>> AntenaBAD  ");}
        } else Serial.print("\n");
      } else Serial.print("\n");

      Serial.print(SerialNum); Serial.print(": ");
      for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  }
}


void handleSerialPort_HF_PC(HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum) {
  static const int zapytanie_HF_LENGTH = 7, zapytanie_UHF_LENGTH = 8;
  static const char zapytanie_HF[zapytanie_HF_LENGTH] = {0x07, 0xFF, 0xB0, 0x01, 0x00, 0x1C, 0x56};
  static const char zapytanie_UHF[zapytanie_UHF_LENGTH] = {0x01, 0x00, 0x00, 0xF0, 0x10, 0x00, 0x00, 0x00};
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

      if(memcmp(sd.buffer, zapytanie_HF, zapytanie_HF_LENGTH)==0) {writingSerial.write(zapytanie_UHF, zapytanie_UHF_LENGTH); LiczbaOdpowiedziUHF = 0;}
      else UHF_init(readingSerial, writingSerial, sd);

      Serial.print("\n"); Serial.print(SerialNum); Serial.print(": ");
      for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  }
}
void UHF_init(HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd) {
  static const int okHFresponse=6, pytL1=20, pytL7=5, pytL8=6, UHFL=8;
  static const char pyt1[pytL1] = {0x14, 0xFF, 0x81, 0x81, 0x00, 0x00, 0x08, 0x01, 0x00, 0x05, 0x00, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x31, 0xCA};
  static const char pyt2[pytL1] = {0x14, 0xFF, 0x81, 0x83, 0x00, 0x08, 0x00, 0x00, 0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xAF, 0x28};
  static const char pyt3[pytL1] = {0x14, 0xFF, 0x81, 0x84, 0x00, 0x00, 0x00, 0x00, 0x19, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x04, 0xC9, 0x39};
  static const char pyt4[pytL1] = {0x14, 0xFF, 0x81, 0x85, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x9B, 0xA0};
  static const char pyt5[pytL1] = {0x14, 0xFF, 0x81, 0x86, 0x02, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x05, 0x04, 0x00, 0x7D, 0x8E};
  static const char pyt6[pytL1] = {0x14, 0xFF, 0x81, 0x87, 0x02, 0x20, 0x2C, 0x01, 0x0D, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x92, 0xFC};
  static const char pyt7[pytL7] = {0x05, 0xFF, 0x63, 0xD3, 0xAE};
  static const char pyt8[pytL8] = {0x06, 0xFF, 0x52, 0x00, 0x0F, 0x6E};
  
  static const char okRes1[okHFresponse] = {0x06, 0x00, 0x81, 0x00, 0xAF, 0xDD};
  static const char okRes7[okHFresponse] = {0x06, 0x00, 0x63, 0x00, 0x86, 0x07};
  static const char okRes8[okHFresponse] = {0x06, 0x00, 0x52, 0x00, 0xFC, 0xA8};

  static const char UHFinit1[UHFL] = {0x01, 0x00, 0x00, 0xF0, 0x18, 0x00, 0x00, 0x00};
  static const char UHFinit2[UHFL] = {0x01, 0x00, 0x14, 0x01, 0xC8, 0x00, 0x00, 0x00};
  static const char UHFinit3[UHFL] = {0x01, 0x00, 0x06, 0x07, 0xC8, 0x00, 0x00, 0x00};
  static const char UHFinit4[UHFL] = {0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00};

  if (sd.mesLength == pytL1) {
    if       (memcmp(sd.buffer, pyt1, pytL1) == 0) { readingSerial.write(okRes1, okHFresponse); writingSerial.write(UHFinit1, UHFL); inicjalizacja=true;}
    else if  (memcmp(sd.buffer, pyt2, pytL1) == 0) { readingSerial.write(okRes1, okHFresponse); writingSerial.write(UHFinit2, UHFL); }
    else if  (memcmp(sd.buffer, pyt3, pytL1) == 0) { readingSerial.write(okRes1, okHFresponse); writingSerial.write(UHFinit3, UHFL); }
    else if  (memcmp(sd.buffer, pyt4, pytL1) == 0) { readingSerial.write(okRes1, okHFresponse); writingSerial.write(UHFinit4, UHFL); }
    else if ((memcmp(sd.buffer, pyt5, pytL1) == 0) || (memcmp(sd.buffer, pyt6, pytL1) == 0)) {readingSerial.write(okRes1, okHFresponse); inicjalizacja=false;}
    else Serial.println("\nUnrecognized Message.");
  }
  else if (memcmp(sd.buffer, pyt7, pytL7)==0) {readingSerial.write(okRes7, okHFresponse);}
  else if (memcmp(sd.buffer, pyt8, pytL8)==0) {readingSerial.write(okRes8, okHFresponse);}
  else Serial.println("\nUnrecognized Message.");
}