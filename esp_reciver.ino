// Upload ONLY to the robot ESP32.
// Standalone test: prints states; no servo/LCD/OLED libraries required.
// Later merge Bluetooth setup/loop code into the robot's existing setup/loop.
// Add real face/servo calls where indicated below.
// Protocol: ASCII '0'=idle, '1'=correct, '2'=wrong, '3'=both.
#include <Arduino.h>
#include <BluetoothSerial.h>

#if !defined(CONFIG_BT_SPP_ENABLED)
#error This sketch requires an original ESP32 with Bluetooth Classic SPP.
#endif

BluetoothSerial SerialBT;

int currentState = -1;
bool freshData = false;
unsigned long lastReceived = 0;
const unsigned long DATA_TIMEOUT_MS = 1000;

void applyState(int state)
{
  // Called once per state change, including after a data timeout.
  switch (state)
  {
    case 0:
      Serial.println("Idle");
      // sleepy_face();
      break;
    case 1:
      Serial.println("Correct muscle");
      // happy_face();
      break;
    case 2:
      Serial.println("Wrong muscle");
      // sad_face();
      break;
    case 3:
      Serial.println("Both muscles");
      // confused_face();
      break;
  }
}

void setup()
{
  Serial.begin(115200);
  if (!SerialBT.begin("EMBS_Robot_RX"))
  {
    Serial.println("Bluetooth startup failed");
    while (true) { delay(1000); }
  }
  Serial.println("Receiver ready. Waiting for muscle data...");
}

void loop()
{
  // Drain queued bytes and apply only the newest valid state.
  int newestState = -1;
  while (SerialBT.available())
  {
    int incoming = SerialBT.read();
    if (incoming >= '0' && incoming <= '3')
    {
      newestState = incoming - '0';
      lastReceived = millis();
    }
  }

  if (newestState >= 0)
  {
    if (!freshData || newestState != currentState)
    {
      currentState = newestState;
      applyState(currentState);
    }
    freshData = true;
  }

  if (freshData &&
      (!SerialBT.hasClient() || millis() - lastReceived > DATA_TIMEOUT_MS))
  {
    freshData = false;
    currentState = -1;
    Serial.println("Disconnected or muscle data timed out");
    // display_disconnect();
    // Add your chosen safe actuator behavior here before enabling motion.
  }

  delay(1);
}

