#include "AND.h"
// Najpierw czujnik HF potem UHF w odległości inline static unsigned int Distance = 8; [HexFun.h] modyfikowany przez port Serial0

static unsigned int LiczbaOdpowiedziUHF = 0;   // Liczba Odpowiedzi czujnika UHF Od wysłania zapytania PC o wykrycie anteny.
static bool inicjalizacja = false;             // Czy właśnie trwa inicjalizacja czujników
static bool UHF_flag = false, HF_flag = false; // Czy już sprawdzono stan anteny HF i UHF 
static bool antenaUHFdobra = false, antena_HFdobra = false; // Stan sprawdzonej anteny
static bool HFbuffer[16];  static unsigned int position;    // Bufor zapisujący poprzednie stany anteny do porównania z aktualnymi stanami drugiej anteny.
// strncpy(HFbuffer,  sd.buffer, HFlength);

void handleSerialPort_HF_CZUJNIK(HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum) {
  static const int ERROR_LENGTH = 6, WYKRYTO_LENGTH = 17, COM_ERROR_LENGTH = 7;
  static const char nieWykryto[ERROR_LENGTH] = {0x06, 0x00, 0xB0, 0x01, 0x5C, 0x63};
  static const char error_antena[ERROR_LENGTH] = {0x06, 0x00, 0xB0, 0x81, 0x54, 0xE7};
  static const char error_comunication[COM_ERROR_LENGTH] = {0x07, 0x00, 0xB0, 0x83, 0x00, 0xB2, 0x2C};
  static const char wykryto[WYKRYTO_LENGTH] = {0x11, 0x00, 0xB0, 0x00, 0x01, 0x03, 0x00, 0xE0, 0x16, 0x80, 0xFF, 0x08, 0x00, 0x50, 0x24, 0x80, 0x75};  /*13,56 MHz Transponder, ISO15693 Tags*/
  static const char wykryto2[WYKRYTO_LENGTH] = {0x11, 0x00, 0xB0, 0x00, 0x01, 0x03, 0x00, 0xE0, 0x16, 0x80, 0xFF, 0x08, 0x00, 0x4F, 0xBC, 0x18, 0x7B}; /*13,56 MHz Transponder, ISO15693 Tags*/
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

      if     (memcmp(sd.buffer, nieWykryto, ERROR_LENGTH)==0)                                                                 {HF_flag=true,   antena_HFdobra=false;          Serial.print("\n> > > > > > > > > > > > > Antena HF not detect ");}
      else if(memcmp(sd.buffer, error_comunication, COM_ERROR_LENGTH)==0 || memcmp(sd.buffer, error_antena, ERROR_LENGTH)==0) {writingSerial.write(sd.buffer, sd.mesLength);  Serial.print("\n> ! > ! > ! > ! > ! > ! > Communication ERROR ");}
      else if(sd.buffer[2]==0xB0 && sd.buffer[3]==0x00)                       { Serial.print("\nDetect: HF, ");          HF_flag=true,  antena_HFdobra=true;}
      else if(sd.buffer[2]==0x81 || sd.buffer[2]==0x63 || sd.buffer[2]==0x52) { Serial.print("\nINICJALIZACJA HF, ");  writingSerial.write(sd.buffer, sd.mesLength);}
      else                                                                    { Serial.print("\nelse, ");              writingSerial.write(sd.buffer, sd.mesLength);}

      /*Serial.print("\n");*/ Serial.print(SerialNum); Serial.print(": ");
      for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  }
}

