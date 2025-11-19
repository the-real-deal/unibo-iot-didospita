// define before EnableInterrupt.h to provide arduinoInterruptedPin
//
// https://
// github.com/GreyGnome/EnableInterrupt?tab=readme-ov-file#determine-the-pin-that-was-interrupted
#define EI_ARDUINO_INTERRUPTED_PIN

#include "config.hpp"
#include "core/button.hpp"
#include "core/game.hpp"
#include "core/sequence.hpp"
#include "lib/display.hpp"
#include "lib/i2c.hpp"
#include "lib/led.hpp"
#include "lib/timer.hpp"
#include "lib/utils.hpp"
#include <Arduino.h>
#include <EnableInterrupt.h>
#include <Hashtable.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>
#include <assert.h>

Sequence sequence;
Timer timer;
Game game;
LiquidCrystal_I2C *lcd;
Hashtable<int16_t, size_t> buttonIndexes; // uint8_t not supported as key

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Wire.begin();
  while (!Serial)
    ;

  for (size_t i = 0; i < SEQUENCE_LENGTH; i++) {
    /*BUTTON SETUP*/
    const uint8_t pin = BUTTON_PINS[i];
    enableInterrupt(
        pin,
        []() {
          const uint8_t pin = arduinoInterruptedPin;
          const size_t index = *buttonIndexes.get((int16_t)pin);
          buttonPressed(pin, &game, index);
        },
        RISING);

    /*LED SETUP*/
    pinMode(GAME_LEDS_PINS[i], OUTPUT);

    /* cache button indexes */
    const size_t index = indexOf<uint8_t>(BUTTON_PINS, SEQUENCE_LENGTH, pin);
    assert(index != -1ul);
    buttonIndexes.put(pin, index);
  }

  pinMode(CONTROL_LED_PIN, OUTPUT);
  pinMode(POTENTIOMETER_PIN, INPUT);

  /* I2C SETUP */
  i2cScan([](const uint8_t address) {
    lcd = createDisplay(address, LCD_DISPLAY_SIZE[0], LCD_DISPLAY_SIZE[1]);
  });

  game = initGame();
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
  gameStep(&game, lcd);
  delay(LOOP_DELAY_MS);
}
