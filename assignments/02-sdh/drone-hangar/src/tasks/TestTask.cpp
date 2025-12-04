#include "TestTask.hpp"
#include <Arduino.h>

TestTask::TestTask(TestSensor *testSensor)
    : Task<TestTask>(new TestTaskAction()), testSensor(testSensor) {}

void TestTaskAction::step(TestTask *task, uint64_t elapsedTime) {
  Serial.println("TestTask: " + String(task->testSensor->getValue()));
}