void handleSerialPort_UHF_CZUJNIK(HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum) {
  static const int GoodResL = 17, badResL = 6;
  static const char GoodRes[GoodResL] = {0x11, 0x00, 0xB0, 0x00, 0x01, 0x03, 0x00, 0xE0, 0x16, 0x80, 0xFF, 0x08, 0x00, 0x50, 0x24, 0x80, 0x75};
  static const char ErrorRes[badResL] = {0x06, 0x00, 0xB0, 0x81, 0x54, 0xE7};
  static const char badRes[badResL] = {0x06, 0x00, 0xB0, 0x01, 0x5C, 0x63};
  static const char UHFinit0[UHF_MESSAGE_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  if (readingSerial.available()) {
    if((  sd.mesLength!=0  )&&(  millis()-sd.mesTime > FRAME_UHF_TIMEOUT  )) {
      if(memcmp(sd.buffer, UHFinit0, sd.mesLength) == 0) writingSerial.write(UHFinit0, UHF_MESSAGE_LENGTH);
      Serial.print("\nUHF Serial "); Serial.print(SerialNum); Serial.print(": TIME OUT !");
      Serial.print(millis()-sd.mesTime); Serial.print(" [ms]: "); for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  
    if(sd.mesLength < UHF_MESSAGE_LENGTH) {
      sd.mesTime = millis();
      sd.buffer[sd.mesLength++] = readingSerial.read();
    } 
    if(sd.mesLength >= UHF_MESSAGE_LENGTH) {

      if(inicjalizacja==false) {
        ++LiczbaOdpowiedziUHF;  
        if(LiczbaOdpowiedziUHF == 3) {
          if(sd.buffer[1]==0x12)       {UHF_flag=true,  antenaUHFdobra=true;  Serial.print("\n Detect: UHF ");}
          else if (sd.buffer[1]==0x00) {UHF_flag=true,  antenaUHFdobra=false; Serial.print("\n>>>>>>>>>>>>>>>>>>>>>>>>> Antena UHF not detect ");}
          else Serial.print("\nNo response recognized, about the UHF antenna");
        } else Serial.print("\n");
      } else Serial.print("\n");

      Serial.print(SerialNum); Serial.print(": ");
      for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  }
}


void handleSerialPort_HF_PC(HardwareSerial& readingSerial, HardwareSerial& writing_UHF_Serial, HardwareSerial& writing_HF_Serial, serialData& sd, const char* SerialNum) {
  static const int zapytanie_HF_LENGTH = 7;
  static const char zapytanie_HF[zapytanie_HF_LENGTH] = {0x07, 0xFF, 0xB0, 0x01, 0x00, 0x1C, 0x56};
  static const char zapytanie_UHF[UHF_MESSAGE_LENGTH] = {0x01, 0x00, 0x00, 0xF0, 0x10, 0x00, 0x00, 0x00};
  if (readingSerial.available()) {
    if ((  sd.mesLength!=0  )&&(  millis()-sd.mesTime > FRAME_TIMEOUT  )) {
      Serial.print("\nHF Serial "); Serial.print(SerialNum); Serial.print(": TIME OUT !");
      Serial.print(millis()-sd.mesTime); Serial.print(" [ms] Message length: "); printHex(sd.buffer[0]);
      sd.mesLength = 0;
    }
    if (sd.mesLength == 0) { // Zczytanie długości wiadomości
      sd.mesLength = readingSerial.read();
      if (sd.mesLength >= BUFFER_SIZE) sd.mesLength = BUFFER_SIZE - 1;
      sd.buffer[0] = sd.mesLength;
      sd.mesTime = millis();
    }
    if ((  sd.mesLength!=0  )&&(  readingSerial.available() >= sd.mesLength-1  )) {
      readingSerial.readBytes(sd.buffer +1, sd.mesLength -1);

      if(memcmp(sd.buffer, zapytanie_HF, zapytanie_HF_LENGTH)==0) {
        writing_UHF_Serial.write(zapytanie_UHF, UHF_MESSAGE_LENGTH); 
        writing_HF_Serial.write( zapytanie_HF,  zapytanie_HF_LENGTH);
        LiczbaOdpowiedziUHF = 0;
      } else UHF_init(readingSerial, writing_UHF_Serial, writing_HF_Serial, sd);

      Serial.print("\n"); Serial.print(SerialNum); Serial.print(": ");
      for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  }
}
void UHF_init(HardwareSerial& readingSerial, HardwareSerial& writing_UHF_Serial, HardwareSerial& writing_HF_Serial, serialData& sd) {
  static const int okHFresponse=6, pytL1=20, pytL7=5, pytL8=6;
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

  static const char UHFinit0[UHF_MESSAGE_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
  static const char UHFinit1[UHF_MESSAGE_LENGTH] = {0x01, 0x00, 0x00, 0xF0, 0x18, 0x00, 0x00, 0x00};
  static const char UHFinit2[UHF_MESSAGE_LENGTH] = {0x01, 0x00, 0x14, 0x01, 0xC8, 0x00, 0x00, 0x00};
  static const char UHFinit3[UHF_MESSAGE_LENGTH] = {0x01, 0x00, 0x06, 0x07, 0xC8, 0x00, 0x00, 0x00};
  static const char UHFinit4[UHF_MESSAGE_LENGTH] = {0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00};
  static const char UHFinit[35][UHF_MESSAGE_LENGTH] = {
    {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x00, 0xF0, 0x18, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x00, 0x07, 0x01, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x01, 0x07, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x02, 0x07, 0x01, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x04, 0x07, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x05, 0x07, 0x64, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x07, 0x07, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x00, 0xF0, 0x18, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x0A, 0x01, 0xC4, 0x38, 0x1D, 0x00},
    {0x01, 0x00, 0x08, 0x01, 0x01, 0x01, 0x00, 0x00},
    {0x01, 0x00, 0x00, 0xF0, 0x27, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x00, 0xF0, 0x18, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x14, 0x01, 0x64, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x06, 0x07, 0x64, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x03, 0x02, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x06, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x00, 0x09, 0xC0, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x01, 0x09, 0x40, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x11, 0x09, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x02, 0x09, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x03, 0x09, 0xF4, 0x40, 0x00, 0x00},
    {0x01, 0x00, 0x05, 0x09, 0x01, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x01, 0x0A, 0x06, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x02, 0x0A, 0x01, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x03, 0x0A, 0x02, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x04, 0x0A, 0x06, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x05, 0x0A, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x06, 0x0A, 0x00, 0x00, 0x00, 0x00},
    {0x01, 0x00, 0x07, 0x0A, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x08, 0x0A, 0x00, 0x00, 0x00, 0x00},
    {0x00, 0x00, 0x05, 0x00, 0x00, 0x00, 0x00, 0x00}
  };

  if(memcmp(sd.buffer, pyt1, pytL1) == 0) {
    inicjalizacja=true;
    writing_HF_Serial.write(pyt1, pytL1);
    for(int i=0; i<9; ++i) writing_UHF_Serial.write(UHFinit[i], UHF_MESSAGE_LENGTH); 
  } else if(memcmp(sd.buffer, pyt2, pytL1) == 0) {
    writing_HF_Serial.write(  pyt2, pytL1);
    for(int i=9; i<17; ++i) writing_UHF_Serial.write(UHFinit[i], UHF_MESSAGE_LENGTH); 
  } else if(memcmp(sd.buffer, pyt3, pytL1) == 0) {
    writing_HF_Serial.write(  pyt3, pytL1);
    for(int i=17; i<26; ++i) writing_UHF_Serial.write(UHFinit[i], UHF_MESSAGE_LENGTH); 
  } else if(memcmp(sd.buffer, pyt4, pytL1) == 0) {
    writing_HF_Serial.write(  pyt4, pytL1);
    for(int i=26; i<35; ++i) writing_UHF_Serial.write(UHFinit[i], UHF_MESSAGE_LENGTH); 
  } else if(memcmp(sd.buffer, pyt8, pytL8)==0) {
    inicjalizacja=false;
    writing_HF_Serial.write(pyt8, pytL8);
  } else writing_HF_Serial.write(sd.buffer, sd.mesLength);
}



void czujnikiANDgate(HardwareSerial& writingSerial) {
  static const int LENGTH = 6, WYKRYTO_LENGTH = 17;
  static const char nieWykryto[LENGTH] = {0x06, 0x00, 0xB0, 0x01, 0x5C, 0x63};
  static const char wykryto[WYKRYTO_LENGTH] = {0x11, 0x00, 0xB0, 0x00, 0x01, 0x03, 0x00, 0xE0, 0x16, 0x80, 0xFF, 0x08, 0x00, 0x50, 0x24, 0x80, 0x75};  /*13,56 MHz Transponder, ISO15693 Tags*/
  if(UHF_flag && HF_flag) {
    if(antenaUHFdobra && antena_HFdobra) {writingSerial.write(wykryto, WYKRYTO_LENGTH); Serial.println("\nXX: DOBRA antena");}
    else {writingSerial.write(nieWykryto, LENGTH); Serial.println("\nXX: ZŁA antena");}
    UHF_flag = false;
    HF_flag = false;
  }
}