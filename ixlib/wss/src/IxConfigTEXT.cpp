/**
 * @file IxConfigTEXT.cpp
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

#include "IxConfigTEXT.h"
#include "IxConfigPort.h"
#include "IxConfigProtocol.h"

#include <fstream>
#include <cstring>

using namespace std;

namespace ixs {

IxConfigTEXT::IxConfigTEXT()
{
    
}

IxConfigTEXT::~IxConfigTEXT()
{
}

bool IxConfigTEXT::configure(const char *conf_path)
{
    if ( !conf_path ) return false;
    ifstream file;
    file.open(conf_path, ios_base::binary);
    if ( !file.is_open() ) return false;
    string m_plugin_dir;
    while ( !file.eof() )
    {
        char readbuffer[256];
        memset(readbuffer, 0x00, sizeof(readbuffer)); 
        char *token = nullptr;

        file.getline(readbuffer, sizeof(readbuffer));
        
        if ( file.fail() || file.bad() ) continue;
        if ( readbuffer[0] == '#' ) continue;
        token = strtok(readbuffer, "=");
        if ( !token ) continue;
        if ( strcmp(token,"plugin_path") == 0 )
        {
            token = strtok(nullptr, ":");
            if ( token != nullptr ) m_plugin_dir.assign(token);
        }
        else if ( strcmp(token,"plugin_map") == 0 )
        {
            int port = 0;
            IxConfigProtocol::IxConfigProtocolType type 
                = IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_NONE;
            string protocol = ""; 
            string plugin = ""; 

            token = strtok(nullptr, ":");
            if ( token ) port = atoi(token);

            token = strtok(nullptr, ":");
            if ( token ) 
            {
                if ( strstr(token, "http") ) 
                    type = IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_HTTP;
                else if ( strstr(token, "ws") )
                    type = IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WSS;
            }

            token = strtok(nullptr, ":");
            if ( token )
            {
                if ( strstr(token,".so") )
                {
                    plugin.assign(m_plugin_dir);
                    plugin.append("/");
                    plugin.append(token);
                }
                else
                {
                    protocol.assign(token);
                }
            }

            token = strtok(nullptr, ":");
            if ( token )
            {
                plugin.assign(m_plugin_dir);
                plugin.append("/");
                plugin.append(token);
            }

            bool exist_port = false; 
            list<IxConfigPort *> ports = getPorts(); 
            for ( auto it : ports ) 
            {
                if ( it && (port == it->getPort()) )  
                {
                    it->addConfigProtocol(
                        new IxConfigProtocol(type, protocol.c_str(), plugin.c_str()));
                    exist_port = true;
                }
            }

            if ( !exist_port )
            {
                IxConfigPort *conf_port = new IxConfigPort(port); 
                if ( conf_port ) 
                    conf_port->addConfigProtocol(
                        new IxConfigProtocol(type, protocol.c_str(), plugin.c_str()));
                addPorts(conf_port); 
            }
        }
    }

    file.close();

    return true;
}

} /* namespace ixs */
