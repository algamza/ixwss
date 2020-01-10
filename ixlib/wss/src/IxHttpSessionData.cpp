/**
 * @file IxHttpSessionData.cpp
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

#include "IxHttpSessionData.h"

#include <libwebsockets.h>
#include <stdlib.h>
#include <string.h>

using namespace std;

namespace ixs {

IxHttpSessionData::IxHttpSessionDataHeader::IxHttpSessionDataHeader(
    void *handle ,unsigned int constents_size)
    : m_handle(handle)
    , m_header(nullptr)
    , m_header_length(0)
    , m_contents_size(constents_size)
{

}

IxHttpSessionData::IxHttpSessionDataHeader::~IxHttpSessionDataHeader()
{
    if ( m_header ) free(m_header);
    m_header = nullptr;
}

bool IxHttpSessionData::IxHttpSessionDataHeader::makeHeader()
{
    unsigned int data_len = m_contents_size;
    if ( !data_len || !m_handle ) return false;

    lws *wsi = static_cast<lws *>(m_handle); 
    unsigned char buffer[4096];
    memset(buffer, 0x00, sizeof(buffer)); 
    unsigned char *p = nullptr, *end = nullptr;
    p = buffer;
    end = p + sizeof(buffer);

    if ( lws_add_http_header_status(wsi, HTTP_STATUS_OK, &p, end) ) return false;
    if ( lws_add_http_header_by_token(wsi, WSI_TOKEN_HTTP_SERVER, 
        (unsigned char *)"libwebsockets", 13, &p, end) ) return false;
    if ( lws_add_http_header_by_token(wsi, WSI_TOKEN_HTTP_CONTENT_TYPE,
        (unsigned char *)"text/plain", 10, &p, end) ) return false;
    if ( lws_add_http_header_content_length(wsi, data_len, &p, end) ) return false;
    if ( lws_finalize_http_header(wsi, &p, end) ) return false;
    *p = '\0';

    if ( m_header ) free(m_header); 
    m_header = nullptr;

    m_header_length = p - buffer;
    m_header = (unsigned char *)malloc(m_header_length);
    if ( !m_header ) return false;
    memset(m_header, 0x00, m_header_length); 
    memcpy(m_header, buffer, m_header_length); 

    return true;
}

const unsigned char *IxHttpSessionData::IxHttpSessionDataHeader::getHeader()
{
    return const_cast<const unsigned char *>(m_header);
}

unsigned int IxHttpSessionData::IxHttpSessionDataHeader::getHeaderSize()
{
    return m_header_length;
}

IxHttpSessionData::IxHttpSessionData(void *handle, const unsigned char *data, unsigned int len)
    : IxSessionData(data, len), m_http_header(nullptr)
{
    m_http_header = new IxHttpSessionDataHeader(handle, len);
    if ( m_http_header ) m_http_header->makeHeader(); 
}

IxHttpSessionData::~IxHttpSessionData()
{
    delete m_http_header;
}

const unsigned char *IxHttpSessionData::getDataHeader() const
{
    if ( !m_http_header ) return nullptr; 
    return m_http_header->getHeader();
}

unsigned int IxHttpSessionData::getDataHeaderSize() const
{
    if ( !m_http_header ) return 0; 
    return m_http_header->getHeaderSize();
   
}

} /* namespace ixs */
