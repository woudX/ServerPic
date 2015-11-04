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
    //  Get system config
    HttpRequest *getConfigJsonReq = new HttpRequestGet("http://192.168.89.1/Test/config.json");
    getConfigJsonReq->contentType = HttpContentType::Json;
    getConfigJsonReq->Connect();

    //  Loading config
    AppSetting *setting = AppSetting::Instance();
    JsonObject *jsonObj = new JsonObject();
    jsonObj->LoadFromText(getConfigJsonReq->text);
    setting->Load(jsonObj);

    //  Release memory
    safe_del(jsonObj);
    safe_del(getConfigJsonReq);

    //  Working
    if (argc == 1)
        ExtractZipFile("Desktop.zip");
    else
        ExtractZipFile(argv[1]);

    //  Release mem and exit
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
