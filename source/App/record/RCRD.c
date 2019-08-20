/*"*****************************************************************************"*/
/*"  FileName: RCRD_dataProcess.c"*/
/*"  Description:     该文件提供记录快照模块对外对象访问接口"*/
/*"  Author: quantp"*/
/*"  Version:   V1.0"*/
/*"  Date:2017.09.09"*/

/*"  Function List:   "*/
/*"                   "*/
/*"  History:         // 历史修改记录"*/
/*"      <author>  <time>   <version >   <desc>"*/
/*"      quantp    2017/9/9     1.0     build this moudle  "*/
/*"*******************************************************************************"*/
#include "system\Sys_def.h"
#include "APP\message\MS.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "APP\message\FRMT_def.h"
#include "RCRD.h"
#include "LR.h"
#include "DataCenterSoftware\Code\record\RCRD_data.h"
#include "RCRD_dataProcess.h"


#define MAXMONTHBILLNUM     12  /*"最大补月结算次数"*/
#define MAXSTEPBILLNUM       4  /*"最大补年结算次数"*/
#define MAXDAYFRZNUM         7  /*"最大补日冻结次数"*/

#define PWRFRZ_BEGIN      0xaa  /*"补结算已经开始"*/
#define PWRFRZ_END        0x00  /*"补结算已经结算"*/

#define _PWR_UP_DOT           0  /*"上电补结算"*/
#define _PWR_DOWN_DOT         1  /*"掉电补结算"*/

typedef enum
{
    SUP_DAY_FRZ = 0, /*"补日冻结"*/
    SUP_MON_BILL,    /*"补月结算"*/
    SUP_STEP_BILL,   /*"补阶梯结算"*/
    MAX_TYPE_ID
}EN_TYPE_TD;

//INT8U RCRDDataBuf[RCRD_BUF_SIZE];/*"数据中心记录文件数据缓冲区，用于事件记录"*/
INT32U mMinCnt;/*"当前时间距离2000年1月1日0点0分的分钟数"*/
NO_INIT ST_RCRD_PARA_TABLE rcrd_para;/*"设置的月结算日"*/
#ifdef MAIN_METER
static ST_3TIME_FORM mstYearBillDate[YEAR_BILL_DATE_NUM];/*"设置的年结算日"*/
#endif

ST_6TIME_FORM mstNowTime;
#if defined(THREE_PHASE)
INT8U is_2_3_mouth_bill; /*" 为月结算第2、3结算 "*/
#endif
extern INT8U meter_type;

#define NO_STEP_MD      0
#define MOUTH_STEP_MD   1
#define YEAR_STEP_MD    2

INT8U GetMeterType(void)
{
    INT8U type;

    type = 0;
#ifdef MAIN_METER    
    GetSingle(E_METER_MODE, (INT8U *)&type);
	type &= METERTPYE;
#endif
    return type;
}

#ifdef MAIN_METER 
static INT8U GetStepMode(void)
{
    INT8U mode;
    
    if(ChkStatus(E_MON_STEP) == TRUE)
    {
        mode = MOUTH_STEP_MD;
    }
    else if(ChkStatus(E_YEAR_STEP) == TRUE)
    {
        mode = YEAR_STEP_MD;
    }
    else
    {
        mode = NO_STEP_MD;
    }
    return mode;
}

void GetYearBillDate(INT8U *dst)
{
#if (APP_TIME_FRMT == BCD_FRMT)
    INT8U i;
#endif    

    GetSingle(E_STEP_YEARBILL_CURBLK, dst);
#if (APP_TIME_FRMT == BCD_FRMT)
    for(i=0; i<(YEAR_BILL_DATE_NUM*sizeof(ST_3TIME_FORM)); i++)
    {/*" 表内参数是HEX，要转换为BCD码 "*/
        dst[i] = LIB_CharToBcd(dst[i]);
    }
#endif
}
#endif

/*"*****************************************************************************"*/
/*"  Function:       RCRD_MonBillFrz"*/
/*"  Description:   进行月累加用电量结算和月度用电量结算"*/
/*"  Calls:          LIB_MemCpy,DC_OutputData，DC_SaveRcrdData"*/
/*"  Called By:      FU_SetMonthBill,FU_Billing1min，FU_PwUpMonBill"*/
/*"  Input:         index:  第几个月结算日； "*/
/*"                 billtime：当前时间地址 "*/
/*"  Output:         "*/
/*"  Return:       无 "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void RCRD_MonBillFrz(INT8U index, INT8U *billtime)
{
    LIB_MemCpy(billtime, &DCDataBuf[0], sizeof(ST_6TIME_FORM));
#if defined(THREE_PHASE)
    is_2_3_mouth_bill = index;
#endif
    RCRD_SaveData(E_MONTH_BILL_FRZ_RCRD, E_FRZ_LST_MOUTH_BILL_FRZ);   /*"电量结算数据块写入文件系统"*/ 
#if defined(THREE_PHASE)
    is_2_3_mouth_bill = 0;
#endif    
    if(0 == index)
    {
        
#ifdef MAIN_METER
        INT8U type;
        type = 0;
        Action(E_ENGY_BILLING, &type, NULL);
#endif
#if defined(THREE_PHASE)
        Action(E_MAX_DEMAND_CLR, NULL, NULL);
#endif
    }
}



/*"*****************************************************************************"*/
/*"  Function:       FU_Freeze_Period"*/
/*"  Description:    周期冻结"*/
/*"  Calls:          DC_OutputData，DC_SaveRcrdData"*/
/*"  Called By:      FU_Freezing1min"*/
/*"  Input:          softdate：当前时间地址 "*/ 
/*"  Output:           "*/
/*"  Return:       无 "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void RCRD_Freeze_Period(ST_6TIME_FORM *softdate)
{
    RCRD_SaveData(E_HOUR_FRZ_RCRD, E_FRZ_LST_HOUR_FRZ); /*"小时冻结据写入文件系统"*/ 
}

/*"*****************************************************************************"*/
/*"  Function:       FU_Freeze_Day"*/
/*"  Description:    日冻结"*/
/*"  Calls:          LIB_MemCpy，DC_OutputData，DC_SaveRcrdData"*/
/*"  Called By:      FU_Freezing1min，FU_PwUpDayFreeze"*/
/*"  Input:          softdate：当前时间地址 "*/ 
/*"  	             FrzBillPara：周期冻结参数地址 "*/ 
/*"  Output:           "*/
/*"  Return:       无 "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void RCRD_Freeze_Day(ST_6TIME_FORM *softdate)
{
    LIB_MemCpy((INT8U *)softdate, &DCDataBuf[0], sizeof(ST_6TIME_FORM));

    RCRD_SaveData(E_DAY_FRZ_RCRD, E_FRZ_LST_DAY_FRZ); /*"日冻结数据写入文件系统"*/
