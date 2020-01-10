/**
 * @file IxConfigPort.cpp
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

#include "IxConfigPort.h"
#include "IxConfigProtocol.h"

using namespace std;

namespace ixs {


IxConfigPort::IxConfigSSL::IxConfigSSL(const char *key, const char *ca, const char *cert)
    : m_private_key(""), m_certificate(""), m_root_ca("")
{
    if ( key ) m_private_key.assign(key); 
    if ( ca ) m_root_ca.assign(ca); 
    if ( cert ) m_certificate.assign(cert); 
}

const char *IxConfigPort::IxConfigSSL::getPrivateKey() const
{
    return m_private_key.c_str();
}

const char *IxConfigPort::IxConfigSSL::getCertificate() const
{
    return m_certificate.c_str();
}

const char *IxConfigPort::IxConfigSSL::getRootCA() const
{
    return m_root_ca.c_str();
}

IxConfigPort::IxConfigPort(int port)
    : m_port(port), is_support_ssl(false), m_ssl(nullptr) 
{

}

IxConfigPort::~IxConfigPort()
{
    delete m_ssl; 
    
    for ( auto it : m_protocols )
    {
        delete it; 
        it = nullptr;
    }
    m_protocols.clear();
}

int IxConfigPort::getPort() const
{
    return m_port; 
}

bool IxConfigPort::setConfigSSL(IxConfigPort::IxConfigSSL *ssl)
{
    if ( !ssl ) return false;
    delete m_ssl; 
    m_ssl = ssl; 
    is_support_ssl = true;
    return true;
}

bool IxConfigPort::isSupportSSL() const
{
    return is_support_ssl;
}

const IxConfigPort::IxConfigSSL *IxConfigPort::getConfigSSL() const
{
    return m_ssl; 
}

bool IxConfigPort::addConfigProtocol(IxConfigProtocol *protocol)
{
    if ( !protocol ) return false;
    m_protocols.push_back(protocol);
    return true; 
}

const IxConfigProtocol *IxConfigPort::getConfigProtocol(int index) const
{
    if ( m_protocols.empty() ) return nullptr;
    auto it = m_protocols.begin();
    advance(it, index); 
    return *it;
}

int IxConfigPort::getConfigProtocolSize() const
{
    return m_protocols.size(); 
}

} /* namespace ixs */
