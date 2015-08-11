#ifndef __DEPTREE_H__
#define __DEPTREE_H__

#include "target.h"
#include <string>
#include <list>
#include <vector>
#include <functional>

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
    std::shared_ptr<Target> data(const std::string & key);
    virtual ~Node(){}
private:
    std::shared_ptr<Target> m_target;
//     std::list<std::shared_ptr<Node>> m_children;
    bool m_visited{false};
};

class DepGraph
{
public:
    void addNode(const std::string & key, Node * node);
    bool isCyclic();
    bool isConsistent();
    bool applyOperation(const std::string& key, std::function< bool > f);
private:
    bool isCyclic(const std::string & key);
    bool isOnStack(const std::string & key);
private:
    std::unique_ptr<std::map<std::string, std::shared_ptr<Node>>> m_nodes {new std::map<std::string, std::shared_ptr<Node>>};
    std::unique_ptr<std::vector<std::string>> m_stack {new std::vector<std::string>};
};
}
#endif
