
#include <MFRC522.h>

// whenever a card with a known UID is detected, toggle the LED
MFRC522 mfrc522(SS_PIN, RST_PIN);

void setup()
{
    // initialize the MFRC522
    mfrc522.PCD_Init();
    Serial.println("setup complete");
}

void loop()
{
    // check if a card is present
    if (mfrc522.PICC_IsNewCardPresent())
    {
        // if a card is present, try to read its UID
        if (mfrc522.PICC_ReadCardSerial())
        {
            // if the UID is known, toggle the LED
            Serial.println("UID: " + String(mfrc522.uid.uidByte[0], HEX) + " " + String(mfrc522.uid.uidByte[1], HEX) + " " + String(mfrc522.uid.uidByte[2], HEX) + " " + String(mfrc522.uid.uidByte[3], HEX));
        }
    }
    delay(1);
}
