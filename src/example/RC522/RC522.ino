#include <MFRC522.h>
#include <Wire.h>

MFRC522 *mfrc522;

void setup()
{
    Serial.begin(9600);
    mfrc522 = new MFRC522(Wire);
}

void loop()
{
    if (mfrc522->PICC_IsNewCardPresent())
    {
        if (mfrc522->PICC_ReadCardSerial())
        {
            Serial.print("Card UID: ");
            for (byte i = 0; i < mfrc522->uid.size; i++)
            {
                Serial.print(mfrc522->uid.uidByte[i] < 0x10 ? " 0" : " ");
                Serial.print(mfrc522->uid.uidByte[i], HEX);
            }
            Serial.println();
        }
    }
}