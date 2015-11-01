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
