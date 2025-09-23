/** PAMIĘTAJ:
Konwerter  TxD---Tx1  MEGA2560
Konwerter  RxD---Rx1  MEGA2560  

KOD OPIS: Program dla MEGA2560, dwufunkcyjny
Płytka MEGA2560 jest połączona z 5 konwerterami TTL <-> RS232
Czujnik  HF: Tx1 Rx1 - Tx2 Rx2
Czujnik UHF: Tx3 Rx3 - "Tx4 (51)" "Rx4 (50)"
Komunikacja: Tx0 Rx0 - Port RS232 w PC |  Piny T/Rx0 muszą być rozwarte przy wgrywaniu programu.
  1. SNIF -  POodsłuchuje komunikacje czujników 
  2. AND gate dla obu czujników razem.      **/
#include <SoftwareSerial.h>
#include "SNIF.h"
#include "AND.h"

#define BOUND_UHF 115200  
#define BOUND_HF 38400 

static bool workAsSnifer = true; // else work as AND gate

SoftwareSerial Serial_4(50, 51); // Pin 50 Rx4,  Pin 51 Tx4,   TYLKO UHF!
serialData serialData1, serialData2, serialData3, serialData4;





void setup() {
/*  8 - 8 bitów danych
    E - Even Parity (parzystość parzysta)
    1 - 1 bit stopu                  */
  Serial.begin(115200);  // USB A/B oraz Tx0 i Rx0  {Komunikacja z PC}

  Serial1.begin(BOUND_HF, SERIAL_8E1);  // Rx1 Tx1 {HF 38400}
  Serial2.begin(BOUND_HF, SERIAL_8E1);  // Rx2 Tx2 {HF 38400}

  Serial3.begin(BOUND_UHF, SERIAL_8N1); // Rx1 Tx1 {UHF 115200}
  Serial_4.begin(BOUND_UHF); //8N1 {UHF 115200}

  while (!Serial) { ; }
  delay(100);
  Serial.println("\n--- MEGA2560 - uruchomiona - 23IX2025 ---");
}

void loop() {
  if(workAsSnifer) {
    handleSerialPort_HF(Serial1, Serial2, serialData1, "01");
    handleSerialPort_HF(Serial2, Serial1, serialData2, "02");
    handleSerialPort_UHF(Serial3, Serial_4, serialData3, "03");
    handleSerialPort_UHF(Serial_4, Serial3, serialData4, "04");
  } else {
    handleSerialPort_HF_CZUJNIK(Serial1, Serial2, serialData1, "01");
    handleSerialPort_HF_PC(Serial2, Serial1, serialData2, "02");
    handleSerialPort_UHF_CZUJNIK(Serial3, Serial1, serialData3, "03");
    /* Port 04 (UHF) jest ingnorowany w tym trybie pracy */
  }
  handleSerialPortCommunication(Serial);
}