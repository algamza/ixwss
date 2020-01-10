/**
 * @file IxWebSocketVhost.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief This opens the IxServer's port, manages the protocol, and relays the client's connection.
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

#ifndef __IXS_WEBSOCKETVHOST__
#define __IXS_WEBSOCKETVHOST__

#include <list>

namespace ixs {

class IxSession;
class IxConnectionInfo;

/**
 * @ingroup IXS
 * @{
 */

/**
 * @brief Redefinition for reference of lws_vhost.
 */
using IxVhostHandle = void*;

/**
 * @brief Redefinition for reference of lws_protocols.
 */
using IxProtocolsHandle = void*;

/**
 * @brief A class that configurate port for server.
 */ 
class IxWebSocketVhost
{
public:

    /**
    * @brief enum that websocket event
    */ 
    enum class IxWebsocketEvent
    {
        /** no event */
        IX_EVENT_NONE = 0,
        /** An event that occurs when a client connects. */
        IX_EVENT_SESSION_CREATE, 
        /** An event that occurs when the client terminates the connection. */
        IX_EVENT_SESSION_DESTROY,
        /** An event that occurs when a client sends data to a websocket. */
        IX_EVENT_WSS_RECEIVE_DATA,
        /** An event that occurs when the websocket data can be sent to the client. */
        IX_EVENT_WSS_SERVER_WRITEABLE,
        /** An event that occurs when a client sends data to a websocket. */
        IX_EVENT_HTTP_RECEIVE_DATA,
        /** An event that occurs when the http data can be sent to the client. */
        IX_EVENT_HTTP_SERVER_WRITEABLE
    };
public:

    /**
     * @brief Constructor for IxWebSocketVhost.  
     * @param[in] context_handle Reference of lws_context. 
     * @param[in] port The port on the server.
     * @param[in] info A reference to a class for connection infomation.
     */
    IxWebSocketVhost(void *context_handle, int port, IxConnectionInfo *info); 

    /**
     * @brief Destructor for IxWebSocketVhost.  
     */
    ~IxWebSocketVhost();

    /**
     * @brief Start the Vhost.
     * @return True if successful, False if failed.
     */
    bool start();

    /**
     * @brief Stop the Vhost.
     * @return True if successful, False if failed.
     */
    bool stop();

    /**
     * @brief Receive events from the client.
	 * @param[in] event A event that occur on the client.
     * @param[in] handle A reference of lws.
     * @param[in] data The data passed by the client.
     * @param[in] len The size of the data to be passed by the client.
     * @return True if successful, False if failed.
     */
    bool onVhostCallback(IxWebsocketEvent event, void *handle, void *data, unsigned int len);

private:
    std::list<IxSession *> m_sessions;
    void *m_context_handle;
    int m_port;
    IxConnectionInfo *m_connection;
    IxVhostHandle m_handle;
    IxProtocolsHandle m_protocols_handle;
};

/** @} */ // End of doxygen group

}  /* namespace ixs */

#endif /* __IXS_WEBSOCKETVHOST__ */