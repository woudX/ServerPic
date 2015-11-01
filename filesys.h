#ifndef FILESYS_H
#define FILESYS_H

#include "stdafx.h"

enum FileOverride
{
    SaveLast, SaveFirst, RenameWithOrder
};

enum MoveFileResult
{
    OK = 0,
    NoSourceFile = 1,
    FileHasExist = 2,
    SameDirectory = 3
};

class FileSys
{
public:
    FileSys();

    /*
     *  Summary : LevelOff a directory, all files below this directory will remove to base dir
     *  Params:
     *      srcDirectory : dir wanted to be leveled off
     *      overrideFlag : the method which will be taken taken when file name repeat
     *      extRegex : file extension regex (default is "")
     *      objDirectory : object directory (default is srcDirectory)
     *      depth : now level off depth (default is 0)
     */
    static void LevelOff(string srcDirectory, FileOverride overrideFlag,
                         string extRegex = "", string objDirectory = "", int depth = 0);

    /*
     *  Summary : check file/folder if exist
     */
    static bool IsFileExist(string filepath);
    static bool IsFileExist(string filepath, string filename);

    /*
     *  Summary : delete file/folder if exist
     */
    static void DeleteFile(string filepath);

    /*
     *  Summary : return all filename in a directory
     *  Return : list<string>*
     */
    static list<string>* GetAllFilename(string directory);

    /*
     *  Summary : create a unique name according path and wanted filename
     */
    static string CreateUniFilename(string path, string filename);

    /*
     *  Summary : create a directory (insist multiplelayer directory)
     *  Params :
     *      path : directory path
     *      mode : directory access authority (default is 777)
     */
    static int CreateDirectory(string path, __mode_t mode = S_IRWXG | S_IRWXO | S_IRWXU);

    /*
     *  Summary : remove filepath's extension
     */
    static string GetFileFullPathWithoutExtension(string filepath);

    /*
     *  Summary : get file extension
     */
    static string GetFileExtension(string filename);

    /**
     * @brief Get file directory
     * @param filename
     * @return
     */
    static string GetFileDirectory(string filepath);

    /**
     * @brief Get filename
     * @param filepath
     * @return
     */
    static string GetFilename(string filepath);

    /*
     *  Summary : move a file to another place
     *  Params :
     *      srcPath : file source path
     *      objPath : the path want to move
     *      filename : omit.
     *      overrideFlag : select the override method
     */
    static MoveFileResult MoveFile(string srcPath, string objPath,
                                   string filename,
                                   FileOverride overrideFlag = FileOverride::SaveLast);
    static MoveFileResult MoveFile(string srcFilename, string objFilename,
                                   FileOverride overrideFlag = FileOverride::SaveLast);
    /*
     *  Summary : format a dir
     */
    static string FormatDir(string dir);

    /*
     *  Summary : return if this file can regex with regex pattern
     */
    static bool RegexFileExt(string filepath, string regexPtn);


private:
    static bool _IsValidDir(const char *dir);

};

#endif // FILESYS_H
