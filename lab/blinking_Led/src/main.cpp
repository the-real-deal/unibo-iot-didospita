#include <Arduino.h>

#define pinR 3
#define pinG 7
#define pinB 10
int red = 0;
int green = 0;
int blue = 0;

void setColor() {
  analogWrite(pinR, red);
  analogWrite(pinG, green);
  analogWrite(pinB, blue);
  delay(50);
}

void setup() {
  pinMode(pinR, OUTPUT);
  pinMode(pinG, OUTPUT);
  pinMode(pinB, OUTPUT);
}

void loop() {

  if (red < 255) {
    red++;
  } else if (blue < 255) {
    blue++;
  } else if (green < 255) {
    green++;
  } else {
    red = 0;
    blue = 0;
    green = 0;
  }

  setColor();
}
