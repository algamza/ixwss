/**
 * @file IXWebSocketPluginSetting.cpp
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

#include "IXWebSocketPluginSetting.h"
#include "IXLog.h"

#include <fstream>
#include <cstring>

using namespace std;

namespace ixwss {

/*--------------------------------------------------------------------------------
 * class IXWebSocketPluginSetting
 *-------------------------------------------------------------------------------*/

IXWebSocketPluginSetting::IXWebSocketPluginSetting()
    : m_plugin_config_path("")
    , m_plugin_base_path("")
{
    
}

IXWebSocketPluginSetting::~IXWebSocketPluginSetting()
{
    m_ports.clear();
    m_protocols.clear();
    m_plugins_path.clear();
}

bool IXWebSocketPluginSetting::setPluginConfigPath(const char *path)
{
    if ( !path ) return false;
    m_plugin_config_path.assign(path);

    if ( !setPluginConfig(m_plugin_config_path) ) return false;
        
    return true;

}

const char *IXWebSocketPluginSetting::getPluginFilePath(int port, const char *protocol_name)
{
    int index = 0;

    for ( auto it : m_ports )
    {
        if ( it == port )
        {
            auto protocol = m_protocols.begin();
            advance(protocol, index);

            string default_name = "default"; 
            if ( protocol_name ) default_name.assign(protocol_name);
            if ( (*protocol).compare(default_name) == 0 )
            {
                auto plugin = m_plugins_path.begin();
                advance(plugin, index); 
                return (*plugin).c_str();
            }
        }
        ++index;
    }
    return nullptr;
}

bool IXWebSocketPluginSetting::setPluginConfig(std::string conf_path)
{
    m_ports.clear();
    m_protocols.clear();
    m_plugins_path.clear();

    char readbuffer[256];
    char *token = NULL;
    ifstream config_file;
    string plugin_config_file_path(conf_path);
    plugin_config_file_path.append("/ixplugins.conf");

    config_file.open(plugin_config_file_path.c_str(), ios_base::binary);
    
    if ( config_file.is_open() != true )
    {
        CRITICAL("It's wrong plugin config path !!! : " << plugin_config_file_path ); 
        return false;
    }
        
    while ( config_file.eof() != true )
    {
        int port = 0;
        string protocol = ""; 
        string plugin = "";
        
        config_file.getline(readbuffer, sizeof(readbuffer));
        if ( !config_file.fail() && !config_file.bad() )
        {
            if ( readbuffer[0] == '#' )
                continue;
    
            token = strtok(readbuffer, "=");

            if ( token == NULL ) continue;

            if ( strcmp(token,"plugin_path") == 0 )
            {
                token = strtok(NULL, ":");
                if ( token != NULL )
                {
                    m_plugin_base_path.assign(token);
                    INFO("* plugin_path : " << m_plugin_base_path );
                }
            }
            else if (strcmp(token,"map_port_protocol_plugin") == 0 )
            {
                token = strtok(NULL, ":");
                if ( token != NULL )
                {
                    port = atoi(token);
                }

                token = strtok(NULL, ":");
                if ( token != NULL ) 
                {
                    /**
                     *@note condition of ixplugins.conf
                     */
                    if ( strstr(token,".so") )
                    {
                        protocol = "default";
                        plugin.assign(m_plugin_base_path);
                        plugin.append("/");
                        plugin.append(token);
                    }
                    else
                    {
                        protocol = token;
                        token = strtok(NULL, ":");
                        if ( token != NULL ) 
                        {
                            plugin.assign(m_plugin_base_path);
                            plugin.append("/");
                            plugin.append(token);
                        }
                        else
                        {
                            plugin = "default.so";
                        }

                    }
                }

                INFO("* port : " << port << ", sub-protocol : " << protocol << ", plugin : " << plugin );
                m_ports.push_back(port);
                m_protocols.push_back(protocol);
                m_plugins_path.push_back(plugin); 
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

} /* namespace ixwss */
