// Pin Definitions
int sensorLeft = A0; // Left light sensor
int sensorRight = A1;// Right light sensor
String sensorReport = " sensor value is ";

int rMotor = 11;
int lMotor = 10;

int lMin = 1023, lMax = 0;
int rMin = 1023, rMax = 0;

int calibrationLED = 2;

unsigned long calibrationTime = 10000;

int i;

void setup() {
    Serial.begin(9600);
    pinMode(sensorLeft, INPUT);
    pinMode(sensorRight, INPUT);
    pinMode(calibrationLED, OUTPUT);
    pinMode(lMotor, OUTPUT);
    pinMode(rMotor, OUTPUT);
    sensorCalibration();
    digitalWrite(calibrationLED, LOW);
    for(i = 3; i < 7; i++) {
      pinMode(i, OUTPUT);
    }
}

void loop() {
  int lRange = lMax-lMin;
  int rRange = rMax-rMin;
  int leftValue = analogRead(sensorLeft);
  int rightValue = analogRead(sensorRight);
  if ((leftValue-lMin) + (rightValue-rMin) > (lRange + rRange) / 4) {
    int L = map(leftValue, lMin, lMax, 0, 255);
    int R = map(rightValue, rMin, rMax, 0, 255);
    analogWrite(lMotor, L/2);
    digitalWrite(3, HIGH);
    digitalWrite(4, LOW);
    analogWrite(rMotor, R/2);
    digitalWrite(5, HIGH);
    digitalWrite(6, LOW);
  } else {
    analogWrite(lMotor, 0);
    analogWrite(rMotor, 0);
    }

  compileReport(leftValue, rightValue);
}

void compileReport(int sensorLeft, int sensorRight) {
  Serial.println("Left");
  Serial.print(sensorReport);
  Serial.print(sensorLeft);
  Serial.println("Right");
  Serial.print(sensorReport);
  Serial.print(sensorRight);
  delay(100);
}

void sensorCalibration() {
  Serial.println("Calibrating light sensors");
  unsigned long startTime = millis();
  digitalWrite(calibrationLED, HIGH);
  while (millis() - startTime < calibrationTime) {
    int leftValue = analogRead(sensorLeft);
    int rightValue = analogRead(sensorRight);
    if (leftValue < lMin) lMin = leftValue;
    if (leftValue > lMax) lMax = leftValue;
    if (rightValue < rMin) rMin = rightValue;
    if (rightValue > rMax) rMax = rightValue;
    Serial.print(" L min ");
    Serial.print(lMin);
    Serial.print(" L max ");
    Serial.print(lMax);
    Serial.print(" R Min ");
    Serial.print(rMin);
    Serial.print(" R max ");
    Serial.print(rMax);
  }
}