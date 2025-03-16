#include "sbus.h"

/* SBUS object, reading SBUS */
bfs::SbusRx sbus_rx(&Serial); // if Arduino is Uno: sbus_rx(&Serial)
/* SBUS object, writing SBUS */
bfs::SbusTx sbus_tx(&Serial); // if Arduino is Uno: sbus_tx(&Serial)
/* SBUS data */
bfs::SbusData data;

void setup() {
  // Set custom baud rate for hardware UART
  uint16_t ubrr_value = 103; // UBRR value for 100,000 baud at 16 MHz clock
  UBRR0H = (ubrr_value >> 8); // Set high byte of UBRR
  UBRR0L = ubrr_value;        // Set low byte of UBRR
  UCSR0A &= ~(1 << U2X0);     // Disable double-speed mode
  UCSR0B = (1 << RXEN0);      // Enable receiver
  UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // Set frame format: 8 data bits, 1 stop bit, no parity
  /* Serial to display data */
  Serial.begin(115200);
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
}