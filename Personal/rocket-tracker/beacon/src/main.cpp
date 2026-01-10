#include <Arduino.h>

// Function prototypes
void on();
void off();
void dot();
void dash();
void symbol_space_delay();
void letter_space_delay();
void word_space_delay();

// basic beacon blinking on and off using gpio 1
#define BEACON_PIN 2
#define LED_PIN 10
#define dot_duration 100
#define dash_duration 300
#define symbol_space 150
#define letter_space 300
#define word_space 400

#define HIGHVAL 255
#define LOWVAL 1

void setup()
{
  Serial.begin(115200);
  pinMode(BEACON_PIN, OUTPUT);
  pinMode(LED_PIN, OUTPUT);
}

void loop()
{
  // blink SAM very quickly (under 1 sec) *** *- --
  // S = dot dot dot
  Serial.println("S");
  dot();
  dot();
  dot();
  letter_space_delay();
  // A = dot dash
  Serial.println("A");
  dot();
  dash();
  letter_space_delay();
  // M = dash dash
  Serial.println("M");
  dash();
  dash();
  word_space_delay();
}

void on()
{
  // digitalWrite(BEACON_PIN, HIGHVAL);
  // digitalWrite(LED_PIN, HIGHVAL);
  analogWrite(LED_PIN, HIGHVAL);
  analogWrite(BEACON_PIN, HIGHVAL);
}

void off()
{
  // digitalWrite(BEACON_PIN, LOWVAL);
  // digitalWrite(LED_PIN, LOWVAL);
  analogWrite(LED_PIN, LOWVAL);
  analogWrite(BEACON_PIN, LOWVAL);
}

void symbol_space_delay()
{
  delay(symbol_space);
}

void letter_space_delay()
{
  delay(letter_space);
}

void word_space_delay()
{
  delay(word_space);
}

void dot()
{
  on();
  Serial.println("Dot");
  delay(dot_duration);
  off();
  symbol_space_delay();
}

void dash()
{
  on();
  Serial.println("Dash");
  delay(dash_duration);
  off();
  symbol_space_delay();
}

