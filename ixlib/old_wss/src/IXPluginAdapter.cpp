/** **********************************************************************************************************
	@file 		IXPluginAdapter.cpp

	@date		2016/07/20
	@author		Humax SW Group
	@breif

	(c) 2016 Humax Co., Ltd.
	This program is produced by Humax Co., Ltd. ("Humax") and
	the proprietary Software of Humax and its licensors. Humax provides you, as an Authorized Licensee,
	non-assignable, non-transferable and non-exclusive license to use this Software.
	You acknowledge that this Software contains valuable trade secrets of Humax and by using this Software
	you agree to the responsibility to take all reasonable efforts to protect the any information
	you receive from Humax. You are not permitted to duplicate, modify, distribute, sell or lease and
	reverse engineer or extract the source code of this Software unless you have Humax's written permission
	to do so.
	If you have no authorized license, discontinue using this Software immediately.

	THE SOFTWARE IS PROVIDED "AS IS" AND HUMAX MAKES NO PROMISES, REPRESENTATIONS OR WARRANTIES,
	EITHER EXPRESS, IMPLIED OR STATUTORY, OR OTHERWISE, WITH RESPECT TO THE SOFTWARE.
	IN NO EVENT SHALL HUMAX BE LIABLE FOR LOST PROFITS, REVENUES, OR DATA, FINANCIAL LOSSES OR
	INDIRECT, SPECIAL, CONSEQUENTIAL, EXEMPLARTY OR PUNITIVE DAMAGES WHATSOEVER RELATING TO YOUR USE OR
	INABILITY TO USE THE SOFTWARE.

	This License is effective until terminated. You may terminate this License at any time
	by destroying all copies of the Software including all documentation. This License will terminate
	immediately without notice from Humax to you if you fail to comply with any provision of this License.
	Upon termination, you must destroy all copies of the Software and all documentation.

	The laws of the Republic of Korea will apply to any disputes arising out of or relating to
	this Copyright Notice. All claims arising out of or relating to this Copyright Notice will be litigated
	in the Seoul Central District Court, in the Republic of Korea.
*********************************************************************************************************** */

#include "IXPluginAdapter.h"
#include "IXLog.h"

#include <dlfcn.h>

using namespace std;

namespace ixwss {

IXPluginAdapter::IXPluginAdapter(IXPluginAdapterListener* listener )
    :m_listener(listener)
    ,m_plugin_handler(nullptr)
    ,m_polling_callback(nullptr)
{
    ENTRY;

    m_plugins_obj.ixwss_adapter = this;
    m_plugins_obj.private_data = nullptr;
    m_plugins_obj.session_handle = nullptr;
}

IXPluginAdapter::~IXPluginAdapter()
{
    ENTRY;
    if ( m_plugin_handler != NULL )
    {
        m_plugin_handler->destroy( &m_plugins_obj );
    }
    if ( m_dl_handle != NULL )
    {
        dlclose( m_dl_handle );
    }
    pthread_mutex_destroy(&m_mutex);
}

bool IXPluginAdapter::openPluginFile(const char *name)
{
    if ( !name )
        return false;

    INFO(name);

    const char* plugin_file = name;
    m_callback.send_buffer = &IXPluginAdapter::send_buffer;
    m_callback.send_event = &IXPluginAdapter::send_event;
    m_callback.set_polling_cb = &IXPluginAdapter::set_polling_cb;

    m_dl_handle = dlopen( plugin_file, RTLD_NOW);
    if ( !m_dl_handle )
        return false;

    m_capability = reinterpret_cast<ixserver_plugin_capability_function *>\
                        (dlsym(m_dl_handle, "ixserver_plugin_capability"));
    if ( m_capability )
    {
        if ( m_capability(&m_callback, &m_plugin_handler) != 0
                || m_plugin_handler->initialize(&m_plugins_obj) != 0 )
        {
            return false;
        }
    }

    pthread_mutex_init(&m_mutex, NULL );

    return true;
}

bool IXPluginAdapter::process( unsigned char *buffer, unsigned int length )
{
    if ( m_plugin_handler != NULL )
    {
        return m_plugin_handler->process( &m_plugins_obj, buffer, length );
    }
    return true;
}

bool IXPluginAdapter::procPollingCallback( void )
{
    if ( !m_polling_callback )
        return false;

    pthread_mutex_lock(&m_mutex);
    int ret = m_polling_callback(&m_plugins_obj);
    pthread_mutex_unlock(&m_mutex);

    return ret == 0;
}

int IXPluginAdapter::send_buffer( ixserver_plugins_object *ctx, unsigned char *buffer, unsigned int length )
{
    IXPluginAdapter *plugin_adapter = static_cast<IXPluginAdapter*>(ctx->ixwss_adapter);
    if ( !plugin_adapter || !plugin_adapter->m_listener )
        return 1;

    return plugin_adapter->m_listener->onSendBuffer( buffer, length );
}

int IXPluginAdapter::send_event( ixserver_plugins_object *ctx, unsigned char *buffer, unsigned int length )
{
    IXPluginAdapter *plugin_adapter = static_cast<IXPluginAdapter*>(ctx->ixwss_adapter);
    if ( !plugin_adapter || !plugin_adapter->m_listener )
        return 1;

    return plugin_adapter->m_listener->onSendBuffer( buffer, length );
}

int IXPluginAdapter::set_polling_cb( ixserver_plugins_object *ctx, ixserver_plugins_polling_callback *cb )
{
    IXPluginAdapter *plugin_adapter = static_cast<IXPluginAdapter*>(ctx->ixwss_adapter);
    if ( !plugin_adapter )
        return 1;

    return plugin_adapter->addPollingCallback( cb );
}

int IXPluginAdapter::addPollingCallback( ixserver_plugins_polling_callback *cb )
{
    m_polling_callback = cb;
    return 0;
}

}  /* namespace ixwss */
