#include <stdbool.h>

// Pin Definitions
int forwardLeft = A0; // Left forward light sensor
int forwardRight = A1;// Right forward light sensor
int aftLeft = A2;// Left aft light sensor
int aftRight = A3;// Right aft light sensor

String sensorReport = " sensor value is ";

//PWM pins

int rMotor = 11;
int lMotor = 10;

//Min and max values for sensors

int flMin = 1023, flMax = 0;
int frMin = 1023, frMax = 0;
int alMin = 1023, alMax = 0;
int arMin = 1023, arMax = 0;

int calibrationLED = 2;

unsigned long calibrationTime = 10000;

int i;

//Booleans for quadrant steering: forward, left, right, aft

bool FRONT = false;
bool LEFT = false;
bool RIGHT = false;
bool AFT = false;

int fullSpeed = 255;
int halfSpeed = 127;
int quarterSpeed = 63;

// Boolean for manual control

bool manualControl = false;

void setup() {
    Serial.begin(9600);
    pinMode(forwardLeft, INPUT);
    pinMode(forwardRight, INPUT);
    pinMode(aftLeft, INPUT);
    pinMode(aftRight, INPUT);
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

  if (manualControl == false) {
  //Find the quadrant to steer towards
  isQuadrant();

  int forwardLeftValue = analogRead(forwardLeft);
  int forwardRightValue = analogRead(forwardRight);
  int aftLeftValue = analogRead(aftLeft);
  int aftRightValue = analogRead(aftRight);

  if (FRONT == true) {steerForward();}
  else if (AFT == true) {steerAft();}
  else if (LEFT == true) {steerLeft();}
  else if (RIGHT == true) {steerRight();}
  else {fullStop();}

  compileReport(forwardLeftValue, forwardRightValue);
  delay(100);
  } else {manualSteering();}
}

void compileReport(int forwardLeft, int forwardRight) {

  Serial.println("Left");
  Serial.print(sensorReport);
  Serial.print(forwardLeft);
  Serial.println("Right");
  Serial.print(sensorReport);
  Serial.print(forwardRight);

}

void sensorCalibration() {

  Serial.println("Calibrating...");

  unsigned long startTime = millis();

  digitalWrite(calibrationLED, HIGH);

  while (millis() - startTime < calibrationTime) {

    //Added to 
    fullStop();

    //Calibration sequence

    //Forward values
    int forwardLeftValue = analogRead(forwardLeft);
    int forwardRightValue = analogRead(forwardRight);

    if (forwardLeftValue < flMin) flMin = forwardLeftValue;
    if (forwardLeftValue > flMax) flMax = forwardLeftValue;
    if (forwardRightValue < frMin) frMin = forwardRightValue;
    if (forwardRightValue > frMax) frMax = forwardRightValue;

    Serial.print(" fL min ");
    Serial.print(flMin);
    Serial.print(" fL max ");
    Serial.print(flMax);
    Serial.print(" fR Min ");
    Serial.print(frMin);
    Serial.print(" fR max ");
    Serial.print(frMax);

    //Aft values
    int aftLeftValue = analogRead(aftLeft);
    int aftRightValue = analogRead(aftRight);

    if (aftLeftValue < alMin) alMin = aftLeftValue;
    if (aftLeftValue > alMax) alMax = aftLeftValue;
    if (aftRightValue < arMin) arMin = aftRightValue;
    if (aftRightValue > arMax) arMax = aftRightValue;

    Serial.print(" aL min ");
    Serial.print(alMin);
    Serial.print(" aL max ");
    Serial.print(alMax);
    Serial.print(" aR Min ");
    Serial.print(arMin);
    Serial.print(" aR max ");
    Serial.print(arMax);
  }
}

void fullStop() {

    analogWrite(lMotor, 0);
    analogWrite(rMotor, 0);
    Serial.println("Full stop");

}

