#include "system\Sys_def.h"
#include "APP\LegallyRelevant\MU_LIB.h"
#include "APP\LegallyRelevant\LRM.h"
#include "Driver\DriverComm.h"
//#include "commonheader.h"

/*"***********************************************************
FileName: MU_LIB.c
Author: ���쿡
Version : 1.0       Date: 2017.11.13
Description:��������ģ��,
���ڼ����ĳ�ʼ�������У�˲ʱ�����ݶ�ȡ����ȫ�ж��Լ���������
		   ����RN8209C����оƬ
Function List:
<FunctionName>
<author> <time> <version > <desc>

***********************************************************"*/

/*"--------RN8209 ��������--------"*/
INT16U Checksum16(INT16U* data,INT8U length);
void MU_Init(INT8U mode);
void MU_Measure_Init(void);
void MU_Ramdata_Init(void);
void MU_Para_Init(INT8U mode);
void MU_Chip_Init(void);
void MU_Defult_Para(void);
void MU_Reg_Write(INT8U num);
INT8U MU_Reg_Init(void);
INT8U MU_CheckMea(void);
void MU_Main(void);
void MU_OnEvery4min(void);
void MU_ReadData(void);
void Transmit_To_DC(void);
void MU_ReadConfig(void);
INT8U MU_ReadCmp(INT16U reg,INT8U length);
void  MU_ReadInstantData(INT16U reg);
void MU_ReadVolt(void);
void MU_ReadCurr(INT8U channel);
void MU_ReadPwr(INT8U channel);
void MU_ReadFreq(void);
void MU_ReadFactor(INT8U channel);
void MU_SetDir(INT8U flag, INT8U channel);
INT8U MU_ReadEmu(void);
void MU_InitChannel(INT8U chnnel);
void SelectChannel(INT8U chnnel);
INT8U MU_NowChannel(void);
void ManageEnergyChannel(void);
INT8U MU_JustMtr(INT8U *pData);
INT8U MU_CaliCmd1(INT8U *pData);
INT8U MU_CaliCmd2(INT8U *pData);
void MU_CaliChip(INT8U *pData);
INT8U MU_CaliReg(INT8U *pData,INT16U reg_addr);
INT8U MU_CaliRegU32(INT8U *pData,INT16U reg_addr);  /* "Add on 20190508" */
void MU_CaliCoefficient(INT8U *pData,INT16U *ram_coefficient);
void MU_CaliConfig1(INT8U *pData);
void MU_CaliConfig2(INT8U *pData);
void MU_CaliAllPara(INT8U *pData);
INT8U MU_CaliCtrl(INT8U *pData);
INT8U MU_CaliCtrlCMD(INT8U *pData,INT16U reg_addr);
INT8U MU_CaliReadData(INT8U *pData);
INT8U MU_CaliReadPara(INT8U *pData);
INT8U MU_CaliReadOther(INT8U *pData,INT8U *length);
#if AMEND_MODE
void Amend_Un(void);
void Amend_Harmonic(void);
void Amend_Imax(void);
void Amend_0_5mT();
void Amend_OneverySeond(void);
void Amend_Up_Down(void);
void Amend_Up_Down_Main(void);
void Amend_Up_Down_Out(void);
void Amend_Up_Down_Judge(void);
void  Write_RegSCALE_E1(void);
INT8U Set_Amend(INT8U *pData);
INT8U Read_Amend(INT8U *pData);
INT8U Current_IN(INT16U Current);
INT8U Cal_gp(INT32S AmendValue);
#endif

NO_INIT ST_MU_PARA  gMU_Para;/*""����ģ�����в�����ֻ��ͨ��У���޸�"*/
ST_MU_INTERDATA  	gMU_Interdata;/*""����ģ���ڲ�˲ʱ������"*/
ST_MMT_DATA  		stMmtData;/*""����ģ�����˲ʱ������"*/
ST_MU_FLAG     	    gMU_Flag;/*"����ģ��״̬��־λ"*/
#if EDT_MODE
ST_EDT_ERRDATA      gEDT_Errdata;/*"���߼������"*/
#endif
#if HARMONIC_MODE
sHarmonicData_TypeDef HarmonicData;
#endif

INT8U Cal_Status_flag=CAL_STATUS_STOP;/*"У��״̬"*/
INT8U Output_Group=0;
INT8U mu_ok;


#if AMEND_MODE
INT16U    MT_K = 0;
INT16U 	RegScale_GP_Now;/*"��ǰ���ʱȲ�Ĵ���(Ӱ�����޵�ʱʹ��)"*/
INT32U 	MT_Power[5];							  	/*""������ƽ��ֵ"*/
INT8U 	mT_Start_Flag;							/*""0.5mT���鿪ʼ����־"*/
const INT16U	U_Data[8] = {1740,1780,1960,2000,2400,2440,2500,2550};
INT16U 	Imax_Data[6] = {600,1800,1800,3000,3000,4200};
static INT8U MT_Timer1;
AMEND_OUTDATA       Amend_out_data;
ST_AMEND_TIMER      gAmendtimer;/*""��У������"*/
#endif

/*"************************************************
Function:CheckSum
Description: 16λУ��ͺ���
Input:  *data �����׵�ַlength ����
Output: 	16λУ���
Return:��
Others:
************************************************"*/
/*"--------����16λУ��ͺ���--------"*/
INT16U Checksum16(INT16U* data,INT8U length)
{
  	INT8U i;
	INT16U checkcrc;

	checkcrc = 0;
  	for(i=length;i>0;i--)
  	{
    	checkcrc += data[i-1];
  	}
  	checkcrc = ~checkcrc;

  	return checkcrc;
}

//INT16U ChecksumEMU(INT16U Chksum)
//{

//}

/*"--------RN2023 ��ʼ��--------"*/
/*"************************************************
Function:MU_Init
Description: RN2023��ʼ������
Input:  ��
Output: 	��
Return:��
Others:
************************************************"*/
void MU_Init(INT8U mode)
{
    mu_ok = 0;
   	MU_Ramdata_Init();
   	MU_Chip_Init();
    MU_ReadInstantData(R_ALLP);/*"���������"*/
}

/*"************************************************
Function:MU_Ramdata_Init
Description: RN8209�̶�������ʼ��
Input:  ��
Output: 	��
Return:��
Others:
************************************************"*/
void MU_Ramdata_Init(void)
{
	/*"					gMU_Flag					"*/
  	LIB_MemSet(0, (INT8U *)&gMU_Flag, sizeof(ST_MU_FLAG));/*""�����оƬ״̬��־λ"*/
	gMU_Flag.MeasureChip_DelayRead_Flag = NOT_NEED_DELAY;
  	gMU_Flag.MeasureChip_Initialize_Flag = NOT_NEED_INIT;
  	gMU_Flag.MeasureChip_Read_Error = NO_ERROR;
  	gMU_Flag.MeasureChip_Reset_Flag = NOT_NEED_RESET;

	/*"					stMmtData					"*/
  	LIB_MemSet(0, (INT8U *)&stMmtData, sizeof(ST_MMT_DATA));     /*"������˲ʱ��"*/
	/*"					gMU_Interdata				"*/
	LIB_MemSet(0, (INT8U *)&gMU_Interdata, sizeof(ST_MU_INTERDATA));/*""��˲ʱ��"*/
	gMU_Interdata.FactorA=10000;
    gMU_Interdata.FactorA=10000;

  	Cal_Status_flag=CAL_STATUS_STOP;
    Output_Group = 0;


#if AMEND_MODE
  	/*"					�޵�����				"*/
    if((gAmendtimer.Amend_Complete!=0)&&((gMU_Para.ram_adj_data.Ram_hconfig2 & 0x0001) == 1))/*"��ֹ����ǰ��������"*/
    {
    	//DC_GetData(E2_MEA_PARA1,(INT8U *)&gMU_Para,0,sizeof(ST_MU_PARA));
        FS_ReadFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT,(INT8U *)&gMU_Para,sizeof(ST_MU_PARA));
    }
    LIB_MemSet(0, (INT8U *)&gAmendtimer, sizeof(ST_AMEND_TIMER));/*"���޵��ı���"*/
    LIB_MemSet(0, (INT8U *)&MT_Power, 20);		        			/*"���޵��ı���"*/
    RegScale_GP_Now = gMU_Para.ram_adj_data.Ram_gp;
    MT_K = 0;
    //DC_GetData(E2_MT_START_FLG,(INT8U *)&mT_Start_Flag,0,1);/*"�ڿ���0.5mT��־"*/
    FS_ReadFile(E_FILE_LRM,FIND(ST_LRM_FILE_DATA,MtStartFlg),(INT8U *)&mT_Start_Flag,1);
    MT_Timer1 = 0;
#endif
}

/*"************************************************
Function:MU_Chip_Init
Description: ���в�����оƬ��ʼ��
Input:  ��
Output: 	��
Return:��
Others:
************************************************"*/
void MU_Chip_Init(void)
{
   	MU_Measure_Init();
}

/*"************************************************
Function:MU_Measure_Init
Description: оƬ��ʼ������
Input:  ��
Output: 	��
Return:��
Others:
************************************************"*/
void MU_Measure_Init(void)
{
    INT8U cnt;
    INT16U data=0;

    for(cnt=0;cnt<3;cnt++)
	{
	    /*"��λ����оƬ"*/
	    MC_Reset();
		MU_IC_Init(PWRMODE_UP);

        if(MU_IC_WriteReg(W_SHAKE,(INT8U *)&data,2) != TRUE)/*"����ʧ��"*/
	    {
			MCU_DelayMs(1);
        }
		else
		{
            break;/*"�ɹ�������"*/
		}
	}

    if(cnt >= 3)
	{
		gMU_Flag.MeasureChip_Initialize_Flag = NEED_INIT;
	}
    else
	{
		gMU_Flag.MeasureChip_Initialize_Flag = NOT_NEED_INIT;
	}
}

/*"************************************************
Function:MU_Defult_Para
Description: ����Ĭ�ϲ�������
Input:  ��
Output: 	��
Return:��
Others:
************************************************"*/
void MU_Defult_Para(void)
{

}

/*"************************************************
Function:MU_OpenPulse
Description: ����ģ������򿪺���
Input:  ��
Output: 	��
Return:��
Others:
************************************************"*/
void MU_OpenPulse(void)
{

}


