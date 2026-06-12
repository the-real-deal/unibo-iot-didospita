#include <Arduino.h>

#include "config.h"

#include "devices/serial.hpp"
#include "devices/button.hpp"
#include "devices/servo.hpp"
#include "devices/potentiometer.hpp"
#include "devices/led.hpp"
#include "devices/lcd.hpp"
#include "devices/i2c.hpp"

#ifndef BTN_EVENT_FAMILY
#define BTN_EVENT_FAMILY 0
#endif

#ifndef POT_EVENT_FAMILY
#define POT_EVENT_FAMILY 1
#endif

#ifndef SERIAL_EVENT_FAMILY
#define SERIAL_EVENT_FAMILY 2
#endif

EventsManager eventManager;
SerialManager serialManager(SERIAL_EVENT_FAMILY, &eventManager);
PushButton button(BTN_PIN, BTN_EVENT_FAMILY, &eventManager);
Potentiometer potentiomenter(POT_PIN, POT_EVENT_FAMILY, &eventManager);
ServoMotor servo(SERVO_PIN, 0);
Led builtinLed(LED_BUILTIN);
LCD lcd;

void setup()
{
  serialManager.setup();
  serialManager.log("setup() started");

  I2CManager i2c;
  i2c.setup();

  uint8_t lcdAddress = i2c.scan();
  lcd.begin(lcdAddress);
  
  builtinLed.setup();
  button.setup();
  potentiomenter.setup();
  servo.setup();

  serialManager.log("setup() finished");
}

void loop()
{
  builtinLed.toggle();
  serialManager.checkEvents();
  potentiomenter.checkEvents();
  eventManager.handleEvents();
  delay(100);
}