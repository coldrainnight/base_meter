#ifndef LRM_DATAPROCESS_H
#define LRM_DATAPROCESS_H

#include "PlatForm.h"
#include "LRM.h"

#define  LRM_SINGLE_LIST_NUM  (sizeof(stLrmSingleDataList)/sizeof(ST_SINGLE_DATA_ATTR))
#define  LRM_BLK_LIST_NUM  (sizeof(stLrmBlkDataList)/sizeof(ST_BLK_DATA_ATTR))
#define  LRM_ACTION_NUM  (sizeof(stLrmActionList)/sizeof(ST_ACTION_ATTR))

extern INT16S LRM_Get (INT16U id, INT8U *pBuff);
extern INT8S LRM_Set (INT16U id, INT8U *pData, INT16U len);
extern INT8S LRM_Action (INT16U id, void * pdata, void * pbuff);
extern INT8U LRM_GetFrmt (INT16U id);

#endif
