#include <Arduino.h>
#include <Wire.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
//// num_runelude <MFRC522DriverI2C.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

//// num_runelude "RFID.h"
#include "MCP23017.h"
#include "header.h"
#include "Quetes.h"

TwoWire I2CD = TwoWire(0);
MCP23017 MCP(MCP23017_ADR, &I2CD);  //* Adresse module ext

////bool new_RFID[] = {false,false,false,false,false}, first_RFID[] = {false,false,false,false,false};
////int32_t test_card;
bool win_quete[] = {false,false,false,false,false};
////uint8_t uid[5][4]; // Array to store UID returned by RC522
MFRC522DriverPinSimple ss1_pin(5), ss2_pin(25), ss3_pin(26), ss4_pin(27), ss5_pin(33); // Configurable, see typical pin layout above.

MFRC522DriverSPI driver_1{ss1_pin}, driver_2{ss2_pin}, driver_3{ss3_pin}, driver_4{ss4_pin}, driver_5{ss5_pin}; //Create SPI driver.

MFRC522 readers[]{driver_1};   //, driver_2 Create MFRC522 instance.
uint8_t num_rune = 0, num_Quetes = 0;

void aff_win(){
  for (int i=0; i<5; i++){
    MCP.write1(RD[i], LOW);
    MCP.write1(GN[i], LOW);
    MCP.write1(BE[i], LOW);
  }
  delay(100);
  for (int i=0; i<5; i++){
    MCP.write1(GN[i], HIGH);
  }
  delay(100);
  for (int i=0; i<5; i++){
    MCP.write1(BE[i], HIGH);
  }
  delay(100);
  for (int i=0; i<5; i++){
    MCP.write1(GN[i], LOW);
  }
  delay(100);
  for (int i=0; i<5; i++){
    MCP.write1(RD[i], HIGH);
  }
  delay(100);
  for (int i=0; i<5; i++){
    MCP.write1(BE[i], LOW);
  }
  delay(100);
  for (int i=0; i<5; i++){
    MCP.write1(GN[i], HIGH);
  }
  delay(100);
  for (int i=0; i<5; i++){
    MCP.write1(RD[i], LOW);
  }
  delay(100);
}

void setup()
{
  Serial.begin(115200); //* Initialize serial communications with the PC for debugging.
  while (!Serial)
    ;                   //* Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4).

  I2CD.begin(I2C_SDA, I2C_SCL, I2C_Freq);

  
  bool b = MCP.begin();

  Serial.println(b ? "true" : "false");
  MCP.pinMode16(0x0000);
  MCP.setPullup16(0x0000);
  ////MCP.write16(1);
  for (MFRC522 reader : readers) {
    reader.PCD_Init(); //* Init each MFRC522 card.
    Serial.print(F("Reader "));
    num_rune++;
    Serial.print(num_rune);
    Serial.print(F(": "));
    MFRC522Debug::PCD_DumpVersionToSerial(reader, Serial);
  }
  init_quetes();
}
void loop()
{
  // Look for new cards.
  num_rune = 0;
  for (MFRC522 reader : readers) {
    //static bool toto = reader.PICC_IsNewCardPresent();
    if (reader.PICC_IsNewCardPresent() && reader.PICC_ReadCardSerial()) {//
        Serial.print(F("Reader "));
        Serial.println(num_rune+1);
        ////first_RFID num_rune] = true;

        ////for (uint8_t i = 0; i < 4; i++) {
        ////    uid num_rune][i] = reader.uid.uidByte[i]; // Store each byte of the UID in the array
        ////}
          if (     reader.uid.uidByte[0] == Quetes[num_Quetes].sort[num_rune][0] 
                && reader.uid.uidByte[1] == Quetes[num_Quetes].sort[num_rune][1] 
                && reader.uid.uidByte[2] == Quetes[num_Quetes].sort[num_rune][2] 
                && reader.uid.uidByte[3] == Quetes[num_Quetes].sort[num_rune][3])
          {
            Serial.println("The UIDs match.");
            win_quete[num_rune] = true;
            MCP.write1(RD[num_rune], LOW);
            MCP.write1(GN[num_rune], HIGH);
            MCP.write1(BE[num_rune], LOW);
          }
          else{
            Serial.println("The UIDs NO match.");
            win_quete[num_rune] = false;
            MCP.write1(RD[num_rune], HIGH);
            MCP.write1(GN[num_rune], LOW);
            MCP.write1(BE[num_rune], LOW);
          }

        //* Show some details of the PICC (that is: the tag/card).
        Serial.print(F(": Card UID:"));
        MFRC522Debug::PrintUID(Serial, reader.uid);
        Serial.println();
        
        ////Serial.print(F("PICC type: "));
        ////MFRC522::PICC_Type piccType = reader.PICC_GetType(reader.uid.sak);
        ////Serial.println(MFRC522Debug::PICC_GetTypeName(piccType));
        Serial.println(reader.PICC_ReadCardSerial());
        //* Halt PICC.
        reader.PICC_HaltA();
        //* Stop encryption on PCD.
        reader.PCD_StopCrypto1();
      } //test si lire
      ////if(!reader.PICC_ReadCardSerial()){
      ////reader.MIFARE_GetValue(0x00, &test_card);
      ////Serial.println(test_card);

      ////if (!reader.PICC_IsNewCardPresent() || !reader.PICC_ReadCardSerial()) {
      ////  Serial.println("NO card");
      ////}
      
      else{
        if(Quetes[num_Quetes].rune[num_rune]){
          Serial.println("NO card");
          MCP.write1(RD[num_rune], LOW);
          MCP.write1(GN[num_rune], LOW);
          MCP.write1(BE[num_rune], HIGH);
        }
        else{
          MCP.write1(RD[num_rune], LOW);
          MCP.write1(GN[num_rune], LOW);
          MCP.write1(BE[num_rune], LOW);
        }
      }
      
    ////test si nouvelle present
    ////Serial.println num_rune);
    ////new_RFID num_rune] = reader.PICC_IsNewCardPresent();
    num_rune++;
  } // for

  //*test si quete réussi
  if(win_quete[0] && win_quete[1] && win_quete[2] && win_quete[3] && win_quete[4]){
    num_Quetes++;
    for (int i=0; i<5; i++) win_quete[i] = false;
    aff_win();
    ////for(i=0; i<10; i++) t[i] = t[i]+1;
  }
}