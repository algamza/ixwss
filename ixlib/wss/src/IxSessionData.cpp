/**
 * @file IxSessionData.cpp
 * @author Group(SW_Browser) <gsw_browser@humaxdigital.com>
 * @brief 
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

#include "IxSessionData.h"

#include <iostream>
#include <cstring>

using namespace std;

namespace ixs {

IxSessionData::IxSessionData(const unsigned char *data, unsigned int len)
    : m_data(nullptr), m_len(len)
{
    if ( len && data ) 
    {
        m_data = (unsigned char *)malloc(len);
        if ( m_data ) 
        {
            memset(m_data, 0x00, len);
            memcpy(m_data, data, len);
        }
    }
}

IxSessionData::~IxSessionData()
{
    m_len = 0; 
    if ( m_data ) free(m_data); 
    m_data = nullptr;
}

const unsigned char *IxSessionData::getData() const
{
    return const_cast<const unsigned char *>(m_data); 
}

unsigned int IxSessionData::getDataSize() const
{
    return m_len;
}

const unsigned char *IxSessionData::getDataHeader() const
{
    return nullptr;
}

unsigned int IxSessionData::getDataHeaderSize() const
{
    return 0;
}

} /* namespace ixs */
