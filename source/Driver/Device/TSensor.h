/*"***********************************************************		
FileName: TSensor.h		
Author: 邹杨       
Version : 1.0       Date: 2018.07.23	
Description: 温度传感器 DS18B20 设备层驱动
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>			
***********************************************************"*/
#ifndef TSENSOR_H
#define TSENSOR_H
#include "system\Sys_def.h"

typedef enum
{
   E_TEMP_L=0,  /*"火线温度"*/
   E_TEMP_N,     /*"零线温度"*/
   E_TEMP_MAX
}E_TEMP_TYPE;
extern INT32S gTempVal[E_TEMP_MAX];


extern void TSensor_Init(void);

extern void TSensorSecTask(void);


#endif  

