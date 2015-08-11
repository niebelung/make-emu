#ifndef __ACTIONPROCESSOR_H__
#define __ACTIONPROCESSOR_H__

#include "action.h"


namespace make_emu
{
class ActionProcessor
{
private:
    ActionProcessor();
    ActionProcessor( const ActionProcessor& ) = delete;
    ActionProcessor& operator=( ActionProcessor& ) = delete;
public:
    static ActionProcessor * instance();
    void execute(Action & action);
private:
    static ActionProcessor * m_instance;
};
}


#endif

