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

const std::string Target::name() const
{
    return m_name;
}

const std::list<std::string> & Target::dependencies() const
{
    return m_deps;
}

}
