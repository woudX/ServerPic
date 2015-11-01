#ifndef FUNCTION
#define FUNCTION

#include "stdafx.h"

class FuncLib
{
public:
    static vector<string> &split(const string &s, char delim, vector<string> &elems);
    static vector<string> &split(const string &s, char delim);
};

#endif // FUNCTION
