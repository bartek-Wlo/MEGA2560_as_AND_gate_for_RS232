#include "AND.h"
// WYKRYTO HF:  11 00 B0 00 01 03 00 E0 16 80 FF 08 00 50 24 80 75    UHF: 4x  01 12 03 04 05 06 07 08 
// NIE wyk HF:  06 00 B0 01 5C 63                                     UHF: 4x  01 00 03 04 05 06 07 08
static unsigned int LiczbaOdpowiedziUHF = 0;   // Liczba Odpowiedzi czujnika UHF Od wysłania zapytania PC o wykrycie anteny.
static bool UHF_FirstErrorDetection = true;
static bool UHF_flag = false, HF_flag = false; // Czy (antenaUHFdobra, antena_HFdobra) zawierają aktualny stan anteny UHF i HF.
static bool antenaUHFdobra = false, antena_HFdobra = false; // Stan aktualnie sprawdzonej anteny UHF oraz HF
static uint8_t IDantenyCRC[4] = {0x00, 0x00, 0x00, 0x00};   // [13], [14], [15], [16] bity id anteny oraz CRC-16/MCRF4XX
static const int WYKRYTO_LENGTH = 17;
static uint8_t wykrytaAntena[WYKRYTO_LENGTH]; /* Zmienna do przechowywania ID wykrytej anteny*/

static bool AntenaDecectedAsBad = false; /* Jeżeli operacja && zakończy się zwróceniem złej anteny, ta zmienna jest TRUE.
Zmienna pilnuje by program się nie rozjechał po wysłaniu wiadomości do PC że antena jest zła, PC sprawdza 2 razy złe anteny (Program sam z siebie też dla UHF)*/
static bool historic_UHF = false; /* Wczytywane z buffora w czujnikiANDgate(). Jeżeli TRUE w handleSerialPort_HF_PC() ponownie sprawdzi antene HF */

/* Zapytania do czujnika HF i UHF o stan anteny */
const char zapytanie_HF[zapytanie_HF_LENGTH] = {0x07, 0xFF, 0xB0, 0x01, 0x00, 0x1C, 0x56};
const char zapytanie_UHF[UHF_MESSAGE_LENGTH] = {0x01, 0x00, 0x00, 0xF0, 0x10, 0x00, 0x00, 0x00};



void resetValuesForConnectionTEstCheck() {
  AntenaDecectedAsBad = false;
  LiczbaOdpowiedziUHF = 0;
  UHF_FirstErrorDetection = true;
}








void handleSerialPort_HF_CZUJNIK(HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum) {
  if (readingSerial.available()) {
    if ((  sd.mesLength!=0  )&&(  millis()-sd.mesTime > FRAME_TIMEOUT  )) {
      DEBUG_PRINT("\nHF Serial "); DEBUG_PRINT(SerialNum); DEBUG_PRINT(": TIME OUT !");
      DEBUG_PRINT(millis()-sd.mesTime); DEBUG_PRINT(" [ms] Message length: "); printHex(sd.buffer[0]);
      sd.mesLength = 0;
    }
    if (sd.mesLength == 0) {
      sd.mesLength = readingSerial.read();
      if (sd.mesLength >= BUFFER_SIZE) sd.mesLength = BUFFER_SIZE - 1;
      sd.buffer[0] = sd.mesLength;
      sd.mesTime = millis();
    }
    if ((  sd.mesLength!=0  )&&(  readingSerial.available() >= sd.mesLength-1  )) {
      decideForCompleteMessage_HF(readingSerial, writingSerial, sd, SerialNum);
    }
  }
}

