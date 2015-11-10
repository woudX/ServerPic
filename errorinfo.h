#ifndef ERRORINFO_H
#define ERRORINFO_H

#include "stdafx.h"

#define error_msg(x) (ErrorInfo::GetErrMsg(x).c_str())

enum Errors
{
    NO_ERROR = 0,
    FILE_NOT_EXIST,
    INVALID_FILE_EXTENSION,
    EMPTY_ZIP_FILE,
    EMPTY_CONFIG_FILE,
    INVALID_ARGV_NUMBERS
};

class ErrorInfo
{
public:
    static void Init();
    static void Add(int errval, string errMsg);
    static string GetErrMsg(int errval);
    static map<int, string> errMsgMaps;
};

#endif // ERROR_H
