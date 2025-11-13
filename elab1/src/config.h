#ifndef CONFIG_H
#define CONFIG_H

#include <Arduino.h>
#include <stdint.h>

const uint8_t SEQUENCE_LENGTH = 4;

const uint8_t BUTTON_PINS[SEQUENCE_LENGTH] = {2, 3, 4, 5};

const uint8_t CONTROL_LED_PIN = 7;
const uint8_t GAME_LEDS_PINS[SEQUENCE_LENGTH] = {8, 9, 10, 11};

const uint8_t POTENTIOMETER_LED = A0;

const uint8_t LCD_DISPLAY_SIZE[2] = {16, 2};

const uint64_t DEFAULT_DEBOUNCE_MS = 40;
const uint64_t SERIAL_BAUD_RATE = 9600;

#endif // CONFIG_H