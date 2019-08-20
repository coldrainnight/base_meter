/*******************************************************************************
*Copyright   : 2017  Wasion Group Ltd.
*FileName    : DU.C
*Description : ��������ģ��
Modify records:
Author       Date               Version           Description
HeZhuli      2017.2.7           v1.0              Created
*******************************************************************************/
#include "system\Sys_def.h"
#include "APP\message\MS.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "APP\message\FRMT_def.h" 
#include "App\demand\DU.h"
/*"*******************************************************************************"*/
st_DU_Ram  g_DU_Ram;    /*������ʱ����*/ 
NO_INIT ST_AVDM_DEF  stAvDmData[MAX_MD_NUM];  /*"ƽ����������"*/

NO_INIT ST_MD_DATA   stMDData;                /*"������������� ���� ��ʱ�ģ�һֱ�仯��*/

INT32S  Current_Demand[2];				      /*"��ǰ�����洢�ռ�0 �й� 1 �޹�"*/ 			          

NO_INIT ST_TLMD_DATA stTotalDm;               /*"��ʷ�ۼ������������"*/

ST_DU_PARA g_DU_Para;   /*��������*/

/*****************************************************************************
 �� �� ��  : DU_LoadDefaultDUPara
 ��������  : ��������ģ��Ĭ�ϲ���
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 04 10
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void DU_LoadDefaultDUPara(void)
{    
    g_DU_Para.validFlg     = mstDefaultDuPara.validFlg;
    g_DU_Para.DemandPeriod = mstDefaultDuPara.DemandPeriod;
    g_DU_Para.DemandTime   = mstDefaultDuPara.DemandTime;    
    g_DU_Para.NumOfPeriod  = g_DU_Para.DemandTime/g_DU_Para.DemandPeriod;    
   
    GetSingle(E_R_ENGY_COM_TYPE1, &g_DU_Para.DemandComMode1);
    GetSingle(E_R_ENGY_COM_TYPE2, &g_DU_Para.DemandComMode2); 
    
    g_DU_Para.Crc = CRC16((INT8U *) &g_DU_Para, FIND(ST_DU_PARA, Crc));
    
    FS_WriteFile(E_FILE_DU_PARA, DU_PARA_NUM_PERIOD_SHIFT, (INT8U *) &g_DU_Para,sizeof(ST_DU_PARA));
}

/*****************************************************************************
 �� �� ��  : DU_Change_Date_Time
 ��������  : ���ײ�ʱ��ת���ɴ洢ʹ�õ�ʱ��
 �������  : INT8U *pSrcDateTime  
             INT8U *pDstDateTime  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 03 31
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void DU_Change_Date_Time(INT8U *pSrcDateTime,INT8U *pDstDateTime)
{
    ST_FMT_ATTR inFormat  = {E_YYMMDDHHMMSSWW,E_NONE_UNIT,0};    //0x0031; 
    ST_FMT_ATTR outFormat = {E_YYMMDDHHMM,E_NONE_UNIT,0};  //0x0032;

    FRMT_DataChange(pSrcDateTime,pDstDateTime,inFormat,outFormat);

    //������? YMDHMSToDateTimeType(pSrcDateTime,pDstDateTime);
}    



/*****************************************************************************
 �� �� ��  : DU_Change_Date_Time
 ��������  : ���ײ�ʱ��ת����ͨ��ʱ�������ʹ�õ�ʱ��
 �������  : INT8U *pSrcDateTime  
             INT8U *pDstDateTime  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 03 31
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void DU_Change_Date_Time_Com(INT8U *pSrcDateTime,INT8U *pDstDateTime)
{
    ST_FMT_ATTR inFormat  = {E_YYMMDDHHMM,E_NONE_UNIT,0};    //0x0031; 
    ST_FMT_ATTR outFormat = {E_YYYYMMDDHHMMSS,E_NONE_UNIT,0};  //0x0032;

    FRMT_DataChange(pSrcDateTime,pDstDateTime,inFormat,outFormat);
    *(pDstDateTime+6) = 0;  /*�Ѹ�ʽת��������FF�̶�д��0*/

    //������? YMDHMSToDateTimeType(pSrcDateTime,pDstDateTime);
}  

