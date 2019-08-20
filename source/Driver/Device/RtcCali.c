/*"***********************************************************  
FileName: RTC_CALIB.c  
Author: 韩潇俊       
Version : 1.0       Date: 2017.11.21 
Description: RTC补偿设备层接口
Function List:   
<FunctionName>  
<author> <time> <version > <desc>  
 
***********************************************************"*/
#include "system\Sys_def.h"

#include "Driver\Device\RtcCali.h"
#include "APP\LegallyRelevant\Rtc_CaliApp.h"


INT16U temp_ad_buffer[8];	/*"ad采样数据缓冲区"*/	

/*"*********************************************************"*/
/*"Function:   RTC_CalibImplement "*/   
/*"Description: ppm值补偿"*/
/*"Author: hanxiaojun"*/
/*"Input:"*/ 
/*"Output:"*/ 
/*"Return:"*/ 
/*"*********************************************************"*/
void RTC_CalibImplement (INT8U mode,INT32S value, INT32U interval)
{
    INT8U i;
    INT32U temp_value;
    INT32S fa,ppm_value;

    if( mstRtcPara.amendpara.amend_flag == 0xAA55 )/*"送检修调模式"*/
    {
        if( (mstRtcMeadata.temperature_ad >= mstRtcPara.amendpara.pata_value[0]) && (mstRtcMeadata.temperature_ad <= mstRtcPara.amendpara.pata_value[_GROUP_SIZE-1]) )/*"进入查表区域"*/
        {				
            for(i=1; i<_GROUP_SIZE; i++)/*"温度区间判断"*/
            {
                if( mstRtcMeadata.temperature_ad <= mstRtcPara.amendpara.pata_value[i] )
                {
                    break;
                }
            }

            /*"补偿值计算"*/
            fa = (INT32S)((mstRtcPara.amendpara.ppm_value[i-1] - mstRtcPara.amendpara.ppm_value[i])*100/((INT32S)mstRtcPara.amendpara.pata_value[i-1] - (INT32S)mstRtcPara.amendpara.pata_value[i]));
            ppm_value = ((fa*((INT32S)(mstRtcMeadata.temperature_ad - mstRtcPara.amendpara.pata_value[i-1])))/100);
            ppm_value = ppm_value + (INT32S)mstRtcPara.amendpara.ppm_value[i-1];
            value = ppm_value;
        }
        else/*"温度区间以外，按正常模式进行补偿"*/
        {			
            
        }
    }
    else if( mstRtcPara.amendpara.amend_flag == 0x55AA )/*"修调校准模式"*/
    {
       value = temp_ppm_value;
    }
    else/*"非修调校准模式,按正常模式进行补偿"*/
    {

    }

    if(mstRtcMeadata.last_calivalue != value)
    { 
        mstRtcMeadata.last_calivalue = value;
    
    	if((value & 0x80000000) == 0x80000000)/*负数*/
        {
            RTC->ADSIGN |= 0x00000001; /*增加补偿*/
            temp_value = ~value+1;
        }   
    	else/*正数*/
        {
            RTC->ADSIGN &= 0x00000000; /*减少补偿*/
            temp_value = value;
        }

        if(temp_value>=MAX_CALI_VALUE)
        {
            RTC->ADJUST = MAX_CALI_VALUE;
        }
        else
        {
            RTC->ADJUST = temp_value;
        }
    }
}
/*"*********************************************************"*/
/*"Function:   RTC_CalTempVal "*/   
/*"Description: 获取温度采样AD值"*/
/*"Author: hanxiaojun"*/
/*"Input:"*/ 
/*"Output:"*/ 
/*"Return:"*/ 
/*"*********************************************************"*/

INT16S RTC_CalTempVal (void)
{
	INT16U adValueTemp;
    INT16S temp_value;
    INT32S ad_ad0;
 	INT8U  i;
    INT16U Adc30Val;
        
  	adValueTemp=0;
    ad_ad0=0;
    Adc30Val = *(INT16U*)ADC_30C_L;
    
  	for(i=0;i<8;i++)
    {
     	adValueTemp+=temp_ad_buffer[i];
    }
    adValueTemp>>=3;/*"求8秒平均值"*/

    mstRtcMeadata.temperature_ad = adValueTemp;

    ad_ad0 = (INT32S)adValueTemp - Adc30Val;//(SINT32)ad0ValueTemp;
    ad_ad0 *= 100000;
    ad_ad0 /= 50875;
    ad_ad0 += 300;
    temp_value = (INT16S)ad_ad0;/*"有符号数、带一位小数"*/
    
	return(temp_value);
}

void RTC_Cali_TempSen_On (INT8U mode)
{
	 
}

void RTC_Cali_TempSen_Off (void)
{

}

