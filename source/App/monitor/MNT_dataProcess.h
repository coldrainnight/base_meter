#ifndef MNT_DATAPROCESS_H
#define MNT_DATAPROCESS_H

#include "PlatForm.h"
#include "MNT.h"

#define  MNT_SINGLE_LIST_NUM  (sizeof(stMntSingleDataList)/sizeof(ST_SINGLE_DATA_ATTR))
#define  MNT_BLK_LIST_NUM  (sizeof(stMntBlkDataList)/sizeof(ST_BLK_DATA_ATTR))
#define  MNT_ACTION_NUM  (sizeof(stMntActionList)/sizeof(ST_ACTION_ATTR))

extern INT16S MNT_Get (INT16U id, INT8U *pBuff);
extern INT8S MNT_Set (INT16U id, INT8U *pData, INT16U len);
extern INT8S MNT_Action (INT16U id, void * pdata, void * pbuff);
extern INT8U MNT_GetFrmt (INT16U id);

#endif
