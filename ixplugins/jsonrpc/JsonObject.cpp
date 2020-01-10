/**
 * @file JsonObject.cpp
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

#include "JsonObject.h"

#include <jansson.h>
#include <math.h>

#define DEBUG_ON        1
#define DEBUG_MEMORY_ON 0

#ifdef LOG
#undef LOG
#endif

#ifdef ASSERT
#undef ASSERT
#endif

#if (DEBUG_ON)

#define LOG(...) \
    printf(__VA_ARGS__)

#define ASSERT(condition) \
    if (!(condition)) { \
        LOG("ASSERTION FAIL! (%s) %s#%d\n", #condition, __FILE__, __LINE__); \
        CRASH(); \
    }

#define ASSERT_VALID_DATA(data) \
    assert_valid_data((json_t*)data, __FILE__, __LINE__)

#define CRASH() \
    *(int *)(unsigned int *)0xbbadbeef = 0;

static bool s_backendInitialized = false;
#define MY_DEBUG_MAGIC_DEAD    (0xbbadbeef)
#define MY_DEBUG_MAGIC_NUM_1   (0xffee11aa)
#define MY_DEBUG_MAGIC_NUM_2   (0xff11eebb)

typedef struct {
    unsigned int  magic_num;
    size_t        size;
} my_mem_debug_hdr;

static unsigned int s_debugTotalMemAllocCount = 0;
static size_t s_debugTotalMemAllocSize = 0;

static void* my_malloc(size_t size)
{
    void* ptr = malloc(sizeof(my_mem_debug_hdr) + size + sizeof(unsigned int));
    ASSERT(ptr);

    my_mem_debug_hdr* hdr = (my_mem_debug_hdr*)ptr;
    hdr->magic_num  = MY_DEBUG_MAGIC_NUM_1;
    hdr->size       = size;

    *(unsigned int*)((char*)ptr + sizeof(my_mem_debug_hdr) + size) = MY_DEBUG_MAGIC_NUM_2;

    s_debugTotalMemAllocCount++;
    s_debugTotalMemAllocSize += size;

    #if (DEBUG_MEMORY_ON)
    LOG("mem + %04u %08lu , %p %lu\n", s_debugTotalMemAllocCount, s_debugTotalMemAllocSize, (char*)ptr + sizeof(my_mem_debug_hdr), size);
    #endif

    return ((char*)ptr + sizeof(my_mem_debug_hdr));
}

static void my_free(void* ptr)
{
    ASSERT(ptr);

    my_mem_debug_hdr* hdr = (my_mem_debug_hdr*)((char*)ptr - sizeof(my_mem_debug_hdr));
    ASSERT(hdr->magic_num == MY_DEBUG_MAGIC_NUM_1);
    ASSERT(*(unsigned int*)((char*)ptr + hdr->size) == MY_DEBUG_MAGIC_NUM_2);

    hdr->magic_num = MY_DEBUG_MAGIC_DEAD;

    s_debugTotalMemAllocCount--;
    s_debugTotalMemAllocSize -= hdr->size;

    free((char*)ptr - sizeof(my_mem_debug_hdr));

    #if (DEBUG_MEMORY_ON)
    LOG("mem - %04u %08lu , %p %lu\n", s_debugTotalMemAllocCount, s_debugTotalMemAllocSize, ptr, hdr->size);
    #endif
}

static void assert_valid_data(json_t* data, const char* file, unsigned int line)
{
    if (data == NULL) {
        LOG("ASSERTION FAIL! (null pointer) %s#%d\n", file, line); 
        CRASH();
    }

    if (data->refcount == (size_t)-1) {
        return;
    }
    
    if (data->refcount == 0) {
        LOG("ASSERTION FAIL! (refcount == 0) %s#%d\n", file, line);
        CRASH();
    }

    my_mem_debug_hdr* hdr = (my_mem_debug_hdr*)(((char*)data) - sizeof(my_mem_debug_hdr));
    if (hdr->magic_num != MY_DEBUG_MAGIC_NUM_1) {
        LOG("ASSERTION FAIL! (invalid magic num 1) %s#%d\n", file, line);
        CRASH();
    }
    
    if (*(unsigned int*)(((char*)data) + hdr->size) != MY_DEBUG_MAGIC_NUM_2) {
        LOG("ASSERTION FAIL! (invalid magic num 2) %s#%d\n", file, line);
        CRASH();
    }
}

#define INIT_BACKEND_ONCE() \
    if (!s_backendInitialized) { \
        json_set_alloc_funcs(my_malloc, my_free); \
        s_backendInitialized = true; \
    }

#else

#define LOG(format, ...) ((void)0)
#define ASSERT(condition) ((void)0)
#define ASSERT_VALID_DATA(data) ((void)0)
#define INIT_BACKEND_ONCE() ((void)0)

#define my_malloc(s)  malloc(s)
#define my_free(p)    free(p)

#endif


JsonObject JsonObject::create(JSONObjectType type)
{
    return JsonObject(type);
}

JsonObject JsonObject::create(const string& json)
{
    JsonObject obj = JsonObject(JO_NULL);
    obj.parse(json);
    return obj;
}

JsonObject JsonObject::create(const char* buffer, size_t buflen)
{
    return create(string(buffer, buflen));
}

JsonObject JsonObject::valueOf(const string& str)
{
    return JsonObject(json_string(str.c_str()));
}

JsonObject JsonObject::valueOf(const char* str)
{
    return JsonObject(json_string(str));
}

JsonObject JsonObject::valueOf(int i)
{
    return JsonObject(json_integer(i));
}

JsonObject JsonObject::valueOf(double d)
{
    return JsonObject(json_real(d));
}

JsonObject JsonObject::valueOf(bool b)
{
    return JsonObject(json_boolean(b));
}

JsonObject::JsonObject(JSONObjectType type)
    : m_data(json_null())
{
    INIT_BACKEND_ONCE();
    setToDefaultValue(type);
}

JsonObject::JsonObject(JSONObjectData data, bool isNewData)
    : m_data(json_null())
{
    INIT_BACKEND_ONCE();
    setPrivateData(data, isNewData);
}

JsonObject::JsonObject(const JsonObject& other)
    : m_data(json_null())
{
    INIT_BACKEND_ONCE();
    setPrivateData(other.getPrivateData(), false);
}

JsonObject::~JsonObject()
{
    ASSERT_VALID_DATA(m_data);
    json_decref((json_t*)m_data);
}

bool JsonObject::parse(const string& json)
{
    json_error_t res;

    json_t* data = json_loads(json.c_str(), JSON_REJECT_DUPLICATES|JSON_DISABLE_EOF_CHECK|JSON_DECODE_ANY, &res);
    if (data == NULL) {
       setPrivateData(json_null());
       return false;
    }

    setPrivateData(data);

    return true;
}

string JsonObject::dump()
{
    ASSERT_VALID_DATA(m_data);

    char* dump = json_dumps((json_t*)m_data, JSON_INDENT(4)|JSON_PRESERVE_ORDER|JSON_ENCODE_ANY);
    if (dump == NULL) {
        return string();
    }

    string str = string(dump);

    my_free(dump);

    return str;
}

JSONObjectType JsonObject::getType() const
{
    ASSERT_VALID_DATA(m_data);

    switch (json_typeof((json_t*)m_data)) {
        case JSON_NULL:     return JO_NULL;
        case JSON_OBJECT:   return JO_OBJECT;
        case JSON_ARRAY:    return JO_ARRAY;
        case JSON_STRING:   return JO_STRING;
        case JSON_INTEGER:  return JO_INTEGER;
        case JSON_REAL:     return JO_REAL;
        case JSON_TRUE:     return JO_BOOLEAN;
        case JSON_FALSE:    return JO_BOOLEAN;
    }

    return JO_UNKOWN;
}

size_t JsonObject::size() const
{
    ASSERT_VALID_DATA(m_data);

    JSONObjectType type = getType();
    if (type == JO_OBJECT) {
        return json_object_size((json_t*)m_data);
    }
    else if (type == JO_ARRAY) {
        return json_array_size((json_t*)m_data);
    }

    return 0;
}

JsonObject JsonObject::get(const char* key) const
{
    ASSERT_VALID_DATA(m_data);

    if (!json_is_object((json_t*)m_data)) {
        return JsonObject();
    }

    json_t* value = json_object_get((json_t*)m_data, key);
    if (value == NULL) {
        return JsonObject();
    }

    return JsonObject(value, false);
}

bool JsonObject::put(string key, JSONObjectData data, bool isNewData)
{
    ASSERT_VALID_DATA(m_data);

    if (!ensureValidType(JO_OBJECT)) {
        return false;
    }

    int res = json_object_set((json_t*)m_data, key.c_str(), (json_t*)data);
    if (res < 0) {
        return false;
    }

    if (isNewData) {
        json_decref((json_t*)data);
    }

    return true;
}

bool JsonObject::put(string key, string str)
{
    return put(key, json_string(str.c_str()));
}

bool JsonObject::put(string key, char* s)
{
    return put(key, json_string(s));
}

bool JsonObject::put(string key, int i)
{
    return put(key, json_integer(i));
}

bool JsonObject::put(string key, double d)
{
    return put(key, json_real(d));
}

bool JsonObject::put(string key, bool b)
{
    return put(key, json_boolean(b));
}

JsonObject JsonObject::getAt(int index) const
{
    ASSERT_VALID_DATA(m_data);

    if (index < 0 || index >= (int)size() || !json_is_array((json_t*)m_data)) {
        return JsonObject();
    }

    JSONObjectData data = json_array_get((json_t*)m_data, index);
    if (data == NULL) {
        return JsonObject();
    }

    return JsonObject(data, false);
}

bool JsonObject::setAt(int index, JsonObject obj)
{
    ASSERT_VALID_DATA(m_data);

    if (index < 0 || index >= (int)size() || !json_is_array((json_t*)m_data)) {
        return JsonObject();
    }

    int res = json_array_set((json_t*)m_data, index, (json_t*)obj.getPrivateData());
    if (res < 0) {
        return false;
    }

    return true;
}

bool JsonObject::append(JsonObject obj)
{
    ASSERT_VALID_DATA(m_data);

    if (!ensureValidType(JO_ARRAY)) {
        return false;
    }

    return json_array_append((json_t*)m_data, (json_t*)obj.getPrivateData());
}

bool JsonObject::insert(int index, JsonObject obj)
{
    if (!ensureValidType(JO_ARRAY)) {
        return false;
    }

    return json_array_insert((json_t*)m_data, index, (json_t*)obj.getPrivateData());
}

bool JsonObject::remove(int index)
{
    ASSERT_VALID_DATA(m_data);

    if (!json_is_array((json_t*)m_data)) {
        return false;
    }

    return json_array_remove((json_t*)m_data, index);
}

JSONObjectData JsonObject::setPrivateData(JSONObjectData data, bool isNewData)
{
    ASSERT_VALID_DATA(m_data);

    if (data == NULL) {
        json_decref((json_t*)m_data);
        m_data = json_null();
    } else if (isNewData) {
        json_decref((json_t*)m_data);
        m_data = data;
        ASSERT_VALID_DATA(m_data);
    } else {
        json_t* other_data = json_incref((json_t*)data);
        if (m_data) {
            json_decref((json_t*)m_data);
        }
        m_data = other_data;
        ASSERT_VALID_DATA(m_data);
    }

    return data;
}

JsonObject& JsonObject::operator=(const JsonObject& other)
{
    setPrivateData(other.getPrivateData(), false);
    return *this;
}

bool JsonObject::operator==(const JsonObject& other) const
{
    ASSERT_VALID_DATA(m_data);

    if (m_data == other.getPrivateData())
        return true;

    return json_equal((json_t*)m_data, (json_t*)other.getPrivateData());
}

JsonObject& JsonObject::operator=(string str)
{
    JSONObjectType type = getType();
    if (type == JO_NULL || type == JO_STRING) {
        setPrivateData(json_string(str.c_str()));
    }
    return *this;
}

JsonObject& JsonObject::operator=(const char* str)
{
    JSONObjectType type = getType();
    if (type == JO_NULL || type == JO_STRING) {
        setPrivateData(json_string(str));
    }
    return *this;
}

JsonObject& JsonObject::operator=(int i)
{
    JSONObjectType type = getType();
    if (type == JO_NULL || type == JO_INTEGER) {
        setPrivateData(json_integer(i));
    } else if (type == JO_REAL) {
        setPrivateData(json_real(i));
    }
    return *this;
}

JsonObject& JsonObject::operator=(double d)
{
    JSONObjectType type = getType();
    if (type == JO_NULL || type == JO_REAL) {
        setPrivateData(json_real(d));
    }
    return *this;
}

JsonObject& JsonObject::operator=(bool b)
{
    JSONObjectType type = getType();
    if (type == JO_NULL || type == JO_BOOLEAN) {
        setPrivateData(json_boolean(b));
    } else if (type == JO_INTEGER) {
        setPrivateData(json_integer(b));
    } else if (type == JO_REAL) {
        setPrivateData(json_real(b));
    }
    return *this;
}

JsonObject::operator string() const
{
    ASSERT_VALID_DATA(m_data);

    if (!json_is_string((json_t*)m_data))
        return string();

    return string(json_string_value((const json_t*)m_data));
}

JsonObject::operator int() const
{
    ASSERT_VALID_DATA(m_data);

    JSONObjectType type = getType();
    if (type == JO_INTEGER) {
        return json_integer_value((const json_t*)m_data);
    } else if (type == JO_REAL) {
        return round(json_real_value((const json_t*)m_data));
    } else if (type == JO_BOOLEAN) {
        return json_is_true((json_t*)m_data);
    }
    return 0;
}

JsonObject::operator double() const
{
    ASSERT_VALID_DATA(m_data);

    JSONObjectType type = getType();
    if (type == JO_INTEGER || type == JO_REAL) {
        return json_number_value((const json_t*)m_data);
    } else if (type == JO_BOOLEAN) {
        return json_is_true((json_t*)m_data);
    }
    return 0.0;
}

JsonObject::operator bool() const
{
    ASSERT_VALID_DATA(m_data);

    JSONObjectType type = getType();
    if (type == JO_INTEGER) {
        return (json_integer_value((const json_t*)m_data) != 0);
    } else if (type == JO_REAL) {
        return (json_real_value((const json_t*)m_data) != 0.0);
    } else if (type == JO_BOOLEAN) {
        return json_is_true((json_t*)m_data);
    }
    return false;
}

void JsonObject::setToDefaultValue(JSONObjectType type)
{
    switch (type) {
        case JO_NULL:   setPrivateData(json_null());    break;
        case JO_OBJECT: setPrivateData(json_object());  break;
        case JO_ARRAY:  setPrivateData(json_array());   break;
        case JO_STRING: setPrivateData(json_string(""));break;
        case JO_INTEGER:setPrivateData(json_integer(0));break;
        case JO_REAL:   setPrivateData(json_real(0.0)); break;
        case JO_BOOLEAN:setPrivateData(json_false());   break;
        default:        setPrivateData(json_null());    break;
    }
}

bool JsonObject::ensureValidType(JSONObjectType type)
{
    if (isNull()) {
        setToDefaultValue(type);
    }

    if (getType() != type) {
        return false;
    }

    return true;
}

