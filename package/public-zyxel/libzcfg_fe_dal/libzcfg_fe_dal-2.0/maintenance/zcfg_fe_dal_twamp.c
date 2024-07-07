#ifdef CONFIG_ZCFG_BE_MODULE_TWAMP
#include <ctype.h>
#include <json/json.h>
#include <time.h>
#include "zcfg_common.h"
#include "zcfg_fe_rdm_string.h"
#include "zcfg_fe_rdm_struct.h"
#include "zcfg_rdm_oid.h"
#include "zcfg_rdm_obj.h"
#include "zcfg_debug.h"
#include "zcfg_msg.h"
#include "zcfg_fe_dal_common.h"

dal_param_t TWAMP_param[]={
{"Enable",			dalType_boolean,	0,		0,	NULL},
{"TcpPort",			dalType_int,		0,	65535,	NULL},
{"UdpPortStart",	dalType_int,		0,	65535,	NULL},
{"UdpPortEnd",		dalType_int,		0,	65535,	NULL},
{"AuthMode",		dalType_string,		0,		0,	NULL,	"Open|Authenticated|Encrypted"},
{"Username",		dalType_string,		0,		64,	NULL},
{"Passphrase",		dalType_string,		0,		63,	NULL},
{NULL,				0,					0,		0,	NULL}
};


void zcfgFeDalShowTWAMP(struct json_object *Jarray){
	int i, len = 0;
	struct json_object *obj;
	
	if (json_object_get_type(Jarray) != json_type_array) {
		printf("wrong Jobj format!\n");
		return;
	}
	
	len = json_object_array_length(Jarray);
	for (i=0; i<len; i++) {
		obj = json_object_array_get_idx(Jarray, i);
		
		if (json_object_get_boolean(json_object_object_get(obj,"Enable")))
			printf("%-20s: Enabled\n", "TWAMP");
		else
			printf("%-20s: Disabled\n", "TWAMP");
		
		printf("%-20s: %s\n", "TcpPort", json_object_get_string(json_object_object_get(obj, "TcpPort")));
		printf("%-20s: %s\n", "UdpPortStart", json_object_get_string(json_object_object_get(obj, "UdpPortStart")));
		printf("%-20s: %s\n", "UdpPortEnd", json_object_get_string(json_object_object_get(obj, "UdpPortEnd")));
		printf("%-20s: %s\n", "AuthMode", json_object_get_string(json_object_object_get(obj, "AuthMode")));
		printf("%-20s: %s\n", "Username", json_object_get_string(json_object_object_get(obj, "Username")));
		printf("%-20s: %s\n", "Passphrase", json_object_get_string(json_object_object_get(obj, "Passphrase")));
	}
	return;
}

zcfgRet_t zcfgFeDal_TWAMP_Edit(struct json_object *Jobj, char *replyMsg){
	struct json_object *twampObj = NULL;
	objIndex_t objIid = {0};
	bool enable = false;
	int tcpport = 0, udpports = 0, udpporte;
	const char *str = NULL;
	zcfgRet_t ret = ZCFG_SUCCESS;
	
	IID_INIT(objIid);
	if (zcfgFeObjJsonGetWithoutUpdate(RDM_OID_ZY_TWAMP_REFLECTOR, &objIid, &twampObj) != ZCFG_SUCCESS)
		return ZCFG_INTERNAL_ERROR;
	
	if (json_object_object_get(Jobj, "Enable")) {
		enable = json_object_get_boolean(json_object_object_get(Jobj, "Enable"));
		json_object_object_add(twampObj, "Enable", json_object_new_boolean(enable));
	}
	
	if (json_object_object_get(Jobj, "TcpPort")) {
		tcpport = json_object_get_int(json_object_object_get(Jobj, "TcpPort"));
		json_object_object_add(twampObj, "TcpPort", json_object_new_int(tcpport));
	}
	
	if (json_object_object_get(Jobj, "UdpPortStart")) {
		udpports = json_object_get_int(json_object_object_get(Jobj, "UdpPortStart"));
		json_object_object_add(twampObj, "UdpPortStart", json_object_new_int(udpports));
	}
	
	if (json_object_object_get(Jobj, "UdpPortEnd")) {
		udpporte = json_object_get_int(json_object_object_get(Jobj, "UdpPortEnd"));
		json_object_object_add(twampObj, "UdpPortEnd", json_object_new_int(udpporte));
	}
		
	if (json_object_object_get(Jobj, "AuthMode")) {
		str = json_object_get_string(json_object_object_get(Jobj, "AuthMode"));
		json_object_object_add(twampObj, "AuthenticationMode", json_object_new_string(str));
	}
	
	if (json_object_object_get(Jobj, "Username")) {
		str = json_object_get_string(json_object_object_get(Jobj, "Username"));
		json_object_object_add(twampObj, "Username", json_object_new_string(str));
	}
	
	if (json_object_object_get(Jobj, "Passphrase")) {
		str = json_object_get_string(json_object_object_get(Jobj, "Passphrase"));
		json_object_object_add(twampObj, "KeyPassphrase", json_object_new_string(str));
	}
	
	ret = zcfgFeObjJsonSet(RDM_OID_ZY_TWAMP_REFLECTOR, &objIid, twampObj, NULL);
		
	zcfgFeJsonObjFree(twampObj);
	return ret;
}

zcfgRet_t zcfgFeDal_TWAMP_Get(struct json_object *Jobj, struct json_object *Jarray, char *replyMsg){
	struct json_object *retObj = NULL;
	struct json_object *twampObj = NULL;
	objIndex_t objIid = {0};
	
	IID_INIT(objIid);
	if (zcfgFeObjJsonGetWithoutUpdate(RDM_OID_ZY_TWAMP_REFLECTOR, &objIid, &twampObj) != ZCFG_SUCCESS)
		return ZCFG_INTERNAL_ERROR;

	retObj = json_object_new_object();
	json_object_object_add(retObj, "Enable", JSON_OBJ_COPY(json_object_object_get(twampObj, "Enable")));
	json_object_object_add(retObj, "TcpPort", JSON_OBJ_COPY(json_object_object_get(twampObj, "TcpPort")));
	json_object_object_add(retObj, "UdpPortStart", JSON_OBJ_COPY(json_object_object_get(twampObj, "UdpPortStart")));
	json_object_object_add(retObj, "UdpPortEnd", JSON_OBJ_COPY(json_object_object_get(twampObj, "UdpPortEnd")));
	json_object_object_add(retObj, "AuthMode", JSON_OBJ_COPY(json_object_object_get(twampObj, "AuthenticationMode")));
	json_object_object_add(retObj, "Username", JSON_OBJ_COPY(json_object_object_get(twampObj, "Username")));
	json_object_object_add(retObj, "Passphrase", JSON_OBJ_COPY(json_object_object_get(twampObj, "KeyPassphrase")));
	
	json_object_array_add(Jarray, retObj);
	
	zcfgFeJsonObjFree(twampObj);
	return ZCFG_SUCCESS;
}

zcfgRet_t zcfgFeDalTWAMP(const char *method, struct json_object *Jobj, struct json_object *Jarray, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;

	if(!strcmp(method, "PUT")){
		ret = zcfgFeDal_TWAMP_Edit(Jobj, NULL);
	}
	else if(!strcmp(method, "GET")){
		ret = zcfgFeDal_TWAMP_Get(Jobj, Jarray, NULL);
	}
	else{
		printf("Unknown method:%s\n", method);
	}
	return ret;
}

#endif
