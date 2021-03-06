// Моторы подключаются к клеммам M1+,M1-,M2+,M2-  
// Motor shield использует четыре контакта 6,5,7,4 для управления моторами 
#define SPEED_LEFT       6
#define SPEED_RIGHT      5 
#define DIR_LEFT         7
#define DIR_RIGHT        4
#define LEFT_SENSOR_PIN  8
#define LMID_SENSOR_PIN  9
#define RMID_SENSOR_PIN 10
#define RIGHT_SENSOR_PIN 11
#define LED = 13
#define SHOCK_SENSOR = 12
 
// Скорость, с которой мы движемся вперёд (0-255)
#define SPEED            100
 
// Скорость прохождения сложных участков
#define SLOW_SPEED       35
 
#define BACK_SLOW_SPEED  30
#define BACK_FAST_SPEED  50
 
// Коэффициент, задающий во сколько раз нужно затормозить
// одно из колёс для поворота
#define BRAKE_K          4
 
#define STATE_FORWARD    0
#define STATE_RIGHT      1
#define STATE_LEFT       2
 
#define SPEED_STEP       2
 
#define FAST_TIME_THRESHOLD     500
 
int state = STATE_FORWARD;
int currentSpeed = SPEED;
int fastTime = 0;
 
void runForward() 
{
    state = STATE_FORWARD;
 
    fastTime += 1;
    if (fastTime < FAST_TIME_THRESHOLD) {
        currentSpeed = SLOW_SPEED;
    } else {
        currentSpeed = min(currentSpeed + SPEED_STEP, SPEED);
    }
 
    analogWrite(SPEED_LEFT, currentSpeed);
    analogWrite(SPEED_RIGHT, currentSpeed);
 
    digitalWrite(DIR_LEFT, HIGH);
    digitalWrite(DIR_RIGHT, HIGH);
}
 
void steerRight() 
{
    state = STATE_RIGHT;
    fastTime = 0;
 
    // Замедляем правое колесо относительно левого,
    // чтобы начать поворот
    analogWrite(SPEED_RIGHT, 0);
    analogWrite(SPEED_LEFT, SPEED);
 
    digitalWrite(DIR_LEFT, HIGH);
    digitalWrite(DIR_RIGHT, HIGH);
}
 
void steerLeft() 
{
    state = STATE_LEFT;
    fastTime = 0;
 
    analogWrite(SPEED_LEFT, 0);
    analogWrite(SPEED_RIGHT, SPEED);
 
    digitalWrite(DIR_LEFT, HIGH);
    digitalWrite(DIR_RIGHT, HIGH);
}
 
 
void stepBack(int duration, int state) {
    if (!duration)
        return;
 
    // В зависимости от направления поворота при движении назад будем
    // делать небольшой разворот 
    int leftSpeed = (state == STATE_RIGHT) ? BACK_SLOW_SPEED : BACK_FAST_SPEED;
    int rightSpeed = (state == STATE_LEFT) ? BACK_SLOW_SPEED : BACK_FAST_SPEED;
 
    analogWrite(SPEED_LEFT, leftSpeed);
    analogWrite(SPEED_RIGHT, rightSpeed);
 
    // реверс колёс
    digitalWrite(DIR_RIGHT, LOW);
    digitalWrite(DIR_LEFT, LOW);
 
    delay(duration);
}


void overcomingRavine()
{
    int value;

    value = digitalRead(SHOCK_SENSOR);
    if (value == HIGH) && (left && lmid && rmid && right)
    {
        
        digitalWrite(LED, LOW);
        //Замедление скорости машинки
        analogWrite(SPEED_LEFT, 50);
        analogWrite(SPEED_RIGHT, 50);
        delay(500);
    } 
    else if (left && lmid && rmid && right) {
        //Ускорение при подьеме
        digitalWrite(LED, HIGH);
        analogWrite(SPEED_LEFT, 200);
        analogWrite(SPEED_RIGHT, 200);
        delay(500);
    }
    else {
        //Горизонтальные поверхности
        digitalWrite(SPEED_LEFT, currentSpeed);
        digitalWrite(SPEED_RIGHT, currentSpeed); 
    }
}
 
 
void setup() 
{
    // Настраивает выводы платы 4,5,6,7 на вывод сигналов 
    for(int i = 4; i <= 7; i++)
        pinMode(i, OUTPUT);
 
    // Сразу едем вперёд
    runForward();
} 
 
void loop() 
{ 
    // Наш робот ездит по белому полю с чёрным треком. В обратном случае не нужно
    // инвертировать значения с датчиков
    boolean left = !digitalRead(LEFT_SENSOR_PIN);
    boolean lmid = !digitalRead(LMID_SENSOR_PIN);
    boolean rmid = !digitalRead(RMID_SENSOR_PIN);
    boolean right = !digitalRead(RIGHT_SENSOR_PIN);
 
    // В какое состояние нужно перейти?
    int targetState;
    
    if (left&&lmid&&rmid&&right == 1) {
        // обнаружение толстой черной линии
        overcomingRavine();
    } else if (lmid == rmid) {
        // под сенсорами всё белое или всё чёрное
        // едем вперёд
        targetState = STATE_FORWARD;
    } else if (left && lmid) {
        // левый сенсор упёрся в трек
        // поворачиваем налево
        targetState = STATE_LEFT;
    } else {
        targetState = STATE_RIGHT;
    }
 
    if (state == STATE_FORWARD && targetState != STATE_FORWARD) {
        int brakeTime = (currentSpeed > SLOW_SPEED) ?
            currentSpeed : 0;
        stepBack(brakeTime, targetState);
    }
 
    switch (targetState) {
        case STATE_FORWARD:
            runForward();
            break;
 
        case STATE_RIGHT:
            steerRight();
            break;
 
        case STATE_LEFT:
            steerLeft();
            break;
    }
 
}
