//Not a real "test", just a snippet of code that I liked and wanted to keep for the future to play polyrhythms on GPIO pins
#include <Arduino.h>
#include <vector>

// Function to create a polyrhythm
// @param intervals: vector of integers representing the intervals of the polyrhythm
// @param pins: vector of integers representing the pins to be used
// @param loops: number of loops to run the polyrhythm for
// @return void
// @note: This function will run indefinitely if loops is set to -1
void polyrhythm(const std::vector<int> &intervals, const std::vector<int> &pins, int loops)
{
  Serial.printf("Polyrhythm with intervals: ");
  for (int i : intervals)
  {
    Serial.printf("%i ", i);
  }
  Serial.printf("on pins: ");
  for (int i : pins)
  {
    Serial.printf("%i ", i);
  }
  Serial.printf("for %i loops\n", loops);

  for(int pin : pins)
  {
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
  }

  int n = intervals.size();
  int totalBeats = 1;
  std::vector<int> states(n, 0x0);
  for (int i : intervals)
  {
    totalBeats *= i;
  }

  int loop = 0;

  while (loops == -1 || loop < loops)
  {
    for (int i = 0; i < totalBeats; ++i)
    {
      for (int j = 0; j < n; ++j)
      {
        if (i % intervals[j] == 0)
        {
          states[j] = !states[j];
          digitalWrite(pins[j], states[j]);
          Serial.printf("Pin %i: %i\n", pins[j], states[j]);
        }
      }
      Serial.printf("Beat %i\n", i);
      delay(250);
    }
    Serial.printf("Loop %i\n", loop);
    loop++;
  }
}