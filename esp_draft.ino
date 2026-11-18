#include <ESP32Servo.h>
#include <DIYables_TFT_Shield.h>

// ---------- TFT wiring ----------
// Shield label -> ESP32 GPIO
const int TFT_D0 = 13;
const int TFT_D1 = 14;
const int TFT_D2 = 16;
const int TFT_D3 = 17;
const int TFT_D4 = 21;
const int TFT_D5 = 22;
const int TFT_D6 = 23;
const int TFT_D7 = 25;

const int TFT_RD  = 26;
const int TFT_WR  = 27;
const int TFT_RS  = 4;
const int TFT_CS  = 5;
const int TFT_RST = 15;

// Constructor order matters.
DIYables_TFT_ILI9486_Shield tft(
  TFT_D0, TFT_D1, TFT_D2, TFT_D3,
  TFT_D4, TFT_D5, TFT_D6, TFT_D7,
  TFT_RD, TFT_WR, TFT_RS, TFT_CS, TFT_RST
);

// ---------- Servo pins ----------
const int SERVO_1_PIN = 18;
const int SERVO_2_PIN = 19;

Servo hand_servo;
Servo elbow_servo;

// Placeholder angles: adjust for your mechanism.
const int elbow_angle = 90;
const int closed_hand_angle = 0;
const int open_hand_angle = 180;

// ---------- Muscle test buttons ----------
const int BUTTON_1_PIN = 32;  // Correct muscle
const int BUTTON_2_PIN = 33;  // Wrong muscle

bool muscle1;
bool muscle2;
int previous_state = -1;

// ---------- Colors ----------
const uint16_t BLACK  = 0x0000;
const uint16_t WHITE  = 0xFFFF;
const uint16_t GREEN  = 0x07E0;
const uint16_t RED    = 0xF800;
const uint16_t YELLOW = 0xFFE0;
const uint16_t CYAN   = 0x07FF;

// ---------- Servo functions ----------
void open_hand()
{
  hand_servo.write(open_hand_angle);
}

void close_hand()
{
  hand_servo.write(closed_hand_angle);
}

void confused_hand()
{
  // Blocks other logic for 1.8 seconds when called.
  for (int i = 0; i < 3; i++)
  {
    hand_servo.write(70);
    delay(300);

    hand_servo.write(110);
    delay(300);
  }
}

void elbow_up()
{
  elbow_servo.write(elbow_angle);
}

void elbow_down()
{
  elbow_servo.write(0);
}

// ---------- TFT face functions ----------
// Landscape drawing area: 480 x 320.
// TFT drawing appears immediately; no display.display() needed.

void face_background(const char* label, uint16_t color)
{
  tft.fillScreen(BLACK);
  tft.setTextWrap(false);
  tft.setTextSize(3);
  tft.setTextColor(color);
  tft.setCursor(20, 18);
  tft.print(label);
}

void happy_face()
{
  face_background("Correct muscle", GREEN);

  // Happy eyes
  tft.drawLine(100, 150, 140, 110, WHITE);
  tft.drawLine(140, 110, 180, 150, WHITE);

  tft.drawLine(300, 150, 340, 110, WHITE);
  tft.drawLine(340, 110, 380, 150, WHITE);

  // Smile
  tft.drawLine(175, 210, 205, 240, GREEN);
  tft.drawLine(205, 240, 275, 240, GREEN);
  tft.drawLine(275, 240, 305, 210, GREEN);
}

void sad_face()
{
  face_background("Wrong muscle", RED);

  tft.fillCircle(140, 140, 20, WHITE);
  tft.fillCircle(340, 140, 20, WHITE);

  // Frown
  tft.drawLine(175, 250, 205, 220, RED);
  tft.drawLine(205, 220, 275, 220, RED);
  tft.drawLine(275, 220, 305, 250, RED);
}

void confused_face()
{
  face_background("Both muscles", YELLOW);

  tft.drawCircle(140, 135, 35, WHITE);
  tft.fillCircle(340, 145, 15, WHITE);

  tft.drawLine(190, 235, 290, 215, YELLOW);

  tft.setTextSize(5);
  tft.setTextColor(YELLOW);
  tft.setCursor(400, 80);
  tft.print("?");
}

void sleepy_face()
{
  face_background("Idle", CYAN);

  // Closed eyes
  tft.fillRect(100, 140, 80, 5, WHITE);
  tft.fillRect(300, 140, 80, 5, WHITE);

  tft.drawCircle(240, 230, 16, CYAN);

  tft.setTextSize(3);
  tft.setTextColor(CYAN);
  tft.setCursor(390, 75);
  tft.print("zZ");
}

void display_disconnect()
{
  face_background("Disconnected", RED);

  // X eyes
  tft.drawLine(110, 110, 170, 170, RED);
  tft.drawLine(170, 110, 110, 170, RED);

  tft.drawLine(310, 110, 370, 170, RED);
  tft.drawLine(370, 110, 310, 170, RED);

  tft.fillRect(200, 225, 80, 5, WHITE);

  tft.setTextSize(2);
  tft.setTextColor(WHITE);
  tft.setCursor(130, 280);
  tft.print("Check connection");
}

// ---------- Setup ----------
void setup()
{
  Serial.begin(9600);

  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);

  hand_servo.setPeriodHertz(50);
  elbow_servo.setPeriodHertz(50);

  hand_servo.attach(SERVO_1_PIN);
  elbow_servo.attach(SERVO_2_PIN);

  tft.begin();
  tft.setRotation(1);

  Serial.println("Ready");
}

// ---------- Main logic ----------
void loop()
{
  muscle1 = digitalRead(BUTTON_1_PIN) == LOW;
  muscle2 = digitalRead(BUTTON_2_PIN) == LOW;

  int current_state;

  if (muscle1 && muscle2)
    current_state = 3;
  else if (muscle1)
    current_state = 1;
  else if (muscle2)
    current_state = 2;
  else
    current_state = 0;

  // Update the display only when the state changes.
  if (current_state != previous_state)
  {
    previous_state = current_state;

    switch (current_state)
    {
      case 0:
        Serial.println("Idle");
        sleepy_face();
        break;

      case 1:
        Serial.println("Correct muscle");
        happy_face();
        // Add your desired servo calls here.
        break;

      case 2:
        Serial.println("Wrong muscle");
        sad_face();
        break;

      case 3:
        Serial.println("Both muscles");
        confused_face();
        break;
    }
  }

  delay(30);
}
