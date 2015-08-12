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
public:
    FileParser(std::string filename);
    virtual ~FileParser(){}
    std::list<std::shared_ptr<Target>> readTargets();

private:
    void processHeader();
    void processActions();
    void processError();
    bool containsHeader(std::string & s);
    bool containsNothing(std::string & s);
    bool containsAction(std::string & s);

private:

    enum class State
    {
        INIT,
        HEADER,
        ACTIONS,
        DONE,
        ERROR
    };

    std::shared_ptr<Target>            m_currentTarget;
    std::list<std::shared_ptr<Target>> m_targets;
    std::string                        m_currentLine;
    std::ifstream                      m_infile;
    State                              m_state {State::INIT};
    int                                m_lineCount{0};
    bool                               m_good{true};

};
}


#endif

