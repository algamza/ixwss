/**
 * @file JsonObject.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief 
 * 
 * (c) 2017 Humax Co., Ltd.
 * This program is produced by Humax Co., Ltd. ("Humax") and
 * the proprietary Software of Humax and its licensors. Humax provides you, as an Authorized Licensee,
 * non-assignable, non-transferable and non-exclusive license to use this Software.
 * You acknowledge that this Software contains valuable trade secrets of Humax and by using this Software
 * you agree to the responsibility to take all reasonable efforts to protect the any information
 * you receive from Humax. You are not permitted to duplicate, modify, distribute, sell or lease and
 * reverse engineer or extract the source code of this Software unless you have Humax's written permission to do so.
 * If you have no authorized license, discontinue using this Software immediately.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND HUMAX MAKES NO PROMISES, REPRESENTATIONS OR WARRANTIES, EITHER EXPRESS,
 * IMPLIED OR STATUTORY, OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.
 * IN NO EVENT SHALL HUMAX BE LIABLE FOR LOST PROFITS, REVENUES, OR DATA, FINANCIAL LOSSES OR INDIRECT, SPECIAL,
 * CONSEQUENTIAL, EXEMPLARTY OR PUNITIVE DAMAGES WHATSOEVER RELATING TO YOUR USE OR INABILITY TO USE THE SOFTWARE.

 * This License is effective until terminated. You may terminate this License at any time by destroying all copies
 * of the Software including all documentation. This License will terminate immediately without notice from Humax
 * to you if you fail to comply with any provision of this License. Upon termination, you must destroy all copies
 * of the Software and all documentation.

 * The laws of the Republic of Korea will apply to any disputes arising out of or relating to this Copyright Notice.
 * All claims arising out of or relating to this Copyright Notice will be litigated in the Seoul Central District Court,
 * in the Republic of Korea.
*/

#ifndef __JSONOBJECT
#define __JSONOBJECT

#include <string>

using namespace std;

typedef void* JSONObjectData;

typedef enum {
    JO_NULL,
    JO_OBJECT,
    JO_ARRAY,
    JO_STRING,
    JO_INTEGER,
    JO_REAL,
    JO_BOOLEAN,
    JO_UNKOWN
} JSONObjectType;

class JsonObject {

public:
    static JsonObject create(JSONObjectType type = JO_NULL);
    static JsonObject create(const string& json);
    static JsonObject create(const char *buffer, size_t buflen);

    static JsonObject valueOf(const string& str);
    static JsonObject valueOf(const char* str);
    static JsonObject valueOf(int i);
    static JsonObject valueOf(double d);
    static JsonObject valueOf(bool b);

    JsonObject(JSONObjectType type = JO_NULL);
    JsonObject(const JsonObject&);
	~JsonObject();

    string dump();
    string toString() { return dump(); }  // deprecated! do not use!

    JSONObjectType getType() const;
    bool isNull() const { return getType() == JO_NULL; }
    bool isObject() const { return getType() == JO_OBJECT; }
    bool isArray() const { return getType() == JO_ARRAY; }
    bool isString() const { return getType() == JO_STRING; }
    bool isInteger() const { return getType() == JO_INTEGER; }
    bool isReal() const { return getType() == JO_REAL; }
    bool isNumber() const { return isInteger() || isReal(); }
    bool isBoolean() const { return getType() == JO_BOOLEAN; }
    bool isPrimitive() const { return isNumber() || isBoolean(); }

    size_t size() const;

    // for object
    JsonObject get(const char* key) const;
	JsonObject get(string key) const { return get(key.c_str()); }
	bool put(string key, JsonObject obj) { return put(key, obj.getPrivateData(), false); }
    bool put(string key, string str);
    bool put(string key, char* s);
    bool put(string key, int i);
    bool put(string key, double d);
    bool put(string key, bool b);

    // for array
    JsonObject getAt(int index) const;
    bool setAt(int index, JsonObject obj);
    bool append(JsonObject obj);
    bool insert(int index, JsonObject obj);
    bool remove(int index);

    bool operator==(const JsonObject&) const;
    bool operator==(string str) const { return (string)*this == str; }
    bool operator==(const char* str) const { return (string)*this == str; }
    bool operator==(int i) const { return !isReal() && (int)*this == i;  }
    bool operator==(double d) const { return (double)*this == d; }
    bool operator==(bool b) const { return (bool)*this == b; }
    bool operator!=(const JsonObject& obj) const { return !(*this == obj); };
    bool operator!=(string str) const { return (string)*this != str; }
    bool operator!=(const char* str) const { return (string)*this != str; }
    bool operator!=(int i) const { return isReal() || (int)*this != i; }
    bool operator!=(double d) const { return (double)*this != d; }
    bool operator!=(bool b) const { return (bool)*this != b; }

    JsonObject& operator=(const JsonObject&);
    JsonObject& operator=(string str);
    JsonObject& operator=(const char* str);
    JsonObject& operator=(int i);
    JsonObject& operator=(double d);
    JsonObject& operator=(bool b);

    operator string() const;
    operator int() const;
    operator double() const;
    operator bool() const;

protected:
    JsonObject(JSONObjectData data, bool isNewData = true);

    bool parse(const string& json);

    bool put(string key, JSONObjectData data, bool isNewData = true);

    JSONObjectData setPrivateData(JSONObjectData data, bool isNewData = true);
    JSONObjectData getPrivateData() const { return m_data; }
    operator JSONObjectData() const { return m_data; }

    void setToDefaultValue(JSONObjectType type);
    bool ensureValidType(JSONObjectType type);

private:
    JSONObjectData m_data;

};

#endif// __JSONOBJECT
