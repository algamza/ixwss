/**
 * @file IXWebSocketSetting.cpp
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

#include "IXWebSocketSetting.h"
#include "IXLog.h"

#include <fstream>
#include <cstring>

using namespace std;

namespace ixwss {

/*--------------------------------------------------------------------------------
 * class IXWebSocketSetting
 *-------------------------------------------------------------------------------*/
IXWebSocketSetting::IXWebSocketSetting()
    : m_sever_config_path("")
{
}

IXWebSocketSetting::~IXWebSocketSetting()
{
}

bool IXWebSocketSetting::addPort(int port, list<string> protocols)
{
    m_port_protocols[port] = protocols;
    return true;
}

void IXWebSocketSetting::getPorts(std::list<int> &ports)
{
    for ( auto it = m_port_protocols.begin(); it != m_port_protocols.end(); ++it )
    {
        ports.push_back(it->first);
    }
}

void IXWebSocketSetting::getProtocols(int port, std::list<std::string> &protocols)
{
    map<int,list<string>>::iterator it = m_port_protocols.find(port); 
    if ( it != m_port_protocols.end() )
    {
        for ( auto protocol : it->second )
        {
            protocols.push_back(protocol);
        }
    }
}

bool IXWebSocketSetting::setServerConfigPath(const char *path)
{
    if ( !path ) return false;
    m_sever_config_path.assign(path);

    if ( !setServerConfig(m_sever_config_path) ) return false;
        
    return true;
}

bool IXWebSocketSetting::setServerConfig(std::string conf_path)
{
    char readbuffer[256];
    char *token = NULL;
    ifstream config_file;
    string wss_config_file_path = conf_path + "/ixwss.conf";

    config_file.open(wss_config_file_path.c_str(), ios_base::binary);
    
    if ( config_file.is_open() != true )
    {
        CRITICAL("It's wrong server config path !!!"); 
        return false;
    }
        
    while ( config_file.eof() != true )
    {
        int port = 0;
        list<string> protocols;

        config_file.getline(readbuffer, sizeof(readbuffer));
        if ( !config_file.fail() && !config_file.bad() )
        {
            if ( readbuffer[0] == '#' )
                continue;
    
            token = strtok(readbuffer, "=");

            if ( token == NULL ) continue;

            if ( strcmp(token,"map_port_protocols") == 0 )
            {
                token = strtok(NULL, ":");
                if ( token != NULL )
                {
                    port = atoi(token);
                }
                
                token = strtok(NULL, ":");
                if ( token != NULL ) 
                {
                    token = strtok(token, ",\n\r");
                    while ( token != NULL )
                    {
                        protocols.push_back(token); 
                        token = strtok(NULL, ",\n\r");
                    }
                }
                else
                {
                    protocols.push_back("default"); 
                }

                addPort(port,protocols);
                INFO("* port : " << port );
                for (list<string>::iterator it=protocols.begin(); it != protocols.end(); ++it)
                {
                    INFO("** sub-protocol : " << *it );
                }
            }
            else
            {
                CRITICAL("* There is wrong field : " << token );
            }
        }
    }
    
    config_file.close();

    return true;
}

}  /* namespace ixwss */