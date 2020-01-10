/**
 * @file IXWebSocketClient.cpp
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief Client based on WebSocket
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


#include "IXWebSocketClient.h"
#include "IXWebSocketClientPrivate.h"

using namespace std;

namespace ixwss {

static IXWebSocketClientListener *s_recv_callback = nullptr;

class __IXWebSocketClientPrivateListener : public IXWebSocketClientPrivateListener 
{
public:
    virtual int onReceiveData( unsigned char *buffer, unsigned int length )
    {
        if ( s_recv_callback )
        {
            s_recv_callback->onReceiveData(buffer,length);
        }
        return 0;
    }
};    

static __IXWebSocketClientPrivateListener *s_private_listener = nullptr;

IXWebSocketClient::IXWebSocketClient(const char *name, const char *address, int port, const char *protocol)
    : m_client_private(nullptr)
{
    m_client_private = new IXWebSocketClientPrivate(name,address,port,protocol);
}

IXWebSocketClient::~IXWebSocketClient()
{
    if ( m_client_private ) m_client_private->removeEventListener();

    if ( m_client_private ) delete m_client_private;

    if ( s_private_listener ) delete s_private_listener;
}

bool IXWebSocketClient::connect()
{
    if ( !m_client_private )
    {
        return false;
    }

    if ( !m_client_private->connect() )
    {
        return false;
    }
    return true;
}

bool IXWebSocketClient::disconnect()
{
    if ( !m_client_private )
    {
        return false;
    }

    if ( !m_client_private->disconnect() )
    {
        return false;
    }   
    return true;
}

bool IXWebSocketClient::send(const char *data, int dlen)
{
    if ( !data || !dlen || !m_client_private ) return false;

    if ( !m_client_private->send(data,dlen) ) return false;

    return true;
}


bool IXWebSocketClient::addEventListener(IXWebSocketClientListener *callback)
{
    if ( !callback || !m_client_private ) return false;

    if ( s_private_listener ) return false;

    s_private_listener = new __IXWebSocketClientPrivateListener();

    m_client_private->addEventListener(s_private_listener);

    s_recv_callback = callback;

    return true;
}

void IXWebSocketClient::removeEventListener()
{
    if ( m_client_private ) m_client_private->removeEventListener();

    if ( s_private_listener ) delete s_private_listener;

    s_recv_callback = nullptr;
}

}  /* namespace ixwss */
