/*"***********************************************************  
FileName: RTC_CALIAPP.c  
Author: ���쿡       
Version : 1.0       Date: 2017.10.23 
Description: RTCӦ�ò�ӿڼ�FM33A0ƽ̨ʵ��
Function List:   
<FunctionName>  
<author> <time> <version > <desc>  
 
***********************************************************"*/
#include "system\Sys_def.h"
#include "APP\LegallyRelevant\Rtc_CaliApp.h"
#include "APP\LegallyRelevant\LRM.h"
#include "Driver\DriverComm.h"
/*" 											"*/  
/*"				    ��������			        "*/  
/*"											    "*/  
void RTC_TempMeter_ErrComp(INT8U mode);
void RTC_CrystalCompenstion(INT8U mode);
void RTC_CalcErr(void);
void  RTC_setCrystalErr(INT16S        err100);
void  RTC_setTemperatureErr(INT16S temp, ST_RTC_PARA *p);
void RTC_CalcPPM(INT8U Mode);
void RTC_CaliInit (INT8U Mode);
void RTC_RamDataInit(void);
void RTC_RamDataInit(void);
void RTC_ParaInit(void);
INT8U RTC_GetEeromFlag();
void RTC_Defult_Para();
void RTC_DataFlashInit();
void RTC_CaliMain (INT8U Mode);
void RTC_SampAndCali( INT8U Mode,INT16U InterVal);
void RTC_RunEvery1s(void);
INT8U RTC_RtcCtrlCmd(INT8U *pBuff);
INT8U RTC_RtcSave(INT8U *pBuff);
INT8U RTC_FlashDataInit(INT8U *pBuff);
INT8U RTC_WRtcPara(INT8U *pBuff);
INT8U RTC_RRtcPara(INT8U *pBuff);
INT8U RTC_RAmendStatus(INT8U *pBuff);
INT8U RTC_RDataTempAD(INT8U *pBuff);
INT8U RTC_RDataTempVal(INT8U *pBuff);
INT8U RTC_RDataCaliPPM(INT8U *pBuff);
INT8U RTC_CaliRead (INT8U *pBuff);
INT8U RTC_CaliWrite (INT8U *pBuff);
INT8U RTC_CaliCtrl (INT8U *pBuff);
/*" 											"*/  
/*"				    ��������			            "*/  
/*"											    "*/ 
INT8U 				calictrl;
INT32U              temp_ppm_value;


NO_INIT ST_RTC_PARA			mstRtcPara;
ST_RTC_CALITIME	   	mstRtcCaliTime;
ST_RTC_MEADATA		mstRtcMeadata;
/************************************************************  
						RTC�����㷨

*************************************************************/
void RTC_TempMeter_ErrComp(INT8U mode)
{
    mstRtcMeadata.temperature_value = RTC_CalTempVal();
    if( mode == PWRMODE_DOWN )
    {
        mstRtcMeadata.temperature_value += 15;/*"������±��ϵ������"*/
    }
    RTC_CrystalCompenstion(mode);
}