void decideForCompleteMessage_HF(HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum) {
  stan_HF = true;
  static const int ERROR_LENGTH = 6, COM_ERROR_LENGTH = 7;
  static const char nieWykryto[ERROR_LENGTH] = {0x06, 0x00, 0xB0, 0x01, 0x5C, 0x63};
  static const char error_antena[ERROR_LENGTH] = {0x06, 0x00, 0xB0, 0x81, 0x54, 0xE7};
  static const char error_comunication[COM_ERROR_LENGTH] = {0x07, 0x00, 0xB0, 0x83, 0x00, 0xB2, 0x2C};

  readingSerial.readBytes(sd.buffer + 1, sd.mesLength - 1);

  if        (memcmp(sd.buffer, nieWykryto, ERROR_LENGTH) == 0)                             { DEBUG_PRINT("\nAntena HF not detect < < < < < "); HF_flag=true; antena_HFdobra=false;
  // } else if (memcmp(sd.buffer, error_comunication, COM_ERROR_LENGTH) == 0 || 
  //            memcmp(sd.buffer, error_antena, ERROR_LENGTH) == 0)                           { DEBUG_PRINT("\nCommunication ERROR < ! < ! <");   writingSerial.write(sd.buffer, sd.mesLength);
  } else if (memcmp(sd.buffer, error_comunication, COM_ERROR_LENGTH) == 0 || 
             memcmp(sd.buffer, error_antena, ERROR_LENGTH) == 0)                           { DEBUG_PRINT("\nCommunication ERROR < ! < ! <");   HF_flag=true; antena_HFdobra=false; // Żeby maszyna się nie zacinała, ustawiamy to jako złe
  } else if (sd.buffer[2] == 0xB0 && sd.buffer[3] == 0x00 && sd.mesLength==WYKRYTO_LENGTH) { DEBUG_PRINT("\nDetect: HF, ");                    memcpy(wykrytaAntena, sd.buffer, WYKRYTO_LENGTH);  HF_flag = true; antena_HFdobra = true;
  } else if (sd.buffer[2] == 0x81 || sd.buffer[2] == 0x63 || sd.buffer[2] == 0x52)         { DEBUG_PRINT("\nINICJALIZACJA HF, ");              writingSerial.write(sd.buffer, sd.mesLength);
  } else                                                                                   { DEBUG_PRINT("\nelse HF, ");                       writingSerial.write(sd.buffer, sd.mesLength);
  }

  /*DEBUG_PRINT("\n");*/ DEBUG_PRINT(SerialNum); DEBUG_PRINT(": ");
  for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
  sd.mesLength = 0;
}






