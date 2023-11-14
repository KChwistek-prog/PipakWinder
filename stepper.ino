#include <Servo.h>
#include <AccelStepper.h>
#define motorInterfaceType 1

Servo myServo;
const int dirPin = 2;
const int stepPin = 3;
const int servoPin = 11;
int servoPosition = 65;
int servoSpeed = 100;
int targetRevs = 15;
AccelStepper myStepper(motorInterfaceType, stepPin, dirPin);

void setup() {
  Serial.begin(9600);
  myServo.attach(servoPin);
  myServo.write(servoPosition);
  myStepper.setMaxSpeed(800);
  myStepper.setSpeed(800);
}

void loop() {
  while (stepperRevs() < targetRevs) {
    myStepper.runSpeed();
    Serial.println(stepperRevs());
  }
  //servoRun();
}

int stepperRevs() {
  return myStepper.currentPosition() / 200;
}

void servoRun() {
  for (servoPosition = 40; servoPosition <= 65; servoPosition += 1) {
    myServo.write(servoPosition);
    delay(servoSpeed);
  }
  for (servoPosition = 65; servoPosition >= 40; servoPosition -= 1) {
    myServo.write(servoPosition);
    delay(servoSpeed);
  }
}
