#include "createproc.h"
#include "global.h"
#include "function.h"
#include "filesys.h"
#include "zextractor.h"
#include "assist.h"
#include "jsonx.h"
#include "assist.h"
#include "opencvex.h"
#include "network.h"
CreateProc::CreateProc()
{
    _filesMapping = new map<string, string>();
}

CreateProc::CreateProc(string fullname, int pid):zipFullname(fullname),_pid(pid)
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

    _zipRandName = Assist::GenerateRandomFilename(FileSys::GetFileFullPathWithoutExtension(_zipFilename));
    _extDir = FileSys::FormatDir(setting->ext_dir + _zipRandName);
    _leveloffDir = FileSys::FormatDir(setting->file_save_dir + _zipRandName + "/original");
}

void CreateProc::Run()
{
    AppSetting *setting = AppSetting::Instance();

    ZExtractor::Extract(zipFullname, _extDir, true);
    FileSys::LevelOff(_extDir, FileOverride::RenameWithOrder, setting->zip_filter_regex, _leveloffDir);
    _RenameFiles();
    _ResizeFiles();

}

void CreateProc::_RenameFiles()
{
    list<string> *files = FileSys::GetAllFilename(_leveloffDir);
    for (list<string>::iterator itor = files->begin(); itor != files->end(); ++itor)
    {
        string oldName = _leveloffDir + (*itor);
        string newName = _leveloffDir + Assist::GenerateRandomFilename(*itor) + FileSys::GetFileExtension(*itor);

        rename(oldName.c_str(), newName.c_str());
        _filesMapping->insert(make_pair(oldName, newName));
    }

    files->clear();
    safe_del(files);
}

void CreateProc::_ResizeFiles()
{
    AppSetting *setting = AppSetting::Instance();

    JsonObject *projData = new JsonObject();
    projData->Add("pid",_pid);

    JsonArray *pages = new JsonArray();
    projData->AddObject("pages", pages);

    for (auto fileItor = _filesMapping->begin(); fileItor != _filesMapping->end(); ++fileItor)
    {
        JsonObject *singleFile = new JsonObject();

        string srcFilename = FileSys::GetFilename(fileItor->first);
        string saveFilename = FileSys::GetFilename(fileItor->second);
        string fileExtension = FileSys::GetFileExtension(fileItor->first);

        singleFile->Add("filename", srcFilename);
        singleFile->Add("imagedir", fileItor->first);

        for (auto sizeItor = setting->pic_size->begin(); sizeItor != setting->pic_size->end(); ++sizeItor)
        {
            string optDir = FileSys::FormatDir(setting->file_save_dir + _zipRandName + "/" + sizeItor->first);
            FileSys::CreateDirectory(optDir);

            //  Resize Picture, but need to rename
            saveFilename = Assist::GenerateRandomFilename(saveFilename) + fileExtension;
            OpencvEx::ResizePictureFile(fileItor->second, optDir + saveFilename, sizeItor->second->x, sizeItor->second->y);

            singleFile->Add("image" + sizeItor->first, optDir + saveFilename);
        }

        pages->AddObject(singleFile);
        safe_del(singleFile);
    }

    //  push to server
    _PushJsonToServer(projData->ToString());

    safe_del(pages);
    safe_del(projData);
}

void CreateProc::_PushJsonToServer(string value)
{
    AppSetting *setting = AppSetting::Instance();

    //  push to server
    HttpRequestPost *postJson = new HttpRequestPost(setting->push_json_url);
    postJson->contentType = HttpContentType::Json;
    postJson->AddParam(setting->push_json_url_key, value);
    postJson->Connect();

    safe_del(postJson);
}


