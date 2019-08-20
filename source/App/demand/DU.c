/*******************************************************************************
*Copyright   : 2017  Wasion Group Ltd.
*FileName    : DU.C
*Description : 需量计量模块
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
st_DU_Ram  g_DU_Ram;    /*需量临时数据*/ 
NO_INIT ST_AVDM_DEF  stAvDmData[MAX_MD_NUM];  /*"平均需量数据"*/

NO_INIT ST_MD_DATA   stMDData;                /*"月最大需量数据 当月 临时的，一直变化的*/

INT32S  Current_Demand[2];				      /*"当前需量存储空间0 有功 1 无功"*/ 			          

NO_INIT ST_TLMD_DATA stTotalDm;               /*"历史累计最大需量数据"*/

ST_DU_PARA g_DU_Para;   /*需量参数*/

/*****************************************************************************
 函 数 名  : DU_LoadDefaultDUPara
 功能描述  : 加载需量模块默认参数
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 04 10
    作    者   : xyt
    修改内容   : 新生成函数

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
 函 数 名  : DU_Change_Date_Time
 功能描述  : 将底层时间转换成存储使用的时间
 输入参数  : INT8U *pSrcDateTime  
             INT8U *pDstDateTime  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期六 2018 03 31
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void DU_Change_Date_Time(INT8U *pSrcDateTime,INT8U *pDstDateTime)
{
    ST_FMT_ATTR inFormat  = {E_YYMMDDHHMMSSWW,E_NONE_UNIT,0};    //0x0031; 
    ST_FMT_ATTR outFormat = {E_YYMMDDHHMM,E_NONE_UNIT,0};  //0x0032;

    FRMT_DataChange(pSrcDateTime,pDstDateTime,inFormat,outFormat);

    //海外用? YMDHMSToDateTimeType(pSrcDateTime,pDstDateTime);
}    



/*****************************************************************************
 函 数 名  : DU_Change_Date_Time
 功能描述  : 将底层时间转换成通信时间或需量使用的时间
 输入参数  : INT8U *pSrcDateTime  
             INT8U *pDstDateTime  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期六 2018 03 31
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void DU_Change_Date_Time_Com(INT8U *pSrcDateTime,INT8U *pDstDateTime)
{
    ST_FMT_ATTR inFormat  = {E_YYMMDDHHMM,E_NONE_UNIT,0};    //0x0031; 
    ST_FMT_ATTR outFormat = {E_YYYYMMDDHHMMSS,E_NONE_UNIT,0};  //0x0032;

    FRMT_DataChange(pSrcDateTime,pDstDateTime,inFormat,outFormat);
    *(pDstDateTime+6) = 0;  /*把格式转换出来的FF固定写成0*/

    //海外用? YMDHMSToDateTimeType(pSrcDateTime,pDstDateTime);
}  

/*****************************************************************************
 函 数 名  : DU_MaxDemandClr
 功能描述  : 最大需量清0
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 04 03
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8S DU_MaxDemandClr(void *pdin, void *pdout)
{
    INT8U clr_value = (DU_TOTALMAX_DM|DU_MAX_DM|DU_CUR_AVE_DM);
    
    DU_Clr(&clr_value,NULL); 
    
    return MS_SUCCESS;
}    


/*****************************************************************************
 函 数 名  : DU_R_DemandClr
 功能描述  : 无功需量清0
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 04 03
    作    者   : xyt
    修改内容   : 新生成函数

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
 函 数 名  : DU_CurrentDemandClr
 功能描述  : 最大需量清0
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 04 03
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8S DU_CurrentDemandClr(void *pdin, void *pdout)
{
    INT8U clr_value = DU_CUR_AVE_DM;
    
    DU_Clr(&clr_value,NULL);
    
    return MS_SUCCESS;
}   


/*****************************************************************************
 函 数 名  : DU_Update_R_com_type
 功能描述  : 更新组合无功特殊值，同步计量模块的组合无功特征字，以便组合无功
             需量计算时用，防止get嵌套
 输入参数  : void *pdin   
             void *pdout  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期五 2018 04 13
    作    者   : xyt
    修改内容   : 新生成函数

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
 函 数 名  : GetTriffDemand
 功能描述  : 获取当前费率的需量
 输入参数  : INT8U tariffNo  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期四 2018 05 10
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void GetTriffDemand(INT8U tariffNo)
{
    INT8U i;
    CRC_DATA_TP crc; 
    
    /*2018-05-05:这里要读出当前费率的值，否则用的是上一次的值，而设置时间后，实际费率可能变了*/
    /*读当前费率的最大需量*/
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
  Description：清除当前需量
  Calls:
  Called By:
  Input:   //flag:DU_CUR_AVE_DM:清当前需量；DU_MAX_DM:最大平均需量；DU_TOTALMAX_DM:清累计最大需量
  Output:  //无
  Return:  //无
  Others:  //其他说明