/*****************************************************************************
 �� �� ��  : DU_MaxDemandClr
 ��������  : ���������0
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 04 03
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8S DU_MaxDemandClr(void *pdin, void *pdout)
{
    INT8U clr_value = (DU_TOTALMAX_DM|DU_MAX_DM|DU_CUR_AVE_DM);
    
    DU_Clr(&clr_value,NULL); 
    
    return MS_SUCCESS;
}    


/*****************************************************************************
 �� �� ��  : DU_R_DemandClr
 ��������  : �޹�������0
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 04 03
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8S DU_R_DemandClr(void *pdin, void *pdout)
{
    Current_Demand[1] = 0;

    LIB_MemSet(0,(INT8U *)&stAvDmData[Qp_I],  sizeof(ST_AVDM_DEF)*(MAX_MD_NUM-2));
    
    LIB_MemSet(0,(INT8U *)&g_DU_Ram.Data[Qp_I],sizeof(st_DU_RamType)*(MAX_MD_NUM-2));

    g_DU_Ram.UpdatePermit_R = g_DU_Para.NumOfPeriod;    
    
    return MS_SUCCESS;
}    

/*****************************************************************************
 �� �� ��  : DU_CurrentDemandClr
 ��������  : ���������0
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 04 03
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8S DU_CurrentDemandClr(void *pdin, void *pdout)
{
    INT8U clr_value = DU_CUR_AVE_DM;
    
    DU_Clr(&clr_value,NULL);
    
    return MS_SUCCESS;
}   


/*****************************************************************************
 �� �� ��  : DU_Update_R_com_type
 ��������  : ��������޹�����ֵ��ͬ������ģ�������޹������֣��Ա�����޹�
             ��������ʱ�ã���ֹgetǶ��
 �������  : void *pdin   
             void *pdout  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 04 13
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8S DU_Update_R_com_type(void *pdin, void *pdout)
{    
    g_DU_Para.DemandComMode1 = *(INT8U*)pdin;
    g_DU_Para.DemandComMode2 = *((INT8U*)pdin+1);  

    g_DU_Para.Crc = CRC16((INT8U *) &g_DU_Para, FIND(ST_DU_PARA, Crc));
    FS_WriteFile(E_FILE_DU_PARA, DU_PARA_NUM_PERIOD_SHIFT, (INT8U *) &g_DU_Para,sizeof(ST_DU_PARA));

    return MS_SUCCESS; 
}    

/*****************************************************************************
 �� �� ��  : GetTriffDemand
 ��������  : ��ȡ��ǰ���ʵ�����
 �������  : INT8U tariffNo  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 05 10
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void GetTriffDemand(INT8U tariffNo)
{
    INT8U i;
    CRC_DATA_TP crc; 
    
    /*2018-05-05:����Ҫ������ǰ���ʵ�ֵ�������õ�����һ�ε�ֵ��������ʱ���ʵ�ʷ��ʿ��ܱ���*/
    /*����ǰ���ʵ��������*/
	for(i=0;i<ERR_RELOADTIMES;i++)
	{		
	    FS_ReadFile(E_FILE_DU_DATA, DU_MD_DATA_ALL_DATA_TRIFF_SHIFT(tariffNo), (INT8U *)&stMDData.CurData,sizeof(ST_MD_FROM)); 
		crc= CRC16((INT8U *)&stMDData.CurData,FIND(ST_MD_FROM,Crc));	
		if(crc == stMDData.CurData.Crc)
		{
			break;
		}
	}
	
	if(i>=ERR_RELOADTIMES)
	{
		LIB_MemSet(0, (INT8U*)&stMDData.CurData, sizeof(stMDData.CurData));
		stMDData.CurData.Crc= CRC16((INT8U *)&stMDData.CurData, FIND(ST_MD_FROM,Crc));	
	}
}    


