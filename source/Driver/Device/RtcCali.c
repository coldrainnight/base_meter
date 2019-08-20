/*"***********************************************************  
FileName: RTC_CALIB.c  
Author: ���쿡       
Version : 1.0       Date: 2017.11.21 
Description: RTC�����豸��ӿ�
Function List:   
<FunctionName>  
<author> <time> <version > <desc>  
 
***********************************************************"*/
#include "system\Sys_def.h"

#include "Driver\Device\RtcCali.h"
#include "APP\LegallyRelevant\Rtc_CaliApp.h"


INT16U temp_ad_buffer[8];	/*"ad�������ݻ�����"*/	

/*"*********************************************************"*/
/*"Function:   RTC_CalibImplement "*/   
/*"Description: ppmֵ����"*/
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

    if( mstRtcPara.amendpara.amend_flag == 0xAA55 )/*"�ͼ��޵�ģʽ"*/
    {
        if( (mstRtcMeadata.temperature_ad >= mstRtcPara.amendpara.pata_value[0]) && (mstRtcMeadata.temperature_ad <= mstRtcPara.amendpara.pata_value[_GROUP_SIZE-1]) )/*"����������"*/
        {				
            for(i=1; i<_GROUP_SIZE; i++)/*"�¶������ж�"*/
            {
                if( mstRtcMeadata.temperature_ad <= mstRtcPara.amendpara.pata_value[i] )
                {
                    break;
                }
            }

            /*"����ֵ����"*/
            fa = (INT32S)((mstRtcPara.amendpara.ppm_value[i-1] - mstRtcPara.amendpara.ppm_value[i])*100/((INT32S)mstRtcPara.amendpara.pata_value[i-1] - (INT32S)mstRtcPara.amendpara.pata_value[i]));
            ppm_value = ((fa*((INT32S)(mstRtcMeadata.temperature_ad - mstRtcPara.amendpara.pata_value[i-1])))/100);
            ppm_value = ppm_value + (INT32S)mstRtcPara.amendpara.ppm_value[i-1];
            value = ppm_value;
        }
        else/*"�¶��������⣬������ģʽ���в���"*/
        {			
            
        }
    }
    else if( mstRtcPara.amendpara.amend_flag == 0x55AA )/*"�޵�У׼ģʽ"*/
    {
       value = temp_ppm_value;
    }
    else/*"���޵�У׼ģʽ,������ģʽ���в���"*/
    {

    }

    if(mstRtcMeadata.last_calivalue != value)
    { 
        mstRtcMeadata.last_calivalue = value;
    
    	if((value & 0x80000000) == 0x80000000)/*����*/
        {
            RTC->ADSIGN |= 0x00000001; /*���Ӳ���*/
            temp_value = ~value+1;
        }   
    	else/*����*/
        {
            RTC->ADSIGN &= 0x00000000; /*���ٲ���*/
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
/*"Description: ��ȡ�¶Ȳ���ADֵ"*/
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
    adValueTemp>>=3;/*"��8��ƽ��ֵ"*/

    mstRtcMeadata.temperature_ad = adValueTemp;

    ad_ad0 = (INT32S)adValueTemp - Adc30Val;//(SINT32)ad0ValueTemp;
    ad_ad0 *= 100000;
    ad_ad0 /= 50875;
    ad_ad0 += 300;
    temp_value = (INT16S)ad_ad0;/*"�з���������һλС��"*/
    
	return(temp_value);
}

void RTC_Cali_TempSen_On (INT8U mode)
{
	 
}

void RTC_Cali_TempSen_Off (void)
{

}

