/**
 * @file IxPluginConnector.cpp
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

#include "IxPluginConnector.h"
#include <dlfcn.h>

using namespace std;

namespace ixs {

IxPluginConnector::IxPluginConnector()
    : m_dl_handle(nullptr)
    , m_capability(nullptr)
    , m_plugin_handler(nullptr)
    , m_polling_callback(nullptr)
{

}

IxPluginConnector::~IxPluginConnector()
{
    if ( m_plugin_handler ) m_plugin_handler->destroy(&m_plugins_obj);
    m_plugin_handler = nullptr;
    if ( m_dl_handle ) dlclose(m_dl_handle);
    m_dl_handle = nullptr;
    pthread_mutex_destroy(&m_mutex);
}

bool IxPluginConnector::initialize(const char *file_path)
{
    if ( !file_path ) return false;
    m_plugins_obj.ixwss_adapter = this;
    m_plugins_obj.private_data = nullptr;
    m_plugins_obj.session_handle = nullptr;

    auto send_buffer 
        = [](ixserver_plugins_object *ctx, unsigned char *buffer, unsigned int length) -> int
    {
        IxPluginConnector *connector = static_cast<IxPluginConnector*>(ctx->ixwss_adapter);
        if ( !connector || !connector->m_response_callback ) return -1; 
        connector->m_response_callback(ctx->session_handle, buffer, length); 
        ctx->session_handle = nullptr;
        return 0; 
    };

    auto send_event
        = [](ixserver_plugins_object *ctx, unsigned char *buffer, unsigned int length) -> int
    {
        IxPluginConnector *connector = static_cast<IxPluginConnector*>(ctx->ixwss_adapter);
        if ( !connector || !connector->m_event_callback ) return -1; 
        connector->m_event_callback(static_cast<void *>(connector), buffer, length); 
        return 0; 
    };

    auto set_polling_cb 
        = [](ixserver_plugins_object *ctx, ixserver_plugins_polling_callback *cb) -> int 
    {
        IxPluginConnector *connector = static_cast<IxPluginConnector*>(ctx->ixwss_adapter);
        if ( !connector ) return -1;
        connector->m_polling_callback = cb;
        return 0;
    };

    m_callback.send_buffer = send_buffer;
    m_callback.send_event = send_event;
    m_callback.set_polling_cb = set_polling_cb;
    m_dl_handle = dlopen(file_path, RTLD_NOW);
    if ( !m_dl_handle ) return false;
    m_capability = reinterpret_cast<ixserver_plugin_capability_function *>(
        dlsym(m_dl_handle, "ixserver_plugin_capability"));
    if ( !m_capability )
    {
        dlclose(m_dl_handle);
        m_dl_handle = nullptr;
        return false;
    }

    if ( m_capability(&m_callback, &m_plugin_handler) != 0 
        || m_plugin_handler->initialize(&m_plugins_obj) != 0 )
    {
        if ( m_plugin_handler ) m_plugin_handler->destroy(&m_plugins_obj);
        m_plugin_handler = nullptr;
        dlclose(m_dl_handle);
        m_dl_handle = nullptr;
        return false;
    }
    pthread_mutex_init(&m_mutex, nullptr);
    return true;
}

bool IxPluginConnector::addResponseListener(std::function<void(
    void *session_handle, unsigned char *data, unsigned int len)> callback)
{
    m_response_callback = callback; 
    return true;
}

bool IxPluginConnector::addEventListener(std::function<void(
    void *connector_handle, unsigned char *data, unsigned int len)> callback)
{
    m_event_callback = callback; 
    return true;
}

bool IxPluginConnector::process(void *handle, unsigned char *data, unsigned int len)
{
    if ( !m_plugin_handler ) return false;
    m_plugins_obj.session_handle = handle;
    return m_plugin_handler->process(&m_plugins_obj, data, len);
}

bool IxPluginConnector::procPollingCallback(void)
{
    if ( !m_polling_callback ) return false;
    pthread_mutex_lock(&m_mutex);
    int ret = m_polling_callback(&m_plugins_obj);
    pthread_mutex_unlock(&m_mutex);
    return ret == 0; 
}

} /* namespace ixs */
