#include <Servo.h>

Servo myServo;
const int dirPin = 2;
const int stepPin = 3;
const int servoPin = 11;
int revs = 0;

int servoPosition = 64;
int servoSpeed = 30;
int servoPosMin = 42;
int servoPosMax = 64;
int targetRevs = 1;
double wireWidth = 0.06;
double pipakWidth = 6.00;
const int stepsPerRevolution = 200;
bool change = false;
int prev = 0;
double windingNum = pipakWidth / wireWidth;


void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(115200);
  myServo.attach(servoPin);
  myServo.write(servoPosition);
}

void loop() {
  digitalWrite(dirPin, HIGH);
  while (revs < targetRevs) {
    makeOneRev();
  }
  Serial.print(revs);
  delay(2000);
}

void makeOneRev() {
  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  revs = revs + 1;
}

void servoRun() {

  // for (servoPosition = servoPosMin; servoPosition <= servoPosMax; servoPosition += 1) {
  //   myServo.write(servoPosition);
  //   break;
  // }
  // for (servoPosition = servoPosMax; servoPosition >= servoPosMin; servoPosition -= 1) {
  //   myServo.write(servoPosition);
  //   break;
  // }
}
