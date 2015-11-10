#include "serverpic.h"
#include "stdinc.h"
#include "createproc.h"
#include "commandmgr.h"


ServerPic::ServerPic()
{
    _cmdMgr = new CommandMgr();
}

int ServerPic::Install(int argc, char *argv[])
{
    string configUrl("http://192.168.89.1/Test/config.json");
    //  init error
    ErrorInfo::Init();
/*
    //  check params count
    if (argc < 2)
        return INVALID_ARGV_NUMBERS;

    //  load argv
    string configUrl(argv[1]);
*/
    //  init libcurl
    curl_global_init(CURL_GLOBAL_ALL);

    //  init system config
    HttpRequest *configReq = new HttpRequestGet(configUrl);
    configReq->contentType = HttpContentType::Json;
    configReq->Connect();
    if (configReq->text.empty())
        return EMPTY_CONFIG_FILE;

    AppSetting *setting = AppSetting::Instance();
    JsonObject *jsonObj = new JsonObject();
    jsonObj->LoadFromText(configReq->text);
    setting->Load(jsonObj);

    //  Release install memory
    safe_del(jsonObj);
    safe_del(configReq);

    return NO_ERROR;
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
        err_log("error opening socket : %s\n", strerror(errno));
        return;
    }

    int reuse;
    if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(int)) == -1)
    {
        err_log("error set resue port : %s\n", strerror(errno));
        return;
    }

    bzero((char *) &serv_addr, sizeof(serv_addr));
    portno = AppSetting::Instance()->listen_port;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    {
        err_log("error on binding : %s\n",strerror(errno));
        return;
    }

    listen(sockfd, 5);
    clilen = sizeof(cli_addr);

    while (true)
    {
        newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

        if (newsockfd < 0)
        {
            err_log("error on acceptig : %s\n", strerror(errno));
            continue;
        }

        bzero(buffer, 256);
        n = read(newsockfd, buffer, 255);

        if (n < 0)
        {
            err_log("error on read buffer : %s", strerror(errno));
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

