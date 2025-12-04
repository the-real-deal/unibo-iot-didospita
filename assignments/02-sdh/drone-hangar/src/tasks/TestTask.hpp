#pragma once

#include "TestSensor.hpp"
#include "core/tasks.hpp"

class TestTaskAction;

class TestTask : public Task<TestTask> {
  friend TestTaskAction;

private:
  TestSensor *testSensor;

public:
  TestTask(TestSensor *testSensor);
};

class TestTaskAction : public TaskAction<TestTask> {
public:
  void step(TestTask *task, uint64_t elapsedTime) override;
};