void RTC_CrystalCompenstion(INT8U mode)
{
    RTC_CalcErr();
    RTC_CalcPPM(mode);
}
/*"************************************************			
Function:CrystalCompenstion
Description:  ���þ����ʼƵƫ�������¶������Ƶ����� 
Input:  ��
Output: 	��
Return:��	
Others: 		
************************************************"*/
void RTC_CalcErr(void)
{
	RTC_setCrystalErr(mstRtcPara.calipara.crystalErr); 
	RTC_setTemperatureErr(mstRtcMeadata.temperature_value, &mstRtcPara);
}
/*"************************************************			
Function:RTC_setCrystalErr
Description: ���þ���ĳ�ʼ����ֵ
Input:  err100����λ:0.01ppm 
Output: 	��
Return:��	
Others: 		
************************************************"*/
void  RTC_setCrystalErr(INT16S        err100)
{
	mstRtcMeadata.init_ppm =  err100;   
}
/*"************************************************			
Function:RTC_setCrystalErr
Description: ���㾧���¶����ֵ
Input: temp ��λ:0.1���϶� 
        *p ���������б��׵�ַ
Output: 	��
Return:��	
Others: 		
************************************************"*/
void  RTC_setTemperatureErr(INT16S temp, ST_RTC_PARA *p)
{   
	INT16S compensation_fact;
	INT32S temp_reg1;
	INT32S temp_reg2;

	if(temp >= ((*p).calipara.temperatureTop)) //when the temperature higher than 24 ,the value of Y=A(T-T25); of A is -3.5/100 
	{
		compensation_fact = (*p).calipara.crystalFactHigh;
	}
	else //when the temperature lower than 24 ,the value of Y=A(T-T25); of A is -2.8/100 
	{
		compensation_fact = (*p).calipara.crystalFactLow;
	}
	temp_reg1 =temp - ((*p).calipara.temperatureTop);
	temp_reg2 = temp_reg1;
    
	temp_reg1 *= temp_reg2;
    
	temp_reg1 *=compensation_fact;
	temp_reg1 /= 10000;
	if((temp_reg1>=-26000)&&(temp_reg1<=26000))
		mstRtcMeadata.temperature_ppm = temp_reg1;
}
/*"************************************************			
Function:RTC_CalcPPM
Description: �����������
Input:  mode:����ģʽ
Output: 	��
Return:��	
Others: 		
************************************************"*/
void RTC_CalcPPM(INT8U Mode)
{
	INT32S value;
    
	mstRtcMeadata.ppm_value = mstRtcMeadata.init_ppm;	
	mstRtcMeadata.ppm_value += mstRtcMeadata.temperature_ppm;
	//mstRtcMeadata.ppm_value *= (INT32S)iRtcParameter.Compen_Interval;      //32��ļ��
	mstRtcMeadata.ppm_value += mstRtcMeadata.remain_ppm;  //�ϴβ���ʣ���
	value =  mstRtcMeadata.ppm_value/PPM_PER_VALUE;
	mstRtcMeadata.remain_ppm =   mstRtcMeadata.ppm_value - value*PPM_PER_VALUE;
	//if( value != mstRtcMeadata.last_calivalue)
	//{
		//mstRtcMeadata.last_calivalue = value;
		RTC_CalibImplement(Mode,value,1);//ִ���¶Ȳ���������RTC�����Ĵ���
	//}
	
}

/************************************************************  
						RTC��ʼ��

*************************************************************/
void RTC_CaliInit (INT8U Mode)
{
	RTC_RamDataInit();

	if(  ( mstRtcPara.rtc_defpara_Flag != EEP_PAR_IS_INIT) || (mstRtcPara.crc != CRC16(  (INT8U *) &mstRtcPara, (FIND(ST_RTC_PARA,crc)) ) ) )/*"�ж�RAM�в�����ȷ�ԣ�������ȷ������ڿ�����"*/
	{
		RTC_ParaInit();
	}			
}

void RTC_RamDataInit(void)
{
	calictrl = CAL_STATUS_STOP;/*"У��δ��ʼ"*/
	mstRtcCaliTime.IntPwr = 8;/*"�ϵ粹��ʱ��"*/
	mstRtcCaliTime.CntPwr = 0;/*"�ϵ粹��ʱ�����"*/
	mstRtcCaliTime.IntLow = 3600;/*"�͹��Ĳ���ʱ��"*/
	mstRtcCaliTime.CntLow = 3592;/*"�͹��Ĳ���ʱ�����"*/
	mstRtcCaliTime.mSecCnt = 0;/*"�����"*/
    LIB_MemSet(0, (INT8U *)&temp_ad_buffer, 16);
    LIB_MemSet(0, (INT8U *)&mstRtcMeadata,sizeof(ST_RTC_MEADATA));
    
    temp_ppm_value = 0;
}

