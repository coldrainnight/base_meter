/*"***********************************************************		
FileName: IIC.h		
Author: 邹杨       
Version : 1.0       Date: 2016.12.22	
Description: IIC驱动实现
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	


#ifndef IIC_H
#define IIC_H

#include "system\Sys_def.h"


/*定义IIC总线ID*/

#define IIC_ID_EEP       (0)
#define IIC_ID_LCD       (1)
#define IIC_ID_HTSENSOR  (2) /*温度传感器*/
#define IIC_ID_ERTC		 (3) /*外部时钟芯片 8025t*/


extern void IIC_Init(INT8U Iic_Id);
extern void IIC_Close(INT8U Iic_Id);

extern void IIC_Start (INT8U Iic_Id);
extern void IIC_Stop(INT8U Iic_Id);
extern INT8U  IIC_SendChar(INT8U IicId,INT8U * Buff,INT16U Len);
extern INT8U  IIC_ReadChar(INT8U IicId,INT8U * Buff,INT16U Len);
#endif

