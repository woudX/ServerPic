#include "jsonx.h"

JsonBase::JsonBase():trusted(false)
{

}

JsonBase::~JsonBase()
{
    if (!trusted)
        SafeDel(_thisJson);
}

json_object *JsonBase::ThisJson()
{
    return _thisJson;
}

string JsonBase::ToString()
{
    string retVal = "";
    if (_thisJson != NULL)
        retVal = json_object_to_json_string(_thisJson);

    return retVal;
}

void JsonBase::SafeDel(json_object *jsonObj)
{
    if (jsonObj != NULL)
        json_object_put(jsonObj);

    jsonObj = NULL;
}

json_object *JsonBase::Create(string val)
{
    return json_object_new_string(val.c_str());
}

json_object *JsonBase::Create(int val)
{
    return json_object_new_int(val);
}

json_object *JsonBase::Create(bool val)
{
    return json_object_new_boolean(val);
}

json_object *JsonBase::Create(double val)
{
    return json_object_new_double(val);
}

json_object *JsonBase::Create(const char *pval)
{
    return json_object_new_string(pval);
}

/// JsonObject
///////////////////////////////////////////////////////////////////

JsonObject::JsonObject():JsonBase()
{
    _thisJson = json_object_new_object();
}

JsonObject::JsonObject(json_object *pJsonObj)
{
    _thisJson = pJsonObj;
}

JsonObject::~JsonObject()
{

}

bool JsonObject::LoadFromFile(string filepath)
{
    SafeDel(_thisJson);
    _thisJson = json_object_from_file(filepath.c_str());

    return _thisJson != NULL;
}

bool JsonObject::LoadFromText(string jsonText)
{
    SafeDel(_thisJson);
    _thisJson = json_tokener_parse(jsonText.c_str());

    return _thisJson != NULL;
}

void JsonObject::SaveToFile(string filepath)
{
    json_object_to_file(filepath.c_str(), _thisJson);
}

void JsonObject::Remove(string key)
{
    if (not_null(_thisJson))
        json_object_object_del(_thisJson, key.c_str());
}

string JsonObject::GetStringVal(string keyname)
{
    json_object *jsonObj = json_object_object_get(_thisJson, keyname.c_str());
    return (string) json_object_get_string(jsonObj);
}

double JsonObject::GetDoubleVal(string keyname)
{
    json_object *jsonObj = json_object_object_get(_thisJson, keyname.c_str());
    return json_object_get_double(jsonObj);
}

int JsonObject::GetIntVal(string keyname)
{
    json_object *jsonObj = json_object_object_get(_thisJson, keyname.c_str());
    return json_object_get_int(jsonObj);
}

bool JsonObject::GetBoolVal(string keyname)
{
    json_object *jsonObj = json_object_object_get(_thisJson, keyname.c_str());
    return json_object_get_boolean(jsonObj);
}

/// JsonArray
///////////////////////////////////////////////////////////////////

JsonArray::JsonArray():JsonBase()
{
    _thisJson = json_object_new_array();
}

JsonArray::JsonArray(json_object *pval)
{
    _thisJson = pval;
}

JsonArray::~JsonArray()
{

}

json_object *JsonArray::ThisJson()
{
    return _thisJson;
}

int JsonArray::Refresh(int idx, JsonObject *jsonObj)
{
    return json_object_array_put_idx(_thisJson, idx, jsonObj->ThisJson());
}

JsonObject *JsonArray::At(int idx)
{
    return new JsonObject(json_object_array_get_idx(_thisJson, idx));
}

int JsonArray::Length()
{
    return json_object_array_length(_thisJson);
}