void RTC_ParaInit(void)
{
	INT8U     cnt1,cnt2,cnt3;

	cnt1 = RTC_GetEeromFlag();
		
	if( cnt1 == TRUE )/*"�ڿ��Ѿ���ʼ����ȡ����A����"*/
	{
		for(cnt2=0;cnt2<2;cnt2++)
		{
		    FS_ReadFile(E_FILE_LRM,RTC_PARA_BAK1_SHIFT,(INT8U *)&mstRtcPara,sizeof(ST_RTC_PARA));
			//DC_GetData(E2_RTC_DATA_BLK1,(INT8U *)&mstRtcPara,place2,sizeof(ST_RTC_PARA));/*"��ȡEEROM ������A�е�����"*/
			if(mstRtcPara.crc  == CRC16(  (INT8U *)& mstRtcPara, (FIND(ST_RTC_PARA,crc)) ) )/*"�ж�������A�����Ƿ���ȷ"*/
		        {
			       break;/*"��ȷ �� ���� "*/
			}
		}
		if( cnt2>= 2 )/*"�ڿ�A ��������ȡ����B   ����"*/
		{
			for(cnt3=0;cnt3<2;cnt3++)
			{		
			        FS_ReadFile(E_FILE_LRM,RTC_PARA_BAK2_SHIFT,(INT8U *)&mstRtcPara,sizeof(ST_RTC_PARA));
		        	//DC_GetData(E2_RTC_DATA_BLK2,(INT8U *)&mstRtcPara,place2,sizeof(ST_RTC_PARA));/*"��ȡEEROM������B�е�����"*/
			       if(mstRtcPara.crc  == CRC16(  (INT8U *) &mstRtcPara, (FIND(ST_RTC_PARA,crc)) ) )/*"�ж�������B�����Ƿ���ȷ"*/
			       {
					break;/*"��ȷ �� ���� "*/
			       }
			}
			if( cnt3 >= 2 )/*"�ڿ�B �������"*/
			{
				RTC_Defult_Para();
			}
			else/*"�ڿ�B ������ȷ"*/
			{

			}
		}
		else/*"�ڿ�A ������ȷ"*/
		{

		}
	}
	else/*"�ڿ�δ����ʼ��"*/
	{
		RTC_Defult_Para();	
	}
}

INT8U RTC_GetEeromFlag()
{
	INT8U cnt1,ret_value = TRUE;
	INT16U EeromInitFlg;
	
	for(cnt1=0;cnt1<3;cnt1++)
	{
	    FS_ReadFile(E_FILE_LRM,RTC_PARA_BAK1_SHIFT,(INT8U *)&EeromInitFlg,sizeof(mstRtcPara.rtc_defpara_Flag));
        //DC_GetData(E2_RTC_DATA_BLK1,(INT8U *)&EeromInitFlg,place1,sizeof(EeromInitFlg));/*"��EEROM��ʼ����־ A"*/
        if( EeromInitFlg != EEP_PAR_IS_INIT )
        {
            FS_ReadFile(E_FILE_LRM,RTC_PARA_BAK2_SHIFT,(INT8U *)&EeromInitFlg,sizeof(mstRtcPara.rtc_defpara_Flag));
        	//DC_GetData(E2_RTC_DATA_BLK2,(INT8U *)&EeromInitFlg,place1,sizeof(EeromInitFlg));/*"��EEROM��ʼ����־ B"*/
        }
        if ( EeromInitFlg == EEP_PAR_IS_INIT)/*"����ڿ��Ѿ���ʼ�� ������"*/
        {
        	return ret_value;
        }
	}
	ret_value = FALSE;
	return ret_value;
}

void RTC_Defult_Para()
{
    INT8U i;

    mstRtcPara.rtc_defpara_Flag = EEP_PAR_IS_DEF;//����оƬĬ�ϲ�����־

    mstRtcPara.calipara.tempFactHigh = 0;
    mstRtcPara.calipara.tempFactLow= 0;
    mstRtcPara.calipara.crystalFactHigh= -320;
    mstRtcPara.calipara.crystalFactLow= -320;
    mstRtcPara.calipara.crystalErr= 0;
    mstRtcPara.calipara.temperatureTop = 252;
    mstRtcPara.calipara.tempInterval = 3600;
    	
    //RTC_DataFlashInit();
    mstRtcPara.amendpara.amend_flag = 0;
    for(i=0;i<_GROUP_SIZE;i++)
    {
        mstRtcPara.amendpara.pata_value[i]=0;
        mstRtcPara.amendpara.ppm_value[i]=0;
    }

    mstRtcPara.crc =  CRC16((INT8U*)&mstRtcPara, FIND(ST_RTC_PARA,crc));
}

void RTC_DataFlashInit()
{
	
}

/************************************************************  
						RTCÿ����ѭ��

*************************************************************/
void RTC_CaliMain (INT8U Mode)
{
	if( Mode == MCU_RUNMODE)
	{
		RTC_SampAndCali(Mode,7);
	}
	else
	{
		if( mstRtcCaliTime.CntLow >= mstRtcPara.calipara.tempInterval-8 )
		{
			RTC_SampAndCali(Mode,mstRtcPara.calipara.tempInterval-1);
		}
		else
		{
			mstRtcCaliTime.CntLow++;
		}
	}
}

