/*"***********************************************************		
FileName: TSensor.h		
Author: 邹杨       
Version : 1.0       Date: 2018.07.23	
Description: 温度传感器 DS18B20 设备层驱动
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>			
***********************************************************"*/
#ifndef HTSENSOR_H
#define HTSENSOR_H
#include "system\Sys_def.h"

typedef struct 
{
    INT16S Temperature; /*温度,两位小数*/
    INT16U Humidity;    /*湿度 1位小数*/
}ST_TH_VAL;

extern INT8U HTSensor_Init(void);
extern INT8U Get_HTSensorVal(INT8U t, ST_TH_VAL * ThVal);
#endif  

