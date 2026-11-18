// Standalone Bluetooth Classic address reader for an original ESP32.
// Upload to each controller separately.
// Open Serial Monitor at 115200 baud, then press RESET/EN.
// Run this sketch separately from your controller/pairing sketch.

#include <Arduino.h>
#include "BluetoothSerial.h"
#include "esp_bt_device.h"

#if !defined(CONFIG_BT_SPP_ENABLED)
#error "Bluetooth Classic SPP is required. Select a compatible original ESP32 board."
#endif

BluetoothSerial SerialBT;

void setup() {
  Serial.begin(115200);
  delay(1000);

  if (!SerialBT.begin("ESP32_Address_Check")) {
    Serial.println("Bluetooth initialization failed. Press RESET/EN to retry.");
    return;
  }

  const uint8_t* address = esp_bt_dev_get_address();
  if (address == nullptr) {
    Serial.println("Could not read the Bluetooth address.");
    return;
  }

  Serial.println();
  Serial.printf("Bluetooth address: %02X:%02X:%02X:%02X:%02X:%02X\n",
                address[0], address[1], address[2],
                address[3], address[4], address[5]);
  Serial.println("Record this address and label which controller it belongs to.");
  Serial.println("Then restore your normal controller sketch.");
}

void loop() {
  // Nothing else needed. Reset the board to print its address again.
}
