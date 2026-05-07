#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <assert.h>

#include "config.h"
#include "std/debug.hpp"
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

LCD lcd;

SerialMessageService serialMessageService;
PIRSensor pir(PIR_PIN);
ArduinoServoMotor servo(SERVO_PIN,
                        SERVO_MIN_FREQ, SERVO_MAX_FREQ,
                        DOOR_CLOSED_ANGLE);
DHTSensor dht(DHT_PIN, static_cast<DHTType>(DHT_TYPE), DHT_INITIAL_TEMP);
UltrasonicSensor sonar(SONAR_ECHO_PIN, SONAR_TRIGGER_PIN, &dht,
                       SONAR_READ_START_US, SONAR_READ_DELAY_US,
                       SONAR_READ_TIMEOUT_US, SONAR_READ_OOR_MARGIN_US,
                       SONAR_MIN_DISTANCE_MM, SONAR_MAX_DISTANCE_MM);
PushButton resetButton(RESET_BUTTON_PIN);

Led builtinLed(LED_BUILTIN);
Led onLed(ON_LED_PIN);
Led inActionLed(IN_ACTION_LED_PIN);
Led alarmLed(ALARM_LED_PIN);

DoorTask doorTask(&servo, &serialMessageService);
DDDTask dddTask(&sonar, &serialMessageService);
DPDTask dpdTask(&pir, &serialMessageService);
BlinkTask blinkTask(&inActionLed);
StateChangeTask stateChangeTask(&lcd, &serialMessageService);
AlarmTask alarmTask(&dht, &alarmLed, initialState);
ResetTask resetTask(&resetButton, initialState);

void setup()
{
  serialMessageService.setup();
  I2CManager i2c;
  i2c.setup();

  int lcdAddress = i2c.scan();
  lcd.begin(lcdAddress, LCD_COLS, LCD_ROWS);

  scheduler.addInput(&serialMessageService);
  scheduler.addInput(&pir);
  scheduler.addInput(&servo);
  scheduler.addInput(&dht);
  scheduler.addInput(&sonar);
  scheduler.addInput(&resetButton);

  scheduler.addThread(&doorTask);
  scheduler.addThread(&dddTask);
  scheduler.addThread(&dpdTask);
  scheduler.addThread(&blinkTask);
  scheduler.addThread(&stateChangeTask);
  scheduler.addThread(&alarmTask);
  scheduler.addThread(&resetTask);

  builtinLed.setup();
  onLed.setup();
  inActionLed.setup();
  alarmLed.setup();
  scheduler.setup();

  onLed.turnOn();
}

void loop()
{
  builtinLed.toggle();
  scheduler.advance();
}
