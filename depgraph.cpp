#include "depgraph.h"

#include <stdexcept>
#include <sstream>
#include <iostream>

namespace make_emu
{
Node::Node(std::shared_ptr<Target> target)
{
    m_target = target;
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
    m_root = std::make_shared<Node>(std::make_shared<Target>(std::string()));
    m_nodes->insert(std::make_pair(std::string(), m_root));
}

void DepGraph::addNode(const std::string& key, std::shared_ptr< make_emu::Target > target)
{
    if(m_nodeCnt >= MAX_TARGETS)
    {
        std::stringstream ss;
        ss << "Maximum 1000000 targets allowed";
        throw ss.str();
    }
    if(key.empty())
    {
        std::stringstream ss;
        ss << "Target key empty!";
        throw ss.str();
    }
    auto node = std::make_shared<Node>(target);
    auto res = m_nodes->insert(std::make_pair(key, node));
    if ( ! res.second ) {
        std::stringstream ss;
        ss << "Target " << key << " duplicated!";
        throw ss.str();
    }
    m_root->data()->addDependency(key);
    m_nodeCnt++;
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
        auto adj = m_nodes->find(name);

        if(adj == m_nodes->end())
        {
            std::stringstream ss;
            ss << "Missing dependency  " << name << " for target " << node->name() << "!!!";
            throw ss.str();
        }

        if(!adj->second->isVisited())
        {
            if(isCyclic(adj->second))
            {
                return true;
            }
        }
        else if (isOnStack(adj->first))
        {
            return true;
        }
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
    std::function<bool(Target&)> f,
    const std::string & key)
{
    auto node = m_nodes->find(key);
    if(node == m_nodes->end())
    {
        std::stringstream ss;
        ss << "Unknown target : " << key << "!!!";
        throw ss.str();
    }

    for(auto & name : node->second->getAdjacent())
    {
        if(!applyOperation(f,name))
        {
            return false;
        }
    }
    if(m_root != node->second)
    {
        return f(*node->second->data().get());
    }
    return true;
}

}
