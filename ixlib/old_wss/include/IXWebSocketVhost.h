/**
 * @file IXWebSocketVhost.h
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

#ifndef __IX_WS_VHOST__
#define __IX_WS_VHOST__

#include <list>
#include "IXWebSocketSession.h"
#include "libwebsockets.h"

namespace ixwss {
/**
 * @ingroup IXWSS
 * @{
 */
typedef struct ixs_protocols {
    const unsigned int MAX_PROTOCOL_COUNT = 10;
    unsigned int protocol_count;
    lws_protocols protocols[10];
    std::string plugin_files[10]; 
} protocols_t;

 /** 
 * @brief Definition of the pointer handle of lws_vhost. 
 */
typedef void* LWSVhostHandle;

/** 
 * @brief A definition of the handle of an internally managed vhost class.
 */
typedef void* LXSVhostHandle;

/** 
 * @brief The definition of pointer handle of protocols_t. 
 */
typedef void* LXSProtocolHandle;


class IXWebSocketVhost
{
public:
    IXWebSocketVhost(LWSVhostHandle handle, LXSProtocolHandle protocol);
    ~IXWebSocketVhost();
    LWSVhostHandle getHandle();
    int getPort();
    void setPort(int port);
    bool createSession(LWSSessionHandle handle, const char *protocol);
    bool destroySession(LWSSessionHandle handle);
    bool receivedData(LWSSessionHandle handle, void *data, int len);
    bool writeAble(LWSSessionHandle handle);
    IXWebSocketSession *findSession(LWSSessionHandle handle);
private:
    void __destorySessions(); 
private:
    std::list<IXWebSocketSession *> m_sessions;
    LWSVhostHandle m_handle;
    LXSProtocolHandle m_protocol_handle; 
    int m_port;
};

/** @} */ // End of doxygen group

}  /* namespace ixwss */

#endif /* __IX_WS_VHOST__ */