#include "esp.hpp"

TaskHandle_t createEspTask(ESPTaskConfig config, void *context, TaskFunction_t fn)
{
    TaskHandle_t taskHandle = nullptr;
    xTaskCreate(fn,
                config.name, config.stackDepth,
                context, config.priority,
                &taskHandle);
    return taskHandle;
}

void deleteEspTask(TaskHandle_t *taskHandle)
{
    // https://docs.espressif.com/projects/esp-idf/en/stable/esp32/api-reference/system/freertos_idf.html#_CPPv411xTaskCreate14TaskFunction_tPCKcK22configSTACK_DEPTH_TYPEPCv11UBaseType_tPC12TaskHandle_t
    if (taskHandle == nullptr || *taskHandle == nullptr)
    {
        return;
    }
    vTaskDelete(*taskHandle);
    *taskHandle = nullptr;
}
