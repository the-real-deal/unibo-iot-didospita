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

GlobalState initialState = GlobalState::Inside;
Scheduler scheduler(SCHEDULER_PERIOD_MS, initialState);
I2CManager i2c;
LCD *lcd = nullptr;

SerialMessageService serialMessageService(SERIAL_MESSAGE_DELIMITER, SERIAL_SYNC_BYTE);
PIRSensor pir(PIR_PIN);
ArduinoServoMotor *servo = nullptr;
DHTSensor *dht = nullptr;
UltrasonicSensor *sonar = nullptr; // depends on dht
PushButton resetButton(RESET_BUTTON_PIN);

Led builtinLed(LED_BUILTIN);
Led onLed(ON_LED_PIN);
Led inActionLed(IN_ACTION_LED_PIN);
Led alarmLed(ALARM_LED_PIN);

DoorTask *doorTask = nullptr; // depends on servo
DDDTask *dddTask = nullptr;   // depends on sonar
DPDTask dpdTask(&pir, &serialMessageService);
BlinkTask blinkTask(&inActionLed, BLINK_PERIOD_MS);
StateChangeTask *stateChangeTask = nullptr; // depends on lcd
AlarmTask *alarmTask = nullptr;             // depends on dht
ResetTask resetTask(&resetButton, initialState);

void setup()
{
  Serial.begin(SERIAL_BAUD);
  while (!Serial)
    ;
  Wire.begin();

  int lcdAddress = i2c.scan();
  auto lcd_raw = LiquidCrystal_I2C(lcdAddress, LCD_COLS, LCD_ROWS);
  lcd = new LCD(lcd_raw);
  lcd->begin();
  stateChangeTask = new StateChangeTask(lcd, &serialMessageService);

  auto servo_raw = Servo();
  servo = new ArduinoServoMotor(servo_raw, SERVO_PIN,
                                SERVO_MIN_FREQ, SERVO_MAX_FREQ,
                                DOOR_CLOSED_ANGLE);
  doorTask = new DoorTask(servo, DOOR_CLOSED_ANGLE,
                          DOOR_OPEN_ANGLE, &serialMessageService);

  auto dht_raw = DHT(DHT_PIN, DHT_TYPE);
  dht = new DHTSensor(dht_raw);
  sonar = new UltrasonicSensor(SONAR_ECHO_PIN, SONAR_TRIGGER_PIN,
                               SONAR_READ_START_US, SONAR_READ_DELAY_US,
                               SONAR_READ_TIMEOUT_US, dht,
                               SONAR_MIN_DISTANCE_MM, SONAR_MAX_DISTANCE_MM);
  dddTask = new DDDTask(sonar, &serialMessageService,
                        OUTSIDE_DISTANCE_MM, OUTSIDE_TIME_MS,
                        INSIDE_DISTANCE_MM, INSIDE_TIME_MS);
  alarmTask = new AlarmTask(dht, &alarmLed,
                            initialState, PREALARM_TEMP,
                            PREALARM_TIME_MS, ALARM_TEMP,
                            ALARM_TIME_MS);

  scheduler.addInput(&serialMessageService);
  scheduler.addInput(&pir);
  scheduler.addInput(servo);
  scheduler.addInput(dht);
  scheduler.addInput(sonar);
  scheduler.addInput(&resetButton);

  scheduler.addThread(doorTask);
  scheduler.addThread(dddTask);
  scheduler.addThread(&dpdTask);
  scheduler.addThread(&blinkTask);
  scheduler.addThread(stateChangeTask);
  scheduler.addThread(alarmTask);
  scheduler.addThread(&resetTask);

  onLed.turnOn();
  inActionLed.turnOff();
  alarmLed.turnOff();

  scheduler.setup();
}

void loop()
{
  builtinLed.toggle();
  scheduler.advance();
}
