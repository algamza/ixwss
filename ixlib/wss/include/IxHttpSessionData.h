/**
 * @file IxHttpSessionData.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief It is used to store the http data received from the client.
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

#ifndef __IXS_HTTPSESSIONDATA__
#define __IXS_HTTPSESSIONDATA__

#include "IxSessionData.h"

namespace ixs {

/**
 * @ingroup IXS
 * @{
 */

/**
 * @brief A class for the management of data for http protocol.
 */ 
class IxHttpSessionData : public IxSessionData
{
public:

    /**
     * @brief A class that contains the header information of the data for the http protocol.
     */ 
    class IxHttpSessionDataHeader
    {
    public:

        /**
         * @brief Constructor for IxHttpSessionDataHeader.  
         * @param[in] handle A reference to lws of the websocket library.
         * @param[in] constents_size The size of the data.
         */
        IxHttpSessionDataHeader(void *handle, unsigned int constents_size);

        /**
         * @brief Destructor for IxHttpSessionDataHeader.  
         */
        ~IxHttpSessionDataHeader();

        /**
         * @brief Create header data of the data to be sent.
         * @return True if successful, False if failed.
         */
        bool makeHeader();

        /**
         * @brief Get header data of the data to be sent.
         * @return Header data of the data to be sent.
         */
        const unsigned char *getHeader();

        /**
         * @brief Get the header data size of the data to be sent.
         * @return Size of the header data of the data to be sent.
         */
        unsigned int getHeaderSize();

    private:
        void *m_handle;
        unsigned char *m_header;
        unsigned int m_header_length;
        unsigned int m_contents_size; 
    };
public:

    /**
     * @brief Constructor for IxHttpSessionData.  
     * @param[in] handle A reference to lws of the websocket library.
     * @param[in] data The data to be sent.
     * @param[in] len The size of the data to send.
     */
    IxHttpSessionData(void *handle, const unsigned char *data, unsigned int len);
    
    /**
     * @brief Destructor for IxHttpSessionData.  
     */
    virtual ~IxHttpSessionData();

    /**
     * @brief Get header data of the data to be sent.
     * @return Header data of the data to be sent.
     */
    const unsigned char *getDataHeader() const override;

    /**
     * @brief Get the header data size of the data to be sent.
     * @return Size of the header data of the data to be sent.
     */
    unsigned int getDataHeaderSize() const override;

private:
    IxHttpSessionDataHeader *m_http_header;
};

/** @} */ // End of doxygen group

}  /* namespace ixs */

#endif /* __IXS_HTTPSESSIONDATA__ */