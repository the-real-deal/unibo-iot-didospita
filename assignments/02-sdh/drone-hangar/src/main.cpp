// #include <Arduino.h>
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

// GlobalState initialState = GlobalState::Inside;
// Scheduler scheduler(SCHEDULER_PERIOD_MS, initialState);

// LCD *lcd = nullptr; // initialized in setup

// DigitalOutputPin builtinLedPin(LED_BUILTIN);
// DigitalInputPin pirPin(PIR_PIN);
// DigitalInputPin sonarEchoPin(SONAR_ECHO_PIN);
// DigitalOutputPin sonarTriggerPin(SONAR_TRIGGER_PIN);
// DigitalInputPin resetButtonPin(RESET_BUTTON_PIN);
// DigitalOutputPin onLedPin(ON_LED_PIN);
// DigitalOutputPin inActionLedPin(IN_ACTION_LED_PIN);
// DigitalOutputPin alarmLedPin(ALARM_LED_PIN);

// SerialMessageService serialMessageService(SERIAL_BAUD,
//                                           SERIAL_MESSAGE_DELIMITER,
//                                           SERIAL_SYNC_BYTE);
// PIRSensor pir(pirPin);
// ArduinoServoMotor servo(SERVO_PIN,
//                         SERVO_MIN_FREQ, SERVO_MAX_FREQ,
//                         DOOR_CLOSED_ANGLE);
// DHTSensor dht(DHT_PIN, static_cast<DHTType>(DHT_TYPE), DHT_INITIAL_TEMP);
// UltrasonicSensor sonar(sonarEchoPin, sonarTriggerPin,
//                        SONAR_READ_START_US, SONAR_READ_DELAY_US,
//                        SONAR_READ_TIMEOUT_US, &dht,
//                        SONAR_MIN_DISTANCE_MM, SONAR_MAX_DISTANCE_MM);
// PushButton resetButton(resetButtonPin);

// Led builtinLed(builtinLedPin);
// Led onLed(onLedPin);
// Led inActionLed(inActionLedPin);
// Led alarmLed(alarmLedPin);

// DoorTask doorTask(&servo,
//                   DOOR_CLOSED_ANGLE, DOOR_OPEN_ANGLE,
//                   DOOR_ANGLE_MARGIN, &serialMessageService);
// DDDTask dddTask(&sonar, &serialMessageService,
//                 OUTSIDE_DISTANCE_MM, OUTSIDE_TIME_MS,
//                 INSIDE_DISTANCE_MM, INSIDE_TIME_MS);
// DPDTask dpdTask(&pir, &serialMessageService);
// BlinkTask blinkTask(&inActionLed, BLINK_PERIOD_MS);
// StateChangeTask *stateChangeTask = nullptr; // depends on lcd
// AlarmTask alarmTask(&dht, &alarmLed,
//                     initialState, PREALARM_TEMP,
//                     PREALARM_TIME_MS, ALARM_TEMP,
//                     ALARM_TIME_MS);
// ResetTask resetTask(&resetButton, initialState);

// void setup()
// {
//   serialMessageService.begin();
//   I2CManager i2c;
//   i2c.begin();

//   builtinLedPin.begin();
//   pirPin.begin();
//   sonarEchoPin.begin();
//   sonarTriggerPin.begin();
//   resetButtonPin.begin();
//   onLedPin.begin();
//   inActionLedPin.begin();
//   alarmLedPin.begin();

//   int lcdAddress = i2c.scan();
//   auto lcd_raw = LiquidCrystal_I2C(lcdAddress, LCD_COLS, LCD_ROWS);
//   lcd = new LCD(lcd_raw);
//   lcd->begin();

//   stateChangeTask = new StateChangeTask(lcd, &serialMessageService);

//   scheduler.addInput(&serialMessageService);
//   scheduler.addInput(&pir);
//   scheduler.addInput(&servo);
//   scheduler.addInput(&dht);
//   scheduler.addInput(&sonar);
//   scheduler.addInput(&resetButton);

//   scheduler.addThread(&doorTask);
//   scheduler.addThread(&dddTask);
//   scheduler.addThread(&dpdTask);
//   scheduler.addThread(&blinkTask);
//   scheduler.addThread(stateChangeTask);
//   scheduler.addThread(&alarmTask);
//   scheduler.addThread(&resetTask);

//   onLed.turnOn();
//   inActionLed.turnOff();
//   alarmLed.turnOff();

//   scheduler.setup();
// }

// void loop()
// {
//   builtinLed.toggle();
//   scheduler.advance();
// }


#include <Arduino.h>

const int pirPin = 6; // Collega il pin OUT del sensore al D2
const int ledPin = LED_BUILTIN; // Useremo il LED integrato per il test

void setup() {
  pinMode(pirPin, INPUT);
  pinMode(ledPin, OUTPUT);
  Serial.begin(9600);
  
  Serial.println("Riscaldamento sensore... attendi 30-60 secondi");
  delay(30000); // Il sensore ha bisogno di stabilizzarsi all'avvio
  Serial.println("Sensore pronto");
}

void loop() {
  int val = digitalRead(pirPin);
  
  if (val == HIGH) {
    digitalWrite(ledPin, HIGH);
    Serial.println("Movimento rilevato!");
  } else {
    digitalWrite(ledPin, LOW);
  }
}