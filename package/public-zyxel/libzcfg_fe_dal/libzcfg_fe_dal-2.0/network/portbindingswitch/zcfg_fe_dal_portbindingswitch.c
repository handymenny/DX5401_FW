#include <ctype.h>
#include <json/json.h>
#include <time.h>
#include <string.h>
#include <arpa/inet.h>

#include "zcfg_common.h"
#include "zcfg_fe_rdm_string.h"
#include "zcfg_fe_rdm_struct.h"
#include "zcfg_rdm_oid.h"
#include "zcfg_rdm_obj.h"
#include "zcfg_debug.h"
#include "zcfg_msg.h"
#include "zcfg_fe_dal_common.h"

#define JSON_OBJ_COPY(json_object) json_tokener_parse(json_object_to_json_string(json_object))

dal_param_t portBindingSwitch_param[] = {
	{"mode",				dalType_int, 	0, 0, NULL,	NULL,	NULL},
	{NULL,		0,	0,	0,	NULL}
};

zcfgRet_t zcfgFeDalPortBindingSwitchSet(json_object *Jobj, char *replyMsg){
	json_object *mgmtSrvObj = NULL;
	objIndex_t mgmtSrvIid = {0};
	const char mode[16] = {0};
	
	IID_INIT(mgmtSrvIid);
	
	if(zcfgFeObjJsonGetWithoutUpdate(RDM_OID_MGMT_SRV, &mgmtSrvIid, &mgmtSrvObj) == ZCFG_SUCCESS){
		int modeindex = json_object_get_int(json_object_object_get(Jobj,"mode"));
		if(modeindex == 0)
			snprintf(mode, 16, "Internet");
		else if(modeindex == 1)
			snprintf(mode, 16, "Single");
		else if(modeindex == 2)
			snprintf(mode, 16, "Multi2");
		else if(modeindex == 3)
			snprintf(mode, 16, "Multi3");

		json_object_object_add(mgmtSrvObj, "X_ZYXEL_TripplePlay", json_object_new_string(mode));
        if(zcfgFeObjJsonBlockedSet(RDM_OID_MGMT_SRV, &mgmtSrvIid, mgmtSrvObj, NULL) != ZCFG_SUCCESS){
			printf("ERROR!! RDM_OID_MGMT_SRV set failed\n");
		}
		zcfgFeJsonObjFree(mgmtSrvObj);
		return ZCFG_SUCCESS;
	}
}
zcfgRet_t zcfgFeDalPortBindingSwitchGet(struct json_object *Jobj, struct json_object *Jarray, char *replyMsg){
	struct json_object *paramJobj = NULL;
	json_object *mgmtSrvObj = NULL;
	objIndex_t mgmtSrvIid = {0};
	const char *mode = NULL;
	
	IID_INIT(mgmtSrvIid);
	paramJobj = json_object_new_object();
	
	if(zcfgFeObjJsonGetWithoutUpdate(RDM_OID_MGMT_SRV, &mgmtSrvIid, &mgmtSrvObj) == ZCFG_SUCCESS){
		mode = json_object_get_string(json_object_object_get(mgmtSrvObj,"X_ZYXEL_TripplePlay"));
		if(!strcmp(mode, "Internet"))
			json_object_object_add(paramJobj, "mode", json_object_new_int(0));
		else if(!strcmp(mode, "Single"))
			json_object_object_add(paramJobj, "mode", json_object_new_int(1));
		else if(!strcmp(mode, "Multi2"))
			json_object_object_add(paramJobj, "mode", json_object_new_int(2));
		else if(!strcmp(mode, "Multi3"))
			json_object_object_add(paramJobj, "mode", json_object_new_int(3));
		
		json_object_array_add(Jarray, JSON_OBJ_COPY(paramJobj));
		zcfgFeJsonObjFree(mgmtSrvObj);
		zcfgFeJsonObjFree(paramJobj);
		return ZCFG_SUCCESS;
	}
}

zcfgRet_t zcfgFeDalPortBindingSwitch(const char *method, struct json_object *Jobj, struct json_object *Jarray, char *replyMsg) {
	zcfgRet_t ret = ZCFG_SUCCESS;
	
	if(!strcmp(method, "PUT"))
		ret = zcfgFeDalPortBindingSwitchSet(Jobj, NULL);
	else if(!strcmp(method, "GET"))
		ret = zcfgFeDalPortBindingSwitchGet(Jobj, Jarray, NULL);
	else
		printf("Unknown method:%s\n", method);


	return ret;
}