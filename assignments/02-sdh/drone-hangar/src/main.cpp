#include <Arduino.h>
// #include <LiquidCrystal_I2C.h>
// #include <assert.h>

// #include "config.h"
// #include "core/i2c.hpp"
// #include "core/scheduler.hpp"
// #include "core/serial.hpp"
// #include "devices/button.hpp"
// #include "devices/dht.hpp"
// #include "devices/lcd.hpp"
// #include "devices/led.hpp"
// #include "devices/pir.hpp"
// #include "devices/servo.hpp"
// #include "devices/sonar.hpp"
// #include "tasks/alarm.hpp"
// #include "tasks/blink.hpp"
// #include "tasks/ddd.hpp"
// #include "tasks/door.hpp"
// #include "tasks/dpd.hpp"
// #include "tasks/reset.hpp"
// #include "tasks/stateChange.hpp"

// I2CManager* i2c;
// Scheduler* scheduler;
// SerialMessageService* serialMessageService;
// LCD* lcd;
// PIRSensor* pir;
// ArduinoServoMotor* servo;
// DHTSensor* dht;
// UltrasonicSensor* sonar;
// PushButton* resetButton;
// Led* onLed;
// Led* inActionLed;
// Led* alarmLed;

void setup() {
  Serial.begin(9600);
  delay(2000);

  // Test 3: String with constructor
  String s1 = String("test");
  Serial.print("String constructor length: ");
  Serial.println(s1.length());
  Serial.print("String constructor: ");
  Serial.println(s1);

  // Test 4: String assignment
  String s2;
  s2 = "test2";
  Serial.print("String assignment length: ");
  Serial.println(s2.length());
  Serial.print("String assignment: ");
  Serial.println(s2);
}

void loop() {}

// void setup() {
//   Serial.begin(SERIAL_BAUD);
//   Wire.begin();

//   GlobalState initialState = GlobalState::Inside;

//   i2c = new I2CManager();
//   // scheduler = new Scheduler(SCHEDULER_PERIOD_MS, initialState);
//   // serialMessageService = new SerialMessageService();

//   int lcdAddress = i2c->scan();
//   assert(lcdAddress != -1);
//   Serial.println("jkldsnajkdsna");
//   lcd = new LCD(lcdAddress, LCD_COLS, LCD_ROWS);
//   Serial.println("lcd");
//   String message = "ciao a tutti";
//   Serial.print("aaaaaaaaaaaaaaaaaaa: ");
//   Serial.println(message.length());
//   lcd->print(&message);

//   // pir = new PIRSensor(PIR_PIN);
//   //   servo = new ArduinoServoMotor(SERVO_PIN, DOOR_CLOSED_ANGLE,
//   //   SERVO_MIN_FREQ,
//   //                                 SERVO_MAX_FREQ);
//   //   dht = new DHTSensor(DHT_PIN, static_cast<DHTType>(DHT_TYPE));
//   //   sonar = new UltrasonicSensor(SONAR_ECHO_PIN, SONAR_TRIGGER_PIN,
//   //                                SONAR_READ_TIMEOUT_US,
//   SONAR_READ_DELAY_US,
//   // #ifdef SONAR_USE_TEMP_SENSOR
//   //                                dht
//   // #else
//   //                                SONAR_TEMP
//   // #endif
//   //   );
//   //   resetButton = new PushButton(RESET_BUTTON_PIN);
//   //   onLed = new Led(ON_LED_PIN);
//   //   inActionLed = new Led(IN_ACTION_LED_PIN);
//   //   alarmLed = new Led(ALARM_LED_PIN);

//   //   scheduler->addInput(serialMessageService);
//   //   scheduler->addInput(pir);
//   //   // temperature reading must be performed before distance reading if
//   the
//   //   sonar
//   //   // does not use a static temperature
//   //   scheduler->addInput(dht);
//   //   scheduler->addInput(sonar);
//   //   scheduler->addInput(resetButton);

//   //   scheduler->addThread(new DoorTask(servo, DOOR_CLOSED_ANGLE,
//   //   DOOR_OPEN_ANGLE,
//   //                                     serialMessageService));
//   //   scheduler->addThread(new DDDTask(sonar, serialMessageService,
//   //                                    OUTSIDE_DISTANCE, OUTSIDE_TIME_MS,
//   //                                    INSIDE_DISTANCE, INSIDE_TIME_MS));
//   //   scheduler->addThread(new DPDTask(pir, serialMessageService));
//   //   scheduler->addThread(new BlinkTask(inActionLed, BLINK_PERIOD_MS));
//   //   scheduler->addThread(
//   //       new StateChangeTask(lcd, serialMessageService, initialState));
//   //   scheduler->addThread(new AlarmTask(dht, alarmLed, initialState,
//   //   PREALARM_TEMP,
//   //                                      PREALARM_TIME_MS, ALARM_TEMP,
//   //                                      ALARM_TIME_MS));
//   //   scheduler->addThread(new ResetTask(resetButton, initialState));

//   //   onLed->turnOn();
// }

// void loop() { scheduler->advance(); }
