#include "zextractor.h"
#include "filesys.h"
#include "assist.h"
#include "global.h"
#include "opencvex.h"
#include "jsonx.h"
#include "createproc.h"

void ExtractZipFile(char *filename);
void ListFile(const char *path);

int main(int argc, char* argv[])
{
    AppSetting *setting = AppSetting::Instance();
    setting->Load("config.json");

    if (argc == 1)
    {
        ExtractZipFile("Desktop.zip");
    }
    else
    {
        ExtractZipFile(argv[1]);
    }

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
