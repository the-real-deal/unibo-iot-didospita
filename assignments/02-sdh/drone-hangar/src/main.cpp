#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <assert.h>

#include "config.h"
#include "core/i2c.hpp"
#include "core/scheduler.hpp"
#include "core/serial.hpp"
#include "devices/button.hpp"
#include "devices/dht.hpp"
#include "devices/lcd.hpp"
#include "devices/led.hpp"
#include "devices/pir.hpp"
#include "devices/servo.hpp"
#include "devices/sonar.hpp"
#include "tasks/alarm.hpp"
#include "tasks/blink.hpp"
#include "tasks/ddd.hpp"
#include "tasks/door.hpp"
#include "tasks/dpd.hpp"
#include "tasks/reset.hpp"
#include "tasks/stateChange.hpp"

Scheduler* scheduler;
SerialManager* serialManager;
LCD* lcd;
DHTSensor* dht;
PIRSensor* pir;
ArduinoServoMotor* servo;
UltrasonicSensor* sonar;
PushButton* resetButton;
Led* onLed;
Led* inActionLed;
Led* alarmLed;

void setup() {
  GlobalState initialState = GlobalState::Inside;
  int lcdAddress = i2cScan();
  assert(lcdAddress != -1);

  scheduler = new Scheduler(SCHEDULER_PERIOD_MS, initialState);

  lcd = new LCD(lcdAddress, LCD_COLS, LCD_ROWS);
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
  resetButton = new PushButton(RESET_BUTTON_PIN);
  onLed = new Led(ON_LED_PIN);
  inActionLed = new Led(IN_ACTION_LED_PIN);
  alarmLed = new Led(ALARM_LED_PIN);

  scheduler->addInput(serialManager);
  scheduler->addInput(pir);
  // temperature reading must be performed before distance reading if the sonar
  // does not use a static temperature
  scheduler->addInput(dht);
  scheduler->addInput(sonar);
  scheduler->addInput(resetButton);

  scheduler->addThread(
      new DoorTask(servo, DOOR_CLOSED_ANGLE, DOOR_OPEN_ANGLE, serialManager));
  scheduler->addThread(new DDDTask(sonar, serialManager, OUTSIDE_DISTANCE,
                                   OUTSIDE_TIME_MS, INSIDE_DISTANCE,
                                   INSIDE_TIME_MS));
  scheduler->addThread(new DPDTask(pir, serialManager));
  scheduler->addThread(new BlinkTask(inActionLed, BLINK_PERIOD_MS));
  scheduler->addThread(new StateChangeTask(lcd, serialManager, initialState));
  scheduler->addThread(new AlarmTask(dht, alarmLed, initialState, PREALARM_TEMP,
                                     PREALARM_TIME_MS, ALARM_TEMP,
                                     ALARM_TIME_MS));
  scheduler->addThread(new ResetTask(resetButton, initialState));

  onLed->turnOn();
}

void loop() { scheduler->advance(); }
