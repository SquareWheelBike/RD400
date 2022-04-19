
void setup()
{
    pinMode(11, OUTPUT);
    pinMode(9, OUTPUT);
    pinMode(7, OUTPUT);
    pinMode(5, OUTPUT);
}

void loop()
{
    digitalWrite(11, HIGH);
    digitalWrite(9, HIGH);
    digitalWrite(7, HIGH);
    digitalWrite(5, HIGH);
    delay(1000);
    digitalWrite(11, LOW);
    digitalWrite(9, LOW);
    digitalWrite(7, LOW);
    digitalWrite(5, LOW);
    delay(1000);
}