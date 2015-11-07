#include "zextractor.h"
#include "global.h"
#include "filesys.h"

void ZExtractor::Extract(string filename)
{
    ZExtractor::Extract(filename, "");
}

void ZExtractor::Extract(string filename, string extDirectory)
{
    string extCmd = AppSetting::Instance()->ext_cmd + " " + filename;

    //  redirect unzip directory
    if (!extDirectory.empty())
        extCmd += " -d " + extDirectory;

    //  create zip directory
    FileSys::CreateDirectory(extDirectory);

    //  output debug info
    cout << "command : " << extCmd << endl;

    system(extCmd.c_str());

}
