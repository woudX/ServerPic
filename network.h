#ifndef NETWORK
#define NETWORK

#include "stdafx.h"

class HttpContentType
{
public:
    static string Json;
    static string Text;
    static string Xml;
    static string FormData;
};

class HttpRequest
{
public:
    HttpRequest();
    HttpRequest(string pUrl);
    virtual ~HttpRequest();

    HttpRequest *AddParam(string key, string val);

    /**
     * @brief Connect with url
     * @return
     */
    virtual string Connect();

    /**
     * @brief don' call this method by yourself, callback will be called automatically
     * @param retText
     */
    virtual void Callback(string retText);

    /**
     * @brief static method for http request
     * @param data
     * @param size
     * @param nmemb
     * @param user_data
     * @return
     */
    static size_t Writer(char *data, size_t size, size_t nmemb, void *user_data);


    string url;
    string text;
    string contentType;

protected:
    CURL *_curl;
    CURLcode _res;
    struct curl_slist *_headers;
    map<string, string> *_params;

    void _InitConnection();
    string _ArrangeParams();

};

class HttpRequestGet : public HttpRequest
{
public:
    HttpRequestGet(string pUrl);

    virtual string Connect();
    virtual void Callback(string retText);

    virtual ~HttpRequestGet();
};

class HttpRequestPost : public HttpRequest
{
public:
    HttpRequestPost(string pUrl);

    virtual string Connect();
    virtual ~HttpRequestPost();

};


#endif // NETWORK
