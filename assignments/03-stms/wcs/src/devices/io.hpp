#pragma once

#include "kernel/setup.hpp"

class ExternalInput : public Setup
{
public:
    virtual void read() = 0;
    ~ExternalInput() = default;
};
