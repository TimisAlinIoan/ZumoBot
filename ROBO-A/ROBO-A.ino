#include <Wire.h>
#include <ZumoShield.h>
#include <Servo.h>

#define LED 13
#define pingPin 6
#define pingPin2 5
#define crash A1
#define crash2 A2//da stabilire
#define servi 4
#define servi 2


#define QTR_THRESHOLD 1500  // microseconds


#define REVERSE_SPEED 200  // 0 is stopped, 400 is full speed
#define TURN_SPEED 200
#define FORWARD_SPEED 200
#define REVERSE_DURATION 200  // ms
#define TURN_DURATION 300     // ms
#define CENTER_TIME 500       // ms
#define SEARCH_SPEED 200
#define SERVO_ROT 180//da valutare

Servo servo1;
Servo servo2;

ZumoBuzzer buzzer;
ZumoMotors motors;
Pushbutton button(ZUMO_BUTTON);  // pushbutton on pin 12

#define NUM_SENSORS 6
unsigned int sensor_values[NUM_SENSORS];

ZumoReflectanceSensorArray sensors(QTR_NO_EMITTER_PIN);

void waitForButtonAndCountDown() {
  digitalWrite(LED, HIGH);
  button.waitForButton();
  digitalWrite(LED, LOW);

  // play audible countdown
  for (int i = 0; i < 3; i++) {
    delay(1000);
    buzzer.playNote(NOTE_G(3), 200, 15);
  }
  delay(1000);
  buzzer.playNote(NOTE_G(4), 500, 15);
  delay(1000);
}

void setup() {


  pinMode(LED, HIGH);

  waitForButtonAndCountDown();
  pinMode(crash,INPUT);
  pinMode(crash2,INPUT);
  servo1.attach(4);
  servo2.attach(2);
}

void loop() {
  if (button.isPressed()) {

    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    button.waitForRelease();
    waitForButtonAndCountDown();
  }


  sensors.read(sensor_values);

  if (sensor_values[0] >= QTR_THRESHOLD) {
    turn('R');
  } else if (sensor_values[5] >= QTR_THRESHOLD) {
    turn('L');
  } else {
    bool direzione = detect(pingPin);
    bool direzione2 = detect(pingPin2);

    if (direzione && direzione2)
      find(4);

    else if (direzione || direzione2) {
      if (direzione)
        find(3);
      else find(2);
    } else if(Crash())
      elevate();

      search_mode();
  }
}
bool Crash(){
  if(digitalRead(crash)==HIGH||digitalRead(crash2)==HIGH)
    return true;
  return false;

};
void elevate(){
  servo1.write(SERVO_ROT);
  servo2.write(SERVO_ROT) ;
  motors.setSpeeds(FORWARD_SPEED,FORWARD_SPEED);
  int sensored;
  int sensores;
  do{
    delay(100);
    sensors.read(sensor_values);
    sensores=sensor_values[0];
    sensored=sensor_values[5];

    
   } while(sensores<=QTR_THRESHOLD&&sensored<=QTR_THRESHOLD);
  motors.setSpeeds(0,0);
  servo1.write(0);
  servo2.write(0);

}
void turn(char direction) {
  if (direction == 'L') {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(-TURN_SPEED, TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(CENTER_TIME);
    motors.setSpeeds(0, 0);
  } else {
    motors.setSpeeds(-REVERSE_SPEED, -REVERSE_SPEED);
    delay(REVERSE_DURATION);
    motors.setSpeeds(TURN_SPEED, -TURN_SPEED);
    delay(TURN_DURATION);
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
    delay(CENTER_TIME);
    motors.setSpeeds(0, 0);
  }
}
void find(int sensors_imput) {
  if (sensors_imput == 4)
    motors.setSpeeds(FORWARD_SPEED, FORWARD_SPEED);
  else if (sensors_imput == 3)
    motors.setSpeeds(FORWARD_SPEED, -FORWARD_SPEED);
  else
    motors.setSpeeds(-FORWARD_SPEED, FORWARD_SPEED);
}
bool detect(int pingpin) {
  //inserire il codice del sensore giusto//
  long duration, cm;


  pinMode(pingpin, OUTPUT);
  digitalWrite(pingpin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingpin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingpin, LOW);


  pinMode(pingpin, INPUT);
  duration = pulseIn(pingpin, HIGH);



  cm = microsecondsToCentimeters(duration);
  if (cm < 70)
    return true;
  return false;
}
long microsecondsToCentimeters(long microseconds) {

  return microseconds / 29 / 2;
}
void search_mode() {
  motors.setSpeeds(SEARCH_SPEED, SEARCH_SPEED);
}
