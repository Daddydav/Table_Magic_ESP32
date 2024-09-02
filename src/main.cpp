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

#include "BluetoothSerial.h"

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

BluetoothSerial SerialBT;
int received;// received value will be stored in this variable
char receivedChar;// received value will be stored as CHAR in this variable

const char Led1 ='1';
const char Led2 ='2';
const char Led3 ='3';
const char Led4 ='4';
const char Led5 ='5';
const char Rouge ='r';
const char Bleu ='b';
const char Vert ='g';
const char Violet ='v';
const char Turquoise ='t';
const char Jaune ='y';
const char Win ='w';
const char Off_led ='o';

//* Liaison vers RC522 :
//*     RTS   ->  21
//*     SDA voir plus bas
//*     MOSI  ->  23
//*     MISO  ->  19
//*     SCK   ->  18

TwoWire I2CD = TwoWire(0);
MCP23017 MCP(MCP23017_ADR, &I2CD);  //* Adresse module ext

////bool new_RFID[] = {false,false,false,false,false}, first_RFID[] = {false,false,false,false,false};
////int32_t test_card;
//bool win_quete[] = {false,false,false,false,false};
////uint8_t uid[5][4]; // Array to store UID returned by RC522
//MFRC522DriverPinSimple ss1_pin(5), ss2_pin(25), ss3_pin(26), ss4_pin(27), ss5_pin(33); // Configurable, see typical pin layout above.

//MFRC522DriverSPI driver_1{ss1_pin}, driver_2{ss2_pin}, driver_3{ss3_pin}, driver_4{ss4_pin}, driver_5{ss5_pin}; //Create SPI driver.

//MFRC522 readers[]{driver_1, driver_2, driver_3, driver_4, driver_5};   // Create MFRC522 instance.
uint8_t num_rune = 9, num_Quetes = 0; 
u_int16_t time_delay = 500;

void aff_win(){
  for (int i=0; i<5; i++){
    MCP.write1(RD[i], LOW);
    MCP.write1(GN[i], LOW);
    MCP.write1(BE[i], LOW);
  }
  delay(time_delay);
  for (int i=0; i<5; i++){
    MCP.write1(GN[i], HIGH);
  }
  delay(time_delay);
  for (int i=0; i<5; i++){
    MCP.write1(BE[i], HIGH);
  }
  delay(time_delay);
  for (int i=0; i<5; i++){
    MCP.write1(GN[i], LOW);
  }
  delay(time_delay);
  for (int i=0; i<5; i++){
    MCP.write1(RD[i], HIGH);
  }
  delay(time_delay);
  for (int i=0; i<5; i++){
    MCP.write1(BE[i], LOW);
  }
  delay(time_delay);
  for (int i=0; i<5; i++){
    MCP.write1(GN[i], HIGH);
  }
  delay(time_delay);
  for (int i=0; i<5; i++){
    MCP.write1(RD[i], LOW);
  }
  delay(time_delay);
}

