int ledPin = 13;
int tiltPin = 3;
int val;

void setup()
{
    pinMode(ledPin, OUTPUT);
    pinMode(tiltPin, INPUT);
}

void loop()
{
    val = digitalRead(tiltPin);
    if(val == HIGH)
    {
        digitalWrite(ledPin, HIGH);
    }
    else
    {
        digitalWrite(ledPin, LOW);
    }
}