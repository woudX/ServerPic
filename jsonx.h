#ifndef JSONX_H
#define JSONX_H

#include "stdafx.h"
#include "convert.h"

/**
 * @brief The JsonBase class
 */
class JsonBase
{
public:
    //  if trusted, this class's json_object will be deleted by other class
    bool trusted;

    /*
     *  Summary : get json_object
     */
    virtual json_object *ThisJson();

    /*
     *  Summary : convert json to string
     */
    virtual string ToString();

    /*
     *  Summary : release json_object safely
     */
    static void SafeDel(json_object *jsonObj);

    /*
     *  Summary : create methods
     */
    static json_object *Create(string val);
    static json_object *Create(int val);
    static json_object *Create(bool val);
    static json_object *Create(double val);
    static json_object *Create(const char *pval);

protected:
    JsonBase();
    json_object *_thisJson = NULL;
    virtual ~JsonBase();
};

/*
 *  JsonObject package json_object from json-c
 */
class JsonObject : public JsonBase
{
public:
    JsonObject();
    JsonObject(json_object *pJsonObj);

    ~JsonObject();

    /*
     *  Summary : load/save json
     */
    bool LoadFromFile(string filepath);
    bool LoadFromText(string jsonText);
    void SaveToFile(string filepath);

    /*
     *  Summary : add/remove value to json_object
     */
    void Remove(string key);
    template<typename T> void AddObject(string key, T *val)
    {
        JsonBase *base = dynamic_cast<JsonBase*>(val);

        //  trust and add
        base->trusted = true;
        json_object_object_add(_thisJson, key.c_str(), base->ThisJson());
    }
    template<typename T> void Add(string key, T val)
    {
        json_object_object_add(_thisJson, key.c_str(), Create(val));
    }

    /*
     *  Summary : get value from json_object with type T
     */

    string GetStringVal(string keyname);
    double GetDoubleVal(string keyname);
    int GetIntVal(string keyname);
    bool GetBoolVal(string keyname);

    /*
    template<typename T> T GetValue(string keyname)
    {
        json_object *valJson = NULL;
        enum json_type jsonType;
        T val;

        valJson = json_object_object_get(_thisJson, keyname.c_str());

        if (valJson != NULL)
        {
            jsonType = json_object_get_type(valJson);

            switch (jsonType)
            {
            case json_type_string:
                val = string(json_object_get_string(valJson));
                break;
            case json_type_int:
                val = json_object_get_int(valJson);
                break;
            case json_type_double:
                val = json_object_get_double(valJson);
                break;
            case json_type_boolean:
                val = json_object_get_boolean(valJson);
                break;
            default:
                break;
            }
        }
        return val;
    };*/

    template<typename T> T* GetObjectVal(string keyname)
    {
        json_object *valJson = json_object_object_get(_thisJson, keyname.c_str());
        return new T(valJson);
    }
};

/*
 *  JsonArray package the json_object(array_list) from json-c
 */
class JsonArray : public JsonBase
{
public:
    JsonArray();
    JsonArray(json_object *pval);
    ~JsonArray();

    json_object *ThisJson();
    JsonObject *At(int idx);

    template<typename T> void AddObject(T *val)
    {
        JsonBase *base = dynamic_cast<JsonBase*>(val);

        //  trust and add
        base->trusted = true;
        json_object_array_add(_thisJson, base->ThisJson());
    }

    template<typename T> void Add(T val)
    {
        json_object_array_add(_thisJson, Create(val));
    }

    int Refresh(int idx, JsonObject *jsonObj);
    int Length();
};

#endif // JSONX_H
