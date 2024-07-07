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

dal_param_t Fsecure_param[]={
	{"sense_enable",	dalType_boolean,	0,	0,	NULL, NULL, dal_Add_ignore},
	{"license_key",	dalType_string,	0,	0,	NULL, NULL, dal_Add_ignore},
	{"license_reset_key",	dalType_string,	0,	0,	NULL, NULL, dal_Add_ignore},
	{"activation_status",	dalType_string,	0,	0,	NULL, NULL, dal_Add_ignore},
	{"license_status",	dalType_string,	0,	0,	NULL, NULL, dal_Add_ignore},
	{NULL,		0,	0,	0,	NULL}
};

void zcfgFeDalShowFsecure(struct json_object *Jarray){
	struct json_object *Jobj;

	if(json_object_get_type(Jarray) != json_type_array){
		printf("wrong Jobj format!\n");
		return;
	}

	if((Jobj = json_object_array_get_idx(Jarray, 0)) == NULL)
		return;

	printf("Secure License %s\n", Jgetb(Jobj, "sense_enable")?"Enabled":"Disabled");
	printf("Secure Key %s\n", Jgets(Jobj, "license_key"));
	printf("Secure Reset Key %s\n", Jgets(Jobj, "license_reset_key"));
	printf("Secure Activation Status %s\n", Jgets(Jobj, "activation_status"));
	printf("Secure License Status %s\n", Jgets(Jobj, "license_status"));
}

zcfgRet_t zcfgFeDalFsecureGet(struct json_object *Jobj, struct json_object *Jarray, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;

	objIndex_t objIid;
	struct json_object *obj = NULL;
	struct json_object *pramJobj = NULL;

	IID_INIT(objIid);
	if(zcfgFeObjJsonGetWithoutUpdate(RDM_OID_F_SECURE, &objIid, &obj) == ZCFG_SUCCESS){
		pramJobj = json_object_new_object();
		replaceParam(pramJobj, "sense_enable", obj, "SENSE_Enable");
		replaceParam(pramJobj, "license_key", obj, "SENSE_License_Key");
		replaceParam(pramJobj, "license_reset_key", obj, "SENSE_Reset_Key");
		replaceParam(pramJobj, "activation_status", obj, "SENSE_Activation_Status");
		replaceParam(pramJobj, "license_status", obj, "SENSE_License_Status");

		json_object_array_add(Jarray, pramJobj);
		zcfgFeJsonObjFree(obj);
	}
	return ret;
}

zcfgRet_t zcfgFeDalFsecureEdit(struct json_object *Jobj, char *replyMsg){

	zcfgRet_t ret = ZCFG_SUCCESS;
	struct json_object *fsecureObj = NULL;
	struct json_object *paramObj = NULL;
	objIndex_t fsecureIid;

	bool senseEnable;
	char *licenseKey = NULL;
	char *resetKey = NULL;
	char *activationStatus = NULL;
	char *licenseStatus = NULL;

	bool enable;
	char *oldKey = NULL;
	char *oldResetKey = NULL;
	char *oldActivationStatus = NULL;
	char *oldLicenseStatus = NULL;
	bool changed = 0;

	IID_INIT(fsecureIid);

	if((ret = zcfgFeObjJsonGet(RDM_OID_F_SECURE, &fsecureIid, &fsecureObj)) != ZCFG_SUCCESS) {
		json_object_object_add(Jobj, "__multi_lang_replyMsg", json_object_new_string("zylang.Parental_Control.error.get_fsecure_err"));
		//if(pramJobj) json_object_put(pramJobj);		
		return ret;
	}

	paramObj = json_object_object_get(Jobj, "sense_enable");
	if (paramObj != NULL)
		senseEnable  = json_object_get_boolean(paramObj);
	else
		senseEnable = Jgetb(fsecureObj, "SENSE_Enable");

	paramObj = json_object_object_get(Jobj, "license_key");
	if (paramObj != NULL)
		licenseKey = (char *)json_object_get_string(paramObj);
	else
		licenseKey = (char *)Jgets(fsecureObj, "SENSE_License_Key");

	paramObj = json_object_object_get(Jobj, "license_reset_key");
	if (paramObj != NULL)
		resetKey = (char *)json_object_get_string(paramObj);
	else
		resetKey = (char *)Jgets(fsecureObj, "SENSE_Reset_Key");

	paramObj = json_object_object_get(Jobj, "activation_status");
	if (paramObj != NULL)
		activationStatus = (char *)json_object_get_string(paramObj);
	else
		activationStatus = (char *)Jgets(fsecureObj, "SENSE_Activation_Status");

	paramObj = json_object_object_get(Jobj, "license_status");
	if (paramObj != NULL)
		licenseStatus = (char *)json_object_get_string(paramObj);
	else
		licenseStatus = (char *)Jgets(fsecureObj, "SENSE_License_Status");

	enable = Jgetb(fsecureObj, "SENSE_Enable");
	oldKey = (char *)Jgets(fsecureObj, "SENSE_License_Key");
	oldResetKey = (char *)Jgets(fsecureObj, "SENSE_Reset_Key");
	oldActivationStatus = (char *)Jgets(fsecureObj, "SENSE_Activation_Status");
	oldLicenseStatus = (char *)Jgets(fsecureObj, "SENSE_License_Status");

	if ( enable != senseEnable ) {
		json_object_object_add(fsecureObj, "SENSE_Enable", json_object_new_boolean(senseEnable));
		changed = 1;
	}
	if (strcmp(licenseKey,oldKey)!=0){
		json_object_object_add(fsecureObj, "SENSE_License_Key", json_object_new_string(licenseKey));
		changed = 1;
	}
	if (strcmp(resetKey,oldResetKey)!=0){
		json_object_object_add(fsecureObj, "SENSE_Reset_Key", json_object_new_string(resetKey));
		changed = 1;
	}
	if (strcmp(activationStatus,oldActivationStatus)!=0){
		json_object_object_add(fsecureObj, "SENSE_Activation_Status", json_object_new_string(activationStatus));
		changed = 1;
	}
	if (strcmp(licenseStatus,oldLicenseStatus)!=0){
		json_object_object_add(fsecureObj, "SENSE_License_Status", json_object_new_string(licenseStatus));
		changed = 1;
	}

	if(changed)
	{
		if((ret = zcfgFeObjJsonSet(RDM_OID_F_SECURE, &fsecureIid, fsecureObj, NULL)) != ZCFG_SUCCESS){
			json_object_object_add(Jobj, "__multi_lang_replyMsg", json_object_new_string("zylang.Parental_Control.error.set_fsecure_err"));
			if(fsecureObj) json_object_put(fsecureObj);
			return ret;
		}
	}

	if(fsecureObj) json_object_put(fsecureObj);

	return ZCFG_SUCCESS;
}

zcfgRet_t zcfgFeDalFsecure(const char *method, struct json_object *Jobj, struct json_object *Jarray, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;
	if(!strcmp(method, "PUT"))
		ret = zcfgFeDalFsecureEdit(Jobj, replyMsg);
	else if(!strcmp(method, "GET"))
		ret = zcfgFeDalFsecureGet(Jobj, Jarray, NULL);
	else
		printf("Unknown method:%s\n", method);
	return ret;
}