#include "convert.h"

string Convert::ToString(int value)
{
    char *szBuf = new char[sizeof(int) + 1];
    memset(szBuf, 0, sizeof(int) + 1);
    sprintf(szBuf, "%d", value);

    string ret(szBuf);
    safe_del(szBuf);

    return ret;
}

string Convert::ToString(long value)
{
    char *szBuf = new char[sizeof(long) + 1];
    memset(szBuf, 0, sizeof(long) + 1);
    sprintf(szBuf, "%ld", value);

    string ret(szBuf);
    safe_del(szBuf);

    return ret;
}

