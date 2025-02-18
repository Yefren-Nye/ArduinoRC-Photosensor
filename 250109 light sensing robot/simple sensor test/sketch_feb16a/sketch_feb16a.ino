void setup() {
  Serial.begin(9600);
}

void loop() {
  int leftValue = analogRead(A0); // Read the left sensor
  int rightValue = analogRead(A1); // Read the right sensor

  Serial.print("Left Sensor: ");
  Serial.print(leftValue);
  Serial.print(" | Right Sensor: ");
  Serial.println(rightValue);

  delay(500); // Wait 0.5 seconds before the next reading
}