#pragma once

#include "command.h"
#include "../globals.h"

class CmdGetVersion : public Command
{
    public:
        explicit CmdGetVersion(const CommandParams& params) :
            Command(params)
        {

        }

        ResponsePtr execute() override
        {
            return std::make_shared<Response>(
                        CommandParams( {
                                           { "status", std::to_string(CommandStatus::OK) },
                                           { "version", std::to_string(VERSION[0])
                                                        + "." + std::to_string(VERSION[1])
                                                        + "." + std::to_string(VERSION[2]) }
                                       } )
                   );
        }
};
