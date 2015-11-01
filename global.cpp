#include "global.h"
#include "jsonx.h"

Vector2d::Vector2d():x(0),y(0)
{

}

Vector2d::Vector2d(int xval, int yval):x(xval),y(yval)
{

}

/// AppSetting
////////////////////////////////////////////////////
AppSetting *AppSetting::_instance = NULL;

AppSetting *AppSetting::Instance()
{
    if (is_null(_instance))
        _instance = new AppSetting();
    return _instance;
}

AppSetting::AppSetting()
{

}

void AppSetting::Load(string filename)
{
    JsonObject *jsonObj = new JsonObject();
    jsonObj->LoadFromFile(filename);

    //  Load zip_config
    JsonObject *zipConfig = jsonObj->GetObjectVal<JsonObject>("zip_config");
    ext_cmd = zipConfig->GetStringVal("extract_command");
    ext_dir = zipConfig->GetStringVal("extract_directory");
    zip_filter_regex = zipConfig->GetStringVal("zip_filter_regex");
    safe_del(zipConfig);

    //  Load main_config
    JsonObject *mainConfig = jsonObj->GetObjectVal<JsonObject>("main_config");
    file_save_dir = mainConfig->GetStringVal("file_save_directory");
    file_override = mainConfig->GetIntVal("file_override");

    pic_size = new map<string, Vector2d*>();
    JsonArray *picJsonArr = mainConfig->GetObjectVal<JsonArray>("pic_size");
    for (int idx = 0; idx < picJsonArr->Length(); ++idx)
    {
        JsonObject *item = picJsonArr->At(idx);
        string picSizename = item->GetStringVal("type");

        Vector2d *size = new Vector2d(item->GetIntVal("width"), item->GetIntVal("height"));
        pic_size->insert(pair<string, Vector2d*>(picSizename, size));

        safe_del(item);
    }
    safe_del(picJsonArr);
    safe_del(mainConfig);

    safe_del(jsonObj);
}

AppSetting::~AppSetting()
{
    if (not_null(pic_size))
    {
        for (auto itor = pic_size->begin(); itor != pic_size->end();)
        {
            safe_del(itor->second);
            itor = pic_size->erase(itor);
        }
        pic_size->clear();
        safe_del(pic_size);
    }
}

