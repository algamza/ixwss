/** **********************************************************************************************************
	@file 		main.cpp
	
	@date		2018/01/05
	@author		Group(SW_Browser) <gsw_browser@humaxdigital.com>
	@breif		
	
	(c) 2016 Humax Co., Ltd. 
	This program is produced by Humax Co., Ltd. ("Humax") and 
	the proprietary Software of Humax and its licensors. Humax provides you, as an Authorized Licensee, 
	non-assignable, non-transferable and non-exclusive license to use this Software. 
	You acknowledge that this Software contains valuable trade secrets of Humax and by using this Software 
	you agree to the responsibility to take all reasonable efforts to protect the any information 
	you receive from Humax. You are not permitted to duplicate, modify, distribute, sell or lease and 
	reverse engineer or extract the source code of this Software unless you have Humax's written permission 
	to do so.
	If you have no authorized license, discontinue using this Software immediately. 
	
	THE SOFTWARE IS PROVIDED "AS IS" AND HUMAX MAKES NO PROMISES, REPRESENTATIONS OR WARRANTIES, 
	EITHER EXPRESS, IMPLIED OR STATUTORY, OR OTHERWISE, WITH RESPECT TO THE SOFTWARE. 
	IN NO EVENT SHALL HUMAX BE LIABLE FOR LOST PROFITS, REVENUES, OR DATA, FINANCIAL LOSSES OR 
	INDIRECT, SPECIAL, CONSEQUENTIAL, EXEMPLARTY OR PUNITIVE DAMAGES WHATSOEVER RELATING TO YOUR USE OR 
	INABILITY TO USE THE SOFTWARE.
	
	This License is effective until terminated. You may terminate this License at any time 
	by destroying all copies of the Software including all documentation. This License will terminate 
	immediately without notice from Humax to you if you fail to comply with any provision of this License. 
	Upon termination, you must destroy all copies of the Software and all documentation.  
	
	The laws of the Republic of Korea will apply to any disputes arising out of or relating to 
	this Copyright Notice. All claims arising out of or relating to this Copyright Notice will be litigated
	in the Seoul Central District Court, in the Republic of Korea.
*********************************************************************************************************** */


#include "IXWebSocketClient.h"
#include <iostream>
#include <string>
#include <signal.h>

using namespace std;
using namespace ixwss;

class ReceiveData : public IXWebSocketClientListener
{
public:	
	ReceiveData() { }
	virtual ~ReceiveData() { }
    virtual int onReceiveData( unsigned char *buffer, unsigned int length )
    {
		cout << "Receive Data: " << buffer << endl;
		return 0;
    }
};

int main( int argc, char** argv)
{
    if ( argc != 3 ) return 0;

    int port;
    string protocol;
    string str("{\"header\":{\"type\":\"request\",\"module\":\"dial\",\"name\":\"init\"},\"body\":null}"); 

    port = atoi(argv[1]);
    protocol.assign(argv[2]);

	cout << "Received the value -port: " << port << ", protocol: " << protocol << endl;

    IXWebSocketClient *client = new IXWebSocketClient("localhost:9999","localhost",port,protocol.c_str());
    ReceiveData *listener = new ReceiveData();

    client->addEventListener(listener);
    client->connect();
    
    for ( ; ; )
    {
        char c = getchar();
        switch ( c )
        {
            case 'q' :
                client->disconnect();
                break;
            case 'r' :
                client->connect();
                break;
            case 's' :
                client->send(str.c_str(),str.size());
                break;   
            default :
                break;
        }
    }

	client->removeEventListener();
    if ( client ) delete client;
	if ( listener ) delete listener;
    return 0;
}