#include "config.h"
#include "debounce.hpp"
#include "display.hpp"
#include "i2c.hpp"
#include "utils.hpp"
#include <Arduino.h>
#define EI_ARDUINO_INTERRUPTED_PIN
#include <EnableInterrupt.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

/*int prevts = 0;
int numFiltered = 0;*/

int seq[SEQUENCE_LENGTH];
LiquidCrystal_I2C *lcd;

void generateSequence(int *const seq, const size_t length) {
  bool gen[length + 1] = {0};
  for (size_t i = 0; i < length; i++) {
    int val;
    do {
      val = (rand() % length) + 1;
    } while (gen[val]);
    gen[val] = true;
    seq[i] = val;
  }
}

void turnOffLeds() {
  for (size_t i = 0; i < SEQUENCE_LENGTH; i++) {
    digitalWrite(GAME_LEDS_PINS[i], LOW);
  }
}

void turnOnLed(const uint8_t pin) {
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

  turnOffLeds();

  Serial.println("Turning on led at pin " + String(pin));
  digitalWrite(pin, HIGH);
}

void buttonPressed(const uint8_t pin) {
  if (checkDebounce()) {
    return;
  }

  const size_t index = indexOf<uint8_t>(BUTTON_PINS, SEQUENCE_LENGTH, pin);
  if (index == -1ul) {
    Serial.println("Unknown button " + String(pin) + " pressed!");
    return;
  }

  Serial.println("Pressed button at pin " + String(pin));
  turnOnLed(GAME_LEDS_PINS[index]);
}

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Wire.begin();
  while (!Serial)
    ;

  for (size_t i = 0; i < SEQUENCE_LENGTH; i++) {
    /*BUTTON SETUP*/
    /*al posto di inc ci va turn_on_led*/
    enableInterrupt(
        BUTTON_PINS[i],
        []() {
          Serial.println("Interrupt on pin " + String(arduinoInterruptedPin));
          buttonPressed(arduinoInterruptedPin);
        },
        RISING);

    /*LED SETUP*/
    pinMode(GAME_LEDS_PINS[i], OUTPUT);
  }

  pinMode(CONTROL_LED_PIN, OUTPUT);

  /* I2C SETUP */
  i2cScan([](const uint8_t address) {
    lcd = createDisplay(address, LCD_DISPLAY_SIZE[0], LCD_DISPLAY_SIZE[1]);
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
  generateSequence(seq, SEQUENCE_LENGTH);
  lcd->setCursor(0, 0);
  for (size_t i = 0; i < SEQUENCE_LENGTH; i++) {
    lcd->print(seq[i]);
  }
  delay(1000);
}
