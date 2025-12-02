#pragma once

#include <stdint.h>

template <class T> class Task;
template <class T> class TaskAction;

template <class T> class TaskAction {
  friend T;

protected:
  virtual void step(const T *task, const uint64_t elapsedTime) = 0;
  virtual ~TaskAction() = default;
};

template <class T> class Task {
  friend class TaskAction<T>;

private:
  TaskAction<T> *action;

protected:
  Task(TaskAction<T> *initialAction) { this->switchAction(initialAction); };

public:
  void step(const uint64_t elapsedTime) {
    this->action.step(this, elapsedTime);
  };

  void switchAction(TaskAction<T> *action) { this->action = action; }
};
