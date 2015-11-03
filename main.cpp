#include "zextractor.h"
#include "filesys.h"
#include "assist.h"
#include "global.h"
#include "opencvex.h"
#include "jsonx.h"
#include "createproc.h"
#include "network.h";

void ExtractZipFile(char *filename);

int main(int argc, char* argv[])
{
    //string req = Network::Request("http://192.168.89.1/Test/config.json");

    AppSetting *setting = AppSetting::Instance();
    JsonObject *jsonObj = new JsonObject();
    jsonObj->LoadFromFile("config.json");
    setting->Load(jsonObj);
    safe_del(jsonObj);

    if (argc == 1)
        ExtractZipFile("Desktop.zip");
    else
        ExtractZipFile(argv[1]);

    safe_del(setting);
    return 0;
}

void ExtractZipFile(char* filename)
{
    string zipFilename(filename);

    CreateProc *proc = new CreateProc(filename);
    proc->Init();
    proc->Run();

    safe_del(proc);
}
