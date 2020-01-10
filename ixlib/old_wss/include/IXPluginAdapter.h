/**
 * @file IXPluginAdapter.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief The adapter class to connect the plugin.
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

#ifndef __IX_WSS_PLUGINADAPTER__
#define __IX_WSS_PLUGINADAPTER__

#include "ixplugin.h"
#include <pthread.h>
#include <list>

namespace ixwss {

/**
 * @ingroup IXWSS
 * @{
 */

 /**
 * @brief Listener class for plugin adapter.
 */
class IXPluginAdapterListener
{
public:
    /**
     * @brief Sends the buffer passed in by the plugin to the listening class.
     * @param[in] buffer The buffer received from the plugin.
	 * @param[in] length The length of the buffer.
     * @return Pass the state value from the inheriting class.
     */
    virtual int onSendBuffer( unsigned char *buffer, unsigned int length ) = 0;
};

 /**
 * @brief A class for connecting plugins.
 */
class IXPluginAdapter 
{
public:
	/**
     * @brief Constructor for IXPluginAdapter. 
     * @param[in] listener Listeners for plug-in listening.
     */
    IXPluginAdapter(IXPluginAdapterListener* listener);

    /**
     * @brief Destructor for IXPluginAdapter.  
     */
	~IXPluginAdapter();

    /**
     * @brief Try dl open the plugin file in the input path.
	 * @param[in] name Plugin file path.
     * @return True if the plugin file was opened, otherwise false.
     */
	bool openPluginFile(const char *name); 

    /**
     * @brief Pass the buffer received from the client to the plug-in.
     * @param[in] buffer The buffer received from the client.
	 * @param[in] length The length of the buffer.
     * @return true if successful
     */
    bool process( unsigned char *buffer, unsigned int length );

	/**
     * @brief Watch for events received by the plugin.
     * @return true if successful
     */
    bool procPollingCallback( void );

    /**
     * @brief Pass the buffer to the registered listener.
     * @param[in] ctx The listener object.
	 * @param[in] buffer The buffer received from the plugin.
	 * @param[in] length The length of the buffer.
     * @return If it is 1, it fails, but not the status value.
     */
    static int send_buffer( ixserver_plugins_object *ctx, unsigned char *buffer, unsigned int length );
    
    static int send_event( ixserver_plugins_object *ctx, unsigned char *buffer, unsigned int length );
	/**
     * @brief Register a callback for event polling.
     * @param[in] ctx The listener object.
	 * @param[in] cb The callback function handle.
     * @return If it is 1, it fails, but not the status value.
     */
	static int set_polling_cb( ixserver_plugins_object *ctx, ixserver_plugins_polling_callback *cb );

protected:
    int addPollingCallback( ixserver_plugins_polling_callback *cb );
    
private:
    IXPluginAdapterListener* m_listener;
    void* m_dl_handle;
    ixserver_plugin_capability_function *m_capability;
    ixserver_plugins_callbacks m_callback;
    ixserver_plugins_plugin_handler* m_plugin_handler;
    ixserver_plugins_polling_callback* m_polling_callback;
    ixserver_plugins_object m_plugins_obj;
    pthread_mutex_t m_mutex;
};

/** @} */ // End of doxygen group

}  /* namespace ixwss */

#endif //__IX_WSS_PLUGINADAPTER__
