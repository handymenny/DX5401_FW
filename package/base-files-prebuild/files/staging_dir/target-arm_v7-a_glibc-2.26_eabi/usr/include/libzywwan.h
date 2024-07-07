#ifndef _LIBZYWWAN_H
#define _LIBZYWWAN_H

#include "zcfg_common.h"
#include "zcfg_rdm_oid.h"
#include "zcfg_rdm_obj.h"
#include "zcfg_msg.h"
#include "zcfg_debug.h"
#include "zyutil.h"
#include "zcfg_fe_rdm_access.h"
#include "zcfg_fe_rdm_struct.h"

void setWwanVersion(void);
bool zy_WWANPackageCheckDecryption(const FILE *imageFileFp, unsigned int packageLen, char *PATH);

#endif
