#include "fileparser.h"

namespace make_emu
{
FileParser::FileParser(std::string filename) :
    m_infile(filename)
{}
std::shared_ptr<Target> FileParser::readTarget()
{

}
}
