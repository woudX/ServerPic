#include "simpleapp.h"

SimpleApp::SimpleApp()
{

}

int SimpleApp::Install()
{
    return 0;
}

int SimpleApp::Run(int argc, char *argv[])
{
    debug_log("%s\n", "please inherite this method!");
    return 0;
}

int SimpleApp::Uninstall()
{
    return 0;
}

SimpleApp::~SimpleApp()
{

}

