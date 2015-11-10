#include "commands.h"
#include "createproc.h"
#include "stdinc.h"

BasicCommand::BasicCommand(vector<string> params, char splitChar):result(-1)
{
    _params = params;
}

void BasicCommand::OnStart()
{
    debug_log("%s\n", "please inherite this method!");
}

void BasicCommand::Execute()
{
    debug_log("%s\n", "please inherite this method!");
}

void BasicCommand::OnFinished()
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

void FormalCommand::OnStart()
{
    if (_params.size() < 2)
        return ;

    string filename = _params[0];
    int pid = Convert::ToInt(_params[1]);

    _createProc = new CreateProc(filename, pid);

    log("formal command start : pid - %d, file - %s\n",
        _createProc->PID(), _createProc->zipFullname.c_str());
}

void FormalCommand::Execute()
{
    if (_createProc)
    {
        //  Run command
        _t = new CThread([](FormalCommand *_this) {
            _this->_createProc->Init();
            _this->result = _this->_createProc->Run();
        }, this);

        _t->Detach();
    }
    else
    {
        result = 0;
    }
}

void FormalCommand::OnFinished()
{
    log("project (%d) formal finished : %s(%d)\n", _createProc->PID(), error_msg(result), result);
}

FormalCommand::~FormalCommand()
{
    safe_del(_createProc);
}
