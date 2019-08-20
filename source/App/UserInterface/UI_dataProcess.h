#ifndef UI_DATAPROCESS_H
#define UI_DATAPROCESS_H

#include "PlatForm.h"
#include "UI.h"

#define  UI_SINGLE_LIST_NUM  (sizeof(stUiSingleDataList)/sizeof(ST_SINGLE_DATA_ATTR))
#define  UI_BLK_LIST_NUM  (sizeof(stUiBlkDataList)/sizeof(ST_BLK_DATA_ATTR))
#define  UI_ACTION_NUM  (sizeof(stUiActionList)/sizeof(ST_ACTION_ATTR))


extern INT16S UI_Get (INT16U id, INT8U *pBuff);
extern INT8S UI_Set (INT16U id, INT8U *pData, INT16U len);
extern INT8S UI_Action (INT16U id, void *pdata, void *pbuff);
extern INT8U UI_GetFrmt (INT16U id);
extern INT8U UI_ChkLCDParaValid(INT8U *pdata);
extern INT8U UI_GetMultiTermn(INT8U *pdata);



#endif

