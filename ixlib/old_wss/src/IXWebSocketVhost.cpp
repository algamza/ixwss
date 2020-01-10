/**
 * @file IXWebSocketVhost.cpp
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

#include "IXWebSocketVhost.h"
#include "IXWebSocketData.h"
#include "IXLog.h"
#include <fstream>
#include <cstring>

using namespace std;

namespace ixwss {

/*--------------------------------------------------------------------------------
 * class IXWebSocketVhost
 *-------------------------------------------------------------------------------*/

IXWebSocketVhost::IXWebSocketVhost(LWSVhostHandle handle, LXSProtocolHandle protocol)
    : m_handle(handle), m_protocol_handle(protocol), m_port(0)
{
}

IXWebSocketVhost::~IXWebSocketVhost()
{
    __destorySessions();
    delete static_cast<protocols_t *>(m_protocol_handle);
}

LWSVhostHandle IXWebSocketVhost::getHandle()
{
    return m_handle;
}

int IXWebSocketVhost::getPort()
{
    return m_port;
}

void IXWebSocketVhost::setPort(int port)
{
    m_port = port;
}

bool IXWebSocketVhost::createSession(LWSSessionHandle handle, const char *protocol)
{
    if ( !m_protocol_handle || !protocol ) return false;
    IXWebSocketSession *session = new IXWebSocketSession(handle, m_port, protocol);
    if ( !session ) return false; 

    protocols_t *protocol_data = static_cast<protocols_t *>(m_protocol_handle);
    for ( unsigned int i = 0; i<protocol_data->protocol_count-1; i++ )
    {
        if ( string(protocol).compare(protocol_data->protocols[i].name) == 0 ) 
        {
            session->setPluginFile(protocol_data->plugin_files[i].c_str());
            break;
        }
    }
    session->start();
    m_sessions.push_back(session);
    
    return true;
}

bool IXWebSocketVhost::destroySession(LWSSessionHandle handle)
{
    for ( auto it = m_sessions.begin(); it!=m_sessions.end();  )
    {
        IXWebSocketSession *session = static_cast<IXWebSocketSession *>(*it);
        if ( session && (session->getHandle() == handle) )
        {
            delete session; 
            it = m_sessions.erase(it);
        } 
        else 
        {
            ++it;
        }
    }
    
    return false;
}

bool IXWebSocketVhost::receivedData(LWSSessionHandle handle, void *data, int len)
{
    for ( auto session : m_sessions )
    {
        if ( session && (session->getHandle() == handle) )
            session->receivedData(new IXWebSocketData(static_cast<unsigned char *>(data), len)); 
    }
    return true;
}

bool IXWebSocketVhost::writeAble(LWSSessionHandle handle)
{
    for ( auto session : m_sessions )
        if ( session && (session->getHandle() == handle) ) session->writeable(); 
    
    return true;
}

IXWebSocketSession *IXWebSocketVhost::findSession(LWSSessionHandle handle)
{
    for ( auto session : m_sessions )
        if ( session && (session->getHandle() == handle) ) return session;
        
    return nullptr;
}

void IXWebSocketVhost::__destorySessions()
{
    for ( auto it = m_sessions.begin(); it != m_sessions.end(); ++it )
    {
        delete (*it); 
        (*it) = nullptr;
    }

    m_sessions.clear();
}

}  /* namespace ixwss */