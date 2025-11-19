#pragma once

#include <Arduino.h>
#include <stdint.h>

const uint64_t LOOP_DELAY_MS = 50;
const uint8_t RANDOM_SEED_PIN = A1;

const uint8_t SEQUENCE_LENGTH = 4;

const uint8_t BUTTON_PINS[SEQUENCE_LENGTH] = {2, 3, 4, 5};

const uint8_t CONTROL_LED_PIN = 11;
const uint16_t CONTROL_LED_FADE_AMOUNT = 20;

const uint8_t GAME_LEDS_PINS[SEQUENCE_LENGTH] = {7, 8, 9, 10};

const uint8_t POTENTIOMETER_PIN = A0;

const uint8_t LCD_DISPLAY_SIZE[2] = {16, 2};

const uint64_t DEFAULT_DEBOUNCE_MS = 150;
const uint64_t SERIAL_BAUD_RATE = 9600;

const uint64_t IDLE_PERIOD_MS = 10 * 1000;
const size_t WAKE_BTN_INDEX = 0;

const uint32_t ROUND_PERIOD_MS = 10 * 1000;
const uint8_t WIN_POINTS = 10;
const size_t N_DIFFICULTIES = 4;
const uint32_t DIFFICULTY_FACTORS[N_DIFFICULTIES] = {250, 500, 750, 1000};
const uint32_t ROUND_TIME_LBOUND_MS[N_DIFFICULTIES] = {1500, 1000, 750, 500};
const uint64_t DIFFICULTY_SHOW_PERIOD_MS = 500;

const uint64_t INIT_GAME_PERIOD_MS = 2 * 1000;
const uint64_t WIN_PERIOD_MS = 5 * 1000;
const uint64_t GAME_OVER_LED_PERIOD_MS = 2 * 1000;
const uint64_t GAME_OVER_PERIOD_MS = 10 * 1000;
