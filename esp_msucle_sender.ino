// Upload ONLY to the button/muscle ESP32.
// Buttons: GPIO32 to GND and GPIO33 to GND (not Uno pins 2/3).
// Uses internal pull-ups. Sends a state every 100 ms.
// Protocol: ASCII '0'=idle, '1'=correct, '2'=wrong, '3'=both.
#include <Arduino.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_SPP_ENABLED)
#error This sketch requires an original ESP32 with Bluetooth Classic SPP.
#endif

BluetoothSerial SerialBT;

const int BUTTON_1_PIN = 32;
const int BUTTON_2_PIN = 33;
String receiverName = "EMBS_Robot_RX";
unsigned long lastAttempt = 0;
unsigned long lastSend = 0;
bool attempted = false;

void setup()
{
  Serial.begin(115200);
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);

  if (!SerialBT.begin("EMBS_Muscle_TX", true)) // true = initiate connections
  {
    Serial.println("Bluetooth startup failed");
    while (true) { delay(1000); }
  }
  Serial.println("Sender ready. Power on receiver.");
}

void loop()
{
  if (!SerialBT.connected())
  {
    if (!attempted || millis() - lastAttempt >= 3000)
    {
      attempted = true;
      Serial.println("Connecting to EMBS_Robot_RX...");
      // Name discovery/connect can block during this standalone test.
      bool connected = SerialBT.connect(receiverName);
      lastAttempt = millis();
      Serial.println(connected ? "Connected" : "Connection failed; will retry");
    }
    delay(10);
    return;
  }

  if (millis() - lastSend >= 100)
  {
    lastSend = millis();
    bool muscle1 = digitalRead(BUTTON_1_PIN) == LOW;
    bool muscle2 = digitalRead(BUTTON_2_PIN) == LOW;
    uint8_t state = (muscle1 ? 1 : 0) | (muscle2 ? 2 : 0);
    SerialBT.write((uint8_t)('0' + state));
    Serial.print("Sent state: ");
    Serial.println(state);
  }
}

