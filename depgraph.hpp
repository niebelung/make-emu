#ifndef __DEPTREE_H__
#define __DEPTREE_H__

#include "target.h"
#include <string>
#include <list>
#include <map>
#include <vector>
#include <functional>

namespace make_emu
{

template <typename T>
using TargetRef = std::shared_ptr<T>;
template <typename T>
using NodeRef = std::shared_ptr<Node<T>>;
template <typename T>
using NodeType = Node<T>;

template <typename T>
class Node
{
public:
    Node(TargetRef target);
    void addAdjacent(NodeRef node);
    std::list<NodeRef> & getAdjacent() const;
    bool isVisited() const;
    void visit();
    TargetRef data();
    virtual ~Node(){}
private:
    TargetRef m_target;
    std::list<NodeRef> m_deps;
    bool m_visited{false};
};

template <typename T>
Node<T>::Node(TargetRef target)
{
    m_target = target;
}

template <typename T>
std::list<NodeRef> & Node<T>::getAdjacent() const
{
    return m_deps;
}

template <typename T>
void Node<T>::addAdjacent(NodeRef node)
{
    m_deps.push_back(node);
}

template <typename T>
bool Node<T>::isVisited() const
{
    return m_visited;
}

template <typename T>
void Node<T>::visit()
{
    m_visited = true;
}

template <typename T>
TargetRef Node<T>::data()
{
    return m_target;
}


template <typename T, typename Key, int SIZE = 1000000>
class DepGraph
{
public:
    template<typename ... Args> DepGraph();
    void addNode(const Key& key, TargetRef target);
    void addEdge(const Key& from, const Key& to);
    bool isCyclic();
    bool isConsistent();
    bool applyOperation(std::function< bool(T&) > f,const Key& key = Key());
private:
    constexpr static int MAX_TARGETS {SIZE};
    
    bool isCyclic(NodeRef node);
    bool isOnStack(NodeRef node);
    bool applyOperation(std::function< bool(T&) > f,NodeRef node);
private:
    std::unique_ptr<std::map<Key, NodeRef>> m_nodes {new std::map<Key, NodeRef>};
    std::unique_ptr<std::vector<NodeRef>> m_stack {new std::vector<NodeRef>};
    NodeRef m_root;
    int m_nodeCnt{0};
};

template <typename T, typename Key, int SIZE>
template<typename ...Args>
DepGraph<T,Key,SIZE>::DepGraph(Args... args)
{
    m_root = std::make_shared<NodeType>(std::make_shared<T>(args));
    m_nodes->insert(std::make_pair(T(), m_root));
}

template <typename T, typename Key, int SIZE>
void DepGraph<T,Key,SIZE>::addNode(const Key& key, TargetRef target)
{
    if(m_nodeCnt >= MAX_TARGETS)
    {
        std::stringstream ss;
        ss << "Maximum " << MAX_TARGETS << " nodes allowed";
        throw ss.str();
    }
    if(key.empty())
    {
        std::stringstream ss;
        ss << "Target key empty!";
        throw ss.str();
    }
    auto node = std::make_shared<NodeType>(target);
    auto res = m_nodes->insert(std::make_pair(key, node));
    if ( ! res.second ) {
        std::stringstream ss;
        ss << "Target " << key << " duplicated!";
        throw ss.str();
    }
    m_root->addAdjacent(key);
    m_nodeCnt++;
}

template <typename T, typename Key, int SIZE>
void DepGraph<T,Key,SIZE>::addEdge(const Key& from, const Key& to)
{
    auto adj = m_nodes->find(from);

    if(adj == m_nodes->end())
    {
        std::stringstream ss;
        ss << "Missing dependency  " << name << " for target " << node->name() << "!!!";
        throw ss.str();
    }
}

template <typename T, typename Key, int SIZE>
bool DepGraph<T,Key,SIZE>::isOnStack(NodeRef node)
{
    for(auto & item : *m_stack)
    {
        if(item == node)
        {
            return true;
        }
    }
    return false;
}

template <typename T, typename Key, int SIZE>
bool DepGraph<T,Key,SIZE>::isCyclic(NodeRef node)
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
        else if (isOnStack(adj->second))
        {
            return true;
        }
    }

    m_stack->pop_back();
    return false;
}

template <typename T, typename Key, int SIZE>
bool DepGraph<T,Key,SIZE>::isCyclic()
{
    if(isCyclic(m_root))
    {
        return true;
    }
    return false;
}

template <typename T, typename Key, int SIZE>
bool DepGraph<T,Key,SIZE>::isConsistent()
{
    return true;
}

template <typename T, typename Key, int SIZE>
bool DepGraph<T,Key,SIZE>::applyOperation(
    std::function< bool(T&) > f,
    NodeRef node)
{
    for(auto & adj : node->getAdjacent())
    {
        if(!applyOperation(f,adj))
        {
            return false;
        }
    }
    if(m_root != node)
    {
        return f(*node->data().get());
    }
    return true;
}

template <typename T, typename Key, int SIZE>
bool DepGraph<T,Key,SIZE>::applyOperation(
    std::function< bool(T&) > f,
    const Key& key = Key())
{
    auto node = m_nodes->find(key);
    if(node == m_nodes->end())
    {
        std::stringstream ss;
        ss << "Unknown target : " << key << "!!!";
        throw ss.str();
    }
    return applyOperation(f,node->second);

}

}
#endif
