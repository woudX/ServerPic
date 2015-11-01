#include "createproc.h"
#include "global.h"
#include "function.h"
#include "filesys.h"
#include "zextractor.h"
#include "assist.h"

CreateProc::CreateProc()
{
    _filesMapping = new map<string, string>();
}

CreateProc::CreateProc(string fullname):zipFullname(fullname)
{
    _filesMapping = new map<string, string>();
}

CreateProc::~CreateProc()
{
    safe_del(_filesMapping);
}

void CreateProc::Init()
{
    AppSetting *setting = AppSetting::Instance();

    vector<string> splits;
    FuncLib::split(zipFullname, '/', splits);

    _zipFilename = splits.at(splits.size() - 1);
    _extension = FileSys::GetFileExtension(_zipFilename);

    string nameWithoutExt = FileSys::GetFileFullPathWithoutExtension(_zipFilename);
    _extDir = FileSys::FormatDir(setting->ext_dir + nameWithoutExt);
    _leveloffDir = FileSys::FormatDir(setting->file_save_dir + nameWithoutExt + "/original");
}

void CreateProc::Run()
{
    AppSetting *setting = AppSetting::Instance();

    //  Extract zip file
    ZExtractor::Extract(zipFullname, _extDir);

    //  Level off zip_dir to file_dir
    debug_log("------------------ Level off : %s ------------------\n", _zipFilename.c_str());
    FileSys::LevelOff(_extDir, FileOverride::RenameWithOrder, setting->zip_filter_regex, _leveloffDir);

    //  Rename all files
    _RenameFiles();

    //  Resize and copy files
    for (auto itor = setting->pic_size->begin(); itor != setting->pic_size->end(); ++itor)
        _ResizeAndCopyFiles(itor->first, itor->second->x, itor->second->y);
}

void CreateProc::_RenameFiles()
{
    debug_log("------------------ Rename : %s ------------------\n", _zipFilename.c_str());

    list<string> *files = FileSys::GetAllFilename(_leveloffDir);
    for (list<string>::iterator itor = files->begin(); itor != files->end(); ++itor)
    {
        string oldName = _leveloffDir + (*itor);
        string newName = _leveloffDir + Assist::GenerateRandomFilename(*itor) + FileSys::GetFileExtension(*itor);

        rename(oldName.c_str(), newName.c_str());
        _filesMapping->insert(make_pair(oldName, newName));

        debug_log("%s --> %s\n", oldName.c_str(), newName.c_str());
    }

    files->clear();
    safe_del(files);
}

void CreateProc::_ResizeAndCopyFiles(string folderName, int width, int height)
{
    AppSetting *setting = AppSetting::Instance();

    //  create objDir
    string objDir = FileSys::FormatDir(setting->file_save_dir + FileSys::GetFileFullPathWithoutExtension(_zipFilename));
    objDir = FileSys::FormatDir(objDir + folderName);
    FileSys::CreateDirectory(objDir);

    for (auto itor = _filesMapping->begin(); itor != _filesMapping->end(); ++itor)
    {
        string filename = FileSys::GetFilename(itor->second);
        Mat inputPic = imread(itor->second, IMREAD_COLOR);
        Mat outputPic;

        //  calculate best factor
        double fx =  width / (double)(inputPic.cols);
        double fy = height / (double)(inputPic.rows);
        double factor = max(fx, fy);

        //  resize pic
        if (fx > 1.0f || fy > 1.0f)
            outputPic = inputPic.clone();
        else
            resize(inputPic, outputPic, Size(0, 0), factor, factor, INTER_LANCZOS4);

        //  output
        string outputFilename = objDir + filename;
        imwrite(outputFilename.c_str(), outputPic);
    }
}


