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
I2CManager i2c;
Scheduler scheduler(SCHEDULER_PERIOD_MS, initialState);
SerialMessageService serialMessageService;
LCD *lcd = nullptr; // in the heap to allow scanning for the i2c address
PIRSensor pir(PIR_PIN);
ArduinoServoMotor servo(SERVO_PIN, DOOR_CLOSED_ANGLE, SERVO_MIN_FREQ,
                        SERVO_MAX_FREQ);
DHTSensor dht(DHT_PIN, static_cast<DHTType>(DHT_TYPE));
UltrasonicSensor sonar(
    SONAR_ECHO_PIN, SONAR_TRIGGER_PIN,
    SONAR_READ_TIMEOUT_US, SONAR_READ_DELAY_US,
#ifdef SONAR_USE_TEMP_SENSOR
    &dht
#else
    SONAR_TEMP
#endif
);
PushButton resetButton(RESET_BUTTON_PIN);
Led onLed(ON_LED_PIN);
Led inActionLed(IN_ACTION_LED_PIN);
Led alarmLed(ALARM_LED_PIN);

DoorTask doorTask(&servo, DOOR_CLOSED_ANGLE, DOOR_OPEN_ANGLE,
                  &serialMessageService);
DDDTask dddTask(&sonar, &serialMessageService,
                OUTSIDE_DISTANCE, OUTSIDE_TIME_MS,
                INSIDE_DISTANCE, INSIDE_TIME_MS);
StateChangeTask *stateChangeTask = nullptr; // in the heap because it depends on lcd
DPDTask dpdTask(&pir, &serialMessageService);
BlinkTask blinkTask(&inActionLed, BLINK_PERIOD_MS);
AlarmTask alarmTask(&dht, &alarmLed, initialState, PREALARM_TEMP,
                    PREALARM_TIME_MS, ALARM_TEMP,
                    ALARM_TIME_MS);
ResetTask resetTask(&resetButton, initialState);

void setup()
{
  Serial.begin(SERIAL_BAUD);
  while (!Serial)
    ;
  Wire.begin();

  int lcdAddress = i2c.scan();
  lcd = new LCD(lcdAddress, LCD_COLS, LCD_ROWS);
  lcd->begin();
  
  String initMsg = "DRONE INSIDE";
  lcd->print(&initMsg);

  stateChangeTask = new StateChangeTask(lcd, &serialMessageService, initialState);

  scheduler.addInput(&serialMessageService);
  scheduler.addInput(&pir);
  scheduler.addInput(&dht);
  scheduler.addInput(&sonar);
  scheduler.addInput(&resetButton);

  scheduler.addThread(&doorTask);
  scheduler.addThread(&dddTask);
  scheduler.addThread(&dpdTask);
  scheduler.addThread(&blinkTask);
  scheduler.addThread(stateChangeTask);
  scheduler.addThread(&alarmTask);
  scheduler.addThread(&resetTask);

  onLed.turnOn();
  inActionLed.turnOff();
  alarmLed.turnOff();
}

void loop() { scheduler.advance(); }
