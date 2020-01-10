/**
 * @file IxWebsocketVhost.cpp
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

#include "IxWebSocketVhost.h"
#include "IxConnectionInfo.h"
#include "IxSession.h"

#include <libwebsockets.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

namespace ixs {

IxWebSocketVhost::IxWebSocketVhost(void *context_handle, int port, IxConnectionInfo *info)
    : m_context_handle(context_handle)
    , m_port(port)
    , m_connection(info)
    , m_handle(nullptr)
    , m_protocols_handle(nullptr)
{
}

IxWebSocketVhost::~IxWebSocketVhost()
{
    for ( auto session : m_sessions ) 
    {
        delete static_cast<IxSession *>(session);
        session = nullptr;
    }
    m_sessions.clear();

    delete[] static_cast<lws_protocols *>(m_protocols_handle);
    m_protocols_handle = nullptr;

    delete m_connection; 
}

bool IxWebSocketVhost::start()
{
    if ( !m_context_handle || !m_connection ) return false;

    // The end must contain a null array. 
    unsigned int arr_size = m_connection->getProtocols().size() + 1; 
    lws_protocols *protocols = new lws_protocols[arr_size];
    if ( !protocols ) return false;
    for ( unsigned int i = 0; i < arr_size; i++ ) 
    {
        protocols[i].name = nullptr;
        protocols[i].per_session_data_size = 0;
        protocols[i].rx_buffer_size = 0;
        protocols[i].id = 0;
        protocols[i].user = nullptr;
        protocols[i].callback = nullptr;
    }

    auto callback = [](struct lws *wsi, enum lws_callback_reasons reason, 
            void *user, void *in, size_t len) -> int
    {
        const lws_protocols *p = lws_get_protocol(wsi);
        if ( !p ) return -1; 
        auto event = IxWebSocketVhost::IxWebsocketEvent::IX_EVENT_NONE;
        
        // printf("(%p) reason : %d\n", wsi, reason); 

        switch ( reason )
        {
            case LWS_CALLBACK_WSI_CREATE:
                event = IxWebSocketVhost::IxWebsocketEvent::IX_EVENT_SESSION_CREATE; break;
            case LWS_CALLBACK_WSI_DESTROY: 
                event = IxWebSocketVhost::IxWebsocketEvent::IX_EVENT_SESSION_DESTROY; break;
            case LWS_CALLBACK_RECEIVE: 
                event = IxWebSocketVhost::IxWebsocketEvent::IX_EVENT_WSS_RECEIVE_DATA; break;
            case LWS_CALLBACK_SERVER_WRITEABLE: 
                event = IxWebSocketVhost::IxWebsocketEvent::IX_EVENT_WSS_SERVER_WRITEABLE; break;
            case LWS_CALLBACK_HTTP: 
                event = IxWebSocketVhost::IxWebsocketEvent::IX_EVENT_HTTP_RECEIVE_DATA; break;
            case LWS_CALLBACK_HTTP_WRITEABLE: 
                event = IxWebSocketVhost::IxWebsocketEvent::IX_EVENT_HTTP_SERVER_WRITEABLE; break;
            default: break;
        }

        if ( event == IxWebSocketVhost::IxWebsocketEvent::IX_EVENT_NONE ) return 0; 

        IxWebSocketVhost *vhost = static_cast<IxWebSocketVhost *>(p->user);
        if ( !vhost->onVhostCallback(event, static_cast<void *>(wsi), in, len) ) 
            return -1;
        
        return 0;
    };

    int index = 0; 
    for ( auto protocol :  m_connection->getProtocols() ) 
    {
        protocols[index].name = protocol;
        protocols[index].callback = callback;
        protocols[index].per_session_data_size = sizeof(this);
        protocols[index].rx_buffer_size = 0;
        protocols[index].id = 0;
        protocols[index++].user = this;
    }

    lws_context_creation_info vhostinfo;
    memset(&vhostinfo, 0, sizeof(vhostinfo));
    vhostinfo.port = m_port; 
    vhostinfo.options = LWS_SERVER_OPTION_VALIDATE_UTF8; 
#ifdef LWS_OPENSSL_SUPPORT
    if ( m_connection->isSupportSSL() ) 
    {
        // vhostinfo.ssl_info_event_mask =| SSL_CB_ALERT;
        vhostinfo.ssl_cert_filepath = m_connection->getSSLCerticatePath(); 
        vhostinfo.ssl_private_key_filepath = m_connection->getSSLPrivateKeyPath(); 
        vhostinfo.ssl_ca_filepath = m_connection->getSSLRootCAPath(); 
        vhostinfo.options |= 
        LWS_SERVER_OPTION_ALLOW_NON_SSL_ON_SSL_PORT // crash when connection no ssl 
        | LWS_SERVER_OPTION_PEER_CERT_NOT_REQUIRED; 
        //| LWS_SERVER_OPTION_REQUIRE_VALID_OPENSSL_CLIENT_CERT  // need a client certificate
        //| LWS_SERVER_OPTION_REDIRECT_HTTP_TO_HTTPS; 
    }
#endif
    vhostinfo.protocols = protocols;

    m_handle = static_cast<IxVhostHandle>(lws_create_vhost(
        static_cast<lws_context *>(m_context_handle), &vhostinfo));
    if ( !m_handle )
    {
        delete[] static_cast<lws_protocols *>(protocols); 
        return false;
    }

    delete[] static_cast<lws_protocols *>(m_protocols_handle); 
    m_protocols_handle = static_cast<IxProtocolsHandle>(protocols);

    return true;
}

bool IxWebSocketVhost::stop()
{
    for ( auto session : m_sessions ) 
    {
        delete static_cast<IxSession *>(session);
        session = nullptr;
    }
    m_sessions.clear();

    delete[] static_cast<lws_protocols *>(m_protocols_handle);
    m_protocols_handle = nullptr;

    return true;
}

bool IxWebSocketVhost::onVhostCallback(IxWebsocketEvent event, void *handle
    , void *data, unsigned int len)
{
    struct lws *wsi = static_cast<struct lws*>(handle);
    if ( !wsi || !m_connection ) return false; 

    auto find_session = [this](struct lws *wsi) -> IxSession *
    {
        for ( auto session : m_sessions ) 
        {
            if ( !session ) continue;
            if ( session->getHandle() == static_cast<IxSessionHandle>(wsi) ) 
                return session;
        }
        return nullptr;
    };

    printf("(%p) event : %d\n", handle, event);

    switch(event)
    {
        case IxWebsocketEvent::IX_EVENT_NONE: 
            break;
        case IxWebsocketEvent::IX_EVENT_SESSION_CREATE:
            m_sessions.push_back(new IxSession(wsi)); 
            break;
        case IxWebsocketEvent::IX_EVENT_SESSION_DESTROY: 
            {   
                IxSession *session = find_session(wsi); 
                if ( !session ) return false; 
                m_sessions.remove(session); 
                delete session;
            }
            break;
        case IxWebsocketEvent::IX_EVENT_WSS_RECEIVE_DATA:
            {
                IxSession *session = find_session(wsi); 
                if ( !session ) return false;
                if ( !session->isValid() )
                {
                    const lws_protocols *p = lws_get_protocol(wsi);
                    if ( !p ) return false; 
                    IxConnectionInfo::IxProtocolType type = m_connection->getProtocolType(p->name);
                    if ( (type == IxConnectionInfo::IxProtocolType::IX_PROT_TYPE_NONE) 
                        || (type == IxConnectionInfo::IxProtocolType::IX_PROT_TYPE_HTTP) )
                        return false; 
                    session->setValid(true); 
                }
                if ( !session->isStarted() )
                {
                    const lws_protocols *p = lws_get_protocol(wsi);
                    if ( !p ) return false; 
                    session->start(IxSession::IxSessionType::IX_SESSION_TYPE_WSS
                        , m_connection->getPluginPath(p->name
                        , IxConnectionInfo::IxDataType::IX_DATA_TYPE_WSS) );
                }
                unsigned char *_data = static_cast<unsigned char*>(data); 
                if ( !session->pushRequestData(const_cast<const unsigned char*>(_data), len) ) 
                    return false;
                
            }
            break;
        case IxWebsocketEvent::IX_EVENT_WSS_SERVER_WRITEABLE: 
            {
                IxSession *session = find_session(wsi); 
                if ( !session ) return false;
                session->popResponseData();
            }
            break;
        case IxWebsocketEvent::IX_EVENT_HTTP_RECEIVE_DATA:
            {
                IxSession *session = find_session(wsi); 
                if ( !session ) return false;
                if ( !session->isValid() )
                {
                    const lws_protocols *p = lws_get_protocol(wsi);
                    if ( !p ) return false; 
                    IxConnectionInfo::IxProtocolType type = m_connection->getProtocolType(p->name);

                    if ( !((type == IxConnectionInfo::IxProtocolType::IX_PROT_TYPE_HTTP_WSS)
                        || (type == IxConnectionInfo::IxProtocolType::IX_PROT_TYPE_HTTP)) )
                        return false;
                    
                    session->setValid(true); 
                }

                if ( !session->isStarted() )
                {
                    const lws_protocols *p = lws_get_protocol(wsi);
                    if ( !p ) return false; 
                    session->start(IxSession::IxSessionType::IX_SESSION_TYPE_HTTP
                        , m_connection->getPluginPath(p->name
                        , IxConnectionInfo::IxDataType::IX_DATA_TYPE_HTTP) );
                }

                unsigned char *_buf = nullptr;
                unsigned char *_data = static_cast<unsigned char*>(data); 
                unsigned int _len = len;
                if ( _data[0] == '/' ) 
                {
                    _len = len-1; 
                    _buf = (unsigned char *)malloc(_len);
                    memset(_buf, 0x00, _len); 
                    memcpy(_buf, &_data[1], _len); 
                    _data = _buf;
                }

                session->pushRequestData(const_cast<const unsigned char*>(_data), _len); 
                if ( _buf ) free(_buf); 
            }
            break;
        case IxWebsocketEvent::IX_EVENT_HTTP_SERVER_WRITEABLE: 
            {
                IxSession *session = find_session(wsi); 
                if ( !session ) return false;
                session->popResponseData();
            }
            break;

        default: break;
    }
    return true;
}

} /* namespace ixs */
