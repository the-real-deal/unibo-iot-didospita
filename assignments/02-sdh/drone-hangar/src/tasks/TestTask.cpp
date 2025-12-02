#include "TestTask.hpp"

TestTask::TestTask() : Task<TestTask>(new TestTaskAction()) {}

void TestTaskAction::step(const TestTask *task, const uint64_t elapsedTime) {}
