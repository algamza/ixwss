/**
 * @file IXWebSocketServerPrivate.cpp
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief Server based on WebSocket
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

#include "IXWebSocketServerPrivate.h"
#include "IXWebSocketData.h"
#include "IXWebSocketPluginSetting.h"
#include "IXWebSocketSetting.h"
#include "IXLog.h"
#include <thread>
#include <fstream>
#include <cstring>
#include <libwebsockets.h>

using namespace std;

namespace ixwss {

static int callback(struct lws *wsi, enum lws_callback_reasons reason, 
    void *user, void *in, size_t len)
{
    switch ( reason )
    {
    case LWS_CALLBACK_ESTABLISHED:
        IXWebSocketServerPrivate::onCreateSession(static_cast<LWSSessionHandle >(wsi)); break;
    case LWS_CALLBACK_WSI_DESTROY:
        IXWebSocketServerPrivate::onDestroySession(static_cast<LWSSessionHandle >(wsi)); break;
    case LWS_CALLBACK_RECEIVE:
        IXWebSocketServerPrivate::onReceivedData(static_cast<LWSSessionHandle >(wsi), in, len); break;
    case LWS_CALLBACK_SERVER_WRITEABLE:
        IXWebSocketServerPrivate::onWriteAble(static_cast<LWSSessionHandle >(wsi)); break;
    default: break;
    }

    return 0;
}

LWSContextHandle IXWebSocketServerPrivate::s_context = nullptr;
STDThreadHandle IXWebSocketServerPrivate::s_thread = nullptr;
bool IXWebSocketServerPrivate::s_try_thread_exit = false;
list<IXWebSocketVhost *> IXWebSocketServerPrivate::s_vhosts;

void IXWebSocketServerPrivate::onCreateSession(LWSSessionHandle handle)
{
    IXWebSocketVhost *vhost = static_cast<IXWebSocketVhost *>(__getVhost(handle));
    if ( vhost ) vhost->createSession(handle, lws_get_protocol(static_cast<lws*>(handle))[0].name);
}

void IXWebSocketServerPrivate::onDestroySession(LWSSessionHandle handle)
{
    IXWebSocketVhost *vhost = static_cast<IXWebSocketVhost *>(__getVhost(handle));
    if ( vhost ) vhost->destroySession(handle);
}

void IXWebSocketServerPrivate::onReceivedData(LWSSessionHandle handle, void *data, int len)
{
    IXWebSocketVhost *vhost = static_cast<IXWebSocketVhost *>(__getVhost(handle));
    if ( vhost ) vhost->receivedData(handle, data, len);
}

void IXWebSocketServerPrivate::onWriteAble(LWSSessionHandle handle)
{
    IXWebSocketVhost *vhost = static_cast<IXWebSocketVhost *>(__getVhost(handle));
    if ( vhost ) vhost->writeAble(handle);
}

IXWebSocketServerPrivate::IXWebSocketServerPrivate(const char *path)
    : m_setting(nullptr), m_plugin_setting(nullptr)
{
    m_plugin_setting = new IXWebSocketPluginSetting();
    m_setting = new IXWebSocketSetting();

    m_setting->setServerConfigPath(path);
    m_plugin_setting->setPluginConfigPath(path);
}

IXWebSocketServerPrivate::~IXWebSocketServerPrivate()
{
    if( m_plugin_setting ) delete m_plugin_setting;
    m_plugin_setting = nullptr;
    if( m_setting ) delete m_setting;
    m_setting = nullptr;
    __destroy_vhosts();
    __destroy_thread();
    __destroy_context();
}

bool IXWebSocketServerPrivate::start()
{
    if ( !__create_context() ) 
        INFO("The context has already started. !!");
    if ( !__create_vhosts() )
    {
        INFO("The vhosts has already started. !!");
        return false;
    } 

    INFO(" Start Server !! ");
    return true;
}

bool IXWebSocketServerPrivate::stop()
{   
    __destroy_vhosts(); 
    __destroy_thread();
    __destroy_context();
    return true;
}

LXSVhostHandle IXWebSocketServerPrivate::__getVhost(LWSSessionHandle handle)
{
#if LWS_LIBRARY_VERSION_NUMBER > 2000000
    lws_vhost *vhost = lws_vhost_get(static_cast<lws *>(handle));
#else
    lws_vhost *vhost = lws_get_vhost(static_cast<lws *>(handle));
#endif
    if ( !vhost ) return nullptr;

    for ( auto _vhost : s_vhosts )
    {
        if ( static_cast<LWSVhostHandle>(vhost) == _vhost->getHandle() )
            return static_cast<LXSVhostHandle>(_vhost);
    }
    return nullptr;
}

void IXWebSocketServerPrivate::_thread()
{
    while(1)
    {
        if ( s_try_thread_exit ) 
        {
            s_try_thread_exit = false;
            break;
        }

        if ( s_context ) lws_service(static_cast<lws_context *>(s_context), 50);
    
        IXThread::sleep(50);
    }
}

void IXWebSocketServerPrivate::__destroy_thread()
{
    if ( !s_thread ) return;
    s_try_thread_exit = true;
    static_cast<thread *>(s_thread)->join();
    delete static_cast<thread *>(s_thread);
    s_thread = nullptr;
}

bool IXWebSocketServerPrivate::__create_context()
{
    if ( s_context ) return false;

    lws_context_creation_info m_contextinfo;
    memset(&m_contextinfo, 0, sizeof(m_contextinfo));
    m_contextinfo.options = LWS_SERVER_OPTION_EXPLICIT_VHOSTS;
    s_context = static_cast<LWSContextHandle>(lws_create_context(&m_contextinfo));
    if ( !s_thread ) s_thread = static_cast<STDThreadHandle>(new thread( [this] { _thread(); } ));
    return true;
}

bool IXWebSocketServerPrivate::__destroy_context()
{
    if ( !s_context ) return false;
    lws_context *_server = static_cast<lws_context *>(s_context);
    s_context = nullptr;
    lws_context_destroy(_server);

    return true;
}

bool IXWebSocketServerPrivate::__create_vhosts()
{
    if ( !s_context ) return false;
    if ( !m_setting ) return false;

    for ( auto vhost : s_vhosts )
    {
        list<int> ports;
        m_setting->getPorts(ports);
        for ( auto port : ports )
        {
            if ( port == vhost->getPort() ) 
                return false;
        }
    }

    list<int> ports; 
    m_setting->getPorts(ports);
    for ( auto port : ports )
    {
        list<string> protocols;
        m_setting->getProtocols(port, protocols);


        LXSProtocolHandle handle = __create_protocol(protocols);
        if ( !handle ) continue;
        
        __set_plugin_protocol_to_match(port, handle);
        protocols_t *data = static_cast<protocols_t *>(handle);
        lws_context_creation_info vhostinfo;
        memset(&vhostinfo, 0, sizeof(vhostinfo));
        
        vhostinfo.port = port;
        vhostinfo.protocols = &data->protocols[0]; 

        lws_vhost *vhost = lws_create_vhost(static_cast<lws_context *>(s_context), &vhostinfo);
        IXWebSocketVhost *ixvhost = new IXWebSocketVhost(static_cast<LWSVhostHandle>(vhost), handle);

        if ( vhost )
        {
            ixvhost->setPort(port);
            s_vhosts.push_back(ixvhost); 
        }
            
    }
    return true;
}

bool IXWebSocketServerPrivate::__destroy_vhosts()
{
    if ( !s_vhosts.size() ) return false;
    for ( auto it = s_vhosts.begin(); it != s_vhosts.end(); ++it ) 
    {
        delete (*it);
        (*it) = nullptr;
    }
    s_vhosts.clear();
    return true;
}

LXSProtocolHandle IXWebSocketServerPrivate::__create_protocol(list<string> &protocols)
{
    protocols_t *data = new protocols_t();
    if ( !data ) return nullptr;

    if ( protocols.size() >= data->MAX_PROTOCOL_COUNT - 1 )
    {
        delete data;
        return nullptr;
    }

    data->protocol_count = 0;

    for ( unsigned int i = 0; i<data->MAX_PROTOCOL_COUNT; i++ )
    {
        data->protocols[i].name = nullptr;
        data->protocols[i].per_session_data_size = 0;
        data->protocols[i].rx_buffer_size = 0;
        data->protocols[i].id = 0;
        data->protocols[i].user = nullptr;
        data->protocols[i].callback = nullptr;
    }

    data->protocols[0].name = "default";
    data->protocols[0].per_session_data_size = 0;
    data->protocols[0].rx_buffer_size = 0;
    data->protocols[0].id = 0;
    data->protocols[0].user = nullptr;
    data->protocols[0].callback = callback;
    data->protocol_count++; 

    for ( auto protocol : protocols )
    {
        data->protocols[data->protocol_count].name = protocol.c_str();
        data->protocols[data->protocol_count].per_session_data_size = 0;
        data->protocols[data->protocol_count].rx_buffer_size = 0;
        data->protocols[data->protocol_count].id = 0;
        data->protocols[data->protocol_count].user = nullptr;
        data->protocols[data->protocol_count++].callback = callback;
    }

    data->protocol_count++;

    return static_cast<LXSProtocolHandle>(data); 
}

bool IXWebSocketServerPrivate::__set_plugin_protocol_to_match(int port, LXSProtocolHandle handle)
{
    if ( !handle ) return false;
    protocols_t *data = static_cast<protocols_t *>(handle);
    if ( !m_plugin_setting ) return false;
   for ( unsigned int i = 0; i < data->protocol_count -1 ; i++ )
    {
        const char *plugin = m_plugin_setting->getPluginFilePath(port, data->protocols[i].name); 
        if ( plugin ) data->plugin_files[i].assign(plugin);
        else data->plugin_files[i].assign("default.so");
    }
    return true;
}

}  /* namespace ixwss */
