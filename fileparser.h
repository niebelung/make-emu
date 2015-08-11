#ifndef __FILEPARSER_H__
#define __FILEPARSER_H__

#include <string>
#include <list>
#include <memory>
#include <fstream>

#include "target.h"

namespace make_emu
{
class FileParser
{
    FileParser(std::string filename);
    virtual ~FileParser(){}
public:
    std::shared_ptr<Target> readTarget();
private:
    std::shared_ptr<Target> m_currentTarget;
    std::ifstream           m_infile;

};
}


#endif

