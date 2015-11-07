#ifndef SIMPLEAPP_H
#define SIMPLEAPP_H

#include "stdafx.h"

class SimpleApp
{
public:
    SimpleApp();

    virtual int Install(int argc, char *argv[]);
    virtual int Run(int argc, char* argv[]);
    virtual int Uninstall();

    virtual ~SimpleApp();
};

#endif // SIMPLEAPP_H
