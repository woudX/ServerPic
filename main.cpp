#include "serverpic.h"
#include "stdinc.h"

int main(int argc, char* argv[])
{
    ServerPic *mainApp = new ServerPic();
    int result;
    if ((result = mainApp->Install(argc, argv)) == NO_ERROR)
    {
        mainApp->Run(argc, argv);
        mainApp->Uninstall();
    }
    else
        err_log("error : %s(%d)\n", error_msg(result), result);

    safe_del(mainApp);
    return 0;
}
