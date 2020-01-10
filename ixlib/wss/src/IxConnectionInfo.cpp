/**
 * @file IxConnectionInfo.cpp
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

#include "IxConnectionInfo.h"
#include "IxConfigProtocol.h"
#include "IxConfigPort.h"

#include <cstring>

using namespace std;

namespace ixs {

IxConnectionInfo::IxConnectionInfo(const IxConfigPort *config)
    : m_config(config)
{
    if ( !config ) return;

    auto has_no_name = [this](IxConfigProtocol::IxConfigProtocolType type) -> bool
    {
        for ( int i = 0; i < m_config->getConfigProtocolSize(); i++ ) 
        {
            const IxConfigProtocol *protocol = m_config->getConfigProtocol(i);
            if ( !protocol ) continue;
            if ( protocol->getProtocolType() != type ) continue;
            if ( !protocol->getSubProtocol() || (strcmp(protocol->getSubProtocol(),"") == 0) )
                return true;
        }
        return false;
    }; 

    bool http = has_no_name(IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_HTTP);
    bool https = has_no_name(IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_HTTPS);
    bool ws = has_no_name(IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WS);
    bool wss = has_no_name(IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WSS);

    if ( (http || https) && (wss || ws) ) m_protocols.push_front("http-wss-hybrid");
    else if ( http || https ) m_protocols.push_front("http-only");
    else if ( ws || wss ) m_protocols.push_front("wss-no-sub-protocol");
    else m_protocols.push_front("none-protocol");
    
    for( int i = 0; i < m_config->getConfigProtocolSize(); i++ ) 
    {
        const IxConfigProtocol *protocol = m_config->getConfigProtocol(i);
        if ( !protocol ) continue;
        IxConfigProtocol::IxConfigProtocolType type = protocol->getProtocolType(); 
        if ( (type != IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WSS)
            && (type != IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WS) ) continue;
        const char *name = protocol->getSubProtocol();
        if ( !name || (strcmp(name, "") == 0) ) continue;
        m_protocols.push_back(name);
    }
}

IxConnectionInfo::~IxConnectionInfo()
{
    m_protocols.clear();
}

list<const char *> IxConnectionInfo::getProtocols()
{
    list<const char *> protocols; 
    for ( auto protocol : m_protocols )
        protocols.push_back(protocol.c_str()); 

    return protocols; 
}

const char *IxConnectionInfo::getPluginPath(const char *protocol_name, IxDataType type)
{
    if ( !m_config || !protocol_name ) return nullptr;

    auto find = [this](IxConfigProtocol::IxConfigProtocolType _type
        , const char *sub_protocol) -> const char *
    {
        for ( int i = 0; i < m_config->getConfigProtocolSize(); i++ ) 
        {
            const IxConfigProtocol *protocol = m_config->getConfigProtocol(i);
            if ( !protocol ) continue;
            if ( protocol->getProtocolType() != _type ) continue;
            if ( !sub_protocol && (!protocol->getSubProtocol() 
                || (strcmp(protocol->getSubProtocol(),"") == 0) ))
                return protocol->getPluginPath();
            if ( !protocol->getSubProtocol() && (!sub_protocol 
                || (strcmp(sub_protocol,"") == 0) )) 
                return protocol->getPluginPath();
            if ( !sub_protocol || !protocol->getSubProtocol() ) continue;
            if ( strcmp(sub_protocol, protocol->getSubProtocol()) == 0 )
                return protocol->getPluginPath();
        } 
        return nullptr;
    };

    auto find_plugin = [this, find, type](IxProtocolType _type
        , const char *name) -> const char *
    {
        switch(_type)
        {
            case IxProtocolType::IX_PROT_TYPE_NONE: return nullptr;
            case IxProtocolType::IX_PROT_TYPE_HTTP:
                {
                    const char *http_protocol = find(
                        IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_HTTP, nullptr);
                    if ( http_protocol ) return http_protocol; 
                    const char *https_protocol = find(
                        IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_HTTPS, nullptr);
                    if ( https_protocol ) return https_protocol; 
                }
                break;
            case IxProtocolType::IX_PROT_TYPE_WSS:
                {
                    const char *ws_protocol = find(
                        IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WS, name);
                    if ( ws_protocol ) return ws_protocol; 
                    const char *wss_protocol = find(
                        IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WSS, name);
                    if ( wss_protocol ) return wss_protocol; 
                }
                break;
            case IxProtocolType::IX_PROT_TYPE_WSS_NO_SUBP:
                {
                    const char *ws_protocol = find(
                        IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WS, nullptr);
                    if ( ws_protocol ) return ws_protocol; 
                    const char *wss_protocol = find(
                        IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WSS, nullptr);
                    if ( wss_protocol ) return wss_protocol; 
                }
                break;
            case IxProtocolType::IX_PROT_TYPE_HTTP_WSS:
                {
                    if ( type == IxConnectionInfo::IxDataType::IX_DATA_TYPE_HTTP )
                    {
                        const char *http_protocol = find(
                            IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_HTTP, nullptr);
                        if ( http_protocol ) return http_protocol; 
                        const char *https_protocol = find(
                            IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_HTTPS, nullptr);
                        if ( https_protocol ) return https_protocol; 
                    }
                    else if ( type == IxConnectionInfo::IxDataType::IX_DATA_TYPE_WSS )
                    {
                        const char *ws_protocol = find(
                        IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WS, nullptr);
                        if ( ws_protocol ) return ws_protocol; 
                        const char *wss_protocol = find(
                            IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WSS, nullptr);
                        if ( wss_protocol ) return wss_protocol; 

                    }
                }
                break;
            default: break;
        }
        return nullptr;
    };

    if ( strcmp(protocol_name, "http-only") == 0 )
        return find_plugin(IxProtocolType::IX_PROT_TYPE_HTTP, nullptr);
    else if ( strcmp(protocol_name, "wss-no-sub-protocol") == 0 )
        return find_plugin(IxProtocolType::IX_PROT_TYPE_WSS_NO_SUBP, nullptr);
    else if ( (strcmp(protocol_name, "none-protocol") == 0) || strcmp(protocol_name, "") == 0 )
        return find_plugin(IxProtocolType::IX_PROT_TYPE_NONE, nullptr);
    else if ( strcmp(protocol_name, "http-wss-hybrid") == 0 )
        return find_plugin(IxProtocolType::IX_PROT_TYPE_HTTP_WSS, nullptr);
    else 
        return find_plugin(IxProtocolType::IX_PROT_TYPE_WSS, protocol_name);

    return nullptr;
}

IxConnectionInfo::IxProtocolType IxConnectionInfo::getProtocolType(const char *protocol_name)
{
    IxProtocolType type = IxProtocolType::IX_PROT_TYPE_NONE; 
    if ( !protocol_name ) return type; 
    for ( auto protocol : m_protocols )
    {
        if ( !(protocol.c_str() && (strcmp(protocol_name, protocol.c_str()) == 0)) ) continue;

        if ( strcmp(protocol_name, "http-only") == 0 ) 
            type = IxProtocolType::IX_PROT_TYPE_HTTP; 
        else if ( strcmp(protocol_name, "wss-no-sub-protocol") == 0 ) 
            type = IxProtocolType::IX_PROT_TYPE_WSS_NO_SUBP;
        else if ( strcmp(protocol_name, "none-protocol") == 0 ) 
            type = IxProtocolType::IX_PROT_TYPE_NONE; 
        else if ( strcmp(protocol_name, "http-wss-hybrid") == 0 ) 
            type = IxProtocolType::IX_PROT_TYPE_HTTP_WSS;
        else 
            type = IxProtocolType::IX_PROT_TYPE_WSS;
    
        break;
    }
    return type;
}

bool IxConnectionInfo::isSupportSSL()
{
    if ( !m_config ) return false;
    bool support_ssl = m_config->isSupportSSL();
    return support_ssl; 
}

const char *IxConnectionInfo::getSSLCerticatePath() 
{
    if ( !m_config ) return nullptr;
    auto config_ssl = m_config->getConfigSSL(); 
    if ( !config_ssl ) return nullptr;
    return config_ssl->getCertificate();
}

const char *IxConnectionInfo::getSSLRootCAPath()
{
    if ( !m_config ) return nullptr;
    auto config_ssl = m_config->getConfigSSL(); 
    if ( !config_ssl ) return nullptr;
    return config_ssl->getRootCA();
}

const char *IxConnectionInfo::getSSLPrivateKeyPath()
{
    if ( !m_config ) return nullptr;
    auto config_ssl = m_config->getConfigSSL(); 
    if ( !config_ssl ) return nullptr;
    return config_ssl->getPrivateKey();
}

} /* namespace ixs */
