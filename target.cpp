#include "target.h"

namespace make_emu
{

Target::Target(std::string name) :
    m_name(name)
{
}

void Target::addAction(Action * action)
{
    std::shared_ptr<Action> p(action);
    m_actions.push_back(p);
}

void Target::addDependency(const std::string & name)
{
    m_deps.push_back(name);
}

// bool Target::build()
// {
//     for(auto & item : m_deps)
//     {
//         if(!item.build())
//         {
//             return false;
//         }
//     }
//     for(auto & item : m_actions)
//     {
//         if(!item.run())
//         {
//             return false;
//         }
//     }
//     return true;
// }

const std::string & Target::name() const
{
    return m_name;
}

std::list<std::string> Target::dependencies() const
{
    //TEST
    std::printf("m_deps.name() -> %s\n",(m_deps.begin())->c_str());
    return m_deps;
}
const std::list< std::shared_ptr<Action>>& Target::actions() const
{
    return m_actions;
}

}
