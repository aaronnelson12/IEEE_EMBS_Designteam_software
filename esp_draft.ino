


#include <ESP32Servo.h> 
#include <LiquidCrystal.h>

// oled declaration
//#include <Wire.h>
//#include <Adafruit_GFX.h>
//#include <Adafruit_SSD1306.h>

// Adafruit_SSD1306 display(128, 64, &Wire, -1);
// Pin assignments

//esp pins
const int SERVO_1_PIN = 18;
const int SERVO_2_PIN = 19;

// LCD connections for esp: RS, E, D4, D5, D6, D7
LiquidCrystal lcd(23, 22, 21, 25, 26, 27);

// OLED connections
// #define SCREEN_WIDTH 128
// #define SCREEN_HEIGHT 64
// #define OLED_RESET -1
// #define OLED_ADDRESS 0x3C

// Adafruit_SSD1306 display(
  // SCREEN_WIDTH,
  // SCREEN_HEIGHT,
  // &Wire,
  // OLED_RESET
//);

// muscle sensors template and variables
const int BUTTON_1_PIN = 2;  // Correct muscle
const int BUTTON_2_PIN = 3;  // Wrong muscle

bool muscle1;
bool muscle2;

// variables
const int elbow_angle= 90;
const int closed_hand_angle=0;
const int open_hand_angle=180;
bool happy;
bool sad;
bool confused;
bool sleepy;
bool disconect;
bool hand_open;
bool hand_close;
bool elbow_is_up;
bool elbow_is_down;
int previous_state = -1; // will be used for the loop

// Hardware objects
Servo hand_servo;
Servo elbow_servo;



// outputs states

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
for (int i = 0; i < 3; i++) 
  {
    hand_servo.write(70);
    delay(300);

    hand_servo.write(110);
    delay(300);
  }
}

void happy_face()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Happy! :");

  lcd.setCursor(0, 1);
  lcd.print("Muscle correct"); 
/*  // display.clearDisplay(); // clears prev face 

  // Drawing eyes
  display.fillCircle(38, 23, 5, WHITE);
  display.fillCircle(90, 23, 5, WHITE);

  // Drawing face
  display.drawLine(40, 40, 50, 48, WHITE);
  display.drawLine(50, 48, 64, 52, WHITE);
  display.drawLine(64, 52, 78, 48, WHITE);
  display.drawLine(78, 48, 88, 40, WHITE);

  display.display() // sends drawing to OLED*/
}

void sad_face()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sad face :(");

  lcd.setCursor(0, 1);
  lcd.print("Wrong muscle");
/*
  //display.clearDisplay();

  // Draw eyes
  display.fillCircle(38, 23, 5, WHITE);
  display.fillCircle(90, 23, 5, WHITE);

  // Draw frowning mouth
  display.drawLine(40, 50, 50, 43, WHITE);
  display.drawLine(50, 43, 64, 40, WHITE);
  display.drawLine(64, 40, 78, 43, WHITE);
  display.drawLine(78, 43, 88, 50, WHITE);

  display.display();*/
}

void confused_face()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Confused!?");
  lcd.setCursor(0, 1);
  lcd.print("Both pressed");
  /*
  //display.clearDisplay();

  // Draw eyes
  display.fillCircle(38, 25, 5, WHITE);
  display.fillCircle(90, 25, 5, WHITE);

  // Draw uneven eyebrows
  display.drawLine(27, 12, 45, 8, WHITE);
  display.drawLine(82, 8, 100, 15, WHITE);

  // Draw confused mouth
  display.drawLine(48, 47, 60, 43, WHITE);
  display.drawLine(60, 43, 72, 48, WHITE);
  display.drawLine(72, 48, 80, 44, WHITE);

  display.display();*/
}

void sleepy_face()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sleepy... zZ");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");
/*
  //display.clearDisplay();

  // Draw eyes (closed)
  display.drawLine(27, 25, 49, 25, WHITE);
  display.drawLine(79, 25, 101, 25, WHITE);

  // Draw neutral mouth
  display.drawLine(48, 45, 80, 45, WHITE);

  // Draw sleep symbols
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(105, 8);
  display.print("Z");

  display.setCursor(112, 1);
  display.print("Z");

  display.display();
  */
}

void display_disconnect()
{
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Disconnected!");
  lcd.setCursor(0, 1);
  lcd.print("Check sensors");

  /*display.clearDisplay();

  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(15, 20);
  display.print("Disconnected!");

  display.setCursor(20, 40);
  display.print("Check sensors");

  display.display();*/
}
void elbow_up()
{
  elbow_servo.write(elbow_angle);
}

void elbow_down()
{
  elbow_servo.write(0);
}
void setup()
{
  Serial.begin(9600);

  // Initialize LCD
  lcd.begin(16, 2);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Ready");

  // Initialize OLED
  /* Wire.begin(21, 22);

  if (!display.begin(SSD1306_SWITCHCAPVCC, OLED_ADDRESS))
  {
    Serial.println("OLED initialization failed!");

    while (true)
    {
      delay(100);
    }
  }

  display.clearDisplay();
  display.display();

  sleepy_face();

  // Initialize servos
  hand_servo.attach(SERVO_1_PIN);
  elbow_servo.attach(SERVO_2_PIN);

  // set up button pins
  pinMode(BUTTON_1_PIN, INPUT_PULLUP);
  pinMode(BUTTON_2_PIN, INPUT_PULLUP);
  */
}

void loop() 
{
  // true = pressed
  muscle1 = digitalRead(BUTTON_1_PIN) == LOW;
  muscle2 = digitalRead(BUTTON_2_PIN) == LOW;

  // Your muscle logic here
  if (muscle1 && muscle2)
  {
    Serial.println("Both muscles");
    confused_face();
  }
  else if (muscle1)
  {
    Serial.println("Correct muscle");
    happy_face();
  }
  else if (muscle2)
  {
    Serial.println("Wrong muscle");
    sad_face();
  }
  else
  {
    Serial.println("Idle");
    sleepy_face();
  }

  delay(100);  // Slow down Serial output
}