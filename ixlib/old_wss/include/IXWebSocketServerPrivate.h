/**
 * @file IXWebSocketServerPrivate.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief Server based on WebSocket
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

#ifndef __IX_WSS_SERVER_PRIVATE_H__
#define __IX_WSS_SERVER_PRIVATE_H__

#include <list>
#include <string>
#include <IXWebSocketSession.h>
#include <IXWebSocketVhost.h>
#include <libwebsockets.h>

namespace ixwss {
class IXWebSocketSetting;
class IXWebSocketPluginSetting;
};

namespace ixwss {

/**
 * @ingroup IXWSS
 * @{
 */

/**
 * @brief The definition of the pointer handle of lws_context.
 */
typedef void* LWSContextHandle;

/**
 * @brief The definition of the pointer handle of a std::thread.
 */ 
typedef void* STDThreadHandle;

/**
 * @brief Web Socket server class
 */ 
class IXWebSocketServerPrivate 
{
public:
    /**
     * @brief It is called from within the server when the session is created.
	 * @param[in] handle The handle of the session.
     */
    static void onCreateSession(LWSSessionHandle handle); 

    /**
     * @brief It is called from within the server when the session is destroyed.
	 * @param[in] handle The handle of the session.
     */
    static void onDestroySession(LWSSessionHandle handle); 

    /**
     * @brief It is called from within the server when the session received data.
	 * @param[in] handle The handle of the session.
     * @param[in] data Data received from the client.
     * @param[in] len The length of the data.
     */
    static void onReceivedData(LWSSessionHandle handle, void *data, int len);

    /**
     * @brief It is called from within the server when When the server 
     * is able to pass data to the client.
	 * @param[in] handle The handle of the session.
     */
    static void onWriteAble(LWSSessionHandle handle); 

	/**
     * @brief Constructor for IXWebSocketServerPrivate. 
     * @param[in] setting Configuration folder path.
     * And ixwss.conf & ixplugins.conf file must be existed in configuration folder path to start websocket server.
     */
    IXWebSocketServerPrivate(const char *path);

    /**
     * @brief Destructor for IXWebSocketServerPrivate.  
     */
    virtual ~IXWebSocketServerPrivate();

    /**
     * @brief Start the server with the settings.
     * @return True means the server is started; false means failure.
     */
    bool start();

    /**
     * @brief Stop the current server.
     * @return True means the server is stopped; false means failure.
     */
    bool stop();

private:
    static LXSVhostHandle __getVhost(LWSSessionHandle handle);

private:
    static void _thread();
    void __destroy_thread();
    bool __create_context();
    bool __destroy_context();
    bool __create_vhosts();
    bool __destroy_vhosts();
    LXSProtocolHandle __create_protocol(std::list<std::string> &protocols);
    bool __set_plugin_protocol_to_match(int port, LXSProtocolHandle protocol);
    
private:
    static LWSContextHandle s_context;
    static STDThreadHandle s_thread;
    static bool s_try_thread_exit; 
    static std::list<IXWebSocketVhost *> s_vhosts;
    IXWebSocketSetting *m_setting;
    IXWebSocketPluginSetting *m_plugin_setting;
};

/** @} */ // End of doxygen group

}  /* namespace ixwss */

#endif /* __IX_WSS_SERVER__ */