/*"--------RN2023 ÿ��ִ��--------"*/
/*"************************************************
Function:MU_CheckMea
Description:��ʱ�߼��жϺ���
Input:  ��
Output: ��
Return:TRUE  �ɹ�  FALSE  ʧ��
Others:
************************************************"*/
INT8U  MU_CheckMea(void)
{
    INT8U RetVal = TRUE;
	if(gMU_Flag.MeasureChip_DelayRead_Flag == NEED_DELAY)
	{
		if(gMU_Flag.MeasureChip_DelayRead_Record<=15)
		{
			gMU_Flag.MeasureChip_DelayRead_Record++;
			RetVal = FALSE;
			return RetVal;
		}
		else
		{
			 gMU_Flag.MeasureChip_DelayRead_Record=0;
		}
	}

    if(gMU_Flag.Read_EffectValue_Timer<60)/*"����60����������г���15�ζ�������λ����оƬ"*/
    {
        gMU_Flag.Read_EffectValue_Timer++;
    }
    else
    {
        if(gMU_Flag.MeasureChip_Read_IntervalError_Record>=15)/*"����оƬ����Ъ�����¼������"*/
        {
            gMU_Flag.MeasureChip_Initialize_Flag = NEED_INIT;/*"����һ��ѭ���жԼ���оƬ���и�λ����ʼ��������"*/
        }
        gMU_Flag.Read_EffectValue_Timer = 0;
        gMU_Flag.MeasureChip_Read_IntervalError_Record = 0;
    }

	if(gMU_Flag.MeasureChip_Initialize_Flag==NEED_INIT)/*"����ñ�־�ֵ���0xAB��ʾ��ʼ�����ɹ���������λ����оƬ��������ʼ��"*/
	{
		MU_Chip_Init();
        if(gMU_Flag.MeasureChip_Initialize_Flag==NEED_INIT)/*"������³�ʼ�����ǲ��ɹ�"*/
    	{
    	  	gMU_Flag.MeasureChip_Initialize_Error_Record++;/*"����оƬ��ʼ�������¼��������1"*/
        	if(gMU_Flag.MeasureChip_Initialize_Error_Record>=6)
        	{
        		LIB_MemSet(0, (INT8U *)&gMU_Interdata, sizeof(ST_MU_INTERDATA));/*"�������θ�λ���ɹ����˲ʱ����0"*/
                gMU_Interdata.FactorA=10000;
	       	    gMU_Flag.MeasureChip_DelayRead_Flag= NEED_DELAY;/*"����оƬ��ʱ����־��"*/
	       	    gMU_Flag.MeasureChip_Initialize_Error_Record = 0;
        	}
		    RetVal = FALSE;
   		    return RetVal;
     	}
		else if(gMU_Flag.MeasureChip_Initialize_Flag==NOT_NEED_INIT)
		{
		  	gMU_Flag.MeasureChip_DelayRead_Flag = NOT_NEED_DELAY;
		  	gMU_Flag.MeasureChip_Initialize_Error_Record =0;
		  	gMU_Flag.MeasureChip_DelayRead_Record =0;
		 }
		else
		{
		}
	}
    return RetVal;
}
INT8U nilm_spi;//todo

extern void Host_Commu_Bm_Data_Sync(INT8U type);

/*"************************************************
Function:MU_Main
Description: MU �����������к���
Input:  ��
Output: 	��
Return:��
Others:
************************************************"*/
void MU_Main(void)
{
/*"///////////////////////////////////"*/
/*"////////////ÿ1s//////////////////"*/
/*"//////////////////////////////////"*/
    if(MU_CheckMea() == TRUE)/*"�߼��ж�Ϊ�棬ִ�ж�����"*/
	{
        MU_ReadData();/*"1sִ��"*/
        Transmit_To_DC();
		if(1==nilm_spi)		/*"�����nilm��spi��Ҫ�ı䣬�����оƬ��������"*/
		{
			MU_IC_WriteReg(W_NILM,&nilm_spi,1); 
		}
        if(mu_ok == 0)
        {
            mu_ok = 1;
            Host_Commu_Bm_Data_Sync(2);
        }
    }

}

/*""************************************************
Function: MU_EngyGet
Description: ��������ģ��Ӽ�����ȡһ��ʱ�����ۼӵĵ���ֵ��
			��������ʱÿ����ö�ȡ��ͣ��ǰ���ö�ȡ��
			��ȡ�����������ݻ�ֱ�Ӵ��ݵ��������ģ����������㡣
Input:  ����32λ���ݵĻ�����ָ�룬���ڴ��ݵ�������λ0.0001WS
Output: ��
Return:
Others:
************************************************"*/
INT8U MU_EngyGet(INT16U EngyId, INT32U *pData)
{
    INT8U Ret=FALSE;
	INT16U PluseCounter,PluseCounter1;					/*"������"*/
	INT32U wSPerPluse;								    /*"���������������λ"*/
	INT32U Engtemp;

    PluseCounter=0;
    PluseCounter1=0;
    wSPerPluse=0;
    Engtemp=0;

    if(EngyId == L_ENGY_P)
    {
    	PluseCounter = MU_IC_GetPulse_Val();					/*"ȡ������"*/
    	PluseCounter1 = MU_IC_GetNoAffrimPulse_Val();			/*"ȡ��ʵ������"*/
    }
    else if(EngyId == L_ENGY_Q)
    {
    	PluseCounter = MU_IC_Get_Q_Pulse_Val();					/*"ȡ������"*/
    	PluseCounter1 = MU_IC_Get_Q_NoAffrimPulse_Val();	    /*"ȡ��ʵ������"*/
    }


	if((PluseCounter1>=150)||(PluseCounter>=150))		/*"������"*/
	{
    	Engtemp = 0;
       	MC_Reset();
        Ret=FALSE;
	}
  	else if(PluseCounter == 0)
 	{
  		Engtemp = 0;
        Ret=TRUE;
 	}
	else if (PluseCounter<150)
   	{
		wSPerPluse = gMU_Para.ram_adj_data.Ram_ecconst / 100;
		wSPerPluse = 360000000/wSPerPluse;
		Engtemp = PluseCounter * wSPerPluse;
		Engtemp = Engtemp/10000;
        Ret=TRUE;
    }
	*pData = Engtemp;

    return Ret;
}
/*"************************************************
Function:MU_ReadData
Description: MU��ʱ�����ݺ���
Input:  ��
Output: 	��
Return:��
Others:
************************************************"*/
void MU_ReadData(void)
{
    #if STATE_MODE

        #if NORMAL_READ_MODE
	    MU_ReadInstantData(R_URMS);/*"����ѹ"*/
	    MU_ReadInstantData(R_IARMS);/*"�����ߵ���"*/
	    MU_ReadInstantData(R_PWRPA);/*"�����߹���"*/
	    if( Output_Group == 0)
    	{
    	    	MU_ReadInstantData(R_FCTRA);/*"����������"*/
    	    	MU_ReadInstantData(R_UFREQ);/*"��Ƶ��"*/
    	    	Output_Group = 1;
    	}
	    else
    	{
    		MU_ReadInstantData(R_IBRMS);/*"�����ߵ���"*/
    		Output_Group = 0;
    	}
        #endif

        #if FAST_READ_MODE
            MU_ReadInstantData(R_ALLD);/*"�����˲ʱ����"*/
            #if EDT_MODE
            MU_ReadInstantData(R_ALLE);/*"��������߼�����"*/
            #endif
            #if OUTDATA_MODE
            //MU_ReadInstantData(R_ALLO);/*"���������ʶ�����"*/
            #endif
            #if HARMONIC_MODE
            MU_ReadInstantData(R_ALLH_LITE);/*"�����г������"*/
            //MU_ReadInstantData(R_ALLH);/*"�����г������"*/
            #endif
        #endif

    #endif

    #if UNSTATE_MODE

    #endif
 }
/*"************************************************
Function:Transmit_To_DC
Description: �����������������ĺ���
Input:  ��
Output: 	��
Return:��
Others:
************************************************"*/
void Transmit_To_DC(void)
{
    INT8U i;

    if((gMU_Interdata.DrA == NEGA_STATUS)&&(gMU_Interdata.PowerPA !=0))/*""����,�����ṩ˲ʱ��Ϊ�޷�������������λ"*/
	{
		stMmtData.actPwr[1] = ~gMU_Interdata.PowerPA+1;
	 	stMmtData.curr[0] = ~gMU_Interdata.CurrentA+1;
	 	stMmtData.factor[1] = ~gMU_Interdata.FactorA+1;
        stMmtData.PowerDir = 0x80;/*"����"*/
	}
    else
    {
       	stMmtData.actPwr[1] = gMU_Interdata.PowerPA;
	 	stMmtData.curr[0] = gMU_Interdata.CurrentA;
	 	stMmtData.factor[1] = gMU_Interdata.FactorA;
        stMmtData.PowerDir = 0x00;/*"����"*/
    }

    if((gMU_Interdata.DrqA == NEGA_STATUS)&&(gMU_Interdata.PowerQA !=0))/*""����,�����ṩ˲ʱ��Ϊ�޷�������������λ"*/
	{
		stMmtData.reactPwr[1] = ~gMU_Interdata.PowerQA+1;
	}
    else
    {
        stMmtData.reactPwr[1] = gMU_Interdata.PowerQA;
    }
    
	stMmtData.calNcurr = gMU_Interdata.CurrentB;
	stMmtData.volt[0] = gMU_Interdata.Voltage/1000;/*""��ѹֻ��ҪһλС��"*/
	stMmtData.freq = gMU_Interdata.Frequency;
	/*"�����A�๦�ʡ�������������ͬ������"*/
	stMmtData.actPwr[0] = stMmtData.actPwr[1];
    stMmtData.reactPwr[0] = stMmtData.reactPwr[1];
	stMmtData.factor[0] = stMmtData.factor[1];

    if(gMU_Flag.MeasureChip_Initialize_Flag == NOT_NEED_INIT)
    {
        Push(E_MU_IC_ERR, FALSE);
    }
    else
    {
        Push(E_MU_IC_ERR, TRUE);
    }

    for(i=0 ; i<E_TEMP_MAX; i++)
    {
       stMmtData.gTmnlTmp[i]=gMU_Interdata.gTmnlTmp[i];
    }
}

/*"/////////////////////////////////////////////"*/
/*"///////////////��ȡ����оƬ�Ĵ�������////////"*/
/*"/////////////////////////////////////////////"*/

/*"************************************************
Function:MU_ReadCmp
Description: ���ݻض���ȽϺ���
Input:  reg �Ĵ�����ַlength ���ݳ���(���ݳ���ҪС�ڵ���4)
Output: 	 ��
Return:TRUE  �ɹ�  FALSE  ʧ��
Others:
************************************************"*/
INT8U MU_ReadCmp(INT16U reg,INT8U length)
{
	if(MU_IC_ReadReg(reg,length)==TRUE)
    {
	    return TRUE;
    }
    else
    {
        return FALSE;
    }
}

