#include <Arduino.h>

#include "config.h"

#include "devices/serial.hpp"
#include "devices/button.hpp"
#include "devices/servo.hpp"
#include "devices/potentiometer.hpp"
#include "devices/led.hpp"
#include "devices/lcd.hpp"
#include "devices/i2c.hpp"
#include "tasks/system.hpp"
#include "tasks/display.hpp"
#include "tasks/control.hpp"

enum class Events : EventFamily
{
  Button,
  Potentiometer,
  Serial,
  Servo,
  StatusChange,
};

EventFamily family(Events event)
{
  return static_cast<EventFamily>(event);
}

EventsManager eventsManager;
SerialManager serialManager(family(Events::Serial));
PushButton button(BTN_PIN, family(Events::Button));
Potentiometer potentiomenter(POT_PIN, family(Events::Potentiometer));
ServoMotor servo(SERVO_PIN, 0, family(Events::Servo));
Led builtinLed(LED_BUILTIN);
LCD lcd;

SystemStatusTask systemStatusTask(SystemStatus::Automatic,
                                  family(Events::StatusChange),
                                  button.getFamily(),
                                  serialManager.getFamily());
DisplayTask displayTask(&lcd, &serialManager,
                        systemStatusTask.getFamily(), servo.getFamily());
ControlTask controlTask(&servo,
                        systemStatusTask.getFamily(),
                        potentiomenter.getFamily(),
                        serialManager.getFamily());

void setup()
{
  serialManager.begin(&eventsManager);
  serialManager.log("setup() started");

  I2CManager i2c;
  i2c.setup();

  uint8_t lcdAddress = i2c.scan();
  lcd.begin(lcdAddress);

  builtinLed.setup();

  button.begin(&eventsManager);
  potentiomenter.begin(&eventsManager);
  servo.begin(&eventsManager);
  systemStatusTask.begin(&eventsManager);
  displayTask.begin(&eventsManager);
  controlTask.begin(&eventsManager);

  serialManager.log("setup() finished");
}

void loop()
{
  builtinLed.toggle();

  serialManager.checkEvents();
  potentiomenter.checkEvents();

  eventsManager.handleEvents();
  delay(LOOP_DELAY_MS);
}
