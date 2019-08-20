#ifndef _CRC_H_
#define _CRC_H_
typedef  INT16U 	CRC_DATA_TP;
typedef  INT16U 	FTR_DATA_TP;		//FEATRUE
//typedef  INT32U 	CRC_DATA_TP;
extern CRC_DATA_TP CRC16(INT8U *startaddr, INT16U len);

#endif