void  MU_ReadInstantData(INT16U reg)
{
	INT32U TempData=0,LastOneData;
    INT8U *Dstp;
    INT8U i;
    INT8U  *para1[]=   {
                        (INT8U *)&gMU_Interdata.Voltage,
                        (INT8U *)&gMU_Interdata.CurrentA,
                        (INT8U *)&gMU_Interdata.CurrentB,
                        (INT8U *)&gMU_Interdata.PowerPA,
                        (INT8U *)&gMU_Interdata.PowerPB,
                        (INT8U *)&gMU_Interdata.PowerQA,
                        (INT8U *)&gMU_Interdata.PowerQB,
                        (INT8U *)&gMU_Interdata.Frequency,
                        (INT8U *)&gMU_Interdata.FactorA,
                        (INT8U *)&gMU_Interdata.FactorB,
                        (INT8U *)&gMU_Interdata.DrA,
                        (INT8U *)&gMU_Interdata.DrB,
                        (INT8U *)&gMU_Interdata.DrqA,
                        (INT8U *)&gMU_Interdata.DrqB,
                        (INT8U *)&gMU_Interdata.gTmnlTmp[0],
                        (INT8U *)&gMU_Interdata.gTmnlTmp[1],
                        (INT8U *)&gMU_Interdata.gTmnlTmp[2],
                        (INT8U *)&gMU_Interdata.gTmnlTmp[3]
                        };

    if(reg == R_ALLD)/*" �����˲ʱ���� "*/
    {
        if(MU_ReadCmp(reg,72) == TRUE)/*"ֻҪ��һ����ȷ�ʹ���"*/
      	{
      	    for(i=0;i<18;i++)
            {
    		    LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[4*(i+1)], (INT8U *)para1[i],4);
            }
     	}
      	else
      	{
      	    gMU_Flag.MeasureChip_Read_IntervalError_Record++;
      	}
    }
    #if EDT_MODE
    else if(reg == R_ALLE)
    {
        if(MU_ReadCmp(R_ALLE_STA,4) == TRUE)/*"��ȡ���߼������״̬"*/
        {
            LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[4], (INT8U *)&TempData,4);
            if(TempData == 1)/*"���߼�������Ѹ���"*/
            {
                if(MU_ReadCmp(reg,72) == TRUE)/*"ֻҪ��һ����ȷ�ʹ���"*/
                {
                    for(i=0;i<18;i++)
                    {
                        LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[4*(i+1)], (INT8U *)(&gEDT_Errdata)+4*i,4);
                    }
                }
                else
                {
                    gMU_Flag.MeasureChip_Read_IntervalError_Record++;
                }
            }
        }
    }
    #endif
    else if(reg == R_ALLP)
    {
    	#if EDT_MODE
		if(MU_ReadCmp(reg, EDT_ALLP_CMD_R_LEN) == TRUE)
		#else
        if(MU_ReadCmp(reg,34) == TRUE)/*"ֻҪ��һ����ȷ�ʹ���"*/
		#endif
      	{
      	    for(i=0;i<17;i++)
            {
    		    LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[2*i+4], (INT8U *)(&gMU_Para.ram_adj_data)+2*i,2);
            }
			#if EDT_MODE
			for(i=0;i<EDT_PARA_NUM;i++)
			{
				LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[4U*i+EDT_STD_POS_IN_ST+4], (INT8U *)(&gMU_Para.edt_stddata)+4*i,4);
			}
			#endif
     	}
      	else
      	{
      	    gMU_Flag.MeasureChip_Read_IntervalError_Record++;
      	}
    }
    #if OUTDATA_MODE
    else if(reg == R_ALLO)
    {
        if(MU_ReadCmp(reg,4) == TRUE)/*"ֻҪ��һ����ȷ�ʹ���"*/
      	{
      	    for(i=0;i<1;i++)
            {
    		    LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[2*i+4], (INT8U *)(&gMU_Para.out_data)+2*i,2);
            }
     	}
      	else
      	{
      	    gMU_Flag.MeasureChip_Read_IntervalError_Record++;
      	}
    }
    #endif
    #if HARMONIC_MODE
    else if(reg == R_ALLH_LITE)
    {
        if(MU_ReadCmp(reg,12) == TRUE)/*"ֻҪ��һ����ȷ�ʹ���"*/
      	{
            LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[4],(INT8U *)& HarmonicData.HarmonicpercentU[0][0], 2);
            LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[6],(INT8U *)& HarmonicData.HarmonicpercentI[0][0], 2);
            LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[8],(INT8U *)& HarmonicData.HarmonicUrms[0], 4);
            LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[12],(INT8U *)& HarmonicData.HarmonicIrms[0], 4);

            LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[4],(INT8U *)& stMmtData.HRVolt[0][0], 2);
            LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[6],(INT8U *)& stMmtData.HRCurr[0][0], 2);
            LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[8],(INT8U *)& stMmtData.HRUrms[0], 4);
            LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[12],(INT8U *)& stMmtData.HRIrms[0], 4);
     	}
      	else
      	{
      	    gMU_Flag.MeasureChip_Read_IntervalError_Record++;
      	}
    }
    else if(reg == R_ALLH)
    {
        if(MU_ReadCmp(reg,60) == TRUE)/*"ֻҪ��һ����ȷ�ʹ���"*/
      	{
      	    for(i=0;i<30;i++)
            {
    		    LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[2*i+4], (INT8U *)(&HarmonicData.HarmonicpercentU[0][0])+2*i,2);
                LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[2*i+4], (INT8U *)(&stMmtData.HRVolt[0][0])+2*i,2);
            }
     	}
      	else
      	{
      	    gMU_Flag.MeasureChip_Read_IntervalError_Record++;
      	}
    }
    #endif
    else/*" �����ȡ˲ʱ���� "*/
    {

        if(reg == R_URMS)/*" ����ѹ "*/
        {
            LastOneData = gMU_Interdata.Voltage;
            Dstp = (INT8U *)&gMU_Interdata.Voltage;
        }
        else if(reg == R_IARMS)/*" �����ߵ��� "*/
        {
            LastOneData = gMU_Interdata.CurrentA;
            Dstp = (INT8U *)&gMU_Interdata.CurrentA;
        }
        else if(reg == R_PWRPA)/*" �������й����� "*/
        {
            LastOneData = gMU_Interdata.PowerPA;
            Dstp = (INT8U *)&gMU_Interdata.PowerPA;
        }
        else if(reg == R_PWRQA)/*" �������޹����� "*/
        {
            LastOneData = gMU_Interdata.PowerQA;
            Dstp = (INT8U *)&gMU_Interdata.PowerQA;
        }
        else if(reg == R_PWRSA)/*" ���������ڹ��� "*/
        {
            LastOneData = gMU_Interdata.PowerSA;
            Dstp = (INT8U *)&gMU_Interdata.PowerSA;
        }
        else if(reg == R_FCTRA)/*" �����߹������� "*/
        {
            LastOneData = gMU_Interdata.FactorA;
            Dstp = (INT8U *)&gMU_Interdata.FactorA;
        }


        if(reg == R_UFREQ)/*" ��Ƶ�� "*/
        {
            LastOneData = gMU_Interdata.Frequency;
            Dstp = (INT8U *)&gMU_Interdata.Frequency;
        }
        else if(reg == R_IBRMS)/*" �����ߵ��� "*/
        {
           LastOneData = gMU_Interdata.CurrentB;
           Dstp = (INT8U *)&gMU_Interdata.CurrentB;
        }
        else if(reg == R_PWRPB)/*" �������й����� "*/
        {
            LastOneData = gMU_Interdata.PowerPB;
            Dstp = (INT8U *)&gMU_Interdata.PowerPB;
        }
        else if(reg == R_PWRQB)/*" �������޹����� "*/
        {
            LastOneData = gMU_Interdata.PowerQB;
            Dstp = (INT8U *)&gMU_Interdata.PowerQB;
        }
        else if(reg == R_PWRSB)/*" ���������ڹ��� "*/
        {
            LastOneData = gMU_Interdata.PowerSB;
            Dstp = (INT8U *)&gMU_Interdata.PowerSB;
        }
        else if(reg == R_FCTRB)/*" �����߹������� "*/
        {
            LastOneData = gMU_Interdata.FactorB;
            Dstp = (INT8U *)&gMU_Interdata.FactorB;
        }

      	if(MU_ReadCmp(reg,4) == TRUE)/*"ֻҪ��һ����ȷ�ʹ���"*/
      	{
    		LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[4], (INT8U *)&TempData,4);
     	 }
      	else
      	{
      	    gMU_Flag.MeasureChip_Read_IntervalError_Record++;
    		TempData = LastOneData;
      	}

      	LIB_MemCpy((INT8U  *)&TempData,Dstp,4);/*"��������ѹֵ����ṹ��"*/
    }
}


/*"--------У �� �� ��--------"*/
/*"************************************************
Function:MU_JustMtr
Description:����У��Э���������������
Input:  pData У������֡����ָ��
Output: 	��
Return:TRUE  �ɹ�  FALSE  ʧ��
Others:
************************************************"*/
INT8U MU_JustMtr(INT8U *pData)
{
	INT8U RetVal = COMM_ERR_NOERR;
	INT8U DataLength;
	INT8U CMD ;
    #if AMEND_MODE
	INT32U temp;
    #endif

	CMD = pData[3];

	if((ChkStatus(E_FAC) == FALSE)&&((CMD==_MU_W_PARA)||(CMD==_MU_W_CMD)||(CMD==_MU_W_AMEND)||(CMD==_RU_W_PARA)||(CMD==_RU_W_CMD)))
	{
	 	RetVal = COMM_ERR_PASSWORD;/*" �ڳ���״̬��ֻ�ܶ�������RTC����"*/
	}
	else
	{
        switch (CMD)
		{
            case _MU_W_PARA:/*"�������� �������ݳ���4���ֽ�"*/
            {
                RetVal = MU_CaliCmd1(pData);
                DataLength = 0;
            }
            break;

            case _MU_W_CMD:/*"�������� �������ݳ���4���ֽ�"*/
            {
                RetVal = MU_CaliCmd2(pData);
                DataLength = 0;
            }
            break;

            case _MU_R_PARA:/*"������ȡ �������ݳ���4���ֽ�"*/
            {
                RetVal = MU_CaliReadPara(pData);
                DataLength = 4;
            }
            break;

            case _MU_R_DATA:/*"˲ʱ���ݶ�ȡ,�������ݳ���4���ֽ�"*/
            {
                RetVal = MU_CaliReadData(pData);
                DataLength = 4;
            }
            break;

            case _MU_R_REG:/*"�Ĵ������ݶ�ȡ,�������ݳ���4���ֽ�"*/
            {
                RetVal = MU_CaliReadOther(pData,&DataLength);
                DataLength = 4;
            }
            break;

            case _MU_W_AMEND:/*"д��У����"*/
            {
                #if AMEND_MODE
                RetVal = Set_Amend(pData);
                DataLength = 0;
                #endif
            }
            break;

            case _MU_R_AMEND:/*"����У����"*/
            {
                #if AMEND_MODE
                RetVal = Read_Amend(pData);
                DataLength = 4;
                LIB_Reverse(&pData[3],(INT8U*)&temp, 4);
                LIB_MemCpy((INT8U*)&temp,&pData[3],4);
                #endif
            }
            break;

            case _RU_W_PARA:   					/*""   ͨ��645Э��дȡRTC��������"*/
            {
                RetVal = RTC_CaliWrite(pData);
                DataLength = 0;
            }
            break;

            case _RU_R_PARA:					/*"" 	ͨ��645Э���ȡRTC�������� "*/
            {
                RetVal = RTC_CaliRead(pData);
                DataLength = 4;
            }
            break;

            case _RU_W_CMD:					/*"" 	ͨ��645Э���޸ġ�У��RTC�������� "*/
            {
                RetVal = RTC_CaliCtrl(pData);
                DataLength = 0;
            }
            break;

            default:
            break;
        }
        pData[2] = DataLength;/*"" �������ݳ���"*/
	}
    return RetVal;
}

