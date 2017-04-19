/*
  Arduino breathalyzer: DIY alcohol tester with MQ-3 gas sensor and OLED display

  This is the code I use for a simple olcohol detector or "breathalYzer",
  using an Arduino Uno, MQ-3 gas sensor, OLED display and a couple of LEDs.

  Everything is connected like this:

  MQ-3 -> Arduino
  ---------------
  AD  -> A0
  DD  -> (not connected)
  GND -> GND
  VCC -> 5V

  OLED -> Arduino
  ---------------
  GND -> GND
  VCC -> 5V
  SCL -> D10
  SDA -> D9
  RES -> D13
  DC  -> D11

  LED -> Arduino
  --------------
  1 Green  -> D6
  2 Green  -> D5
  3 Yellow -> D4
  4 Yellow -> D3
  5 Red    -> D2

  See the blog post at https://oneguyoneblog.com/2017/04/12/arduino-breathalyzer-alcohol-tester-mq-3/
*/

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

// OLED pins, using software SPI:
#define OLED_MOSI   9
#define OLED_CLK   10
#define OLED_DC    11
#define OLED_CS    12
#define OLED_RESET 13
Adafruit_SSD1306 display(OLED_MOSI, OLED_CLK, OLED_DC, OLED_RESET, OLED_CS);

#if (SSD1306_LCDHEIGHT != 64)
#error("Height incorrect, please fix Adafruit_SSD1306.h!");
#endif

int TIME_UNTIL_WARMUP = 900; // warm up for 15 minutes!
unsigned long time;

int analogPin = 0;
int val = 0;

// LED pins
int led1 = 2; // Red
int led2 = 3; // Yellow
int led3 = 4; // Yellow
int led4 = 5; // Green
int led5 = 6; // Green

int ledState = LOW;
unsigned long previousMillis = 0;
long blinkInterval = 250;

void setup()   {
  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);
  pinMode(led4, OUTPUT);
  pinMode(led5, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);
  digitalWrite(led4, LOW);
  digitalWrite(led5, LOW);

  Serial.begin(9600);
  display.begin(SSD1306_SWITCHCAPVCC);
  display.clearDisplay();
}

void loop() {

  unsigned long currentMillis = millis();

  if (currentMillis - previousMillis >= blinkInterval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;

    // if the LED is off turn it on and vice-versa:
    if (ledState == LOW) {
      ledState = HIGH;
    } else {
      ledState = LOW;
    }
  }

  delay(100);

  val = readAlcohol();
  printTitle();
  printWarming();

  time = millis() / 1000;

  if (time <= TIME_UNTIL_WARMUP)
  {
    time = map(time, 0, TIME_UNTIL_WARMUP, 0, 100);
    display.drawRect(10, 50, 110, 10, WHITE); //Empty Bar
    display.fillRect(10, 50, time, 10, WHITE);
  } else
  {
    printTitle();
    printAlcohol(val);
    printAlcoholLevel(val);
  }
  display.display();

}

void printTitle()
{
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(22, 0);
  display.println("Breathalyzer");
}

void printWarming()
{
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(0, 20);
  display.println("Warming up");
}

void printAlcohol(int value)
{
  display.setTextSize(2);
  display.setTextColor(WHITE);
  display.setCursor(45, 25);
  display.println(val);
}

void printAlcoholLevel(int value)
{
  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(10, 55);

  if (value < 250)
  {
    display.println("You are sober.");
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, LOW);
    digitalWrite(led5, HIGH);
  }
  if (value >= 250 && value < 300)
  {
    display.println("One beer.");
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, LOW);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  }
  if (value >= 300 && value < 350)
  {
    blinkInterval = 750; // blinking slow
    display.println("2+ beers.");
    digitalWrite(led1, LOW);
    digitalWrite(led2, LOW);
    digitalWrite(led3, ledState); // blinking yellow LED
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  }
  if (value >= 350 && value < 450)
  {
    blinkInterval = 250; // blinking faster
    display.println("Many beers!");
    digitalWrite(led1, LOW);
    digitalWrite(led2, ledState); // blinking yellow LED
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  }
  if (value > 450)
  {
    blinkInterval = 25; // blinking fast
    display.println("You are drunk!");
    digitalWrite(led1, ledState); // blinking red LED
    digitalWrite(led2, HIGH);
    digitalWrite(led3, HIGH);
    digitalWrite(led4, HIGH);
    digitalWrite(led5, HIGH);
  }
}

int readAlcohol()
{
  int val = 0;
  int val1;
  int val2;
  int val3;

  display.clearDisplay();
  val1 = analogRead(analogPin);
  delay(10);
  val2 = analogRead(analogPin);
  delay(10);
  val3 = analogRead(analogPin);

  val = (val1 + val2 + val3) / 3;
  return val;
}
