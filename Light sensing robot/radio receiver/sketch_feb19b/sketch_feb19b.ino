#include <sbus.h>
#include <SoftwareSerial.h>

/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial); // if Arduino is Uno: sbus_rx(&Serial)
/* SBUS object, writing SBUS */
bfs::SbusTx sbus_tx(&Serial); // if Arduino is Uno: sbus_tx(&Serial)
/* SBUS data */
bfs::SbusData data;
SoftwareSerial sbusSerial(10, 11); // RX, TX

void setup() {
  
  /* Serial to display data */
  Serial.begin(115200);
  sbusSerial.begin(100000); // SBUS baud rate
  Serial.println("SBUS Debugging Started!");
  while (!Serial) {}
  /* Begin the SBUS communication */
  sbus_rx.Begin();
  sbus_tx.Begin();
}

void loop () {
  if (sbus_rx.Read()) {
    /* Grab the received data */
    data = sbus_rx.data();
    /* Display the received data */
    for (int8_t i = 0; i < data.NUM_CH; i++) {
      Serial.print(data.ch[i]);
      Serial.print("\t");
    }
    /* Display lost frames and failsafe data */
    Serial.print(data.lost_frame);
    Serial.print("\t");
    Serial.println(data.failsafe);
    /* Set the SBUS TX data to the received data */
    sbus_tx.data(data);
    /* Write the data to the servos */
    sbus_tx.Write();
  }

  if (sbusSerial.available()) {
    uint8_t sbusData = sbusSerial.read();
    Serial.print("SBUS Data: ");
    Serial.println(sbusData, HEX); // Print in hexadecimal format
  }
}