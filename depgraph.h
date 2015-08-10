#ifndef __DEPTREE_H__
#define __DEPTREE_H__

#include "target.h"
#include <string>
#include <list>
#include <stack>

namespace make_emu
{
class Node
{
public:
    Node(Target * node);
    const std::list<std::string> & getAdjacent() const;
    bool isVisited() const;
    void visit();
    std::string name() const;
    virtual ~Node(){}
private:
    std::shared_ptr<Target> m_target;
    std::list<std::shared_ptr<Node>> m_children;
    bool m_visited{false};
};

class DepGraph
{
public:
    void addNode(const std::string & key, Node * node);
    bool isCyclic();
    bool isConsistent();
private:
    bool isCyclic(const std::string & key);
private:
    std::map<std::string, std::shared_ptr<Node>> m_nodes;
    std::stack<std::string> m_stack;
};
}
#endif 