/*"************************************************
Function:MU_CaliCmd1
Description:��������У������
Input:  pData У������֡����ָ��
Output: 	��
Return:����Ӧ��״̬
Others:
************************************************"*/
INT8U  MU_CaliCmd1(INT8U *pData)
{
	INT8U result=COMM_ERR_NOERR;

	if(Cal_Status_flag == CAL_STATUS_START)/*"��У��ʼ���������޸�"*/
	{
		switch(pData[4])
		{
            case 0x80:
                result = MU_CaliReg(pData,W_VCOEF);/*"��ѹϵ��"*/
            break;

            case 0x81:
                result = MU_CaliReg(pData,W_IACOEF);/*"����ϵ��"*/
            break;

            case 0x82:
                result = MU_CaliReg(pData,W_PACOEF);/*"����ϵ��"*/
            break;

            case 0x83:
                result = MU_CaliReg(pData,W_GPQA);/*"��������"*/
            break;

            case 0x84:
                result = MU_CaliReg(pData,W_APOSA);/*"����ƫ��"*/
            break;

            case 0x85:
                result = MU_CaliReg(pData,W_PHSA);/*"��λ����"*/
            break;

            case 0x86:

            break;

            case 0x87:

            break;

            case 0x88:
            break;

            case 0x89:
                result = MU_CaliReg(pData,W_HFCNST);/*"���峣��"*/
            break;

            case 0x8A:

            break;

            case 0x8B:
                result = MU_CaliReg(pData,W_PSTRT);/*"Ǳ����ֵ"*/
            break;

            case 0x8C:
                result = MU_CaliReg(pData,W_MTRCFG);/*"����������"*/
            break;

            case 0x8D:
                result = MU_CaliReg(pData,W_MTRCFG2);/*"����������2"*/
            break;

            case 0x8E:
                result = MU_CaliReg(pData,W_ECCNST);/*"���õ�����峣��"*/
            break;

            case 0x8F:
                result = MU_CaliReg(pData,W_ALLP);/*"�������в���"*/
            break;

            case 0x91:
                result = MU_CaliReg(pData,W_IBCOEF);/*"���ߵ���ϵ��"*/
            break;

            case 0x92:
                result = MU_CaliReg(pData,W_PBCOEF);/*"���߹���ϵ��"*/
            break;

            case 0x93:
                result = MU_CaliReg(pData,W_GPQB);/*"���߹�������	"*/
            break;

            case 0x94:
                result = MU_CaliReg(pData,W_APOSB);/*"���߹���ƫ��	"*/
            break;

            case 0x95:
                result = MU_CaliReg(pData,W_PHSB);/*"������λ����	"*/
            break;

            case 0x96:

            break;

            case 0xA0:
                //MU_CaliChip(pData);
            break;

			case _05_W_I_AMPSTD:
				result = MU_CaliRegU32(pData,W_I_AMPSTD); /* "���߼�������·��ֵ��׼" */
				break;
			case _05_W_I_PHSSTD:
				result = MU_CaliRegU32(pData,W_I_PHSSTD); /* "���߼�������·��λ��׼" */
				break;
			case _05_W_U400_AMPSTD:
				result = MU_CaliRegU32(pData,W_U400_AMPSTD);/* "���߼���ѹ��·��ֵ��׼U400" */
				break;
			case _05_W_U400_PHSSTD:
				result = MU_CaliRegU32(pData,W_U400_PHSSTD);/* "���߼���ѹ��·��λ��׼U400" */
				break;
			case _05_W_U10_AMPSTD:
				result = MU_CaliRegU32(pData,W_U10_AMPSTD);/* "���߼���ѹ��·��ֵ��׼U10" */
				break;
			case _05_W_U10_PHSSTD:
				result = MU_CaliRegU32(pData,W_U10_PHSSTD);/* "���߼���ѹ��·��λ��׼U10" */
				break;
            case _05_W_U10_AMPSTD2:
				result = MU_CaliRegU32(pData,W_U10_AMPSTD2);/* "���߼���ѹ��·��ֵ��׼U10" */
				break;
			case _05_W_U10_PHSSTD2:
				result = MU_CaliRegU32(pData,W_U10_PHSSTD2);/* "���߼���ѹ��·��λ��׼U10" */
				break;

            default :
            result= COMM_ERR_NODATA;
            break;
		}
	}
	else
	{
		result= COMM_ERR_NODATA;
	}
	return result;
}
/*"************************************************
Function:MU_CaliCmd2
Description:�����������ƺ���
Input:  pData У������֡����ָ��
Output: 	��
Return:����Ӧ��״̬
Others:
************************************************"*/
INT8U  MU_CaliCmd2(INT8U *pData)
{
  	INT8U result=COMM_ERR_NOERR;
	switch(pData[4])
	{
        case 0xfa:
            result = MU_CaliCtrl(pData);/*"��������"*/
        break;

        default :
            result= COMM_ERR_NODATA;
        break;
    }
  	return result;
}

#if CHIPCAL_MODE
/*"************************************************
Function:MU_Chip_Cal
Description:һ��У׼����
Input:  pData У������֡����ָ��
Output: 	��
Return:����Ӧ��״̬
Others:
************************************************"*/
void MU_CaliChip(INT8U *pData)
{

}

#endif
/*"************************************************
Function:MU_CaliReg
Description:
Input:  pData
Output: 	��
Return:��
Others:
************************************************"*/
INT8U MU_CaliReg(INT8U *pData,INT16U reg_addr)
{
    INT8U ret;
    INT8U length,index;

    if(reg_addr == W_ALLP)
    {
        length = 34;
        index = 5;
    }
    else
    {
        length = 2;
        index = 7;
    }

    if(MU_IC_WriteReg(reg_addr,&pData[index],length) == TRUE)
    {
        ret = COMM_ERR_NOERR;
    }
    else
    {
        ret = COMM_ERR_NODATA;
    }

    return(ret);
}

/*"************************************************
Function:MU_CaliRegU32
Description:
Input:  pData, reg_addr
Output: ��
Return: ����
Others:
************************************************"*/
INT8U MU_CaliRegU32(INT8U *pData,INT16U reg_addr)
{
    INT8U ret;

    if(MU_IC_WriteReg(reg_addr,&pData[5],4) == TRUE)
    {
        ret = COMM_ERR_NOERR;
    }
    else
    {
        ret = COMM_ERR_NODATA;
    }

    return(ret);
}

/*"************************************************
Function:MU_CaliCtrl
Description:�����ƺ���
Input:  pData У������֡����ָ��
Output: 	��
Return:��
Others:
************************************************"*/
INT8U MU_CaliCtrl(INT8U *pData)
{
    INT8U result=COMM_ERR_NOERR;
    switch(pData[8])
    {
        case 1 :
        result = MU_CaliCtrlCMD(pData,W_START);/*"��ʼ"*/
        //mT_Start_Flag = 0;
        //DC_OutputData(E2_MT_START_FLG,&mT_Start_Flag,0,1);/*"�洢0.5mT��־"*/
        break;
        case 2:
        if(Cal_Status_flag == CAL_STATUS_START)/*"����У��״̬���ܷ�Ĭ�ϲ���"*/
        {
           result = MU_CaliCtrlCMD(pData,W_RESET);/*"ʹ��Ĭ�ϲ���"*/
        }
        break;
        case 3 :
        #if AMEND_MODE
        if(gAmendtimer.Amend_Resume==0)/*"�������޵������ڱ������������Ķ��������"*/
        {
            result = MU_CaliCtrlCMD(pData,W_SAVE);/*"��������"*/
        }
        else
        {
            result = COMM_ERR_OTHERS;
        }
        #else
        result = MU_CaliCtrlCMD(pData,W_SAVE);/*"��������"*/
        #endif
        break;
        case 4 :
            result = MU_CaliCtrlCMD(pData,W_STOP);/*"����"*/
        break;
		 
        default:
            result= COMM_ERR_NODATA;
        break;
    }
    return result;
}

/*"************************************************
Function:MU_CaliCtrlCMD
Description:У����������
Input:  pData У������֡����ָ��
Output: 	��
Return:��
Others:
************************************************"*/
INT8U MU_CaliCtrlCMD(INT8U *pData,INT16U reg_addr)
{
  	INT8U ret=COMM_ERR_NOERR;

    if(MU_IC_WriteReg(reg_addr,&pData[7],2) == TRUE)
    {
        if(reg_addr == W_START)
        {
            Cal_Status_flag = CAL_STATUS_START;
        }
        else
        if(reg_addr == W_STOP)
        {
            Cal_Status_flag = CAL_STATUS_STOP;
        }
        ret = COMM_ERR_NOERR;
    }
    else
    {
        ret = COMM_ERR_NODATA;
    }

    return(ret);

}

/*"************************************************
Function:MU_CaliReadAllPara
Description:д�����������
Input:  pData У������֡����ָ��
Output: 	��
Return:��
Others:
************************************************"*/
void MU_CaliReadAllPara(INT8U location,INT8U *pData)
{
    if(location == RAM_REGION_A)
    {
        LIB_MemCpy((INT8U *)&gMU_Para, (INT8U *)pData,sizeof(ST_MU_PARA));
    }
    else if(location == EEP_REGION_A)
    {
        FS_ReadFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT,(INT8U *)pData,sizeof(ST_MU_PARA));
    }
    else if(location == EEP_REGION_B)
    {
        FS_ReadFile(E_FILE_LRM,MU_PARA_BAK2_SHIFT,(INT8U *)pData,sizeof(ST_MU_PARA));
    }
    else
    {

    }
}

