#include "command_factory.h"
#include "command_code.h"

#include "cmd_get_version.hpp"
#include "cmd_generate_license.hpp"

CommandFactory::CommandFactory()
{

}

CommandPtr CommandFactory::create(const CommandParams& params)
{
    if ( params.find("command") != params.end() ) {

        auto command = std::stoi(params.at("command"));

        switch(command)
        {
            case GET_VERSION:
                return std::make_shared<CmdGetVersion>(params);

            case GENERATE_LICENSE:
                return std::make_shared<CmdGenerateLicense>(params);
        }
    }

    return {};
}
