/**
 * @file IxPluginManager.cpp
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

#include "IxPluginManager.h"
#include "IxPluginConnector.h"
#include <cstring>

using namespace std;

namespace ixs {

IxPluginManager::IxPluginData::IxPluginData(unsigned char *data, unsigned int len)
    : m_data(nullptr), m_len(len)
{
    if ( len && data )
    {
        m_data = (unsigned char *)malloc(len);
        if ( m_data ) 
        {
            memset(m_data, 0, len);
            memcpy(m_data, data, len);
        }
    }
}

IxPluginManager::IxPluginData::~IxPluginData()
{
    if ( m_data ) free(m_data); 
}
const unsigned char *IxPluginManager::IxPluginData::getData()
{
    return m_data;
}
unsigned int IxPluginManager::IxPluginData::getSize()
{
    return m_len;
}

IxPluginManager::IxPluginManager()
{
    IxThread::start(1);
}

IxPluginManager::~IxPluginManager()
{
    IxThread::stop();
    m_plugins.clear();
}

bool IxPluginManager::registSession(void *handle
    , function<bool(unsigned char *data, unsigned int len)> callback, const char *plugin_path)
{
    if ( !handle || !plugin_path ) return false;

    for ( auto plugin : m_plugins ) 
    {
        if ( handle == std::get<0>(plugin) ) return false;
        if ( strcmp(plugin_path, std::get<2>(plugin)) == 0 )
        {
            m_plugins.push_back(make_tuple(handle, callback, plugin_path, std::get<3>(plugin)));
            return true;
        }
    }
    
    auto response_callback = [this](void *session_handle, unsigned char *data, unsigned int len)
    {
        if ( !session_handle ) return; 
        for ( auto plugin : m_plugins ) 
        {
            if ( session_handle != std::get<0>(plugin) ) continue; 
            std::get<1>(plugin)(data, len); 
        }
    }; 

    auto event_callback = [this](void *connector_handle, unsigned char *data, unsigned int len)
    {
        if ( !connector_handle ) return; 
        for ( auto plugin : m_plugins )
        {
            if ( static_cast<IxPluginConnector *>(connector_handle) != std::get<3>(plugin).get() )
                continue;
            std::get<1>(plugin)(data, len); 
        }
    }; 


    IxPluginConnector *_connector = new IxPluginConnector(); 
    if ( !_connector ) return false;
    if ( !_connector->initialize(plugin_path) ) return false;
    _connector->addResponseListener(response_callback); 
    _connector->addEventListener(event_callback); 
    shared_ptr<IxPluginConnector> connector(_connector);
    m_plugins.push_back(make_tuple(handle, callback, plugin_path, connector)); 

    return true;
}

bool IxPluginManager::unRegistSession(void *handle)
{
    for ( auto it = m_plugins.begin(); it != m_plugins.end(); )
    {
        if ( handle == std::get<0>(*it) )
            it = m_plugins.erase(it);
        else ++it; 
    }

    return true;
}

IxPluginManager::IxPluginData *IxPluginManager::syncCall(void *handle
    , const unsigned char *data, unsigned int len)
{
    // I will implement it when necessary.
    return nullptr;
}

bool IxPluginManager::asyncCall(void *handle, const unsigned char *data, unsigned int len)
{
    for ( auto plugin : m_plugins ) 
    {
        if ( handle != std::get<0>(plugin) ) continue; 
        std::get<3>(plugin)->process(handle, const_cast<unsigned char*>(data), len);
        break;
    }

    return true;
}

void IxPluginManager::run()
{
    for ( auto plugin : m_plugins ) 
    {
        if ( !std::get<3>(plugin) ) continue;
        std::get<3>(plugin)->procPollingCallback(); 
    }
}

}