void RTC_SampAndCali( INT8U Mode,INT16U InterVal)
{
    INT16U temp_ad;

    AC_AdcInit(Mode,AC_ADCID_TEMP);
    temp_ad = AC_AdcEnable(AC_ADCID_TEMP);
    AC_AdcDisable(AC_ADCID_TEMP);
    if((temp_ad >=1000) && (temp_ad <= 1900))
    {
        if( Mode == PWRMODE_UP )
    	{
    	    temp_ad_buffer[mstRtcCaliTime.CntPwr] = temp_ad;
    	    if( mstRtcCaliTime.CntPwr >= InterVal )
    	    {			
    		    mstRtcCaliTime.CntPwr=0;
    		    RTC_RunEvery1s();/*"����У��"*/
    		    RTC_TempMeter_ErrComp(Mode);/*"����ppm"*/
    	    }
    	    else
    	    {
    		    mstRtcCaliTime.CntPwr++;
    	    }
    	}
	    else
    	{
    		temp_ad_buffer[mstRtcCaliTime.CntLow-(mstRtcPara.calipara.tempInterval-8)] = temp_ad;
            if( mstRtcCaliTime.CntLow >= InterVal )
    		{
    			mstRtcCaliTime.CntLow=0;
    			RTC_RunEvery1s();/*"����У��"*/
    			RTC_TempMeter_ErrComp(Mode);/*"����ppm"*/
    		}
    		else
    		{
    			mstRtcCaliTime.CntLow++;
    		}
    	}
    }
    else
    {
        AC_AdcDisable(AC_ADCID_TEMP);
    }
}

void RTC_RunEvery1s(void)
{
	/*"��ʱ�ж�RAM�в��������ݵ���Ч��"*/
	mstRtcCaliTime.mSecCnt++;
	if( mstRtcCaliTime.mSecCnt >= 3600 )/*"ÿСʱִ��"*/
	{
		if( (mstRtcPara.rtc_defpara_Flag == EEP_PAR_IS_DEF) || (mstRtcPara.crc != CRC16(  (INT8U *) &mstRtcPara, (FIND(ST_RTC_PARA,crc)) )  ) )/*"RAM������λĬ��ֵ,���¶�ȡ�ڿ��е�����"*/
		{
			RTC_ParaInit();
		}
		mstRtcCaliTime.mSecCnt = 0;
	}
}
/************************************************************  
							RTCУ��Э��

*************************************************************/
INT8U RTC_RtcCtrlCmd(INT8U *pBuff)
{
    INT8U ret_value = COMM_ERR_NOERR;

    if( *(pBuff+4) ==0x01 )
    {
        calictrl = CAL_STATUS_START;/*"У��ʼ"*/
        mstRtcPara.rtc_defpara_Flag = EEP_PAR_NOT_INIT;
        //mstRtcPara.crc = CRC16((INT8U*)&mstRtcPara, sizeof(ST_RTC_PARA) - 2); 
        mstRtcPara.crc =  CRC16((INT8U*)&mstRtcPara, FIND(ST_RTC_PARA,crc));
    }
    else
        calictrl = CAL_STATUS_STOP;/*"У�����"*/

    return ret_value;
}

INT8U RTC_RtcSave(INT8U *pBuff)
{
    INT8U ret_value = COMM_ERR_NOERR;

    mstRtcPara.rtc_defpara_Flag = EEP_PAR_IS_INIT;
    //mstRtcPara.crc = CRC16((INT8U*)&mstRtcPara, sizeof(ST_RTC_PARA) - 2); 
    mstRtcPara.crc =  CRC16((INT8U*)&mstRtcPara, FIND(ST_RTC_PARA,crc));
    //DC_OutputData(E2_RTC_DATA_BLK1,(INT8U *)&mstRtcPara,0,sizeof(ST_RTC_PARA));
    //DC_OutputData(E2_RTC_DATA_BLK2,(INT8U *)&mstRtcPara,0,sizeof(ST_RTC_PARA));
    FS_WriteFile(E_FILE_LRM,RTC_PARA_BAK1_SHIFT,(INT8U *)&mstRtcPara,sizeof(ST_RTC_PARA));
    FS_WriteFile(E_FILE_LRM,RTC_PARA_BAK2_SHIFT,(INT8U *)&mstRtcPara,sizeof(ST_RTC_PARA));

    mstRtcPara.rtc_defpara_Flag = EEP_PAR_NOT_INIT;

    return ret_value;
}

INT8U RTC_FlashDataInit(INT8U *pBuff)
{
    INT8U ret_value = COMM_ERR_NOERR;
    
    RTC_Defult_Para();

    return ret_value;
}

