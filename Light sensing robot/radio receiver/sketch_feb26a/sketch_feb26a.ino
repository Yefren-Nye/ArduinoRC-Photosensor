#include <SoftwareSerial.h>

// Create a SoftwareSerial object
SoftwareSerial sbusSerial(10, 11); // RX on pin 10, TX not used

// SBUS variables
const int SBUS_FRAME_LENGTH = 25; // SBUS frame is 25 bytes
uint8_t sbusFrame[SBUS_FRAME_LENGTH];
int channels[16]; // Array to store channel values

void setup() {
  // Start Serial for debugging
  Serial.begin(115200);

  // Start SoftwareSerial at 100000 baud
  sbusSerial.begin(100000);
}

void loop() {
  // Check if data is available
  if (sbusSerial.available() >= SBUS_FRAME_LENGTH) {
    // Read the SBUS frame
    for (int i = 0; i < SBUS_FRAME_LENGTH; i++) {
      sbusFrame[i] = sbusSerial.read();
    }

    // Decode the SBUS frame
    decodeSBUS(sbusFrame, channels);

    // Print the channel values
    for (int i = 0; i < 16; i++) {
      Serial.print("Channel ");
      Serial.print(i + 1);
      Serial.print(": ");
      Serial.println(channels[i]);
    }
  }
}

// Function to decode SBUS frame
void decodeSBUS(uint8_t frame[25], int channels[16]) {
  // Decode channels 1-8
  channels[0]  = ((frame[1]       | frame[2]  << 8) & 0x07FF);
  channels[1]  = ((frame[2]  >> 3 | frame[3]  << 5) & 0x07FF);
  channels[2]  = ((frame[3]  >> 6 | frame[4]  << 2 | frame[5] << 10) & 0x07FF);
  channels[3]  = ((frame[5]  >> 1 | frame[6]  << 7) & 0x07FF);
  channels[4]  = ((frame[6]  >> 4 | frame[7]  << 4) & 0x07FF);
  channels[5]  = ((frame[7]  >> 7 | frame[8]  << 1 | frame[9] << 9) & 0x07FF);
  channels[6]  = ((frame[9]  >> 2 | frame[10] << 6) & 0x07FF);
  channels[7]  = ((frame[10] >> 5 | frame[11] << 3) & 0x07FF);

  // Decode channels 9-16
  channels[8]  = ((frame[12]      | frame[13] << 8) & 0x07FF);
  channels[9]  = ((frame[13] >> 3 | frame[14] << 5) & 0x07FF);
  channels[10] = ((frame[14] >> 6 | frame[15] << 2 | frame[16] << 10) & 0x07FF);
  channels[11] = ((frame[16] >> 1 | frame[17] << 7) & 0x07FF);
  channels[12] = ((frame[17] >> 4 | frame[18] << 4) & 0x07FF);
  channels[13] = ((frame[18] >> 7 | frame[19] << 1 | frame[20] << 9) & 0x07FF);
  channels[14] = ((frame[20] >> 2 | frame[21] << 6) & 0x07FF);
  channels[15] = ((frame[21] >> 5 | frame[22] << 3) & 0x07FF);
}