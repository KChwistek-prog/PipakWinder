#include <Servo.h>

Servo myServo;
const int dirPin = 2;
const int stepPin = 3;
const int servoPin = 11;
const int stepsPerRevolution = 200;
const int servoPosMin = 65;
const int servoPosMax = 85;
double wireWidth = 0.06;
double pipakWidth = 6.00;

int revs = 0;
int factor = -1;
int counter = 0;

int servoSpeed = 30;
int targetRevs = 200;  // docelowa liczba nawojow

boolean firstRun = true;
double windingNum = pipakWidth / wireWidth;
int stepNum = windingNum / (servoPosMax - servoPosMin);

void setup() {
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  Serial.begin(115200);
  myServo.attach(servoPin);
  digitalWrite(dirPin, HIGH);
}

void loop() {
  if (firstRun == true) {
    myServo.write(servoPosMax);
    firstRun = false;
  }

  while (revs < targetRevs) {
    makeOneRev();
    Serial.println(revs);
  }
}

void makeOneRev() {
  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }
  counter = counter + 1;

  if (counter == stepNum) {
    servoMove();
    Serial.print("pyk ");
    counter = 0;
  }
  revs = revs + 1;
}

void servoMove() {
  int move = myServo.read() + factor;
  myServo.write(move);
  if (myServo.read() == servoPosMax) {
    factor = -1;
  } else if (myServo.read() == servoPosMin) {
    factor = 1;
  }
}