#if defined(THREE_PHASE)
    Action(E_DAY_VOLTHEALTH_CLR, NULL, NULL);
#endif
}

/*"*****************************************************************************"*/
/*"  Function:       FU_Timing_Freeze"*/
/*"  Description:    定时冻结"*/
/*"  Calls:          DC_OutputData，DC_SaveRcrdData"*/
/*"  Called By:      FU_Freezing1min"*/
/*"  Input:          softdate：当前时间地址 "*/ 
/*"  Output:           "*/
/*"  Return:       无 "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void RCRD_Mouth_Freeze(ST_6TIME_FORM *softdate)
{
    RCRD_SaveData(E_MONTH_FRZ_RCRD, E_FRZ_LST_MOUTH_FRZ); /*"定时冻结数据写入文件系统"*/
#if defined(THREE_PHASE)
    Action(E_MON_VOLTHEALTH_CLR, NULL, NULL);
#endif
}


/*" 将对象降序排列插入到队列中 , 时间比较到分 ， 已经有相同的时间点时不加入到队列中 "*/
INT8U add_tm_array_descend(ST_7TIME_FORM *cur, ST_7TIME_FORM * list, INT8U sum)
{
    INT8U i, j;
    INT8S ret;

    for(i=0; i<sum; i++)
    {
        ret = LIB_MemCmp((INT8U *)cur, (INT8U *)&list[i], 6);
        if(ret == EQUAL)
        {/*" 已经有相同的时间点时不加入到队列中 "*/
            return sum;
        }
        else if(ret == BIG)
        {
            break;
        }
    }

    if(i == sum)
    {
        LIB_MemCpy((INT8U *)cur, (INT8U *)&list[i], 6);
    }
    else
    {
        for(j=sum-1; j>=i; j--)
        {
            LIB_MemCpy((INT8U *)&list[j], (INT8U *)&list[j + 1], 6);
            if(j == 0)
            {
                break;
            }
        }
        LIB_MemCpy((INT8U *)cur, (INT8U *)&list[i], 6);
    }
    return (sum+1);
}


void SubOneMouth(ST_7TIME_FORM *rDate)
{
    INT8U data[2];
    INT8U i;
   
    for(i=0;i<2;i++)
    {
        #if   (APP_TIME_FRMT == HEX_FRMT)
        data[i]=*((INT8U *)rDate+i);
        #else
        data[i]=LIB_BcdToChar(*((INT8U *)rDate+i));
        #endif
    }

    if( data[1] > 1 )
    {
        data[1]--;
    }
    else                        
    {
        data[1]=12; 
        data[0]=(data[0]-1)%100;
    }

            
  for(i=0;i<2;i++)
  {
    #if   (APP_TIME_FRMT == HEX_FRMT)
    *((INT8U *)rDate+i)=data[i];
    #else
    *((INT8U *)rDate+i)=LIB_CharToBcd(data[i]);
    #endif
  }

  rDate->day = 1;
  rDate->hour=0;
  rDate->min=0;
  rDate->sec=0;
}


void SubOneDay(ST_7TIME_FORM *rDate)
{
    INT8U data[3];
    INT8U i;
   
    for(i=0;i<3;i++)
    {
        #if (APP_TIME_FRMT == HEX_FRMT)
        data[i]=(*((INT8U *)rDate+i));
        #else
        data[i]=LIB_BcdToChar(*((INT8U *)rDate+i));
        #endif
    }

    if( data[2] > 1 )
    {/*" 日大于 1 "*/
        data[2]--;
    }
    else                        
    {/*" 日小于 1 ，跳到下月最后一天 "*/
        if( data[1] > 1 )
        {
            data[1]--;
        }
        else                        
        {
            data[1]=12; 
            data[0]=(data[0]-1)%100;
        }
    
        data[2] = LIB_CalDayHEX(data[0], data[1]);
    }

            
  for(i=0;i<3;i++)
  {
    #if (APP_TIME_FRMT == HEX_FRMT)
    *((INT8U *)rDate+i)=(data[i]);
    #else
    *((INT8U *)rDate+i)=LIB_CharToBcd(data[i]);
    #endif
  }

  rDate->hour=0;
  rDate->min=0;
  rDate->sec=0;
}


#ifdef MAIN_METER
/*"*****************************************************************************"*/
/*"  Function:       FU_StepLevelBilling"*/
/*"  Description:    进行年度用电量结算"*/
/*"  Calls:          DC_OutputData，DC_SaveRcrdData"*/
/*"  Called By:      FU_SetYearBill,FU_Billing1min，FU_PwUpYearBill"*/
/*"  Input:          billtime：当前时间地址 "*/              
/*"  Output:          "*/
/*"  Return:       无 "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void RCRD_StepLevelBilling(INT8U *billtime)
{
    INT8U type;
    	
    LIB_MemCpy(billtime, &DCDataBuf[0], sizeof(ST_6TIME_FORM));
    
    RCRD_SaveData(E_STEP_BILL_FRZ_RCRD, E_FRZ_LST_STEP_BILL_FRZ);   /*"结算数据块写入文件系统"*/
    
    /*"当前阶梯结算用电量清零"*/  
    type = 1;
    Action(E_ENGY_BILLING, &type, NULL);
}

