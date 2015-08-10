#ifndef __FILEPARSER_H__
#define __FILEPARSER_H__

#include <string>
#include <list>



namespace make_emu
{
//TODO Remove forward decl
class Target;

using Targets = std::list<Target>;

class FileParser
{
    FileParser(std::string filename);
    virtual ~FileParser(){}
public:
    
    
};
}


#endif

