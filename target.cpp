#include "target.h"

namespace make_emu
{

Target::Target(std::string name) :
    m_name(name)
{}

void Target::addAction(Action * action)
{
    std::shared_ptr<Action> p(action);
    m_actions.push_back(p);
}

void Target::addDependency(const std::string & name)
{
    m_deps.push_back(name);
}

const std::string & Target::name() const
{
    return m_name;
}

std::list<std::string> Target::dependencies() const
{
    return m_deps;
}
const std::list< std::shared_ptr<Action>>& Target::actions() const
{
    return m_actions;
}

void Target::markProcessed()
{
    m_isProcessed = true;
}

const bool Target::isProcessed() const
{
    return m_isProcessed;
}

}
