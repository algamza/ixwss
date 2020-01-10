/**
 * @file IXWebSocketServer.h
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

/**
 * @mainpage Interface eXtension Web Socket Server (IXWSS)

 * @section intro Introduction
 * This is the Interface eXtension Web Socket Server (IXWSS).

 * IX of IXWSS stands for Interface Extension, and WSS stands for WebSocket Server. \n
 * In other words, IXWSS can be understood as a component that can easily extend the interface between client and server based on WebSocket communication protocol. \n 
 * In a client / server architecture, a communication protocol is basically a standard method, but various data formats may be required for data to be transmitted. \n
 * IXWSS provides an environment that can be developed using individual plugins so that users can directly process data in various formats. \n

 * All APIs in IXWSS are defined in the ixwss namespace. \n
  
 * @note All code examples in this documentation assume that this namespace is used in the code scope:
   
 * @code
 * using namespace ixwss;
 * @endcode

 * For more documentation see the <a href="modules.html">modules</a> list. 
*/

/**
 * @defgroup IXWSS Interface Extension Websocket Server SDK
 * @brief Interface Extension Websocket Server SDK. \n
 * @{
 * @}
 */

#ifndef __IX_WSS_SERVER_H__
#define __IX_WSS_SERVER_H__

namespace ixwss {
class IXWebSocketServerPrivate;
};

namespace ixwss {

/**
 * @defgroup SERVER server
 * @brief IX websocket server. \n
 * 
 * @ingroup IXWSS
 * @{
 */

/**
 * @brief Web Socket server class
 */ 
class IXWebSocketServer 
{
public:
	/**
     * @brief Constructor for IXWebSocketServer. 
     * @param[in] path Configuration folder path. \n
     * And ixwss.conf file and ixplugins.conf file must be existed in that path to start websocket server.
     *
     * @note 
     * Configuration folder should have two files. \n
     * 1. ixwss.conf file : The information about websocketserver's setting. \n
     * 2. ixplugins.conf file : The information about plugin setting. \n
     *
     * @note
     * Here is example of ixwss.conf file \n
     * - Set the port number and sub-protocol list that websocketserver has to enable \n
     * -- field name : map_port_protocols \n
     * -- description : 
     * if you don't want to use sub-protocol about some port , please refer to port 9997 in the example below \n 
     * -- sample > 
     * @code 
     *  map_port_protocols=9999:jsonrpc,simple
     *  map_port_protocols=9998:jsonrpc
     *  map_port_protocols=9997
     * @endcode
     *
     * @note 
     * Here is example of ixplugins.conf file \n
     * - Set the path of ixplugin library folder \n
     * -- field name : plugin_path \n
     * -- description : 
     * if you don't want to use sub-protocol about some port , please refer to port 9997 in the example below \n 
     * -- sample > 
     * @code 
     *  plugin_path=/usr/browser/unittest/ixplugins/bin
     * @endcode
     * - Set the port numerber and sub-protocol name and ixplugin library file name that ixwss has to load when the web client require. \n
     * -- field name : map_port_protocol_plugin \n
     * -- sample > 
     * @code 
     *  map_port_protocol_plugin=9999:simple:libixplugin_simple.so
     *  map_port_protocol_plugin=9999:jsonrpc:libixplugin_jsonrpc.so 
     *  map_port_protocol_plugin=9998:jsonrpc:libixplugin_jsonrpc.so
     *  map_port_protocol_plugin=9997::libixplugin_simple.so
     * @endcode
     */

    IXWebSocketServer(const char *path);

    /**
     * @brief Destructor for IXWebSocketServer.  
     */
    virtual ~IXWebSocketServer();

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
    IXWebSocketServerPrivate *m_server_private;
};

/** @} */ // End of doxygen group

}  /* namespace ixwss */

#endif /* __IX_WSS_SERVER__ */