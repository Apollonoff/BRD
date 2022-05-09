// int ledPin = 13;
// int tiltPin = 3;
// int val;

// void setup()
// {
//     pinMode(ledPin, OUTPUT);
//     pinMode(tiltPin, INPUT);
// }

// void loop()
// {
//     val = digitalRead(tiltPin);
//     if(val == HIGH)
//     {
//         digitalWrite(ledPin, HIGH);
//     }
//     else
//     {
//         digitalWrite(ledPin, LOW);
//     }
// }

int led = 13;
int shockSensor = 12;
int value;

void setup(){
    pinMode(led, OUTPUT);
    pinMode(shockSensor, INPUT);

}

void OvercomingRavine()
{
    value = digitalRead(shockSensor);
    if (value == HIGH) && (left && lmid && rmid && right)
    {
        
        digitalWrite(led, LOW);
        //Замедление скорости машинки
        analogWrite(SPEED_LEFT, 50 );
        analogWrite(SPEED_RIGHT, 50);
        delay(500);
    } 
    else if (left && lmid && rmid && right) {
        //Ускорение при подьеме
        digitalWrite(led, HIGH);
        analogWrite(SPEED_LEFT, 200);
        analogWrite(SPEED_RIGHT, 200);
        delay(500);
    }
    else {
        //Горизонтальные поверхности
        digitalWrite(SPEED_LEFT, SPEED);
        digitalWrite(SPEED_RIGHT, SPEED); 
    }
}