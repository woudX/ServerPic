#include "serverpic.h"


int main(int argc, char* argv[])
{
    ServerPic *mainApp = new ServerPic();
    if (mainApp->Install(argc, argv) == 0)
    {
        mainApp->Run(argc, argv);
        mainApp->Uninstall();
    }
    else
    {
        debug_log("%s\n", "please input config.json url");
    }

    safe_del(mainApp);

    return 0;
}
