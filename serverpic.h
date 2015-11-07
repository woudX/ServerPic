#ifndef SERVERPIC_H
#define SERVERPIC_H

#include "simpleapp.h"

class CommandMgr;

class ServerPic : public SimpleApp
{
public:
    ServerPic();

    virtual int Install(int argc, char *argv[]);
    virtual int Uninstall();
    virtual int Run(int argc, char *argv[]);

    ~ServerPic();

private:
    void _StartServer();

    int maxParallelTask = 10;

    CommandMgr *_cmdMgr;
};

#endif // SERVERPIC_H
