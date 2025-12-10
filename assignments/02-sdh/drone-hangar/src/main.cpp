#include "core/i2c.hpp"
#include "core/scheduler.hpp"
#include "core/serial.hpp"
#include "tasks/lcd.hpp"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <assert.h>

SerialManager *serialManager;
LiquidCrystal_I2C *lcd;
Scheduler *scheduler;

void setup() {
  int lcdAddress = i2cScan();
  assert(lcdAddress != -1);
  lcd = new LiquidCrystal_I2C(lcdAddress, 16, 2);
  serialManager = new SerialManager(9600);

  scheduler = new Scheduler(500, StateType::INSIDE);
  scheduler->addInput(serialManager);
  scheduler->addThread(new LCDTask(lcd));
}

void loop() { scheduler->advance(); }
