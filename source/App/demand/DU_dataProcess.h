#ifndef DU_DATAPROCESS_H
#define DU_DATAPROCESS_H
/*"*******************************************************************************"*/
#include "PlatForm.h"
#include "DU.h"
/*"*******************************************************************************"*/
#define  DU_SINGLE_LIST_NUM (sizeof(stDuSingleDataList)/sizeof(ST_SINGLE_DATA_ATTR))
#define  DU_BLK_LIST_NUM    (sizeof(stDuBlkDataList)/sizeof(ST_BLK_DATA_ATTR))
#define  DU_ACTION_NUM      (sizeof(stDuActionList)/sizeof(ST_ACTION_ATTR))

extern INT16S DU_Get (INT16U id, INT8U *pBuff);
extern INT8S  DU_Set (INT16U id, INT8U *pData, INT16U len);
extern INT8S  DU_Action (INT16U id, void * pdata, void * pbuff);
extern INT8U  DU_GetFrmt (INT16U id);

/*"*******************************************************************************"*/

#endif
