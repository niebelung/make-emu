#include "action.h"

#include <cstdlib>

namespace make_emu
{
Action::Action(const std::string & command) :
    m_command(command)
{
}

std::string Action::toString() const
{
    return m_command;
}
}
