#include "zextractor.h"
#include "filesys.h"
#include "assist.h"
#include "global.h"
#include "opencvex.h"
#include "jsonx.h"
#include "createproc.h"
#include "network.h";

void ExtractZipFile(char *filename);

int install()
{
    //  init libcurl
    curl_global_init(CURL_GLOBAL_ALL);

    //  init system config
    HttpRequest *configReq = new HttpRequestGet("http://192.168.89.1/Test/config.json");
    configReq->contentType = HttpContentType::Json;
    configReq->Connect();
    if (configReq->text.empty())
        return 1;

    AppSetting *setting = AppSetting::Instance();
    JsonObject *jsonObj = new JsonObject();
    jsonObj->LoadFromText(configReq->text);
    setting->Load(jsonObj);

    //  Release install memory
    safe_del(jsonObj);
    safe_del(configReq);

    return 0;
}

int uninstall()
{
    curl_global_cleanup();
    safe_del(AppSetting::Instance());
}

int main(int argc, char* argv[])
{
    //  setup system
    int errId;
    if ((errId = install()) != 0)
    {
        debug_log("install error : %d\n", errId);
        return -1;
    }

    //  Working
    if (argc == 1)
        ExtractZipFile("Desktop.zip");
    else
        ExtractZipFile(argv[1]);

    //  Release mem and exit
    uninstall();
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