/*********************************************************************************
  Function:  DU_Clr
  Description�������ǰ����
  Calls:
  Called By:
  Input:   //flag:DU_CUR_AVE_DM:�嵱ǰ������DU_MAX_DM:���ƽ��������DU_TOTALMAX_DM:���ۼ��������
  Output:  //��
  Return:  //��
  Others:  //����˵��
*********************************************************************************/
INT8S DU_Clr(void *pdin, void *pdout)
{
	INT8U i;
    INT8U tariffNo; 
    ST_7TIME_FORM DateTime;
    INT8U flag;
    
    flag = *(INT8U*)pdin;
    
    GetSingle(E_SYS_TIME, (INT8U *)&DateTime);
    
    /*1.���������������ʱ����*/  /* ���嵱ǰ���� */
	if (flag&DU_CUR_AVE_DM) 
	{
	    Current_Demand[0] = 0;
        Current_Demand[1] = 0;

        /*DU_FIRST_DMҲ��������0��*/    
    	LIB_MemSet(0, (INT8U*)&g_DU_Ram, sizeof(g_DU_Ram));
		LIB_MemSet(0, (INT8U*)&stAvDmData, sizeof(ST_AVDM_DEF)*MAX_MD_NUM); 
        
		g_DU_Ram.last_min    = DateTime.min;        
        
		if ((DU_THROWN)&&(DateTime.sec != 0))
		{
			DU_FIRST1MIN =1;
		}
		else
		{
		               
		}

        /*�����·���*/
        //GetSingle(E_TARIFF_NOW, &tariffNo);	
        //g_DU_Ram.Cur_TariffNo = tariffNo;   /*����ֵ�������Ϊ0���ַ��ʻ���һ���������ڳ���*/
        GetTriffDemand(tariffNo);
		
	} 
    
    /*2.�嵱��������� �ܼ�������������*/
	if(flag&DU_MAX_DM)
	{
		//LIB_MemSet(0, (INT8U*)&stMDData.Total, sizeof(stMDData.Total.Data));
		//LIB_MemSet(0, (INT8U*)&stMDData.CurData, sizeof(stMDData.CurData.Data));
        LIB_MemSet(0, (INT8U*)&stMDData, sizeof(stMDData));

        #if 0  //��Ӧ���У�����ʱ����и�ֵ����̫��
		for(i=0; i<MAX_MD_NUM; i++)
		{
			DU_Change_Date_Time((INT8U*)&DateTime, (INT8U*)&stMDData.CurData.Data[i].Time);
			DU_Change_Date_Time((INT8U*)&DateTime, (INT8U*)&stMDData.Total.Data[i].Time);
		}
        #endif

        /*��CRC������CRC����һ��*/
		stMDData.Total.Crc= CRC16((INT8U *)&stMDData.Total, FIND(ST_MD_FROM,Crc));	
		stMDData.CurData.Crc= CRC16((INT8U *)&stMDData.CurData, FIND(ST_MD_FROM,Crc));	
		stMDData.Crc= CRC16((INT8U *)&stMDData, FIND(ST_MD_DATA,Crc));	
        
		/*���������*/
        FS_WriteFile(E_FILE_DU_DATA,DU_MD_DATA_ALL_DATA_SUM_SHIFT,(INT8U *)&stMDData.Total,sizeof(stMDData.Total));
        
		/*��ʱ�������*/
		for(i=1; i<=TARIFF_NUM; i++)
		{
            FS_WriteFile(E_FILE_DU_DATA,DU_MD_DATA_ALL_DATA_TRIFF_SHIFT(i),(INT8U *)&stMDData.CurData,sizeof(stMDData.CurData)); 
		}
	}

    /*3.����ʷ�������*/
    /*��ʷ�ۼ��������������ڿ�*/	
	if(flag&DU_TOTALMAX_DM)
	{		
		LIB_MemSet(0, (INT8U*)&stTotalDm, sizeof(stTotalDm));
		stTotalDm.Crc= CRC16((INT8U *)&stTotalDm, FIND(ST_TLMD_DATA,Crc));	
        FS_WriteFile(E_FILE_DU_DATA,DU_TOTAL_DM_SHIFT,(INT8U *)&stTotalDm,sizeof(ST_TLMD_DATA)); 
	}  

    return MS_SUCCESS;
    
}
/*****************************************************************************
 �� �� ��  : DU_ParaInit
 ��������  : ����������ʼ��
 �������  : 3  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 04 10
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
#define READ_DU_PARA_MAX_CNT    (3)
static void DU_ParaCheck(void)
{
    INT8U i; 
    INT8U Ret;

    Ret = LIB_ChkDataValid((INT8U *)&g_DU_Para,FIND(ST_DU_PARA, Crc),g_DU_Para.Crc);
    if(TRUE==Ret)
    {
        return;
    }
    
    for(i = 0; i < READ_DU_PARA_MAX_CNT; i++)
    {
        FS_ReadFile(E_FILE_DU_PARA, DU_PARA_NUM_PERIOD_SHIFT, (INT8U *) &g_DU_Para,sizeof(ST_DU_PARA));

        Ret = LIB_ChkDataValid((INT8U *)&g_DU_Para,FIND(ST_DU_PARA, Crc),g_DU_Para.Crc);
        if(TRUE == Ret)
        {
            break;
        }
        MCU_DelayMs(10);
    }

    /*"ȡ�ڿ�����ʧ��ʱ����ȱʡ����"*/
    if(i >= READ_DU_PARA_MAX_CNT) 
    {
        DU_LoadDefaultDUPara();
    }
}
/*********************************************************************************
  Function:  DU_Para_check
  Description������������⣬����ͳһ�ĳ�CRC��������Χ�����
  Calls:
  Called By:
  Input:   //��
  Output:  //��
  Return:  //��
  Others:  //����˵��
*********************************************************************************/
void DU_Para_check(void)
{
    DU_ParaCheck();

	if(g_DU_Para.DemandPeriod > MAX_MD_PERIOD)             /*1.����ʱ��*/
	{
		g_DU_Para.DemandPeriod = 1;
	}	
	else if(g_DU_Para.DemandTime > MAX_MD_PERIOD)          /*2.��������*/
	{
        g_DU_Para.DemandTime = 15;
	}

    
	if ((0==g_DU_Para.NumOfPeriod)||(g_DU_Para.NumOfPeriod > MAX_SLIP_NUM))  /*������*/
	{
		g_DU_Para.NumOfPeriod = MAX_SLIP_NUM;
	}

    if((0 == g_DU_Para.DemandComMode1)||(0 == g_DU_Para.DemandComMode2))
    {
        g_DU_Para.DemandComMode1 = DEFAULT_NA_MODE_1_VALUE;
        g_DU_Para.DemandComMode2 = DEFAULT_NA_MODE_2_VALUE;
    }    
    
}

