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

#include <SPI.h>
#include <MFRC522.h>
// #include <SoftwareSerial.h>

// whenever a card with a known UID is detected, toggle the LED

#define LED_PIN 7
#define RST_PIN 9
#define SS_PIN 10 // take any pin, use SoftSerial to make it work

MFRC522 rfid(SS_PIN, RST_PIN); // Create MFRC522 instance.
MFRC522::MIFARE_Key key;
byte uid[4];

void setup()
{
    Serial.begin(9600); // Initialize serial communications with the PC (optional)
    uint16_t stime = millis();
    while (!Serial && (millis() - stime) < 1000)
    {
        delay(1); // wait for serial port to connect. Needed for Leonardo only
    }
    Serial.println(F("Starting SPI"));
    SPI.begin();
    // generate 3.3V pwm on _3V3_PIN
    // pinMode(_3V3_PIN, OUTPUT);
    // analogWrite(_3V3_PIN, 255 * (3.4 / 5.0));

    pinMode(LED_PIN, OUTPUT);
    digitalWrite(LED_PIN, LOW);
    // initialize the MFRC522
    Serial.println(F("Initializing MFRC522"));
    rfid.PCD_Init();
    Serial.println(F("Setting keybytes"));
    for (byte i = 0; i < 6; i++)
    {
        key.keyByte[i] = 0xFF;
    }

    Serial.println(F("This code scan the MIFARE Classic NUID."));
    Serial.print(F("Using the following key:"));
    printHex(key.keyByte, MFRC522::MF_KEY_SIZE);

    if (rfid.PCD_PerformSelfTest())
        Serial.println(F("Passed Self-Test"));

    // // set the pinmode of the LED pin to output
    Serial.println(F("\nsetup complete"));

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
    if (!rfid.PICC_IsNewCardPresent() || !rfid.PICC_ReadCardSerial())
        return;

    digitalWrite(LED_PIN, !digitalRead(LED_PIN));
    for (byte i = 0; i < 4; i++)
    {
        uid[i] = rfid.uid.uidByte[i];
    }
    printHex(rfid.uid.uidByte, rfid.uid.size);
    Serial.println();
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
