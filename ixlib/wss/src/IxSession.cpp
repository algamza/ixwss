/**
 * @file IxSession.cpp
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

#include "IxSession.h"
#include "IxSessionData.h"
#include "IxWebSocketSessionData.h"
#include "IxHttpSessionData.h"
#include "IxPluginManager.h"

#include <libwebsockets.h>
#include <cstring>

using namespace std;

namespace ixs {

IxSession::IxSession(IxSessionHandle handle)
    : m_handle(handle)
    , m_type(IxSessionType::IX_SESSION_TYPE_NONE)
    , m_is_started(false)
    , m_is_valid(false)
{

}

IxSession::~IxSession()
{
    IxPluginManager::getInstance()->unRegistSession(this);
    while(!m_data.empty())
    {
        auto data = m_data.front(); 
        delete data;
        m_data.pop();
    }
}

bool IxSession::isStarted()
{
    return m_is_started;
}

bool IxSession::start(IxSessionType type, const char *plugin_path)
{
    if ( !plugin_path || m_is_started
        || (IxSession::IxSessionType::IX_SESSION_TYPE_NONE == type) ) 
        return false;

    auto plugin_callback = [this](unsigned char *data, unsigned int len) -> bool
    {
        struct lws* wsi = static_cast<struct lws*>(m_handle); 
        if ( !wsi ) return false; 
        if ( m_type == IxSession::IxSessionType::IX_SESSION_TYPE_HTTP ) 
        {
            IxHttpSessionData *_data = new IxHttpSessionData(static_cast<void*>(m_handle)
                , const_cast<const unsigned char*>(data), len); 
            if ( !_data ) return false; 
            unsigned int _header_len = _data->getDataHeaderSize(); 
            const unsigned char *_header_data = _data->getDataHeader();
            if ( !_header_len || !_header_data ) return false;
            unsigned char header_buf[LWS_PRE + _header_len];
            memset(header_buf, 0x00, sizeof(header_buf)); 
            memcpy(&header_buf[LWS_PRE], _header_data, _header_len);
            lws_write(wsi, &header_buf[LWS_PRE], _header_len, LWS_WRITE_HTTP_HEADERS);
            m_data.push(_data);
            lws_callback_on_writable(wsi);
        }
        else if ( m_type == IxSession::IxSessionType::IX_SESSION_TYPE_WSS )
        {
            m_data.push(new IxWebSocketSessionData(const_cast<const unsigned char*>(data), len));
            lws_callback_on_writable(wsi);
        }
        else 
            return false;

        return true;
    };

    if ( !IxPluginManager::getInstance()->registSession(this, plugin_callback, plugin_path) )
        return false;

    m_type = type;
    m_is_started = true;

    return true;
}

IxSessionHandle IxSession::getHandle()
{
    return m_handle;
}

bool IxSession::pushRequestData(const unsigned char *data, unsigned int len)
{
    if ( !data || !len ) return false;
    
    if ( ! IxPluginManager::getInstance()->asyncCall(this, data, len) ) 
        return false;
    return true;
}

bool IxSession::popResponseData()
{
    struct lws* wsi = static_cast<struct lws*>(m_handle); 
    if ( !wsi ) return false; 

    if ( m_data.empty() ) return false;

    const IxSessionData *data = m_data.front(); 
    if ( !data ) return false;

    const unsigned char *_data = data->getData(); 
    unsigned int _len = data->getDataSize(); 
    if ( !_data || !_len ) 
    {
        delete data;
        m_data.pop();
        return false;
    }

    switch(m_type)
    {
        case IxSessionType::IX_SESSION_TYPE_NONE: break;
        case IxSessionType::IX_SESSION_TYPE_HTTP: 
            {
                unsigned char buf[LWS_PRE + _len];
                memset(buf, 0x00, sizeof(buf)); 
                memcpy(&buf[LWS_PRE], _data, _len); 
                lws_write(wsi, &buf[LWS_PRE], _len, LWS_WRITE_HTTP);
                if ( lws_http_transaction_completed(wsi) ) 
                {
                    delete data;
                    m_data.pop();
                    return false;
                }
            }
            break;
        case IxSessionType::IX_SESSION_TYPE_WSS: 
            {
                unsigned char buf[LWS_PRE + _len];
                memset(buf, 0x00, sizeof(buf));  
                memcpy(&buf[LWS_PRE], _data, _len);
                lws_write(wsi, &buf[LWS_PRE], _len, LWS_WRITE_TEXT);
            }
            break;
        default: break;
    }; 

    delete data;
    m_data.pop();
    data = nullptr;

    lws_callback_on_writable(wsi);
    
    return true;
}

bool IxSession::isValid()
{
    return m_is_valid;
}

bool IxSession::setValid(bool valid)
{
    m_is_valid = valid;
    return true;
}

} /* namespace ixs */
