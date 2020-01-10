/**
 * @file Casware.cpp
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

#include "Casware.h"
#include <cstring>

using namespace std;

Casware* Casware::m_instance = nullptr;

Casware::Casware()
    : EventTarget("casware")
{
    printf("[%s:%s:%d]\n", __FILE__, __FUNCTION__, __LINE__ );
	native_casware_OnCreate();
    native_casware_setListener(onCaswareEvent);
}

Casware::~Casware()
{
    printf("[%s:%s:%d]\n", __FILE__, __FUNCTION__, __LINE__ );
	native_casware_unsetListener();
}

Casware* Casware::getInstance()
{
	if ( m_instance == nullptr )
	{
		m_instance = new Casware();
	}
	return m_instance;
}

void Casware::onCaswareEvent(CaswareEvent evt_type, void *data/*, event data*/)
{
	JsonObject eventParam;
    string eventMethod, eventJSON;

	switch ( evt_type )
	{
		case CASWARE_EVENT_SYSTEM_INFO_CHANGE:
			eventMethod	="casware.onSystemInfoChange";
			eventParam	= onSystemInfoChange(data);
			break;
		default:
			return;
	}

	Event *_event = new Event(eventMethod.c_str(),eventParam);
	if ( _event )
	{
		Casware::getInstance()->dispatchEvent(_event);
	}
}

PROC_RETURN Casware::doAction( const char* _name, JsonObject _param, JsonObject *_retval )
{
    IF_TRUE_RETURN_VAL( _name == nullptr || _retval == nullptr, PROC_RETURN_ERROR );

    DO_PROCFUNCTION( "getProducts", getProducts );

    return PROC_RETURN_ERROR;
}

PROC_RETURN Casware::getProducts( JsonObject param, JsonObject *retval )
{
	printf("[%s:%s:%d]\n", __FILE__, __FUNCTION__, __LINE__ );

    int i,j,itemCount=0;
    CaswareSoftcellProductItem *softcellProductItem = NULL;
    //send param, get response
    native_casware_getProduct(&itemCount, &softcellProductItem);

    JsonObject prod_obj = JsonObject::create(JO_OBJECT);
    JsonObject productList_obj = JsonObject::create(JO_OBJECT);
    {
        JsonObject irdetoSoftCell_obj = JsonObject::create(JO_ARRAY);
		for(j=0; j<itemCount;j++)
		{
			JsonObject softCellItem_obj = JsonObject::create(JO_OBJECT);
			{
				JsonObject sector_obj = JsonObject::valueOf(softcellProductItem[j].sector);
				softCellItem_obj.put("Sector",sector_obj);

				JsonObject dateCode_obj = JsonObject::valueOf(softcellProductItem[j].dateCode);
				softCellItem_obj.put("DateCode",dateCode_obj);

				JsonObject products_obj = JsonObject::create(JO_ARRAY);
				{
					for(i=0; i<20; i++)
					{
						if( strlen( softcellProductItem[j].productItems[i].productId ) > 0 )
						{
							JsonObject productsItem_obj = JsonObject::create(JO_OBJECT);

							JsonObject productId_obj = JsonObject::valueOf(softcellProductItem[j].productItems[i].productId);
							productsItem_obj.put("ProductId",productId_obj);

							JsonObject startDate_obj = JsonObject::valueOf(softcellProductItem[j].productItems[i].startDate);
							productsItem_obj.put("StartDate",startDate_obj);

							JsonObject duration_obj = JsonObject::valueOf(softcellProductItem[j].productItems[i].duration);
							productsItem_obj.put("Duration",duration_obj);

							products_obj.append(productsItem_obj);
						}
					}
				}
				softCellItem_obj.put("Products",products_obj);
			}
			irdetoSoftCell_obj.append(softCellItem_obj);
		}
        productList_obj.put("IrdetoSoftcell", irdetoSoftCell_obj);
    }
    prod_obj.put("ProductLists",productList_obj);
    (*retval).put("result",prod_obj);

    return PROC_RETURN_OK;
}

JsonObject Casware::onSystemInfoChange(void* data)
{
	CaswareSystemInfo *pstSystemInfo = (CaswareSystemInfo *)data;
    JsonObject softcellStatusObj, stateObj, smartcardStatusObj, systemObj;

	softcellStatusObj = JsonObject::create(JO_OBJECT);
	softcellStatusObj.put("ClientName", JsonObject::valueOf(pstSystemInfo->irdetoSoftcell.clientName));
	softcellStatusObj.put("Version", JsonObject::valueOf(pstSystemInfo->irdetoSoftcell.version));
	softcellStatusObj.put("Build", JsonObject::valueOf(pstSystemInfo->irdetoSoftcell.build));
	softcellStatusObj.put("Device", JsonObject::valueOf(pstSystemInfo->irdetoSoftcell.device));
	softcellStatusObj.put("Key", JsonObject::valueOf(pstSystemInfo->irdetoSoftcell.key));
	softcellStatusObj.put("CSSN", JsonObject::valueOf(pstSystemInfo->irdetoSoftcell.cssn));
	softcellStatusObj.put("L-", JsonObject::valueOf(pstSystemInfo->irdetoSoftcell.loadSequenceNumber));
	softcellStatusObj.put("FirmwareVersion", JsonObject::valueOf(pstSystemInfo->irdetoSoftcell.firmwareVersion));
	softcellStatusObj.put("LoaderVersion", JsonObject::valueOf(pstSystemInfo->irdetoSoftcell.loaderVersion));
	softcellStatusObj.put("SerialNumber", JsonObject::valueOf(pstSystemInfo->irdetoSoftcell.serialNumber));

	smartcardStatusObj = JsonObject::create(JO_OBJECT);
	smartcardStatusObj.put("Casware", JsonObject::valueOf(pstSystemInfo->casware));
	smartcardStatusObj.put("IrdetoSoftcell", softcellStatusObj);

	systemObj = JsonObject::create(JO_OBJECT);
	systemObj.put("SystemInfo", smartcardStatusObj);

	printf("[01;35m[%s]\n%s[00m\n", __FUNCTION__, systemObj.dump().c_str());

    return systemObj;
}