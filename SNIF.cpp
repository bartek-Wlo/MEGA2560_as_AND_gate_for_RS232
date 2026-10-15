#include "SNIF.h"
bool workAsSnifer = false; // else work as AND gate, Tryb pracy programu


void handleSerialPort_HF(Stream& readingSerial, Stream& writingSerial, serialData& sd, const char* SerialNum) {
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
      readingSerial.readBytes(sd.buffer +1, sd.mesLength -1);
      writingSerial.write(sd.buffer, sd.mesLength);
      DEBUG_PRINT("\n"); DEBUG_PRINT(SerialNum); DEBUG_PRINT(": ");
      for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  }
}


void handleSerialPort_UHF(Stream& readingSerial, Stream& writingSerial, serialData& sd, const char* SerialNum) {
  if (readingSerial.available()) {
    char data = readingSerial.read();
    writingSerial.write(data); /* Od razu po zczytaniu bajd jest przesyłany dalej */

    /* Jeżeli jest timeout wysyłamy na konsole wszystko co doszło do tej pory */ 
    if((  sd.mesLength!=0  )&&(  millis()-sd.mesTime > FRAME_UHF_TIMEOUT  )) {
      DEBUG_PRINT("\nUHF Serial "); DEBUG_PRINT(SerialNum); DEBUG_PRINT(": TIME OUT ! ");
      DEBUG_PRINT(millis()-sd.mesTime); DEBUG_PRINT(" [ms]: "); for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    } /* Zapisujemy nowe bajty */
    if(sd.mesLength < UHF_MESSAGE_LENGTH) {
      sd.mesTime = millis();
      sd.buffer[sd.mesLength++] = data;
    } /* jeżeli jest cała wiadomość */
    if(sd.mesLength >= UHF_MESSAGE_LENGTH) {
      DEBUG_PRINT("\n"); DEBUG_PRINT(SerialNum); DEBUG_PRINT(": ");
      for (int i = 0; i < sd.mesLength; ++i) {printHex(sd.buffer[i]);}
      sd.mesLength = 0;
    }
  }
}


