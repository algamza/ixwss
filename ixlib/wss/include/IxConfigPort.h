/**
 * @file IxConfigPort.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief It is used to store the IxServer's port configuration information.
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

#ifndef __IXS_CONGIGPORT__
#define __IXS_CONGIGPORT__

#include <list>
#include <string>

namespace ixs {

class IxConfigProtocol; 

/**
 * @ingroup IXS
 * @{
 */

/**
 * @brief A class that configurate port for server.
 */ 
class IxConfigPort
{
public:

    /**
     * @brief A class with SSL information.
     */ 
    class IxConfigSSL 
    {
    public:

        /**
         * @brief Constructor for IxConfigSSL.  
         * @param[in] key The path to the server's private key.
         * @param[in] ca The path to the RootCA certificate.
         * @param[in] cert The path to the server's cerificate.
         */
        IxConfigSSL(const char *key, const char *ca, const char *cert);

        /**
         * @brief Get the server's private key.
         * @return The server's private key.
         */
        const char *getPrivateKey() const; 

        /**
         * @brief Get the server's cerificate.
         * @return The server's cerificate.
         */
        const char *getCertificate() const;

        /**
         * @brief Get the RootCA certificate.
         * @return The RootCA certificate.
         */
        const char *getRootCA() const; 

    private:
        std::string m_private_key;
        std::string m_certificate;
        std::string m_root_ca;
    };
public:

    /**
     * @brief Constructor for IxConfigPort.  
     * @param[in] port The port on the server.
     */
    explicit IxConfigPort(int port);

    /**
     * @brief Destructor for IxConfigPort.  
     */
    ~IxConfigPort();

    /**
     * @brief Get the port.
     * @return Port number. 0 if not.
     */
    int getPort() const; 

    /**
     * @brief Configure the ssl configuration.
     * @param[in] ssl Pointer value of ssl configuration.
     * @return True if successful, False if failed.
     */
    bool setConfigSSL(IxConfigSSL *ssl);

    /**
     * @brief Get ssl support.
     * @return True if successful, False if failed.
     */
    bool isSupportSSL() const;

    /**
     * @brief Get ssl configuration value.
     * @return The value for the ssl configuration.
     */
    const IxConfigSSL *getConfigSSL() const; 

    /**
     * @brief Add an object for protocol configuration.
	 * @param[in] protocol A reference to an object for protocol configuration.
     * @return True if successful, False if failed.
     */
    bool addConfigProtocol(IxConfigProtocol *protocol); 

    /**
     * @brief Get a reference to an object for protocol configuration.
	 * @param[in] index The index value of the object reference for the protocol configuration.
     * @return A reference to an object for protocol configuration.
     */
    const IxConfigProtocol *getConfigProtocol(int index) const; 

    /**
     * @brief Get the number of object references for the protocol configuration.
     * @return The number of object references for protocol configuration.
     */
    int getConfigProtocolSize() const; 

private:
    int m_port;
    std::list<IxConfigProtocol *> m_protocols;
    bool is_support_ssl; 
    IxConfigSSL *m_ssl; 
};

/** @} */ // End of doxygen group

}  /* namespace ixs */

#endif /* __IXS_CONGIGPORT__ */