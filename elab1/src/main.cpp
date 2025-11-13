// define before EnableInterrupt.h to provide arduinoInterruptedPin
// https://github.com/GreyGnome/EnableInterrupt?tab=readme-ov-file#determine-the-pin-that-was-interrupted
#define EI_ARDUINO_INTERRUPTED_PIN

#include "config.hpp"
#include "core/button.hpp"
#include "lib/display.hpp"
#include "lib/i2c.hpp"
#include "lib/led.hpp"
#include "lib/sequence.hpp"
#include "lib/utils.hpp"
#include <Arduino.h>
#include <EnableInterrupt.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

int seq[SEQUENCE_LENGTH];
LiquidCrystal_I2C *lcd;

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
