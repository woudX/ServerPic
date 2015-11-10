#include "filesys.h"
#include "convert.h"

FileSys::FileSys()
{

}

void FileSys::LevelOff(string srcDirectory, FileOverride overrideFlag, string extRegex, string objDirectory, int depth)
{
    DIR *pDir;
    struct dirent *ent;
    struct stat st;

    //  init objDirectory
    string objDir = (objDirectory.empty()) ? srcDirectory : objDirectory;

    //  open now dir
    pDir = opendir(srcDirectory.c_str());

    while ((ent = readdir(pDir)) != NULL)
    {
        if (!IsSpecialDir(ent->d_name))
        {
            //  create full path of file
            string fullPath = srcDirectory;
            fullPath.append(ent->d_name);

            //  get file stat

            if (IsDir(fullPath))
            {
                fullPath.append("/");
                LevelOff(fullPath, overrideFlag, extRegex, objDir, depth + 1);
            }
            else if (IsFile(fullPath))
            {
                //  check if need regex
                if (extRegex.empty())
                {
                    MoveFileResult result = MoveFile(FormatDir(srcDirectory), FormatDir(objDir),
                             string(ent->d_name), overrideFlag);

                    if (result == MoveFileResult::FileHasExist)
                    {
                        string filepath = FormatDir(srcDirectory) + string(ent->d_name);
                        DeleteFile(filepath);
                        debug_log("delete file : %s\n", filepath.c_str());
                    }
                }
                else
                {
                    //  regex pic
                    bool regSucc = RegexFileExt(string(ent->d_name), extRegex);
                    MoveFileResult result = MoveFileResult::SameDirectory;

                    if (regSucc)
                        result = MoveFile(FormatDir(srcDirectory), FormatDir(objDir),
                                 string(ent->d_name), overrideFlag);

                    //  Delete source file
                    if (!regSucc || result == MoveFileResult::FileHasExist)
                    {
                        string filepath = FormatDir(srcDirectory) + string(ent->d_name);
                        DeleteFile(filepath);
                        debug_log("delete file : %s\n", filepath.c_str());
                    }
                }
            }
        }
    }

    //  Delete if this file is folder
    if (IsDir(srcDirectory))
        DeleteFile(srcDirectory);

    closedir(pDir);
}


bool FileSys::IsFileExist(string filepath)
{
    return access(filepath.c_str(), F_OK) == 0;
}

bool FileSys::IsFileExist(string filepath, string filename)
{
    string fullPath = FormatDir(filepath) + filename;
    return IsFileExist(fullPath);
}

list<string>* FileSys::GetAllFilename(string directory)
{
    DIR *pDir;
    struct dirent *ent;
    struct stat st;

    string fmtDir = FormatDir(directory);
    list<string>* pathList = new list<string>();
    pDir = opendir(fmtDir.c_str());

    while ((ent = readdir(pDir)) != NULL)
    {
        if (!IsSpecialDir(ent->d_name))
        {
            string filename(ent->d_name);
            pathList->push_back(filename);
        }
    }

    closedir(pDir);
    return pathList;
}

string FileSys::CreateUniFilename(string path, string filename)
{
    string originFn = FormatDir(path) + filename;
    string finalFn = originFn;

    int extra = 0;
    bool isOver = false;

    do
    {
        if (!IsFileExist(finalFn.c_str()))
            isOver = true;
        else
        {
            ++extra;
            finalFn = GetFileFullPathWithoutExtension(originFn) + "_"
                    + Convert::ToString(extra) + GetFileExtension(originFn);
        }
    }
    while (!isOver);

    return finalFn;
}

int FileSys::CreateDirectory(string path, __mode_t mode)
{
    string fmtPath = FormatDir(path);
    string lastCreateDir;
    int createSucc = 0;


    for (int idx = 0; idx < fmtPath.length(); ++idx)
    {
        //  need to create subDir
        if (fmtPath[idx] == '/' || fmtPath[idx] == '\\')
        {
            lastCreateDir = fmtPath.substr(0, idx + 1);
            if (!IsFileExist(lastCreateDir))
                createSucc = (mkdir(FormatDir(lastCreateDir).c_str(), mode));
        }
    }

    //  when failed, recover
    if (createSucc != 0 && !lastCreateDir.empty())
    {
        int firstSubDir = min(lastCreateDir.find('/'), lastCreateDir.find('\\'));
        DeleteFile(lastCreateDir.substr(0, firstSubDir + 1));
    }

    return createSucc;
}

