/**
 * @file arduino.ino
 * @author cole fuerth
 * @brief debounced input pullups for inputs, and toggleable outputs
 * @version 0.2
 * @date 2022-04-14
 *
 * @copyright Copyright (c) 2022
 *
 */

// toggle debug mode (define before including rd400.h)
#define __DEBUG
#define __RFID

#include "rd400.h"
#include <SPI.h>
#ifdef __RFID
#include <MFRC522.h>
#include <EEPROM.h>
#endif

#define BRIGHTS_IN A1
#define BRIGHTS_OUT 6
#define LT_IN A2
#define LT_OUT 5
#define RT_IN A3
#define RT_OUT 4
#define UNDERGLOW_OUT 8

// #define SDA 2
// #define SCL 3
#define RFID_BUTTON 3 // button to jump over RFID
#define POWER_OUT 7   // main power
#define RST_PIN 9     // arbitrary, can be any GPIO pin
#define SS_PIN 10     // also arbitrary

BRIGHTS brights(BRIGHTS_OUT, new TOGGLE_BUTTON(BRIGHTS_IN));
TURNSIGNAL lt(LT_OUT, new TOGGLE_BUTTON(LT_IN));
TURNSIGNAL rt(RT_OUT, new TOGGLE_BUTTON(RT_IN));

IO *io_list[] = {&brights, &lt, &rt};

#ifdef __RFID
MFRC522 rfid(SS_PIN, RST_PIN); // Create MFRC522 instance.
// registered cards, hard coded for now
const byte uids[3][4] = {
    {0x7A, 0xB3, 0x2D, 0x68},
    {0xBA, 0x60, 0x22, 0x68},
    {0x95, 0xAB, 0xA0, 0xEE}};
int RFID_enable;
#endif

void setup()
{
    // begin by immediately initializing pins, as there is a delay when Serial starts
    pinMode(POWER_OUT, OUTPUT);
    digitalWrite(POWER_OUT, LOW);
    // initialize pinModes (not set when constructing above objects)
    for (auto i : io_list)
    {
        pinMode(i->pin, OUTPUT);
        // DEBUGLN("pin " + String(i->pin) + " output");
        pinMode(i->control->pin, INPUT_PULLUP);
        // DEBUGLN("pin " + String(i->control->pin) + " input");
    }
#ifdef __DEBUG
    Serial.begin(9600);
    for (unsigned long stime = millis(); !Serial && (millis() - stime) < 2000; delay(1))
        ;
#endif

    // underglow control pin is on all the time for now
    pinMode(UNDERGLOW_OUT, OUTPUT);
    digitalWrite(UNDERGLOW_OUT, HIGH);

#ifdef __RFID
    // if EEPROM byte is 0xFF, then we haven't seen the card yet
    if (EEPROM.read(0) == 0xFF)
    {
        DEBUGLN("initializing EEPROM...");
        // 0x00 indicates that we are going to use RFID to control the power
        EEPROM.write(0, 0x00);
    }

    pinMode(RFID_BUTTON, INPUT_PULLUP);
    delay(1);
    if (digitalRead(RFID_BUTTON) == LOW)
    {
        DEBUGLN("RFID button pressed, toggling NFC control");
        // 0x01 indicates that we are going to use RFID to control the power
        EEPROM.write(0, EEPROM.read(0) ^ 0x01);
    }

    RFID_enable = EEPROM.read(0);
    switch (RFID_enable)
    {
    case 0x00:
        DEBUGLN("NFC control enabled");
        break;
    case 0x01:
        DEBUGLN("NFC control disabled");
        break;
    default:
        DEBUGLN("NFC control unknown");
        break;
    }

    if (RFID_enable == 0x00)
    {
        digitalWrite(POWER_OUT, LOW);
        DEBUGLN(F("Starting SPI"));
        SPI.begin();
        DEBUGLN(F("Initializing MFRC522"));
        rfid.PCD_Init();
        if (rfid.PCD_PerformSelfTest())
            DEBUGLN(F("NFC Passed Self-Test"));
    }
    else
    {
        digitalWrite(POWER_OUT, HIGH);
    }
#else
    digitalWrite(POWER_OUT, HIGH);
#endif

    // flash the LEDs to indicate startup
    for (int i = 0; i < 5; i++)
    {
        digitalWrite(BRIGHTS_OUT, HIGH);
        digitalWrite(LT_OUT, HIGH);
        digitalWrite(RT_OUT, HIGH);
        digitalWrite(UNDERGLOW_OUT, LOW);
        delay(200);
        digitalWrite(BRIGHTS_OUT, LOW);
        digitalWrite(LT_OUT, LOW);
        digitalWrite(RT_OUT, LOW);
        digitalWrite(UNDERGLOW_OUT, HIGH);
        delay(200);
    }

    DEBUGLN("setup complete");
}

void loop()
{

    for (auto i : io_list)
    {
        i->update_state();
    }

#ifdef __RFID
static OneShotFlasher flash(5);
    if (RFID_enable == 0x00 && flash.state())
    {
        switch (checkNFC())
        {
        case 0:
            break;
        case -1:
            DEBUG(F("Unrecognized card detected "));
            printHex(rfid.uid.uidByte, rfid.uid.size);
            DEBUGLN();
            break;
        default:
            DEBUG(F("Card detected "));
            printHex(rfid.uid.uidByte, rfid.uid.size);
            DEBUGLN();
            digitalWrite(POWER_OUT, !digitalRead(POWER_OUT)); // toggle power
            rfid.PICC_HaltA();
            rfid.PCD_StopCrypto1();
            break;
        }
    }
#endif
    delay(20);
}

// ----------------- NFC FUNCTIONS -----------------

void printHex(byte *buffer, byte bufferSize)
{
    for (byte i = 0; i < bufferSize; i++)
    {
        DEBUG(buffer[i] < 0x10 ? " 0" : " ");
        DEBUG(String(buffer[i], HEX));
    }
}

/**
 * @brief Check for NFC card available, and return whether the card is recognized or not.
 *
 * @return int 1 if card is recognized, 0 if card is not found, -1 if card is unrecognized.
 */
#ifdef __RFID
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
#endif
