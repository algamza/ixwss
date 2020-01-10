/**
 * @file IXWebSocketSetting.h
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

#ifndef __IX_WSS_SETTING__
#define __IX_WSS_SETTING__

#include <list>
#include <map>
#include <string>

namespace ixwss {
/**
 * @ingroup IXWSS
 * @{
 */

/**
 * @brief A class that stores settings for running the server.
 */ 
class IXWebSocketSetting 
{
public:
    /**
     * @brief Constructor for IXWebSocketSetting.  
     */
    IXWebSocketSetting();

    /**
     * @brief Destructor for IXWebSocketSetting.  
     */
    ~IXWebSocketSetting();

    /**
     * @brief Add port settings.
	 * @param[in] port The port value.
     * @param[in] protocols Sub protocol list.
     * @return True if the append succeeds; false otherwise.
     */
    bool addPort(int port, std::list<std::string> protocols);

    /**
     * @brief Get the configured port.
	 * @param[out] ports List of ports. 
     */
    void getPorts(std::list<int> &ports);

    /**
     * @brief Get the configured port.
	 * @param[in] port The port value.
     * @param[out] protocols The list to get the subprotocol.
     */
    void getProtocols(int port, std::list<std::string> &protocols);

    /**
     * @brief Sets the server's config path to the file.
	 * @param[in] path server config file path.
     * @return True if the setting succeeds; false otherwise.
     */
    bool setServerConfigPath(const char *path);

private:
    bool setServerConfig(std::string conf_path);

private:
    std::map<int,std::list<std::string>> m_port_protocols;
    std::string m_sever_config_path;
};

/** @} */ // End of doxygen group

}  /* namespace ixwss */

#endif /* __IX_WSS_SETTING__ */
