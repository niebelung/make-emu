#ifndef __TARGET_H__
#define __TARGET_H__

#include <string>
#include <list>
#include <memory>

#include "action.h"

namespace make_emu
{
class Target
{
public:
    Target(std::string name = std::string());
    
    virtual ~Target(){}
    void addAction(Action * action);
    void addDependency(const std::string & name);
//     bool build();
    const std::string& name() const;
    const std::list<std::string> & dependencies() const;
    const std::list<std::shared_ptr<Action>> & actions() const;
private:
    std::list<std::string> m_deps;
    std::list<std::shared_ptr<Action>> m_actions;
    const std::string m_name;
    
};    
}                           

#endif
