#include "system\Sys_def.h"
#include "APP\LegallyRelevant\MU_LIB.h"
#include "APP\LegallyRelevant\LRM.h"
#include "Driver\DriverComm.h"
//#include "commonheader.h"

/*"***********************************************************
FileName: MU_LIB.c
Author: 韩潇俊
Version : 1.0       Date: 2017.11.13
Description:计量运行模块,
用于计量的初始化，运行，瞬时量数据读取，安全判断以及电量传递
		   用于RN8209C计量芯片
Function List:
<FunctionName>
<author> <time> <version > <desc>

***********************************************************"*/

/*"--------RN8209 函数声明--------"*/
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

NO_INIT ST_MU_PARA  gMU_Para;/*""计量模块运行参数，只能通过校表修改"*/
ST_MU_INTERDATA  	gMU_Interdata;/*""计量模块内部瞬时量数据"*/
ST_MMT_DATA  		stMmtData;/*""计量模块输出瞬时量数据"*/
ST_MU_FLAG     	    gMU_Flag;/*"计量模块状态标志位"*/
#if EDT_MODE
ST_EDT_ERRDATA      gEDT_Errdata;/*"在线监测数据"*/
#endif
#if HARMONIC_MODE
sHarmonicData_TypeDef HarmonicData;
#endif

INT8U Cal_Status_flag=CAL_STATUS_STOP;/*"校表状态"*/
INT8U Output_Group=0;
INT8U mu_ok;


#if AMEND_MODE
INT16U    MT_K = 0;
INT16U 	RegScale_GP_Now;/*"当前功率比差寄存器(影响量修调时使用)"*/
INT32U 	MT_Power[5];							  	/*""功率算平均值"*/
INT8U 	mT_Start_Flag;							/*""0.5mT试验开始检测标志"*/
const INT16U	U_Data[8] = {1740,1780,1960,2000,2400,2440,2500,2550};
INT16U 	Imax_Data[6] = {600,1800,1800,3000,3000,4200};
static INT8U MT_Timer1;
AMEND_OUTDATA       Amend_out_data;
ST_AMEND_TIMER      gAmendtimer;/*""调校功能用"*/
#endif

/*"************************************************
Function:CheckSum
Description: 16位校验和函数
Input:  *data 数据首地址length 长度
Output: 	16位校验和
Return:无
Others:
************************************************"*/
/*"--------计算16位校验和函数--------"*/
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

/*"--------RN2023 初始化--------"*/
/*"************************************************
Function:MU_Init
Description: RN2023初始化函数
Input:  无
Output: 	无
Return:无
Others:
************************************************"*/
void MU_Init(INT8U mode)
{
    mu_ok = 0;
   	MU_Ramdata_Init();
   	MU_Chip_Init();
    MU_ReadInstantData(R_ALLP);/*"打包读参数"*/
}

/*"************************************************
Function:MU_Ramdata_Init
Description: RN8209固定参数初始化
Input:  无
Output: 	无
Return:无
Others:
************************************************"*/
void MU_Ramdata_Init(void)
{
	/*"					gMU_Flag					"*/
  	LIB_MemSet(0, (INT8U *)&gMU_Flag, sizeof(ST_MU_FLAG));/*""清计量芯片状态标志位"*/
	gMU_Flag.MeasureChip_DelayRead_Flag = NOT_NEED_DELAY;
  	gMU_Flag.MeasureChip_Initialize_Flag = NOT_NEED_INIT;
  	gMU_Flag.MeasureChip_Read_Error = NO_ERROR;
  	gMU_Flag.MeasureChip_Reset_Flag = NOT_NEED_RESET;

	/*"					stMmtData					"*/
  	LIB_MemSet(0, (INT8U *)&stMmtData, sizeof(ST_MMT_DATA));     /*"清除输出瞬时量"*/
	/*"					gMU_Interdata				"*/
	LIB_MemSet(0, (INT8U *)&gMU_Interdata, sizeof(ST_MU_INTERDATA));/*""清瞬时量"*/
	gMU_Interdata.FactorA=10000;
    gMU_Interdata.FactorA=10000;

  	Cal_Status_flag=CAL_STATUS_STOP;
    Output_Group = 0;


#if AMEND_MODE
  	/*"					修调参数				"*/
    if((gAmendtimer.Amend_Complete!=0)&&((gMU_Para.ram_adj_data.Ram_hconfig2 & 0x0001) == 1))/*"防止掉电前在做自热"*/
    {
    	//DC_GetData(E2_MEA_PARA1,(INT8U *)&gMU_Para,0,sizeof(ST_MU_PARA));
        FS_ReadFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT,(INT8U *)&gMU_Para,sizeof(ST_MU_PARA));
    }
    LIB_MemSet(0, (INT8U *)&gAmendtimer, sizeof(ST_AMEND_TIMER));/*"清修调的变量"*/
    LIB_MemSet(0, (INT8U *)&MT_Power, 20);		        			/*"清修调的变量"*/
    RegScale_GP_Now = gMU_Para.ram_adj_data.Ram_gp;
    MT_K = 0;
    //DC_GetData(E2_MT_START_FLG,(INT8U *)&mT_Start_Flag,0,1);/*"内卡读0.5mT标志"*/
    FS_ReadFile(E_FILE_LRM,FIND(ST_LRM_FILE_DATA,MtStartFlg),(INT8U *)&mT_Start_Flag,1);
    MT_Timer1 = 0;
#endif
}

/*"************************************************
Function:MU_Chip_Init
Description: 运行参数和芯片初始化
Input:  无
Output: 	无
Return:无
Others:
************************************************"*/
void MU_Chip_Init(void)
{
   	MU_Measure_Init();
}