/*"***************************************************************"*/
/*"  Function:       Powerup_Bill_Frz_Proc "*/
/*"  Description:   上电补阶梯结算 "*/
/*"  Input:         pd_date 掉电时间 "*/
/*"                 now 上电时间 "*/
/*"                 "*/
/*"  Output:         "*/
/*"  Return:        无 "*/
/*"****************************************************************"*/
void Powerup_Step_Bill_Frz_Proc(ST_7TIME_FORM *pd_date, ST_7TIME_FORM *now)
{
    ST_7TIME_FORM date[12];
    ST_7TIME_FORM cur;
    INT8U num;
    INT8U i;
    INT8S ret;
    INT8U num_bill_date;
    ST_7TIME_FORM Year_BillDay_Dot[4];/*""*/
    ST_7TIME_FORM tmp;
    INT8U step_mode;
    ST_7TIME_FORM st_date;
    INT8U flg, type;
    ST_6TIME_FORM lst_tm;

    num_bill_date = 0;
    LIB_MemSet(0x00, (INT8U *)&Year_BillDay_Dot[0], sizeof(Year_BillDay_Dot));
    step_mode = GetStepMode();
    GetYearBillDate((INT8U *)&mstYearBillDate[0]);
    if(step_mode == MOUTH_STEP_MD)
    {
        Year_BillDay_Dot[0].year = 18; /*" 无关量 "*/
        Year_BillDay_Dot[0].month = 1; /*" 无关量 "*/
        Year_BillDay_Dot[0].day = mstYearBillDate[0].day;
        Year_BillDay_Dot[0].hour = mstYearBillDate[0].hour;
        Year_BillDay_Dot[0].min = 0;
        Year_BillDay_Dot[0].sec = 0;
        Year_BillDay_Dot[0].week = 0;
        num_bill_date = 1;
    }
    else if(step_mode == YEAR_STEP_MD)
    {
        for(num=0;num<4; num++)	
    	{
             tmp.year = 18; /*" 无关量 "*/
             tmp.month = mstYearBillDate[num].month;
    		 tmp.day = mstYearBillDate[num].day;
    		 tmp.hour = mstYearBillDate[num].hour;
    		 tmp.min = 0;
    		 tmp.sec = 0;
    		 tmp.week = 0;
             if(LIB_ChkDateTime((ST_6TIME_FORM *)&tmp) == TRUE)
             {
                num_bill_date = add_tm_array_descend(&tmp, Year_BillDay_Dot, num_bill_date);
             }
    	}
    }
    else
    {
        return;
    }

    FS_ReadFile(E_FILE_RCRD_DATA, STEP_FRZ_PROC_FLG, &flg, 1);
    FS_ReadFile(E_FILE_RCRD_DATA, STEP_FRZ_LST_FRZ_DATE, (INT8U *)&lst_tm, 6);
    LIB_MemCpy((INT8U *)pd_date, (INT8U *)&st_date, 6);
    if((flg == PWRFRZ_BEGIN) && (TRUE == LIB_ChkDateTime((ST_6TIME_FORM *)&lst_tm)))
    {
        LIB_MemCpy((INT8U *)&lst_tm, (INT8U *)&st_date, 6);
    }

    LIB_MemCpy(&now->year , (INT8U *)&cur.year, 6);
    //num = 1;
    //SubOneMouth(&cur);
    num = 0;
    ret = LIB_MemCmp(&cur.year, &st_date.year, 6);
    cur.min= 0;
    cur.sec= 0;
    while(ret != SMALL)
    {
        for(i=0; i<num_bill_date; i++)
        {/*" 一个月可能有多个年结算日 "*/
            if((step_mode == YEAR_STEP_MD) && (cur.month != Year_BillDay_Dot[i].month))
            {
                if(cur.month < Year_BillDay_Dot[i].month)
                {
                    continue;
                }
                cur.month = Year_BillDay_Dot[i].month;
            }
            cur.day = Year_BillDay_Dot[i].day;
            cur.hour = Year_BillDay_Dot[i].hour;

            ret = LIB_MemCmp(&cur.year, &now->year, 6);
            if(ret != SMALL)
            {
                continue;
            }
            
            ret = LIB_MemCmp(&cur.year, &st_date.year, 6);
            if(num >= 5)
            {
                break; /*" 最多补冻结4次"*/
            }
            if(ret != SMALL)
            {
                LIB_MemCpy(&cur.year , (INT8U *)&date[num], 6);
                num++;
            }
        }
        if(num >= 5)
        {
            break; /*" 最多补冻结4次"*/
        }
                    
        SubOneMouth(&cur);
        if(step_mode == MOUTH_STEP_MD)
        {
            cur.day = Year_BillDay_Dot[0].day;
            cur.hour = Year_BillDay_Dot[0].hour;
        }
#if   (APP_TIME_FRMT == HEX_FRMT)
        if(cur.year == 0xff)
        {
            break;
        }
#else
        if(cur.year == 0x99)
        {
            break;
        }
#endif
        ret = LIB_MemCmp(&cur.year, &st_date.year, 6);
    }
    
    if(num > 0)
    {
        flg = PWRFRZ_BEGIN;
        FS_WriteFile(E_FILE_RCRD_DATA, STEP_FRZ_PROC_FLG, &flg, 1);
    }
    for(i=0; i<num; i++)
    {
        if((num == 5) && (i == 0))
        {/*" 多预留一次用于判断是否超过4个阶梯结算 "*/
            /*"当前阶梯结算用电量清零"*/  
            type = 1;
            Action(E_ENGY_BILLING, &type, NULL);
            continue;
        }
        if((i == 0) && (LIB_MemCmp(&lst_tm.year, (INT8U *)&date[num - i - 1], 6) == EQUAL))
        {
            continue;
        }
                
        RCRD_StepLevelBilling((INT8U *)&date[num - i - 1]);
    }
    if(num > 0)
    {
        flg = 0;
        FS_WriteFile(E_FILE_RCRD_DATA, STEP_FRZ_PROC_FLG, &flg, 1);
    }
}

 
/*"***************************************************************"*/
/*"  Function:       Powerup_Mouth_Frz_Proc "*/
/*"  Description:   上电补月结算 "*/
/*"  Input:         pd_date 掉电时间 "*/
/*"                 now 上电时间 "*/
/*"                 "*/
/*"  Output:         "*/
/*"  Return:        无 "*/
/*"****************************************************************"*/
void Powerup_Mouth_Bill_Frz_Proc(ST_7TIME_FORM *pd_date, ST_7TIME_FORM *now)
{
    ST_7TIME_FORM date[MAXMONTHBILLNUM + 1];
    INT8U bill_idx[MAXMONTHBILLNUM + 1];
    INT8U bill_num[MON_BILL_DATE_NUM];
    ST_7TIME_FORM cur;
    INT8U num;
    INT8U i;
    INT8S ret;
    INT8U num_bill_date;
    ST_7TIME_FORM Mouth_BillDay_Dot[4];/*""*/
    ST_7TIME_FORM tmp;
    ST_7TIME_FORM st_date;
    INT8U flg, type;
    ST_6TIME_FORM lst_tm;

    FS_ReadFile(E_FILE_RCRD_DATA, MOUTH_BILL_FRZ_PROC_FLG, &flg, 1);
    FS_ReadFile(E_FILE_RCRD_DATA, MOUTH_BILL_FRZ_LST_FRZ_DATE, (INT8U *)&lst_tm, 6);
    LIB_MemCpy((INT8U *)pd_date, (INT8U *)&st_date, 6);
    if((flg == PWRFRZ_BEGIN) && (TRUE == LIB_ChkDateTime((ST_6TIME_FORM *)&lst_tm)))
    {
        LIB_MemCpy((INT8U *)&lst_tm, (INT8U *)&st_date, 6);
    }

    num_bill_date = 0;
    LIB_MemSet(0x00, (INT8U *)&Mouth_BillDay_Dot[0], sizeof(Mouth_BillDay_Dot));
    for(num=0;num<MON_BILL_DATE_NUM; num++)	
	{
         tmp.year = 0x01; /*" 无关量 "*/
         tmp.month = 0x01; /*" 无关量 "*/
		 tmp.day = rcrd_para.monBillDay[num].day;
		 tmp.hour = rcrd_para.monBillDay[num].hour;
		 tmp.min = 0;
		 tmp.sec = 0;
		 tmp.week = 0;
         if(LIB_ChkDateTime((ST_6TIME_FORM *)&tmp) == TRUE)
         {
            num_bill_date = add_tm_array_descend(&tmp, Mouth_BillDay_Dot, num_bill_date);
         }
	}

    for(num=0;num<num_bill_date; num++)
    {
        if((Mouth_BillDay_Dot[num].day == rcrd_para.monBillDay[0].day)
            && (Mouth_BillDay_Dot[num].hour == rcrd_para.monBillDay[0].hour))
        {
            bill_num[num] = 0;
        }
        else
        {
            bill_num[num] = 1;
        }
    }

    LIB_MemCpy(&now->year , (INT8U *)&cur.year, 6);
    num = 0;
    ret = LIB_MemCmp(&cur.year, &st_date.year, 6);
    cur.min = 0;
    cur.sec = 0;
    while(ret != SMALL)
    {
        for(i=0; i<num_bill_date; i++)
        {/*" 一个月可能有多个月结算日 "*/
            cur.day = Mouth_BillDay_Dot[i].day;
            cur.hour = Mouth_BillDay_Dot[i].hour;

            ret = LIB_MemCmp(&cur.year, &now->year, 6);
            if(ret != SMALL)
            {
                continue;
            }
            
            ret = LIB_MemCmp(&cur.year, &st_date.year, 6);
            if(num >= (MAXMONTHBILLNUM + 1))
            {
                break; /*" 最多补冻结12次"*/
            }
            if(ret != SMALL)
            {
                LIB_MemCpy(&cur.year , (INT8U *)&date[num], 6);
                bill_idx[num] = bill_num[i];
                num++;
            }
        }
        if(num >= (MAXMONTHBILLNUM + 1))
        {
            break; /*" 最多补冻结12次"*/
        }
                    
        SubOneMouth(&cur);
        cur.day = Mouth_BillDay_Dot[0].day;
        cur.hour = Mouth_BillDay_Dot[0].hour;
#if   (APP_TIME_FRMT == HEX_FRMT)
        if(cur.year == 0xff)
        {
            break;
        }
#else
        if(cur.year == 0x99)
        {
            break;
        }
#endif
        ret = LIB_MemCmp(&cur.year, &st_date.year, 6);
    }

    if(num > 0)
    {
        flg = PWRFRZ_BEGIN;
        FS_WriteFile(E_FILE_RCRD_DATA, MOUTH_BILL_FRZ_PROC_FLG, &flg, 1);
    }
    for(i=0; i<num; i++)
    {
        if((num == (MAXMONTHBILLNUM + 1)) && (i == 0))
        {/*" 多预留一次用于判断是否超过4个阶梯结算 "*/
            /*"TODO 当前月度用电量清零"*/
            type = 0;
            Action(E_ENGY_BILLING, &type, NULL);
            continue;
        }
        if((i == 0) && (LIB_MemCmp(&lst_tm.year, (INT8U *)&date[num - i - 1], 6) == EQUAL))
        {
            continue;
        }
                
        RCRD_MonBillFrz(bill_idx[num - i - 1], (INT8U *)&date[num - i - 1]);
    }
    if(num > 0)
    {
        flg = 0;
        FS_WriteFile(E_FILE_RCRD_DATA, MOUTH_BILL_FRZ_PROC_FLG, &flg, 1);
    }
}

 
/*"***************************************************************"*/
/*"  Function:       Powerup_Day_Frz_Proc "*/
/*"  Description:   上电补日冻结 "*/
/*"  Input:         pd_date 掉电时间 "*/
/*"                 now 上电时间 "*/
/*"                 "*/
/*"  Output:         "*/
/*"  Return:        无 "*/
/*"****************************************************************"*/
void Powerup_Day_Frz_Proc(ST_7TIME_FORM *pd_date, ST_7TIME_FORM *now)
{
    ST_7TIME_FORM date[MAXDAYFRZNUM + 1];
    ST_7TIME_FORM cur;
    INT8U num;
    INT8U i;
    INT8S ret;
    ST_7TIME_FORM st_date;
    INT8U flg;
    ST_6TIME_FORM lst_tm;

    FS_ReadFile(E_FILE_RCRD_DATA, DAY_FRZ_PROC_FLG, &flg, 1);
    FS_ReadFile(E_FILE_RCRD_DATA, DAY_FRZ_LST_FRZ_DATE, (INT8U *)&lst_tm, 6);
    LIB_MemCpy((INT8U *)pd_date, (INT8U *)&st_date, 6);
    if((flg == PWRFRZ_BEGIN) && (TRUE == LIB_ChkDateTime((ST_6TIME_FORM *)&lst_tm)))
    {
        LIB_MemCpy((INT8U *)&lst_tm, (INT8U *)&st_date, 6);
    }

    LIB_MemCpy(&now->year , (INT8U *)&cur.year, 6);
    num = 0;
    ret = LIB_MemCmp(&cur.year, &st_date.year, 6);
    cur.hour = 0;
    cur.min = 0;
    cur.sec = 0;
    while(ret != SMALL)
    {
        ret = LIB_MemCmp(&cur.year, &st_date.year, 6);
        if(ret != SMALL)
        {
            LIB_MemCpy(&cur.year , (INT8U *)&date[num], 6);
            num++;
        }
        
        if(num >= MAXDAYFRZNUM)
        {
            break; /*" 最多补冻结7次"*/
        }
                    
        SubOneDay(&cur);
#if   (APP_TIME_FRMT == HEX_FRMT)
        if(cur.year == 0xff)
        {
            break;
        }
#else
        if(cur.year == 0x99)
        {
            break;
        }
#endif
        ret = LIB_MemCmp(&cur.year, &st_date.year, 6);
    }

    if(num > 0)
    {
        flg = PWRFRZ_BEGIN;
        FS_WriteFile(E_FILE_RCRD_DATA, DAY_FRZ_PROC_FLG, &flg, 1);
    }

    for(i=0; i<num; i++)
    {
        if((i == 0) && (LIB_MemCmp(&lst_tm.year, (INT8U *)&date[num - i - 1], 6) == EQUAL))
        {
            continue;
        }
        RCRD_Freeze_Day((ST_6TIME_FORM *)&date[num - i - 1]);
    }
    
    if(num > 0)
    {
        flg = 0x00;
        FS_WriteFile(E_FILE_RCRD_DATA, DAY_FRZ_PROC_FLG, &flg, 1);
    }
}


