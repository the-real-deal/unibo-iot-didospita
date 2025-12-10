#include "config.h"
#include "core/i2c.hpp"
#include "core/scheduler.hpp"
#include "core/serial.hpp"
#include "devices/servo.hpp"
#include "tasks/lcd.hpp"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <assert.h>

SerialManager *serialManager;
LiquidCrystal_I2C *lcd;
ServoMotor *servo;
Scheduler *scheduler;

void setup() {
  serialManager = new SerialManager(SERIAL_BAUD);
  int lcdAddress = i2cScan();
  assert(lcdAddress != -1);
  lcd = new LiquidCrystal_I2C(lcdAddress, LCD_COLS, LCD_ROWS);
#if defined(SERVO_MIN_FREQ) && defined(SERVO_MAX_FREQ)
  servo = new ServoMotor(SERVO_PIN, SERVO_MIN_FREQ, SERVO_MAX_FREQ);
#else
  servo = new ServoMotor(SERVO_PIN);
#endif

  scheduler = new Scheduler(SCHEDULER_PERIOD, StateType::INSIDE);
  scheduler->addInput(serialManager);
  scheduler->addThread(new LCDTask(lcd));
}

void loop() { scheduler->advance(); }
