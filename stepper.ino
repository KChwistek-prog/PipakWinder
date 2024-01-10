#include <Servo.h>
#include <LiquidCrystal.h>  //Dołączenie bilbioteki

Servo myServo;
const int DIR_PIN = 2;      //winding direction pin HIGH = CW, LOW=CCW
const int STEPPER_PIN = 3;  //stepper motor pin
const int SERVO_PIN = 11;
const int BUTTON_PIN = 4;

const int stepsPerRevolution = 200;
const int servoPosMin = 54;
const int servoPosMax = 88;

double wireWidth = 0.060;
double pipakWidth = 6.00;
int buttonState;
int revs = 0;
int factor = -1;
int counter;
int servoSpeed = 10;

double windingNum = pipakWidth / wireWidth;
double servoRange = servoPosMax - servoPosMin;
double stepNum = windingNum / servoRange;
LiquidCrystal lcd(12, 10, 13, 6, 7, 8, 9);

boolean firstRun = true;

void setup() {
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("Nawoje: 0");
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
    lcd.setCursor(8, 0);
    lcd.print(revs);
  }
}

void makeOneRev() {
  for (int x = 0; x < stepsPerRevolution; x++) {
    digitalWrite(STEPPER_PIN, HIGH);
    delayMicroseconds(1400);
    digitalWrite(STEPPER_PIN, LOW);
    delayMicroseconds(1400);
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
  Serial.println("Move servo every " + String(round(stepNum)) + " revs");
  Serial.println("Reset counter to  0");
  myServo.write(servoPosMin);
  delay(1000);
  Serial.println("Reset servo pos to  0");
  myServo.write(servoPosMax);
  counter = 0;
}
