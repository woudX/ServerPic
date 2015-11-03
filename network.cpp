#include "network.h"

Network::Network():text("")
{

}

int Network::writer(char *data, size_t size, size_t nmemb, void *user_data)
{
    if (data != NULL)
    {
        text.append(data);
        return nmemb;
    }

    return 0;
}

string Network::Request(string url)
{
    CURL *curl;
    CURLcode res;
    struct curl_slist *headers = NULL;  //  seems very important to set null
    ostringstream oss;
    curl_slist_append(headers, "Accept: application/json");
    curl_slist_append(headers, "Content-Type: application/json");
    curl_slist_append(headers, "charsets: utf-8");
    curl = curl_easy_init();

    if (curl)
    {
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);
        curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, &Network::writer);
        res = curl_easy_perform(curl);

        if (CURLE_OK == res)
        {
            char *ct;
            res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
            if ((CURLE_OK == res) && ct)
                return text;
        }

        curl_slist_free_all(headers);
    }
}
