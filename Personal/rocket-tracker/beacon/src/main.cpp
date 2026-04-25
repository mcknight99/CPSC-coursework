#include <Arduino.h>

// Function prototypes
void dot();
void dash();
void symbol_space_delay();
void letter_space_delay();
void word_space_delay();

// basic beacon blinking on and off using gpio 1
#define BEACON_PIN 10
#define dot_duration 100
#define dash_duration 300
#define symbol_space 150
#define letter_space 300
#define word_space 400
#define toggle_rate 1600 //Hz

#define HIGHVAL 255
#define LOWVAL 1

void setup()
{
  Serial.begin(115200);
  pinMode(BEACON_PIN, OUTPUT);

  // flash 3 times
  for (int i = 0; i < 3; i++)
  {
    digitalWrite(BEACON_PIN, HIGHVAL);
    delay(200);
    digitalWrite(BEACON_PIN, LOWVAL);
    delay(200);
  }
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

  Serial.println("Dot");

  // pulse on and off at toggle rate for duration of a dot
  for (int i = 0; i < dot_duration * toggle_rate / 1000; i++)
  {
    digitalWrite(BEACON_PIN, HIGHVAL);
    delay(1000 / toggle_rate / 2);
    digitalWrite(BEACON_PIN, LOWVAL);
    delay(1000 / toggle_rate / 2);
  }

  digitalWrite(BEACON_PIN, LOWVAL);
  delay(symbol_space);
  

}

void dash()
{
  Serial.println("Dash");

  // pulse on and off at toggle rate for duration of a dash
  for (int i = 0; i < dash_duration * toggle_rate / 1000; i++)
  {
    digitalWrite(BEACON_PIN, HIGHVAL);
    delay(1000 / toggle_rate / 2);
    digitalWrite(BEACON_PIN, LOWVAL);
    delay(1000 / toggle_rate / 2);
  }

  digitalWrite(BEACON_PIN, LOWVAL);
  delay(symbol_space);
}

