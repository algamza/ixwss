/**
 * @file IXWebSocketClientPrivate.cpp
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief Private based on WebSocket
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


#include "IXWebSocketClientPrivate.h"
#include "IXWebSocketClientData.h"
#include "IXClientLog.h"
#include "IXClientThread.h"
#include <thread>
#include <fstream>
#include <cstring>

using namespace std;

namespace ixwss {

LWSContextHandle IXWebSocketClientPrivate::s_context = nullptr;
STDThreadHandle IXWebSocketClientPrivate::s_thread = nullptr;
bool IXWebSocketClientPrivate::s_try_thread_exit = false;
IXWebSocketClientPrivateListener *s_client_recv_callback = nullptr;
bool IXWebSocketClientPrivate::s_is_exist_send_data = false;

IXWebSocketClientPrivate* IXWebSocketClientPrivate::s_client = nullptr;
LWSConnectionHandle IXWebSocketClientPrivate::s_wsi = nullptr;

static int callback(struct lws *wsi, enum lws_callback_reasons reason, void *user, void *in, size_t len)
{
    switch ( reason )
    {
    case LWS_CALLBACK_CLIENT_ESTABLISHED:
        INFO("MSG - Connect with server success."); break;
    case LWS_CALLBACK_CLIENT_CONNECTION_ERROR:
        INFO("MSG - Connect with server error."); break;
    case LWS_CALLBACK_CLOSED:
        INFO("MSG - LWS_CALLBACK_CLOSED."); break;
    case LWS_CALLBACK_CLIENT_RECEIVE:
        INFO("MSG - Client recvived data.");
        IXWebSocketClientPrivate::onReceivedData(wsi, in); break;
    case LWS_CALLBACK_CLIENT_WRITEABLE:
        INFO("MSG - On writeable is called.");
        IXWebSocketClientPrivate::onWriteAble(wsi); break;  
    default: break;
    }

    return 0;
}

void IXWebSocketClientPrivate::onReceivedData(struct lws *wsi, void *in)
{
    if ( s_wsi != wsi ) return;
    if ( s_client_recv_callback ) 
    {
        string msg;
        msg.assign((char*)in);
        s_client_recv_callback->onReceiveData( (unsigned char *)msg.c_str(), msg.size() );
    }
}

void IXWebSocketClientPrivate::onWriteAble(struct lws *wsi)
{
    if ( s_wsi != wsi ) return;
    s_client->writeable(wsi);
}


IXWebSocketClientPrivate::IXWebSocketClientPrivate(const char *name, const char *address, 
    int port, const char *protocol)
    :m_host_name(name)
    ,m_address(address)
    ,m_port(port)
    ,m_protocol(protocol)
{
    s_client = this;
}

IXWebSocketClientPrivate::~IXWebSocketClientPrivate()
{
    __destroy_thread();
    __destroy_context();

    const IXWebSocketClientData *data = nullptr;
    while ( (data = __popWriteQ()) != nullptr )
        delete data;

    s_client = nullptr;
    s_wsi = nullptr;
}

bool IXWebSocketClientPrivate::connect()
{
    if ( !__create_context() )
    { 
        INFO("Client can't connect with the server.");
        return false;
    }
    return true;
}

bool IXWebSocketClientPrivate::disconnect()
{   
    __destroy_thread();
    __destroy_context();
    return true;
}

bool IXWebSocketClientPrivate::send(const char *data, int dlen)
{
    IXWebSocketClientData *send_data = new IXWebSocketClientData((unsigned char*)data, dlen); 
    if ( send_data == NULL )
        return false;

    writeData(send_data); 
    s_is_exist_send_data = true;

    return true;
}

bool IXWebSocketClientPrivate::addEventListener(IXWebSocketClientPrivateListener *_callback)
{
    if ( !_callback ) return false;

    s_client_recv_callback = _callback;

    return true;
}

void IXWebSocketClientPrivate::removeEventListener()
{
    s_client_recv_callback = NULL;
}

bool IXWebSocketClientPrivate::writeData(const IXWebSocketClientData *data)
{
    m_wq.push(data);
    return true;
}

void IXWebSocketClientPrivate::writeable(struct lws *wsi_in)
{
    const IXWebSocketClientData *data = nullptr;
    data = __popWriteQ();
    if ( !data ) return;

    int _len = data->getDataLength();
    unsigned char *_data = const_cast<unsigned char*>(data->getData());
    unsigned char buf[LWS_PRE + _len + 1];
    memset(buf, 0x00, LWS_PRE + _len + 1);
    memcpy(&buf[LWS_PRE], _data, _len);
    lws_write(wsi_in, &buf[LWS_PRE], _len, LWS_WRITE_TEXT);

    if ( data ) delete data;
}

const IXWebSocketClientData *IXWebSocketClientPrivate::__popWriteQ()
{
    if ( m_wq.empty() ) return nullptr;

    const IXWebSocketClientData *wdata = m_wq.front();
    m_wq.pop();

    if ( m_wq.empty() )
    {
        s_is_exist_send_data = false;
    } 


    return wdata;
}

void IXWebSocketClientPrivate::_thread()
{
    while(1)
    {
        if ( s_try_thread_exit ) 
        {
            s_try_thread_exit = false;
            break;
        }
        if ( s_wsi && s_is_exist_send_data )
        {
            lws_callback_on_writable(static_cast<lws *>(s_wsi));
        }

        if ( s_context ) lws_service(static_cast<lws_context *>(s_context), 50);
        IXClientThread::sleep(50);
    }
}

void IXWebSocketClientPrivate::__destroy_thread()
{
    if ( !s_thread ) return;
    s_try_thread_exit = true;
    static_cast<thread *>(s_thread)->join();
    delete static_cast<thread *>(s_thread);
    s_thread = nullptr;
}

struct session_data {
    int fd;
};

bool IXWebSocketClientPrivate::__create_context()
{
    if ( s_context ) return false;

    INFO("Try to connect server with port: " << m_port << ", protocol: " << m_protocol );
    lws *wsi = NULL;

    memset(&m_contextinfo, 0, sizeof(m_contextinfo));
    m_contextinfo.port = CONTEXT_PORT_NO_LISTEN;
    m_contextinfo.iface = NULL;
    m_contextinfo.protocols = &m_pcol;
    m_contextinfo.ssl_cert_filepath = NULL;
    m_contextinfo.ssl_private_key_filepath = NULL;
    // It is not recommended to use this API. 
    // It has been removed from the latest version of libwebsocket. 
    // If you have problems using the client in the latest version of the SDK, 
    // you need to use another method.
    // m_contextinfo.extensions = lws_get_internal_extensions(); 
    m_contextinfo.gid = -1;
    m_contextinfo.uid = -1;
    m_contextinfo.options = 0;

    memset(&m_pcol, 0, sizeof(m_pcol));
    m_pcol.name  = m_protocol.c_str();
    m_pcol.callback = &callback;
    m_pcol.per_session_data_size = sizeof(struct session_data);
    m_pcol.rx_buffer_size = 0;
    m_pcol.id = 0;
    m_pcol.user = NULL;
    
    s_context = static_cast<LWSContextHandle>(lws_create_context(&m_contextinfo));

    memset(&m_ccinfo, 0, sizeof(m_ccinfo));
    m_ccinfo.context = static_cast<lws_context *>(s_context);
    m_ccinfo.address = m_address.c_str();
    m_ccinfo.port = m_port;
    m_ccinfo.ssl_connection = 0;
    m_ccinfo.path = "/";
    m_ccinfo.host = m_host_name.c_str();
    m_ccinfo.origin = NULL;
    m_ccinfo.protocol = m_protocol.c_str();
    m_ccinfo.ietf_version_or_minus_one = -1;
    wsi = lws_client_connect_via_info(&m_ccinfo);

    if ( !wsi ) return false; 

    s_wsi = static_cast<LWSConnectionHandle>(wsi);

    if ( !s_thread ) s_thread = static_cast<STDThreadHandle>(new thread( [this] { _thread(); } ));

    return true;
}

bool IXWebSocketClientPrivate::__destroy_context()
{
    if ( !s_context ) return false;
    lws_context *_server = static_cast<lws_context *>(s_context);
    lws_context_destroy(_server);
    s_context = nullptr;
    s_wsi = nullptr;
    s_is_exist_send_data = false;
    
    return true;
}

}  /* namespace ixwss */
