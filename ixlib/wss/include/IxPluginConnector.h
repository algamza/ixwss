/**
 * @file IxPluginConnector.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief It is used to load plug-in files and exchange data.
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

#ifndef __IXS_PLUGINCONNECTOR__
#define __IXS_PLUGINCONNECTOR__

#include "ixplugin.h"
#include <pthread.h>
#include <list>
#include <functional>

namespace ixs {

/**
 * @ingroup IXS
 * @{
 */

/**
 * @brief A class for the management of data for http protocol.
 */ 
class IxPluginConnector
{
public:

    /**
     * @brief Constructor for IxPluginConnector.  
     */
    IxPluginConnector(); 

    /**
     * @brief Destructor for IxPluginConnector.  
     */
    ~IxPluginConnector();

    /**
     * @brief Initialize the connector.
	 * @param[in] file_path The path to the plugin file.
     * @return True if successful, False if failed.
     */
    bool initialize(const char *file_path);

    /**
     * @brief Registers a listener for receiving a response to the request.
	 * @param[in] callback A listener for receiving a response to a request.
     * @return True if successful, False if failed.
     */
    bool addResponseListener(std::function<void(void *session_handle
        , unsigned char *data, unsigned int len)> callback);

    /**
     * @brief Registers a listener for receiving a plugin event. 
	 * @param[in] callback A listener for receiving a plugin event. 
     * @return True if successful, False if failed.
     */    
    bool addEventListener(std::function<void(void *connector_handle
        , unsigned char *data, unsigned int len)> callback);

    /**
     * @brief Request data from the plug-in.
	 * @param[in] handle The handle registered in the listener.
     * @param[in] data Requesting data.
     * @param[in] len The size of the requested data.
     * @return True if successful, False if failed.
     */
    bool process(void *handle, unsigned char *data, unsigned int len);

    /**
     * @brief Poll the plug-in event. It should be called periodically to receive events.
     * @return True if successful, False if failed.
     */
    bool procPollingCallback(void);

private:
    void* m_dl_handle;
    ixserver_plugin_capability_function *m_capability;
    ixserver_plugins_callbacks m_callback;
    ixserver_plugins_plugin_handler* m_plugin_handler;
    ixserver_plugins_polling_callback* m_polling_callback;
    ixserver_plugins_object m_plugins_obj;
    pthread_mutex_t m_mutex;
    std::function<void(void *session_handle, unsigned char *data
        , unsigned int len)> m_response_callback;
    std::function<void(void *connector_handle, unsigned char *data
        , unsigned int len)> m_event_callback;
};

/** @} */ // End of doxygen group

}  /* namespace ixs */

#endif /* __IXS_PLUGINCONNECTOR__ */