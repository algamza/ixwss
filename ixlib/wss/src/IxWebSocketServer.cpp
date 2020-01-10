/**
 * @file IxWebsocketServer.cpp
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

#include "IxWebSocketServer.h"
#include "IxConfigXML.h"
#include "IxConfigPort.h"
#include "IxConnectionInfo.h"
#include "IxWebSocketVhost.h"

#include <cstring>
#include <libwebsockets.h>

using namespace std;

namespace ixs {

IxWebSocketServer::IxWebSocketServer()
    : m_config(nullptr), m_context(nullptr)
{
    
}

IxWebSocketServer::~IxWebSocketServer()
{
    stop();
    delete m_config;
}

bool IxWebSocketServer::initialize(const char *conf_path)
{
    delete m_config; 
    m_config = new IxConfigXML(); 
    if ( !(m_config && m_config->configure(conf_path)) ) return false;
    return true;
}

bool IxWebSocketServer::start()
{
    if ( m_context ) return false; 
    
    auto ports = m_config->getPorts(); 
    bool support_ssl = false; 
    for ( auto port : ports ) 
        if ( port->isSupportSSL() ) support_ssl = true;

    lws_context_creation_info context;
    memset(&context, 0, sizeof(context)); 
    context.options = LWS_SERVER_OPTION_EXPLICIT_VHOSTS; 
    if ( support_ssl ) 
    {
#ifdef LWS_OPENSSL_SUPPORT
        context.options |= 
            LWS_SERVER_OPTION_DO_SSL_GLOBAL_INIT
            | LWS_SERVER_OPTION_SKIP_SERVER_CANONICAL_NAME;
#endif
    }
    m_context = static_cast<IxServerContextHandle>(lws_create_context(&context)); 

    if ( !m_config ) return false;

    for ( auto port : ports ) 
    {
        if ( !port ) continue;
        IxConnectionInfo *protocol = new IxConnectionInfo(port); 
        if ( !protocol ) continue;
        IxWebSocketVhost *vhost = new IxWebSocketVhost(m_context, port->getPort(), protocol); 
        if ( vhost ) vhost->start();
        m_vhosts.push_back(vhost);
    }
    
    IxThread::start(50);
    
    return true;
}

bool IxWebSocketServer::stop()
{
    IxThread::stop();

    if ( m_context ) lws_context_destroy(static_cast<lws_context *>(m_context));
    m_context = nullptr;

    for ( auto vhost : m_vhosts ) 
    {
        delete vhost;
        vhost = nullptr;
    }
    m_vhosts.clear();

    return true;
}

void IxWebSocketServer::run()
{
    if ( m_context ) lws_service(static_cast<lws_context *>(m_context), 50);
}

} /* namespace ixs */