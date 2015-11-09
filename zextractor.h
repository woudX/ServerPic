#ifndef ZEXTRACTOR
#define ZEXTRACTOR

#include "stdafx.h"

class ZExtractor
{
public:
    static void Extract(string filename);
    static void Extract(string filename, string extDirectory, bool deleteFlag = false);
};

#endif // ZEXTRACTOR

