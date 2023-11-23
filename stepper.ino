#include <Servo.h>

Servo myServo;
const int DIR_PIN = 2;      //winding direction pin HIGH = CW, LOW=CCW
const int STEPPER_PIN = 3;  //stepper motor pin
const int SERVO_PIN = 11;
const int BUTTON_PIN = 4;

const int stepsPerRevolution = 200;
const int servoPosMin = 62;
const int servoPosMax = 98;

double wireWidth = 0.15;
double pipakWidth = 6.00;
int buttonState;
int revs = 0;
int factor = -1;
int counter;
int servoSpeed = 10;

double windingNum = pipakWidth / wireWidth;
double servoRange = servoPosMax - servoPosMin;
double stepNum = windingNum / (servoRange);

void setup() {
  pinMode(BUTTON_PIN, INPUT_PULLUP);
  pinMode(STEPPER_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  Serial.begin(115200);
  myServo.attach(SERVO_PIN);
  digitalWrite(DIR_PIN, LOW);
  calibrate();
}

void loop() {

  buttonState = digitalRead(BUTTON_PIN);
  if (buttonState == LOW) {
    makeOneRev();
  }
}

void makeOneRev() {
  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(STEPPER_PIN, HIGH);
    delayMicroseconds(900);
    digitalWrite(STEPPER_PIN, LOW);
    delayMicroseconds(900);
  }
  counter = counter + 1;

  if (counter >= stepNum) {
    servoMove();
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

void calibrate() {
  Serial.println("-- -- -- -- --");
  Serial.println("Winding number per layer: " + String(windingNum));
  Serial.println("Servo move range: " + String(servoRange));
  Serial.println("Move servo every " + String(stepNum) + " revs");
  Serial.println("Reset servo pos to  0");
  myServo.write(servoPosMax);
  Serial.println("Reset counter to  0");
  counter = 0;
}
