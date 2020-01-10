/** **********************************************************************************************************
	@file 		main.cpp
	
	@date		2016/07/20
	@author		Humax SW Group
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


#include "IXWebSocketServer.h"
#include <iostream>

using namespace std;
using namespace ixwss;

int main( int argc, char** argv)
{
	IXWebSocketServer *ixserver = new IXWebSocketServer("../ixconf");
	if ( !ixserver )
	{
		cout << "error : I can't make the ixserver." << endl;
		return -1;
	}

	if ( !ixserver->start() )
	{
		cout << "error : I can't start the ixserver." << endl;
		if ( ixserver ) delete ixserver;
		return -1;
	}

    for ( ;; )
	{
		char c = getchar();
        switch(c)
        {
            case '1':
				{
					ixserver->stop();
				}
				break;
            case '2': 
                {
					ixserver->start();
				}
				break;
            default: 
                break;
        }
		//IXServerThread::sleep(1000);//IXServerThread::sleep(1000);
	}

	if ( ixserver ) delete ixserver;
    return 0;   
}