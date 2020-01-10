/**
 * @file IxConnectionInfo.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief A class that manages connection information.
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

#ifndef __IXS_CONNECTIONINFO__
#define __IXS_CONNECTIONINFO__

#include <list>
#include <string>

namespace ixs {

class IxConfigPort; 

/**
 * @ingroup IXS
 * @{
 */

/**
 * @brief A class that manages connection information.
 */ 
class IxConnectionInfo
{
public:
    /**
    * @brief enum that protocol type
    */ 
    enum class IxProtocolType
    {
        /** no protocol type. */
        IX_PROT_TYPE_NONE = 0,
        /** only http protocol type. */
        IX_PROT_TYPE_HTTP,
        /** websocket type with sub-protocol. */
        IX_PROT_TYPE_WSS,
        /** websocket type without sub-protocol. */
        IX_PROT_TYPE_WSS_NO_SUBP,
        /** A type that allows both http and websocket without sub-protocol. */
        IX_PROT_TYPE_HTTP_WSS
    }; 
        
    /**
    * @brief enum that data type
    */ 
    enum class IxDataType
    {
        /** no data type. */
        IX_DATA_TYPE_NONE = 0,
        /** http data type. */
        IX_DATA_TYPE_HTTP,
        /** websocket data type. */
        IX_DATA_TYPE_WSS
    };

public:

    /**
     * @brief Constructor for IxConnectionInfo.  
     * @param[in] config A reference to the class for configuring the port.
     */
    IxConnectionInfo(const IxConfigPort *config);

    /**
     * @brief Destructor for IxConnectionInfo.  
     */
    ~IxConnectionInfo();

    /**
     * @brief The protocol list to be set by the configuration is fetched.
     * @return A list of protocol name
     */
    std::list<const char *> getProtocols();

    /**
     * @brief Get the plugin path corresponding to the protocol.
	 * @param[in] protocol_name The name of the plugin.
     * @param[in] type The type of the data.
     * @return The path to the plugin.
     */
    const char *getPluginPath(const char *protocol_name
        , IxDataType type = IxDataType::IX_DATA_TYPE_NONE );

    /**
     * @brief Get the protocol type by protocol name.
	 * @param[in] protocol_name The name of the protocol.
     * @return The type of the protocol.
     */
    IxProtocolType getProtocolType(const char *protocol_name); 

    /**
     * @brief Get ssl support.
     * @return True if successful, False if failed.
     */
    bool isSupportSSL();

    /**
     * @brief Get server certificate path.
     * @return The server CA certificate. 
     */
    const char *getSSLCerticatePath(); 

    /**
     * @brief Get Root CA certificate path.
     * @return The Root CA certificate. 
     */
    const char *getSSLRootCAPath();

    /**
     * @brief Get the path to the server's private key.
     * @return The path to the server's private key.
     */
    const char *getSSLPrivateKeyPath(); 

private:
    const IxConfigPort *m_config; 
    std::list<std::string> m_protocols;
};

/** @} */ // End of doxygen group

}  /* namespace ixs */

#endif /* __IXS_CONNECTIONINFO__ */