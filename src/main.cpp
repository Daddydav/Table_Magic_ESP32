#include <Arduino.h>
#include <Wire.h>
#include <MFRC522v2.h>
#include <MFRC522DriverSPI.h>
//#include <MFRC522DriverI2C.h>
#include <MFRC522DriverPinSimple.h>
#include <MFRC522Debug.h>

#include "MCP23017.h"
#include "header.h"

TwoWire I2C1 = TwoWire(0);
MCP23017 MCP(MCP23017_ADR, &I2C1);  // Adresse module ext

MFRC522DriverPinSimple ss1_pin(5); // Configurable, see typical pin layout above.
MFRC522DriverPinSimple ss2_pin(5);
MFRC522DriverPinSimple ss3_pin(13);
MFRC522DriverPinSimple ss4_pin(18);
MFRC522DriverPinSimple ss5_pin(25);

MFRC522DriverSPI driver_1{ss1_pin}; //Create SPI driver.
MFRC522DriverSPI driver_2{ss2_pin};

MFRC522 readers[]{driver_1, driver_2};   // Create MFRC522 instance.

String tagContent = "";

void setup()
{
  Serial.begin(115200); // Initialize serial communications with the PC for debugging.
  while (!Serial)
    ;                                              // Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4).
  static uint8_t i = 0;

  I2C1.begin(I2C_SDA, I2C_SCL, I2C_Freq);

  
  bool b = MCP.begin();

  Serial.println(b ? "true" : "false");
  MCP.pinMode16(0x0000);  //  0 = output , 1 = input
  MCP.setPullup16(0x0000);
  //MCP.write16(1);
  for (MFRC522 reader : readers) {
    reader.PCD_Init(); // Init each MFRC522 card.
    Serial.print(F("Reader "));
    i++;
    Serial.print(i);
    Serial.print(F(": "));
    MFRC522Debug::PCD_DumpVersionToSerial(reader, Serial);
  }
}
void loop()
{
  // Look for new cards.
  for (MFRC522 reader : readers) {
    if (reader.PICC_ReadCardSerial()) {
      if (reader.PICC_IsNewCardPresent()) {
        Serial.print(F("Reader "));
        static uint8_t i = 0;
        i++;
        Serial.print(i);
        
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