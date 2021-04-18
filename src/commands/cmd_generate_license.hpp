#pragma once

#include "command.h"

class CmdGenerateLicense : public Command
{
    public:
        explicit CmdGenerateLicense(const CommandParams& params) :
            Command(params)
        {

        }

        ResponsePtr execute() override
        {
            return {};
        }
};
