#include <Servo.h>
#include <LiquidCrystal.h>

Servo myServo;
const int DIR_PIN = 2;      //winding direction pin HIGH = CW, LOW=CCW
const int STEPPER_PIN = 3;  //stepper motor pin
const int SERVO_PIN = 11;
const int BUTTON_PIN = 4;
const int STEPSPERREV = 200;
const int SERVOPOSMIN = 52;
const int SERVOPOSMAX = 78;

double wireWidth = 0.060;
double pipakWidth = 6.00;
int buttonState;
int revs = 0;
int factor = -1;
int counter;
int servoSpeed = 10;
int windingSpeed = 1200;
boolean switchButton = false;

double windingNum = pipakWidth / wireWidth;
double servoRange = SERVOPOSMAX - SERVOPOSMIN;
double stepNum = windingNum / servoRange;
LiquidCrystal lcd(12, 10, 13, 6, 7, 8, 9);

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Windings: ");
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
    delay(300);
    turnOnOffAgain();
    delay(300);
  }

  switch (switchButton) {
    case true:
      makeOneRev();
      lcd.setCursor(10, 0);
      lcd.print(revs);
      lcd.setCursor(0, 1);
      lcd.print("Running         ");
      break;
    case false:
      lcd.setCursor(0, 1);
      lcd.print("Waiting");
      break;
  }
}

void turnOnOffAgain() {
  switch (switchButton) {
    case true:
      switchButton = false;
      break;
    case false:
      switchButton = true;
      break;
  }
}

void makeOneRev() {
  for (int x = 0; x < STEPSPERREV; x++) {
    digitalWrite(STEPPER_PIN, HIGH);
    delayMicroseconds(windingSpeed);
    digitalWrite(STEPPER_PIN, LOW);
    delayMicroseconds(windingSpeed);
  }
  counter = counter + 1;

  if (counter >= round(stepNum)) {
    servoMove();
    counter = 0;
  }
  revs = revs + 1;
}

void servoMove() {
  int move = myServo.read() + factor;
  myServo.write(move);
  if (myServo.read() == SERVOPOSMAX) {
    factor = -1;
  } else if (myServo.read() == SERVOPOSMIN) {
    factor = 1;
  }
}

void calibrate() {
  lcd.setCursor(0, 1);
  lcd.print("     Calibrating");
  Serial.println("-- -- -- -- --");
  Serial.println("Winding number per layer: " + String(windingNum));
  Serial.println("Servo move range: " + String(servoRange));
  Serial.println("Move servo every " + String(round(stepNum)) + " revs");
  Serial.println("Reset counter to  0");
  myServo.write(SERVOPOSMIN);
  delay(1000);
  Serial.println("Reset servo position");
  myServo.write(SERVOPOSMAX);
  counter = 0;
  Serial.println("Ready.");
  lcd.setCursor(0, 1);
  lcd.print("           Ready");
}
