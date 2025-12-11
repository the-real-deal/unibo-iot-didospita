#include "config.h"
#include "core/i2c.hpp"
#include "core/scheduler.hpp"
#include "core/serial.hpp"
#include "devices/servo.hpp"
#include "tasks/door.hpp"
#include "tasks/lcd.hpp"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <assert.h>

SerialManager *serialManager;
LiquidCrystal_I2C *lcd;
ServoMotor *servo;
Scheduler *scheduler;

class Testing : public LogicThread {
private:
  uint64_t total;

public:
  void step(SchedulerContext *context) override {
    this->total += context->elapsed();
    Serial.print("Testing: ");
    Serial.println((int)this->total);
    if (this->total < 4 * 1000) {
      return;
    }
    switch (context->state()) {
    case StateType::Inside:
      context->setState(StateType::Takeoff);
      break;
    case StateType::Takeoff:
      context->setState(StateType::Outside);
      break;
    case StateType::Outside:
      context->setState(StateType::Landing);
      break;
    case StateType::Landing:
      context->setState(StateType::Inside);
      break;
    default:
      break;
    }
    this->total = 0;
  }
};

void setup() {
  scheduler = new Scheduler(SCHEDULER_PERIOD, StateType::Inside);

  serialManager = new SerialManager(SERIAL_BAUD);
  scheduler->addInput(serialManager);

  // int lcdAddress = i2cScan();
  // assert(lcdAddress != -1);
  // lcd = new LiquidCrystal_I2C(lcdAddress, LCD_COLS, LCD_ROWS);
  // scheduler->addThread(new LCDTask(lcd));

  servo = new ServoMotor(SERVO_PIN, 0, SERVO_MIN_FREQ, SERVO_MAX_FREQ);
  scheduler->addThread(new DoorTask(servo, DOOR_CLOSED_ANGLE, DOOR_OPEN_ANGLE));

  scheduler->addThread(new Testing());
}

void loop() { scheduler->advance(); }