void setup()
{
  Serial.begin(115200); //* Initialize serial communications with the PC for debugging.
  while (!Serial)
    ;                   //* Do nothing if no serial port is opened (added for Arduinos based on ATMEGA32U4).

  I2CD.begin(I2C_SDA, I2C_SCL, I2C_Freq);

  
  bool b = MCP.begin();
  
  SerialBT.begin("Magic_table"); //Bluetooth device name
  
  Serial.println(b ? "MCP -> true" : "MCP -> false");
  MCP.pinMode16(0x0000);
  MCP.setPullup16(0x0000);
  ////MCP.write16(1);
  /*
  for (MFRC522 reader : readers) {
    reader.PCD_Init(); //* Init each MFRC522 card.
    Serial.print(F("Reader "));
    num_rune++;
    Serial.print(num_rune);
    Serial.print(F(": "));
    MFRC522Debug::PCD_DumpVersionToSerial(reader, Serial);
    if(reader.PCD_GetVersion() == 0xFF) {MCP.write1(RD[num_rune], HIGH);}
    else {MCP.write1(GN[num_rune], HIGH);}
    delay(100);    
  }
  init_quetes();
  delay(3000);
  */
}
void loop()
{
  receivedChar =(char)SerialBT.read();
  /*
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
          //Serial.println("NO card");
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
  */
 if (SerialBT.available()) {
    
    SerialBT.print("Received:");// write on BT app
    SerialBT.println(receivedChar);// write on BT app      
    Serial.print ("Received:");//print on serial monitor
    Serial.println(receivedChar);//print on serial monitor    
    //SerialBT.println(receivedChar);//print on the app    
    //SerialBT.write(receivedChar); //print on serial monitor
    if(receivedChar == Led1)
    {
      num_rune = 0;
    }
    if(receivedChar == Led2)
    {
      num_rune = 1;
    }
    if(receivedChar == Led3)
    {
      num_rune = 2;
    }
    if(receivedChar == Led4)
    {
      num_rune = 3;
    }
    if(receivedChar == Led5)
    {
      num_rune = 4;
    }

    if(receivedChar == Rouge)
    {
      if (num_rune != 9)
      {
        MCP.write1(RD[num_rune], HIGH);
        MCP.write1(GN[num_rune], LOW);
        MCP.write1(BE[num_rune], LOW);
        num_rune = 9;
      } 
    }
    if(receivedChar == Vert)
    {
      if (num_rune != 9)
      {
        MCP.write1(RD[num_rune], LOW);
        MCP.write1(GN[num_rune], HIGH);
        MCP.write1(BE[num_rune], LOW);
        num_rune = 9;
      } 
    }
    if(receivedChar == Bleu)
    {
      if (num_rune != 9)
      {
        MCP.write1(RD[num_rune], LOW);
        MCP.write1(GN[num_rune], LOW);
        MCP.write1(BE[num_rune], HIGH);
        num_rune = 9;
      }
    }
    if(receivedChar == Violet)
    {
      if (num_rune != 9)
      {
        MCP.write1(RD[num_rune], HIGH);
        MCP.write1(GN[num_rune], LOW);
        MCP.write1(BE[num_rune], HIGH);
        num_rune = 9;
      }
    }
    if(receivedChar == Jaune)
    {
      if (num_rune != 9)
      {
        MCP.write1(RD[num_rune], HIGH);
        MCP.write1(GN[num_rune], HIGH);
        MCP.write1(BE[num_rune], LOW);
        num_rune = 9;
      }
    }
    if(receivedChar == Turquoise)
    {
      if (num_rune != 9)
      {
        MCP.write1(RD[num_rune], LOW);
        MCP.write1(GN[num_rune], HIGH);
        MCP.write1(BE[num_rune], HIGH);
        num_rune = 9;
      }
    }
    if(receivedChar == Win)
    {
      aff_win();
      for (int i=0; i<5; i++){
          MCP.write1(RD[i], LOW);
          MCP.write1(GN[i], LOW);
          MCP.write1(BE[i], LOW);
        }
        num_rune = 9;
    }
    if(receivedChar == Off_led)
    {
      if (num_rune != 9)
      {
        MCP.write1(RD[num_rune], LOW);
        MCP.write1(GN[num_rune], LOW);
        MCP.write1(BE[num_rune], LOW);
        num_rune = 9;
      }
    } 
/*
  if (value == "led_off") {
      for (int i=0; i<5; i++){
        MCP.write1(RD[i], LOW);
        MCP.write1(GN[i], LOW);
        MCP.write1(BE[i], LOW);
      }
    } else if (value == "red") {
      for (int i=0; i<5; i++){
        MCP.write1(RD[i], HIGH);
        MCP.write1(GN[i], LOW);
        MCP.write1(BE[i], LOW);
      }
    }
    else if (value == "green") {
      for (int i=0; i<5; i++){
        MCP.write1(RD[i], LOW);
        MCP.write1(GN[i], HIGH);
        MCP.write1(BE[i], LOW);
      }
    }
    else if (value == "blue") {
      for (int i=0; i<5; i++){
        MCP.write1(RD[i], LOW);
        MCP.write1(GN[i], LOW);
        MCP.write1(BE[i], HIGH);
      }
    }
    else if (value == "yellow") {
      for (int i=0; i<5; i++){
        MCP.write1(RD[i], LOW);
        MCP.write1(GN[i], HIGH);
        MCP.write1(BE[i], HIGH);
      }
    }
    else if (value == "purple") {
      for (int i=0; i<5; i++){
        MCP.write1(RD[i], HIGH);
        MCP.write1(GN[i], LOW);
        MCP.write1(BE[i], HIGH);
      }
    }
*/
  }
  /*
  //*test si quete réussi
  if(win_quete[0] && win_quete[1] && win_quete[2] && win_quete[3] && win_quete[4]){
    num_Quetes++;
    for (int i=0; i<5; i++) win_quete[i] = false;
    aff_win();
    ////for(i=0; i<10; i++) t[i] = t[i]+1;
  }
  delay(1000);
  */
}