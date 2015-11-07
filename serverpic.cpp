#include "serverpic.h"
#include "zextractor.h"
#include "filesys.h"
#include "assist.h"
#include "global.h"
#include "opencvex.h"
#include "jsonx.h"
#include "createproc.h"
#include "network.h";
#include "commandmgr.h"
#include "cthread.h"
#include "function.h"

ServerPic::ServerPic()
{
    _cmdMgr = new CommandMgr();
}

int ServerPic::Install()
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

int ServerPic::Uninstall()
{
    curl_global_cleanup();
    safe_del(AppSetting::Instance());
}

int ServerPic::Run(int argc, char *argv[])
{
    CThread serverThread(&ServerPic::_StartServer, this);
    serverThread.Detach();

    //  Mainloop
    while (!serverThread.IsOver())
    {
        _cmdMgr->Update();
        sleep(2);
    }
    return 0;
}

ServerPic::~ServerPic()
{

}

void ServerPic::_CreateProcFunc(string filename)
{
    string zipFilename(filename);

    CreateProc *proc = new CreateProc(filename);
    thread t([](CreateProc *cp) {
        cp->Init();
        cp->Run();
        safe_del(cp);
    }, proc);

    t.detach();
}

void ServerPic::_StartServer()
{
    int sockfd, newsockfd, portno;
    socklen_t clilen;
    char buffer[1024];

    struct sockaddr_in serv_addr, cli_addr;
    int n;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        debug_log("error opening socket : %d\n", sockfd);
        return;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = 8999;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        debug_log("%s\n","error on binding");
        return;
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (true)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0)
        {
            debug_log("error on acceptig : %d\n", newsockfd);
            continue;
        }

        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);

        if (n < 0)
        {
            debug_log("error on read buffer : %d", n);
            continue;
        }

        //  process command
        debug_log("recieve message : %s", buffer);
        string cmdStr = FuncLib::filter(buffer, '\n');
        if (cmdStr == "exit")
            break;

        _cmdMgr->PushCmd(_cmdMgr->AnalysisCmd(cmdStr));

        //  feedback infos
        n = write(newsockfd, "got it", 6);
    }
    close(newsockfd);
    close(sockfd);
    return ;
}