/*"************************************************
Function:MU_Dlt05Cmd
Description:�����������ݺ���
Input:  pData У������֡����ָ��
Output: 	��
Return:TRUE  �ɹ�  FALSE  ʧ��
Others:
************************************************"*/
INT8U MU_CaliReadData(INT8U *pData)
{
	INT8U result=COMM_ERR_NOERR;
	INT32U temp,temp1;

	switch(pData[4])
	{
        case 0x00:/*"��ѹ"*/
            temp=gMU_Interdata.Voltage/100;
        break;

        case 0x01:/*"����"*/
        	temp=gMU_Interdata.CurrentA/10;
        break;

        case 0x02:/*"����"*/
        	temp=gMU_Interdata.PowerPA;
        break;

        //case 0x03:/*"���ڹ���"*/
        //	result= COMM_ERR_NODATA;
        //break;

        //case 0x04:/*"��������"*/
        	//	temp=gMU_Interdata.FactorA;
        //break;

        //case 0x05:/*"��ѹƵ��"*/
        	//	temp=gMU_Interdata.Frequency;
        //break;

        case 0x06:/*"���ߵ���"*/
        	temp=gMU_Interdata.CurrentB/10;
        break;

        case 0x07:/*"���߹���"*/
        	temp=gMU_Interdata.PowerPB;
        break;

        case 0x0A:  /*"��ʣ�������4λС��"*/
            temp = gMU_Interdata.CurrentB;
        break;
            
        //case 0x08:/*"�������ڹ���"*/
        //	result= COMM_ERR_NODATA;
        //break;

        //case 0x09:/*"0.5 mT ��ʵ�ʹ���"*/
        //result= COMM_ERR_NODATA;
        //break;

		#if EDT_MODE
		/* "���߼�����ֵ" */
		case _05_R_I_AMP:
			temp = gEDT_Errdata.I_Amp;
			break;
		case _05_R_I_PHS:
			temp = gEDT_Errdata.I_Phs;
			break;
		case _05_R_U400_AMP:
			temp = gEDT_Errdata.U400_Amp;
			break;
		case _05_R_U400_PHS:
			temp = gEDT_Errdata.U400_Phs;
			break;
		case _05_R_U10_AMP:
			temp = gEDT_Errdata.U10_Amp;
			break;
		case _05_R_U10_PHS:
			temp = gEDT_Errdata.U10_Phs;
			break;
        case _05_R_U10_AMP2:
			temp = gEDT_Errdata.U10_Amp2;
			break;
		case _05_R_U10_PHS2:
			temp = gEDT_Errdata.U10_Phs2;
			break;
		/* "���߼�����ֵ, ������" */
		case _05_R_I_AMP_ERR:
			temp = gEDT_Errdata.I_Amp_Err;
			break;
		case _05_R_I_PHS_ERR:
			temp = gEDT_Errdata.I_Phs_Err;
			break;
		case _05_R_U400_AMP_ERR:
			temp = gEDT_Errdata.U400_Amp_Err;
			break;
		case _05_R_U400_PHS_ERR:
			temp = gEDT_Errdata.U400_Phs_Err;
			break;
		case _05_R_U10_AMP_ERR:
			temp = gEDT_Errdata.U10_Amp_Err;
			break;
		case _05_R_U10_PHS_ERR:
			temp = gEDT_Errdata.U10_Phs_Err;
			break;
        case _05_R_U10_AMP_ERR2:
			temp = gEDT_Errdata.U10_Amp_Err2;
			break;
		case _05_R_U10_PHS_ERR2:
			temp = gEDT_Errdata.U10_Phs_Err2;
			break;
        case _05_R_P_AMP_ERR:
			temp = gEDT_Errdata.I_Amp_Err;
			break;
		case _05_R_P_PHS_ERR:
			temp = gEDT_Errdata.I_Phs_Err;
		#endif /*"EDT_MODE"*/

		/* "������о״̬��" */
		case _05_R_READ_ERR_RECORD:
			temp = gMU_Flag.MeasureChip_Read_Error_Record;
			break;
		case _05_R_INIT_ERR_RECORD:
			temp = gMU_Flag.MeasureChip_Initialize_Error_Record;
			break;
		case _05_R_DELAYREAD_FLAG:
			temp = gMU_Flag.MeasureChip_DelayRead_Flag;
			break;
		case _05_R_INIT_FLAG:
			temp = gMU_Flag.MeasureChip_Initialize_Flag;
			break;
		case _05_R_READ_ERR:
			temp = gMU_Flag.MeasureChip_Read_Error;
			break;
		case _05_R_RST_FLAG:
			temp = gMU_Flag.MeasureChip_Reset_Flag;
			break;
		case _05_R_DELAYREAD_RECORD:
			temp = gMU_Flag.MeasureChip_DelayRead_Record;
			break;
		case _05_R_READ_EFFECT_TIMER:
			temp = gMU_Flag.Read_EffectValue_Timer;
			break;
		case _05_R_READ_INTERVALERR_RECORD:
			temp = gMU_Flag.MeasureChip_Read_IntervalError_Record;
			break;
		case _05_R_PULSE_FLAG:
			temp = gMU_Flag.MeasureChip_pluse_flag;
			break;
        default :
        	result= COMM_ERR_NODATA;
        break;
	}

	#if EDT_MODE
	if((pData[4] >= _05_R_I_AMP_ERR) && (pData[4] <= _05_R_U10_PHS_ERR))
	{
		if(temp & IF_SIGNED_BIT_SET) 
        {  /*"����תBCD��"*/
			/*" ��λ��8 "*/
			temp = (INT32U)((-(INT32S)temp) + ADD_IF_SIGNED_VALUE);
		}
	}
	#endif /*"EDT_MODE"*/
    
	if((pData[4] >= _05_R_READ_ERR_RECORD) && (pData[4] <= _05_R_PULSE_FLAG)) {
		temp1 = temp;	/*" ��оƬ״̬��תBCD "*/
	}
	else {
		LIB_LongToBcd((INT8U *)&temp, (INT8U *)&temp1);
	}

	LIB_Reverse((INT8U*)&temp1, (INT8U*)&pData[3], 4);
	return result;
}

/*"************************************************
Function:MU_ReadOnePara
Description:������У�����
Input:  tmp �������ַ mu_reg 16λ������ �������� reg_len
Output: 	��
Return: COMM_ERR_NOERR �ɹ� COMM_ERR_NODATA ʧ��
Others: Ŀǰ��ָ��ȶ���4�ֽ�
************************************************"*/
static INT8U MU_ReadOnePara(INT8U *tmp, INT16U mu_reg, INT8U reg_len)
{
	if (MU_IC_ReadReg(mu_reg, reg_len) == TRUE) 
    {
		/*" ȡ������С�� "*/
		LIB_MemCpy(&gMeaIc_Uart.Uart_Rx_Data[4U], tmp, reg_len);
		return COMM_ERR_NOERR;
	}
	else 
    {
		return COMM_ERR_NODATA;
	}
}

/*"************************************************
Function:MU_Dlt05Cmd
Description:��У���������
Input:  pData У������֡����ָ��
Output: 	��
Return:TRUE  �ɹ�  FALSE  ʧ��
Others:
************************************************"*/
INT8U MU_CaliReadPara(INT8U *pData)
{
	INT8U result=COMM_ERR_NOERR,tmp;
    INT32U temp;
    RAM_ADJ_DATA * PAdjdata;

    PAdjdata =  &gMU_Para.ram_adj_data;

    tmp = pData[4];
    LIB_MemSet(0, &pData[3], 4);
	switch(tmp)
	{
        case 0x00:/*"��ѹ����ϵ��"*/
			result = MU_ReadOnePara((INT8U*)(&temp), R_VCOEF, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_V_Coefficient = (INT16U)temp;
			}
        break;
        case 0x01:/*"��������ϵ��"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_IACOEF, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_Ia_Coefficient = (INT16U)temp;
			}
        break;
        case 0x02:/*"���ʻ���ϵ��"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_PACOEF, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_Pa_Coefficient = (INT16U)temp;
			}
        break;
        case 0x03:/*"��������"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_GPQA, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_GPQA = (INT16U)temp;
			}
        break;
        case 0x04:/*"����ƫ��"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_APOSA, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_APOSA = (INT16U)temp;
			}
        break;
        case 0x05:/*"��λ����"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_PHSA, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_PhsA = (INT16U)temp;
			}
        break;
        case 0x06:

        break;
        case 0x07:

        break;
        case 0x09:/*"���峣��"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_HFCNST, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_hfconst = (INT16U)temp;
			}
        break;
        case 0x0b:/*"Ǳ����ֵ"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_PSTRT, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_PStart = (INT16U)temp;
			}
        break;
        case 0x0c:/*"����������"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_MTRCFG, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_MeterCfg = (INT16U)temp;
			}
        break;
        case 0x0d:/*"����ģ��汾��"*/
            temp = VERSION_NUM;
        break;
        case 0x0e:/*"����������2"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_MTRCFG2, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_MeterCfg2 = (INT16U)temp;
			}
        break;
        case 0x0f:/*"������������"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_ECCNST, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_ecconst = (INT16U)temp;
			}
        break;
        case 0x11:/*"���ߵ�������ϵ��"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_IBCOEF, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_Ib_Coefficient = (INT16U)temp;
			}
        break;
        case 0x12:/*"���߹��ʻ���ϵ��"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_PBCOEF, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_Pb_Coefficient = (INT16U)temp;
			}
        break;
        case 0x13:/*"���߹�������"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_GPQB, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_GPQB = (INT16U)temp;
			}
        break;
        case 0x14:/*"���߹���ƫ��"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_APOSB, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_APOSB = (INT16U)temp;
			}
        break;
        case 0x15:/*"������λ����"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_PHSB, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_PhsB = (INT16U)temp;
			}
        break;
        case 0x16:
        	break;
        #if EDT_MODE
		case _05_R_I_AMPSTD:	/* "���߼�������·��ֵ��׼" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_I_AMPSTD, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.I_AmpStd = temp;
			}
			break;
		case _05_R_I_PHSSTD:	/* "���߼�������·��λ��׼" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_I_PHSSTD, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.I_PhsStd = temp;
			}
			break;
		case _05_R_U400_AMPSTD:	/* "���߼���ѹ��·��ֵ��׼U400" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_U400_AMPSTD, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.U400_AmpStd = temp;
			}
			break;
		case _05_R_U400_PHSSTD:	/* "���߼���ѹ��·��λ��׼U400" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_U400_PHSSTD, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.U400_PhsStd = temp;
			}
			break;
		case _05_R_U10_AMPSTD:	/* "���߼���ѹ��·��ֵ��׼U10" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_U10_AMPSTD, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.U10_AmpStd = temp;
			}
			break;
		case _05_R_U10_PHSSTD:	/* "���߼���ѹ��·��λ��׼U10" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_U10_PHSSTD, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.U10_PhsStd = temp;
			}
			break;
        case _05_R_U10_AMPSTD2:	/* "���߼���ѹ��·��ֵ��׼U10" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_U10_AMPSTD2, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.U10_AmpStd2 = temp;
			}
			break;
		case _05_R_U10_PHSSTD2:	/* "���߼���ѹ��·��λ��׼U10" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_U10_PHSSTD2, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.U10_PhsStd2 = temp;
			}
			break;
        #endif /* "EDT_MODE" */

        default :
        	result= COMM_ERR_NODATA;
        break;
	}

    #if CT_WIDE_RANGE
    if( (tmp >= 0x20) && (tmp <=0x29) )
    {
        //LIB_MemCpy((INT8U *)&gMea_para.ct_cali_data.Ct_IMAX+2*(tmp-0x20), (UINT8 *)temp,2);
        result = COMM_ERR_NOERR;
    }
    #endif

	LIB_Reverse((INT8U*)&temp, (INT8U*)&pData[3], 4);
	return result;
}
/*"************************************************
Function:MU_Dlt05Cmd
Description:������оƬ�Ĵ�������
Input:  pData У������֡����ָ��
Output: 	��
Return:TRUE  �ɹ�  FALSE  ʧ��
Others:
************************************************"*/
INT8U MU_CaliReadOther(INT8U *pData,INT8U *length)
{
  INT8U ret=COMM_ERR_NOERR;

  return ret;
}

