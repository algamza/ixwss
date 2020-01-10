/**
 * @file IXWebSocketSession.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief Session to manage the client based on WebSocket
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

#ifndef __IX_WSS_SESSION__
#define __IX_WSS_SESSION__

#include "IXThread.h"
#include "IXPluginAdapter.h"
#include <string>
#include <queue>

namespace ixwss {

class IXWebSocketData;

/**
 * @ingroup IXWSS
 * @{
 */

/** 
 * @brief Define the handle of lws pointer 
 */
typedef void* LWSSessionHandle;

/**
 * @brief Web Socket session class
 */ 
class IXWebSocketSession : public IXThread, public IXPluginAdapterListener
{
private:
    typedef enum {
        IXWSSESSION_STATE_IDLE		= 0,
        IXWSSESSION_STATE_PROC_RECV_Q,
        IXWSSESSION_STATE_PROC_SEND_Q,
    } eIXWSSessionState;

public:
    /**
     * @brief Constructor for IXWebSocketSession
     * @param[in] handle A unique handle for the session. 
     */
    IXWebSocketSession(LWSSessionHandle handle);

    /**
     * @brief Constructor for IXWebSocketSession
     * @param[in] handle A unique handle for the session. 
     * @param[in] port The port of the session.
     * @param[in] protocol The protocol name of the session.
     */
    IXWebSocketSession(LWSSessionHandle handle, int port, const char *protocol);

    /**
     * @brief Destructor for IXWebSocketSession
     */
    virtual ~IXWebSocketSession();

    /**
     * @brief Start a session.
     * @return If true, the session was started; if false, it failed.
     */ 
    bool start();

    /**
     * @brief Get the unique handle of the session. 
     * @return The handle of the session. 
     */
    const LWSSessionHandle getHandle();
    
    /**
     * @brief Gets a protocol name used by the session. 
     * @return A protocol name. 
     */
    const char *getProtocol();
    
    /**
     * @brief Gets the peername used by the session. 
     * @return A peer name. 
     */    
    std::string getPeerName();
    
    /**
     * @brief When data is received from the server, it puts data in the queue.\n
     *       This should only be called on the server. 
     * @param[in] data A websocket data object. 
     * @return If true, the data was pushed in queue; if false, the data was not pushed. 
     */ 
    bool receivedData(const IXWebSocketData *data);

    /**
     * @brief Writes data to the client to which the session is connected. 
     * @param[in] data A websocket data object. 
     * @return If true, the data was sent; if false, the data was not sent. 
     */    
    bool writeData(const IXWebSocketData *data); 
    
    /**
     * @brief Called when the server is ready to write. 
     */    
    void writeable();

    /**
     * @brief Sets the plugin file path.
     * @param[in] plugin The path to the plugin file. 
     */  
    void setPluginFile(const char *plugin);
    
    /**
     * @brief Called when data is received from the server. 
     * @param[in] data websocket data object. 
     */    
    virtual void onReceivedData(const IXWebSocketData *data);

    /**
     * @brief Receives an event buffer from the registered plug-in adapter.
     * @param[in] buffer Data received by the plugin.
     * @param[in] length The length of the buffer.
     */
    virtual int onSendBuffer( unsigned char *buffer, unsigned int length );

protected:
    virtual void *run();

private:
    const IXWebSocketData *__popReadQ();
    const IXWebSocketData *__popWriteQ();
    void __stateIdle();
    void __stateProcReceivedQueue();
    void __stateProcSendQueue();

private:
    IXPluginAdapter* m_plugin_adapter;
    LWSSessionHandle m_handle;
    std::string m_protocol;
    eIXWSSessionState m_state;
    bool m_try_thread_exit;
    std::queue<const IXWebSocketData *>  m_rq;
    std::queue<const IXWebSocketData *>  m_wq;
    int m_port;
    std::string m_plugin_file;
};

/** @} */ // End of doxygen group

}  /* namespace ixwss */

#endif /* __IX_WSS_SESSION__ */