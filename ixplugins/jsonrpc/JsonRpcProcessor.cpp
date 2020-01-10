/**
 * @file JsonRpcProcessor.cpp
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

#include "JsonRpcProcessor.h"
#include "JsonRpcParser.h"
#include "ObjectFactory.h"
#include "EventTarget.h"
#include "Event.h"
#include "JsonObject.h"
#include <cstring>

using namespace std;

JsonRpcProcessor::JsonRpcProcessor(ixserver_plugins_object *obj, ixserver_plugins_callbacks *cb)
    : m_ixwss_plugin_obj(obj)
    , m_ixwss_cb(cb)
{
    printf("[%s:%s:%d]\n", __FILE__, __FUNCTION__, __LINE__);
}

JsonRpcProcessor::~JsonRpcProcessor()
{
    printf("[%s:%s:%d]\n", __FILE__, __FUNCTION__, __LINE__);

    vector<ObjectInstance *>::iterator it = m_obj_instances.begin();
    while( it != m_obj_instances.end() )
    {
        delete *it;
    }
    m_obj_instances.clear();

    m_ixwss_cb->set_polling_cb(m_ixwss_plugin_obj, nullptr);
}

ixserver_plugins_object* JsonRpcProcessor::getPluginObj()
{  
    return m_ixwss_plugin_obj;
}

int JsonRpcProcessor::process(unsigned char *buffer, unsigned int length)
{
    printf("[%s:%s:%d] buffer : %s \n", __FILE__, __FUNCTION__, __LINE__, buffer);

    JsonRpcParser *parser = nullptr;
    parser = new JsonRpcParser();

    if ( !parser || !parser->import(buffer) )
    {
        return -1;
    }

    string _class = parser->getClass();
    string _function = parser->getFunction();
    JsonObject _param = parser->getParam();
    JsonObject _id = parser->getId();

    ObjectInstance *_target = nullptr;
    JsonObject result;

    if ( ObjectFactory::createObject(_class.c_str(), &_target) && _target )
    {
        _target->doAction(_function.c_str(), _param, &result);

        // reply
        __reply(_id, result);
    }

    // save objectInstance to the queue
    __add_obj(_target);

    delete parser;

    return 0;    
}

int JsonRpcProcessor::pollingCallback()
{   
    vector<ObjectInstance *>::iterator it = m_obj_instances.begin();
    while( it != m_obj_instances.end() )
    {
        if ( dynamic_cast<EventTarget *>(*it) == nullptr ) continue;
        
        EventTarget *target = dynamic_cast<EventTarget *>(*it);

        if ( target->getEvent() == nullptr ) continue;
        
        printf("[%s:%s:%d] \n", __FILE__, __FUNCTION__, __LINE__);

        __sendEvent(target->getEvent()->getName(), target->getEvent()->getParam());

        target->deleteEvent(target->getEvent());

        ++it;
    }

    return 0;
}

void JsonRpcProcessor::__add_obj(ObjectInstance *obj)
{
    vector<ObjectInstance *>::iterator it = m_obj_instances.begin();
    
    while( it != m_obj_instances.end() )
    {
        if ( obj == (*it) )
        {
            return;
        }
        ++it;
    }

    m_obj_instances.push_back(obj); 
    return;
}

void JsonRpcProcessor::__reply(JsonObject _id, JsonObject _result)
{
    JsonObject response;
    JsonObject version_value = JsonObject::valueOf("2.0");
    response.put("jsonrpc",version_value);
    JsonObject result_value = _result.get("result");
    if( !strcmp(result_value.dump().c_str(), "null") )
    {
        JsonObject error_value = _result.get("error");
        response.put("error",error_value);
    }
    else
    {
        response.put("result",result_value);
    }
    response.put("id",_id);

    string str = response.dump();

    m_ixwss_cb->send_buffer(m_ixwss_plugin_obj, (unsigned char *)str.c_str(), str.length());
}

void JsonRpcProcessor::__sendEvent(string _event_method, JsonObject _event_param)
{
    JsonObject event;
    JsonObject jsonRpcVer_obj=JsonObject::valueOf("2.0");
    event.put("jsonrpc",jsonRpcVer_obj);
    JsonObject method_obj=JsonObject::valueOf(_event_method.c_str());
    event.put("method",method_obj);
    event.put("params",_event_param);

    string e = event.dump();
    
    m_ixwss_cb->send_event(m_ixwss_plugin_obj, (unsigned char *)e.c_str(), e.length());
}