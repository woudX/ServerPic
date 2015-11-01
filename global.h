#ifndef GLOBAL
#define GLOBAL

#include "stdafx.h"

class Vector2d
{
public:
    int x;
    int y;

    Vector2d();
    Vector2d(int xval, int yval);
};

class AppSetting
{
public:
    static AppSetting *Instance();
    ~AppSetting();

    /**
     * @brief Load AppSetting
     * @param filename
     */
    void Load(string filename);

    string ext_cmd;
    string ext_dir;
    string zip_filter_regex;
    string file_save_dir;
    map<string, Vector2d*> *pic_size;
    int file_override;

protected:
    AppSetting();

private:
    static AppSetting *_instance;
};

#endif // GLOBAL

