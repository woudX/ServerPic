#ifndef COMMANDMGR_H
#define COMMANDMGR_H

#include "stdafx.h"

class BasicCommand;

class CommandMgr
{
public:
    CommandMgr();

    BasicCommand *AnalysisCmd(string cmd);
    void PushCmd(BasicCommand *pVal);
    void Update();

    ~CommandMgr();

    int maxParallalCommands;

private:

    void _ExecuteCmd(BasicCommand *pval);

    mutex _waitingListMutex;
    list<BasicCommand *> _waitingList;
    list<BasicCommand *> _runningList;
};

#endif // COMMANDMGR_H
