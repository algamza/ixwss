/**
 * @file IxSessionData.h
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief It is the base class used to store data received by the session.
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

#ifndef __IXS_SESSIONDATA__
#define __IXS_SESSIONDATA__

namespace ixs {

/**
 * @ingroup IXS
 * @{
 */

/**
 * @brief Base class for the data used by the session.
 */ 
class IxSessionData
{
public:

    /**
     * @brief Constructor for IxSessionData.  
     * @param[in] data The data processed by the session.
     * @param[in] len The size of the data to be processed by the session.
     */
    IxSessionData(const unsigned char *data, unsigned int len);

    /**
     * @brief Destructor for IxSessionData.  
     */
    virtual ~IxSessionData();

    /**
     * @brief Get data to process in the session.
     * @return The data processed by the session.
     */
    const unsigned char *getData() const; 

    /**
     * @brief Gets the size of the data processed by the session.
     * @return The size of the data to be processed by the session.
     */
    unsigned int getDataSize() const;

    /**
     * @brief Get data header to process in the session. 
     * @return The header data processed by the session.
     */
    virtual const unsigned char *getDataHeader() const;

    /**
     * @brief Gets the size of the header data processed by the session.
     * @return The size of the header data to be processed by the session.
     */
    virtual unsigned int getDataHeaderSize() const;

private:
    IxSessionData(const IxSessionData &obj) = delete;
private:
    unsigned char *m_data; 
    unsigned int m_len; 
};

/** @} */ // End of doxygen group

}  /* namespace ixs */

#endif /* __IXS_SESSIONDATA__ */