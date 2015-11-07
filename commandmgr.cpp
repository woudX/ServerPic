#include "commandmgr.h"
#include "commands.h"
#include "function.h"

CommandMgr::CommandMgr():maxParallalCommands(0)
{

}

void CommandMgr::PushCmd(BasicCommand *pVal)
{
    if (pVal != NULL)
        _waitingList.push_back(pVal);
}

BasicCommand *CommandMgr::AnalysisCmd(string cmd)
{
    BasicCommand *retCmd = NULL;
    vector<string> cmdParams;
    FuncLib::split(cmd, ',', cmdParams);

    if (cmdParams.size() < 0)
        return retCmd;

    //  get command type
    string cmdType = cmdParams[0];
    cmdParams.erase(cmdParams.begin());

    //  create command
    if (cmdType == "formal")
        retCmd = new FormalCommand(cmdParams);
    else
        err_log("error : undefined command type %s\n", cmdType.c_str());

    return retCmd;
}

void CommandMgr::Update()
{
    //  recycle overed command in runninglist
    list<BasicCommand *> finishedCmd;
    for (auto runningCmd : _runningList)
    {
        if (runningCmd->IsOver())
            finishedCmd.push_back(runningCmd);
    }

    //  Add new command to running list
    list<BasicCommand *> removeFromWaiting;

    int runningCount = _runningList.size();
    while (_waitingList.size() > 0 && (maxParallalCommands == 0 || runningCount < maxParallalCommands))
    {
        auto item = _waitingList.front();
        _waitingList.pop_front();

        removeFromWaiting.push_back(item);
    }

    //  Remove finished cmd
    for (auto item : finishedCmd)
    {
        _runningList.remove(item);
        safe_del(item);
        item = nullptr;
    }
    finishedCmd.clear();

    //  Remove cmd from waiting list, and add these cmds to running list
    _waitingListMutex.lock();
    for (auto item : removeFromWaiting)
    {
        _waitingList.remove(item);

        _ExecuteCmd(item);
    }
    removeFromWaiting.clear();
    _waitingListMutex.unlock();
}

CommandMgr::~CommandMgr()
{
    safe_del_container(_waitingList);
    safe_del_container(_runningList);
}

void CommandMgr::_ExecuteCmd(BasicCommand *pval)
{
    _runningList.push_back(pval);

    pval->Execute();
}