INT8U RTC_AmendOpen(INT8U *pBuff)
{
	INT8U ret_value = COMM_ERR_NOERR;

	mstRtcPara.amendpara.amend_flag = 0xAA55;/*"�����޵�ģʽ"*/
	
	return ret_value;
}

INT8U RTC_AmendTest(INT8U *pBuff)
{
	INT8U ret_value = COMM_ERR_NOERR;

	mstRtcPara.amendpara.amend_flag = 0x55AA;/*"�������ģʽ"*/
	
	return ret_value;
}

INT8U RTC_AmendClose(INT8U *pBuff)
{
	INT8U ret_value = COMM_ERR_NOERR;

	mstRtcPara.amendpara.amend_flag = 0x0000;/*"�˳��޵�ģʽ��������ͨģʽ"*/
	
	return ret_value;
}


INT8U RTC_WRtcPara(INT8U *pBuff)
{
    INT8U ret_value = COMM_ERR_NOERR;
    INT32U para_data1;
    INT8U addr ;

    LIB_Reverse((INT8U *)(pBuff+5), ((INT8U *)&para_data1), 4);

    addr = *(pBuff+4);
    addr -= 0x01;

    switch(addr)
    {
        case 0:
        {
            mstRtcPara.calipara.tempFactHigh = para_data1;
        }
        break;
        case 1:
        {
            mstRtcPara.calipara.tempFactLow = para_data1;
        }
        break;
        case 2:
        {
            mstRtcPara.calipara.crystalFactHigh = para_data1;
        }
        break;
        case 3:
        {
            mstRtcPara.calipara.crystalFactLow = para_data1;
        }
        break;
        case 4:
        {
            mstRtcPara.calipara.crystalErr = para_data1;
        }
        break;
        case 5:
        {
            mstRtcPara.calipara.temperatureTop = para_data1;
        }
        break;
        case 6:
        {
            mstRtcPara.calipara.tempInterval = para_data1;
        }
        break;
    }

    if( (addr>=0x07) && (addr<(0x07+_GROUP_SIZE)) )
	{
   		mstRtcPara.amendpara.pata_value[addr - 0x07] = para_data1;
	}

	if( (addr>=0x12) && (addr<(0x12+_GROUP_SIZE)) )
	{
		mstRtcPara.amendpara.ppm_value[addr - 0x12]  = para_data1;
		temp_ppm_value = para_data1;
	}
    
    //mstRtcPara.crc = CRC16((INT8U*)&mstRtcPara, sizeof(ST_RTC_PARA) - 2); 
    mstRtcPara.crc =  CRC16((INT8U*)&mstRtcPara, FIND(ST_RTC_PARA,crc));
    return ret_value;
}


INT8U RTC_RRtcPara(INT8U *pBuff)
{
    INT8U ret_value = COMM_ERR_NOERR;
    INT32U para_data1;
    INT8U addr ;
    

    addr = *(pBuff+4);
    addr -= 0x81;

    switch(addr)
    {
        case 0:
        {
            para_data1 = mstRtcPara.calipara.tempFactHigh;
        }
        break;
        case 1:
        {
            para_data1 = mstRtcPara.calipara.tempFactLow;
        }
        break;
        case 2:
        {
            para_data1 = mstRtcPara.calipara.crystalFactHigh;
        }
        break;
        case 3:
        {
            para_data1 = mstRtcPara.calipara.crystalFactLow;
        }
        break;
        case 4:
        {
            para_data1 = mstRtcPara.calipara.crystalErr;
        }
        break;
        case 5:
        {
            para_data1 = mstRtcPara.calipara.temperatureTop;
        }
        break;
        case 6:
        {
            para_data1 = mstRtcPara.calipara.tempInterval;
        }
        break;
        case 7:
        {
            para_data1 = RTCCALI_VERSION;
        }
        break;
    }

    if( (addr>=0x10) && (addr<(0x10+_GROUP_SIZE)) )
    {
        para_data1 = mstRtcPara.amendpara.pata_value[addr - 0x10];
    }

    if( (addr>=0x1C) && (addr<(0x1C+_GROUP_SIZE)) )
    {
        para_data1 = mstRtcPara.amendpara.ppm_value[addr - 0x1c];
    }

    
    LIB_MemCpy(((INT8U *)&para_data1),pBuff+3,  4);
    
    return ret_value;
}

