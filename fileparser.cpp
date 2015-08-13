#include "fileparser.h"

#include <cstdio>
#include <cstring>
#include <algorithm>
#include <sstream>
#include <cstdlib>
#include <unistd.h>

#include <iostream>

namespace make_emu
{
FileParser::FileParser(std::string filename) :
    m_infile(filename),
    m_currentTarget(nullptr)
{
        if(!m_infile.good())
        {
            std::stringstream ss;
            ss << "No file to parse. Please provide Makefile!";
            throw ss.str();
        }
}



void FileParser::processHeader()
{
    if(containsHeader(m_currentLine))
    {
        std::stringstream ss(m_currentLine);
        std::string token;
        if(!std::getline(ss,token,':'))
        {
            std::stringstream ss;
            ss << "Parsing error : tokenizer failed!! ";
            throw ss.str();
        }

        if(!m_currentTarget)
        {
            m_currentTarget.reset(new Target(std::string(token)));
        }
        else
        {
            std::stringstream ss;
            ss << "Parsing error : target already exists! ";
            throw ss.str();
        }

        while(std::getline(ss,token,' '))
        {
            if(token.empty())
            {
                continue;
            }
            m_currentTarget->addDependency(token);
        }

        m_state = State::ACTIONS;
        m_currentLine.clear();
        return;
    }
    else if(containsNothing(m_currentLine))
    {
        m_currentLine.clear();
        return;
    }
    else
    {
        m_state = State::ERROR;
        return;
    }

}

void FileParser::processActions()
{
    if(containsAction(m_currentLine))
    {
        std::size_t found = m_currentLine.find_first_not_of(" \t");
        m_currentTarget->addAction(new Action(m_currentLine.substr(found)));
        m_currentLine.clear();
    }
    else if(containsNothing(m_currentLine))
    {
        m_currentLine.clear();
        return;
    }
    else if(containsHeader(m_currentLine))
    {
        m_state = State::DONE;
        return;
    }
    else
    {
        m_state = State::ERROR;
        return;
    }
}

void FileParser::processError()
{

}

bool FileParser::containsNothing(std::string & s)
{
    std::size_t found = m_currentLine.find_first_not_of(" \t\n\r");
    if (found!=std::string::npos)
    {
        return false;
    }
    return true;
}
bool FileParser::containsHeader(std::string & s)
{
    if (s.size() == 0)
    {
        return false;
    }
    size_t n = std::count(s.begin(), s.end(), ':');
    if (n != 1)
    {
        return false;
    }
    auto pos = s.find(':');
    if(pos == 0)
    {
        return false;
    }
    if(s.at(0) == ' ' || s.at(0) == '\t')
    {
        return false;
    }
    return true;
}

bool FileParser::containsAction(std::string & s)
{
    if (s.size() == 0)
    {
        return false;
    }
    if(s.at(0) != ' ' && s.at(0) != '\t')
    {
        return false;
    }
    std::size_t found = m_currentLine.find_first_not_of(" \t");
    if (found==std::string::npos || found == 0)
    {
        return false;
    }
    return true;
}

std::list< std::shared_ptr< Target > > FileParser::readTargets()
{
    std::list<std::shared_ptr<Target>> targets;
    while(m_good)
    {
        if(m_currentLine.empty())
        {
            ++m_lineCount;
            if(!std::getline(m_infile,m_currentLine))
            {
                break;
            }
        }

        switch(m_state)
        {
        case State::INIT:
            m_state = State::HEADER;
            break;
        case State::HEADER:
            processHeader();
            break;
        case State::ACTIONS:
            processActions();
            break;
        case State::DONE:
            targets.push_back(m_currentTarget);
            m_currentTarget.reset();
            m_state = State::HEADER;
            break;
        case State::ERROR:
        {
            std::stringstream ss;
            ss << "Parsing error on line " << m_lineCount;
            throw ss.str();
            break;
        }
        default:
            break;
        }
    }
    if (m_state == State::ACTIONS)
    {
        targets.push_back(m_currentTarget);
    }
    m_currentTarget.reset();
    return targets;
}

}
