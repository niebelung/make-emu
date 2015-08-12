#include "depgraph.h"

#include <stdexcept>
#include <sstream>
#include <iostream>

namespace make_emu
{
Node::Node(Target * target)
{
    m_target = std::shared_ptr<Target>(target);
}

std::list< std::string > Node::getAdjacent() const
{
    return m_target->dependencies();
}

bool Node::isVisited() const
{
    return m_visited;
}

void Node::visit()
{
    m_visited = true;
}

std::string Node::name() const
{
    return m_target->name();
}

std::shared_ptr<Target> Node::data()
{
    return m_target;
}


DepGraph::DepGraph()
{
    m_root = std::make_shared<Node>(new Target(std::string()));
    m_nodes->insert(std::make_pair(std::string(), m_root));
}

void DepGraph::addNode(const std::string& key, std::shared_ptr< make_emu::Target > target)
{

    if(key.empty())
    {
        std::stringstream ss;
        ss << "Target key empty!";
        throw ss.str();
    }
    auto node = std::make_shared<Node>(target.get());
    auto res = m_nodes->insert(std::make_pair(key, node));
    if ( ! res.second ) {
        std::stringstream ss;
        ss << "Target " << key << " duplicated!";
        throw ss.str();
    }
    m_root->data()->addDependency(key);
}

bool DepGraph::isOnStack(const std::string & key)
{
    for(auto & item : *m_stack.get())
    {
        if(!item.compare(key))
        {
            return true;
        }
    }
    return false;
}

bool DepGraph::isCyclic(std::shared_ptr<Node> node)
{
    node->visit();
    m_stack->push_back(node->name());

    auto names = node->getAdjacent();
    for(auto & name : names)
    {
        //TEST
        std::cout << name << std::endl;
        auto adj = m_nodes->find(name);
//         //TEST
//         std::cout << name << std::endl;

        if(adj == m_nodes->end())
        {
            std::stringstream ss;
            ss << "Missing dependency  " << name << " for target " << node->name() << "!!!";
            throw ss.str();
        }
//         //TEST
//         std::cout << name << std::endl;

        if(!adj->second->isVisited())
        {
        //TEST
        std::cout << "foo" << std::endl;
            return isCyclic(adj->second);
        }
        else if (isOnStack(adj->first))
        {
            return true;
        }
        //TEST
        std::cout << "bar" << std::endl;
    }

    m_stack->pop_back();
    return false;
}

bool DepGraph::isCyclic()
{
    if(isCyclic(m_root))
    {
        return true;
    }
    return false;
}

bool DepGraph::isConsistent()
{
    return true;
}

bool DepGraph::applyOperation(
    const std::string & key,
    std::function<bool(Target&)> f)
{
    if(key.empty())
    {
        std::stringstream ss;
        ss << "Target key empty!";
        throw ss.str();
    }

    auto node = m_nodes->find(key);
    if(node == m_nodes->end())
    {
        std::stringstream ss;
        ss << "Unknown target : " << key << "!!!";
        throw ss.str();
    }

    for(auto & name : node->second->getAdjacent())
    {
        if(!applyOperation(name,f))
        {
            return false;
        }
    }

    return f(*node->second->data().get());
}

}