*********************************************************************************/
INT8S DU_Clr(void *pdin, void *pdout)
{
	INT8U i;
    INT8U tariffNo; 
    ST_7TIME_FORM DateTime;
    INT8U flag;
    
    flag = *(INT8U*)pdin;
    
    GetSingle(E_SYS_TIME, (INT8U *)&DateTime);
    
    /*1.清需量计算相关临时数据*/  /* 即清当前需量 */
	if (flag&DU_CUR_AVE_DM) 
	{
	    Current_Demand[0] = 0;
        Current_Demand[1] = 0;

        /*DU_FIRST_DM也在这里清0了*/    
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

        /*更新下费率*/
        //GetSingle(E_TARIFF_NOW, &tariffNo);	
        //g_DU_Ram.Cur_TariffNo = tariffNo;   /*赋下值，否则会为0，分费率会慢一个需量周期出来*/
        GetTriffDemand(tariffNo);
		
	} 
    
    /*2.清当月最大需量 总及费率需量数据*/
	if(flag&DU_MAX_DM)
	{
		//LIB_MemSet(0, (INT8U*)&stMDData.Total, sizeof(stMDData.Total.Data));
		//LIB_MemSet(0, (INT8U*)&stMDData.CurData, sizeof(stMDData.CurData.Data));
        LIB_MemSet(0, (INT8U*)&stMDData, sizeof(stMDData));

        #if 0  //不应该有，否则时间就有个值，不太好
		for(i=0; i<MAX_MD_NUM; i++)
		{
			DU_Change_Date_Time((INT8U*)&DateTime, (INT8U*)&stMDData.CurData.Data[i].Time);
			DU_Change_Date_Time((INT8U*)&DateTime, (INT8U*)&stMDData.Total.Data[i].Time);
		}
        #endif

        /*算CRC，三个CRC都算一下*/
		stMDData.Total.Crc= CRC16((INT8U *)&stMDData.Total, FIND(ST_MD_FROM,Crc));	
		stMDData.CurData.Crc= CRC16((INT8U *)&stMDData.CurData, FIND(ST_MD_FROM,Crc));	
		stMDData.Crc= CRC16((INT8U *)&stMDData, FIND(ST_MD_DATA,Crc));	
        
		/*总最大需量*/
        FS_WriteFile(E_FILE_DU_DATA,DU_MD_DATA_ALL_DATA_SUM_SHIFT,(INT8U *)&stMDData.Total,sizeof(stMDData.Total));
        
		/*分时最大需量*/
		for(i=1; i<=TARIFF_NUM; i++)
		{
            FS_WriteFile(E_FILE_DU_DATA,DU_MD_DATA_ALL_DATA_TRIFF_SHIFT(i),(INT8U *)&stMDData.CurData,sizeof(stMDData.CurData)); 
		}
	}

    /*3.清历史最大需量*/
    /*历史累计最大需量清除存内卡*/	
	if(flag&DU_TOTALMAX_DM)
	{		
		LIB_MemSet(0, (INT8U*)&stTotalDm, sizeof(stTotalDm));
		stTotalDm.Crc= CRC16((INT8U *)&stTotalDm, FIND(ST_TLMD_DATA,Crc));	
        FS_WriteFile(E_FILE_DU_DATA,DU_TOTAL_DM_SHIFT,(INT8U *)&stTotalDm,sizeof(ST_TLMD_DATA)); 
	}  

    return MS_SUCCESS;
    
}
/*****************************************************************************
 函 数 名  : DU_ParaInit
 功能描述  : 需量参数初始化
 输入参数  : 3  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 04 10
    作    者   : xyt
    修改内容   : 新生成函数

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

    /*"取内卡参数失败时调用缺省参数"*/
    if(i >= READ_DU_PARA_MAX_CNT) 
    {
        DU_LoadDefaultDUPara();
    }
}
/*********************************************************************************
  Function:  DU_Para_check
  Description：需量参数检测，后面统一改成CRC及参数范围都检测
  Calls:
  Called By:
  Input:   //无
  Output:  //无
  Return:  //无
  Others:  //其他说明
*********************************************************************************/
void DU_Para_check(void)
{
    DU_ParaCheck();

	if(g_DU_Para.DemandPeriod > MAX_MD_PERIOD)             /*1.滑差时间*/
	{
		g_DU_Para.DemandPeriod = 1;
	}	
	else if(g_DU_Para.DemandTime > MAX_MD_PERIOD)          /*2.需量周期*/
	{
        g_DU_Para.DemandTime = 15;
	}

    
	if ((0==g_DU_Para.NumOfPeriod)||(g_DU_Para.NumOfPeriod > MAX_SLIP_NUM))  /*滑差数*/
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
  Description：费率号切换检测
  Calls:
  Called By:
  Input:   //无
  Output:  //无
  Return:  //无
  Others:  //其他说明
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
 函 数 名  : DU_Tariff_Switch
 功能描述  : 需量时段切换
 输入参数  : void *pdin   
             void *pdout  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期日 2018 05 20
    作    者   : xyt
    修改内容   : 新生成函数

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
  Description：获得瞬时功率值
  Calls:		
  Called By:
  Input:   index:Ap_P/Ap_N/Qp_P/Qp_N
  Output:  //无
  Return:  INT32，返回需量值
  Others:  //其他说明
*********************************************************************************/
INT32U GetPowerValue(INT8U index)
{
	INT8U  TmpIndex = E_MAC_DEMAND_TYPE;
    INT32S TmpPower = 0;
    
    //const INT16U DEMAND_OBIS[]={E_ACTIVE_PWR,E_REACTIVE_PWR};

    //if(index>=E_MAC_DEMAND_TYPE)
        
    /*有功需量*/
    if(index<=Ap_N)
    {
        GetSingle(E_ACTIVE_PWR,(INT8U *) &TmpPower);  
        if(TmpPower<0)
        {
            TmpIndex = Ap_N;             
            TmpPower = 0- TmpPower;     /*因为反向也要取正值*/
        }  
        else
        {
            TmpIndex = Ap_P;                
        }       
    } 
    else if(index<=Qp_IV) /*无功需量*/
    {
        GetSingle(E_QUAD_TOTAL,&TmpIndex);
                
        /* 正向无功功率， 在1、2象限为正 */
    	if((0==TmpIndex)||(1==TmpIndex)) 
        {
            GetSingle(E_REACTIVE_PWR,(INT8U *) &TmpPower);            
        }
        else
        {
            GetSingle(E_REACTIVE_PWR,(INT8U *) &TmpPower);   
            TmpPower = 0-TmpPower;
        }  
        
        TmpIndex += 2;      /*无功对应索引为2-6，所以+2*/  
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
  Description：功率值累加
  Calls:
  Called By:
  Input:   //无
  Output:  //无
  Return:  //无
  Others:  //其他说明
*********************************************************************************/
void DU_PowerAdd(void)
{
	INT8U i;
	INT32U tmp32u;
    
    /*不足1min的需量丢弃*/
	if (DU_FIRST1MIN)
	{
		return;
	}    
    
	g_DU_Ram.SkipTime_Count++;  
    
	for (i=0; i<MAX_MD_NUM; i++)
	{       
		tmp32u = GetPowerValue(i);
        /*容下错，防止计量出错的情况，测试出现57.7V上30A，象限与功率不匹配的问题*/
        if(MAX_POWER<tmp32u)
        {
           tmp32u = 0; 
        }    
   		g_DU_Ram.Data[i].SkipTime_Power += tmp32u/MULTIPLY_POWER;	
	}
}

/*********************************************************************************
  Function:    DU_CurAveValue
  Description：当前平均值计算接口
  Calls:
  Called By:
  Input:   flag:DU_FALSE,正常计算需量，满滑差时间计算
           flag:DU_TRUE, 实时计算需量，当前滑差时间内当前需量计算
  Output:  //无
  Return:  //无
  Others:  //其他说明
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
 函 数 名  : GetDemandMode
 功能描述  : 获取需量同步方式
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期一 2018 05 07
    作    者   : xyt
    修改内容   : 新生成函数

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
  Description：每个周期时间内的需量累计值平均
  Calls:
  Called By:
  Input:   //无
  Output:  //无
  Return:  //无
  Others:  //其他说明
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
    
	/*周期数判断，大于周期，翻转*/
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
/*"函数名："*/
/*"用途：计算当前需量，根据当前功率方向和象限计算当前的有功无功需量"*/
/*"参数："*/
/*"返回："*/
/*"作者：trend 版本：1.00"*/
/*"主要是根据象限关系，赋值‘正负号’"*/
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

    /*象限变化后，不满需量周期不出无功需量*/
    if(g_DU_Ram.UpdatePermit_R)
    {
        return;
    }    
    
    GetSingle(E_QUAD_TOTAL,&TmpIndex);    

    /* 正向无功功率， 在1、2象限为正 */
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
  Description：最大需量更新
  Calls:
  Called By:
  Input:   //无
  Output:  //无
  Return:  //无
  Others:  //其他说明
*********************************************************************************/
void DU_MaxValue(void)
{
	INT8U  saveflag , i;//crc;
  ST_7TIME_FORM DateTime;
  INT8U tariffNo;
    

    /*通过存储标识，在数据变化时，才存储，避免频繁写EEPROM*/
	saveflag = 0; 
    
	/*达到周期数后需量更新*/
	if (DU_FIRST_DM ==1)
	{
        OC_MultiTermn(E_MULTI_DEMANDPERIOD);/*"多功能端子输出时段切换信号"*/
    
        GetSingle(E_SYS_TIME, (INT8U *)&DateTime);

        /*最大需量比较*/
		for (i=0; i<MAX_MD_NUM; i++)
		{
		    /*无功象限变化后，不满一个需量周期，不出无功需量*/
            if((i>=Qp_I)&&(g_DU_Ram.UpdatePermit_R))
            {
                continue;
            }    
        
        	/* 计算g_DU_Para.number_of_periods 个周期的数据和*/
			stAvDmData[i].LastValue = stAvDmData[i].CurrValue;
			DU_Change_Date_Time((INT8U*)&DateTime, (INT8U*)&stAvDmData[i].CapTime);

            /*2018-03-31 下面两个函数要特殊处理下 ，但698不用起始时间，所以可以暂时不用管*/
            #if 0
            YMDHMSToDateTimeType((INT8U*)&DateTime, (INT8U*)&datetime);
			DateTimeShiftBack(&datetime,(g_DU_Para.DemandPeriod*(g_DU_Para.NumOfPeriod-1)));  /* 时间调后一个需量周期 */
			LIB_MemCpy((INT8U*)&datetime, (INT8U*)&stAvDmData[i].StarTime, sizeof(DateTimeType));
            #else
			//DU_Change_Date_Time((INT8U*)&DateTime, (INT8U*)&stAvDmData[i].StarTime);
            #endif
    
			/*最大总需量*/
			if (stAvDmData[i].LastValue>stMDData.Total.Data[i].Value)
			{
				stMDData.Total.Data[i].Value = stAvDmData[i].LastValue;
				DU_Change_Date_Time((INT8U *)&DateTime, (INT8U*)&stMDData.Total.Data[i].Time);
				//stMDData.Total.Crc= CRC16((INT8U *)&stMDData.Total,FIND(ST_MD_FROM,Crc));	
				saveflag |= 0x01;
			}
            
			/*最大分时需量*/
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
            
			/*累计最大需量*/			
			if (stAvDmData[i].LastValue>stTotalDm.Value[i])
			{
				stTotalDm.Value[i] =stAvDmData[i].LastValue;
				saveflag |= 0x04;
			}
		}

        Get_Current_Demand();
        
		/*总最大需量*/
		if (saveflag&0x01)
		{
			stMDData.Total.Crc= CRC16((INT8U *)&stMDData.Total, FIND(ST_MD_FROM,Crc));	
            FS_WriteFile(E_FILE_DU_DATA,DU_MD_DATA_ALL_DATA_SUM_SHIFT,(INT8U *)&stMDData.Total,sizeof(stMDData.Total));
		}
        
		/*分时最大需量*/
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

		/*历史最大需量*/
		if (saveflag&0x04)
		{ 
			stTotalDm.Crc= CRC16((INT8U *)&stTotalDm, FIND(ST_TLMD_DATA,Crc));
            FS_WriteFile(E_FILE_DU_DATA,DU_TOTAL_DM_SHIFT,(INT8U *)&stTotalDm,sizeof(ST_TLMD_DATA)); 
		}	 
	}
}

/*****************************************************************************
 函 数 名  : DU_DefaultInit
 功能描述  : 初始化需量参数和数据
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期二 2018 04 10
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void DU_DefaultInit(void)
{
    /*1.参数初始化*/
    DU_LoadDefaultDUPara();
   
    /*2.数据初始化*/
    FS_DelFile(E_FILE_DU_DATA, DU_MD_DATA_ALL_SHIFT,sizeof(ST_DU_FILE_DATA));    
}

/*****************************************************************************
 函 数 名  : DU_DataInit
 功能描述  : 数据初始化
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 04 11
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
void DU_DataInit(void)
{
    INT8U i,ret; 

    /*1.累计最大需量*/
	for (i=0; i<ERR_RELOADTIMES; i++)
	{
		//ret =EEP_Read(TLMDDATA_ADDR,(INT8U *)&stTotalDm,TLMDDATA_SIZE); 
        ret =FS_ReadFile(E_FILE_DU_DATA,DU_TOTAL_DM_SHIFT,(INT8U *)&stTotalDm,sizeof(ST_TLMD_DATA)); 

		if((ret ==TRUE)&&(stTotalDm.Crc ==CRC16((INT8U *)&stTotalDm, FIND(ST_TLMD_DATA,Crc))))
		{
			break;
		}
	}

    /*2.总最大需量*/
	for (i=0; i<ERR_RELOADTIMES; i++)
	{
		//ret =EEP_Read(TLMDDATA_ADDR,(INT8U *)&stTotalDm,TLMDDATA_SIZE); 
        ret =FS_ReadFile(E_FILE_DU_DATA,DU_MD_DATA_ALL_DATA_SUM_SHIFT,(INT8U *)&stMDData.Total,sizeof(ST_MD_FROM));
		if((ret ==TRUE)&&(stMDData.Total.Crc ==CRC16((INT8U *)&stMDData.Total, FIND(ST_MD_FROM,Crc))))
		{
			break;
		}
	}

    /*分时的，不用在这里读了，在DU_Clr实现了*/
    #if 0
    /*3.分时最大需量*/
    #endif
    
}    
/*********************************************************************************
  Function:    DU_Init
  Description：上电需量初始化接口
  Calls:
  Called By:
  Input:   //无
  Output:  //无
  Return:  //无
  Others:  //其他说明
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
 函 数 名  : DuDemandMode
 功能描述  : 时钟同步处理
 输入参数  : void  
 输出参数  : 无
 返 回 值  : 0xff:整除，要计一个滑差需量，0:不用计一个需量
 调用函数  : 
 被调函数  : 
 
 修改历史      :
  1.日    期   : 星期三 2018 05 23
    作    者   : xyt
    修改内容   : 新生成函数

*****************************************************************************/
INT8U DuDemandMode(void)
{
    INT8U ModeValue;
    INT8U DemandMode;
    
    ModeValue = g_DU_Ram.last_min%g_DU_Para.DemandPeriod; 

    DemandMode = GetDemandMode();    

    /*时钟同步，要整除后除最大需量*/
    if((0 == ModeValue)&&(0 == DemandMode))
    {       
       g_DU_Ram.SkipTime_Count = g_DU_Para.DemandPeriod*60;  /*时钟同步，不满一个滑差间隔，按一个滑差间隔算*/
       return 0xff;  
    }    
    else
    {
        return 0;
    }    
}    

/*********************************************************************************
  Function:    DU_Main
  Description：需量计算函数，进行当前需量、月最大需量、整个流程最大需量的计算
  Calls:	   
  Called By:   
  Input:   //无
  Output:  //无
  Return:  //无
  Others:  //每秒调用
*********************************************************************************/
void DU_Main(void)
{
    ST_7TIME_FORM DateTime;

	if (DU_TRUE == Task_ChkTsk_1s(MAINTASK_DU))
    {
		DU_PowerAdd();/*功率每S累计*/
	}
    
	GetSingle(E_SYS_TIME, (INT8U *)&DateTime);

	/*硬时钟分钟更新时累计*/
	if (g_DU_Ram.last_min ==DateTime.min)
	{
         return;		
	} 
	else
	{	
		g_DU_Ram.last_min =DateTime.min;

        /*如果不足一分钟，则以一分钟计算?以便数据变大点? 有没有必要?*/
        #if 1
        if(0 == DU_THROWN)
        {
            if(g_DU_Ram.SkipTime_Count<60)
            {
                g_DU_Ram.SkipTime_Count = 60;
            }    
        } 
        #endif
        
		if (DU_FIRST1MIN)   /*上电非整分标识*/
		{	
			DU_FIRST1MIN=0;           
            
            return;			
		}
	}   
    
	/*参数正确性自检*/
	DU_Para_check();
    
	/*费率切换检测*/
	//DU_TariffNo_Check();
 
	if ((g_DU_Ram.SkipTime_Count< (INT16U)g_DU_Para.DemandPeriod*60)&&
         (0==DuDemandMode()))
	{ 
        return;     /*不足滑差时间或不是时钟同步时间退出*/      
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