/*"--------�� �� �� ��--------"*/
#if AMEND_MODE
void Amend_OneverySeond(void)
{
  gAmendtimer.Amend_Resume=0;
  	Amend_out_data.Voltage = (INT16U)(gMU_Interdata.Voltage/1000);
	Amend_out_data.Current = (INT16U)(gMU_Interdata.CurrentA/100);
	Amend_out_data.Factor= (INT16U)(gMU_Interdata.FactorA/100);

      // if(((Amend_out_data.Current>2300)&&(Amend_out_data.Current<2700))||((Amend_out_data.Current>4700)&&(Amend_out_data.Current<5300)))/*""��5%IB=5A����5%�����Լ�10%IB 0.5L���޵���ѹӰ����"*/
	if(Current_IN(25)||Current_IN(50))
	  {
	Amend_Un();/*""��ѹӰ�����޵�"*/
	    }
	Amend_Harmonic();/*""5��г��Ӱ���޵�����"*/
	Amend_Imax();/*""����ʵ�鲹��"*/
	Amend_0_5mT();/*""0.5MT�޵�"*/
	Amend_Up_Down_Main();/*"��������޵�"*/
	if((gAmendtimer.Amend_Resume==0)&&(gAmendtimer.Amend_Complete!=0))/*""�����ǰ״̬��������������֮ǰ���������ͻָ��������ĵ�Уֵxl,20110907"*/
		{

		   //DC_GetData(E2_MEA_PARA1,(INT8U *)&gMU_Para,0,sizeof(ST_MU_PARA));
                   FS_ReadFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT,(INT8U *)&gMU_Para,sizeof(ST_MU_PARA));
			MU_IC_WriteReg(WRT_PRTCT, (INT8U  *)&Write_Unlock,1);//��д����
			MU_IC_WriteReg(GP, (INT8U  *)&gMU_Para.ram_adj_data.Ram_gp,2);//д�Ĵ���_GP
			MU_IC_WriteReg(WRT_PRTCT, (INT8U  *)&Write_lock,1);//��д����
		      RegScale_GP_Now = gMU_Para.ram_adj_data.Ram_gp;
		      gMU_Para.CRC_Val = CRC16((INT8U*)&gMU_Para, FIND(ST_MU_PARA,CRC_Val));
	           gAmendtimer.Amend_Complete=0;/*""���õ�ǰΪδ����״̬ xl,20110907"*/
		    gAmendtimer.Amend_Resume=0; /*"" ���õ�ǰΪ����Ҫ�������� xl,20110907"*/
		}
}
INT8U Set_Amend(INT8U *pData)/*""�����޵�����"*/
{
		 INT8U RetVal = COMM_ERR_NOERR;
		 INT32S *pointer;
		 INT32U temp;
		  LIB_Reverse (&pData[5], (INT8U *)&temp, 4);/*""У�������·���ʱ��û�иߵ��ֽڷ���"*/
		  if(Cal_Status_flag==CAL_STATUS_START)
		  	{
		  		if(pData[4]<=0x0F)/*""��У��ʼ�����������÷�Χ������޸�"*/
				 {
        			pointer = (INT32S *)&gMU_Para.AmendValue.Ib_80Un_1;
             	       	pointer += pData[4];
				if((pData[4]>0)&&(pData[4]<0x06))	/*""��ѹ�޵�Э���Ӧ"*/
				{
				pointer++;
				}
				else if((pData[4]>=0x06)&&(pData[4]<=0x08))	/*""��������޵�ֵЭ���Ӧ"*/
				{
				pointer+=2;
				}
				else if((pData[4]>=0x09)&&(pData[4]<=0x0B))
				{
				pointer = (INT32S *)&gMU_Para.AmendValue.Imax_10min_1;
				}
				LIB_MemCpy((INT8U *)&temp,(INT8U*)pointer,4);
				//gAmendtimer.Amend_Setchange=1;
				 }
				else if(pData[4]==0xFF)
			  	 {
				LIB_MemSet(0, (INT8U *)&gMU_Para.AmendValue, sizeof(ST_AMEND_VALUE));
				//gAmendtimer.Amend_Setchange=1;
				}
			//	else  if(pData[4]==0x0F)//5��г��
				//{
			//	LIB_MemCpy((INT8U *)&temp,(INT8U*)&gMU_Para.AmendValue.Harmonic, 4);
				//gAmendtimer.Amend_Setchange=1;
				//}
				else
				{
			   	RetVal= COMM_ERR_NODATA;
				}
				if(RetVal == COMM_ERR_NOERR)
		        	 {
		        	 gAmendtimer.Amend_Setchange=1;
		         	//DC_GetData(E2_MEA_PARA1,(INT8U *)&gMU_Para.ram_adj_data.Ram_gp,12,2);//��ȡ���ʱȲ��ԭʼֵ
			        FS_ReadFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT+FIND(ST_MU_PARA,ram_adj_data)+FIND(RAM_ADJ_DATA,Ram_gp),(INT8U *)&gMU_Para.ram_adj_data.Ram_gp,2);
		        	MU_CaliSavePara();
		        	 }
		  	}
			else
				{
			       RetVal= COMM_ERR_NODATA;
				}
		      return RetVal;
}


INT8U  Read_Amend(INT8U *pData)/*""���޵�����"*/
{
	INT8U  result=COMM_ERR_NOERR;
	//INT8U  i;
	 INT32S *pointer;
	if(pData[4]<=0x0F)
		{
		//DC_GetData(E2_MEA_PARA1,(INT8U *)&gMU_Para,0,sizeof(ST_MU_PARA));/*""�����޵����ڿ�ֵ"*/
                FS_ReadFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT,(INT8U *)&gMU_Para,sizeof(ST_MU_PARA));
		pointer = (INT32S *)&gMU_Para.AmendValue.Ib_80Un_1;
              pointer += pData[4];
		if((pData[4]>0)&&(pData[4]<0x06))	/*""��ѹ�޵���Ӧֵ"*/
		{
		pointer++;
		}
		else if((pData[4]>=0x06)&&(pData[4]<=0x08))	/*""��������޵�ֵЭ���Ӧ"*/
		{
		pointer+=2;
		}
		else if((pData[4]>=0x09)&&(pData[4]<=0x0B))
		{
		pointer = (INT32S *)&gMU_Para.AmendValue.Imax_10min_1;
		}
	       LIB_MemCpy((INT8U*)pointer, &pData[3], 4);
		}

//	else if(i==0x0F)
	//	{
	//	LIB_MemCpy((INT8U*)&gMU_Para.AmendValue.Harmonic,&pData[3], 4);
	//	}
    else
    	{
    	  result= COMM_ERR_NODATA;
    	}
	return result;
}


INT8U LoadEEParam_Amend(void)
{
    INT8U RetVal = TRUE;
    //DC_GetData(E2_MEA_PARA1,(INT8U *)&gMU_Para,0,sizeof(ST_MU_PARA));/*""�����޵����ڿ�ֵ"*/
    FS_ReadFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT,(INT8U *)&gMU_Para,sizeof(ST_MU_PARA));
    if(gMU_Para.CRC_Val !=  CRC16((INT8U *)&gMU_Para, FIND(ST_MU_PARA,CRC_Val)))
	LIB_MemSet(0, (INT8U *)&gMU_Para.AmendValue, sizeof(ST_AMEND_VALUE));/*""������ڿ�������ֵCRCУ��ʧ�ܣ�����޵�ֵ��0�������޵�"*/
	return RetVal;
}


/*"************************************************
Function: Amend_Un
Description:X%Un����¾�����������
Global: mAmendValue��gAmendtimer
Calls:  ��
Input:  ��
Output: ��
Return: ��
Others: ��
*************************************************/
INT8U UType = 0;
INT8U factorNum = 0;
INT32S TempAmendValue1;
 void Amend_Un(void)
 {
	//INT32U TempAmendValue;
	//INT32U TempAmendValue1;
	INT32S *pointer;
	INT8U Num;
	pointer = (INT32S *)&gMU_Para.AmendValue.Ib_80Un_1;

     for(UType=0;UType<4;UType++)
     	{
 	if((Amend_out_data.Voltage>U_Data[UType*2])&&(Amend_out_data.Voltage<U_Data[UType*2+1]))/*""��ǰ��ѹΪdata1Min��data1Max֮��ʱ"*/
   	{
   		 gAmendtimer.Amend_Resume=1;/*""���õ�ǰΪ��������"*/
    		if((Amend_out_data.Factor>95)&&(Amend_out_data.Factor<104))/*""������������Ϊ1.0ʱ�ľ���"*/
    		{
    		        factorNum = 1;
    		}
		if((Amend_out_data.Factor>45)&&(Amend_out_data.Factor<55))/*""������������Ϊ0.5Lʱ�ľ���"*/
    		{
    		        factorNum = 2;
    		}
		if(factorNum!=0)
		{
			Num = (factorNum + UType*2);//�������ĸ��޵�����
		   if((gAmendtimer.Amend_Complete==Num)&&(gAmendtimer.Amend_Setchange==0))/*""�����һ���޸��޸��˵㲢���޸�ֵû�����±����ã��򲻽�����"*/
	 		{
	 		}
			else
			{
				LoadEEParam_Amend();
				pointer += (Num-1);
				TempAmendValue1= (INT32S)*pointer;
				if(Cal_gp(TempAmendValue1))
					{
					gAmendtimer.Amend_Complete=Num;//�����������Ӧ�������״̬
					}
			}
		}
 	}
     	}
 }


