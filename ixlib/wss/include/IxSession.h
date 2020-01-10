/**
 * @file IxSession.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief This is the session used to maintain the connection of the client connecting to the IxServer.
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

#ifndef __IXS_SESSION__
#define __IXS_SESSION__


#include <queue>

namespace ixs {

class IxSessionData;

/**
 * @ingroup IXS
 * @{
 */

/**
 * @brief Redefinition for reference of lws.
 */
using IxSessionHandle = void*;

/**
 * @brief Session class for client connection and data transfer.
 */ 
class IxSession
{
public:

    /**
    * @brief enum that session type
    */ 
    enum class IxSessionType
    {
        /** no session type. */
        IX_SESSION_TYPE_NONE = 0,
        /** http session type. */
        IX_SESSION_TYPE_HTTP,
        /** websocket session type. */
        IX_SESSION_TYPE_WSS
    }; 
public:

    /**
     * @brief Constructor for IxSession.  
     * @param[in] handle A reference to lws of the websocket library.
     */
    explicit IxSession(IxSessionHandle handle);

    /**
     * @brief Destructor for IxSession.  
     */
    virtual ~IxSession(); 

    /**
     * @brief Check whether the session is started.
     * @return True if successful, False if failed.
     */
    bool isStarted(); 

    /**
     * @brief Start a session.
	 * @param[in] type The type of session.
     * @param[in] plugin_path The path to the plugin to connect to.
     * @return True if successful, False if failed.
     */
    bool start(IxSessionType type, const char *plugin_path);

    /**
     * @brief Gets a reference to lws of the websocket library.
     * @return A reference to lws of the websocket library.
     */
    IxSessionHandle getHandle();

    /**
     * @brief Request data from the plug-in.
	 * @param[in] data The data to request from the plug-in.
     * @param[in] len The size of the data to request for the plugin.
     * @return True if successful, False if failed.
     */
    bool pushRequestData(const unsigned char *data, unsigned int len);

    /**
     * @brief Process any data received from the plug-in.
     * @return True if successful, False if failed.
     */
    bool popResponseData(); 

    /**
     * @brief Check that the session is available.
     * @return True if successful, False if failed.
     */
    bool isValid(); 

    /**
     * @brief Sets whether the session is available.
	 * @param[in] valid The value of whether the session is enabled.
     * @return True if successful, False if failed.
     */
    bool setValid(bool valid); 
    
private:
    IxSessionHandle m_handle;
    IxSessionType m_type;
    std::queue<const IxSessionData *> m_data;
    bool m_is_started;
    bool m_is_valid;
};

/** @} */ // End of doxygen group

}  /* namespace ixs */

#endif /* __IXS_SESSION__ */