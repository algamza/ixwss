/**
 * @file IXWebSocketClient.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief Client based on WebSocket
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

#ifndef __IX_WSS_CLIENT_H__
#define __IX_WSS_CLIENT_H__

namespace ixwss {
class IXWebSocketClientPrivate;
};

namespace ixwss {

/**
 * @defgroup Client client
 * @brief IX websocket client. \n
 * 
 * @ingroup IXWSS
 * @{
 */

/**
 * @brief Web Socket client Listener class
 */ 
class IXWebSocketClientListener
{
public:
    /**
     * @brief Data from the server to the client.
     * @param[in] buffer The buffer received from the server.
	 * @param[in] length The length of the buffer.
     * @return Pass the state value from the inheriting class.
     */
    virtual int onReceiveData( unsigned char *buffer, unsigned int length ) = 0;
};

/**
 * @brief Web Socket client class
 */ 
class IXWebSocketClient 
{
public:
    /**
     * @brief Constructor for IXWebSocketClient. 
     * @param[in] name The name of host.
     * @param[in] address The Ip address.
     * @param[in] port The port number.
     * @param[in] protocol The name of protocol.
     */
    IXWebSocketClient(const char *name, const char *address, int port, const char *protocol);

    /**
     * @brief Destructor for IXWebSocketClient.  
     */
    virtual ~IXWebSocketClient();

    /**
     * @brief Connect with the server.
     * @return True means the connection with ther server is successful; false means failure.
     */
    bool connect();
    
    /**
     * @brief Disconnect with the server.
     * @return True means the result is successful; false means failure.
     */
    bool disconnect();

    /**
     * @brief Send the data to the server.
     * @param[in] data The data you want to send to the server.
     * @param[in] dlen The length of data.
     * @return True means the result is successful; false means failure.
     */
    bool send(const char *data, int dlen);

    /**
     * @brief Register the Listener.
     * @note It can register single Listener.
     * @param[in] callback the Listener callback.
     * @return True means the result is successful; false means failure.
     */
    bool addEventListener(IXWebSocketClientListener *callback);

    /**
     * @brief Unegister the Listener.
     * @note It can unregister single Listener.
     */
    void removeEventListener();

private:
    IXWebSocketClientPrivate *m_client_private;
};

/** @} */ // End of doxygen group

}  /* namespace ixwss */

#endif /* __IX_WSS_CLIENT__ */