/*"*****************************************************************************"*/
/*"  Function:       FU_Billing1min"*/
/*"  Description:    结算分钟任务主要包括月结算、年结算。"*/
/*"  Calls:          DC_GetData,FU_MonBilling,FU_StepLevelBilling"*/
/*"  Called By:      FU_Main"*/
/*"  Input:          无 "*/
/*"  Output:         无 "*/
/*"  Return:         无"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void RCRD_Billing1min(void)
{
    INT8U num;
    ST_3TIME_FORM *pYDate;

    for(num = 0; num < MON_BILL_DATE_NUM; num++)/*"检查3个结算日是否与当前时间相等"*/
    {
        if( (EQUAL == LIB_MemCmp((INT8U *)&mstNowTime.day, (INT8U *)&rcrd_para.monBillDay[num].day, 2))
         && (mstNowTime.min == 0) )
        {
            RCRD_MonBillFrz(num, (INT8U*)&mstNowTime);
            break;
        }

    }

    if(LOCALMETER == GetMeterType())
    {/*"本地"*/
        pYDate = &mstYearBillDate[0];

        GetYearBillDate((INT8U *)&mstYearBillDate[0]);
        
        for(num = 0; num < YEAR_BILL_DATE_NUM; num++)/*"检查4个结算日是否与当前时间相等"*/
        {
			if(GetStepMode() == MOUTH_STEP_MD)
			{
				if(((pYDate->day == mstNowTime.day) && (pYDate->hour == mstNowTime.hour))
				&& (mstNowTime.min == 0) && (num == 0))/*"只看第一结算日中的日时"*/
				{
					RCRD_StepLevelBilling((INT8U*)&mstNowTime);
					break;
				}
			}
			else
			{
				if( (EQUAL == LIB_MemCmp((INT8U *)&mstNowTime.month, (INT8U *)pYDate, 3))
				 && (mstNowTime.min == 0) )
				{
					RCRD_StepLevelBilling((INT8U*)&mstNowTime);
					break;
				}
				
				pYDate++;
			}

        }
    }

}


