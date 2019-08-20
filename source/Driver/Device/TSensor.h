/*"***********************************************************		
FileName: TSensor.h		
Author: ����       
Version : 1.0       Date: 2018.07.23	
Description: �¶ȴ����� DS18B20 �豸������
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>			
***********************************************************"*/
#ifndef TSENSOR_H
#define TSENSOR_H
#include "system\Sys_def.h"

typedef enum
{
   E_TEMP_L=0,  /*"�����¶�"*/
   E_TEMP_N,     /*"�����¶�"*/
   E_TEMP_MAX
}E_TEMP_TYPE;
extern INT32S gTempVal[E_TEMP_MAX];


extern void TSensor_Init(void);

extern void TSensorSecTask(void);


#endif  

