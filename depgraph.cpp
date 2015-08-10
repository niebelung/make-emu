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


void DepGraph::addNode(const std::string & key, Node * node)
{
    auto res = m_nodes.insert(std::make_pair(key,std::shared_ptr<Node>(node)));
    if ( ! res.second ) {
        std::stringstream ss;
        ss << "Target " << key << " duplicated!";
        throw std::runtime_error(ss.str());
    }
}

bool DepGraph::isCyclic(std::shared_ptr<Node> node)
{
    node->visit();
    m_stack.push(node->name());

    for(auto & name : node->getAdjacent())
    {
        auto adj = m_nodes.find(name);

        if(adj == m_nodes.end())
        {
            std::stringstream ss;
            ss << "Couldn't resolve dependency  " << name << " for target " << node->name() << "!!!";
            throw std::runtime_error(ss.str());
        }
        
        if(!adj.second->isVisited())
        {
            ret
        }
        else if ()
        {
            
        }
    }
}

bool DepGraph::isCyclic()
{
    
}

bool DepGraph::isConsistent()
{
    
}

}
