#include <Arduino.h>

#define pinG 3
#define pinY 7
#define pinW 10
#define pinR 2
#define pinB 5
#define N_LEDS 5
int pins[] = {pinG, pinY, pinW, pinR, pinB};
int current = 0;
int step = 1;

void setup() {

  for (int i = 0; i < 5; i++) {
    pinMode(pins[i], OUTPUT);
  }
}

void loop() {
  digitalWrite(pins[current], HIGH);
  delay(1000);
  digitalWrite(pins[current], LOW);
  delay(500);
  current += step;
  if (current == N_LEDS) {
    current -= 2;
    step = -1;
  } else if (current < 0) {
    current = 1;
    step = 1;
  }
}
