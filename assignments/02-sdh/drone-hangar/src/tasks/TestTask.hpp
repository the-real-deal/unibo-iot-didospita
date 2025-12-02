#pragma once

#include "../core/tasks.hpp"

class TestTask : public Task<TestTask> {
public:
  TestTask();
};

class TestTaskAction : public TaskAction<TestTask> {
public:
  TestTaskAction();
  void step(const TestTask *task, const uint64_t elapsedTime) override;
};
