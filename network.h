#ifndef NETWORK
#define NETWORK

#include "stdafx.h"

class Network
{
public:
    Network();

    int writer(char *data, size_t size, size_t nmemb, void *user_data);
    string Request(string url);
    string text;
};


#endif // NETWORK
