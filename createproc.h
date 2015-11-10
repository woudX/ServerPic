#ifndef CREATEPROC
#define CREATEPROC

#include "stdafx.h"

class CreateProc
{
public:
    CreateProc();
    CreateProc(string fullname, int pid);
    ~CreateProc();

    void Init();
    int Run();
    int PID();
    /**
     * @brief file which will be process
     */
    string zipFullname;


private:
    string _zipFilename;
    string _zipRandName;
    string _extension;
    string _extDir;
    string _leveloffDir;
    int _pid;

    map<string, string> *_filesMapping;

    void _RenameFiles();
    void _ResizeFiles();
    void _PushJsonToServer(string value);
};

#endif // CREATEPROC
