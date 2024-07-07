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

dal_param_t DHCP_ACL_POOL_param[] =
{
	{"Active",				dalType_boolean,	0,	0,	NULL,	NULL,	dal_Edit},
	{"DHCPACL_MinAddress",	dalType_v4Addr,	0,	0,	NULL,	NULL,	dal_Edit},
	{"DHCPACL_MaxAddress",	dalType_v4Addr,	0,	0,	NULL,	NULL,	dal_Edit},
	{NULL,                          	0,                 	0, 	0, 	NULL,	NULL,	NULL}
};

zcfgRet_t zcfgFeDalDHCP_Acl_Pool_Edit(struct json_object *Jobj, char *replyMsg)
{
	zcfgRet_t ret = ZCFG_SUCCESS;

	bool bFind = false, bDefault = false;
	objIndex_t ipifaceIid = {0};
	objIndex_t dhcpv4srvIid = {0};
	objIndex_t findIid = {0};
	struct json_object *paramJobj = NULL;
	struct json_object *ipifaceObj = NULL;
	struct json_object *dhcpv4srvObj = NULL;
	struct json_object *defaultObj = NULL;

	bool enable = false;
	const char *dhcpMinAdr = NULL, *dhcpMaxAdr = NULL, *poolInterface = NULL;

	enable = Jgetb(Jobj, "Active");
	dhcpMinAdr = Jgets(Jobj, "DHCPACL_MinAddress");
	dhcpMaxAdr = Jgets(Jobj, "DHCPACL_MaxAddress");

	IID_INIT(dhcpv4srvIid);
	while(zcfgFeObjJsonGetNext(RDM_OID_DHCPV4_SRV_POOL, &dhcpv4srvIid, &dhcpv4srvObj) == ZCFG_SUCCESS)
	{
		if ( Jgetb(dhcpv4srvObj, "X_ZYXEL_DHCPAclPool") )
		{
			bFind = true;

			IID_INIT(findIid);
			findIid.level = dhcpv4srvIid.level;
			findIid.idx[0] = dhcpv4srvIid.idx[0];
		}
		else if (bDefault == false)
		{
			IID_INIT(ipifaceIid);

			poolInterface = Jgets(dhcpv4srvObj, "Interface");
			sscanf(poolInterface, "IP.Interface.%hhu", &ipifaceIid.idx[0]);
			ipifaceIid.level = 1;

			if ( zcfgFeObjJsonGetWithoutUpdate(RDM_OID_IP_IFACE, &ipifaceIid, &ipifaceObj) == ZCFG_SUCCESS )
			{
				if ( strcmp (Jgets(ipifaceObj, "X_ZYXEL_SrvName"), "Default") == 0 )
				{
					bDefault = true;
				}

				zcfgFeJsonObjFree (ipifaceObj);
			}			
		}

		if ( bFind == true )
		{
			if (bDefault == true)
			{
				break;
			}
		}
		else if (bDefault == true)
		{
			defaultObj = dhcpv4srvObj;
		}
		else
		{
			zcfgFeJsonObjFree (dhcpv4srvObj);
		}
	}

	if ( bFind == true )
	{
		if ( bDefault == true )
		{
			json_object_object_add(dhcpv4srvObj, "Interface", JSON_OBJ_COPY(json_object_object_get(defaultObj, "Interface")));
			json_object_object_add(dhcpv4srvObj, "SubnetMask", JSON_OBJ_COPY(json_object_object_get(defaultObj, "SubnetMask")));
			json_object_object_add(dhcpv4srvObj, "DNSServers", JSON_OBJ_COPY(json_object_object_get(defaultObj, "DNSServers")));
			json_object_object_add(dhcpv4srvObj, "X_ZYXEL_DNS_Type", JSON_OBJ_COPY(json_object_object_get(defaultObj, "X_ZYXEL_DNS_Type")));
			json_object_object_add(dhcpv4srvObj, "LeaseTime", JSON_OBJ_COPY(json_object_object_get(defaultObj, "LeaseTime")));
			json_object_object_add(dhcpv4srvObj, "Status", JSON_OBJ_COPY(json_object_object_get(defaultObj, "Status")));
			json_object_object_add(dhcpv4srvObj, "X_ZYXEL_SetEnable", JSON_OBJ_COPY(json_object_object_get(defaultObj, "X_ZYXEL_SetEnable")));
			json_object_object_add(dhcpv4srvObj, "X_ZYXEL_AutoReserveLanIp", JSON_OBJ_COPY(json_object_object_get(defaultObj, "X_ZYXEL_AutoReserveLanIp")));
			json_object_object_add(dhcpv4srvObj, "X_ZYXEL_ServerConfigurable", JSON_OBJ_COPY(json_object_object_get(defaultObj, "X_ZYXEL_ServerConfigurable")));

			zcfgFeJsonObjFree(defaultObj);	
		}

		json_object_object_add(dhcpv4srvObj, "Enable", json_object_new_boolean(enable));
		json_object_object_add(dhcpv4srvObj, "MinAddress", json_object_new_string(dhcpMinAdr));
		json_object_object_add(dhcpv4srvObj, "MaxAddress", json_object_new_string(dhcpMaxAdr));

		ret = zcfgFeObjJsonSet (RDM_OID_DHCPV4_SRV_POOL, &findIid, dhcpv4srvObj, NULL);

		zcfgFeJsonObjFree(dhcpv4srvObj);
	}

	return ret;
}

