#pragma once

// general
#define OUTSIDE_DISTANCE_MM 500 // D1
#define OUTSIDE_TIME_MS 3000    // T1
#define INSIDE_DISTANCE_MM 100  // D2
#define INSIDE_TIME_MS 3000     // T2
#define PREALARM_TEMP 30        // Temp1
#define PREALARM_TIME_MS 2000   // T3
#define ALARM_TEMP 35           // Temp2
#define ALARM_TIME_MS 1000      // T4

// leds
#define ON_LED_PIN 7        // L1
#define IN_ACTION_LED_PIN 8 // L2
#define ALARM_LED_PIN 9     // L3

// dht
#define DHT_PIN 11 // TEMP
#define DHT_TYPE 11

// reset
#define RESET_BUTTON_PIN 10 // RESET

// timer
#define TIMER_READ_FREQ_MS 10

// scheduler
#define SCHEDULER_PERIOD_MS 200
#define SCHEDULER_MAX_INPUTS 10
#define SCHEDULER_MAX_THREADS 20

// serial
#define SERIAL_BAUD 9600
#define SERIAL_SYNC_BYTE '\0'
#define SERIAL_MESSAGES_QUEUE_SIZE 10

// lcd
#define LCD_COLS 16
#define LCD_ROWS 2

// pir
#define PIR_PIN 6

// servo
#define SERVO_PIN 3
#define SERVO_MIN_FREQ 500
#define SERVO_MAX_FREQ 2500

// sonar
#define SONAR_ECHO_PIN 5
#define SONAR_TRIGGER_PIN 4
#define SONAR_READ_START_US 3
#define SONAR_READ_DELAY_US 5
#define SONAR_READ_TIMEOUT_US (30L * 1000L) // 30ms =~ 5m
#define SONAR_MIN_DISTANCE_MM 50
#define SONAR_MAX_DISTANCE_MM 3000

// door
#define DOOR_CLOSED_ANGLE 0
#define DOOR_OPEN_ANGLE 180

// blink
#define BLINK_PERIOD_MS 500
