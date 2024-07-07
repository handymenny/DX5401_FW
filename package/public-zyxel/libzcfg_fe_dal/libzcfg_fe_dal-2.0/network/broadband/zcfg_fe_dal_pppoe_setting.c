#include <ctype.h>
#include <unistd.h>	// for sleep()
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
#include "zcfg_fe_dal.h"


zcfgRet_t zcfgFeDal_PPPoE_Edit(struct json_object *Jobj, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;
	struct json_object *ipObj = NULL, *pppoeObj = NULL;
    objIndex_t iid, pppoeIid;
	bool isDfGateway = false;
	const char *svrName = NULL, *lowerLayers = NULL, *userName = NULL;
	const char *password = NULL, *oriUserName = NULL, *oriPassword = NULL;

	IID_INIT(iid);
	IID_INIT(pppoeIid);
	while(zcfgFeObjJsonGetNext(RDM_OID_IP_IFACE, &iid, &ipObj) == ZCFG_SUCCESS){
		isDfGateway = json_object_get_boolean(json_object_object_get(ipObj, "X_ZYXEL_DefaultGatewayIface"));
		svrName = json_object_get_string(json_object_object_get(ipObj, "X_ZYXEL_SrvName"));
		if(isDfGateway && strcmp(svrName, "WWAN")){
			lowerLayers = json_object_get_string(json_object_object_get(ipObj, "LowerLayers"));
			pppoeIid.level = 1;
			sscanf(lowerLayers, "PPP.Interface.%hhu", &pppoeIid.idx[0]);
			ret = zcfgFeObjJsonGetWithoutUpdate(RDM_OID_PPP_IFACE, &pppoeIid, &pppoeObj);
			
			if(ret == ZCFG_SUCCESS){
				oriUserName = json_object_get_string(json_object_object_get(pppoeObj, "Username"));
				oriPassword = json_object_get_string(json_object_object_get(pppoeObj, "Password"));
				userName = json_object_get_string(json_object_object_get(Jobj, "pppUsername"));
				password = json_object_get_string(json_object_object_get(Jobj, "pppPassword"));
				if(strcmp(oriUserName, userName) || strcmp(oriPassword, password)){	
						json_object_object_add(pppoeObj, "Username", json_object_new_string(userName));
						json_object_object_add(pppoeObj, "Password", json_object_new_string(password));
						zcfgFeObjJsonSet(RDM_OID_PPP_IFACE, &pppoeIid, pppoeObj, NULL);
					}
				zcfgFeJsonObjFree(ipObj);
				zcfgFeJsonObjFree(pppoeObj);
				break;
			}
		}
		zcfgFeJsonObjFree(ipObj);
	}

	return ret;
}

zcfgRet_t zcfgFeDal_PPPoE_Get(struct json_object *Jobj, struct json_object *Jarray, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;
	struct json_object *ipObj = NULL, *pppoeObj = NULL, *retObj = NULL;
    objIndex_t iid, pppoeIid;
	bool isDfGateway = false;
	const char *svrName = NULL, *lowerLayers = NULL;
	const char *oriUserName = NULL, *oriPassword = NULL;

	retObj = json_object_new_object();
	IID_INIT(iid);
	IID_INIT(pppoeIid);
	while(zcfgFeObjJsonGetNext(RDM_OID_IP_IFACE, &iid, &ipObj) == ZCFG_SUCCESS){
		isDfGateway = json_object_get_boolean(json_object_object_get(ipObj, "X_ZYXEL_DefaultGatewayIface"));
		svrName = json_object_get_string(json_object_object_get(ipObj, "X_ZYXEL_SrvName"));
		if(isDfGateway && strcmp(svrName, "WWAN")){
			lowerLayers = json_object_get_string(json_object_object_get(ipObj, "LowerLayers"));
			pppoeIid.level = 1;
			sscanf(lowerLayers, "PPP.Interface.%hhu", &pppoeIid.idx[0]);
			ret = zcfgFeObjJsonGetWithoutUpdate(RDM_OID_PPP_IFACE, &pppoeIid, &pppoeObj);
			
			if(ret == ZCFG_SUCCESS){
				oriUserName = json_object_get_string(json_object_object_get(pppoeObj, "Username"));
				oriPassword = json_object_get_string(json_object_object_get(pppoeObj, "Password"));
				json_object_object_add(retObj, "pppUsername", json_object_new_string(oriUserName));
				json_object_object_add(retObj, "pppPassword", json_object_new_string(oriPassword));
				zcfgFeJsonObjFree(ipObj);
				zcfgFeJsonObjFree(pppoeObj);
				break;
			}
			
		}
		zcfgFeJsonObjFree(ipObj);
	}
	json_object_array_add(Jarray, retObj);
	printf("(%d) Jarray = %s\n", __LINE__, json_object_to_json_string(Jarray));

	return ret;
}


zcfgRet_t zcfgFeDalPPPoEQuickSet(const char *method, struct json_object *Jobj, struct json_object *Jarray, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;

	if(!strcmp(method, "PUT")){
		ret = zcfgFeDal_PPPoE_Edit(Jobj, NULL);
	}
	else if(!strcmp(method, "GET")){
		ret = zcfgFeDal_PPPoE_Get(Jobj, Jarray, NULL);
	}
	else{
		printf("Unknown method:%s\n", method);
	}

	return ret;
}

