#include <Arduino.h>

#include "config.h"

#include "core/serial.hpp"
#include "devices/button.hpp"
#include "devices/servo.hpp"
#include "devices/potentiometer.hpp"
#include "devices/led.hpp"

#ifndef BTN_EVENT_FAMILY
#define BTN_EVENT_FAMILY 0
#endif

#ifndef POT_EVENT_FAMILY
#define POT_EVENT_FAMILY 1
#endif

class BtnLogObserver : public EventObserver<ButtonEvent>
{
protected:
  void onEvent(ButtonEvent eventData) override
  {
    Serial.print(F("BTN "));
    switch (eventData)
    {
    case ButtonEvent::Press:
      Serial.println(F("PRESS"));
      break;

    case ButtonEvent::Release:
      Serial.println(F("RELEASE"));
      break;
    }
    Serial.flush();
  }

public:
  BtnLogObserver() : EventObserver(BTN_EVENT_FAMILY) {}
};

class PotLogObserver : public EventObserver<PotentiometerEvent>
{
private:
  ServoMotor *servo;

protected:
  void onEvent(PotentiometerEvent eventData) override
  {
    int angle = (160 * eventData.value) + 10;
    Serial.print(F("POT :"));
    Serial.print(eventData.value);
    Serial.print(F("/"));
    Serial.println(angle);
    Serial.flush();
    this->servo->setAngle(angle);
  }

public:
  PotLogObserver(ServoMotor *servo) : EventObserver(POT_EVENT_FAMILY), servo(servo) {}
};

SerialManager serialManager;
EventsManager eventManager;
PushButton button(BTN_PIN, BTN_EVENT_FAMILY, &eventManager);
Potentiometer potentiomenter(POT_PIN, POT_EVENT_FAMILY, &eventManager);
ServoMotor servo(SERVO_PIN, 10);
Led builtinLed(LED_BUILTIN);

BtnLogObserver btnObserver;
PotLogObserver potObserver(&servo);

void setup()
{
  serialManager.setup();
  serialManager.log("setup() started");

  eventManager.registerObserver(&btnObserver);
  eventManager.registerObserver(&potObserver);
  
  builtinLed.setup();
  button.setup();
  potentiomenter.setup();
  servo.setup();

  serialManager.log("setup() finished");
}

void loop()
{
  builtinLed.toggle();
  potentiomenter.checkEvents();
  eventManager.handleEvents();
  delay(50);
}