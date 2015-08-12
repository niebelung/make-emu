#include "fileparser.h"
#include "depgraph.h"
#include "target.h"
#include "actionprocessor.h"
#include "action.h"

#include <iostream>

int main(int argc, char **argv) {
    if(argc != 2)
    {
        std::cout << "Wrong args num : " << argc - 1 << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
    auto actionProcessor = make_emu::ActionProcessor::instance();
    make_emu::FileParser parser(std::string("Makefile"));

    auto targetList = parser.readTargets();
    make_emu::DepGraph depGraph;
    for(auto & target : targetList)
    {
        depGraph.addNode(target->name(), target);
        target.reset();
    }

    if(depGraph.isCyclic())
    {
        std::cout << "Cyclic dependencies detected!"<< std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "bla" << std::endl;

    depGraph.applyOperation(
        std::string(argv[1]),
        [&](make_emu::Target & target)
        {
            for(auto & action : target.actions())
            {
                actionProcessor->execute(*action.get());
            }
            return true;
        });


    }
    catch(std::string & e)
    {
           std::cout << e << std::endl;
    }

}
