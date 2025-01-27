#include "RF24.h"
#include <SPI.h>
#define CE_PIN D4
#define CSN_PIN D2
constexpr int SPI_SPEED = 16000000;
RF24 radio(CE_PIN, CSN_PIN, SPI_SPEED);
int bluetooth_channels[] = { 32, 34, 46, 48, 50, 52, 0, 1, 2, 4, 6, 8, 22, 24, 26, 28, 30, 74, 76, 78, 80 };
int ble_channels[] = { 2, 26, 80 };

int currentMode = 0;


void configureRadio(RF24 &radio, int channel, SPIClass *spi);
void handleModeChange();
void executeMode();
void updateNeoPixel();
void jamBLE();
void jamBluetooth();
void jamAll();

void setup() {
  Serial.begin(115200);

  radio.begin();
  configureRadio(radio, bluetooth_channels[0]);
}

void configureRadio(RF24 &radio, int channel) {
  if (radio.begin()) {
    radio.setAutoAck(false);
    radio.stopListening();
    radio.setRetries(0, 0);
    radio.setPALevel(RF24_PA_MAX, true);
    radio.setDataRate(RF24_2MBPS);
    radio.setCRCLength(RF24_CRC_DISABLED);
    radio.startConstCarrier(RF24_PA_HIGH, channel);
  }
}

void loop() {
  // Check if data is available on the serial port
  if (Serial.available() > 0) {
    // Read the incoming byte as a character
    char incomingByte = Serial.read(); // Read as char

    // Check if the incoming byte corresponds to a valid mode
    if (incomingByte >= '0' && incomingByte <= '3') {
      currentMode = incomingByte - '0'; // Convert char to int
      Serial.print("Selected mode: ");
      Serial.println(currentMode);
    } else if (incomingByte == '\n' || incomingByte == '\r') {
      // Ignore newline characters
      return; // Exit the loop iteration
    } else {
      Serial.println("Invalid mode. Please enter a number between 0 and 3.");
    }
  }

  executeMode();
}
void handleModeChange() {
  currentMode = (currentMode + 1) % 4;
  Serial.print("Mode changed to: ");
  Serial.println(currentMode);
}


void executeMode() {
  switch (currentMode) {
    case 0:
      //radio.powerDown();
      Serial.println("Power Off");
      //radioHSPI.powerDown();
      delay(100);
      break;
    case 1:
      jamBLE();
      Serial.println("JamBLE");
      break;
    case 2:
      jamBluetooth();
      Serial.println("JamBluetooth");
      break;
    case 3:
      jamAll();
      Serial.println("JamAll");
      break;
  }
}

void jamBLE() {
  int randomIndex = random(0, sizeof(ble_channels) / sizeof(ble_channels[0]));
  int channel = ble_channels[randomIndex];
  radio.setChannel(channel);
}

void jamBluetooth() {
  int randomIndex = random(0, sizeof(bluetooth_channels) / sizeof(bluetooth_channels[0]));
  int channel = bluetooth_channels[randomIndex];
  radio.setChannel(channel);
}

void jamAll() {
  if (random(0, 2)) {
    jamBluetooth();
  } else {
    jamBLE();
  }
  //delayMicroseconds(20);
}