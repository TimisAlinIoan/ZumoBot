#include <Wire.h>
#include <ZumoShield.h>


#define LED 13
#define pingPin 6
#define crash 2
#define crash2 4




#define QTR_THRESHOLD 1500  // microseconds


#define REVERSE_SPEED 200  // 0 is stopped, 400 is full speed
#define TURN_SPEED 200
#define FORWARD_SPEED 200
#define REVERSE_DURATION 200  // ms
#define TURN_DURATION 300     // ms
#define FAST_SPEED 400 
#define SEARCH_SPEED 200
#define CENTER_TIME 200




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
   if( detect(pingPin))
     Retro(1);
    
    if (Crash())
      Retro(-1);
      

      search_mode();
  }
}
bool Crash(){
  if(digitalRead(crash)==HIGH||digitalRead(crash2)==HIGH)
    return true;
  return false;

};
void Retro(int val){
    motors.setSpeeds(val*FAST_SPEED,val* FAST_SPEED);
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
  if (cm < 7)
    return true;
  return false;
}
long microsecondsToCentimeters(long microseconds) {

  return microseconds / 29 / 2;
}
void search_mode() {
  motors.setSpeeds(SEARCH_SPEED, SEARCH_SPEED);
}
