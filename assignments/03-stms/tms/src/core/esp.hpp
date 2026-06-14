#pragma once

#include <stddef.h>
#include <Arduino.h>

#ifndef ESP_DEFAULT_TASK_STACK_DEPTH
#define ESP_DEFAULT_TASK_STACK_DEPTH 10000
#endif

#ifndef ESP_DEFAULT_TASK_PRIORITY
#define ESP_DEFAULT_TASK_PRIORITY 1
#endif

struct ESPTaskConfig
{
    uint32_t stackDepth = ESP_DEFAULT_TASK_STACK_DEPTH;
    UBaseType_t priority = ESP_DEFAULT_TASK_PRIORITY;
};

TaskHandle_t createEspTask(const char *name, ESPTaskConfig config,
                           void *context, TaskFunction_t fn);
void deleteEspTask(TaskHandle_t *taskHandle);
