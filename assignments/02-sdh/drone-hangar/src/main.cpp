#include "config.h"
#include "core/i2c.hpp"
#include "core/scheduler.hpp"
#include "core/serial.hpp"
#include "devices/dht.hpp"
#include "devices/pir.hpp"
#include "devices/servo.hpp"
#include "devices/sonar.hpp"
#include "tasks/door.hpp"
#include "tasks/lcd.hpp"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <assert.h>

Scheduler *scheduler;
SerialManager *serialManager;
LiquidCrystal_I2C *lcd;
DHTSensor *dht;
PIRSensor *pir;
ArduinoServoMotor *servo;
UltrasonicSensor *sonar;

class Testing : public LogicThread {
private:
  uint64_t total;

public:
  void step(SchedulerContext *context) override {
    this->total += context->getElapsedMillis();
    Serial.print("Testing: ");
    Serial.println((int)this->total);
    if (this->total < 4 * 1000) {
      return;
    }
    switch (context->getState()) {
    case GlobalState::Inside:
      context->setState(GlobalState::Takeoff);
      break;
    case GlobalState::Takeoff:
      context->setState(GlobalState::Outside);
      break;
    case GlobalState::Outside:
      context->setState(GlobalState::Landing);
      break;
    case GlobalState::Landing:
      context->setState(GlobalState::Inside);
      break;
    default:
      break;
    }
    this->total = 0;
  }
};

void setup() {
  scheduler = new Scheduler(SCHEDULER_PERIOD, GlobalState::Inside);

  serialManager = new SerialManager(SERIAL_BAUD);
  scheduler->addInput(serialManager);

  // int lcdAddress = i2cScan();
  // assert(lcdAddress != I2C_NOT_FOUND);
  // lcd = new LiquidCrystal_I2C(lcdAddress, LCD_COLS, LCD_ROWS);
  // scheduler->addThread(new LCDTask(lcd));

  dht = new DHTSensor(DHT_PIN, static_cast<DHTType>(DHT_TYPE));
  pir = new PIRSensor(PIR_PIN);
  servo = new ArduinoServoMotor(SERVO_PIN, DOOR_CLOSED_ANGLE, SERVO_MIN_FREQ,
                                SERVO_MAX_FREQ);
  sonar = new UltrasonicSensor(SONAR_ECHO_PIN, SONAR_TRIGGER_PIN,
                               SONAR_READ_TIMEOUT_US, SONAR_READ_DELAY_US,
#ifdef SONAR_USE_TEMP_SENSOR
                               dht
#else
                               SONAR_TEMP
#endif
  );

  scheduler->addInput(dht);
  scheduler->addInput(pir);
  scheduler->addInput(sonar);

  scheduler->addThread(new DoorTask(servo, DOOR_CLOSED_ANGLE, DOOR_OPEN_ANGLE));
  scheduler->addThread(new Testing());
}

void loop() { scheduler->advance(); }
