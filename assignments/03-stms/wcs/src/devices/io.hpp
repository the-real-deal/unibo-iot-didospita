#pragma once

#include "core/setup.hpp"

class ExternalInput : public Setup
{
public:
    virtual void read() = 0;
    ~ExternalInput() = default;
};
