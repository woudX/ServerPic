#include "zextractor.h"
#include "global.h"
#include "filesys.h"

void ZExtractor::Extract(string filename)
{
    ZExtractor::Extract(filename, "");
}

void ZExtractor::Extract(string filename, string extDirectory, bool deleteFlag)
{
    string extCmd = AppSetting::Instance()->ext_cmd + " " + filename;

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


}
