#include "assist.h"
#include "convert.h"

long Assist::HashKeyTimes33(string key)
{
    uint nHash = 0;
    for (int idx = 0; idx < key.length(); ++idx)
        nHash = (nHash << 5) + nHash + key[idx];

    return nHash;
}

string Assist::GenerateRandomFilename(string key)
{
    string sysTime = Convert::ToString(GetCurrentSystemTime());
    string nHash = Convert::ToString(HashKeyTimes33(key));

    return sysTime + "_" + nHash;
}

long Assist::GetCurrentSystemTime()
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}
