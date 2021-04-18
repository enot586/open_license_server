#include "command.h"

Command::Command():
    m_params{}
{

}

Command::Command(const CommandParams& params) :
    m_params(params)
{

}

ResponsePtr Command::execute()
{
    return {};
}