extern INT32U overMaxDemand[6];
extern ST_5TIME_FORM overMaxDemandTime[6];
EN_RCRD_FILE_ID lst_id;

INT8U evt_test;
ST_6TIME_FORM ld_time;
extern ST_7TIME_FORM stSysTime;

ST_7TIME_FORM pdtime;
ST_7TIME_FORM now_t;

extern void Lrm_test(void);

void poweron_frz_test(void)
{
    if((FALSE == LIB_ChkDateTime((ST_6TIME_FORM *)&pdtime)) 
        || (FALSE == LIB_ChkDateTime((ST_6TIME_FORM *)&now_t))
        || (LIB_MemCmp((INT8U *)&pdtime, (INT8U *)&now_t, 6) == BIG) )
    {
        return;
    }
    else
    {
        Powerup_Step_Bill_Frz_Proc(&pdtime, &now_t);
        //Powerup_Mouth_Bill_Frz_Proc(&pdtime, &now_t);
    }
}


void evt_test_proc(void)
{
    EN_RCRD_FILE_ID id;
    INT8U ret;
    INT8U bak;
#if defined(THREE_PHASE)
    INT8U i;
#endif
    GetSingle(E_SYS_TIME, &DCDataBuf[0]);
    GetSingle(E_SYS_TIME, &DCDataBuf[6]);
#if defined(THREE_PHASE)
    for(i=0; i<6; i++)
    {
        overMaxDemand[i] = 10000;
        LIB_MemCpy(&DCDataBuf[0], (INT8U *)&overMaxDemandTime[i], 5);
    }
#endif
    bak = stSysTime.sec;

    for(id=E_PWR_DOWN_EVNT_RCRD; id<E_EVT_END_RCRD; id++)
    {
        stSysTime.sec = id%60;
        DCDataBuf[0+5] = id%60;
        DCDataBuf[6+5] = id%60;
        lst_id = id;
        ret = RCRD_SaveData(id, (EN_FELXOBJ_ID)stFileDataList[id&0xff].startFelxObj);
        if(ret == FALSE)
        {
            break;
        }
        if(stFileDataList[id&0xff].endFelxObj != NULL)
        {
            stSysTime.sec = (id+1)%60;
            DCDataBuf[0+5] = (id+1)%60;
            DCDataBuf[6+5] = (id+1)%60;
            ret = RCRD_SaveData(id, (EN_FELXOBJ_ID)stFileDataList[id&0xff].endFelxObj);
            if(ret == FALSE)
            {
                break;
            }
        }
    }

    RCRD_SaveData(E_MMT_FRZ_RCRD, E_FRZ_LST_MON_FRZ);
    for(id=E_SEASON_TBL_SW_FRZ_RCRD; id<=E_STEP_BILL_FRZ_RCRD; id++)
    {
        RCRD_SaveData(id, (EN_FELXOBJ_ID)stFileDataList[id&0xff].startFelxObj);
    }
    
    stSysTime.sec = bak;
/*
    if(evt_test == 1)
    {
        RCRD_SaveData(E_LOST_VOLT_LA_EVNT_RCRD,E_EVTLST_LOST_VOLT_S);
    }
    else if(evt_test == 2)
    {
        RCRD_SaveData(E_LOST_VOLT_LA_EVNT_RCRD,E_EVTLST_LOST_VOLT_E);
    }
*/
    //SaveRcrd(E_KEY_UPDATE_EVNT_RCRD,E_EVTLST_KEY_UPDATE); 
    //SaveRcrd(E_LOST_VOLT_LA_EVNT_RCRD,E_EVTLST_LOST_VOLT_S); 
    //SaveRcrd(E_LOST_VOLT_LA_EVNT_RCRD,E_EVTLST_LOST_VOLT_E);

    /*
    overMaxDemand[0] = 10000;
    LIB_MemCpy(&DCDataBuf[0], (INT8U *)&overMaxDemandTime[0], 5);
    RCRD_SaveData(E_OVER_PA_DEMAND_EVNT_RCRD,E_EVTLST_OVER_PA_DEMAND_S);
    RCRD_SaveData(E_OVER_PA_DEMAND_EVNT_RCRD,E_EVTLST_OVER_PA_DEMAND_E);
    */

    //DCDataBuf[0] = 0;
    //SaveRcrd(E_R_ENGY_COM_PRG_EVNT_RCRD, E_EVTLST_R_ENGY_COM_PRG_S);

    //DCDataBuf[0] = 1;
    //SaveRcrd(E_R_ENGY_COM_PRG_EVNT_RCRD, E_EVTLST_R_ENGY_COM_PRG_S);
    
    //SaveRcrd(E_VOLT_IMBALANCE_EVNT_RCRD,E_EVTLST_VOLT_IMBALANCE_S);
    //SaveRcrd(E_VOLT_IMBALANCE_EVNT_RCRD,E_EVTLST_VOLT_IMBALANCE_E);
    //SaveRcrd(E_OVER_CURR_LA_EVNT_RCRD,E_EVTLST_OVERCURR_S); 
    //SaveRcrd(E_OVER_CURR_LA_EVNT_RCRD,E_EVTLST_OVERCURR_E); 
    //RCRD_SaveData(E_MTR_COVER_OPEN_EVNT_RCRD, E_EVTLST_MTR_COVER_OPEN_S);
    //RCRD_SaveData(E_MTR_COVER_OPEN_EVNT_RCRD, E_EVTLST_MTR_COVER_OPEN_E);
    //RCRD_SaveData(E_MTR_CLR_EVNT_RCRD, E_EVTLST_CLRMTR);
    //LIB_MemCpy((INT8U *)&mstNowTime, &DCDataBuf[0], sizeof(ST_6TIME_FORM));
}
#endif

