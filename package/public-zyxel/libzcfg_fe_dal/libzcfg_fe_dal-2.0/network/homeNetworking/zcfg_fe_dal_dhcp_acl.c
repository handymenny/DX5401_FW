#include <ctype.h>
#include <json/json.h>
//#include <json/json_object.h>
#include <time.h>

#include "zcfg_common.h"
#include "zcfg_fe_rdm_string.h"
#include "zcfg_fe_rdm_struct.h"
#include "zcfg_rdm_oid.h"
#include "zcfg_rdm_obj.h"
#include "zcfg_debug.h"
//#include "zcfg_eid.h"
#include "zcfg_msg.h"
#include "zcfg_fe_dal_common.h"

dal_param_t DHCP_ACL_param[] =
{
	{"Index",                       dalType_int,        0, 	0,	NULL,	NULL, 	dal_Edit|dal_Delete},
	{"Enable",                      dalType_boolean,    0, 	0,	NULL,	NULL, 	dal_Add|dal_Edit},
	{"Action",                      dalType_string,     0, 	0, 	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt60_Enable",                dalType_boolean,    0, 	0, 	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt60_IDExclude",             dalType_boolean,    0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt60_VendorClassId",         dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt61_Enable",                dalType_boolean,    0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt61_IAID",                  dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt61_DUIDType",              dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt61_Hardwaretype",          dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt61_Time",                  dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt61_LinkLayerAddress",      dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt61_EnterpriseNumber",      dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt61_IDExclude",             dalType_boolean,    0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt61_Identifier",            dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt61_DUID",                  dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt125_Enable",               dalType_boolean,    0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt125_EnterpriseNumber",     dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt125_ManufacturerOUI",      dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt125_ProductClass",         dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt125_ModelName",            dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt125_SerialNumberExclude",  dalType_boolean,    0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"Opt125_SerialNumber",         dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"VlanIdEnable",                dalType_boolean,    0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{"VlanId",                      dalType_string,     0,	0,	NULL,	NULL,	dal_Add|dal_Edit},
	{NULL,                          0,                  0, 	0, 	NULL,	NULL,	NULL}
};

void zcfgFeDalShowDHCPAcl(struct json_object *Jarray){
	int i, len = 0;
	struct json_object *obj = NULL;
	

	if(json_object_get_type(Jarray) != json_type_array){
		printf("wrong Jobj format!\n");
		return;
	}
	printf("%-10s %-10s %-18s %-35s %-58s %-10s %-10s\n",
		    "Index", "Status", "Opt60", "Opt61", "Opt125", "VLAN ID", "Action");
	len = json_object_array_length(Jarray);
	for(i=0; i<len;i++){
		obj = json_object_array_get_idx(Jarray, i);
		printf("%-10s %-10d ", 
				json_object_get_string(json_object_object_get(obj, "Index")),
				json_object_get_boolean(json_object_object_get(obj, "Enable")));
		if(json_object_get_boolean(json_object_object_get(obj, "Opt60_Enable"))){
			printf("%-1d/%-7s/%-9s",
					json_object_get_boolean(json_object_object_get(obj, "Opt60_Enable")),
					json_object_get_boolean(json_object_object_get(obj, "Opt60_IDExclude"))?"Exclude":"Include",
					json_object_get_string(json_object_object_get(obj, "Opt60_VendorClassId")));
		}
		else
			printf("%-18s ", "opt60 disable");

		if(json_object_get_boolean(json_object_object_get(obj, "Opt61_Enable"))){
			printf("%-1d/", json_object_get_boolean(json_object_object_get(obj, "Opt61_Enable")));
			if(!strcmp(json_object_get_string(json_object_object_get(obj, "Opt61_DUIDType")),"llt")){
				printf("%-3s/%-6s/%-6s/%-16s",
						"llt",
						json_object_get_string(json_object_object_get(obj, "Opt61_Hardwaretype")),
						json_object_get_string(json_object_object_get(obj, "Opt61_Time")),
						json_object_get_string(json_object_object_get(obj, "Opt61_LinkLayerAddress")));
			}
			else if(!strcmp(json_object_get_string(json_object_object_get(obj, "Opt61_DUIDType")),"ll")){
				printf("%-2s/%-10s/%-20s",
						"ll",
						json_object_get_string(json_object_object_get(obj, "Opt61_Hardwaretype")),
						json_object_get_string(json_object_object_get(obj, "Opt61_LinkLayerAddress")));
			}
			else if(!strcmp(json_object_get_string(json_object_object_get(obj, "Opt61_DUIDType")),"en")){
				printf("%-2s/%-10s/%-7s/%-12s",
						"en",
						json_object_get_string(json_object_object_get(obj, "Opt61_EnterpriseNumber")),
						json_object_get_boolean(json_object_object_get(obj, "Opt61_IDExclude"))?"Exclude":"Include",
						json_object_get_string(json_object_object_get(obj, "Opt61_Identifier")));

			}
			else if(!strcmp(json_object_get_string(json_object_object_get(obj, "Opt61_DUIDType")),"other")){
				printf("%-5s/%-28s",
						"other",
						json_object_get_string(json_object_object_get(obj, "Opt61_DUID")));
			}
			else
				printf("%-34s", "");
		}
		else
			printf("%-35s","opt61 disable");
		
		if(json_object_get_boolean(json_object_object_get(obj, "Opt125_Enable"))){
			printf("%-1d/%-5s/%-5s/%-9s/%-10s/%-7s/%-16s",
					json_object_get_boolean(json_object_object_get(obj, "Opt125_Enable")),
					json_object_get_string(json_object_object_get(obj, "Opt125_EnterpriseNumber")),
					json_object_get_string(json_object_object_get(obj, "Opt125_ManufacturerOUI")),
					json_object_get_string(json_object_object_get(obj, "Opt125_ProductClass")),
					json_object_get_string(json_object_object_get(obj, "Opt125_ModelName")),
					json_object_get_boolean(json_object_object_get(obj, "Opt125_SerialNumberExclude"))?"Exclude":"Include",
					json_object_get_string(json_object_object_get(obj, "Opt125_SerialNumber")));
		}
		else
			printf("%-59s", "opt125 disable");
		
		if(json_object_get_boolean(json_object_object_get(obj, "VlanIdEnable"))){
			printf("%-11s", json_object_get_string(json_object_object_get(obj, "VlanId")));
		}
		else
			printf("%-11s", "VLNdisable");

		printf("%-10s\n", json_object_get_string(json_object_object_get(obj, "Action")));
	}
	
}


zcfgRet_t zcfgFeDalDHCP_Acl_Add(struct json_object *Jobj, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;
	struct json_object *dhcpAclObj = NULL;
	objIndex_t dhcpAclIid = {0};
	bool enable = false;
	const char *Action = NULL;
	bool Opt60_Enable = false;
	bool Opt60_IDExclude = false;
	const char *Opt60_VendorClassId = NULL;
	bool Opt61_Enable = false;
	const char *Opt61_IAID = NULL;
	const char *Opt61_DUIDType = NULL;
	const char *Opt61_Hardwaretype = NULL;
	const char *Opt61_Time = NULL;
	const char *Opt61_LinkLayerAddress = NULL;
	const char *Opt61_EnterpriseNumber = NULL;
	bool Opt61_IDExclude = false;
	const char *Opt61_Identifier = NULL;
	const char *Opt61_DUID = NULL;
	bool Opt125_Enable = false;
	const char *Opt125_EnterpriseNumber = NULL;
	const char *Opt125_ManufacturerOUI = NULL;
	const char *Opt125_ProductClass = NULL;
	const char *Opt125_ModelName = NULL;
	bool Opt125_SerialNumberExclude = false;
	const char *Opt125_SerialNumber = NULL;
	bool VlanIdEnable = false;
	const char *VlanId = NULL;

	IID_INIT(dhcpAclIid);
	ret = zcfgFeObjJsonAdd(RDM_OID_DHCP_ACL_ENTRY, &dhcpAclIid, NULL);

	if(ret == ZCFG_SUCCESS){
		ret = zcfgFeObjJsonGet(RDM_OID_DHCP_ACL_ENTRY, &dhcpAclIid, &dhcpAclObj);
	}
	if(ret == ZCFG_SUCCESS){
		//get value from web obj, and saved to function variable
		enable = json_object_get_boolean(json_object_object_get(Jobj, "Enable"));
		Action = json_object_get_string(json_object_object_get(Jobj, "Action"));
		Opt60_Enable = json_object_get_boolean(json_object_object_get(Jobj, "Opt60_Enable"));
		Opt60_IDExclude = json_object_get_boolean(json_object_object_get(Jobj, "Opt60_IDExclude"));
		Opt60_VendorClassId = json_object_get_string(json_object_object_get(Jobj, "Opt60_VendorClassId"));
		Opt61_Enable = json_object_get_boolean(json_object_object_get(Jobj, "Opt61_Enable"));
		Opt61_IAID = json_object_get_string(json_object_object_get(Jobj, "Opt61_IAID"));
		Opt61_DUIDType = json_object_get_string(json_object_object_get(Jobj, "Opt61_DUIDType"));
		Opt61_Hardwaretype = json_object_get_string(json_object_object_get(Jobj, "Opt61_Hardwaretype"));
		Opt61_Time = json_object_get_string(json_object_object_get(Jobj, "Opt61_Time"));
		Opt61_LinkLayerAddress = json_object_get_string(json_object_object_get(Jobj, "Opt61_LinkLayerAddress"));
		Opt61_EnterpriseNumber = json_object_get_string(json_object_object_get(Jobj, "Opt61_EnterpriseNumber"));
		Opt61_IDExclude = json_object_get_boolean(json_object_object_get(Jobj, "Opt61_IDExclude"));
		Opt61_Identifier = json_object_get_string(json_object_object_get(Jobj, "Opt61_Identifier"));
		Opt61_DUID = json_object_get_string(json_object_object_get(Jobj, "Opt61_DUID"));
		Opt125_Enable = json_object_get_boolean(json_object_object_get(Jobj, "Opt125_Enable"));
		Opt125_EnterpriseNumber = json_object_get_string(json_object_object_get(Jobj, "Opt125_EnterpriseNumber"));
		Opt125_ManufacturerOUI = json_object_get_string(json_object_object_get(Jobj, "Opt125_ManufacturerOUI"));
		Opt125_ProductClass = json_object_get_string(json_object_object_get(Jobj, "Opt125_ProductClass"));
		Opt125_ModelName = json_object_get_string(json_object_object_get(Jobj, "Opt125_ModelName"));
		Opt125_SerialNumberExclude = json_object_get_boolean(json_object_object_get(Jobj, "Opt125_SerialNumberExclude"));
		Opt125_SerialNumber = json_object_get_string(json_object_object_get(Jobj, "Opt125_SerialNumber"));
		VlanIdEnable = json_object_get_boolean(json_object_object_get(Jobj, "VlanIdEnable"));
		VlanId = json_object_get_string(json_object_object_get(Jobj, "VlanId"));

		//set the value into json obj for setting it to RDM
		json_object_object_add(dhcpAclObj, "Enable", json_object_new_boolean(enable));
		json_object_object_add(dhcpAclObj, "Opt60_Enable", json_object_new_boolean(Opt60_Enable));
		json_object_object_add(dhcpAclObj, "Opt60_IDExclude", json_object_new_boolean(Opt60_IDExclude));
		json_object_object_add(dhcpAclObj, "Opt60_VendorClassId", json_object_new_string(Opt60_VendorClassId));
		json_object_object_add(dhcpAclObj, "Opt61_Enable", json_object_new_boolean(Opt61_Enable));
		json_object_object_add(dhcpAclObj, "Opt61_IAID", json_object_new_string(Opt61_IAID));
		json_object_object_add(dhcpAclObj, "Opt61_DUIDType", json_object_new_string(Opt61_DUIDType));
		json_object_object_add(dhcpAclObj, "Opt61_Hardwaretype", json_object_new_string(Opt61_Hardwaretype));
		json_object_object_add(dhcpAclObj, "Opt61_Time", json_object_new_string(Opt61_Time));
		json_object_object_add(dhcpAclObj, "Opt61_LinkLayerAddress", json_object_new_string(Opt61_LinkLayerAddress));
		json_object_object_add(dhcpAclObj, "Opt61_EnterpriseNumber", json_object_new_string(Opt61_EnterpriseNumber));
		json_object_object_add(dhcpAclObj, "Opt61_IDExclude", json_object_new_boolean(Opt61_IDExclude));
		json_object_object_add(dhcpAclObj, "Opt61_Identifier", json_object_new_string(Opt61_Identifier));
		json_object_object_add(dhcpAclObj, "Opt61_DUID", json_object_new_string(Opt61_DUID));
		json_object_object_add(dhcpAclObj, "Opt125_Enable", json_object_new_boolean(Opt125_Enable));
		json_object_object_add(dhcpAclObj, "Opt125_EnterpriseNumber", json_object_new_string(Opt125_EnterpriseNumber));
		json_object_object_add(dhcpAclObj, "Opt125_ManufacturerOUI", json_object_new_string(Opt125_ManufacturerOUI));
		json_object_object_add(dhcpAclObj, "Opt125_ProductClass", json_object_new_string(Opt125_ProductClass));
		json_object_object_add(dhcpAclObj, "Opt125_ModelName", json_object_new_string(Opt125_ModelName));
		json_object_object_add(dhcpAclObj, "Opt125_SerialNumberExclude", json_object_new_boolean(Opt125_SerialNumberExclude));
		json_object_object_add(dhcpAclObj, "Opt125_SerialNumber", json_object_new_string(Opt125_SerialNumber));
		json_object_object_add(dhcpAclObj, "VlanIdEnable", json_object_new_boolean(VlanIdEnable));
		json_object_object_add(dhcpAclObj, "VlanId", json_object_new_string(VlanId));

		if ( strcmp (Action, "assign_pool") == 0 ) {
			json_object_object_add(dhcpAclObj, "Action", json_object_new_string("assign"));
			json_object_object_add(dhcpAclObj, "DHCP_Index", json_object_new_int(1));
		}
		else {
			json_object_object_add(dhcpAclObj, "Action", json_object_new_string(Action));
			json_object_object_add(dhcpAclObj, "DHCP_Index", json_object_new_int(0));
		}

		ret = zcfgFeObjJsonSet(RDM_OID_DHCP_ACL_ENTRY, &dhcpAclIid, dhcpAclObj, NULL);
	}

	return ret;
}

zcfgRet_t zcfgFeDalDHCP_Acl_Edit(struct json_object *Jobj, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;
	struct json_object *dhcpAclObj = NULL;
	objIndex_t dhcpAclIid = {0};
	int rowIndex = 0;
	int count = 1;
	bool found = false;	
	bool enable = false;
	const char *Action = NULL;
	bool Opt60_Enable = false;
	bool Opt60_IDExclude = false;
	const char *Opt60_VendorClassId = NULL;
	bool Opt61_Enable = false;
	const char *Opt61_IAID = NULL;
	const char *Opt61_DUIDType = NULL;
	const char *Opt61_Hardwaretype = NULL;
	const char *Opt61_Time = NULL;
	const char *Opt61_LinkLayerAddress = NULL;
	const char *Opt61_EnterpriseNumber = NULL;
	bool Opt61_IDExclude = false;
	const char *Opt61_Identifier = NULL;
	const char *Opt61_DUID = NULL;
	bool Opt125_Enable = false;
	const char *Opt125_EnterpriseNumber = NULL;
	const char *Opt125_ManufacturerOUI = NULL;
	const char *Opt125_ProductClass = NULL;
	const char *Opt125_ModelName = NULL;
	bool Opt125_SerialNumberExclude = false;
	const char *Opt125_SerialNumber = NULL;
	bool VlanIdEnable = false;
	const char *VlanId = NULL;

	rowIndex = json_object_get_int(json_object_object_get(Jobj, "Index"));	
	IID_INIT(dhcpAclIid);

	while(zcfgFeObjJsonGetNext(RDM_OID_DHCP_ACL_ENTRY, &dhcpAclIid, &dhcpAclObj) == ZCFG_SUCCESS) {
		if(count == rowIndex){
			found = true;
			break;
		}
			count++;
		zcfgFeJsonObjFree(dhcpAclObj);
	}

	if(found)
	{
		enable = json_object_get_boolean(json_object_object_get(Jobj, "Enable"));
		Action = json_object_get_string(json_object_object_get(Jobj, "Action"));
		Opt60_Enable = json_object_get_boolean(json_object_object_get(Jobj, "Opt60_Enable"));
		Opt60_IDExclude = json_object_get_boolean(json_object_object_get(Jobj, "Opt60_IDExclude"));
		Opt60_VendorClassId = json_object_get_string(json_object_object_get(Jobj, "Opt60_VendorClassId"));
		Opt61_Enable = json_object_get_boolean(json_object_object_get(Jobj, "Opt61_Enable"));
		Opt61_IAID = json_object_get_string(json_object_object_get(Jobj, "Opt61_IAID"));
		Opt61_DUIDType = json_object_get_string(json_object_object_get(Jobj, "Opt61_DUIDType"));
		Opt61_Hardwaretype = json_object_get_string(json_object_object_get(Jobj, "Opt61_Hardwaretype"));
		Opt61_Time = json_object_get_string(json_object_object_get(Jobj, "Opt61_Time"));
		Opt61_LinkLayerAddress = json_object_get_string(json_object_object_get(Jobj, "Opt61_LinkLayerAddress"));
		Opt61_EnterpriseNumber = json_object_get_string(json_object_object_get(Jobj, "Opt61_EnterpriseNumber"));
		Opt61_IDExclude = json_object_get_boolean(json_object_object_get(Jobj, "Opt61_IDExclude"));
		Opt61_Identifier = json_object_get_string(json_object_object_get(Jobj, "Opt61_Identifier"));
		Opt61_DUID = json_object_get_string(json_object_object_get(Jobj, "Opt61_DUID"));
		Opt125_Enable = json_object_get_boolean(json_object_object_get(Jobj, "Opt125_Enable"));
		Opt125_EnterpriseNumber = json_object_get_string(json_object_object_get(Jobj, "Opt125_EnterpriseNumber"));
		Opt125_ManufacturerOUI = json_object_get_string(json_object_object_get(Jobj, "Opt125_ManufacturerOUI"));
		Opt125_ProductClass = json_object_get_string(json_object_object_get(Jobj, "Opt125_ProductClass"));
		Opt125_ModelName = json_object_get_string(json_object_object_get(Jobj, "Opt125_ModelName"));
		Opt125_SerialNumberExclude = json_object_get_boolean(json_object_object_get(Jobj, "Opt125_SerialNumberExclude"));
		Opt125_SerialNumber = json_object_get_string(json_object_object_get(Jobj, "Opt125_SerialNumber"));
		VlanIdEnable = json_object_get_boolean(json_object_object_get(Jobj, "VlanIdEnable"));
		VlanId = json_object_get_string(json_object_object_get(Jobj, "VlanId"));

		json_object_object_add(dhcpAclObj, "Enable", json_object_new_boolean(enable));
		json_object_object_add(dhcpAclObj, "Opt60_Enable", json_object_new_boolean(Opt60_Enable));
		json_object_object_add(dhcpAclObj, "Opt60_IDExclude", json_object_new_boolean(Opt60_IDExclude));
		json_object_object_add(dhcpAclObj, "Opt60_VendorClassId", json_object_new_string(Opt60_VendorClassId));
		json_object_object_add(dhcpAclObj, "Opt61_Enable", json_object_new_boolean(Opt61_Enable));
		json_object_object_add(dhcpAclObj, "Opt61_IAID", json_object_new_string(Opt61_IAID));
		json_object_object_add(dhcpAclObj, "Opt61_DUIDType", json_object_new_string(Opt61_DUIDType));
		json_object_object_add(dhcpAclObj, "Opt61_Hardwaretype", json_object_new_string(Opt61_Hardwaretype));
		json_object_object_add(dhcpAclObj, "Opt61_Time", json_object_new_string(Opt61_Time));
		json_object_object_add(dhcpAclObj, "Opt61_LinkLayerAddress", json_object_new_string(Opt61_LinkLayerAddress));
		json_object_object_add(dhcpAclObj, "Opt61_EnterpriseNumber", json_object_new_string(Opt61_EnterpriseNumber));
		json_object_object_add(dhcpAclObj, "Opt61_IDExclude", json_object_new_boolean(Opt61_IDExclude));
		json_object_object_add(dhcpAclObj, "Opt61_Identifier", json_object_new_string(Opt61_Identifier));
		json_object_object_add(dhcpAclObj, "Opt61_DUID", json_object_new_string(Opt61_DUID));
		json_object_object_add(dhcpAclObj, "Opt125_Enable", json_object_new_boolean(Opt125_Enable));
		json_object_object_add(dhcpAclObj, "Opt125_EnterpriseNumber", json_object_new_string(Opt125_EnterpriseNumber));
		json_object_object_add(dhcpAclObj, "Opt125_ManufacturerOUI", json_object_new_string(Opt125_ManufacturerOUI));
		json_object_object_add(dhcpAclObj, "Opt125_ProductClass", json_object_new_string(Opt125_ProductClass));
		json_object_object_add(dhcpAclObj, "Opt125_ModelName", json_object_new_string(Opt125_ModelName));
		json_object_object_add(dhcpAclObj, "Opt125_SerialNumberExclude", json_object_new_boolean(Opt125_SerialNumberExclude));
		json_object_object_add(dhcpAclObj, "Opt125_SerialNumber", json_object_new_string(Opt125_SerialNumber));
		json_object_object_add(dhcpAclObj, "VlanIdEnable", json_object_new_boolean(VlanIdEnable));
		json_object_object_add(dhcpAclObj, "VlanId", json_object_new_string(VlanId));

		if ( strcmp (Action, "assign_pool") == 0 ) {
			json_object_object_add(dhcpAclObj, "Action", json_object_new_string("assign"));
			json_object_object_add(dhcpAclObj, "DHCP_Index", json_object_new_int(1));
		}
		else {
			json_object_object_add(dhcpAclObj, "Action", json_object_new_string(Action));
			json_object_object_add(dhcpAclObj, "DHCP_Index", json_object_new_int(0));
		}

		ret = zcfgFeObjJsonSet(RDM_OID_DHCP_ACL_ENTRY, &dhcpAclIid, dhcpAclObj, NULL);
		zcfgFeJsonObjFree(dhcpAclObj);	
	}
	
	return ret;
}
zcfgRet_t zcfgFeDalDHCP_Acl_Delete(struct json_object *Jobj, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;	
	struct json_object *dhcpAclObj = NULL;
	objIndex_t dhcpAclIid = {0};
	int rowIndex = 0;
	int count = 1;
	bool found = false;

	rowIndex = json_object_get_int(json_object_object_get(Jobj, "Index"));	
	IID_INIT(dhcpAclIid);

	while(zcfgFeObjJsonGetNext(RDM_OID_DHCP_ACL_ENTRY, &dhcpAclIid, &dhcpAclObj) == ZCFG_SUCCESS) {
		if(count == rowIndex){
			found = true;
			break;
		}
			count++;
		zcfgFeJsonObjFree(dhcpAclObj);
	}

	if(found){
		ret = zcfgFeObjJsonDel(RDM_OID_DHCP_ACL_ENTRY, &dhcpAclIid, NULL);
		zcfgFeJsonObjFree(dhcpAclObj);
	}
	return ret;
	
}

void getDHCPAclObj(int count, int iid, struct json_object *dhcpAclObj, struct json_object *Jarray){
		const char *Action = NULL;
		struct json_object *paramJobj = NULL;

		paramJobj = json_object_new_object();

		json_object_object_add(paramJobj, "Index", json_object_new_int(count));
		json_object_object_add(paramJobj, "Enable", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Enable")));
		json_object_object_add(paramJobj, "Opt60_Enable", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt60_Enable")));
		json_object_object_add(paramJobj, "Opt60_IDExclude", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt60_IDExclude")));
		json_object_object_add(paramJobj, "Opt60_VendorClassId", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt60_VendorClassId")));
		json_object_object_add(paramJobj, "Opt61_Enable", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt61_Enable")));
		json_object_object_add(paramJobj, "Opt61_IAID", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt61_IAID")));
		json_object_object_add(paramJobj, "Opt61_DUIDType", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt61_DUIDType")));
		json_object_object_add(paramJobj, "Opt61_Hardwaretype", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt61_Hardwaretype")));
		json_object_object_add(paramJobj, "Opt61_Time", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt61_Time")));
		json_object_object_add(paramJobj, "Opt61_LinkLayerAddress", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt61_LinkLayerAddress")));
		json_object_object_add(paramJobj, "Opt61_EnterpriseNumber", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt61_EnterpriseNumber")));
		json_object_object_add(paramJobj, "Opt61_IDExclude", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt61_IDExclude")));
		json_object_object_add(paramJobj, "Opt61_Identifier", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt61_Identifier")));
		json_object_object_add(paramJobj, "Opt61_DUID", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt61_DUID")));
		json_object_object_add(paramJobj, "Opt125_Enable", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt125_Enable")));
		json_object_object_add(paramJobj, "Opt125_EnterpriseNumber", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt125_EnterpriseNumber")));
		json_object_object_add(paramJobj, "Opt125_ManufacturerOUI", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt125_ManufacturerOUI")));
		json_object_object_add(paramJobj, "Opt125_ProductClass", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt125_ProductClass")));
		json_object_object_add(paramJobj, "Opt125_ModelName", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt125_ModelName")));
		json_object_object_add(paramJobj, "Opt125_SerialNumberExclude", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt125_SerialNumberExclude")));
		json_object_object_add(paramJobj, "Opt125_SerialNumber", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Opt125_SerialNumber")));
		json_object_object_add(paramJobj, "VlanIdEnable", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"VlanIdEnable")));
		json_object_object_add(paramJobj, "VlanId", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"VlanId")));

		Action = json_object_get_string(json_object_object_get(dhcpAclObj, "Action"));
		if ( strcmp (Action, "assign") == 0 && json_object_get_int(json_object_object_get(dhcpAclObj, "DHCP_Index")) != 0 )
			json_object_object_add(paramJobj, "Action", json_object_new_string("assign_pool"));
		else
			json_object_object_add(paramJobj, "Action", JSON_OBJ_COPY(json_object_object_get(dhcpAclObj,"Action")));

		json_object_array_add(Jarray, paramJobj);
		zcfgFeJsonObjFree(dhcpAclObj);
}

zcfgRet_t zcfgFeDalDHCP_Acl_Get(struct json_object *Jobj, struct json_object *Jarray, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;

	struct json_object *dhcpAclObj = NULL;	//Get data model parameter
	objIndex_t dhcpAclIid = {0};
	int index = 0;

	IID_INIT(dhcpAclIid);
	if(json_object_object_get(Jobj, "Index")){ //for New GUI get single obj
		index = json_object_get_int(json_object_object_get(Jobj, "Index"));		
		ret = convertIndextoIid(index, &dhcpAclIid, RDM_OID_DHCP_ACL_ENTRY, NULL, NULL, replyMsg);		
		if(ret != ZCFG_SUCCESS)
			return ret;
		if(zcfgFeObjJsonGetWithoutUpdate(RDM_OID_DHCP_ACL_ENTRY, &dhcpAclIid , &dhcpAclObj) == ZCFG_SUCCESS){
			getDHCPAclObj(index, dhcpAclIid.idx[0], dhcpAclObj, Jarray);		
		}
	}else{
		while(zcfgFeObjJsonGetNext(RDM_OID_DHCP_ACL_ENTRY, &dhcpAclIid, &dhcpAclObj) == ZCFG_SUCCESS){
			index ++;
			getDHCPAclObj(index, dhcpAclIid.idx[0], dhcpAclObj, Jarray);			
		}
	}
	return ret;
}


zcfgRet_t zcfgFeDalDHCPAcl(const char *method, struct json_object *Jobj, struct json_object *Jarray, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;

	if(!method || !Jobj)
		return ZCFG_INTERNAL_ERROR;

	if(!strcmp(method, "PUT")) {
		ret = zcfgFeDalDHCP_Acl_Edit(Jobj, NULL);
	}else if(!strcmp(method, "POST")) {
		ret = zcfgFeDalDHCP_Acl_Add(Jobj, NULL);
	}else if(!strcmp(method, "DELETE")) {
		ret = zcfgFeDalDHCP_Acl_Delete(Jobj, NULL);
	}else if(!strcmp(method, "GET"))
		ret = zcfgFeDalDHCP_Acl_Get(Jobj, Jarray, NULL);
	else
		printf("Unknown method:%s\n", method);

	return ret;
}
