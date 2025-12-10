#include "core/i2c.hpp"
#include "core/scheduler.hpp"
#include "core/serial.hpp"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <assert.h>

Scheduler *scheduler;
SerialManager *serialManager;
LiquidCrystal_I2C *lcd;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }

  int lcdAddress = i2cScan();
  assert(lcdAddress != -1);
  lcd = new LiquidCrystal_I2C(lcdAddress, 16, 2);
  scheduler = new Scheduler(500, StateType::INSIDE);
  serialManager = new SerialManager();
  scheduler->addInput(serialManager);
}

void loop() { scheduler->advance(); }