void handleSerialPort_UHF_CZUJNIK(HardwareSerial& readingSerial, HardwareSerial& writingSerial, serialData& sd, const char* SerialNum) {
  static const char  error_UHF[UHF_MESSAGE_LENGTH] = {0x00, 0x00, 0x00, 0x00, 0xF0, 0x00, 0x06, 0x02};
  static const char error2_UHF[UHF_MESSAGE_LENGTH] = {0x00, 0x00, 0x00, 0x0F, 0x00, 0x00, 0x00, 0x00};
  static const char error3_UHF[UHF_MESSAGE_LENGTH] = {0x01, 0x00, 0x12, 0x00, 0x21, 0x00, 0x00, 0x00};
  static const char CMD_error[UHF_MESSAGE_LENGTH] =  {0x55, 0x0F, 0xDD, 0xDD, 0xDD, 0x83, 0x23, 0xCC};
  if (readingSerial.available()) {
    if((  sd.mesLength!=0  )&&(  millis()-sd.mesTime > FRAME_UHF_TIMEOUT  )) {
      DEBUG_PRINT("\nUHF Serial "); DEBUG_PRINT(SerialNum); DEBUG_PRINT(": TIME OUT !");
      DEBUG_PRINT(millis()-sd.mesTime); DEBUG_PRINT(" [ms]: "); for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  
    if(sd.mesLength < UHF_MESSAGE_LENGTH) {
      sd.mesTime = millis();
      sd.buffer[sd.mesLength++] = readingSerial.read();
    } 
    if(sd.mesLength >= UHF_MESSAGE_LENGTH) {
      ++LiczbaOdpowiedziUHF;  
      if(LiczbaOdpowiedziUHF == 3) {
        if(sd.buffer[1]==0x12)       {UHF_flag=true,  antenaUHFdobra=true;  DEBUG_PRINT("\n Detect: UHF ");}
        else if (sd.buffer[1]==0x00) {BAD_UHF_CZUJNIK(readingSerial);       DEBUG_PRINT("\n Antena UHF not detect < < < < <");}
        else                         {BAD_UHF_CZUJNIK(readingSerial);       DEBUG_PRINT("\nNo response recognized, about the UHF antenna ");}
      } else DEBUG_PRINT("\n");

      if(( memcmp(sd.buffer, error_UHF, UHF_MESSAGE_LENGTH)==0 )||( memcmp(sd.buffer, error2_UHF, UHF_MESSAGE_LENGTH)==0 )||( memcmp(sd.buffer, error3_UHF, UHF_MESSAGE_LENGTH)==0 )) { AppPort.write(CMD_error, 8); stanUHF = false;}
      else stanUHF = true; // Jeżeli maszyna w ramach odpowiedzi zgłasza dziwne błędy to nie złgaszamy że UHF stan odpowiada tylko milczymy.
      DEBUG_PRINT(SerialNum); DEBUG_PRINT(": ");
      for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  }
}
void BAD_UHF_CZUJNIK(HardwareSerial& readingSerial) {
  if(UHF_FirstErrorDetection) {
    UHF_FirstErrorDetection = false;
    readingSerial.write((const uint8_t*)zapytanie_UHF, UHF_MESSAGE_LENGTH); // Polecenie powtórzenia badania
    LiczbaOdpowiedziUHF = 0;
  } else { /* Pownie zły wynik anteny */
    UHF_FirstErrorDetection = true;
    UHF_flag=true;
    antenaUHFdobra=false;
  }
}









void handleSerialPort_HF_PC(HardwareSerial& readingSerial, HardwareSerial& writing_UHF_Serial, HardwareSerial& writing_HF_Serial, serialData& sd, const char* SerialNum) {
  static const char error_comunication2[7] = {0x07, 0x00, 0xB0, 0x83, 0x00, 0xB2, 0x2C};
  if (readingSerial.available()) {
    if ((  sd.mesLength!=0  )&&(  millis()-sd.mesTime > FRAME_TIMEOUT  )) {
      DEBUG_PRINT("\nHF Serial "); DEBUG_PRINT(SerialNum); DEBUG_PRINT(": TIME OUT !");
      DEBUG_PRINT(millis()-sd.mesTime); DEBUG_PRINT(" [ms] Message length: "); printHex(sd.buffer[0]);
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

/** A) JEŻELI WIADOMOŚĆ OD PC TO ZAPYTANIE O STAN ANTENY: ********************************************************************************/
      if(memcmp(sd.buffer, zapytanie_HF, zapytanie_HF_LENGTH)==0) {
/** STOP) ZATRZYMANIE PRODUKCJI                                                                                                         **/
        if(stop_maszyny) {stop_maszyny=false; readingSerial.write(error_comunication2, 7);}
/** A.1) ODCZYT NOWEJ ANTENY *************************************************************************************************************/
        else if(AntenaDecectedAsBad == false) {
          writing_UHF_Serial.write(zapytanie_UHF, UHF_MESSAGE_LENGTH);
          LiczbaOdpowiedziUHF = 0; /* Liczba udzielonych odpowiedzi od zapytania o stan anteny = 0*/
          UHF_FirstErrorDetection = true; /* Liczba prób zakończonych wykryciem złej anteny */

          writing_HF_Serial.write( zapytanie_HF,  zapytanie_HF_LENGTH);
/** A.2) ODCZYT ANTENY POPRZEDNIO WYKRYTEJ JAKO ZŁA **************************************************************************************/
        } else { /* Sprawdzenie UHF z przeszłości jest nie możliwe (oraz redundantne) zwracamy ten sam wynik UHF co ostatnio:            */
          UHF_flag = true;  /* Stan zmiennej antenaUHFdobra pozostaje taki sam jak w poprzednim raporcie                                 */

          /* IF  AntenaDecectedAsBad == true  to jedna z anten była zła, 
             sens ponownego sprawdzania HF ma tylko jeżeli był to przypadek:  UHF:1 && HF:0 
             Dla  UHF:0 && HF:0  nie ma sensu powtażać zapytania o HF.   Ten przypadek byłby sprawdzany przez: if( ! antena_HFdobra) ... */
          if(historic_UHF == true) writing_HF_Serial.write( zapytanie_HF,  zapytanie_HF_LENGTH);
          else HF_flag = true;
        }
/** B) INNE ZAPYTANIA:, NP. KONFIGURACYJNE ***********************************************************************************************/
      } else writing_HF_Serial.write(sd.buffer, sd.mesLength); /* Zachowanie jak SNIF */

/** C) WYDRUKOWANIE ZAWARTOŚCI WIADOMOŚĆ OD PC *******************************************************************************************/
      DEBUG_PRINT("\n"); DEBUG_PRINT(SerialNum); DEBUG_PRINT(": ");
      for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  }
}








/* Dla przedstawionego: 1   2   3   4   5   6   7   8   9   Distance = 9                   */
/* ==[x]=[x]=[czujnik UHF]=[x]=[x]=[x]=[x]=[x]=[x]=[x]=[czujnik HF]=[x]=[x]==  -> kierunek */
bool ringbufferUHFhistoric[64] = {0}; // Bufor do zapisywania przeszłych stanów czujnika HF i porównywania go z aktualnym UHF
void czujnikiANDgate(HardwareSerial& writingSerial, HardwareSerial& writing_HF_Serial, Stream &AppSerial) {
  static unsigned int ringbuffer_position = 0; // wskaźnik na pozycje do zapisania stanu HF, najmlodszy zapis w buforze

  static const int LENGTH = 6;
  static const char nieWykryto[LENGTH] = {0x06, 0x00, 0xB0, 0x01, 0x5C, 0x63};
  uint8_t responseToApp[6]; // [6] bo jeszcze CRC16response

  if(UHF_flag && HF_flag) { 
    ringbufferUHFhistoric[ringbuffer_position] = antenaUHFdobra;
    unsigned int TempRingbufferposition = ((ringbuffer_position + 1) % Distance);
    historic_UHF = ringbufferUHFhistoric[TempRingbufferposition];

    if(AntenaDecectedAsBad) { 
      DEBUG_PRINT("\n&&: Ponowne zgłoszenie tej samej anteny, UHF:");  DEBUG_PRINT((int)historic_UHF);  DEBUG_PRINT(" HF:");  DEBUG_PRINTLN((int)antena_HFdobra);
      if(antena_HFdobra && historic_UHF) { // 2sd AND == TRUE
        writingSerial.write(wykrytaAntena, WYKRYTO_LENGTH);  
        DEBUG_PRINTLN("\n&&: DOBRA antena, ");
        responseToApp[1] = 0xFF; //        HF DOBRA
        responseToApp[2] = 0xFF; // FF II AND DOBRY
      } else {
        writingSerial.write(nieWykryto, LENGTH); 
        DEBUG_PRINT("\n&&: ZŁA antena, ");
        if(antena_HFdobra) responseToApp[1] = 0xFF;
        else responseToApp[1] = historic_UHF ? /*powtórzono badanie - ZŁA*/ 0x00 : /*nie powtórzono - nie wiadomo*/0xF0;  // <-- HF
        responseToApp[2] = 0xF0; // F0 II AND ZŁY
      }
      AntenaDecectedAsBad = false; 
      ringbuffer_position = TempRingbufferposition;
      responseToApp[0] = historic_UHF   ? 0xFF : 0x00; // UHF, mimo że nie było nowego badania przesyłamy ponownie
      responseToApp[3] = antenaUHFdobra ? 0xFF : 0x00; // bufor UHF, mimo że nie było nowego badania przesyłamy ponownie

    } else {
      if(antena_HFdobra && historic_UHF) { // 1st AND == TRUE
        writingSerial.write(wykrytaAntena, WYKRYTO_LENGTH);  
        DEBUG_PRINTLN("\n&&: DOBRA antena");  
        ringbuffer_position = TempRingbufferposition;
        AntenaDecectedAsBad = false; /* reset zmiennej, jeśli coś się źle przestawi, jeżeli wykryto dobrą antene to napweno jest FALSE*/
        responseToApp[0] = 0xFF; // UHF 
        responseToApp[1] = 0xFF; // HF 
        responseToApp[2] = 0x0F; // 0F I AND DOBRY
      } else {
        writingSerial.write(nieWykryto, LENGTH); 
        DEBUG_PRINT("\n&&: ZŁA antena, UHF:");  DEBUG_PRINT((int)historic_UHF);  DEBUG_PRINT(" HF:");  DEBUG_PRINTLN((int)antena_HFdobra);
        responseToApp[0] = historic_UHF   ? 0xFF : 0x00; // UHF 
        responseToApp[1] = antena_HFdobra ? 0xFF : 0x00; // HF 
        responseToApp[2] = 0x00; // 00 I AND ZŁY

        /* W następnym wywołaniu sprawdzenia stanów anten przez PC, pętla od razu zwraca inf: złe anteny bez redundantnego sprawdzania */
        AntenaDecectedAsBad = true; 
      }
      responseToApp[3] = antenaUHFdobra ? 0xFF : 0x00; // bufor UHF
    }
    reportToControlApplication(AppSerial, responseToApp);
    #ifdef DEBUG_DISPLAY
      printRingBuffer(ringbufferUHFhistoric, 64, ringbuffer_position, Distance);
    #endif

    UHF_flag = false;
    HF_flag = false;
  }
}



/* Funcja wyświetlająca stan ring buffer, do DEBUG*/
void printRingBuffer(bool* buffer, unsigned int size, unsigned int currentPos, unsigned int distance) {
  DEBUG_PRINT("History Buffer [");
  DEBUG_PRINT(distance);
  DEBUG_PRINT("]: ");
  
  for (unsigned int i = 0; i < distance; i++) {
    if (i == currentPos) DEBUG_PRINT(">");
    DEBUG_PRINT(buffer[i] ? "1" : "0");
    if (i == currentPos) DEBUG_PRINT("<");
    if (i < distance - 1) DEBUG_PRINT(", ");
  }
  DEBUG_PRINTLN();
}
