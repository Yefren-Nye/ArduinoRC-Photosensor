#include <sbus.h>

// Create an SBUS object
bfs::SbusRx sbus(&Serial1, false);

// Array to hold the channel data
uint16_t channels[16];
bool failSafe;
bool lostFrame;

void setup() {
  // Start serial communication for debugging
  Serial.begin(115200);
  Serial.println("Initializing SBUS...");

  // Initialize SBUS at 100000 baud
  sbus.begin();
  Serial.println("SBUS initialized. Waiting for data...");
}

void loop() {
  // Read the SBUS data
  if (sbus.read(&channels[0], &failSafe, &lostFrame)) {
    // Print the channel values
    for (int i = 0; i < 16; i++) {
      Serial.print("Channel ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(channels[i]);
    }

    // Print lost frame and failsafe status
    Serial.print("Lost Frame: ");
    Serial.print(lostFrame);
    Serial.print("\t");
    Serial.print("Failsafe: ");
    Serial.println(failSafe);
    Serial.println();
  } else {
    Serial.println("No SBUS data received.");
  }

  // Add a small delay
  delay(10);
}