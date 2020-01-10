/** **********************************************************************************************************
	@file 		NativeCasware.h

	@date		2016/07/31
	@author		browser team ( psw_browser@humaxdigital.com )
	@breif

	(c) 2011-2016 Humax Co., Ltd.
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
#ifndef __NATIVECASWARE
#define __NATIVECASWARE

#include "NativeTypes.h"
typedef enum{
	CASWARE_EVENT_SYSTEM_INFO_CHANGE=0,
	CASWARE_EVENT_SMARTCARD_CHANGE,
	CASWARE_EVENT_SMARTCARD_INSERTION,
	CASWARE_EVENT_SMARTCARD_ERROR,
	CASWARE_EVENT_SMARTCARD_REMOVAL,
	CASWARE_EVENT_SERVICE_CHANGE,
	CASWARE_EVENT_FINGER_PRINTING,
	CASWARE_EVENT_MESSAGE,
	CASWARE_EVENT_PIN_RESET,
	CASWARE_EVENT_MAX
}CaswareEvent;
typedef void (caswareListener)(CaswareEvent evt_type, void* data); //need to sync params to Casware::onCaswareEvent

typedef struct{
	char productId[20];
	char startDate[20];
	char duration[20];
}CaswareProductItem;


typedef struct{
	char sector[20];
	char dateCode[20];
	CaswareProductItem productItems[20];
}CaswareSoftcellProductItem;

typedef struct{
	char handle[20];
	char name[20];
	char instance[20];
	char description[20];
}CaswareSoftcellServiceItem;


typedef struct{
	char clientName[32];
	char version[20];
	char build[32];
	char device[32];
	char key[20];
	char cssn[20];
	char loadSequenceNumber[20];
	char firmwareVersion[20];
	char loaderVersion[20];
	char serialNumber[20];
}CaswareIrdetoSoftcellInfo;

typedef struct{
	char casware[20];
	CaswareIrdetoSoftcellInfo irdetoSoftcell;
}CaswareSystemInfo;

typedef struct{
	char caCode[20];
	char serialNumber[20];
	char type[20];
	char version[20];
	char build[20];
	char variant[20];
	char patchLevel[20];
	char ownerId[20];
	char nationality[20];
}CaswareIrdetoSoftcellSmartcard;

typedef struct{
	char state[20];
	CaswareIrdetoSoftcellSmartcard cardInfo;
}CaswareSmartcard;


typedef struct{
	char handle[20];
	char name[20];
	char globalStatus[20];
	char internalStatus[20];
	char smartCardId[20];
	char smartCardStatus[20];
	char emmSourceId[20];
	char emmSourceStatus[20];
	char monitoring[40];
}CaswareIrdetoSoftcellServiceStatus;


typedef struct{
	char fp_text[20];
	char fp_style_left[20];
	char fp_style_top[20];
	char fp_style_bgColor[32];
	char fp_style_color[32];
	int fp_duration_ms;
	int irdeto_coverageCode;
}CaswareFingerPrinting;


typedef struct{
	int msg_total;
	int msg_unread;
	int popup_msgid;
	char popup_text[1024];
	char popup_ir_type[20];
	int popup_ir_duration_ms;
}CaswareMessage;

typedef struct{
	char adultCode[20];
	char technicalCode[20];
}CaswarePinReseted;

int _casware_EventListener(CaswareEvent evt_type);
NATIVE_RETURN native_casware_OnCreate();
NATIVE_RETURN native_casware_setListener(caswareListener listener);
NATIVE_RETURN native_casware_unsetListener();

NATIVE_RETURN native_casware_addServiceListenerOn(char *paramHandleStr, char *resultStr);
NATIVE_RETURN native_casware_removeServiceListenerOn(char *paramHandleStr, char *resultStr);
NATIVE_RETURN native_casware_getProduct(int *itemCount, CaswareSoftcellProductItem **softcellProductItem);
NATIVE_RETURN native_casware_getServiceList(int *itemCount, CaswareSoftcellServiceItem **serviceItemList);
NATIVE_RETURN native_casware_getHomingChannel(char *channelName);

#endif // __NATIVECASWARE
