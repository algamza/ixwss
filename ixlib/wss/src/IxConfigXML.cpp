/**
 * @file IxConfigXML.cpp
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

#include "IxConfigXML.h"
#include "IxConfigPort.h"
#include "IxConfigProtocol.h"

#include <libxml/tree.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>

#include <cstring>

using namespace std;

namespace ixs {

IxConfigXML::IxConfigXML()
{
    
}

IxConfigXML::~IxConfigXML()
{
}

bool IxConfigXML::configure(const char *conf_path)
{
    if ( !conf_path ) return false; 
    auto doc = xmlParseFile(conf_path); 
    if ( !doc ) return false;
    auto root = xmlDocGetRootElement(doc); 
    if ( !root ) return false;

    auto attribute = [](xmlNodePtr node, const char *name) -> string 
    {
        if ( !name || !node ) return ""; 
        auto _attr = xmlGetProp(node, reinterpret_cast<const xmlChar*>(name));
        if ( !_attr ) return ""; 
        string attr = reinterpret_cast<const char*>(_attr); 
        xmlFree(_attr);
        return attr; 
    };

    auto child_content = [](xmlNodePtr node, const char *name) -> string
    {
        if ( !name || !node ) return ""; 
        auto child = node->children; 
        while(child)
        {
            if ( !child->name ) return "";
            if ( strcmp(reinterpret_cast<const char*>(child->name), name) == 0 ) 
            {
                auto _content = xmlNodeGetContent(child); 
                if ( !_content ) return ""; 
                string content = reinterpret_cast<const char*>(_content); 
                xmlFree(_content);
                return content; 
            }
            child = child->next; 
        }
        return ""; 
    };

    auto protocol = [attribute, child_content](xmlNodePtr node, IxConfigPort *conf) -> xmlNodePtr
    {
        if ( !node || !node->name || !conf ) return nullptr;
        const char *name = reinterpret_cast<const char*>(node->name); 
        IxConfigProtocol::IxConfigProtocolType type
            = IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_NONE;
        if ( strcmp(name, "ws") == 0 )
            type = IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WS; 
        else if ( strcmp(name, "wss") == 0 )
            type = IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_WSS; 
        else if ( strcmp(name, "http") == 0 )
            type = IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_HTTP; 
        else if ( strcmp(name, "https") == 0 )
            type = IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_HTTPS; 

        if ( type != IxConfigProtocol::IxConfigProtocolType::PROTOCOL_TYPE_NONE )
        {
            conf->addConfigProtocol(new IxConfigProtocol(type, 
                attribute(node, "subprotocol").c_str(), child_content(node, "plugin").c_str()));
        }

        return node->next;
    }; 

    auto setting = [protocol](xmlNodePtr node, IxConfigPort *conf) -> xmlNodePtr
    {
        if ( !node || !node->name || !conf ) return nullptr;
        if ( strcmp(reinterpret_cast<const char*>(node->name), "setting") != 0 ) return node->next;
        auto child = node->children; 
        while(child) child = protocol(child, conf);
        return node->next;
    };

    auto ssl = [child_content](xmlNodePtr node, IxConfigPort *conf) -> xmlNodePtr
    {
        if ( !node || !node->name || !conf ) return nullptr;

        if ( strcmp(reinterpret_cast<const char*>(node->name), "ssl") != 0 ) return node->next;
        
        string cert = child_content(node, "certificate");
        string ca = child_content(node, "rootca"); 
        string key = child_content(node, "privatekey"); 
        conf->setConfigSSL(new IxConfigPort::IxConfigSSL(
            key.c_str(), ca.c_str(), cert.c_str()));
        
        return node->next;
    };

    auto vhost = [this, setting, ssl, attribute](xmlNodePtr node) -> xmlNodePtr
    {
        if ( !node ) return nullptr;

        string port = attribute(node, "port");
        if ( port.compare("") == 0 ) return node->next;
        IxConfigPort *conf_port = new IxConfigPort(stoi(port));
        if ( !conf_port ) return nullptr;
        addPort(conf_port); 

        auto setting_child = node->children;
        while(setting_child) setting_child = setting(setting_child, conf_port); 

        if ( attribute(node, "ssl").compare("true") != 0 ) return node->next;
        
        auto ssl_child = node->children;
        while(ssl_child) ssl_child = ssl(ssl_child, conf_port);

        return node->next;
    };

    xmlNodePtr node = root->children; 
    while(node) node = vhost(node);

    xmlFreeDoc(doc); 

    return true;
}

} /* namespace ixs */
