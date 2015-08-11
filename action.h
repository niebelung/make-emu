#ifndef __ACTION_H__
#define __ACTION_H__

#include <string>

namespace make_emu
{
class Action
{
public:
    Action(const std::string & command);
    virtual ~Action(){}
    std::string toString() const;
private:
    std::string m_command;
};
}

#endif
