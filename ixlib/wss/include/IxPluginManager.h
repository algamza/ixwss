/**
 * @file IxPluginManager.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief It manages and relays plug-in files.
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

#ifndef __IXS_PLUGINMANAGER__
#define __IXS_PLUGINMANAGER__

#include "IxThread.h"
#include "IxSingleton.h"
#include <functional>
#include <list>
#include <tuple>
#include <memory>

namespace ixs {

class IxPluginConnector;

/**
 * @ingroup IXS
 * @{
 */

/**
 * @brief A class for managing connections to plug-ins.
 */ 
class IxPluginManager 
    : public IxThread
    , public IxSingleton<IxPluginManager> 
{
public: 

    /**
     * @brief The data class that the plug-in manager passes.
     */ 
    class IxPluginData
    {
    public:

        /**
         * @brief Constructor for IxPluginData.  
         * @param[in] data The plugin data to send.
         * @param[in] len The size of the plugin data to send.
         */
        IxPluginData(unsigned char *data, unsigned int len);

        /**
         * @brief Destructor for IxPluginData.  
         */
        ~IxPluginData();

        /**
         * @brief Get plugin data to send.
         * @return The plugin data to send.
         */
        const unsigned char *getData();  

        /**
         * @brief Gets the size of the plugin data.
         * @return The size of the plugin data to send.
         */
        unsigned int getSize(); 

    private:
        unsigned char *m_data;
        unsigned int m_len;
    };
public:

    /**
     * @brief Friend declaration for Getting the singleton object of IxPluginManager.
     * @return Singleton object of IxPluginManager.
     */
    friend std::shared_ptr<IxPluginManager> IxSingleton<IxPluginManager>::getInstance();
public: 

    /**
     * @brief Destructor for IxPluginManager.  
     */
    virtual ~IxPluginManager(); 

    /**
     * @brief Registers a listener for receiving plug-in data.
	 * @param[in] handle Handle for receiving plugin data.
     * @param[in] callback Callback for receiving plugin data.
     * @param[in] plugin_path The path to the plugin to connect to.
     * @return True if successful, False if failed.
     */
    bool registSession(void *handle, std::function<bool(unsigned char *data, unsigned int len)>
        callback , const char *plugin_path);

    /**
     * @brief Unregisters a listener for receiving plug-in data.
	 * @param[in] handle The handle that you registered to receive plug-in data.
     * @return True if successful, False if failed.
     */
    bool unRegistSession(void *handle); 

    /**
     * @brief Receives plugin data as a return.
	 * @param[in] handle The handle that you registered to receive plug-in data.
     * @param[in] data Data requested to receive plug-in data.
     * @param[in] len Size of data requested to receive plugin data.
     * @return The plugin data value.
     */
    IxPluginData *syncCall(void *handle, const unsigned char *data, unsigned int len);

    /**
     * @brief Receives plugin data as a registered listener.
	 * @param[in] handle The handle that you registered to receive plug-in data.
     * @param[in] data Data requested to receive plug-in data.
     * @param[in] len Size of data requested to receive plugin data.
     * @return True if successful, False if failed.
     */
    bool asyncCall(void *handle, const unsigned char *data, unsigned int len);

private:
    IxPluginManager();
    void run() override;
private:
    std::list<std::tuple<void*, std::function<bool(unsigned char *, unsigned int)>
        , const char *, std::shared_ptr<IxPluginConnector>>> m_plugins;
};

/** @} */ // End of doxygen group

}  /* namespace ixs */

#endif /* __IXS_PLUGINMANAGER__ */