void isQuadrant() {

  FRONT = false;
  LEFT = false;
  RIGHT = false;
  AFT = false;

  int fl = analogRead(forwardLeft);
  int fr = analogRead(forwardRight);
  int al = analogRead(aftLeft);
  int ar = analogRead(aftRight);

  int f = fl+fr;
  int l = fl+al;
  int r = fr+ar;
  int a = al+ar;

  if(f > l && f > r && f > a) {
    FRONT = true;
    Serial.println("Forward quadrant");}
  else if (l > r && l > a && l > a) {
    LEFT = true;
    Serial.println("Left quadrant");}
  else if (r > f && r > l && r > a) {
    RIGHT = true;
    Serial.println("Right quadrant");}
  else if (a > f && a > l && a > r) {
    AFT = true;
    Serial.println("Aft quadrant");}
  else {Serial.println("No quadrant found");}
}

void steerForward() {
  int forwardLeftValue = analogRead(forwardLeft);
  int forwardRightValue = analogRead(forwardRight);

  int L = map(forwardLeftValue, 0, 1023, 0, 255);
  int R = map(forwardRightValue, 0, 1023, 0, 255);

  //Left motor forward with feedback
  analogWrite(lMotor, R/2);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  Serial.println("Left speed ");
  Serial.print(R/2);

  //Right motor forward with feedback
  analogWrite(rMotor, L/2);
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  Serial.println("Right speed ");
  Serial.print(L/2);
}

void steerAft() {
  int aftLeftValue = analogRead(aftLeft);
  int aftRightValue = analogRead(aftRight);

  int L = map(aftLeftValue, 0, 1023, 0, 255);
  int R = map(aftRightValue, 0, 1023, 0, 255);

  //Left motor backward with feedback
  analogWrite(lMotor, R/2);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  Serial.println("Left speed ");
  Serial.print(R/2);

  //Right motor backward with feedback
  analogWrite(rMotor, L/2);
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  Serial.println("Right speed ");
  Serial.print(L/2);
}

void steerLeft() {
  int forwardLeftValue = analogRead(forwardLeft);
  int aftLeftValue = analogRead(aftLeft);

  if (forwardLeftValue > aftLeftValue) {

  //Left motor backward with feedback
  analogWrite(lMotor, quarterSpeed);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  Serial.println("Left speed ");
  Serial.print(quarterSpeed);

  //Right motor forward with feedback
  analogWrite(rMotor, quarterSpeed);
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  Serial.println("Right speed ");
  Serial.print(quarterSpeed);
  } else {
  //Left motor forward with feedback
  analogWrite(lMotor, quarterSpeed);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  Serial.println("Left speed ");
  Serial.print(quarterSpeed);

  //Right motor backward with feedback
  analogWrite(rMotor, quarterSpeed);
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  Serial.println("Right speed ");
  Serial.print(quarterSpeed);}
}

void steerRight() {
  int forwardRightValue = analogRead(forwardRight);
  int aftRightValue = analogRead(aftRight);

  if (forwardRightValue > aftRightValue) {

  //Left motor forward with feedback
  analogWrite(lMotor, quarterSpeed);
  digitalWrite(3, HIGH);
  digitalWrite(4, LOW);
  Serial.println("Left speed ");
  Serial.print(quarterSpeed);

  //Right motor backward with feedback
  analogWrite(rMotor, quarterSpeed);
  digitalWrite(5, LOW);
  digitalWrite(6, HIGH);
  Serial.println("Right speed ");
  Serial.print(quarterSpeed);

  } else {

  //Left motor backward with feedback
  analogWrite(lMotor, quarterSpeed);
  digitalWrite(3, LOW);
  digitalWrite(4, HIGH);
  Serial.println("Left speed ");
  Serial.print(quarterSpeed);

  //Right motor forward with feedback
  analogWrite(rMotor, quarterSpeed);
  digitalWrite(5, HIGH);
  digitalWrite(6, LOW);
  Serial.println("Right speed ");
  Serial.print(quarterSpeed);
  }
}

void manualSteering() {}