/*"*****************************************************************************"*/
/*"  Function:       PrdFrzCmp"*/
/*"  Description:    周期冻结比较"*/
/*"  Calls:          "*/
/*"  Called By:      FU_Freezing1min"*/
/*"  Input:          softdate：当前时间地址 "*/ 
/*"  	             pStartTime：周期冻结参数地址 "*/ 
/*"  Output:           "*/
/*"  Return:       _TRUE：满足冻结条件 "*/
/*"                _FALSE：不满足冻结条件"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U PrdFrzCmp(ST_6TIME_FORM *softdate, ST_5TIME_FORM *pStartTime)
{
    ST_6TIME_FORM curr_date;
    ST_6TIME_FORM start_date; 
    INT8U frzperiod;
    INT32S delta_T; 

    LIB_MemCpy((INT8U *)pStartTime, (INT8U *)&start_date, 5);
    start_date.sec = 0;

    frzperiod = rcrd_para.frzPara.prdFrzIntval;
    if((frzperiod == 0) || (frzperiod > 60))
    {/*"周期冻结时间间隔非法时取固定值"*/
        frzperiod = 60;
    }
    
    LIB_MemCpy((INT8U *)softdate, (INT8U *)&curr_date, sizeof(ST_6TIME_FORM));
    curr_date.sec = 0;

    delta_T = LIB_CmpTime(&curr_date, &start_date);
    delta_T = delta_T / 60;
    
    if(delta_T >= 0)
    {
        if((delta_T % frzperiod) == 0)
        {
            return TRUE;
        }
    }

    return FALSE;
}


/*"*****************************************************************************"*/
/*"  Function:       FU_Freezing1min"*/
/*"  Description:    冻结分钟任务主要包括定时冻结、周期冻结和日冻结"*/
/*"  Calls:          DC_GetData,FU_Freeze_Day,FU_Timing_Freeze,FU_Freeze_Period"*/
/*"  Called By:      FU_Main"*/
/*"  Input:          无 "*/
/*"  Output:         无 "*/
/*"  Return:         无"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void RCRD_Freezing1min(void)
{
    ST_FRZ_PARA *frzpara;
    ST_4TIME_FORM *pTimeFrz;
    ST_2TIME_FORM *pDayFrz;
    ST_5TIME_FORM *pPrdFrz;
    INT8U code99;

    frzpara = &rcrd_para.frzPara;

    #if   (APP_TIME_FRMT == HEX_FRMT)
    code99 = 99;
    #else
    code99 = 0x99;
    #endif
    
    pTimeFrz = (ST_4TIME_FORM *)&frzpara->timeFrzTm;
    if( ((pTimeFrz->month == mstNowTime.month) || (pTimeFrz->month == code99))
     && ((pTimeFrz->day == mstNowTime.day) || (pTimeFrz->day == code99))
     && ((pTimeFrz->hour == mstNowTime.hour) || (pTimeFrz->hour == code99))
     && (pTimeFrz->min == mstNowTime.min))
    {/*"定时冻结"*/
        RCRD_Mouth_Freeze(&mstNowTime); 
    }
    
    pPrdFrz = (ST_5TIME_FORM *)&frzpara->prdFrzStartTime;
    if(TRUE == PrdFrzCmp(&mstNowTime, pPrdFrz))
    {/*"周期冻结"*/
        RCRD_Freeze_Period(&mstNowTime);
    }

    pDayFrz = (ST_2TIME_FORM *)&frzpara->dayFrzTm;
    
    if( (pDayFrz->hour == mstNowTime.hour)
     && (pDayFrz->min == mstNowTime.min) )
    {/*"日冻结"*/
        RCRD_Freeze_Day(&mstNowTime);
    }
}

/*" 时间递增len分钟，形成一个新的合法的时间"*/
/*" len 最大不会超过99分钟 "*/
void time_add_minute(ST_6TIME_FORM *req_tm, INT16U len)
{
    INT16U timeCounter;
	INT8U temp;
    INT8U nMaxDay; 
    INT8U min;
    INT8U hour;
    INT8U day;
    INT8U month;
    INT8U year;

    //if ()/*" 如果不跨小时"*/ 
    //{
    //}
    //BCD2Byte(req_tm->Hour, &temp);
    #if   (APP_TIME_FRMT == HEX_FRMT)
    temp = req_tm->hour;
    #else
    temp = LIB_BcdToChar(req_tm->hour);
    #endif
	timeCounter = temp * 60;
	//BCD2Byte(req_tm->Min, &temp);
	#if   (APP_TIME_FRMT == HEX_FRMT)
    temp = req_tm->min;
    #else
    temp = LIB_BcdToChar(req_tm->min);
    #endif
	timeCounter += temp;

    timeCounter += len;  
    hour = timeCounter/60;
    if (hour >= 24)
    {
        //temp = 0;
        hour = hour % 24;/*"更新小时"*/
        day = (req_tm->day);
        day++;
        req_tm->day = (day);
        //req_tm->Day++;
        /*
        nMaxDay = DaysOfMonth[(req_tm->month)-1]; 
         if((req_tm->year & 0x03) == 0  && req_tm->month == 0x02) 
         {
          nMaxDay++;
         }
         */
         nMaxDay = LIB_CalDay(req_tm->year, req_tm->month);
        if (req_tm->day > (nMaxDay))
        {
            req_tm->day = 0x01;
            month = (req_tm->month);
            month++;
            req_tm->month = (month);
            //req_tm->Month++;
            if(req_tm->month > 12)
            {
                req_tm->month = 0x01;
                year = (req_tm->year);
                year++;
                req_tm->year = (year);
                //req_tm->Year++;
            }
        }
    }
    //Byte2BCD(hour, &req_tm->Hour);
    #if   (APP_TIME_FRMT == HEX_FRMT)
    req_tm->hour = hour;
    #else
    req_tm->hour = LIB_CharToBcd(hour);
    #endif
    min = timeCounter%60;
    //Byte2BCD(min, &req_tm->Min);
    #if   (APP_TIME_FRMT == HEX_FRMT)
    req_tm->min = min;
    #else
    req_tm->min = LIB_CharToBcd(min);
    #endif
}




