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
class Node
{
public:
    Node(std::shared_ptr< make_emu::Target > target);
    std::list<std::string> getAdjacent() const;
    bool isVisited() const;
    void visit();
    std::string name() const;
    std::shared_ptr<Target> data();
    virtual ~Node(){}
private:
    std::shared_ptr<Target> m_target;
    bool m_visited{false};
};

class DepGraph
{
public:
    DepGraph();
    void addNode(const std::string& key, std::shared_ptr< make_emu::Target > target);
    bool isCyclic();
    bool isConsistent();
    bool applyOperation(std::function< bool(Target&) > f,const std::string& key = std::string());
private:
    constexpr static int MAX_TARGETS {1000000};
    
    bool isCyclic(std::shared_ptr<Node> node);
    bool isOnStack(const std::string & key);
private:
    std::unique_ptr<std::map<std::string, std::shared_ptr<Node>>> m_nodes {new std::map<std::string, std::shared_ptr<Node>>};
    std::unique_ptr<std::vector<std::string>> m_stack {new std::vector<std::string>};
    std::shared_ptr<Node> m_root;
    int m_nodeCnt{0};
};
}
#endif
