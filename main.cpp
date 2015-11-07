#include "serverpic.h"


int main(int argc, char* argv[])
{
    ServerPic *mainApp = new ServerPic();
    mainApp->Install();
    mainApp->Run(argc, argv);
    mainApp->Uninstall();

    safe_del(mainApp);

    return 0;
}
