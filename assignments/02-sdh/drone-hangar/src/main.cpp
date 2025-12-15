#include "config.h"
#include "core/i2c.hpp"
#include "core/scheduler.hpp"
#include "core/serial.hpp"
#include "devices/dht.hpp"
#include "devices/led.hpp"
#include "devices/pir.hpp"
#include "devices/servo.hpp"
#include "devices/sonar.hpp"
#include "tasks/blink.hpp"
#include "tasks/ddd.hpp"
#include "tasks/door.hpp"
#include "tasks/dpd.hpp"
#include "tasks/lcd.hpp"
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <assert.h>

class Testing : public LogicThread {
private:
  uint64_t total;

public:
  void step(Context *context) override {
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

Scheduler *scheduler;
SerialManager *serialManager;
LiquidCrystal_I2C *lcd;
DHTSensor *dht;
PIRSensor *pir;
ArduinoServoMotor *servo;
UltrasonicSensor *sonar;
Led *onLed;
Led *inActionLed;

void setup() {
  scheduler = new Scheduler(SCHEDULER_PERIOD_MS, GlobalState::Inside);

  // int lcdAddress = i2cScan();
  // assert(lcdAddress != -1);
  // lcd = new LiquidCrystal_I2C(lcdAddress, LCD_COLS, LCD_ROWS);
  // scheduler->addThread(new LCDTask(lcd));

  serialManager = new SerialManager(SERIAL_BAUD);
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
  onLed = new Led(ON_LED_PIN);
  inActionLed = new Led(IN_ACTION_LED_PIN);

  scheduler->addInput(serialManager);
  scheduler->addInput(pir);
  // temperature reading must be performed before distance reading if the sonar
  // does not use a static temperature
  scheduler->addInput(dht);
  scheduler->addInput(sonar);

  scheduler->addThread(
      new DoorTask(servo, DOOR_CLOSED_ANGLE, DOOR_OPEN_ANGLE, serialManager));
  scheduler->addThread(new DDDTask(sonar, OUTSIDE_DISTANCE, OUTSIDE_TIME_MS,
                                   INSIDE_DISTANCE, INSIDE_TIME_MS));
  scheduler->addThread(new DPDTask(pir, serialManager));
  scheduler->addThread(new BlinkTask(inActionLed, BLINK_PERIOD_MS));

  onLed->turnOn();
}

void loop() { scheduler->advance(); }
