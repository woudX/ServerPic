#include "function.h"

vector<string> &FuncLib::split(const string &s, char delim, vector<string> &elems)
{
    stringstream ss(s);
    string item;
    while (getline(ss, item, delim))
        elems.push_back(item);

    return elems;
}

vector<string> &FuncLib::split(const string &s, char delim)
{
    vector<string> elems;
    return split(s, delim, elems);
}

string FuncLib::filter(const string &val, char filterChar)
{
    string retStr = "";
    for(auto item : val)
    {
        if (item != filterChar)
            retStr += item;
    }

    return retStr;
}

