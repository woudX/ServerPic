#include "errorinfo.h"

map<int, string> ErrorInfo::errMsgMaps;

void ErrorInfo::Init()
{
    Add(NO_ERROR, "no error");
    Add(FILE_NOT_EXIST, "file doesn't exist!");
    Add(INVALID_FILE_EXTENSION, "invalid file extension!");
    Add(EMPTY_ZIP_FILE, "empty zip file!");
    Add(EMPTY_CONFIG_FILE, "config file content is empty!");
    Add(INVALID_ARGV_NUMBERS, "invalid argv numbers");
}

void ErrorInfo::Add(int errval, string errMsg)
{
    errMsgMaps.insert(make_pair(errval, errMsg));
}

string ErrorInfo::GetErrMsg(int errval)
{
    auto result = errMsgMaps.find(errval);
    if (result != errMsgMaps.end())
        return result->second;

    return "";
}
