/** 
KOD OPIS: Program dla MEGA2560, dwufunkcyjny:
  1. SNIF -  Podsłuchuje komunikacje czujników 
  2. AND gate dla obu czujników razem - właściwe zadanie, domyślna praca programu.

Konwerter  TxD---Tx1  MEGA2560
Konwerter  RxD---Rx1  MEGA2560  

Płytka MEGA2560 jest połączona z 5 konwerterami TTL <-> RS232
Czujnik  HF: Tx1 Rx1 - Tx2 Rx2
Czujnik UHF: Tx3 Rx3 - "Tx4 (51)" "Rx4 (50)"

Komunikacja: Tx0 Rx0 - Port RS232 w PC |  Piny T/Rx0 muszą być rozwarte przy wgrywaniu programu.  

// ZAPYTANIE HF:  07 FF B0 01 00 1C 56                                  UHF:     01 00 00 F0 10 00 00 00
// WYKRYTO   HF:  11 00 B0 00 01 03 00 E0 16 80 FF 08 00 50 24 80 75    UHF: 4x  01 12 03 04 05 06 07 08 
// NIE wyk   HF:  06 00 B0 01 5C 63                                     UHF: 4x  01 00 03 04 05 06 07 08
**/

#include <SoftwareSerial.h>
#include "config.h"
#include "HexFun.h"
#include "SNIF.h"
#include "AND.h"
#include "AppCommunication.h"

SoftwareSerial Serial_4(50, 51); // Pin 50 Rx4,  Pin 51 Tx4,   TYLKO UHF!
SoftwareSerial Serial_5(52, 53); // Pin 52 Rx4,  Pin 53 Tx4,   KOMUNIKACJA Z APLIKACJA
serialData serialData1, serialData2, serialData3, serialData4;

unsigned int Distance = DEFAULT_DISTANCE; /* Domyślna licznba anten pomiedzy czujnika + 2 */
HardwareSerial& PC_USB     = Serial;
HardwareSerial& HF_Sensor  = Serial1; // HF
HardwareSerial& HF_PC      = Serial2; // PC maszyna (HF)
HardwareSerial& UHF_Sensor = Serial3; // UHF
SoftwareSerial& UHF_PC     = Serial_4;
SoftwareSerial& AppPort    = Serial_5; // Aplikacja


void setup() {
/*  8 - 8 bitów danych
    E - Even Parity (parzystość parzysta)
    1 - 1 bit stopu                  */
  PC_USB.begin(115200);  // USB A/B oraz Tx0 i Rx0  {Komunikacja z PC}

  HF_Sensor.begin(BOUND_HF, SERIAL_8E1);  // Rx1 Tx1 {HF 38400}
  HF_PC.begin(BOUND_HF, SERIAL_8E1);      // Rx2 Tx2 {HF 38400} PC

  UHF_Sensor.begin(BOUND_UHF, SERIAL_8N1);// Rx1 Tx1 {UHF 115200}
  UHF_PC.begin(BOUND_UHF);     // Rx 50  Tx 51   8N1 {UHF 115200} PC
  AppPort.begin(BOUND_HF);     // Rx 52  Tx 53   8N1 {Aplikacja 38400} APLIKACJA

  delay(100);
  PC_USB.print(F("\n--- MEGA2560 --- turn ON --- AND gate MODE --- D: ")); PC_USB.println(Distance);
  PC_USB.print(F("==[x]=[x]=[czujnik UHF]")); for (unsigned i = 0; i<(Distance-2); ++i) PC_USB.print(F("=[x]")); PC_USB.print(F("=[czujnik HF]=[x]=[x]=[inker]==")); // --> Kierunek taśmy
  okResponse(AppPort);
}


// void loop() {
//   if(workAsSnifer) {
//     handleSerialPort_HF(  HF_Sensor,  HF_PC,     serialData1, "S01");
//     handleSerialPort_HF(  HF_PC,      HF_Sensor, serialData2, "S02");
//     handleSerialPort_UHF(UHF_Sensor, UHF_PC,     serialData3, "S03");
//     handleSerialPort_UHF(UHF_PC,     UHF_Sensor, serialData4, "S04");
//   } else {
//     handleSerialPort_HF_CZUJNIK( HF_Sensor,  HF_PC,                serialData1, "01");
//     handleSerialPort_HF_PC(      HF_PC,     UHF_Sensor, HF_Sensor, serialData2, "02");
//     handleSerialPort_UHF_CZUJNIK(           UHF_Sensor, HF_PC,     serialData3, "03");
//     /* Port 04 (UHF) jest ingnorowany w tym trybie pracy */
//     czujnikiANDgate(HF_PC, HF_Sensor, AppPort);
//   }
//   // handleSerialPortCommunication(PC_USB, HF_PC);
//   handleSerialPortCommunication(AppPort, HF_PC);
//   // debugCommunication(PC_USB);
// }



void loop() { /* Z powodu problemów HARDWAREowych program może działać tylko jako AND gate. */
  handleSerialPort_HF_CZUJNIK( HF_Sensor,  HF_PC,                serialData1, "01");
  handleSerialPort_HF_PC(      HF_PC,     UHF_Sensor, HF_Sensor, serialData2, "02");
  handleSerialPort_UHF_CZUJNIK(           UHF_Sensor, HF_PC,     serialData3, "03");
  /* Port 04 (UHF) jest ingnorowany w tym trybie pracy */
  czujnikiANDgate(HF_PC, HF_Sensor, AppPort);

  // handleSerialPortCommunication(PC_USB, HF_PC);
  handleSerialPortCommunication(AppPort, HF_PC);
  // debugCommunication(PC_USB);
}