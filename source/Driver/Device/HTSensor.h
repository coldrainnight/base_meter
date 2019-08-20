/*"***********************************************************		
FileName: TSensor.h		
Author: ����       
Version : 1.0       Date: 2018.07.23	
Description: �¶ȴ����� DS18B20 �豸������
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>			
***********************************************************"*/
#ifndef HTSENSOR_H
#define HTSENSOR_H
#include "system\Sys_def.h"

typedef struct 
{
    INT16S Temperature; /*�¶�,��λС��*/
    INT16U Humidity;    /*ʪ�� 1λС��*/
}ST_TH_VAL;

extern INT8U HTSensor_Init(void);
extern INT8U Get_HTSensorVal(INT8U t, ST_TH_VAL * ThVal);
#endif  