/*"************************************************
Function: Amend_Imax
Description:Imax��������¾�����������
Global: mAmendValue��gAmendtimer
Calls:  ��
Input:  ��
Output: ��
Return: ��
Others: ��
*************************************************/
INT32S Temp_Amend;
 void Amend_Imax()
 {
	//INT32S Temp;
	INT8U Min;
	INT8U factor = 0;
	if(gMU_Para.AmendValue.Imax_10min_1!=0)//��չIMAX10����1.0��Ϊ�̵���ģʽ�л��㡣
	{
	Imax_Data[0] = 600;//�귢�̵���
	Imax_Data[1] = 1800;
	Imax_Data[2] = 1800;
	Imax_Data[3] = 3000;
	Imax_Data[4] = 3000;
	Imax_Data[5] = 4200;
	}
	else
	{
	Imax_Data[0] = 180;//�����̵���
	Imax_Data[1] = 420;
	Imax_Data[2] = 420;
	Imax_Data[3] = 600;
	Imax_Data[4] = 600;
	Imax_Data[5] = 4200;
	}

 	//if((Amend_out_data.Current>5500)&&(Amend_out_data.Current<9000))/*""��ǰ����Ϊ55A��90A֮��ʱ"*/
	if(Current_IN(6000))
	{
			if(gAmendtimer.Amend_Imax_Time<4200)/*""��70������"*/
   			{
   				gAmendtimer.Amend_Imax_Time++;/*""Imax��������ʱ��ÿ������"*/
   			}
  		gAmendtimer.Amend_Imax_LoseTime=0;/*""��������仯ʱ����0"*/
        for(Min=0;Min<3;Min++)
        {
   		if((gAmendtimer.Amend_Imax_Time>Imax_Data[Min*2])&&(gAmendtimer.Amend_Imax_Time<=Imax_Data[Min*2+1]))/*""Imax����600�뵽1800��֮���ʼ��һ������"*/
   		{
   			gAmendtimer.Amend_Resume = 1;/*""���õ�ǰΪ��������"*/
   			 if((Amend_out_data.Factor>95)&&(Amend_out_data.Factor<104))
   			 	{
   			 	factor = 0x0D;
   			 	}
			else if ((Amend_out_data.Factor>45)&&(Amend_out_data.Factor<55))
				{
				factor = 0x0E;
				}
			if(factor!=0)
      			{
      				if((gAmendtimer.Amend_Complete==(factor + 3*Min))&&(gAmendtimer.Amend_Setchange==0))/*""�����һ���޸��޸��˵㲢���޸�ֵû�����±����ã��򲻽���������ά��ԭ״"*/
	 			{
	 			}
				else
				{
						LoadEEParam_Amend();/*""���ڿ���ȡ����ֵ"*/
						Temp_Amend = (INT32S)(*((INT32U *)&gMU_Para.AmendValue.Imax_50min_1+(factor-0x0D)));
                       			 Temp_Amend = (Temp_Amend*(Min+1))/3;
				 		TempAmendValue1 = Temp_Amend;
	#if 0
						if(((*(&gMU_Para.AmendValue.Imax_50min_1+(factor-0x0D)))&0x80000000)!=0)/*""�����УֵΪ��ֵ"*/
						{
						TempAmendValue1   =(0xFFFFFFFF- (0xFFFFFFFF- (*(&gMU_Para.AmendValue.Imax_50min_1+(factor-0x0D))))*(Min+1)/3);
						}
						else
						{
						TempAmendValue1   = (*(&gMU_Para.AmendValue.Imax_50min_1+(factor-0x0D)))*(Min+1)/3;
						}
	#endif
						if(Cal_gp(TempAmendValue1))
						{
	 					gAmendtimer.Amend_Complete=(INT8U)(factor + 3*Min);/*"�����������Ӧ�������״̬"*/
     	 					}


     	 				}
						}
   			 }
   		}
   	}
	else
  	{
	  		gAmendtimer.Amend_Imax_LoseTime++;/*""��������10IBʱ����������仯ʱ�俪ʼ��ʱ"*/
	 		if(gAmendtimer.Amend_Imax_LoseTime>15)/*""����������仯ʱ�����15����ʱ���жϵ�ǰ�Ѿ��л�����������С�"*/
	 		{
	 			gAmendtimer.Amend_Imax_Time=0;
				gAmendtimer.Amend_Imax_LoseTime=0;
	 		}
	   		return;
  	}
    }


/*"************************************************
Function: Amend_Harmonic
Description:5��г���¾�����������
Global: mAmendValue��gAmendtimer
Calls:  ��
Input:  ��
Output: ��
Return: ��
Others: ��
*************************************************/
void Amend_Harmonic()//5��г��Ӱ��
{
	if(((Amend_out_data.Current>2200)&&(Amend_out_data.Current<2970))||((Amend_out_data.Current>3030)&&(Amend_out_data.Current<4500)))//&&(((Data1>2050000)&&(Data1<2185000))||((Data1>2215000)&&(Data1<2350000))))
  	{
		gAmendtimer.Amend_Resume=1;/*""���õ�ǰΪ��������"*/
		if((gAmendtimer.Amend_Complete==0x16)&&(gAmendtimer.Amend_Setchange==0))/*""�����һ���޸��޸��˵㲢���޸�ֵû�����±����ã��򲻽���������ά��ԭ״"*/
		{
		}
		else
		{/*""�����һ�β��������˵�������������˵�Уֵ����ִ������"*/
			LoadEEParam_Amend();/*""���ڿ���ȡ����ֵ"*/
			if(Cal_gp(gMU_Para.AmendValue.Harmonic))
			{
		         	gAmendtimer.Amend_Complete=0x16;/*""�����������Ӧ�������״̬"*/
	          	}
	           	//}
		}/*""���õ�У����Ϊδ�仯״̬"*/
	}
    	else
	{
		return;
	}
}

INT32U MT_P0 = 0;/*""��׼����"*/
INT32U MT_P1 = 0;/*""��ȡ����"*/
INT32U MT_U0 = 0;/*""��ȡ��ѹ"*/
INT32U MT_U1 = 0;/*""��ȡ��ѹ"*/


/*"************************************************
Function: Amend_0_5mT
Description:0.5mT�޵�Ӱ����������������
Global: mAmendValue��gAmendtimer
Calls:  ��
Input:  ��
Output: ��
Return: ��
Others: ��
*************************************************/
void Amend_0_5mT(void)/*""0.5MT�޵�"*/
{
  INT8U MT_Test;
  INT32S Abs;
       if(((280 < gMU_Interdata.CurrentA)&&(gMU_Interdata.CurrentA < 850)))/*"��50mA�������ж�˲ʱ������"*/
	{
			MT_Timer1++;
			if((MT_Timer1 > 10)&&(mT_Start_Flag==0))/*"����ʮ�붼��50���븽������ΪҪ׼����0.5mT����"*/
			 {
			 mT_Start_Flag = 0x01;/*"��ʼ��0.5mT��־"*/
			 //DC_OutputData(E2_MT_START_FLG,(INT8U *)&mT_Start_Flag,0,1);/*"�洢0.5mT��־"*/
             FS_WriteFile(E_FILE_LRM,FIND(ST_LRM_FILE_DATA,MtStartFlg),(INT8U *)&mT_Start_Flag,1);
			 }
	}
	else
	{
			 MT_Timer1 = 0;
	}

 // if(((48000 < gMU_Interdata.CurrentA)&&(gMU_Interdata.CurrentA< 52000)) &&(mT_Start_Flag == 1))/*""��5A��������ʼ��0.5MT��־"*/
 	if(Current_IN(500)&&(mT_Start_Flag == 1))
  	{
  	  if(MT_K < 3600)
  	  {
	   if(MT_K == 0)
	    {
	    MT_P0 = 0;
	    }
  	    MT_K++;
  	    if((MT_K > 11)&&(MT_K <= 15))
  	    {
	    	MT_P0 += gMU_Interdata.PowerPA;
	 //   i = (INT8U)(MT_K-11);
	   // MT_Power[i] = gMU_Interdata.PowerPA;
  		}
		if(MT_K == 16)
		{
		//for(k=0;k<5;k++)
		//{
		//MT_P0 += MT_Power[k];
		//}
		MT_P0 = MT_P0>>2;						/*""ȡ4�ι���ƽ��ֵΪ�ο�ֵ��"*/
		MT_U0 = Amend_out_data.Voltage;
		MT_Test = 0;
		//DC_OutputData(E2_MT_START_FLG,&MT_Test,0,1);/*""�洢0.5mT��־"*/
		FS_WriteFile(E_FILE_LRM,FIND(ST_LRM_FILE_DATA,MtStartFlg),(INT8U *)&MT_Test,1);
              LoadEEParam_Amend();/*""���ڿ���ȡ����ֵ"*/
              RegScale_GP_Now = gMU_Para.ram_adj_data.Ram_gp;/*""׼����ԭʼ���ʱȲ�Ĵ�����ֵ"*/
		}
		if((15 < MT_K)&&(MT_K <3600))/*""�ӵ�15s���ӿ�ʼ��1��Сʱ"*/
		{
		gAmendtimer.Amend_Resume=1;/*""���õ�ǰΪ��������"*/
		MT_P1 = gMU_Interdata.PowerPA;/*""������ֵ"*/
		MT_U1 = Amend_out_data.Voltage;/*""����ѹֵ"*/
		//Abs = LIB_ABS(MT_P1 - MT_P0);
		Abs = MT_P1-MT_P0;
		if((Abs < 55)&&(Abs >-55))			/*""���ʴ��ڱ�־ֵ, ����Ϊ1.1Kw��0.5%"*/
		{
			Abs = ((Abs<<15)/ MT_P0);/*"(P1-P0/P0)*32768"*/
			//if(MT_P1  > MT_P0 )
		//	{
	  		//RegScale_GP_Now = (RegScale_GP_Now - Abs);
	  		//}
			//else if(MT_P1  < MT_P0 )
			//{
			RegScale_GP_Now =(INT32U)(RegScale_GP_Now + Abs);
			//}
		}

			/*""���Ƶ�ѹ��0.5%��Χ�ڱ䶯2.2V"*/
 			/*" ���ʱȲ�Ĵ�����ԭʼֵ�Ƚϸı�������0.5%����򲻲������ָ�ԭʼֵ"*/
			//DC_GetData(E2_MEA_PARA1,(INT8U *)&gMU_Para.ram_adj_data.Ram_gp,12,2);//��ȡ���ʱȲ��ԭʼֵ
			FS_ReadFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT+FIND(ST_MU_PARA,ram_adj_data)+FIND(RAM_ADJ_DATA,Ram_gp),(INT8U *)&gMU_Para.ram_adj_data.Ram_gp,2);
			if((LIB_ABS(MT_U1 - MT_U0)>22)||(LIB_ABS((INT32U)gMU_Para.ram_adj_data.Ram_gp - (INT32U)RegScale_GP_Now)>165))/*""165����0.5%���Ĺ��ʱȲ�ֵ"*/
			{
			RegScale_GP_Now = gMU_Para.ram_adj_data.Ram_gp;
			}
	 	Write_RegSCALE_E1();
	     gAmendtimer.Amend_Complete=0x17;/*""�����������Ӧ�������״̬"*/
	     gAmendtimer.Amend_Setchange=0;
	    }
  	   }
   }
}

void  Write_RegSCALE_E1(void)
{
    gMU_Para.ram_adj_data.Ram_gp = RegScale_GP_Now;
	gMU_Para.CRC_Val = CRC16((INT8U*)&gMU_Para, FIND(ST_MU_PARA,CRC_Val));
	MU_IC_WriteReg(WRT_PRTCT, (INT8U  *)&Write_Unlock,1);//��д����
	MU_IC_WriteReg(GP, (INT8U  *)&gMU_Para.ram_adj_data.Ram_gp,2);//д�Ĵ���_GP
	MU_IC_WriteReg(WRT_PRTCT, (INT8U  *)&Write_lock,1);//��д����
}


/*"************************************************
Function: Amend_Up_Down_Main
Description:��������޵�������
Global: mAmendValue��gAmendtimer
Calls:  ��
Input:  ��
Output: ��
Return: ��
Others: ��
*************************************************/
void Amend_Up_Down_Main()
{
	Amend_Up_Down_Out();			/*"��������޵��˳�����"*/
	if((Amend_out_data.Voltage > 2150)&&(Amend_out_data.Voltage < 2250)&&(Amend_out_data.Factor>95)&&(Amend_out_data.Factor<104))
	{
	Amend_Up_Down_Judge();		/*"��������޵��жϻ���"*/
	Amend_Up_Down();				/*"��������޵���������"*/
	}
}

