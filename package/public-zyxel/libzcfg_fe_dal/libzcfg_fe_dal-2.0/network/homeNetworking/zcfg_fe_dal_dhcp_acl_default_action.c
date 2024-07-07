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

dal_param_t DHCP_ACL_DEF_ACT_param[] =
{
	{"ActionNonVlan",               dalType_string,     0, 	0,	NULL,	NULL, 	dal_Edit},
	{"ActionVlan",                  dalType_string,     0, 	0, 	NULL,	NULL,	dal_Edit},
	{NULL,                          0,                  0, 	0, 	NULL,	NULL,	NULL}
};

void zcfgFeDalShowDHCPAclDefAct(struct json_object *Jarray){
	int i, len = 0;
	struct json_object *obj = NULL;
	

	if(json_object_get_type(Jarray) != json_type_array){
		printf("wrong Jobj format!\n");
		return;
	}
	printf("%-15s %-15s\n", "Type", "Action");
	len = json_object_array_length(Jarray);
	for(i=0; i<len;i++){
		obj = json_object_array_get_idx(Jarray, i);
		printf("%-15s %-15s\n",	"ActionNonVlan", json_object_get_string(json_object_object_get(obj, "ActionNonVlan")));
		printf("%-15s %-15s\n",	"ActionVlan", json_object_get_string(json_object_object_get(obj, "ActionVlan")));		
	}
	
}


zcfgRet_t zcfgFeDalDHCP_Acl_Defautl_Action_Edit(struct json_object *Jobj, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;
	struct json_object *dhcpAclDefActObj = NULL;
	objIndex_t dhcpAclDefActIid = {0};
	const char *ActionNonVlan = NULL;
	const char *ActionVlan = NULL;

	IID_INIT(dhcpAclDefActIid);	
	if(zcfgFeObjJsonGet(RDM_OID_DHCP_ACL_DEFAULT_ACTION, &dhcpAclDefActIid, &dhcpAclDefActObj) == ZCFG_SUCCESS) {
		ActionNonVlan = json_object_get_string(json_object_object_get(Jobj, "ActionNonVlan"));
		ActionVlan = json_object_get_string(json_object_object_get(Jobj, "ActionVlan"));
		
		json_object_object_add(dhcpAclDefActObj, "ActionNonVlan", json_object_new_string(ActionNonVlan));
		json_object_object_add(dhcpAclDefActObj, "ActionVlan", json_object_new_string(ActionVlan));
	
		ret = zcfgFeObjJsonSet(RDM_OID_DHCP_ACL_DEFAULT_ACTION, &dhcpAclDefActIid, dhcpAclDefActObj, NULL);
		zcfgFeJsonObjFree(dhcpAclDefActObj);
	}
	return ret;
}

void getDHCPAclDefault_Action_Obj(int iid, struct json_object *dhcpAclDefActObj, struct json_object *Jarray){
	struct json_object *paramJobj = NULL;
	
	paramJobj = json_object_new_object();
	
	json_object_object_add(paramJobj, "ActionNonVlan", JSON_OBJ_COPY(json_object_object_get(dhcpAclDefActObj,"ActionNonVlan")));
	json_object_object_add(paramJobj, "ActionVlan", JSON_OBJ_COPY(json_object_object_get(dhcpAclDefActObj,"ActionVlan")));
	
	json_object_array_add(Jarray, paramJobj);	
	zcfgFeJsonObjFree(dhcpAclDefActObj);


}

zcfgRet_t zcfgFeDalDHCP_Acl_Defautl_Action_Get(struct json_object *Jobj, struct json_object *Jarray, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;

	struct json_object *dhcpAclDefActObj = NULL;	//Get data model parameter
	objIndex_t dhcpAclDefActIid = {0};

	IID_INIT(dhcpAclDefActIid);
	if(zcfgFeObjJsonGet(RDM_OID_DHCP_ACL_DEFAULT_ACTION, &dhcpAclDefActIid, &dhcpAclDefActObj) == ZCFG_SUCCESS){	
		getDHCPAclDefault_Action_Obj(dhcpAclDefActIid.idx[0], dhcpAclDefActObj, Jarray);			
	}
	return ret;
}


zcfgRet_t zcfgFeDalDHCPAclDefAct(const char *method, struct json_object *Jobj, struct json_object *Jarray, char *replyMsg){
	zcfgRet_t ret = ZCFG_SUCCESS;

	if(!method || !Jobj)
		return ZCFG_INTERNAL_ERROR;

	if(!strcmp(method, "PUT")) {
		ret = zcfgFeDalDHCP_Acl_Defautl_Action_Edit(Jobj, NULL);
	}else if(!strcmp(method, "GET"))
		ret = zcfgFeDalDHCP_Acl_Defautl_Action_Get(Jobj, Jarray, NULL);
	else
		printf("Unknown method:%s\n", method);

	return ret;
}