#if 0
/*********************************************************************************
  Function:  DU_TariffNo_Check
  Description�����ʺ��л����
  Calls:
  Called By:
  Input:   //��
  Output:  //��
  Return:  //��
  Others:  //����˵��
*********************************************************************************/
void DU_TariffNo_Check(void)
{
    INT8U tariffNo;
    
    GetSingle(E_TARIFF_NOW, &tariffNo);
    
	if ((tariffNo!=g_DU_Ram.Cur_TariffNo)&&(tariffNo !=0)&&(tariffNo<=TARIFF_NUM))
	{
        g_DU_Ram.Cur_TariffNo = tariffNo;    
        GetTriffDemand(tariffNo);

        g_DU_Ram.UpdatePermit = g_DU_Para.NumOfPeriod;
	}
}
#endif


/*****************************************************************************
 �� �� ��  : DU_Tariff_Switch
 ��������  : ����ʱ���л�
 �������  : void *pdin   
             void *pdout  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 05 20
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8S DU_Tariff_Switch(void *pdin, void *pdout)
{
    INT8U tariffNo;

    //GetSingle(E_TARIFF_NOW, &tariffNo);

    tariffNo= *(INT8U*)pdin;
    
    if((tariffNo !=0)&&(tariffNo<=TARIFF_NUM))
    {    
        GetTriffDemand(tariffNo);
        g_DU_Ram.UpdatePermit = g_DU_Para.NumOfPeriod;
        return MS_SUCCESS; 
    }
    else
    {
        return MS_OTHER_ERR; 
    }    
}

/*********************************************************************************
  Function:    GetPowerValue
  Description�����˲ʱ����ֵ
  Calls:		
  Called By:
  Input:   index:Ap_P/Ap_N/Qp_P/Qp_N
  Output:  //��
  Return:  INT32����������ֵ
  Others:  //����˵��
*********************************************************************************/
INT32U GetPowerValue(INT8U index)
{
	INT8U  TmpIndex = E_MAC_DEMAND_TYPE;
    INT32S TmpPower = 0;
    
    //const INT16U DEMAND_OBIS[]={E_ACTIVE_PWR,E_REACTIVE_PWR};

    //if(index>=E_MAC_DEMAND_TYPE)
        
    /*�й�����*/
    if(index<=Ap_N)
    {
        GetSingle(E_ACTIVE_PWR,(INT8U *) &TmpPower);  
        if(TmpPower<0)
        {
            TmpIndex = Ap_N;             
            TmpPower = 0- TmpPower;     /*��Ϊ����ҲҪȡ��ֵ*/
        }  
        else
        {
            TmpIndex = Ap_P;                
        }       
    } 
    else if(index<=Qp_IV) /*�޹�����*/
    {
        GetSingle(E_QUAD_TOTAL,&TmpIndex);
                
        /* �����޹����ʣ� ��1��2����Ϊ�� */
    	if((0==TmpIndex)||(1==TmpIndex)) 
        {
            GetSingle(E_REACTIVE_PWR,(INT8U *) &TmpPower);            
        }
        else
        {
            GetSingle(E_REACTIVE_PWR,(INT8U *) &TmpPower);   
            TmpPower = 0-TmpPower;
        }  
        
        TmpIndex += 2;      /*�޹���Ӧ����Ϊ2-6������+2*/  
    }    

    if(TmpIndex==index)
    {
        return TmpPower;
    }    
    else
    {
        return 0;
    }        
	
}
/*********************************************************************************
  Function:    DU_PowerAdd
  Description������ֵ�ۼ�
  Calls:
  Called By:
  Input:   //��
  Output:  //��
  Return:  //��
  Others:  //����˵��
*********************************************************************************/
void DU_PowerAdd(void)
{
	INT8U i;
	INT32U tmp32u;
    
    /*����1min����������*/
	if (DU_FIRST1MIN)
	{
		return;
	}    
    
	g_DU_Ram.SkipTime_Count++;  
    
	for (i=0; i<MAX_MD_NUM; i++)
	{       
		tmp32u = GetPowerValue(i);
        /*���´���ֹ�����������������Գ���57.7V��30A�������빦�ʲ�ƥ�������*/
        if(MAX_POWER<tmp32u)
        {
           tmp32u = 0; 
        }    
   		g_DU_Ram.Data[i].SkipTime_Power += tmp32u/MULTIPLY_POWER;	
	}
}

