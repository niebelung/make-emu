#include "fileparser.h"
#include "depgraph.hpp"
#include "target.h"
#include "actionprocessor.h"
#include "action.h"

#include <iostream>

int main(int argc, char **argv) {
    if(argc != 2 && argc !=1)
    {
        std::cout << "Wrong args num : " << argc - 1 << std::endl;
        return EXIT_FAILURE;
    }

    try
    {
    auto actionProcessor = make_emu::ActionProcessor::instance();
    make_emu::FileParser parser(std::string("Makefile"));

    auto targetList = parser.readTargets();
    make_emu::DepGraph<make_emu::Target, std::string, 1000000> depGraph;
    for(auto & target : targetList)
    {
        depGraph.addNode(target->name(), target);
        target.reset();
    }

    if(depGraph.isCyclic())
    {
        std::cout << "ERROR! Cyclic dependencies detected!"<< std::endl;
        return EXIT_FAILURE;
    }

    auto arg = (argc == 2) ? std::string(argv[1]) : std::string();

    depGraph.applyOperation(
        [&](make_emu::Target & target)
        {
            if(target.isProcessed())
            {
                return true;
            }
            std::cout << "\"" << target.name() << "\" :" << std::endl;
            for(auto & action : target.actions())
            {
                actionProcessor->execute(*action.get());
            }
            target.markProcessed();
            return true;
        },
        arg);


    }
    catch(std::string & e)
    {
           std::cout << "ERROR! " << e << std::endl;
    }

}
