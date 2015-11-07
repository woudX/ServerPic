#include "commands.h"
#include "function.h"
#include "createproc.h"
#include "cthread.h"
#include "convert.h"

BasicCommand::BasicCommand(vector<string> params, char splitChar)
{
    _params = params;
}

void BasicCommand::Execute()
{
    debug_log("%s\n", "please inherite this method!");
}

BasicCommand::~BasicCommand()
{
    safe_del(_t);
}

bool BasicCommand::IsOver()
{
    if (_t)
        return _t->state == CThreadState::Abort || _t->state == CThreadState::Finish;
    else
        return false;
}

/// FormalCommand
//////////////////////////////////////////////////////
FormalCommand::FormalCommand(vector<string> param, char splitChar):BasicCommand(param, splitChar)
{

}

void FormalCommand::Execute()
{
    if (_params.size() < 2)
        return ;

    string filename = _params[0];
    int pid = Convert::ToInt(_params[1]);

    _createProc = new CreateProc(filename, pid);

    //  Run command
    _t = new CThread([](FormalCommand *_this) {
        _this->_createProc->Init();
        _this->_createProc->Run();
    }, this);

    _t->Detach();
}

FormalCommand::~FormalCommand()
{
    safe_del(_createProc);
}
