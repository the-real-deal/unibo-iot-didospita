#include "core/scheduler.hpp"
#include "tasks/TestTask.hpp"
#include "core/serial.hpp"
#include <Arduino.h>

Scheduler *scheduler;
TestSensor *sensor;
SerialManager *serialManager;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }

  scheduler = new Scheduler(500);
  sensor = new TestSensor(A0);
  serialManager = new SerialManager();
  scheduler->addInput(sensor);
  scheduler->addInput(serialManager);
  scheduler->addThread(new TestTask(sensor));

}

void loop() { scheduler->advance(); }