/*********************************************************************************
  Function:    DU_CurAveValue
  Description����ǰƽ��ֵ����ӿ�
  Calls:
  Called By:
  Input:   flag:DU_FALSE,��������������������ʱ�����
           flag:DU_TRUE, ʵʱ������������ǰ����ʱ���ڵ�ǰ��������
  Output:  //��
  Return:  //��
  Others:  //����˵��
*********************************************************************************/
void DU_CurAveValue(INT8U flag)
{
	INT8U i,j;
	TYPE_DEMAND_CAL demand_kw;
    
	demand_kw = 0;
    
	for (i=0; i<MAX_MD_NUM; i++)
	{
        for(j = 0; j < g_DU_Para.NumOfPeriod; j++) 
        {
            demand_kw += g_DU_Ram.Data[i].power_buf[j];
        }
        
		if (DU_TRUE==flag)
		{
			demand_kw =demand_kw-g_DU_Ram.Data[i].power_buf[g_DU_Ram.Periods_Count]+(g_DU_Ram.Data[i].SkipTime_Power/(g_DU_Para.DemandPeriod*60));
            stAvDmData[i].TempValue =(demand_kw/g_DU_Para.NumOfPeriod)/MULTIPLY_POWER_POINTER;	  
        }
        else
        {
    		stAvDmData[i].CurrValue = (demand_kw/g_DU_Para.NumOfPeriod)/MULTIPLY_POWER_POINTER;	
        }   
        
        demand_kw =0;
	}
}


/*****************************************************************************
 �� �� ��  : GetDemandMode
 ��������  : ��ȡ����ͬ����ʽ
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ����һ 2018 05 07
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U GetDemandMode(void)
{
    INT8U DemandMode;
    
    GetSingle(E_EXT_MODE1, &DemandMode);

    if(DemandMode&0x80)
    {
        return 1;
    } 
    else
    {
        return 0;
    }    
    
}    

/*********************************************************************************
  Function:  UpDate_PowerBuf
  Description��ÿ������ʱ���ڵ������ۼ�ֵƽ��
  Calls:
  Called By:
  Input:   //��
  Output:  //��
  Return:  //��
  Others:  //����˵��
*********************************************************************************/
void UpData_PowerBuf(void)
{
	INT8U i;

	for (i=0; i<MAX_MD_NUM; i++)
	{
	    g_DU_Ram.Data[i].power_buf[g_DU_Ram.Periods_Count] = g_DU_Ram.Data[i].SkipTime_Power/(g_DU_Ram.SkipTime_Count);
		g_DU_Ram.Data[i].SkipTime_Power =0;
	}
    
    g_DU_Ram.Periods_Count++;
    
	/*�������жϣ��������ڣ���ת*/
	if (g_DU_Ram.Periods_Count>=g_DU_Para.NumOfPeriod) 
	{
		g_DU_Ram.Periods_Count = 0;
		DU_FIRST_DM            = 1;
	}

    //--------------------------------------------
	if (g_DU_Ram.UpdatePermit>0)
	{
		g_DU_Ram.UpdatePermit--;
	}

    if (g_DU_Ram.UpdatePermit_R>0)
	{
		g_DU_Ram.UpdatePermit_R--;
	}
    

}

