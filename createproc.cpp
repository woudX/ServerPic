#include "createproc.h"
#include "global.h"
#include "function.h"
#include "filesys.h"
#include "zextractor.h"
#include "assist.h"
#include "jsonx.h"
#include "opencvex.h"

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
    //FileSys::DeleteFile(setting->file_save_dir + FileSys::GetFileFullPathWithoutExtension(_zipFilename));
    FileSys::LevelOff(_extDir, FileOverride::RenameWithOrder, setting->zip_filter_regex, _leveloffDir);

    //  Rename all files
    _RenameFiles();

    //  Resize and copy files
    JsonObject *projData = new JsonObject();
    projData->Add("pid",3);

    JsonArray *pages = new JsonArray();
    projData->AddObject("pages", pages);

    debug_log("------------------ Resize : %s ------------------\n", _zipFilename.c_str());
    for (auto fileItor = _filesMapping->begin(); fileItor != _filesMapping->end(); ++fileItor)
    {
        JsonObject *singleFile = new JsonObject();

        string srcFilename = FileSys::GetFilename(fileItor->first);
        string saveFilename = FileSys::GetFilename(fileItor->second);

        singleFile->Add("filename", srcFilename);
        singleFile->Add("savename", saveFilename);
        singleFile->Add("imagedir", FileSys::GetFileDirectory(fileItor->first));

        for (auto sizeItor = setting->pic_size->begin(); sizeItor != setting->pic_size->end(); ++sizeItor)
        {
            string optDir = FileSys::FormatDir(setting->file_save_dir + FileSys::GetFileFullPathWithoutExtension(_zipFilename) + "/" + sizeItor->first);
            FileSys::CreateDirectory(optDir);
            OpencvEx::ResizePictureFile(fileItor->second, optDir + saveFilename, sizeItor->second->x, sizeItor->second->y);

            debug_log(" %s --> %s : finished!\n", (optDir + saveFilename).c_str(),  sizeItor->first.c_str());
            singleFile->Add("image" + sizeItor->first, optDir);
        }

        pages->AddObject(singleFile);
        safe_del(singleFile);
    }
    projData->SaveToFile("output.json");

    safe_del(pages);
    safe_del(projData);
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

        //  release
        inputPic.release();
        outputPic.release();
    }
}


