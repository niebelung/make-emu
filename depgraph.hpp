#ifndef __DEPTREE_H__
#define __DEPTREE_H__

#include "target.h"
#include <string>
#include <list>
#include <map>
#include <vector>
#include <functional>
#include <type_traits>
#include <sstream>
#include <utility>

namespace make_emu
{

template <typename T>
class Node
{
public:
    Node(std::shared_ptr<T> target);
    void addAdjacent(std::shared_ptr<Node<T>> node);
    std::list<std::shared_ptr<Node<T>>> getAdjacent() const;
    bool isVisited() const;
    void visit();
    std::shared_ptr<T> data();
    virtual ~Node(){}
private:
    std::shared_ptr<T> m_target;
    std::list<std::shared_ptr<Node<T>>> m_deps;
    bool m_visited{false};
};

template <typename T>
Node<T>::Node(std::shared_ptr<T> target)
{
    m_target = target;
}

template <typename T>
std::list<std::shared_ptr<Node<T>>> Node<T>::getAdjacent() const
{
    return m_deps;
}

template <typename T>
void Node<T>::addAdjacent(std::shared_ptr<Node<T>> node)
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
std::shared_ptr<T> Node<T>::data()
{
    return m_target;
}

constexpr static int MAX_TARGETS {1000000};

template <typename Key, typename T, int SIZE = MAX_TARGETS>
class DepGraph
{
public:
    template<typename ... Args> DepGraph(Args ... args);
    void addNode(const Key& key, std::shared_ptr<T> target);
    void addEdge(const Key& from, const Key& to);
    bool isCyclic();
    bool isConsistent();
    bool applyOperation(std::function< bool(T&) > f,const Key& key = Key());
private:
    
    bool isCyclic(std::shared_ptr<Node<T>> node);
    bool isOnStack(std::shared_ptr<Node<T>> node);
    bool applyOperation(std::function< bool(T&) > f,std::shared_ptr<Node<T>> node);
private:
    std::unique_ptr<std::map<Key, std::shared_ptr<Node<T>>>> m_nodes {new std::map<Key, std::shared_ptr<Node<T>>>};
    std::unique_ptr<std::vector<std::shared_ptr<Node<T>>>> m_stack {new std::vector<std::shared_ptr<Node<T>>>};
    std::shared_ptr<Node<T>> m_root;
    int m_nodeCnt{0};
};

template <typename Key, typename T, int SIZE>
template<typename ...Args>
DepGraph<Key,T,SIZE>::DepGraph(Args... args)
{
    static_assert(SIZE <= MAX_TARGETS,"Exceeded MAX_TARGETS");
    m_root = std::make_shared<Node<T>>(std::make_shared<T>(args...));
    m_nodes->insert(std::make_pair(Key(), m_root));
}

template <typename Key, typename T, int SIZE>
void DepGraph<Key,T,SIZE>::addNode(const Key& key, std::shared_ptr<T> target)
{
    if(m_nodeCnt >= SIZE)
    {
        std::stringstream ss;
        ss << "Maximum " << SIZE << " nodes allowed!";
        throw ss.str();
    }
    auto node = std::make_shared<Node<T>>(target);
    auto res = m_nodes->insert(std::make_pair(key, node));
    if ( ! res.second ) {
        std::stringstream ss;
        ss << "Target " << key << " duplicated!";
        throw ss.str();
    }
    m_root->addAdjacent(node);
    m_nodeCnt++;
}

template <typename Key, typename T, int SIZE>
void DepGraph<Key,T,SIZE>::addEdge(const Key& from, const Key& to)
{
    auto fromNode = m_nodes->find(from);

    if(fromNode == m_nodes->end())
    {
        std::stringstream ss;
        ss << "Missing node : \"" << from <<  "\"!";
        throw ss.str();
    }
    
    auto toNode = m_nodes->find(to);

    if(toNode == m_nodes->end())
    {
        std::stringstream ss;
        ss << "Missing node : \"" << to <<  "\"!";
        throw ss.str();
    }

    fromNode->second->addAdjacent(toNode->second);
}

template <typename Key, typename T, int SIZE>
bool DepGraph<Key,T,SIZE>::isOnStack(std::shared_ptr<Node<T>> node)
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

template <typename Key, typename T, int SIZE>
bool DepGraph<Key,T,SIZE>::isCyclic(std::shared_ptr<Node<T>> node)
{
    node->visit();
    m_stack->push_back(node);

    auto nodes = node->getAdjacent();
    for(auto & item : nodes)
    {
        if(!item->isVisited())
        {
            if(isCyclic(item))
            {
                return true;
            }
        }
        else if (isOnStack(item))
        {
            return true;
        }
    }

    m_stack->pop_back();
    return false;
}

template <typename Key, typename T, int SIZE>
bool DepGraph<Key,T,SIZE>::isCyclic()
{
    if(isCyclic(m_root))
    {
        return true;
    }
    return false;
}

template <typename Key, typename T, int SIZE>
bool DepGraph<Key,T,SIZE>::isConsistent()
{
    return true;
}

template <typename Key, typename T, int SIZE>
bool DepGraph<Key,T,SIZE>::applyOperation(
    std::function< bool(T&) > f,
    std::shared_ptr<Node<T>> node)
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

template <typename Key, typename T, int SIZE>
bool DepGraph<Key,T,SIZE>::applyOperation(
    std::function< bool(T&) > f,
    const Key& key)
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