/*"************************************************
Function:MU_Measure_Init
Description: 芯片初始化函数
Input:  无
Output: 	无
Return:无
Others:
************************************************"*/
void MU_Measure_Init(void)
{
    INT8U cnt;
    INT16U data=0;

    for(cnt=0;cnt<3;cnt++)
	{
	    /*"复位计量芯片"*/
	    MC_Reset();
		MU_IC_Init(PWRMODE_UP);

        if(MU_IC_WriteReg(W_SHAKE,(INT8U *)&data,2) != TRUE)/*"握手失败"*/
	    {
			MCU_DelayMs(1);
        }
		else
		{
            break;/*"成功则跳出"*/
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
Description: 调用默认参数函数
Input:  无
Output: 	无
Return:无
Others:
************************************************"*/
void MU_Defult_Para(void)
{

}

/*"************************************************
Function:MU_OpenPulse
Description: 计量模块脉冲打开函数
Input:  无
Output: 	无
Return:无
Others:
************************************************"*/
void MU_OpenPulse(void)
{

}


/*"--------RN2023 每秒执行--------"*/
/*"************************************************
Function:MU_CheckMea
Description:定时逻辑判断函数
Input:  无
Output: 无
Return:TRUE  成功  FALSE  失败
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

    if(gMU_Flag.Read_EffectValue_Timer<60)/*"连续60秒钟内如果有超过15次读错误，则复位计量芯片"*/
    {
        gMU_Flag.Read_EffectValue_Timer++;
    }
    else
    {
        if(gMU_Flag.MeasureChip_Read_IntervalError_Record>=15)/*"计量芯片读间歇错误记录计数器"*/
        {
            gMU_Flag.MeasureChip_Initialize_Flag = NEED_INIT;/*"在下一个循环中对计量芯片进行复位、初始化操作；"*/
        }
        gMU_Flag.Read_EffectValue_Timer = 0;
        gMU_Flag.MeasureChip_Read_IntervalError_Record = 0;
    }

	if(gMU_Flag.MeasureChip_Initialize_Flag==NEED_INIT)/*"如果该标志字等于0xAB表示初始化不成功，立即复位计量芯片，启动初始化"*/
	{
		MU_Chip_Init();
        if(gMU_Flag.MeasureChip_Initialize_Flag==NEED_INIT)/*"如果重新初始化后还是不成功"*/
    	{
    	  	gMU_Flag.MeasureChip_Initialize_Error_Record++;/*"计量芯片初始化错误记录计数器加1"*/
        	if(gMU_Flag.MeasureChip_Initialize_Error_Record>=6)
        	{
        		LIB_MemSet(0, (INT8U *)&gMU_Interdata, sizeof(ST_MU_INTERDATA));/*"连续六次复位不成功则把瞬时量置0"*/
                gMU_Interdata.FactorA=10000;
	       	    gMU_Flag.MeasureChip_DelayRead_Flag= NEED_DELAY;/*"计量芯片延时读标志字"*/
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
Description: MU 计量功能运行函数
Input:  无
Output: 	无
Return:无
Others:
************************************************"*/
void MU_Main(void)
{
/*"///////////////////////////////////"*/
/*"////////////每1s//////////////////"*/
/*"//////////////////////////////////"*/
    if(MU_CheckMea() == TRUE)/*"逻辑判断为真，执行读操作"*/
	{
        MU_ReadData();/*"1s执行"*/
        Transmit_To_DC();
		if(1==nilm_spi)		/*"如果对nilm的spi需要改变，向计量芯片发送命令"*/
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
Description: 电量处理模块从计量读取一段时间内累加的电量值，
			正常运行时每秒调用读取，停电前调用读取。
			读取操作电量数据会直接传递到数据中心，并读后清零。
Input:  输入32位数据的缓存区指针，用于传递电能量单位0.0001WS
Output: 无
Return:
Others:
************************************************"*/
INT8U MU_EngyGet(INT16U EngyId, INT32U *pData)
{
    INT8U Ret=FALSE;
	INT16U PluseCounter,PluseCounter1;					/*"脉冲数"*/
	INT32U wSPerPluse;								    /*"计算的脉冲能量单位"*/
	INT32U Engtemp;

    PluseCounter=0;
    PluseCounter1=0;
    wSPerPluse=0;
    Engtemp=0;

    if(EngyId == L_ENGY_P)
    {
    	PluseCounter = MU_IC_GetPulse_Val();					/*"取脉冲数"*/
    	PluseCounter1 = MU_IC_GetNoAffrimPulse_Val();			/*"取真实脉冲数"*/
    }
    else if(EngyId == L_ENGY_Q)
    {
    	PluseCounter = MU_IC_Get_Q_Pulse_Val();					/*"取脉冲数"*/
    	PluseCounter1 = MU_IC_Get_Q_NoAffrimPulse_Val();	    /*"取真实脉冲数"*/
    }


	if((PluseCounter1>=150)||(PluseCounter>=150))		/*"防飞走"*/
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
Description: MU定时读数据函数
Input:  无
Output: 	无
Return:无
Others:
************************************************"*/
void MU_ReadData(void)
{
    #if STATE_MODE

        #if NORMAL_READ_MODE
	    MU_ReadInstantData(R_URMS);/*"读电压"*/
	    MU_ReadInstantData(R_IARMS);/*"读火线电流"*/
	    MU_ReadInstantData(R_PWRPA);/*"读火线功率"*/
	    if( Output_Group == 0)
    	{
    	    	MU_ReadInstantData(R_FCTRA);/*"读功率因素"*/
    	    	MU_ReadInstantData(R_UFREQ);/*"读频率"*/
    	    	Output_Group = 1;
    	}
	    else
    	{
    		MU_ReadInstantData(R_IBRMS);/*"读零线电流"*/
    		Output_Group = 0;
    	}
        #endif

        #if FAST_READ_MODE
            MU_ReadInstantData(R_ALLD);/*"打包读瞬时数据"*/
            #if EDT_MODE
            MU_ReadInstantData(R_ALLE);/*"打包读在线监测误差"*/
            #endif
            #if OUTDATA_MODE
            //MU_ReadInstantData(R_ALLO);/*"打包读负荷识别参数"*/
            #endif
            #if HARMONIC_MODE
            MU_ReadInstantData(R_ALLH_LITE);/*"打包读谐波数据"*/
            //MU_ReadInstantData(R_ALLH);/*"打包读谐波数据"*/
            #endif
        #endif

    #endif

    #if UNSTATE_MODE

    #endif
 }
/*"************************************************
Function:Transmit_To_DC
Description: 传输数据至数据中心函数
Input:  无
Output: 	无
Return:无
Others:
************************************************"*/
void Transmit_To_DC(void)
{
    INT8U i;

    if((gMU_Interdata.DrA == NEGA_STATUS)&&(gMU_Interdata.PowerPA !=0))/*""负的,对外提供瞬时量为无符号数，带方向位"*/
	{
		stMmtData.actPwr[1] = ~gMU_Interdata.PowerPA+1;
	 	stMmtData.curr[0] = ~gMU_Interdata.CurrentA+1;
	 	stMmtData.factor[1] = ~gMU_Interdata.FactorA+1;
        stMmtData.PowerDir = 0x80;/*"反向"*/
	}
    else
    {
       	stMmtData.actPwr[1] = gMU_Interdata.PowerPA;
	 	stMmtData.curr[0] = gMU_Interdata.CurrentA;
	 	stMmtData.factor[1] = gMU_Interdata.FactorA;
        stMmtData.PowerDir = 0x00;/*"正向"*/
    }

    if((gMU_Interdata.DrqA == NEGA_STATUS)&&(gMU_Interdata.PowerQA !=0))/*""负的,对外提供瞬时量为无符号数，带方向位"*/
	{
		stMmtData.reactPwr[1] = ~gMU_Interdata.PowerQA+1;
	}
    else
    {
        stMmtData.reactPwr[1] = gMU_Interdata.PowerQA;
    }
    
	stMmtData.calNcurr = gMU_Interdata.CurrentB;
	stMmtData.volt[0] = gMU_Interdata.Voltage/1000;/*""电压只需要一位小数"*/
	stMmtData.freq = gMU_Interdata.Frequency;
	/*"单相表A相功率、功率因数数据同总数据"*/
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
/*"///////////////读取计量芯片寄存器数据////////"*/
/*"/////////////////////////////////////////////"*/

/*"************************************************
Function:MU_ReadCmp
Description: 数据回读与比较函数
Input:  reg 寄存器地址length 数据长度(数据长度要小于等于4)
Output: 	 无
Return:TRUE  成功  FALSE  失败
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

    if(reg == R_ALLD)/*" 打包读瞬时数据 "*/
    {
        if(MU_ReadCmp(reg,72) == TRUE)/*"只要有一次正确就处理"*/
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
        if(MU_ReadCmp(R_ALLE_STA,4) == TRUE)/*"读取在线监测数据状态"*/
        {
            LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[4], (INT8U *)&TempData,4);
            if(TempData == 1)/*"在线监测数据已更新"*/
            {
                if(MU_ReadCmp(reg,72) == TRUE)/*"只要有一次正确就处理"*/
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
        if(MU_ReadCmp(reg,34) == TRUE)/*"只要有一次正确就处理"*/
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
        if(MU_ReadCmp(reg,4) == TRUE)/*"只要有一次正确就处理"*/
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
        if(MU_ReadCmp(reg,12) == TRUE)/*"只要有一次正确就处理"*/
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
        if(MU_ReadCmp(reg,60) == TRUE)/*"只要有一次正确就处理"*/
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
    else/*" 分项读取瞬时数据 "*/
    {

        if(reg == R_URMS)/*" 读电压 "*/
        {
            LastOneData = gMU_Interdata.Voltage;
            Dstp = (INT8U *)&gMU_Interdata.Voltage;
        }
        else if(reg == R_IARMS)/*" 读火线电流 "*/
        {
            LastOneData = gMU_Interdata.CurrentA;
            Dstp = (INT8U *)&gMU_Interdata.CurrentA;
        }
        else if(reg == R_PWRPA)/*" 读火线有功功率 "*/
        {
            LastOneData = gMU_Interdata.PowerPA;
            Dstp = (INT8U *)&gMU_Interdata.PowerPA;
        }
        else if(reg == R_PWRQA)/*" 读火线无功功率 "*/
        {
            LastOneData = gMU_Interdata.PowerQA;
            Dstp = (INT8U *)&gMU_Interdata.PowerQA;
        }
        else if(reg == R_PWRSA)/*" 读火线视在功率 "*/
        {
            LastOneData = gMU_Interdata.PowerSA;
            Dstp = (INT8U *)&gMU_Interdata.PowerSA;
        }
        else if(reg == R_FCTRA)/*" 读火线功率因素 "*/
        {
            LastOneData = gMU_Interdata.FactorA;
            Dstp = (INT8U *)&gMU_Interdata.FactorA;
        }


        if(reg == R_UFREQ)/*" 读频率 "*/
        {
            LastOneData = gMU_Interdata.Frequency;
            Dstp = (INT8U *)&gMU_Interdata.Frequency;
        }
        else if(reg == R_IBRMS)/*" 读零线电流 "*/
        {
           LastOneData = gMU_Interdata.CurrentB;
           Dstp = (INT8U *)&gMU_Interdata.CurrentB;
        }
        else if(reg == R_PWRPB)/*" 读零线有功功率 "*/
        {
            LastOneData = gMU_Interdata.PowerPB;
            Dstp = (INT8U *)&gMU_Interdata.PowerPB;
        }
        else if(reg == R_PWRQB)/*" 读零线无功功率 "*/
        {
            LastOneData = gMU_Interdata.PowerQB;
            Dstp = (INT8U *)&gMU_Interdata.PowerQB;
        }
        else if(reg == R_PWRSB)/*" 读零线视在功率 "*/
        {
            LastOneData = gMU_Interdata.PowerSB;
            Dstp = (INT8U *)&gMU_Interdata.PowerSB;
        }
        else if(reg == R_FCTRB)/*" 读零线功率因素 "*/
        {
            LastOneData = gMU_Interdata.FactorB;
            Dstp = (INT8U *)&gMU_Interdata.FactorB;
        }

      	if(MU_ReadCmp(reg,4) == TRUE)/*"只要有一次正确就处理"*/
      	{
    		LIB_MemCpy((INT8U *)&gMeaIc_Uart.Uart_Rx_Data[4], (INT8U *)&TempData,4);
     	 }
      	else
      	{
      	    gMU_Flag.MeasureChip_Read_IntervalError_Record++;
    		TempData = LastOneData;
      	}

      	LIB_MemCpy((INT8U  *)&TempData,Dstp,4);/*"将读到电压值存入结构体"*/
    }
}


/*"--------校 表 函 数--------"*/
/*"************************************************
Function:MU_JustMtr
Description:计量校表协议解析及操作函数
Input:  pData 校表命令帧数据指针
Output: 	无
Return:TRUE  成功  FALSE  失败
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
	 	RetVal = COMM_ERR_PASSWORD;/*" 在出厂状态下只能读计量及RTC数据"*/
	}
	else
	{
        switch (CMD)
		{
            case _MU_W_PARA:/*"参数设置 发送数据长度4个字节"*/
            {
                RetVal = MU_CaliCmd1(pData);
                DataLength = 0;
            }
            break;

            case _MU_W_CMD:/*"参数设置 发送数据长度4个字节"*/
            {
                RetVal = MU_CaliCmd2(pData);
                DataLength = 0;
            }
            break;

            case _MU_R_PARA:/*"参数读取 返回数据长度4个字节"*/
            {
                RetVal = MU_CaliReadPara(pData);
                DataLength = 4;
            }
            break;

            case _MU_R_DATA:/*"瞬时数据读取,返回数据长度4个字节"*/
            {
                RetVal = MU_CaliReadData(pData);
                DataLength = 4;
            }
            break;

            case _MU_R_REG:/*"寄存器数据读取,返回数据长度4个字节"*/
            {
                RetVal = MU_CaliReadOther(pData,&DataLength);
                DataLength = 4;
            }
            break;

            case _MU_W_AMEND:/*"写调校参数"*/
            {
                #if AMEND_MODE
                RetVal = Set_Amend(pData);
                DataLength = 0;
                #endif
            }
            break;

            case _MU_R_AMEND:/*"读调校参数"*/
            {
                #if AMEND_MODE
                RetVal = Read_Amend(pData);
                DataLength = 4;
                LIB_Reverse(&pData[3],(INT8U*)&temp, 4);
                LIB_MemCpy((INT8U*)&temp,&pData[3],4);
                #endif
            }
            break;

            case _RU_W_PARA:   					/*""   通过645协议写取RTC补偿参数"*/
            {
                RetVal = RTC_CaliWrite(pData);
                DataLength = 0;
            }
            break;

            case _RU_R_PARA:					/*"" 	通过645协议读取RTC补偿参数 "*/
            {
                RetVal = RTC_CaliRead(pData);
                DataLength = 4;
            }
            break;

            case _RU_W_CMD:					/*"" 	通过645协议修改、校正RTC补偿参数 "*/
            {
                RetVal = RTC_CaliCtrl(pData);
                DataLength = 0;
            }
            break;

            default:
            break;
        }
        pData[2] = DataLength;/*"" 返回数据长度"*/
	}
    return RetVal;
}

/*"************************************************
Function:MU_CaliCmd1
Description:计量参数校正函数
Input:  pData 校表命令帧数据指针
Output: 	无
Return:错误应答状态
Others:
************************************************"*/
INT8U  MU_CaliCmd1(INT8U *pData)
{
	INT8U result=COMM_ERR_NOERR;

	if(Cal_Status_flag == CAL_STATUS_START)/*"发校表开始命令后才能修改"*/
	{
		switch(pData[4])
		{
            case 0x80:
                result = MU_CaliReg(pData,W_VCOEF);/*"电压系数"*/
            break;

            case 0x81:
                result = MU_CaliReg(pData,W_IACOEF);/*"电流系数"*/
            break;

            case 0x82:
                result = MU_CaliReg(pData,W_PACOEF);/*"功率系数"*/
            break;

            case 0x83:
                result = MU_CaliReg(pData,W_GPQA);/*"功率增益"*/
            break;

            case 0x84:
                result = MU_CaliReg(pData,W_APOSA);/*"功率偏置"*/
            break;

            case 0x85:
                result = MU_CaliReg(pData,W_PHSA);/*"相位补偿"*/
            break;

            case 0x86:

            break;

            case 0x87:

            break;

            case 0x88:
            break;

            case 0x89:
                result = MU_CaliReg(pData,W_HFCNST);/*"脉冲常数"*/
            break;

            case 0x8A:

            break;

            case 0x8B:
                result = MU_CaliReg(pData,W_PSTRT);/*"潜动阀值"*/
            break;

            case 0x8C:
                result = MU_CaliReg(pData,W_MTRCFG);/*"计量配置字"*/
            break;

            case 0x8D:
                result = MU_CaliReg(pData,W_MTRCFG2);/*"计量配置字2"*/
            break;

            case 0x8E:
                result = MU_CaliReg(pData,W_ECCNST);/*"设置电表脉冲常数"*/
            break;

            case 0x8F:
                result = MU_CaliReg(pData,W_ALLP);/*"配置所有参数"*/
            break;

            case 0x91:
                result = MU_CaliReg(pData,W_IBCOEF);/*"零线电流系数"*/
            break;

            case 0x92:
                result = MU_CaliReg(pData,W_PBCOEF);/*"零线功率系数"*/
            break;

            case 0x93:
                result = MU_CaliReg(pData,W_GPQB);/*"零线功率增益	"*/
            break;

            case 0x94:
                result = MU_CaliReg(pData,W_APOSB);/*"零线功率偏置	"*/
            break;

            case 0x95:
                result = MU_CaliReg(pData,W_PHSB);/*"零线相位补偿	"*/
            break;

            case 0x96:

            break;

            case 0xA0:
                //MU_CaliChip(pData);
            break;

			case _05_W_I_AMPSTD:
				result = MU_CaliRegU32(pData,W_I_AMPSTD); /* "在线监测电流回路幅值基准" */
				break;
			case _05_W_I_PHSSTD:
				result = MU_CaliRegU32(pData,W_I_PHSSTD); /* "在线监测电流回路相位基准" */
				break;
			case _05_W_U400_AMPSTD:
				result = MU_CaliRegU32(pData,W_U400_AMPSTD);/* "在线监测电压回路幅值基准U400" */
				break;
			case _05_W_U400_PHSSTD:
				result = MU_CaliRegU32(pData,W_U400_PHSSTD);/* "在线监测电压回路相位基准U400" */
				break;
			case _05_W_U10_AMPSTD:
				result = MU_CaliRegU32(pData,W_U10_AMPSTD);/* "在线监测电压回路幅值基准U10" */
				break;
			case _05_W_U10_PHSSTD:
				result = MU_CaliRegU32(pData,W_U10_PHSSTD);/* "在线监测电压回路相位基准U10" */
				break;
            case _05_W_U10_AMPSTD2:
				result = MU_CaliRegU32(pData,W_U10_AMPSTD2);/* "在线监测电压回路幅值基准U10" */
				break;
			case _05_W_U10_PHSSTD2:
				result = MU_CaliRegU32(pData,W_U10_PHSSTD2);/* "在线监测电压回路相位基准U10" */
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
Description:计量参数控制函数
Input:  pData 校表命令帧数据指针
Output: 	无
Return:错误应答状态
Others:
************************************************"*/
INT8U  MU_CaliCmd2(INT8U *pData)
{
  	INT8U result=COMM_ERR_NOERR;
	switch(pData[4])
	{
        case 0xfa:
            result = MU_CaliCtrl(pData);/*"计量控制"*/
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
Description:一键校准函数
Input:  pData 校表命令帧数据指针
Output: 	无
Return:错误应答状态
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
Output: 	无
Return:无
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
Output: 无
Return: 布尔
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
Description:标表控制函数
Input:  pData 校表命令帧数据指针
Output: 	无
Return:无
Others:
************************************************"*/
INT8U MU_CaliCtrl(INT8U *pData)
{
    INT8U result=COMM_ERR_NOERR;
    switch(pData[8])
    {
        case 1 :
        result = MU_CaliCtrlCMD(pData,W_START);/*"开始"*/
        //mT_Start_Flag = 0;
        //DC_OutputData(E2_MT_START_FLG,&mT_Start_Flag,0,1);/*"存储0.5mT标志"*/
        break;
        case 2:
        if(Cal_Status_flag == CAL_STATUS_START)/*"处于校表状态才能发默认参数"*/
        {
           result = MU_CaliCtrlCMD(pData,W_RESET);/*"使用默认参数"*/
        }
        break;
        case 3 :
        #if AMEND_MODE
        if(gAmendtimer.Amend_Resume==0)/*"不能在修调区间内保存参数，以免改动基本误差"*/
        {
            result = MU_CaliCtrlCMD(pData,W_SAVE);/*"参数保存"*/
        }
        else
        {
            result = COMM_ERR_OTHERS;
        }
        #else
        result = MU_CaliCtrlCMD(pData,W_SAVE);/*"参数保存"*/
        #endif
        break;
        case 4 :
            result = MU_CaliCtrlCMD(pData,W_STOP);/*"结束"*/
        break;
		 
        default:
            result= COMM_ERR_NODATA;
        break;
    }
    return result;
}

/*"************************************************
Function:MU_CaliCtrlCMD
Description:校表控制命令函数
Input:  pData 校表命令帧数据指针
Output: 	无
Return:无
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
Description:写打包参数函数
Input:  pData 校表命令帧数据指针
Output: 	无
Return:无
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
Description:读计量数数据函数
Input:  pData 校表命令帧数据指针
Output: 	无
Return:TRUE  成功  FALSE  失败
Others:
************************************************"*/
INT8U MU_CaliReadData(INT8U *pData)
{
	INT8U result=COMM_ERR_NOERR;
	INT32U temp,temp1;

	switch(pData[4])
	{
        case 0x00:/*"电压"*/
            temp=gMU_Interdata.Voltage/100;
        break;

        case 0x01:/*"电流"*/
        	temp=gMU_Interdata.CurrentA/10;
        break;

        case 0x02:/*"功率"*/
        	temp=gMU_Interdata.PowerPA;
        break;

        //case 0x03:/*"视在功率"*/
        //	result= COMM_ERR_NODATA;
        //break;

        //case 0x04:/*"功率因素"*/
        	//	temp=gMU_Interdata.FactorA;
        //break;

        //case 0x05:/*"电压频率"*/
        	//	temp=gMU_Interdata.Frequency;
        //break;

        case 0x06:/*"零线电流"*/
        	temp=gMU_Interdata.CurrentB/10;
        break;

        case 0x07:/*"零线功率"*/
        	temp=gMU_Interdata.PowerPB;
        break;

        case 0x0A:  /*"读剩余电流，4位小数"*/
            temp = gMU_Interdata.CurrentB;
        break;
            
        //case 0x08:/*"零线视在功率"*/
        //	result= COMM_ERR_NODATA;
        //break;

        //case 0x09:/*"0.5 mT 读实际功率"*/
        //result= COMM_ERR_NODATA;
        //break;

		#if EDT_MODE
		/* "在线监测计算值" */
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
		/* "在线监测误差值, 带符号" */
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

		/* "读计量芯状态字" */
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
        {  /*"负数转BCD码"*/
			/*" 高位置8 "*/
			temp = (INT32U)((-(INT32S)temp) + ADD_IF_SIGNED_VALUE);
		}
	}
	#endif /*"EDT_MODE"*/
    
	if((pData[4] >= _05_R_READ_ERR_RECORD) && (pData[4] <= _05_R_PULSE_FLAG)) {
		temp1 = temp;	/*" 读芯片状态不转BCD "*/
	}
	else {
		LIB_LongToBcd((INT8U *)&temp, (INT8U *)&temp1);
	}

	LIB_Reverse((INT8U*)&temp1, (INT8U*)&pData[3], 4);
	return result;
}

/*"************************************************
Function:MU_ReadOnePara
Description:读单个校表参数
Input:  tmp 存参数地址 mu_reg 16位命令码 参数长度 reg_len
Output: 	无
Return: COMM_ERR_NOERR 成功 COMM_ERR_NODATA 失败
Others: 目前读指令长度都是4字节
************************************************"*/
static INT8U MU_ReadOnePara(INT8U *tmp, INT16U mu_reg, INT8U reg_len)
{
	if (MU_IC_ReadReg(mu_reg, reg_len) == TRUE) 
    {
		/*" 取参数域，小端 "*/
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
Description:读校表参数函数
Input:  pData 校表命令帧数据指针
Output: 	无
Return:TRUE  成功  FALSE  失败
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
        case 0x00:/*"电压换算系数"*/
			result = MU_ReadOnePara((INT8U*)(&temp), R_VCOEF, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_V_Coefficient = (INT16U)temp;
			}
        break;
        case 0x01:/*"电流换算系数"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_IACOEF, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_Ia_Coefficient = (INT16U)temp;
			}
        break;
        case 0x02:/*"功率换算系数"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_PACOEF, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_Pa_Coefficient = (INT16U)temp;
			}
        break;
        case 0x03:/*"功率增益"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_GPQA, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_GPQA = (INT16U)temp;
			}
        break;
        case 0x04:/*"功率偏置"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_APOSA, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_APOSA = (INT16U)temp;
			}
        break;
        case 0x05:/*"相位补偿"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_PHSA, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_PhsA = (INT16U)temp;
			}
        break;
        case 0x06:

        break;
        case 0x07:

        break;
        case 0x09:/*"脉冲常数"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_HFCNST, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_hfconst = (INT16U)temp;
			}
        break;
        case 0x0b:/*"潜动阀值"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_PSTRT, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_PStart = (INT16U)temp;
			}
        break;
        case 0x0c:/*"计量配置字"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_MTRCFG, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_MeterCfg = (INT16U)temp;
			}
        break;
        case 0x0d:/*"计量模块版本号"*/
            temp = VERSION_NUM;
        break;
        case 0x0e:/*"计量配置字2"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_MTRCFG2, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_MeterCfg2 = (INT16U)temp;
			}
        break;
        case 0x0f:/*"计量走字脉冲"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_ECCNST, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_ecconst = (INT16U)temp;
			}
        break;
        case 0x11:/*"零线电流换算系数"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_IBCOEF, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_Ib_Coefficient = (INT16U)temp;
			}
        break;
        case 0x12:/*"零线功率换算系数"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_PBCOEF, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_Pb_Coefficient = (INT16U)temp;
			}
        break;
        case 0x13:/*"零线功率增益"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_GPQB, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_GPQB = (INT16U)temp;
			}
        break;
        case 0x14:/*"零线功率偏置"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_APOSB, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_APOSB = (INT16U)temp;
			}
        break;
        case 0x15:/*"零线相位补偿"*/
            result = MU_ReadOnePara((INT8U*)(&temp), R_PHSB, 4U);
			if(result == COMM_ERR_NOERR) {
				PAdjdata->Ram_PhsB = (INT16U)temp;
			}
        break;
        case 0x16:
        	break;
        #if EDT_MODE
		case _05_R_I_AMPSTD:	/* "在线监测电流回路幅值基准" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_I_AMPSTD, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.I_AmpStd = temp;
			}
			break;
		case _05_R_I_PHSSTD:	/* "在线监测电流回路相位基准" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_I_PHSSTD, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.I_PhsStd = temp;
			}
			break;
		case _05_R_U400_AMPSTD:	/* "在线监测电压回路幅值基准U400" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_U400_AMPSTD, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.U400_AmpStd = temp;
			}
			break;
		case _05_R_U400_PHSSTD:	/* "在线监测电压回路相位基准U400" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_U400_PHSSTD, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.U400_PhsStd = temp;
			}
			break;
		case _05_R_U10_AMPSTD:	/* "在线监测电压回路幅值基准U10" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_U10_AMPSTD, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.U10_AmpStd = temp;
			}
			break;
		case _05_R_U10_PHSSTD:	/* "在线监测电压回路相位基准U10" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_U10_PHSSTD, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.U10_PhsStd = temp;
			}
			break;
        case _05_R_U10_AMPSTD2:	/* "在线监测电压回路幅值基准U10" */
            result = MU_ReadOnePara((INT8U*)(&temp), R_U10_AMPSTD2, 4U);
			if(result == COMM_ERR_NOERR) {
				gMU_Para.edt_stddata.U10_AmpStd2 = temp;
			}
			break;
		case _05_R_U10_PHSSTD2:	/* "在线监测电压回路相位基准U10" */
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
Description:读计量芯片寄存器函数
Input:  pData 校表命令帧数据指针
Output: 	无
Return:TRUE  成功  FALSE  失败
Others:
************************************************"*/
INT8U MU_CaliReadOther(INT8U *pData,INT8U *length)
{
  INT8U ret=COMM_ERR_NOERR;

  return ret;
}

/*"--------精 调 函 数--------"*/
#if AMEND_MODE
void Amend_OneverySeond(void)
{
  gAmendtimer.Amend_Resume=0;
  	Amend_out_data.Voltage = (INT16U)(gMU_Interdata.Voltage/1000);
	Amend_out_data.Current = (INT16U)(gMU_Interdata.CurrentA/100);
	Amend_out_data.Factor= (INT16U)(gMU_Interdata.FactorA/100);

      // if(((Amend_out_data.Current>2300)&&(Amend_out_data.Current<2700))||((Amend_out_data.Current>4700)&&(Amend_out_data.Current<5300)))/*""在5%IB=5A正负5%电流以及10%IB 0.5L下修调电压影响量"*/
	if(Current_IN(25)||Current_IN(50))
	  {
	Amend_Un();/*""电压影响量修调"*/
	    }
	Amend_Harmonic();/*""5次谐波影响修调函数"*/
	Amend_Imax();/*""自热实验补偿"*/
	Amend_0_5mT();/*""0.5MT修调"*/
	Amend_Up_Down_Main();/*"升降变差修调"*/
	if((gAmendtimer.Amend_Resume==0)&&(gAmendtimer.Amend_Complete!=0))/*""如果当前状态不在修正区间且之前修正过，就恢复成正常的调校值xl,20110907"*/
		{

		   //DC_GetData(E2_MEA_PARA1,(INT8U *)&gMU_Para,0,sizeof(ST_MU_PARA));
                   FS_ReadFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT,(INT8U *)&gMU_Para,sizeof(ST_MU_PARA));
			MU_IC_WriteReg(WRT_PRTCT, (INT8U  *)&Write_Unlock,1);//开写保护
			MU_IC_WriteReg(GP, (INT8U  *)&gMU_Para.ram_adj_data.Ram_gp,2);//写寄存器_GP
			MU_IC_WriteReg(WRT_PRTCT, (INT8U  *)&Write_lock,1);//关写保护
		      RegScale_GP_Now = gMU_Para.ram_adj_data.Ram_gp;
		      gMU_Para.CRC_Val = CRC16((INT8U*)&gMU_Para, FIND(ST_MU_PARA,CRC_Val));
	           gAmendtimer.Amend_Complete=0;/*""设置当前为未修正状态 xl,20110907"*/
		    gAmendtimer.Amend_Resume=0; /*"" 设置当前为不需要修正区间 xl,20110907"*/
		}
}
INT8U Set_Amend(INT8U *pData)/*""设置修调函数"*/
{
		 INT8U RetVal = COMM_ERR_NOERR;
		 INT32S *pointer;
		 INT32U temp;
		  LIB_Reverse (&pData[5], (INT8U *)&temp, 4);/*""校表命令下发的时候没有高低字节反序"*/
		  if(Cal_Status_flag==CAL_STATUS_START)
		  	{
		  		if(pData[4]<=0x0F)/*""发校表开始命令且在设置范围后才能修改"*/
				 {
        			pointer = (INT32S *)&gMU_Para.AmendValue.Ib_80Un_1;
             	       	pointer += pData[4];
				if((pData[4]>0)&&(pData[4]<0x06))	/*""电压修调协议对应"*/
				{
				pointer++;
				}
				else if((pData[4]>=0x06)&&(pData[4]<=0x08))	/*""升降变差修调值协议对应"*/
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
			//	else  if(pData[4]==0x0F)//5次谐波
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
		         	//DC_GetData(E2_MEA_PARA1,(INT8U *)&gMU_Para.ram_adj_data.Ram_gp,12,2);//读取功率比差的原始值
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


INT8U  Read_Amend(INT8U *pData)/*""读修调函数"*/
{
	INT8U  result=COMM_ERR_NOERR;
	//INT8U  i;
	 INT32S *pointer;
	if(pData[4]<=0x0F)
		{
		//DC_GetData(E2_MEA_PARA1,(INT8U *)&gMU_Para,0,sizeof(ST_MU_PARA));/*""读出修调的内卡值"*/
                FS_ReadFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT,(INT8U *)&gMU_Para,sizeof(ST_MU_PARA));
		pointer = (INT32S *)&gMU_Para.AmendValue.Ib_80Un_1;
              pointer += pData[4];
		if((pData[4]>0)&&(pData[4]<0x06))	/*""电压修调对应值"*/
		{
		pointer++;
		}
		else if((pData[4]>=0x06)&&(pData[4]<=0x08))	/*""升降变差修调值协议对应"*/
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
    //DC_GetData(E2_MEA_PARA1,(INT8U *)&gMU_Para,0,sizeof(ST_MU_PARA));/*""读出修调的内卡值"*/
    FS_ReadFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT,(INT8U *)&gMU_Para,sizeof(ST_MU_PARA));
    if(gMU_Para.CRC_Val !=  CRC16((INT8U *)&gMU_Para, FIND(ST_MU_PARA,CRC_Val)))
	LIB_MemSet(0, (INT8U *)&gMU_Para.AmendValue, sizeof(ST_AMEND_VALUE));/*""如果从内卡读出的值CRC校验失败，则把修调值置0，不做修调"*/
	return RetVal;
}


/*"************************************************
Function: Amend_Un
Description:X%Un情况下精度修正函数
Global: mAmendValue，gAmendtimer
Calls:  无
Input:  无
Output: 无
Return: 无
Others: 无
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
 	if((Amend_out_data.Voltage>U_Data[UType*2])&&(Amend_out_data.Voltage<U_Data[UType*2+1]))/*""当前电压为data1Min到data1Max之间时"*/
   	{
   		 gAmendtimer.Amend_Resume=1;/*""设置当前为修正区间"*/
    		if((Amend_out_data.Factor>95)&&(Amend_out_data.Factor<104))/*""修正功率因素为1.0时的精度"*/
    		{
    		        factorNum = 1;
    		}
		if((Amend_out_data.Factor>45)&&(Amend_out_data.Factor<55))/*""修正功率因素为0.5L时的精度"*/
    		{
    		        factorNum = 2;
    		}
		if(factorNum!=0)
		{
			Num = (factorNum + UType*2);//计算是哪个修调参数
		   if((gAmendtimer.Amend_Complete==Num)&&(gAmendtimer.Amend_Setchange==0))/*""如果上一次修复修复此点并且修复值没有重新被设置，则不进行修"*/
	 		{
	 		}
			else
			{
				LoadEEParam_Amend();
				pointer += (Num-1);
				TempAmendValue1= (INT32S)*pointer;
				if(Cal_gp(TempAmendValue1))
					{
					gAmendtimer.Amend_Complete=Num;//修正完后置相应点的修正状态
					}
			}
		}
 	}
     	}
 }


/*"************************************************
Function: Amend_Imax
Description:Imax自热情况下精度修正函数
Global: mAmendValue，gAmendtimer
Calls:  无
Input:  无
Output: 无
Return: 无
Others: 无
*************************************************/
INT32S Temp_Amend;
 void Amend_Imax()
 {
	//INT32S Temp;
	INT8U Min;
	INT8U factor = 0;
	if(gMU_Para.AmendValue.Imax_10min_1!=0)//扩展IMAX10分钟1.0的为继电器模式切换点。
	{
	Imax_Data[0] = 600;//宏发继电器
	Imax_Data[1] = 1800;
	Imax_Data[2] = 1800;
	Imax_Data[3] = 3000;
	Imax_Data[4] = 3000;
	Imax_Data[5] = 4200;
	}
	else
	{
	Imax_Data[0] = 180;//中坤继电器
	Imax_Data[1] = 420;
	Imax_Data[2] = 420;
	Imax_Data[3] = 600;
	Imax_Data[4] = 600;
	Imax_Data[5] = 4200;
	}

 	//if((Amend_out_data.Current>5500)&&(Amend_out_data.Current<9000))/*""当前电流为55A到90A之间时"*/
	if(Current_IN(6000))
	{
			if(gAmendtimer.Amend_Imax_Time<4200)/*""在70分钟内"*/
   			{
   				gAmendtimer.Amend_Imax_Time++;/*""Imax电流持续时间每秒增加"*/
   			}
  		gAmendtimer.Amend_Imax_LoseTime=0;/*""允许电流变化时间置0"*/
        for(Min=0;Min<3;Min++)
        {
   		if((gAmendtimer.Amend_Imax_Time>Imax_Data[Min*2])&&(gAmendtimer.Amend_Imax_Time<=Imax_Data[Min*2+1]))/*""Imax持续600秒到1800秒之间后开始第一次修正"*/
   		{
   			gAmendtimer.Amend_Resume = 1;/*""设置当前为修正区间"*/
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
      				if((gAmendtimer.Amend_Complete==(factor + 3*Min))&&(gAmendtimer.Amend_Setchange==0))/*""如果上一次修复修复此点并且修复值没有重新被设置，则不进行修正，维持原状"*/
	 			{
	 			}
				else
				{
						LoadEEParam_Amend();/*""从内卡读取修正值"*/
						Temp_Amend = (INT32S)(*((INT32U *)&gMU_Para.AmendValue.Imax_50min_1+(factor-0x0D)));
                       			 Temp_Amend = (Temp_Amend*(Min+1))/3;
				 		TempAmendValue1 = Temp_Amend;
	#if 0
						if(((*(&gMU_Para.AmendValue.Imax_50min_1+(factor-0x0D)))&0x80000000)!=0)/*""如果调校值为负值"*/
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
	 					gAmendtimer.Amend_Complete=(INT8U)(factor + 3*Min);/*"修正完后置相应点的修正状态"*/
     	 					}


     	 				}
						}
   			 }
   		}
   	}
	else
  	{
	  		gAmendtimer.Amend_Imax_LoseTime++;/*""当不处于10IB时，电流允许变化时间开始计时"*/
	 		if(gAmendtimer.Amend_Imax_LoseTime>15)/*""当电流允许变化时间大于15秒钟时，判断当前已经切换成其他点进行。"*/
	 		{
	 			gAmendtimer.Amend_Imax_Time=0;
				gAmendtimer.Amend_Imax_LoseTime=0;
	 		}
	   		return;
  	}
    }


/*"************************************************
Function: Amend_Harmonic
Description:5次谐波下精度修正函数
Global: mAmendValue，gAmendtimer
Calls:  无
Input:  无
Output: 无
Return: 无
Others: 无
*************************************************/
void Amend_Harmonic()//5次谐波影响
{
	if(((Amend_out_data.Current>2200)&&(Amend_out_data.Current<2970))||((Amend_out_data.Current>3030)&&(Amend_out_data.Current<4500)))//&&(((Data1>2050000)&&(Data1<2185000))||((Data1>2215000)&&(Data1<2350000))))
  	{
		gAmendtimer.Amend_Resume=1;/*""设置当前为修正区间"*/
		if((gAmendtimer.Amend_Complete==0x16)&&(gAmendtimer.Amend_Setchange==0))/*""如果上一次修复修复此点并且修复值没有重新被设置，则不进行修正，维持原状"*/
		{
		}
		else
		{/*""如果上一次不是修正此点或者重新设置了调校值，则执行下面"*/
			LoadEEParam_Amend();/*""从内卡读取修正值"*/
			if(Cal_gp(gMU_Para.AmendValue.Harmonic))
			{
		         	gAmendtimer.Amend_Complete=0x16;/*""修正完后置相应点的修正状态"*/
	          	}
	           	//}
		}/*""设置调校参数为未变化状态"*/
	}
    	else
	{
		return;
	}
}

INT32U MT_P0 = 0;/*""基准功率"*/
INT32U MT_P1 = 0;/*""读取功率"*/
INT32U MT_U0 = 0;/*""读取电压"*/
INT32U MT_U1 = 0;/*""读取电压"*/


/*"************************************************
Function: Amend_0_5mT
Description:0.5mT修调影响量精度修正函数
Global: mAmendValue，gAmendtimer
Calls:  无
Input:  无
Output: 无
Return: 无
Others: 无
*************************************************/
void Amend_0_5mT(void)/*""0.5MT修调"*/
{
  INT8U MT_Test;
  INT32S Abs;
       if(((280 < gMU_Interdata.CurrentA)&&(gMU_Interdata.CurrentA < 850)))/*"在50mA附近且有读瞬时量操作"*/
	{
			MT_Timer1++;
			if((MT_Timer1 > 10)&&(mT_Start_Flag==0))/*"连续十秒都在50毫秒附近则认为要准备做0.5mT试验"*/
			 {
			 mT_Start_Flag = 0x01;/*"开始做0.5mT标志"*/
			 //DC_OutputData(E2_MT_START_FLG,(INT8U *)&mT_Start_Flag,0,1);/*"存储0.5mT标志"*/
             FS_WriteFile(E_FILE_LRM,FIND(ST_LRM_FILE_DATA,MtStartFlg),(INT8U *)&mT_Start_Flag,1);
			 }
	}
	else
	{
			 MT_Timer1 = 0;
	}

 // if(((48000 < gMU_Interdata.CurrentA)&&(gMU_Interdata.CurrentA< 52000)) &&(mT_Start_Flag == 1))/*""在5A附件，则开始做0.5MT标志"*/
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
		MT_P0 = MT_P0>>2;						/*""取4次功率平均值为参考值点"*/
		MT_U0 = Amend_out_data.Voltage;
		MT_Test = 0;
		//DC_OutputData(E2_MT_START_FLG,&MT_Test,0,1);/*""存储0.5mT标志"*/
		FS_WriteFile(E_FILE_LRM,FIND(ST_LRM_FILE_DATA,MtStartFlg),(INT8U *)&MT_Test,1);
              LoadEEParam_Amend();/*""从内卡读取修正值"*/
              RegScale_GP_Now = gMU_Para.ram_adj_data.Ram_gp;/*""准备好原始功率比差寄存器的值"*/
		}
		if((15 < MT_K)&&(MT_K <3600))/*""从第15s秒钟开始到1个小时"*/
		{
		gAmendtimer.Amend_Resume=1;/*""设置当前为修正区间"*/
		MT_P1 = gMU_Interdata.PowerPA;/*""读功率值"*/
		MT_U1 = Amend_out_data.Voltage;/*""读电压值"*/
		//Abs = LIB_ABS(MT_P1 - MT_P0);
		Abs = MT_P1-MT_P0;
		if((Abs < 55)&&(Abs >-55))			/*""功率大于标志值, 上限为1.1Kw的0.5%"*/
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

			/*""限制电压在0.5%范围内变动2.2V"*/
 			/*" 功率比差寄存器和原始值比较改变量大于0.5%误差则不补偿，恢复原始值"*/
			//DC_GetData(E2_MEA_PARA1,(INT8U *)&gMU_Para.ram_adj_data.Ram_gp,12,2);//读取功率比差的原始值
			FS_ReadFile(E_FILE_LRM,MU_PARA_BAK1_SHIFT+FIND(ST_MU_PARA,ram_adj_data)+FIND(RAM_ADJ_DATA,Ram_gp),(INT8U *)&gMU_Para.ram_adj_data.Ram_gp,2);
			if((LIB_ABS(MT_U1 - MT_U0)>22)||(LIB_ABS((INT32U)gMU_Para.ram_adj_data.Ram_gp - (INT32U)RegScale_GP_Now)>165))/*""165对于0.5%误差的功率比差值"*/
			{
			RegScale_GP_Now = gMU_Para.ram_adj_data.Ram_gp;
			}
	 	Write_RegSCALE_E1();
	     gAmendtimer.Amend_Complete=0x17;/*""修正完后置相应点的修正状态"*/
	     gAmendtimer.Amend_Setchange=0;
	    }
  	   }
   }
}

void  Write_RegSCALE_E1(void)
{
    gMU_Para.ram_adj_data.Ram_gp = RegScale_GP_Now;
	gMU_Para.CRC_Val = CRC16((INT8U*)&gMU_Para, FIND(ST_MU_PARA,CRC_Val));
	MU_IC_WriteReg(WRT_PRTCT, (INT8U  *)&Write_Unlock,1);//开写保护
	MU_IC_WriteReg(GP, (INT8U  *)&gMU_Para.ram_adj_data.Ram_gp,2);//写寄存器_GP
	MU_IC_WriteReg(WRT_PRTCT, (INT8U  *)&Write_lock,1);//关写保护
}


/*"************************************************
Function: Amend_Up_Down_Main
Description:升降变差修调主函数
Global: mAmendValue，gAmendtimer
Calls:  无
Input:  无
Output: 无
Return: 无
Others: 无
*************************************************/
void Amend_Up_Down_Main()
{
	Amend_Up_Down_Out();			/*"升降变差修调退出机制"*/
	if((Amend_out_data.Voltage > 2150)&&(Amend_out_data.Voltage < 2250)&&(Amend_out_data.Factor>95)&&(Amend_out_data.Factor<104))
	{
	Amend_Up_Down_Judge();		/*"升降变差修调判断机制"*/
	Amend_Up_Down();				/*"升降变差修调补偿机制"*/
	}
}

/*"************************************************
Function: Amend_Up_Down_Judge
Description:升降变差精度识别机制
Global: mAmendValue，gAmendtimer
Calls:  无
Input:  无
Output: 无
Return: 无
Others: 无
*************************************************/
void Amend_Up_Down_Judge()
{
//if((Amend_out_data.Current > 23)&&(Amend_out_data.Current < 27))/*"当前电流为230mA到270mA之间时"*/
if(Current_IN(25))
  {
    	 if(gAmendtimer.Amend_Up0_05Ib_Timer <=30)/*"在5%IB之间时，维持30秒钟以上时间"*/
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

//if((Amend_out_data.Current > 480)&&(Amend_out_data.Current < 520))/*"当前电流为4.8A到5.2A之间时"*/
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

//if((Amend_out_data.Current > 5500)&&(Amend_out_data.Current < 6500))/*"当前电流为55A到65A之间时"*/
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
	    	gAmendtimer.Amend_UpDownFlg = 0xAA;/*"判断处于修调状态标识"*/
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
Description:升降变差精度退出机制
Global: mAmendValue，gAmendtimer
Calls:  无
Input:  无
Output: 无
Return: 无
Others: 无
*************************************************/
void Amend_Up_Down_Out()
{
  if((gAmendtimer.Amend_UpDownIbFlg == 0x55)&&(gAmendtimer.Amend_UpDownFlg!=0xAA))
	{
		gAmendtimer.Amend_Judge_Timer++;
		if(gAmendtimer.Amend_Judge_Timer > 600)/*"如果电流上升过程超过了10分钟，则清零标志，重新判断,实际测试升降变差时间上升时间<5分钟"*/
		{
		//Amend_Up_Down_Clear();
		LIB_MemSet(0, (INT8U *)&gAmendtimer.Amend_Judge_Timer, 10);
		}
	}

  if(gAmendtimer.Amend_Down_0_05Ib_Timer > 480)/*"如果电流补偿过程5%IB超过了8分钟，则退出"*/
  	{
		//Amend_Up_Down_Clear();
		LIB_MemSet(0, (INT8U *)&gAmendtimer.Amend_Judge_Timer, 10);
  	}

  if(gAmendtimer.Amend_Down_0_05Ib_Timer >30)/*"如果电流补偿过程5%IB超过了30秒钟，则下一次切换成其他点则退出"*/
  	{
	 if((Amend_out_data.Current > 23)&&(Amend_out_data.Current < 27)&&(Amend_out_data.Voltage > 2150)&&(Amend_out_data.Voltage < 2250)&&(Amend_out_data.Factor>95)&&(Amend_out_data.Factor<104))
	 	{
	 	}
	 else			/*"如果已经补偿了5%IB电流，则切换下个电流点则退出"*/
	 	{
		//Amend_Up_Down_Clear();
		LIB_MemSet(0, (INT8U *)&gAmendtimer.Amend_Judge_Timer, 10);
	 	}
  	}
}
	#if 0
/*"清零升降变差的判断标志，时间"*/
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
Description:升降变差精度补偿
Global: mAmendValue，gAmendtimer
Calls:  无
Input:  无
Output: 无
Return: 无
Others: 无
*************************************************/
void Amend_Up_Down()
{
     if((gAmendtimer.Amend_UpDownFlg == 0xAA)&&(gAmendtimer.Amend_UpDownIbFlg == 0x55)&&(gAmendtimer.Amend_UpDownImaxFlg == 0x55))/*"必须之前5%IB,IB,Imax都置过标识，在判断进入修调"*/
     	{
     		//if((Amend_out_data.Current > 23)&&(Amend_out_data.Current < 27))/*"当前电流为230mA到270mA之间时"*/
			if(Current_IN(25))
			{
     		 	gAmendtimer.Amend_Resume = 1;/*""设置当前为修正区间"*/
     		 	gAmendtimer.Amend_Down_0_05Ib_Timer ++;
	 		if((gAmendtimer.Amend_Complete!=0x18)&&(gAmendtimer.Amend_Setchange==0))/*""如果上一次修复修复此点并且修复值没有重新被设置，则不进行修正，维持原状"*/
	 		{
	 			LoadEEParam_Amend();/*""从内卡读取修正值"*/
				if(Cal_gp(gMU_Para.AmendValue.Up_Down_0_05Ib))
				{
	 				gAmendtimer.Amend_Complete=0x18;/*"修正完后置相应点的修正状态"*/
     	 			}
	 		}
     		}
		else
		{
		gAmendtimer.Amend_Down_0_05Ib_Timer = 0;
		}

		//if((Amend_out_data.Current > 480)&&(Amend_out_data.Current < 520))/*"当前电流为4.8A到5.2A之间时"*/
		if(Current_IN(500))
		{
	 		gAmendtimer.Amend_Resume = 1;/*""设置当前为修正区间"*/
			if((gAmendtimer.Amend_Complete!=0x19)&&(gAmendtimer.Amend_Setchange==0))/*""如果上一次修复修复此点并且修复值没有重新被设置，则不进行修正，维持原状"*/
	 		{
	 			LoadEEParam_Amend();/*""从内卡读取修正值"*/
				if(Cal_gp(gMU_Para.AmendValue.Up_Down_Ib))
				{
	 				gAmendtimer.Amend_Complete=0x19;/*"修正完后置相应点的修正状态"*/
     	 			}
	 		}
	 	}

		//if((Amend_out_data.Current > 5500)&&(Amend_out_data.Current < 6500))
		if(Current_IN(6000))
		{
			gAmendtimer.Amend_Resume = 1;/*""设置当前为修正区间"*/
			if((gAmendtimer.Amend_Complete!=0x1A)&&(gAmendtimer.Amend_Setchange==0))/*""如果上一次修复修复此点并且修复值没有重新被设置，则不进行修正，维持原状"*/
	 		{
	 			LoadEEParam_Amend();/*""从内卡读取修正值"*/
				if(Cal_gp(gMU_Para.AmendValue.Up_Down_Imax))
				{
	 				gAmendtimer.Amend_Complete=0x1A;/*"修正完后置相应点的修正状态"*/
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
	if((AmendValue!=0)||(Cal_Status_flag == CAL_STATUS_START)||(gAmendtimer.Amend_Complete!=0))/*"在校表状态或者是校表修调值不为0或者上一秒有其他修调项目则进行参数计算"*/
		{
	        	//if((AmendValue<0)/*""如果调校值为负值"*/
			//{
	       	//  RegScale_GP_Now =(INT16U) (gMU_Para.ram_adj_data.Ram_gp - AmendValue);
			//}
			//else
			//{
			RegScale_GP_Now = (INT16U) (gMU_Para.ram_adj_data.Ram_gp +AmendValue);
			//}
			Write_RegSCALE_E1();
			gAmendtimer.Amend_Setchange=0;/*"设置调校参数为未变化状态"*/
		}
	else
		{
		result= FALSE;
		}
	return result;
}
#endif