#if 0
void Lrm_test(void)
{
    INT8U i;
    INT64U data;
    INT64U sum;
    #if defined (THREE_PHASE)
    INT64U phase_eng;
    #endif

    sum = 0;
    for(i=0; i<TARIFF_NUM; i++)
    {
        data = 100000 + i*10000;
        sum += data;
        LIB_MemCpy((INT8U *)&data, (INT8U *)&stNowEngyBak.TariffEngy[i+1].ComP[0],sizeof(ST_ENERGY_FORM));
    }
    LIB_MemCpy((INT8U *)&sum, (INT8U *)&stNowEngyBak.TariffEngy[0].ComP[0],sizeof(ST_ENERGY_FORM));

    sum = 0;
    for(i=0; i<TARIFF_NUM; i++)
    {
        data = 200000 + i*10000;
        sum += data;
        LIB_MemCpy((INT8U *)&data, (INT8U *)&stNowEngyBak.TariffEngy[i+1].ComP[1],sizeof(ST_ENERGY_FORM));
    }
    LIB_MemCpy((INT8U *)&sum, (INT8U *)&stNowEngyBak.TariffEngy[0].ComP[1],sizeof(ST_ENERGY_FORM));
#if defined (THREE_PHASE)
    phase_eng = sum/3;
#endif
    for(i=0; i<MU_ELEMENT_NUM; i++)
    {
        #if defined (THREE_PHASE)
        LIB_MemCpy((INT8U *)&phase_eng, (INT8U *)&stNowEngyBak.Phase[i].ComP[0],sizeof(ST_ENERGY_FORM));
        #endif
        stMmtData.curr[0] = 49000 +i*1000;
    }
}


extern void MinToDateTime(INT32U Min, ST_6TIME_FORM *pDateTime);

void RCRD_test(void)
{
    ST_7TIME_FORM now;
    ST_7TIME_FORM bak;
    ST_6TIME_FORM tm;
    if(evt_test == 1)
    {
        if(mstNowTime.year == 0)
        {
            GetSingle(E_SYS_TIME, (INT8U *)&now);/*"取系统时间"*/  
            LIB_MemCpy((INT8U *)&now, (INT8U *)&mstNowTime, 6);
        }
        Lrm_test();
        LIB_MemCpy((INT8U *)&stSysTime, &bak.year, 7);
        LIB_MemCpy((INT8U *)&mstNowTime, &stSysTime.year, 6);
        mMinCnt = LIB_MToStdDate((ST_6TIME_FORM *)&mstNowTime);
        LR_Main();
        RCRD_Freezing1min();
        RCRD_Billing1min();
        MinToDateTime(mMinCnt, &tm);
        if(LIB_MemCmp((INT8U *)&tm, (INT8U *)&mstNowTime, 6) != EQUAL)
        {
            evt_test = 0;
        }
        time_add_minute(&mstNowTime, 15);
        LIB_MemCpy((INT8U *)&bak, &stSysTime.year, 7);
    }
}
#endif

extern void Host_Commu_Frz_Data_Push(void);

/*"************************************************************"*/
/*"Function:    RCRD_Task                                        "*/
/*"Description: 记录快照模块主处理函数                             "*/
/*"Input:       无                                             "*/
/*"Output:      无                                             "*/
/*"Return:      无                                             "*/
/*"Others:                                                     "*/
/*"************************************************************"*/
void RCRD_Task(void)
{
    ST_7TIME_FORM now;

    if(TRUE == Task_ChkTsk_1min(MAINTASK_EM) )
    {/*"分时分钟任务到"*/
        Host_Commu_Frz_Data_Push();
        GetSingle(E_SYS_TIME, (INT8U *)&now);/*"取系统时间"*/  
        LIB_MemCpy((INT8U *)&now, (INT8U *)&mstNowTime, 6);
        mstNowTime.sec = 0;/*"保证记录的冻结时间为0秒，防止698协议筛选记录失败"*/
        mMinCnt = LIB_MToStdDate((ST_6TIME_FORM *)&mstNowTime);
#if defined(THREE_PHASE)
        if((mMinCnt % 6) == 0)
        {
            upd_evt_amp_eeprom();
        }
#endif
        LR_Main();
        RCRD_Freezing1min();
#ifdef MAIN_METER
        RCRD_Billing1min(); 
#endif
    }
}


/*"*****************************************************************************"*/
/*"  Function:       setBillDay"*/
/*"  Description:    设置结算日期写前处理接口函数，检测结算日参数的合法性，记录结算日编程事件"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          pdata      输入参数指针"*/
/*"  Output          "*/
/*"  Return:         _TRUE 参数在允许范围内  _FALSE 参数在允许范围外"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
INT8S setBillDay(INT16U obis, INT8U *pdata, INT8U len)
{
    INT8U flg99, flg01;
    INT8U rsult;
    INT8U IId, cnt, i;
    ST_7TIME_FORM nowtime;
    INT8U off;
    INT8U code99;

    flg99 = NOT_EQUAL;
    rsult = TRUE;
    flg01 = 0;

    if((len == sizeof(ST_DAY_HOUR)*3) && (obis == E_MOUTH_BILLDAY))
    {
        cnt = 3;
        IId = 1;
        off = FIND(ST_RCRD_PARA_TABLE, monBillDay);
    }
    else if((len == sizeof(ST_DAY_HOUR)) && (obis >= E_MOUTH_BILLDAY1 && obis <= E_MOUTH_BILLDAY3))
    {
        cnt = 1;
        IId = obis - E_MOUTH_BILLDAY1 + 1;
        //off = (obis - E_MOUTH_BILLDAY1)*sizeof(ST_DAY_HOUR);
        off = FIND(ST_RCRD_PARA_TABLE, monBillDay[obis - E_MOUTH_BILLDAY1]);
    }
    else
    {
        return MS_OTHER_ERR;
    }

    #if   (APP_TIME_FRMT == HEX_FRMT)
    code99 = 99;
    #else
    code99 = 0x99;
    #endif

    for(i = 0; i < cnt; i++)
    {
        switch(IId)
        {
        case 0x01:
            flg01 = 0x55;
            break;
        case 0x02:
        case 0x03:
            flg99 = LIB_ChkValue(pdata + i * 2, 2, code99);/*"第2、3结算日可设置为9999"*/
            break;
        default:
             rsult = FALSE;
            break;
        }

        if(NOT_EQUAL == flg99)
        {
            if(((*(pdata + i * 2 + 0)) > 28) || ((*(pdata + i * 2 + 1)) > 23)
            || (0 == (*(pdata + i * 2 + 0))))
            {
                rsult = FALSE;
                break;
            }
        }

        IId += 1;
    }

    if(TRUE == rsult)
    {
        if(flg01 == 0x55)/*"设置第一结算日，需结算"*/
        {
            GetSingle(E_SYS_TIME, (INT8U *)&nowtime);/*"取系统时间"*/  
            RCRD_MonBillFrz(0,(INT8U*)&nowtime);
        }

        //RCRD_SaveData(E_BILL_DAY_PRG_EVNT_RCRD, E_EVTLST_BILL_DAY_PRG);/*"记录结算日编程事件"*/

        /*"关闭掉电检测中断"*/
        MCU_PwrChkCloseINT();
        
        rsult = FS_WriteFile(E_FILE_RCRD_PARA, RCRD_PARA_TBL_OFF+off, pdata, len);
        
        LIB_MemCpy(pdata, (INT8U *)&rcrd_para + off, len);
        rcrd_para.Crc = CRC16((INT8U *)&rcrd_para, FIND(ST_RCRD_PARA_TABLE, Crc));
        FS_WriteFile(E_FILE_RCRD_PARA, RCRD_PARA_TBL_CRC_OFF, (INT8U *)&rcrd_para.Crc, sizeof(CRC_DATA_TP));

        /*"开启掉电检测中断"*/
        MCU_PwrChkOpenINT();

    }
    if(TRUE == rsult)
    {
        return MS_SUCCESS;
    }
    else
    {
        return MS_OTHER_ERR;
    }
}

