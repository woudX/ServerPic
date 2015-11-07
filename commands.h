#ifndef COMMANDS
#define COMMANDS

#include "stdafx.h"

class CThread;
class CreateProc;

class BasicCommand
{
public:
    BasicCommand(vector<string> params, char splitChar = ',');
    virtual void Execute();
    bool IsOver();

    ~BasicCommand();

protected:
    CThread *_t;
    vector<string> _params;
};

class FormalCommand : public BasicCommand
{
public:
    FormalCommand(vector<string> params, char splitChar = ',');
    virtual void Execute();

    ~FormalCommand();

protected:
    CreateProc *_createProc;
};

#endif // COMMANDS

