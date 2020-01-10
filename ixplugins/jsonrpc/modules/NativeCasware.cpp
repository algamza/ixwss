/**
 * @file NativeCasware.cpp
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

#include "NativeCasware.h"
#include <cstdio>
#include <memory>
#include <stdio.h>
#include <cstring>


static caswareListener *g_CaswareListener =NULL;
//static CaswareSoftcellServiceItem *s_pCaswareServiceItem=NULL;
//static CaswareSoftcellProductItem *s_pCaswareSoftcellProductItem=NULL;

#define CONFIG_ENABLE_DUMMY_DATA_FOR_TEST


#ifdef CONFIG_ENABLE_DUMMY_DATA_FOR_TEST
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

CaswareSystemInfo caswareSoftcellSystemInfo={"prj_board 5.0.0",{"DVB Client (PVR DRM)","3.18.4","Sagem_HW05 Official","0040, 0005, 0041","FFFF, 0000, 0000","14550062","0001", "5.0.0", "05.00.0000","123456789"}};
CaswareSmartcard caswareSoftcellSmartCard={"OK",{"D000-4","1022 4502 89 (7)","0006","7.0","02","00","0000","0002","ZAF"}};
CaswareSmartcard caswareSoftcellSmartCardInsertion={"INSERTED",{"I007-4"}};
CaswareSmartcard caswareSoftcellSmartCardError={"MUTE",{"E005-4"}};
CaswareIrdetoSoftcellServiceStatus caswareSoftcellServiceStatus[2]={{"0x0000","EMM","D100-5","D100-5","0x0000","D000-4","0x0000","D100-6","EMM: (030) 0500 - 03 00 00 - 00 40 00"}, \
    {"0x0000","EMM","D100-5","D100-5","0x0000","D000-4","0x0000","D100-6","EMM: (030) 0500 - 03 00 00 - 00 40 00"}};
CaswareFingerPrinting fingerPrintInfo={"New message","20%","10%","rgba(0,0,0,0.2)","rgba(255,255,255,0.8)", 2000, 5};
CaswareMessage casMessage={1,0,32769, "Ekim ay\u0131na ait faturan\u0131z\u0131n...","mail",20000};
CaswarePinReseted casPinReset={"reseted","default"};

void *t_function(void *data);
void lowlayer_casware_registerListener();
void lowlayer_casware_unregisterListener();
#endif




static int _casware_EventListener(int module, int slot, int type, int session, int eventType, void *pvData)
{
	CaswareEvent event = CASWARE_EVENT_MAX;
	void *data = NULL;

#ifdef CONFIG_ENABLE_DUMMY_DATA_FOR_TEST
    switch ( type )
    {
        case 1:
            event = CASWARE_EVENT_SYSTEM_INFO_CHANGE;
            data  = &caswareSoftcellSystemInfo;
            break;
        default:
            break;
    }
#else
    // you need to make the code to use platform resource 
#endif
    if ( g_CaswareListener && data ) g_CaswareListener(event, data);

    return NATIVE_RETURN_OK;
}

NATIVE_RETURN native_casware_OnCreate()
{
#ifdef CONFIG_ENABLE_DUMMY_DATA_FOR_TEST
    // make task for generating events.
    lowlayer_casware_registerListener();

#else
	// you need to make the code to use platform resource 
    
    //refer to native_RecordingSchedulerOnCreate
    //1. make instance function
    //2. make addEventlistener
    //OPL_CasUi_RegisterListener(0, _casware_EventListener);

#endif    
    return NATIVE_RETURN_OK;
}

NATIVE_RETURN native_casware_setListener(caswareListener listener)
{
    g_CaswareListener = (caswareListener *)listener;

    return NATIVE_RETURN_OK;
}

NATIVE_RETURN native_casware_unsetListener()
{
    g_CaswareListener = NULL;

    return NATIVE_RETURN_OK;
}

NATIVE_RETURN native_casware_getProduct(int *itemCount, CaswareSoftcellProductItem **softcellProductItem)
{
#ifdef CONFIG_ENABLE_DUMMY_DATA_FOR_TEST
    static CaswareSoftcellProductItem caswareSoftcellProductItem={"0","0C23",{{"0001","0C23","254"},{"0002","0C23","254"}}};
    *softcellProductItem=&caswareSoftcellProductItem;
	*itemCount=1;
#else
	// you need to make the code to use platform resource 
#endif
    return NATIVE_RETURN_OK;
}


#ifdef CONFIG_ENABLE_DUMMY_DATA_FOR_TEST

pthread_t s_p_thread;
int s_a = 1;

void *t_function(void *data)
{
    int id;
    int i = 1;
    id = *((int *)data);

    while(1)
    {
        printf("temp event thread(%d) generate the event : %d\n", id, i);
        _casware_EventListener(0,0,i,0,0,NULL);

        //i++;
        sleep(15);
    }
    return nullptr;    
}

void lowlayer_casware_registerListener()
{
    int thr_id;

    thr_id = pthread_create(&s_p_thread, NULL, t_function, (void *)&s_a);
    if ( thr_id < 0 )
    {
        perror("thread create error : ");
        exit(0);
    }
}

void lowlayer_casware_unregisterListener()
{
    int status;

    pthread_join(s_p_thread,(void **)&status);
}
#endif


