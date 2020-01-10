/**
 * @file IxConfigProtocol.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief It is used to store protocol configuration information of IxServer.
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

#ifndef __IXS_CONFIGPROTOCOL__
#define __IXS_CONFIGPROTOCOL__

#include <string>

namespace ixs {

/**
 * @ingroup IXS
 * @{
 */

/**
 * @brief A class that configurate protocol for port of server.
 */ 
class IxConfigProtocol
{
public:

    /**
    * @brief enum that protocol type
    */ 
    enum class IxConfigProtocolType
    {
        /** no protocol type. */
        PROTOCOL_TYPE_NONE = 0,
        /** http protocol type. */
        PROTOCOL_TYPE_HTTP,
        /** https protocol type. */
        PROTOCOL_TYPE_HTTPS,
        /** websocket protocol type. */
        PROTOCOL_TYPE_WS,
        /** websocket ssl protocol type. */
        PROTOCOL_TYPE_WSS
    };
public:

    /**
     * @brief Constructor for IxConfigPort.  
     * @param[in] type Protocol type.
     * @param[in] protocol Sub-protocol value. If not, enter a null value.
     * @param[in] plugin The path to the plugin to configure.
     */
    IxConfigProtocol(IxConfigProtocolType type, const char *protocol, const char *plugin);

    /**
     * @brief Get the protocol type.
     * @return Protocol type.
     */
    IxConfigProtocolType getProtocolType() const;

    /**
     * @brief Get the sub-protocol value. 
     * @return The sub-protocol name.
     */
    const char *getSubProtocol() const; 

    /**
     * @brief Get the plugin path for the protocol.
     * @return The plugin path for the protocol.
     */
    const char *getPluginPath() const;

private:
    IxConfigProtocolType m_type; 
    std::string m_protocol; 
    std::string m_plugin; 
};

/** @} */ // End of doxygen group

}  /* namespace ixs */

#endif /* __IXS_CONFIGPROTOCOL__ */