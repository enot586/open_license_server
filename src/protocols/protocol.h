#pragma once

#include <vector>
#include <memory>
#include <string>

#include "../commands/command.h"

class Protocol
{
public:
    virtual CommandPtr receive() = 0;
    virtual std::string Serialize(const CommandPtr command) = 0;
};

using ProtocolPtr = std::shared_ptr<Protocol>;
