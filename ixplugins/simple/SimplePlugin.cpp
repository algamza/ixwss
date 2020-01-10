/**
 * @file SimplePlugin.cpp
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

#include "SimplePlugin.h"
#include <iostream>
#include <string>
#include <vector>
#include <cstdio>

using namespace std;

static ixserver_plugins_plugin_handler s_plugin_handler;
static ixserver_plugins_callbacks *s_ixwss_cb = nullptr;

static int json_rpc_plugin_pollingcallback( ixserver_plugins_object *obj )
{
    string msg("Hi! client. I'm simple plugin. nice to meet you :) this is event that i send after the plugin has been initialized");

    static bool first = true;

    if ( first )
    {
        s_ixwss_cb->send_buffer( obj, (unsigned char *)msg.c_str(), msg.length() );
        first = false;
    }

    return 0;
}

static int json_rpc_plugin_initialize( ixserver_plugins_object *obj )
{
    ENTRY;

    s_ixwss_cb->set_polling_cb( obj, json_rpc_plugin_pollingcallback );

    return 0;
}

static int json_rpc_plugin_destroy( ixserver_plugins_object *obj )
{
    ENTRY;

    s_ixwss_cb->set_polling_cb( obj, nullptr );

    return 0;
}

static int json_rpc_plugin_process( ixserver_plugins_object *obj, unsigned char *buffer, unsigned int length )
{
    ENTRY;
    
    string buf((const char *)buffer,length);
    cout << "received data : " << buf << endl;

    string msg("Hi! client. I'm simple plugin. I get the data from you : ");
    msg.append((const char *)buffer,length);
    s_ixwss_cb->send_buffer( obj, (unsigned char *)msg.c_str(), msg.length() );

    return 0;
}


extern "C" {

int ixserver_plugin_capability( ixserver_plugins_callbacks *cb, ixserver_plugins_plugin_handler **ret )
{
    ENTRY;
    
    s_plugin_handler.process = &json_rpc_plugin_process;
    s_plugin_handler.initialize = &json_rpc_plugin_initialize;
    s_plugin_handler.destroy = &json_rpc_plugin_destroy;
    *ret = &s_plugin_handler;
    s_ixwss_cb = cb;

    return 0;
}

};

