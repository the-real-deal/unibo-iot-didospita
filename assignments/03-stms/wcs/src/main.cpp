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
#include "tasks/lcd.hpp"

enum class Events : EventFamily
{
  Button,
  Potentiometer,
  Serial,
  OperationMode,
};

EventFamily family(Events event)
{
  return static_cast<EventFamily>(event);
}

EventsManager eventsManager;
SerialManager serialManager(family(Events::Serial));
PushButton button(BTN_PIN, family(Events::Button));
Potentiometer potentiomenter(POT_PIN, family(Events::Potentiometer));
ServoMotor servo(SERVO_PIN, 0);
Led builtinLed(LED_BUILTIN);
LCD lcd;

SystemStatusTask operationModeTask(SystemStatus::Automatic,
                                    family(Events::OperationMode),
                                    button.getFamily(),
                                    serialManager.getFamily());
LCDTask lcdTask(&lcd, operationModeTask.getFamily());

void setup()
{
  serialManager.begin(&eventsManager);
  serialManager.log("setup() started");

  I2CManager i2c;
  i2c.setup();

  uint8_t lcdAddress = i2c.scan();
  lcd.begin(lcdAddress);

  builtinLed.setup();
  servo.setup();

  button.begin(&eventsManager);
  potentiomenter.begin(&eventsManager);

  operationModeTask.begin(&eventsManager);
  lcdTask.begin(&eventsManager);

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