zcfgRet_t zcfgFeDalDHCP_Acl_Pool_Get(struct json_object *Jobj, struct json_object *Jarray, char *replyMsg)
{
	bool bFind = false;
	objIndex_t dhcpv4srvIid = {0};
	struct json_object *paramJobj = NULL;
	struct json_object *dhcpv4srvObj = NULL;

	paramJobj = json_object_new_object();

	IID_INIT(dhcpv4srvIid);
	while(!bFind && zcfgFeObjJsonGetNext(RDM_OID_DHCPV4_SRV_POOL, &dhcpv4srvIid, &dhcpv4srvObj) == ZCFG_SUCCESS)
	{
		if ( Jgetb(dhcpv4srvObj, "X_ZYXEL_DHCPAclPool") )
		{
			bFind = true;

			json_object_object_add(paramJobj, "Active", JSON_OBJ_COPY(json_object_object_get(dhcpv4srvObj,"Enable")));
			json_object_object_add(paramJobj, "DHCPACL_MinAddress", JSON_OBJ_COPY(json_object_object_get(dhcpv4srvObj,"MinAddress")));
			json_object_object_add(paramJobj, "DHCPACL_MaxAddress", JSON_OBJ_COPY(json_object_object_get(dhcpv4srvObj,"MaxAddress")));
		}

		zcfgFeJsonObjFree(dhcpv4srvObj);
	}

	if ( bFind == false )
	{
		json_object_object_add(paramJobj, "Active", json_object_new_boolean(false));
		json_object_object_add(paramJobj, "DHCPACL_MinAddress", json_object_new_string(""));
		json_object_object_add(paramJobj, "DHCPACL_MaxAddress", json_object_new_string(""));
	}

	json_object_array_add(Jarray, paramJobj);

	return ZCFG_SUCCESS;
}

zcfgRet_t zcfgFeDalDHCPAclPool(const char *method, struct json_object *Jobj, struct json_object *Jarray, char *replyMsg)
{
	zcfgRet_t ret = ZCFG_SUCCESS;

	if(!method || !Jobj)
		return ZCFG_INTERNAL_ERROR;

	if(!strcmp(method, "PUT")) {
		ret = zcfgFeDalDHCP_Acl_Pool_Edit(Jobj, NULL);
	}else if(!strcmp(method, "GET"))
		ret = zcfgFeDalDHCP_Acl_Pool_Get(Jobj, Jarray, NULL);
	else
		printf("Unknown method:%s\n", method);

	return ret;
}
