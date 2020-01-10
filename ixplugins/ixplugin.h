/**
 * @file ixplugin.h
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


#ifndef __IXSERVER_PLUGINS_H__
#define __IXSERVER_PLUGINS_H__


typedef struct ixserver_plugins_object
{
	void *ixwss_adapter;
	void *private_data;
	void *session_handle; 
}ixserver_plugins_object;

/** used to send buffer as response.
    @param ctx the handle value for context handling
	@param buffer a pointer indicating buffer to be sent
	@param length the length of buffer
	@return zero if successful.
*/
typedef int (ixserver_plugins_callback_send_buffer)( ixserver_plugins_object *obj, unsigned char *buffer, unsigned int length );

/** used to send buffer as event.
    @param ctx the handle value for context handling
	@param buffer a pointer indicating buffer to be sent
	@param length the length of buffer
	@return zero if successful.
*/
typedef int (ixserver_plugins_callback_send_event)( ixserver_plugins_object *obj, unsigned char *buffer, unsigned int length );

/** callback function when wss is ready to calling, it will be called.
	@param ctx the handle value for context handling
	@return zero if successful.
*/
typedef int (ixserver_plugins_polling_callback)( ixserver_plugins_object *obj );

/** used to set a callback to wss server, the wss server will this callback when it's ready to call.
	@param ctx the handle value for context handling
	@param cb the callback, user set callback to make wss server check plugins context.
	@return zero if successful.
*/
typedef int (ixserver_plugins_callback_set_polling_cb)( ixserver_plugins_object *obj, ixserver_plugins_polling_callback *cb );

typedef struct ixserver_plugins_callbacks
{
    ixserver_plugins_callback_send_buffer *send_buffer;
	ixserver_plugins_callback_send_event *send_event;
	ixserver_plugins_callback_set_polling_cb *set_polling_cb;
}ixserver_plugins_callbacks;

/** called when wss server recieved some data from client to get response 
	@param ctx the handle value for context handling
	@param buffer the point indicating buffer has client sent.
	@param length the length of buffer client sent.
	@return zero if successful.
*/
typedef int (ixserver_plugins_plugin_handler_process)( ixserver_plugins_object *obj, unsigned char *buffer, unsigned int length );

/** called when wss server want to prepare wss_plugin resources
	@param ctx the handle value for context handling
	@return zero if successful.
*/
typedef int (ixserver_plugins_plugin_handler_initialize)( ixserver_plugins_object *obj );

/** called when wss server destroy and release all resources of this plugins.
	@param cb the callback, user set callback to make wss server check plugins context.
	@return zero if successful.
*/
typedef int (ixserver_plugins_plugin_handler_destroy)( ixserver_plugins_object *obj );

typedef struct ixserver_plugins_plugin_handler
{
    ixserver_plugins_plugin_handler_process *process;
    ixserver_plugins_plugin_handler_initialize *initialize;
	ixserver_plugins_plugin_handler_destroy *destroy;
}ixserver_plugins_plugin_handler;

/** every plugin shall have and implement this function to be loaded by ixserver 
	@param cb the callback, plugin will call this function set to ask some action from session instance.
	@param ret return value to pass function set of plugin hanlder, session instance will manage a plugin with this function set. 
	@return zero if successful.
*/
typedef int (ixserver_plugin_capability_function)( ixserver_plugins_callbacks *cb, ixserver_plugins_plugin_handler **ret );
#endif //__IXSERVER_PLUGINS_H__
