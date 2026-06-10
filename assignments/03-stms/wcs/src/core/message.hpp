#pragma once

enum class SerialMessageType
{
  SERIAL_SYNC,
  LOG,
};
const char *const SERIAL_MESSAGE_TYPE_STRINGS[] = {
    "SERIAL_SYNC",
    "LOG",
};