#pragma once

#include <string>
#include <map>
#include <memory>

#include "command_code.h"

using CommandParams = std::map<std::string, std::string>;

class Command : public std::enable_shared_from_this<Command>
{
public:
    Command();
    explicit Command(const CommandParams& params);

    virtual std::shared_ptr<Command> execute();

    const CommandParams& get_params() const { return m_params; }

protected:
    CommandParams m_params;
};

using CommandPtr = std::shared_ptr<Command>;

using Response = Command;
using ResponsePtr = CommandPtr;
