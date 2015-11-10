#include "zextractor.h"
#include "global.h"
#include "filesys.h"
#include "errorinfo.h"

void ZExtractor::Extract(string filename)
{
    ZExtractor::Extract(filename, "");
}

int ZExtractor::Extract(string filename, string extDirectory, bool deleteFlag)
{
    string extCmd = AppSetting::Instance()->ext_cmd + " " + filename;

    if (!FileSys::IsFileExist(filename))
        return FILE_NOT_EXIST;

    if (FileSys::GetFileExtension(filename) != ".zip")
        return INVALID_FILE_EXTENSION;

    //  redirect unzip directory
    if (!extDirectory.empty())
        extCmd += " -d " + extDirectory;

    //  create zip directory
    FileSys::CreateDirectory(extDirectory);

    //  extract zip
    system(extCmd.c_str());

    //  delete zip file
    if (deleteFlag)
        FileSys::DeleteFile(filename);

    return NO_ERROR;
}
