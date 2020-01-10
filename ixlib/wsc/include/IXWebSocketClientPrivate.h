/**
 * @file IXWebSocketClientPrivate.h
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

#ifndef __IX_WSS_CLIENT_PRIVATE_H__
#define __IX_WSS_CLIENT_PRIVATE_H__

#include <string>
#include <queue>
#include <libwebsockets.h>

namespace ixwss {

class IXWebSocketClientData;

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
 * @brief The definition of the pointer handle of lws_client_connect_via_info.
 */ 
typedef void* LWSConnectionHandle;

class IXWebSocketClientPrivateListener
{
public:
    virtual ~IXWebSocketClientPrivateListener() {}
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
class IXWebSocketClientPrivate 
{
public:
    static void onReceivedData(struct lws *wsi, void *in);
    static void onWriteAble(struct lws *wsi);

    IXWebSocketClientPrivate(const char *name, const char *address, int port, const char *protocol);
    virtual ~IXWebSocketClientPrivate();

    bool connect();
    bool disconnect();

    bool send(const char *data, int dlen);

    bool writeData(const IXWebSocketClientData *data);
    void writeable(struct lws *wsi_in);

    bool addEventListener(IXWebSocketClientPrivateListener *callback);
    void removeEventListener();

private:
    const IXWebSocketClientData* __popWriteQ();
    static void _thread();
    void __destroy_thread();
    bool __create_context();
    bool __destroy_context();

private:
    static LWSContextHandle s_context;
    static STDThreadHandle s_thread;
    static bool s_try_thread_exit;
    static bool s_is_exist_send_data;
    static IXWebSocketClientPrivateListener *s_recv_callback;

    std::string m_host_name;
    std::string m_address;
    int m_port; 
    std::string m_protocol;
    std::queue<const IXWebSocketClientData *>  m_wq;


    /** It need to be changed !!! */
    static IXWebSocketClientPrivate *s_client;
    static LWSConnectionHandle s_wsi;

    lws_protocols m_pcol;
    lws_context_creation_info m_contextinfo;
    lws_client_connect_info m_ccinfo;
};

/** @} */ // End of doxygen group

}  /* namespace ixwss */

#endif /* __IX_WSS_CLIENT_PRIVATE__ */