INT8U RTC_RAmendStatus(INT8U *pBuff)
{
	INT8U ret_value = COMM_ERR_NOERR;
	INT32U amend_flag;

	amend_flag = mstRtcPara.amendpara.amend_flag;

	LIB_MemCpy(((INT8U *)&amend_flag),pBuff+3,  4);

	return ret_value;
}


INT8U RTC_RDataTempAD(INT8U *pBuff)
{
    INT8U ret_value = COMM_ERR_NOERR;

    LIB_MemCpy(((INT8U *)&mstRtcMeadata.temperature_ad),pBuff+3,  4);

    return ret_value;
}

INT8U RTC_RDataTempVal(INT8U *pBuff)
{
    INT8U ret_value = COMM_ERR_NOERR;
    INT32S temperature_value;
    
    temperature_value= (INT32S)mstRtcMeadata.temperature_value;

    LIB_MemCpy(((INT8U *)&temperature_value),pBuff+3,  4);

    return ret_value;
}

INT8U RTC_RDataCaliPPM(INT8U *pBuff)
{
    INT8U ret_value = COMM_ERR_NOERR;
        
    LIB_MemCpy(((INT8U *)&mstRtcMeadata.ppm_value),pBuff+3,  4);

    return ret_value;
}

INT8U RTC_RDataTempPPM(INT8U *pBuff)
{
    INT8U ret_value = COMM_ERR_NOERR;
        
    LIB_MemCpy(((INT8U *)&mstRtcMeadata.temperature_ppm),pBuff+3,  4);

    return ret_value;
}

INT8U RTC_CaliRead (INT8U *pBuff)
{
    INT8U ret_value = COMM_ERR_NOERR;

    if( *(pBuff+4) >=0x81 && *(pBuff+4) <=0x87  )
    {
        ret_value = RTC_RRtcPara(pBuff);
    }
    else if( *(pBuff+4) ==0x8C )
    {
        ret_value = RTC_RAmendStatus(pBuff);  
    }
    else if( *(pBuff+4) ==0x8D )
    {
        ret_value = RTC_RDataTempAD(pBuff);  
    }
    else if( *(pBuff+4) ==0x8E )
    {
        ret_value = RTC_RDataCaliPPM(pBuff); 
    }
    else if( *(pBuff+4) ==0x8F )
    {
        ret_value = RTC_RDataTempVal(pBuff); 
    }
    else if( *(pBuff+4) ==0x90 )
    {
        ret_value = RTC_RDataTempPPM(pBuff); 
    }
    else if( *(pBuff+4) >=0x91 && *(pBuff+4) <=0xA7 )
    {
        ret_value = RTC_RRtcPara(pBuff);
    }
    LIB_Reverse(&pBuff[3], &pBuff[7], 4);
    LIB_MemCpy(&pBuff[7], &pBuff[3], 4);
    return ret_value;
}

INT8U RTC_CaliWrite (INT8U *pBuff)
{
    INT8U ret_value=COMM_ERR_NOERR;

    if( *(pBuff+4) >=0x01 && *(pBuff+4) <=0x1D  )
    {
        if( calictrl == CAL_STATUS_START)
        {
            ret_value = RTC_WRtcPara(pBuff);
        }
        else
        {
            ret_value = COMM_ERR_OTHERS;
        }
    }
       
    return ret_value;
}

INT8U RTC_CaliCtrl (INT8U *pBuff)
{
    INT8U ret_value=COMM_ERR_NOERR;

    if( *(pBuff+4) >=0x01 && *(pBuff+4) <=0x02  )
    {
        ret_value = RTC_RtcCtrlCmd(pBuff);
    }
    if ( calictrl == CAL_STATUS_START )
    {
        if( *(pBuff+4) ==0x03 )
        {
            ret_value = RTC_RtcSave(pBuff);
        }
        else if(*(pBuff+4) ==0x04 )
        {
            ret_value = RTC_FlashDataInit(pBuff);
        }
        else if(*(pBuff+4) ==0x05 )
		{
			ret_value = RTC_AmendOpen(pBuff);
		}
		else if(*(pBuff+4) ==0x06 )
		{
			ret_value = RTC_AmendClose(pBuff);
		}
		else if(*(pBuff+4) ==0x07 )
		{
			ret_value = RTC_AmendTest(pBuff);
		}
    }
    else
    {
        ret_value = COMM_ERR_OTHERS;
    }
    return ret_value;
}

