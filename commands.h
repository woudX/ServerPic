#ifndef COMMANDS
#define COMMANDS

#include "stdafx.h"

class CThread;
class CreateProc;

class BasicCommand
{
public:
    BasicCommand(vector<string> params, char splitChar = ',');
    virtual void OnStart();
    virtual void Execute();
    virtual void OnFinished();
    bool IsOver();
    ~BasicCommand();

    int result;
protected:

    CThread *_t;
    vector<string> _params;
};

class FormalCommand : public BasicCommand
{
public:
    FormalCommand(vector<string> params, char splitChar = ',');
    virtual void OnStart();
    virtual void Execute();
    virtual void OnFinished();

    ~FormalCommand();

protected:
    CreateProc *_createProc;
};

#endif // COMMANDS

