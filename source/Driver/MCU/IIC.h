/*"***********************************************************		
FileName: IIC.h		
Author: ����       
Version : 1.0       Date: 2016.12.22	
Description: IIC����ʵ��
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	


#ifndef IIC_H
#define IIC_H

#include "system\Sys_def.h"


/*����IIC����ID*/

#define IIC_ID_EEP       (0)
#define IIC_ID_LCD       (1)
#define IIC_ID_HTSENSOR  (2) /*�¶ȴ�����*/
#define IIC_ID_ERTC		 (3) /*�ⲿʱ��оƬ 8025t*/


extern void IIC_Init(INT8U Iic_Id);
extern void IIC_Close(INT8U Iic_Id);

extern void IIC_Start (INT8U Iic_Id);
extern void IIC_Stop(INT8U Iic_Id);
extern INT8U  IIC_SendChar(INT8U IicId,INT8U * Buff,INT16U Len);
extern INT8U  IIC_ReadChar(INT8U IicId,INT8U * Buff,INT16U Len);
#endif

