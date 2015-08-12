#include "actionprocessor.h"

#include <cstdlib>
#include <cstdio>
#include <string>
#include <sstream>

namespace make_emu
{
ActionProcessor::ActionProcessor()
{
    if (std::system(nullptr))
    {
        std::printf("Command interpreter is available!\n");
    }
    else
    {
        std::stringstream ss;
        ss << "Command interpreter is not available!";
        throw ss.str();
    }
}

ActionProcessor * ActionProcessor::instance()
{
    if(!m_instance)
    {
        m_instance = new ActionProcessor();
    }
    return m_instance;
}

void ActionProcessor::execute(const Action& action)
{
    if (std::system(action.toString().c_str()))
    {
        std::stringstream ss;
        ss << "Command " << action.toString() << " returned with error!";
        throw ss.str();
    }
}

ActionProcessor * ActionProcessor::m_instance = nullptr;
}
