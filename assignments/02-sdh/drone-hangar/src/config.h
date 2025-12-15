#pragma once

// general
#define OUTSIDE_DISTANCE 100  // D1
#define OUTSIDE_TIME_MS 500   // T1
#define INSIDE_DISTANCE 40    // D2
#define INSIDE_TIME_MS 500    // T2
#define PREALARM_TEMP 30      // Temp1
#define PREALARM_TIME_MS 2000 // T3
#define ALARM_TEMP 35         // Temp2
#define ALARM_TIME_MS 1000    // T4

// serial
#define SERIAL_BAUD 9600

// scheduler
#define SCHEDULER_PERIOD 50

// lcd
#define LCD_COLS 16
#define LCD_ROWS 2

// dht
#define DHT_PIN A0
#define DHT_TYPE 11

// pir
#define PIR_PIN 6

// servo
#define SERVO_PIN 3
#define SERVO_MIN_FREQ 500
#define SERVO_MAX_FREQ 2500

// sonar
#define SONAR_ECHO_PIN 5
#define SONAR_TRIGGER_PIN 4
#define SONAR_READ_DELAY_US 5
#define SONAR_READ_TIMEOUT_US 30 * 1000 // 30ms ~= 5m
#define SONAR_USE_TEMP_SENSOR
#define SONAR_TEMP 20 // static temperature instead of temp sensor

// door
#define DOOR_CLOSED_ANGLE 0
#define DOOR_OPEN_ANGLE 180