/*"============================================================"*/
/*"��������"*/
/*"��;�����㵱ǰ���������ݵ�ǰ���ʷ�������޼��㵱ǰ���й��޹�����"*/
/*"������"*/
/*"���أ�"*/
/*"���ߣ�trend �汾��1.00"*/
/*"��Ҫ�Ǹ������޹�ϵ����ֵ�������š�"*/
/*"============================================================"*/
void Get_Current_Demand(void)
{
	INT8U  TmpIndex;  
    
    if(FALSE == ChkStatus(E_PWR_P_DIR))
    {
        Current_Demand[0] = stAvDmData[0].LastValue; 
    }  
    else
    {
        Current_Demand[0] = 0-stAvDmData[1].LastValue; 
    }   

    /*���ޱ仯�󣬲����������ڲ����޹�����*/
    if(g_DU_Ram.UpdatePermit_R)
    {
        return;
    }    
    
    GetSingle(E_QUAD_TOTAL,&TmpIndex);    

    /* �����޹����ʣ� ��1��2����Ϊ�� */
	if((0==TmpIndex)||(1==TmpIndex)) 
    {        
        Current_Demand[1] = stAvDmData[TmpIndex+2].LastValue; 
    }
    else
    {
        Current_Demand[1] = 0-stAvDmData[TmpIndex+2].LastValue;           
    }  
    
}

