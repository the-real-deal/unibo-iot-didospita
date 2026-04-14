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

Scheduler* scheduler = nullptr;

void setup()
{
  Serial.begin(SERIAL_BAUD);
  while (!Serial);
  Wire.begin();
  Serial.print(SERIAL_SYNC_BYTE);

  auto serialMessageService = new SerialMessageService();

  GlobalState initialState = GlobalState::Inside;
  scheduler = new Scheduler(SCHEDULER_PERIOD_MS, initialState);
  
  auto i2c = new I2CManager();

  int lcdAddress = i2c->scan();
  auto lcd = new LCD(lcdAddress, LCD_COLS, LCD_ROWS);
  lcd->begin();

  auto pir = new PIRSensor(PIR_PIN);
  auto servo = new ArduinoServoMotor(SERVO_PIN, DOOR_CLOSED_ANGLE, SERVO_MIN_FREQ,
                                     SERVO_MAX_FREQ);
  auto dht = new DHTSensor(DHT_PIN, static_cast<DHTType>(DHT_TYPE));
  auto sonar = new UltrasonicSensor(
      SONAR_ECHO_PIN, SONAR_TRIGGER_PIN,
      SONAR_READ_TIMEOUT_US, SONAR_READ_DELAY_US,
#ifdef SONAR_USE_TEMP_SENSOR
      dht
#else
      SONAR_TEMP
#endif
  );
  auto resetButton = new PushButton(RESET_BUTTON_PIN);
  auto onLed = new Led(ON_LED_PIN);
  auto inActionLed = new Led(IN_ACTION_LED_PIN);
  auto alarmLed = new Led(ALARM_LED_PIN);

  auto doorTask = new DoorTask(servo, DOOR_CLOSED_ANGLE, DOOR_OPEN_ANGLE,
                               serialMessageService);
  auto dddTask = new DDDTask(sonar, serialMessageService,
                             OUTSIDE_DISTANCE, OUTSIDE_TIME_MS,
                             INSIDE_DISTANCE, INSIDE_TIME_MS);
  auto stateChangeTask = new StateChangeTask(lcd, serialMessageService);
  auto dpdTask = new DPDTask(pir, serialMessageService);
  auto blinkTask = new BlinkTask(inActionLed, BLINK_PERIOD_MS);
  auto alarmTask = new AlarmTask(dht, alarmLed, initialState, PREALARM_TEMP,
                                 PREALARM_TIME_MS, ALARM_TEMP,
                                 ALARM_TIME_MS);
  auto resetTask = new ResetTask(resetButton, initialState);

  scheduler->addInput(serialMessageService);
  scheduler->addInput(pir);
  scheduler->addInput(dht);
  scheduler->addInput(sonar);
  scheduler->addInput(resetButton);

  scheduler->addThread(doorTask);
  scheduler->addThread(dddTask);
  scheduler->addThread(dpdTask);
  scheduler->addThread(blinkTask);
  scheduler->addThread(stateChangeTask);
  scheduler->addThread(alarmTask);
  scheduler->addThread(resetTask);

  onLed->turnOn();
  inActionLed->turnOff();
  alarmLed->turnOff();
}

void loop()
{
  scheduler->advance();
}
