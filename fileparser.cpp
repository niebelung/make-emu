#include "fileparser.h"

#include <cstdio>
#include <cstring>

namespace make_emu
{
FileParser::FileParser(std::string filename) :
    m_infile(filename),
    m_currentTarget(nullptr)
{}

std::shared_ptr<Target> FileParser::readTarget()
{
    while(m_infile.good())
    {
        std::getline(m_infile,m_currentLine);
    }
}


std::list<std::string> FileParser::tokenize(std::string s)
{
  char str[] ="- This, a sample string.";
  char * pch;
  printf ("Splitting string \"%s\" into tokens:\n",str);
  pch = strtok (str," ,.-");
  while (pch != NULL)
  {
    printf ("%s\n",pch);
    pch = strtok (NULL, " ,.-");
  }
  return 0;
}


Edit & Run

}
