#include "core/scheduler.hpp"
#include "tasks/TestTask.hpp"
#include <Arduino.h>

Scheduler *scheduler;
TestSensor *sensor;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
  }

  scheduler = new Scheduler(500);
  sensor = new TestSensor(A0);
  scheduler->addInput(sensor);
  scheduler->addThread(new TestTask(sensor));
}

void loop() { scheduler->advance(); }
