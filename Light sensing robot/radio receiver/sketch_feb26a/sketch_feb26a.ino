//This is code copied from deepseek, I'm trying to get my arduino UNO to take SBUS data sent by a receiver and convert it for use in controlling DC motors.

#include <SoftwareSerial.h>

#include <sbus.h>

// Define SoftwareSerial pins for SBUS
#define SBUS_RX_PIN 10;  // Connect SBUS receiver to pin 10

// Create a SoftwareSerial object for SBUS
SoftwareSerial(uint8_t SBUS_RX_PIN, uint8_t transmitPin, bool inverse_logic = false);; // No TX pin needed for receiving

void setup() {
  // Initialize Serial for debugging (Serial Monitor)
  Serial.begin(115200);
  while (!Serial) {}

  // Initialize SoftwareSerial for SBUS
  sbusSerial.begin(100000);
}

// Create an SBUS object
SBUS sbus(sbusSerial);

// SBUS data
uint16_t channels[16];
bool lostFrame = false;
bool failsafe = false;

void setup() {
  // Initialize Serial for debugging (Serial Monitor)
  Serial.begin(115200);
  while (!Serial) {}

  // Initialize SBUS
  sbus.begin();
}

void loop() {
  // Check for new SBUS data
  if (sbus.read(&channels[0], &lostFrame, &failsafe)) {
    // Display the received data
    for (int8_t i = 0; i < 16; i++) {
      Serial.print("Channel ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(channels[i]);
    }

    // Display lost frames and failsafe data
    Serial.print("Lost Frame: ");
    Serial.println(lostFrame);
    Serial.print("Failsafe: ");
    Serial.println(failsafe);

    // Use the channel values to control outputs
    int motorSpeed = map(channels[0], 0, 2047, 0, 255); // Map channel 0 to motor speed
    analogWrite(9, motorSpeed); // Control motor on pin 9
  }




/* SECOND SET, slightly simpler to try to implement later.

#include <SBUS.h>

// Define SoftwareSerial pins for SBUS
#define SBUS_RX_PIN 10  // Connect SBUS receiver to pin 10

// Create a SoftwareSerial object for SBUS
SoftwareSerial sbusSerial(SBUS_RX_PIN, SBUS_TX_PIN); // No TX pin needed for receiving

// Create an SBUS object
SBUS sbus(sbusSerial);

// SBUS data
uint16_t channels[16];
bool lostFrame = false;
bool failsafe = false;

void setup() {
  // Initialize Serial for debugging (Serial Monitor)
  Serial.begin(115200);
  while (!Serial) {}

  // Initialize SBUS
  sbus.begin();
}

void loop() {
  // Check for new SBUS data
  if (sbus.read(&channels[0], &lostFrame, &failsafe)) {
    // Display the received data
    for (int8_t i = 0; i < 16; i++) {
      Serial.print("Channel ");
      Serial.print(i);
      Serial.print(": ");
      Serial.println(channels[i]);
    }

    // Display lost frames and failsafe data
    Serial.print("Lost Frame: ");
    Serial.println(lostFrame);
    Serial.print("Failsafe: ");
    Serial.println(failsafe);

    // Use the channel values to control outputs
    int motorSpeed = map(channels[0], 0, 2047, 0, 255); // Map channel 0 to motor speed
    analogWrite(9, motorSpeed); // Control motor on pin 9
  }
}*/