string FileSys::GetFileFullPathWithoutExtension(string filepath)
{
    return filepath.substr(0, filepath.rfind('.'));
}

string FileSys::GetFileExtension(string filename)
{
    return filename.substr(filename.rfind('.'));
}

string FileSys::GetFileDirectory(string filepath)
{
    int place = filepath.rfind('/');
    if (place >= 0)
        return filepath.substr(0, place + 1);

    return "";
}

string FileSys::GetFilename(string filepath)
{
    int place = filepath.rfind('/');
    if (place >= 0)
        return filepath.substr(place + 1, filepath.length() - place - 1);

    return filepath;
}

bool FileSys::IsEmptyDir(string filepath)
{
    if (IsDir(filepath))
    {
        DIR *pDir;
        struct dirent *ent;

        pDir = opendir(filepath.c_str());
        while ((ent = readdir(pDir)) != NULL)
        {
            string fullpath(FormatDir(filepath) + ent->d_name);

            if (IsSpecialDir(fullpath))
                continue;

            if (IsDir(filepath) || IsFile(filepath))
                return false;
        }

        return true;
    }
    return false;
}

void FileSys::DeleteFile(string filepath)
{
    if (IsFileExist(filepath))
        remove(filepath.c_str());
}

bool FileSys::IsFile(string filepath)
{
    struct stat statBuf;
    if (lstat(filepath.c_str(), &statBuf) == 0)
    {
        return S_ISREG(statBuf.st_mode) != 0;
    }

    return false;
}

bool FileSys::IsDir(string filepath)
{
    struct stat statBuf;
    if (lstat(filepath.c_str(), &statBuf) == 0)
    {
        return S_ISDIR(statBuf.st_mode) != 0;
    }

    return false;
}

MoveFileResult FileSys::MoveFile(string srcPath, string objPath, string filename, FileOverride overrideFlag)
{
    MoveFile(srcPath + filename, objPath + filename, overrideFlag);
}

MoveFileResult FileSys::MoveFile(string srcFilename, string objFilename, FileOverride overrideFlag)
{
    bool isExist = IsFileExist(srcFilename);
    string finalObjFilename = objFilename;

    //  same dir return
    if (srcFilename == objFilename)
        return MoveFileResult::SameDirectory;

    //  no source file
    if (!isExist)
        return MoveFileResult::NoSourceFile;

    //  if file exist && SaveFirst, there is no need to move file
    if (overrideFlag == FileOverride::SaveFirst && isExist)
        return MoveFileResult::FileHasExist;

    if (overrideFlag == FileOverride::RenameWithOrder)
        finalObjFilename = CreateUniFilename(GetFileDirectory(objFilename), GetFilename(objFilename));

    CreateDirectory(GetFileDirectory(objFilename));
    rename(srcFilename.c_str(), finalObjFilename.c_str());

    return MoveFileResult::OK;
}

string FileSys::FormatDir(string dir)
{
    string ret(dir);

    if (ret.rfind('/') != (ret.length() - 1))
        ret = ret.append("/");

    return ret;
}

bool FileSys::RegexFileExt(string filepath, string regexPtn)
{
    regex_t regex;

    //  compile regex
    int errCode = regcomp(&regex, regexPtn.c_str(), REG_EXTENDED | REG_NOSUB);
    char errbuf[128];
    if (errCode != 0)
    {
        regerror(errCode, &regex, errbuf, sizeof(errbuf));
        err_log("error : %s", errbuf);
        return false;
    }

    int eflags = 0;
    errCode = regexec(&regex, filepath.c_str(), 0, NULL, eflags);
    regfree(&regex);

    return errCode == 0;
}

bool FileSys::IsSpecialDir(string filepath)
{
    return filepath == "." || filepath == "..";
}
