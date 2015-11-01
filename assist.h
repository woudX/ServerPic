#ifndef ASSIST
#define ASSIST

#include "stdafx.h"

class Assist
{
public:
    static long HashKeyTimes33(string key);
    static string GenerateRandomFilename(string key);
    static long GetCurrentSystemTime();
};

#endif // ASSIST

