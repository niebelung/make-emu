#include "depgraph.h"

#include <stdexcept>

namespace make_emu
{
Node::Node(Target * target)
{
    m_target = std::shared_ptr<Target> p(target);
}

const std::list<std::string> & Node::getAdjacent() const
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

std::shared_ptr<Target> Node::data(const std::string & key)
{
    return m_target;
}


void DepGraph::addNode(const std::string & key, Node * node)
{
    auto res = m_nodes.insert(std::make_pair(key,std::shared_ptr<Node>(node)));
    if ( ! res.second ) {
        std::stringstream ss;
        ss << "Target " << key << " duplicated!";
        throw ss.str();
    }
}

bool DepGraph::isOnStack(const std::string & key)
{
    for(auto & item : m_stack)
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

    for(auto & name : node->getAdjacent())
    {
        auto adj = m_nodes->find(name);

        if(adj == m_nodes->end())
        {
            std::stringstream ss;
            ss << "Missing dependency  " << name << " for target " << node->name() << "!!!";
            throw ss.str();
        }

        if(!adj.second->isVisited())
        {
            return isCyclic(adj.second);
        }
        else if (isOnStack(adj.second))
        {
            return true;
        }
    }

    m_stack->pop_back();
    return false;
}

bool DepGraph::isCyclic()
{
    for(auto & node : m_nodes)
    {
        if(isCyclic(node.second))
        {
            return true;
        }
    }
    return false;
}

bool DepGraph::isConsistent()
{
    return true;
}

bool DepGraph::applyOperation(
    const std::string & key,
    std::function<bool(std::shared_ptr<Node>)> f)
{
    auto node = m_nodes->find(key);
    if(node == m_nodes->end())
    {
        std::stringstream ss;
        ss << "Unknown target : " << key << "!!!";
        throw ss.str();
    }

    for(auto & name : node->getAdjacent())
    {
        if(!applyOperation(name,f))
        {
            return false;
        }
    }

    return f(node->data());
}

}
