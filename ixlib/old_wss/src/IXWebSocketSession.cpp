/**
 * @file IXWebSocketSession.cpp
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief Session to manage the client based on WebSocket
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

#include "IXWebSocketSession.h"
#include "IXWebSocketServer.h"
#include "IXWebSocketData.h"
#include "IXLog.h"
#include <stdio.h>
#include <string.h>
#include <libwebsockets.h>

using namespace std;

namespace ixwss {

/*--------------------------------------------------------------------------------
 * class IXWebSocketSession
 *-------------------------------------------------------------------------------*/

IXWebSocketSession::IXWebSocketSession(LWSSessionHandle handle)
    : m_handle(handle)
    , m_protocol("default")
    , m_state(IXWSSESSION_STATE_IDLE)
    , m_try_thread_exit(false)
    , m_port(0)
    , m_plugin_file("default.so")
{
}

IXWebSocketSession::IXWebSocketSession(LWSSessionHandle handle, int port, const char *protocol)
    : m_handle(handle)
    , m_protocol(protocol)
    , m_state(IXWSSESSION_STATE_IDLE)
    , m_try_thread_exit(false)
    , m_port(port)
    , m_plugin_file("default.so")
{

}

IXWebSocketSession::~IXWebSocketSession()
{
    delete m_plugin_adapter;
    m_plugin_adapter = nullptr;
    const IXWebSocketData *data = nullptr;
    while ( (data = __popReadQ()) != nullptr )
        delete data;
    while ( (data = __popWriteQ()) != nullptr )
        delete data;
    if ( IXThread::isAlive() )
    {
        m_try_thread_exit = true;
        join();
    }
}

const LWSSessionHandle IXWebSocketSession::getHandle()
{
    return m_handle;
}

const char *IXWebSocketSession::getProtocol()
{
    return m_protocol.c_str();
}

string IXWebSocketSession::getPeerName()
{
    char client_name[256];
    memset(client_name, 0x00, sizeof(client_name));  
    char client_ip[256]; 
    memset(client_ip, 0x00, sizeof(client_ip)); 

    lws_get_peer_addresses(static_cast<lws *>(m_handle),
            lws_get_socket_fd(static_cast<lws *>(m_handle)),                               
            client_name, sizeof(client_name), client_ip, sizeof(client_ip)); 

    return client_name;
}

bool IXWebSocketSession::writeData(const IXWebSocketData *data)
{
    m_wq.push(data);
    return true;
}

void IXWebSocketSession::writeable()
{
    const IXWebSocketData *data = nullptr;
    data = __popWriteQ();
    if ( !data ) return;

    int _len = data->getDataLength();
    unsigned char *_data = const_cast<unsigned char*>(data->getData());
    unsigned char buf[LWS_PRE + _len + 1];
    memset(buf, 0x00, LWS_PRE + _len + 1);
    memcpy(&buf[LWS_PRE], _data, _len);
    lws_write(static_cast<lws *>(m_handle), &buf[LWS_PRE], _len, LWS_WRITE_TEXT);

    if ( data ) delete data;
    m_state = IXWSSESSION_STATE_IDLE;
}

bool IXWebSocketSession::start()
{
    m_plugin_adapter = new IXPluginAdapter(this);
    if ( !m_plugin_adapter->openPluginFile(m_plugin_file.c_str()) ) 
    {
        delete m_plugin_adapter;
        m_plugin_adapter = nullptr;
        return false;
    }
    IXThread::start(); 
    return true;
}

void IXWebSocketSession::setPluginFile(const char *plugin)
{
    DEBUG("IXWebSocketSession::setPluginFile");
    m_plugin_file = plugin;
}

void *IXWebSocketSession::run()
{
    for ( ;; )                                                                                 
    {                
        if ( m_try_thread_exit ) 
        {
            m_try_thread_exit = false;
            break;
        }                  

        switch ( m_state )                                                                     
        {                                                                                      
            case IXWSSESSION_STATE_IDLE:                                                            
                __stateIdle();                                                                       
                break;                                                                             
            case IXWSSESSION_STATE_PROC_RECV_Q:                                                          
                __stateProcReceivedQueue();                                                                      
                break;                                                                             
            case IXWSSESSION_STATE_PROC_SEND_Q:                                                         
                __stateProcSendQueue();                                                                     
                break;                                                                             
            default:
                INFO("The session thread has been killed now.");                                                                               
                return nullptr;                                                                       
        }                                                                                      

        IXThread::sleep(1);                                                                  
    }

    DEBUG("The session thread has been killed now.");                                                                                          
    return nullptr;                  
}

bool IXWebSocketSession::receivedData(const IXWebSocketData *data)
{
    if ( !data ) return false;
    m_rq.push(data);
    return true;
}

void IXWebSocketSession::onReceivedData(const IXWebSocketData *data)
{
    if ( !data ) return;
    if ( !m_plugin_adapter ) return;

    unsigned char *buf = const_cast<unsigned char *>(data->getData()); 
    m_plugin_adapter->process(buf, data->getDataLength());
}

int IXWebSocketSession::onSendBuffer( unsigned char *buffer, unsigned int length )
{
    IXWebSocketData *send_data = new IXWebSocketData(buffer, length); 
    if ( send_data == NULL )
        return 1;
    
    writeData(send_data); 
    
    return 0;
}

const IXWebSocketData *IXWebSocketSession::__popReadQ()
{
    if ( m_rq.empty() ) return nullptr;

    const IXWebSocketData *rdata = m_rq.front();
    m_rq.pop();
    return rdata;
}

const IXWebSocketData *IXWebSocketSession::__popWriteQ()
{
    if ( m_wq.empty() ) return nullptr;

    const IXWebSocketData *wdata = m_wq.front();
    m_wq.pop();
    return wdata;
}

void IXWebSocketSession::__stateIdle()
{
    if ( !m_wq.empty() )    
    {    
        m_state = IXWSSESSION_STATE_PROC_SEND_Q;    
        DEBUG("There is the data to write in sending queue.");
        DEBUG("State Change [IDLE->SEND_Q]");
        return;    
    }    

    if ( !m_rq.empty() )    
    {    
        DEBUG("There is the data to read in receiving queue.");
        DEBUG("State Change [IDLE->RECV_Q]");
        m_state = IXWSSESSION_STATE_PROC_RECV_Q;    
        return;    
    }    

    if ( m_plugin_adapter != nullptr )
    {
        m_plugin_adapter->procPollingCallback( );
    }
}

void IXWebSocketSession::__stateProcReceivedQueue()
{
    if ( m_rq.empty() ) return;

    const IXWebSocketData *rdata = __popReadQ();
    onReceivedData(rdata);
    delete rdata;
    DEBUG("Pop the data from receiving queue and process it");

    m_state = IXWSSESSION_STATE_IDLE;
    DEBUG("State Change [RECV_Q->IDLE]");
}

void IXWebSocketSession::__stateProcSendQueue()
{
    lws_callback_on_writable(static_cast<lws *>(m_handle));
}

}  /* namespace ixwss */