/*********************************************************************************
  Function:    DU_MaxValue
  Description�������������
  Calls:
  Called By:
  Input:   //��
  Output:  //��
  Return:  //��
  Others:  //����˵��
*********************************************************************************/
void DU_MaxValue(void)
{
	INT8U  saveflag , i;//crc;
  ST_7TIME_FORM DateTime;
  INT8U tariffNo;
    

    /*ͨ���洢��ʶ�������ݱ仯ʱ���Ŵ洢������Ƶ��дEEPROM*/
	saveflag = 0; 
    
	/*�ﵽ����������������*/
	if (DU_FIRST_DM ==1)
	{
        OC_MultiTermn(E_MULTI_DEMANDPERIOD);/*"�๦�ܶ������ʱ���л��ź�"*/
    
        GetSingle(E_SYS_TIME, (INT8U *)&DateTime);

        /*��������Ƚ�*/
		for (i=0; i<MAX_MD_NUM; i++)
		{
		    /*�޹����ޱ仯�󣬲���һ���������ڣ������޹�����*/
            if((i>=Qp_I)&&(g_DU_Ram.UpdatePermit_R))
            {
                continue;
            }    
        
        	/* ����g_DU_Para.number_of_periods �����ڵ����ݺ�*/
			stAvDmData[i].LastValue = stAvDmData[i].CurrValue;
			DU_Change_Date_Time((INT8U*)&DateTime, (INT8U*)&stAvDmData[i].CapTime);

            /*2018-03-31 ������������Ҫ���⴦���� ����698������ʼʱ�䣬���Կ�����ʱ���ù�*/
            #if 0
            YMDHMSToDateTimeType((INT8U*)&DateTime, (INT8U*)&datetime);
			DateTimeShiftBack(&datetime,(g_DU_Para.DemandPeriod*(g_DU_Para.NumOfPeriod-1)));  /* ʱ�����һ���������� */
			LIB_MemCpy((INT8U*)&datetime, (INT8U*)&stAvDmData[i].StarTime, sizeof(DateTimeType));
            #else
			//DU_Change_Date_Time((INT8U*)&DateTime, (INT8U*)&stAvDmData[i].StarTime);
            #endif
    
			/*���������*/
			if (stAvDmData[i].LastValue>stMDData.Total.Data[i].Value)
			{
				stMDData.Total.Data[i].Value = stAvDmData[i].LastValue;
				DU_Change_Date_Time((INT8U *)&DateTime, (INT8U*)&stMDData.Total.Data[i].Time);
				//stMDData.Total.Crc= CRC16((INT8U *)&stMDData.Total,FIND(ST_MD_FROM,Crc));	
				saveflag |= 0x01;
			}
            
			/*����ʱ����*/
			if (0==g_DU_Ram.UpdatePermit)
            {     
                if(stAvDmData[i].LastValue>stMDData.CurData.Data[i].Value)
    			{
    				stMDData.CurData.Data[i].Value = stAvDmData[i].LastValue;
    				DU_Change_Date_Time((INT8U*)&DateTime, (INT8U*)&stMDData.CurData.Data[i].Time);
    				//stMDData.CurData.Crc= CRC16((INT8U *)&stMDData.CurData, FIND(ST_MD_FROM,Crc));	
    				saveflag |= 0x02;
    			}
            }
            
			/*�ۼ��������*/			
			if (stAvDmData[i].LastValue>stTotalDm.Value[i])
			{
				stTotalDm.Value[i] =stAvDmData[i].LastValue;
				saveflag |= 0x04;
			}
		}

        Get_Current_Demand();
        
		/*���������*/
		if (saveflag&0x01)
		{
			stMDData.Total.Crc= CRC16((INT8U *)&stMDData.Total, FIND(ST_MD_FROM,Crc));	
            FS_WriteFile(E_FILE_DU_DATA,DU_MD_DATA_ALL_DATA_SUM_SHIFT,(INT8U *)&stMDData.Total,sizeof(stMDData.Total));
		}
        
		/*��ʱ�������*/
		if (saveflag&0x02)
		{
			  //GetSingle(E_TARIFF_NOW, &tariffNo);
		      //if(tariffNo>TARIFF_NUM)	
			  {
				tariffNo = 1;
			  }				
			  stMDData.CurData.Crc= CRC16((INT8U *)&stMDData.CurData, FIND(ST_MD_FROM,Crc));	
              FS_WriteFile(E_FILE_DU_DATA,DU_MD_DATA_ALL_DATA_TRIFF_SHIFT(tariffNo),
                      (INT8U *)&stMDData.CurData,sizeof(stMDData.CurData)); 
		}

		/*��ʷ�������*/
		if (saveflag&0x04)
		{ 
			stTotalDm.Crc= CRC16((INT8U *)&stTotalDm, FIND(ST_TLMD_DATA,Crc));
            FS_WriteFile(E_FILE_DU_DATA,DU_TOTAL_DM_SHIFT,(INT8U *)&stTotalDm,sizeof(ST_TLMD_DATA)); 
		}	 
	}
}

/*****************************************************************************
 �� �� ��  : DU_DefaultInit
 ��������  : ��ʼ����������������
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ���ڶ� 2018 04 10
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void DU_DefaultInit(void)
{
    /*1.������ʼ��*/
    DU_LoadDefaultDUPara();
   
    /*2.���ݳ�ʼ��*/
    FS_DelFile(E_FILE_DU_DATA, DU_MD_DATA_ALL_SHIFT,sizeof(ST_DU_FILE_DATA));    
}

