#include <Arduino.h>
#include <Wire.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
//#include <MFRC522DriverI2C.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

//#include "RFID.h"
#include "MCP23017.h"
#include "header.h"

TwoWire I2CD = TwoWire(0);
MCP23017 MCP(MCP23017_ADR, &I2CD);  // Adresse module ext

uint8_t uid[5][4]; // Array to store UID returned by RC522
MFRC522DriverPinSimple ss1_pin(5), ss2_pin(5), ss3_pin(13), ss4_pin(18), ss5_pin(25); // Configurable, see typical pin layout above.

MFRC522DriverSPI driver_1{ss1_pin}, driver_2{ss2_pin}, driver_3{ss3_pin}, driver_4{ss4_pin}, driver_5{ss5_pin}; //Create SPI driver.

MFRC522 readers[]{driver_1, driver_2};   // Create MFRC522 instance.
uint8_t inc = 0;
String tagContent = "";

void setup()
{
  Serial.begin(115200); // Initialize serial communications with the PC for debugging.
  while (!Serial)
    ;                                              // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4).

  I2CD.begin(I2C_SDA, I2C_SCL, I2C_Freq);

  
  bool b = MCP.begin();

  Serial.println(b ? "true" : "false");
  MCP.pinMode16(0x0000);  //  0 = output , 1 = input
  MCP.setPullup16(0x0000);
  //MCP.write16(1);
  for (MFRC522 reader : readers) {
    reader.PCD_Init(); // Init each MFRC522 card.
    Serial.print(F("Reader "));
    inc++;
    Serial.print(inc);
    Serial.print(F(": "));
    MFRC522Debug::PCD_DumpVersionToSerial(reader, Serial);
  }
}
void loop()
{
  // Look for new cards.
  inc = 0;
  for (MFRC522 reader : readers) {
    if (reader.PICC_IsNewCardPresent()) {
      if (reader.PICC_ReadCardSerial()) {
        Serial.print(F("Reader "));
        Serial.print(inc+1);
        for (uint8_t i = 0; i < 4; i++) {
            uid[inc][i] = reader.uid.uidByte[i]; // Store each byte of the UID in the array
        }
        // Show some details of the PICC (that is: the tag/card).
        Serial.print(F(": Card UID:"));
        MFRC522Debug::PrintUID(Serial, reader.uid);
        Serial.println();
        
        Serial.print(F("PICC type: "));
        MFRC522::PICC_Type piccType = reader.PICC_GetType(reader.uid.sak);
        Serial.println(MFRC522Debug::PICC_GetTypeName(piccType));
        
        // Halt PICC.
        reader.PICC_HaltA();
        // Stop encryption on PCD.
        reader.PCD_StopCrypto1();
      }
    }
    //Serial.println(inc);
    inc++;
  }

if (uid[0][0] == NEGATION[0] && uid[0][1] == NEGATION[1] && uid[0][2] == NEGATION[2] && uid[0][3] == NEGATION[3]) {
  Serial.println("The UIDs match.");
}
  //MCP.write8(0,1);
//  MCP.write1(RD_1, HIGH);
//  delay(500);
//  MCP.write1(RD_1, LOW);
//  delay(500);
//  MCP.write1(GN_1, HIGH);
//  delay(500);
//  MCP.write1(GN_1, LOW);
//  delay(500);
//  MCP.write1(BE_1, HIGH);
//  delay(500);
//  MCP.write1(BE_1, LOW);
//  delay(500);

}