INT8S GetBillDate(INT16U id, INT8U *pBuff)
{
    INT8U off, len;
    
    if(id == E_MOUTH_BILLDAY)
    {
        //off = 0;
        off = FIND(ST_RCRD_PARA_TABLE, monBillDay);
        len = sizeof(ST_DAY_HOUR)*3;
    }
    else
    {
        //off = sizeof(ST_DAY_HOUR)*(id - E_MOUTH_BILLDAY1);
        off = FIND(ST_RCRD_PARA_TABLE, monBillDay[id - E_MOUTH_BILLDAY1]);
        len = sizeof(ST_DAY_HOUR);    
    }
    FS_ReadFile(E_FILE_RCRD_PARA, RCRD_PARA_TBL_OFF+off, pBuff, len);
    return len;
}

/*"*****************************************************************************"*/
/*"  Function:       ChkMntParaValid"*/
/*"  Description:    检查法定模块参数数据合法性，并从备份数据进行恢复处理"*/
/*"  Calls:          "*/
/*"  Called By:      初始化，set"*/
/*"  Input:          "*/
/*"  Output:         "*/
/*"  Return:         TRUE  参数合法  FALSE 参数非法"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U ChkRCRDParaValid(void)
{
    INT8U rsult;

    rsult = LIB_ChkDataValid((INT8U *)&rcrd_para, FIND(ST_RCRD_PARA_TABLE, Crc), rcrd_para.Crc);
    if(FALSE == rsult)
    {
        FS_ReadFile(E_FILE_RCRD_PARA, RCRD_PARA_TBL_OFF, (INT8U *)&rcrd_para, sizeof(ST_RCRD_PARA_TABLE));
        rsult = LIB_ChkDataValid((INT8U *)&rcrd_para, FIND(ST_RCRD_PARA_TABLE, Crc), rcrd_para.Crc);
        if(FALSE == rsult)
        {
        	set_default_other_para();
        }
    }

    return (rsult);
}

INT8U set_default_other_para(void)
{
    INT8U rsult;
    
    LIB_MemCpy((INT8U *)&DefMouthBillDay, (INT8U *)&rcrd_para, sizeof(ST_RCRD_PARA_TABLE));
    rcrd_para.Crc = CRC16((INT8U *)&rcrd_para, FIND(ST_RCRD_PARA_TABLE, Crc));
    rsult = FS_WriteFile(E_FILE_RCRD_PARA, RCRD_PARA_TBL_OFF, 
        (INT8U *)&rcrd_para, sizeof(ST_RCRD_PARA_TABLE));

    
    return rsult;
}

void RCRD_Clr_All_Run_Data(void)
{
    ST_RCRD_DATA_FILE data;

    LIB_MemSet(0x00, (INT8U *)&data, sizeof(ST_RCRD_DATA_FILE));
    FS_WriteFile(E_FILE_RCRD_DATA, 0, (INT8U *)&data, sizeof(ST_RCRD_DATA_FILE));

    LR_ClrRecord();

    LR_SetDefaultIdx();
}

/*"*****************************************************************************"*/
/*"  Function:       RCRD_Init"*/
/*"  Description:    初始化参数，上电补年月结算和补日冻结"*/
/*"  Calls:          FU_PwUpBill"*/
/*"  Called By:      SoftwareInit"*/
/*"  Input:          无 "*/
/*"  Output:         无 "*/
/*"  Return:         无"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void RCRD_Init(INT8U mode)
{
#ifdef MAIN_METER
    ST_7TIME_FORM now;
    ST_7TIME_FORM pdtime;
    INT16S res;
#endif
    BUILD_BUG_ON(MAX_ALL_RCRD!=RCRD_LIST_NUM);
    BUILD_BUG_ON(E_PWR_DOWN_EVNT_RCRD != 0x7000);
    //BUILD_BUG_ON(E_LD_ST_TIME !=  (E_LD_TYPE1_PERIOD + MIN_FRZ_CLASS_NUM));
    meter_type = GetMeterType();
    if(mode == MCU_LPMODE_DEEP)
    {
        return;
    }
    if(FIRST_PWR_ON == mode)
    {
        //SetDefaultRoad();
        set_default_other_para();
        RCRD_Clr_All_Run_Data();
    }
    
    ChkRCRDParaValid();
    
    LR_Init(mode);
#if defined(THREE_PHASE)
    poweron_init_evt_amp();
#endif
    Task_MainTaskRegister(MAINTASK_EM, RCRD_Task);

#ifdef MAIN_METER
    if((FIRST_PWR_ON != mode) && (ChkStatus(E_MTR_RST) != TRUE))
    {
        res = GetSingle(E_PD_TIME, (INT8U *)&pdtime);/*"从数据中心获取掉电时间"*/
        if(res < MS_SUCCESS)
        {
            return;
        }
        res = GetSingle(E_SYS_TIME, (INT8U *)&now);/*"取系统时间"*/  
        if(res < MS_SUCCESS)
        {
            return;
        }
        if((FALSE == LIB_ChkDateTime((ST_6TIME_FORM *)&pdtime)) 
            || (FALSE == LIB_ChkDateTime((ST_6TIME_FORM *)&now))
            || (LIB_MemCmp((INT8U *)&pdtime, (INT8U *)&now, 6) == BIG) )
        {
            return;
        }

        /*"上电依次补日冻结、补月结算、补年结算"*/
        Powerup_Day_Frz_Proc(&pdtime, &now);
        Powerup_Mouth_Bill_Frz_Proc(&pdtime, &now);
        Powerup_Step_Bill_Frz_Proc(&pdtime, &now);
    }
#endif
}


