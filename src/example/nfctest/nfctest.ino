/*
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS 1    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI SS 2    SDA(SS)      ** custom, take a unused pin, only HIGH/LOW required **
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

// note: it should be known that global memory use is insecure and should be avoided, but I am short on time and this is easier to code.

#include <SPI.h>
#include <MFRC522.h>

// whenever a card with a known UID is detected, toggle the LED

#define LED_PIN 7
#define RST_PIN 9 // arbitrary, can be any GPIO pin
#define SS_PIN 10 // also arbitrary

MFRC522 rfid(SS_PIN, RST_PIN); // Create MFRC522 instance.
// MFRC522::MIFARE_Key key;
// byte uid[4];

// registered cards, hard coded for now
const byte uids[3][4] = {
    {0x7A, 0xB3, 0x2D, 0x68},
    {0xBA, 0x60, 0x22, 0x68},
    {0x95, 0xAB, 0xA0, 0xEE}};

void setup()
{
  Serial.begin(9600); // Initialize serial communications with the PC (optional)
  uint16_t stime = millis();
  while (!Serial && (millis() - stime) < 2000)
  {
    delay(1); // wait for serial port to connect. Needed for Leonardo only
  }
  Serial.println(F("Starting SPI"));
  SPI.begin();

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);
  // initialize the MFRC522
  Serial.println(F("Initializing MFRC522"));
  rfid.PCD_Init();
  // Serial.println(F("Setting keybytes"));
  // for (byte i = 0; i < 6; i++)
  // {
  //   key.keyByte[i] = 0xFF;
  // }

  // Serial.println(F("This code scan the MIFARE Classic NUID."));
  // Serial.print(F("Using the following key:"));
  // printHex(key.keyByte, MFRC522::MF_KEY_SIZE);
  // Serial.print(F("\n"));

  if (rfid.PCD_PerformSelfTest())
    Serial.println(F("Passed Self-Test"));

  // // set the pinmode of the LED pin to output
  Serial.println(F("setup complete"));

  // flash the LED 5 times fast to indicate that the setup is complete
  for (int i = 0; i < 5; i++)
  {
    digitalWrite(LED_PIN, HIGH);
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
  }
}

void loop()
{
  delay(1);
  switch (checkNFC())
  {
  case 0:
    return;
  case -1:
    Serial.print(F("Unrecognized card detected "));
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
    return;
  default:
    Serial.println(F("Card detected"));
    break;
  }
  printHex(rfid.uid.uidByte, rfid.uid.size);
  Serial.println();
  digitalWrite(LED_PIN, !digitalRead(LED_PIN));
  rfid.PICC_HaltA();
  rfid.PCD_StopCrypto1();
}

void printHex(byte *buffer, byte bufferSize)
{
  for (byte i = 0; i < bufferSize; i++)
  {
    Serial.print(buffer[i] < 0x10 ? " 0" : " ");
    Serial.print(buffer[i], HEX);
  }
}

/**
 * @brief Check for NFC card available, and return whether the card is recognized or not.
 *
 * @return int 1 if card is recognized, 0 if card is not found, -1 if card is unrecognized.
 */
int checkNFC()
{
  // check if a card is available
  if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
    return 0;

  // this is a bit of a hack, but it works.
  bool found = false;
  for (int u = 0; u < 3; u++)
  {
    byte *uid = uids[u];
    found = true;
    for (byte i = 0; i < 4; i++)
    {
      if (uid[i] != rfid.uid.uidByte[i])
      {
        found = false;
        break;
      }
    }
    if (found)
      return 1;
  }
  return -1;
}
