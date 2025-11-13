#include "config.h"
#include "debounce.hpp"
#include "display.hpp"
#include "i2c.hpp"
#include <Arduino.h>
#include <EnableInterrupt.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

/*int prevts = 0;
int numFiltered = 0;*/

const int numb[] = {1, 2, 3, 4};
int seq[sizeof(numb) / sizeof(numb[0])];
LiquidCrystal_I2C *lcd;

void generateSequence() {
  bool gen[5] = {0};
  for (int i = 0; i < 4; i++) {
    int val;
    do {
      val = (rand() % 4) + 1;
    } while (gen[val]);
    gen[val] = true;
    seq[i] = val;
  }
}

void turnOffLeds() {
  digitalWrite(PIN_LED_G1, LOW);
  digitalWrite(PIN_LED_G2, LOW);
  digitalWrite(PIN_LED_G3, LOW);
  digitalWrite(PIN_LED_G4, LOW);
}

void turnOnLed(const int pinButton) {
  if (checkDebounce()) {
    return;
  }
  /*Spegni tutti i led accessi
  (L'idea è quella di lasciare il led accesso fino
  al prossimo click, se non si vuole così allora si mette
  un delay)*/
  /*Controlli il pinPremuto con il primo numero nella lista.
  Se non è corretto cambi stato della partita a game over
  Se è la lista diventa vuota allora vuol dire che si ha vinto
  e quindi si mette lo stato in vittoria*/

  /*Se non va che i define usare i numeri o trovare un altro
  modo*/

  Serial.print("Button pressed: ");
  Serial.println(pinButton);

  turnOffLeds();
  switch (pinButton) {
  case PIN_BUTTON_1:
    Serial.println("1");
    digitalWrite(PIN_LED_G1, HIGH);
    break;
  case PIN_BUTTON_2:
    Serial.println("2");
    digitalWrite(PIN_LED_G2, HIGH);
    break;
  case PIN_BUTTON_3:
    Serial.println("3");
    digitalWrite(PIN_LED_G3, HIGH);
    break;
  case PIN_BUTTON_4:
    Serial.println("4");
    digitalWrite(PIN_LED_G4, HIGH);
    break;
  }
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Wire.begin();
  while (!Serial)
    ;

  /*BUTTON SETUP*/
  /*al posto di inc ci va turn_on_led*/
  enableInterrupt(PIN_BUTTON_1, []() { turnOnLed(PIN_BUTTON_1); }, RISING);
  enableInterrupt(PIN_BUTTON_2, []() { turnOnLed(PIN_BUTTON_2); }, RISING);
  enableInterrupt(PIN_BUTTON_3, []() { turnOnLed(PIN_BUTTON_3); }, RISING);
  enableInterrupt(PIN_BUTTON_4, []() { turnOnLed(PIN_BUTTON_4); }, RISING);

  /*LED SETUP*/
  pinMode(PIN_LED_R, OUTPUT);
  pinMode(PIN_LED_G1, OUTPUT);
  pinMode(PIN_LED_G2, OUTPUT);
  pinMode(PIN_LED_G3, OUTPUT);
  pinMode(PIN_LED_G4, OUTPUT);

  /* I2C SETUP */
  i2cScan([](const uint8_t address) {
    lcd = createDisplay(address, LCD_DISPLAY_WIDTH, LCD_DISPLAY_HEIGHT);
  });
  interrupts();
}

void loop() {
  /*AIUTO SPITA COME CAZZO DEVO GESTIRE TUTTO??*/
  /*Questione del deepsleep*/
  /*Usare uno switch con enum o state variable*/

  /*Cloni la lista con dentro 1234*/
  /*Shuffle List di quella clonata*/
  /*Display sequence*/
  /*Interrupts*/
  generateSequence();
  lcd->setCursor(0, 0);
  for (size_t i = 0; i < 4; i++) {
    lcd->print(seq[i]);
  }
  delay(1000);
}
