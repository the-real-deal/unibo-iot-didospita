#pragma once

enum class OperationMode
{
    Manual,
    Automatic,
};
const char *const OPERATION_MODE_STRINGS[] = {
    "MANUAL",
    "AUTOMATIC",
};

struct OperationModeEvent
{
    OperationMode mode;
    OperationMode prev;
};
