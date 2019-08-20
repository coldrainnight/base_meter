#ifndef _AUTO_DATAPROCESS_H
#define _AUTO_DATAPROCESS_H

extern INT16S AUTO_Get (INT16U id, INT8U *pBuff);
extern INT8S AUTO_Set (INT16U id, INT8U *pData, INT16U len);
extern INT8S AUTO_Action (INT16U id, void * pdata, void * pbuff);
extern INT8U AUTO_GetFrmt (INT16U id);

#endif