/*"************************************************
Function: Amend_Up_Down_Judge
Description:��������ʶ�����
Global: mAmendValue��gAmendtimer
Calls:  ��
Input:  ��
Output: ��
Return: ��
Others: ��
*************************************************/
void Amend_Up_Down_Judge()
{
//if((Amend_out_data.Current > 23)&&(Amend_out_data.Current < 27))/*"��ǰ����Ϊ230mA��270mA֮��ʱ"*/
if(Current_IN(25))
  {
    	 if(gAmendtimer.Amend_Up0_05Ib_Timer <=30)/*"��5%IB֮��ʱ��ά��30��������ʱ��"*/
     		{
     		gAmendtimer.Amend_Up0_05Ib_Timer ++;
     		}
	 else if((gAmendtimer.Amend_UpDownIbFlg != 0x55)&&(gAmendtimer.Amend_Up0_05Ib_Timer >30))
	 	{
	 	gAmendtimer.Amend_UpDownIbFlg = 0x55;
		gAmendtimer.Amend_Judge_Timer =0;
	 	}
     }
else
    {
	gAmendtimer.Amend_Up0_05Ib_Timer = 0;
    }

//if((Amend_out_data.Current > 480)&&(Amend_out_data.Current < 520))/*"��ǰ����Ϊ4.8A��5.2A֮��ʱ"*/
	if(Current_IN(500))
     {
	if(gAmendtimer.Amend_UpDownIbFlg ==0x55)
	{
		if(gAmendtimer.Amend_UpIb_Timer < 4)
		{
		gAmendtimer.Amend_UpIb_Timer ++;
		}
		else if((gAmendtimer.Amend_UpDownImaxFlg != 0x55)&&(gAmendtimer.Amend_UpIb_Timer >=4))
		{
		gAmendtimer.Amend_UpDownImaxFlg = 0x55;
		}

	}
     }
else
      	{
      		gAmendtimer.Amend_UpIb_Timer = 0;
      	}

//if((Amend_out_data.Current > 5500)&&(Amend_out_data.Current < 6500))/*"��ǰ����Ϊ55A��65A֮��ʱ"*/
	if(Current_IN(6000))
	{
	  if(gAmendtimer.Amend_UpDownImaxFlg ==0x55)
	  {
	    if(gAmendtimer.Amend_UpImax_Timer < 60)
	    	{
	    	  gAmendtimer.Amend_UpImax_Timer ++;
	    	}
	    else if((gAmendtimer.Amend_UpDownFlg != 0xAA)&&(gAmendtimer.Amend_UpImax_Timer >=60))
	    	{
	    	gAmendtimer.Amend_UpDownFlg = 0xAA;/*"�жϴ����޵�״̬��ʶ"*/
		gAmendtimer.Amend_Judge_Timer = 0;
	    	}
	  }
	}
else
	 {
	 gAmendtimer.Amend_UpImax_Timer = 0;
	 }
}
/*"************************************************
Function: Amend_Up_Down_Out
Description:���������˳�����
Global: mAmendValue��gAmendtimer
Calls:  ��
Input:  ��
Output: ��
Return: ��
Others: ��
*************************************************/
void Amend_Up_Down_Out()
{
  if((gAmendtimer.Amend_UpDownIbFlg == 0x55)&&(gAmendtimer.Amend_UpDownFlg!=0xAA))
	{
		gAmendtimer.Amend_Judge_Timer++;
		if(gAmendtimer.Amend_Judge_Timer > 600)/*"��������������̳�����10���ӣ��������־�������ж�,ʵ�ʲ����������ʱ������ʱ��<5����"*/
		{
		//Amend_Up_Down_Clear();
		LIB_MemSet(0, (INT8U *)&gAmendtimer.Amend_Judge_Timer, 10);
		}
	}

  if(gAmendtimer.Amend_Down_0_05Ib_Timer > 480)/*"���������������5%IB������8���ӣ����˳�"*/
  	{
		//Amend_Up_Down_Clear();
		LIB_MemSet(0, (INT8U *)&gAmendtimer.Amend_Judge_Timer, 10);
  	}

  if(gAmendtimer.Amend_Down_0_05Ib_Timer >30)/*"���������������5%IB������30���ӣ�����һ���л������������˳�"*/
  	{
	 if((Amend_out_data.Current > 23)&&(Amend_out_data.Current < 27)&&(Amend_out_data.Voltage > 2150)&&(Amend_out_data.Voltage < 2250)&&(Amend_out_data.Factor>95)&&(Amend_out_data.Factor<104))
	 	{
	 	}
	 else			/*"����Ѿ�������5%IB���������л��¸����������˳�"*/
	 	{
		//Amend_Up_Down_Clear();
		LIB_MemSet(0, (INT8U *)&gAmendtimer.Amend_Judge_Timer, 10);
	 	}
  	}
}
	#if 0
/*"�������������жϱ�־��ʱ��"*/
void Amend_Up_Down_Clear()
{
	LIB_MemSet(0, (INT8U *)&gAmendtimer.Amend_Judge_Timer, 10);

	gAmendtimer.Amend_Judge_Timer = 0;
	gAmendtimer.Amend_UpDownIbFlg = 0;
	gAmendtimer.Amend_UpDownImaxFlg = 0;
	gAmendtimer.Amend_UpDownFlg = 0;
	gAmendtimer.Amend_UpImax_Timer = 0;
	gAmendtimer.Amend_UpIb_Timer = 0;
	gAmendtimer.Amend_Down_0_05Ib_Timer = 0;
	gAmendtimer.Amend_Up0_05Ib_Timer = 0;

}
	#endif

/*"************************************************
Function: Amend_Up_Down
Description:�������Ȳ���
Global: mAmendValue��gAmendtimer
Calls:  ��
Input:  ��
Output: ��
Return: ��
Others: ��
*************************************************/
void Amend_Up_Down()
{
     if((gAmendtimer.Amend_UpDownFlg == 0xAA)&&(gAmendtimer.Amend_UpDownIbFlg == 0x55)&&(gAmendtimer.Amend_UpDownImaxFlg == 0x55))/*"����֮ǰ5%IB,IB,Imax���ù���ʶ�����жϽ����޵�"*/
     	{
     		//if((Amend_out_data.Current > 23)&&(Amend_out_data.Current < 27))/*"��ǰ����Ϊ230mA��270mA֮��ʱ"*/
			if(Current_IN(25))
			{
     		 	gAmendtimer.Amend_Resume = 1;/*""���õ�ǰΪ��������"*/
     		 	gAmendtimer.Amend_Down_0_05Ib_Timer ++;
	 		if((gAmendtimer.Amend_Complete!=0x18)&&(gAmendtimer.Amend_Setchange==0))/*""�����һ���޸��޸��˵㲢���޸�ֵû�����±����ã��򲻽���������ά��ԭ״"*/
	 		{
	 			LoadEEParam_Amend();/*""���ڿ���ȡ����ֵ"*/
				if(Cal_gp(gMU_Para.AmendValue.Up_Down_0_05Ib))
				{
	 				gAmendtimer.Amend_Complete=0x18;/*"�����������Ӧ�������״̬"*/
     	 			}
	 		}
     		}
		else
		{
		gAmendtimer.Amend_Down_0_05Ib_Timer = 0;
		}

		//if((Amend_out_data.Current > 480)&&(Amend_out_data.Current < 520))/*"��ǰ����Ϊ4.8A��5.2A֮��ʱ"*/
		if(Current_IN(500))
		{
	 		gAmendtimer.Amend_Resume = 1;/*""���õ�ǰΪ��������"*/
			if((gAmendtimer.Amend_Complete!=0x19)&&(gAmendtimer.Amend_Setchange==0))/*""�����һ���޸��޸��˵㲢���޸�ֵû�����±����ã��򲻽���������ά��ԭ״"*/
	 		{
	 			LoadEEParam_Amend();/*""���ڿ���ȡ����ֵ"*/
				if(Cal_gp(gMU_Para.AmendValue.Up_Down_Ib))
				{
	 				gAmendtimer.Amend_Complete=0x19;/*"�����������Ӧ�������״̬"*/
     	 			}
	 		}
	 	}

		//if((Amend_out_data.Current > 5500)&&(Amend_out_data.Current < 6500))
		if(Current_IN(6000))
		{
			gAmendtimer.Amend_Resume = 1;/*""���õ�ǰΪ��������"*/
			if((gAmendtimer.Amend_Complete!=0x1A)&&(gAmendtimer.Amend_Setchange==0))/*""�����һ���޸��޸��˵㲢���޸�ֵû�����±����ã��򲻽���������ά��ԭ״"*/
	 		{
	 			LoadEEParam_Amend();/*""���ڿ���ȡ����ֵ"*/
				if(Cal_gp(gMU_Para.AmendValue.Up_Down_Imax))
				{
	 				gAmendtimer.Amend_Complete=0x1A;/*"�����������Ӧ�������״̬"*/
     	 			}
	 		}
     	 	}
   }
}


INT8U Current_IN(INT16U Current)
{
   INT16U Current_Min,Current_Max;
   INT8U result= FALSE;

   switch(Current)
   	{
   	case 25 ://5%IB
		Current_Min = 23;
		Current_Max =27;
	break;
	case 6000 ://Imax
		Current_Min = 5500;
		Current_Max =6500;
	break;
	case 500://Ib
		Current_Min = 480;
		Current_Max =520;
	break;
	case 50://10%Ib
		Current_Min = 48;
		Current_Max =52;
	default :
         	result= FALSE;
       break;
   	}
	//if((Amend_out_data.Current > Current_Min)&&(Amend_out_data.Current < Current_Max))
	if(Amend_out_data.Current > Current_Min)
	{
	  if(Amend_out_data.Current < Current_Max)
	  	{
	  		result = TRUE;
	  	}
	}
	return result;
}

INT8U Cal_gp(INT32S AmendValue)
{
 INT8U result= TRUE;
	if((AmendValue!=0)||(Cal_Status_flag == CAL_STATUS_START)||(gAmendtimer.Amend_Complete!=0))/*"��У��״̬������У���޵�ֵ��Ϊ0������һ���������޵���Ŀ����в�������"*/
		{
	        	//if((AmendValue<0)/*""�����УֵΪ��ֵ"*/
			//{
	       	//  RegScale_GP_Now =(INT16U) (gMU_Para.ram_adj_data.Ram_gp - AmendValue);
			//}
			//else
			//{
			RegScale_GP_Now = (INT16U) (gMU_Para.ram_adj_data.Ram_gp +AmendValue);
			//}
			Write_RegSCALE_E1();
			gAmendtimer.Amend_Setchange=0;/*"���õ�У����Ϊδ�仯״̬"*/
		}
	else
		{
		result= FALSE;
		}
	return result;
}
#endif
