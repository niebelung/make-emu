#ifndef __ACTION_H__
#define __ACTION_H__

#include <string>

namespace make_emu
{
class Action
{
    Action(const std::string & command);
    virtual ~Action(){}
};
}

#endif
