#ifndef CREATEPROC
#define CREATEPROC

#include "stdafx.h"

class CreateProc
{
public:
    CreateProc();
    CreateProc(string fullname);
    ~CreateProc();

    void Init();
    void Run();

    /**
     * @brief file which will be process
     */
    string zipFullname;

private:
    string _zipFilename;
    string _extension;
    string _extDir;
    string _leveloffDir;

    map<string, string> *_filesMapping;

    void _RenameFiles();
    void _ResizeAndCopyFiles(string folderName, int width, int height);
};

#endif // CREATEPROC
