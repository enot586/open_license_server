#pragma once

#include "command.h"

class CommandFactory
{
public:
    CommandFactory();

    static CommandPtr create(const CommandParams& params);
};
