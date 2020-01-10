/**
 * @file JsonRpcParser.cpp
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

#include "JsonRpcParser.h"
#include <cstring>


JsonRpcParser::JsonRpcParser()
    : m_class("")
    , m_function("")
{

}

JsonRpcParser::~JsonRpcParser()
{

}

bool JsonRpcParser::import(unsigned char *buffer)
{
    JsonObject methodObj;
    string fullMethod;
    char fullMethodChar[64];
    int whileCount=0;

    m_request = JsonObject::create((char *)buffer, strlen((char *)buffer));

    //parse method, get m_class, m_function
    methodObj = m_request.get(MSG_KEY_STRING[MSG_METHOD]);

    //result fullMethod Ex:"casware.addServiceListenerOn", "avio.HDMI.getAudioMode"
    fullMethod = string(methodObj);   
    sprintf(fullMethodChar,"%s",fullMethod.c_str());

    if ( strlen(fullMethodChar)==0 )
    {
        return false; 
    }

    char *ptr = strtok(fullMethodChar, "."); 

    m_class=string(ptr);
    ptr = strtok( NULL, ".");
    while ( ptr != NULL ) 
    {
       m_function=string(ptr);
       whileCount++;
       ptr = strtok( NULL, ".");
    }

    if ( whileCount==0 )
    {
        m_class=string("others");
        m_function=string(fullMethodChar);
    }

    m_param = m_request.get(MSG_KEY_STRING[MSG_PARAMS]);
    m_id = m_request.get(MSG_KEY_STRING[MSG_ID]);

    return true;
}

std::string JsonRpcParser::getClass()
{
    return m_class;
}

std::string JsonRpcParser::getFunction()
{
    return m_function;
}

JsonObject JsonRpcParser::getParam()
{
    return m_param;
}

JsonObject JsonRpcParser::getId()
{   
    return m_id;
}