/*****************************************************************************
 �� �� ��  : DU_DataInit
 ��������  : ���ݳ�ʼ��
 �������  : void  
 �������  : ��
 �� �� ֵ  : 
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 04 11
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
void DU_DataInit(void)
{
    INT8U i,ret; 

    /*1.�ۼ��������*/
	for (i=0; i<ERR_RELOADTIMES; i++)
	{
		//ret =EEP_Read(TLMDDATA_ADDR,(INT8U *)&stTotalDm,TLMDDATA_SIZE); 
        ret =FS_ReadFile(E_FILE_DU_DATA,DU_TOTAL_DM_SHIFT,(INT8U *)&stTotalDm,sizeof(ST_TLMD_DATA)); 

		if((ret ==TRUE)&&(stTotalDm.Crc ==CRC16((INT8U *)&stTotalDm, FIND(ST_TLMD_DATA,Crc))))
		{
			break;
		}
	}

    /*2.���������*/
	for (i=0; i<ERR_RELOADTIMES; i++)
	{
		//ret =EEP_Read(TLMDDATA_ADDR,(INT8U *)&stTotalDm,TLMDDATA_SIZE); 
        ret =FS_ReadFile(E_FILE_DU_DATA,DU_MD_DATA_ALL_DATA_SUM_SHIFT,(INT8U *)&stMDData.Total,sizeof(ST_MD_FROM));
		if((ret ==TRUE)&&(stMDData.Total.Crc ==CRC16((INT8U *)&stMDData.Total, FIND(ST_MD_FROM,Crc))))
		{
			break;
		}
	}

    /*��ʱ�ģ�������������ˣ���DU_Clrʵ����*/
    #if 0
    /*3.��ʱ�������*/
    #endif
    
}    
/*********************************************************************************
  Function:    DU_Init
  Description���ϵ�������ʼ���ӿ�
  Calls:
  Called By:
  Input:   //��
  Output:  //��
  Return:  //��
  Others:  //����˵��
*********************************************************************************/
void DU_Init(INT8U mode)
{
    INT8U clr_value = DU_CUR_AVE_DM;

    if(FIRST_PWR_ON == mode)
    {
        DU_DefaultInit();
    }
    else
    {
       DU_ParaCheck();    
       
       DU_DataInit();
    }    
    
	DU_Clr(&clr_value,NULL);
}


/*****************************************************************************
 �� �� ��  : DuDemandMode
 ��������  : ʱ��ͬ������
 �������  : void  
 �������  : ��
 �� �� ֵ  : 0xff:������Ҫ��һ������������0:���ü�һ������
 ���ú���  : 
 ��������  : 
 
 �޸���ʷ      :
  1.��    ��   : ������ 2018 05 23
    ��    ��   : xyt
    �޸�����   : �����ɺ���

*****************************************************************************/
INT8U DuDemandMode(void)
{
    INT8U ModeValue;
    INT8U DemandMode;
    
    ModeValue = g_DU_Ram.last_min%g_DU_Para.DemandPeriod; 

    DemandMode = GetDemandMode();    

    /*ʱ��ͬ����Ҫ��������������*/
    if((0 == ModeValue)&&(0 == DemandMode))
    {       
       g_DU_Ram.SkipTime_Count = g_DU_Para.DemandPeriod*60;  /*ʱ��ͬ��������һ������������һ����������*/
       return 0xff;  
    }    
    else
    {
        return 0;
    }    
}    

/*********************************************************************************
  Function:    DU_Main
  Description���������㺯�������е�ǰ���������������������������������ļ���
  Calls:	   
  Called By:   
  Input:   //��
  Output:  //��
  Return:  //��
  Others:  //ÿ�����
*********************************************************************************/
void DU_Main(void)
{
    ST_7TIME_FORM DateTime;

	if (DU_TRUE == Task_ChkTsk_1s(MAINTASK_DU))
    {
		DU_PowerAdd();/*����ÿS�ۼ�*/
	}
    
	GetSingle(E_SYS_TIME, (INT8U *)&DateTime);

	/*Ӳʱ�ӷ��Ӹ���ʱ�ۼ�*/
	if (g_DU_Ram.last_min ==DateTime.min)
	{
         return;		
	} 
	else
	{	
		g_DU_Ram.last_min =DateTime.min;

        /*�������һ���ӣ�����һ���Ӽ���?�Ա����ݱ���? ��û�б�Ҫ?*/
        #if 1
        if(0 == DU_THROWN)
        {
            if(g_DU_Ram.SkipTime_Count<60)
            {
                g_DU_Ram.SkipTime_Count = 60;
            }    
        } 
        #endif
        
		if (DU_FIRST1MIN)   /*�ϵ�����ֱ�ʶ*/
		{	
			DU_FIRST1MIN=0;           
            
            return;			
		}
	}   
    
	/*������ȷ���Լ�*/
	DU_Para_check();
    
	/*�����л����*/
	//DU_TariffNo_Check();
 
	if ((g_DU_Ram.SkipTime_Count< (INT16U)g_DU_Para.DemandPeriod*60)&&
         (0==DuDemandMode()))
	{ 
        return;     /*���㻬��ʱ�����ʱ��ͬ��ʱ���˳�*/      
	}
	else
	{	   
		UpData_PowerBuf();
		DU_CurAveValue(DU_FALSE);
		g_DU_Ram.SkipTime_Count = 0;
	}

	DU_MaxValue();
    
}

/*"*******************************************************************************"*/


