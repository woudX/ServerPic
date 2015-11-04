#include "network.h"

string HttpContentType::Json = "Content-Type: application/json";
string HttpContentType::Text = "Content-Type: text/html";
string HttpContentType::FormData = "Content-Type: multipart/form-data";

/// Request
//////////////////////////////////////////////////////////////////
HttpRequest::HttpRequest():url(""),text(""),_headers(NULL),contentType(HttpContentType::Text)
{
    _params = new map<string, string>();
}

HttpRequest::HttpRequest(string pUrl):url(pUrl),text(""),_headers(NULL),contentType(HttpContentType::Text)
{
    _params = new map<string, string>();
}

HttpRequest::~HttpRequest()
{
    if (_headers)
        curl_slist_free_all(_headers);

    if (_curl)
        curl_free(_curl);

    safe_del(_params);
}

HttpRequest *HttpRequest::AddParam(string key, string val)
{
    _params->insert(pair<string, string>(key, val));
    return this;
}

string HttpRequest::Connect()
{
    debug_log("%s\n", "please inherite this method");
}

void HttpRequest::Callback(string retText)
{
    text = retText;
}

size_t HttpRequest::Writer(char *data, size_t size, size_t nmemb, void *user_data)
{
    if (data != NULL)
    {
        static_cast<HttpRequest*>(user_data)->Callback(string(data));
        return nmemb;
    }

    return 0;
}

string HttpRequest::_ArrangeParams()
{
    string fParams = "";

    for (auto itor = _params->begin(); itor != _params->end(); ++itor)
        fParams.append(itor->first + "=" + itor->second + "&");

    fParams = fParams.substr(0, fParams.length() - 1);
    return fParams;
}


/// HttpRequestGet
//////////////////////////////////////////////////////////////////
HttpRequestGet::HttpRequestGet(string pUrl):HttpRequest(pUrl)
{

}

string HttpRequestGet::Connect()
{
    curl_slist_append(_headers, "Accept: application/json, text/html, */*");
    curl_slist_append(_headers, contentType.c_str());
    curl_slist_append(_headers, "charsets: utf-8");
    _curl = curl_easy_init();

    if (_curl)
    {
        curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, _headers);
        curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(_curl, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(_curl, CURLOPT_VERBOSE, debug_mode);
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, &HttpRequest::Writer);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, this);

        _res = curl_easy_perform(_curl);

        if (CURLE_OK == _res)
        {
            char *ct;
            _res = curl_easy_getinfo(_curl, CURLINFO_CONTENT_TYPE, &ct);
            if ((CURLE_OK == _res) && ct)
            return text;
        }

        curl_easy_cleanup(_curl);
    }
}

void HttpRequestGet::Callback(string retText)
{
    HttpRequest::Callback(retText);
}

/// HttpRequestPost
//////////////////////////////////////////////////////////////////

HttpRequestPost::HttpRequestPost(string pUrl):HttpRequest(pUrl)
{

}

string HttpRequestPost::Connect()
{
    string params = _ArrangeParams();

    curl_slist_append(_headers, "Accept: application/json, text/html, */*");
    curl_slist_append(_headers, contentType.c_str());
    curl_slist_append(_headers, "charsets: utf-8");
    _curl = curl_easy_init();

    if (_curl)
    {
        curl_easy_setopt(_curl, CURLOPT_HTTPHEADER, _headers);
        curl_easy_setopt(_curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(_curl, CURLOPT_POST, 1);
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDS, params.c_str());
        curl_easy_setopt(_curl, CURLOPT_POSTFIELDSIZE, params.length());
        curl_easy_setopt(_curl, CURLOPT_VERBOSE, debug_mode);
        curl_easy_setopt(_curl, CURLOPT_WRITEFUNCTION, &HttpRequest::Writer);
        curl_easy_setopt(_curl, CURLOPT_WRITEDATA, this);

        _res = curl_easy_perform(_curl);

        if (CURLE_OK == _res)
        {
            char *ct;
            _res = curl_easy_getinfo(_curl, CURLINFO_CONTENT_TYPE, &ct);
            if ((CURLE_OK == _res) && ct)
            return text;
        }

        curl_easy_cleanup(_curl);
    }
}
