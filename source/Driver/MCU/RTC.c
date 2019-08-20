/*"**********************************************************"*/		
/*"FileName: RTC.c"*/		
/*"Author: "*/      
/*"Version : 1.0       Date: 2017.01.05	"*/
/*"Description: 定时器，系统层"*/
/*"Function List: "*/		
/*"<FunctionName>	"*/	
/*"<author> <time> <version > <desc>"*/			
/*"**********************************************************"*/
#include "Driver\MCU\RTC.h"
#include "Driver\MCU\MCU.h"
#include "Lib\LIB_func.h"

volatile INT32U  gsecCnt;/*"秒中断累计数"*/
volatile INT8U gSecFlag ;
/****************对外接口**********************/
INT8U INRTC_Init(INT8U Mode);
INT8U INRTC_WriteTime(INT8U *t);
INT8U INRTC_ReadTime(INT8U *t);
void RTC_Enable(void);
void RTC_Disable(void);
void RTC_OpenSec(void);
void RTC_CloseSec(void);
/***************内部函数**********************/
static INT16U RTC_Get_Calendar(rtc_Calendar * const Calendar_val);
static INT16U RTC_Set_Calendar(rtc_Calendar Calendar_val);
static void RTC_SecAdd(void);
static void RTC_SecPd(void);

/*********************************************************/



/*"*****************************************************************************"*/
/*"  Function:       INRTC_Init"*/
/*"  Description:    内置时钟芯片初始化"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          Mode:INIT_MODE0_INRTC,正常初始化流程，只在芯片彻底掉电Ram乱掉时才初始化RTC配置"*/
/*"                             INIT_MODE1_INRTC,RTC出错等异常状态初始化；会重新配置RTC控制寄存器值"*/
/*"  Output          "*/
/*"  Return:         TRUE      初始化成功       FALSE    初始化失败"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U INRTC_Init(INT8U Mode)
{
   
    gsecCnt = 0;
    gSecFlag = FALSE;
    if(Mode == MCU_RUNMODE)
    {
        RTC->RTCIF = 0xFFFF;/*清除中断标志*/
        RTC->RTCIE = 0x0008;/*开启秒中断*/
        RTC->FSEL = 0x0000;/*输出精确1秒方波，需要开启PLL*/
        RTC->PR1SEN = 0x0001;/*使能虚拟调教*/
        INT_VectSet(E_INT_ID_RTC, RTC_SecAdd);
        INT_IntPrioSet(E_INT_ID_RTC, E_INT_PRIO_LEVEL_3);
        INT_En(E_INT_ID_RTC);
    }
    else
    {
        RTC->RTCIF = 0xFFFF;/*清除中断标志*/
        RTC->RTCIE = 0x0008;/*开启秒中断*/
        INT_VectSet(E_INT_ID_RTC, RTC_SecPd);
        INT_IntPrioSet(E_INT_ID_RTC, E_INT_PRIO_LEVEL_3);
        INT_En(E_INT_ID_RTC);

    }
    return (TRUE);
}


void RTC_Enable(void)
{
  
}


void RTC_Disable(void)
{
   
}

/***********************************************************************************************************************
* Function Name: R_RTC_Get_CalendarCounterValue
* Description  : This function reads the results of real-time clock and store them in the variables.
* Arguments    : counter_read_val -
*                    the expected real-time clock value(BCD code)
* Return Value : status -
*                    MD_OK or MD_ERROR
***********************************************************************************************************************/
static INT16U RTC_Get_Calendar(rtc_Calendar * const Calendar_val)
{
    Calendar_val->year = RTC->BCDYEAR;
    Calendar_val->month = RTC->BCDMONTH;
    Calendar_val->day = RTC->BCDDATE;
    Calendar_val->week = RTC->BCDWEEK;	
    Calendar_val->hour = RTC->BCDHOUR;
    Calendar_val->min = RTC->BCDMIN;
    Calendar_val->sec = RTC->BCDSEC;
    return TRUE;
}

/*"*********************************************************"*/
/*"Function:   RTC_Read    "*/
/*"Description: Read Real Time"*/
/*"Author: chenkai"*/
/*"Input: "*/
/*"Output:"*/ 
/*"Return: "*/
/*"*********************************************************"*/
INT8U INRTC_ReadTime(INT8U *t)
{ 
    INT8U i,rValue;
    INT8U t1[DATE_NUM];
    rtc_Calendar rtctime;

    rValue = FALSE;
    for (i = 0; i< RTC_RD_CNT; i++)
    {
        RTC_Get_Calendar(&rtctime);
        t[YEAR] = (INT8U)(rtctime.year & 0x00FF);
        t[MON] = rtctime.month;
        t[DAY] = rtctime.day;
        t[HOUR] = (INT8U)(rtctime.hour & 0x3F);
        t[MINU] = rtctime.min;
        t[SEC] = rtctime.sec;
        t[WEEK] = rtctime.week;
        RTC_Get_Calendar(&rtctime);
        t1[YEAR] = (INT8U)(rtctime.year & 0x00FF);
        t1[MON] = rtctime.month;
        t1[DAY] = rtctime.day;
        t1[HOUR] = (INT8U)(rtctime.hour & 0x3F);
        t1[MINU] = rtctime.min;
        t1[SEC] = rtctime.sec;
        t1[WEEK] = rtctime.week;
        if (LIB_MemCmp(t, t1, DATE_NUM) == EQUAL)
        {
            rValue = TRUE;
            break;
        }	
    }

    if (t[WEEK] == 0x07)  /*"周日用'0'表示"*/
    {
        t[WEEK] = 0x00; 
    }	
    return rValue; 
}

/***********************************************************************************************************************
* Function Name: R_RTC_Set_CalendarCounterValue
* Description  : This function changes the calendar real-time clock value.
* Arguments    : counter_write_val -
*                    the expected real-time clock value(BCD code)
* Return Value : status -
*                
***********************************************************************************************************************/
#define RTC_WRITR_PROTECT_WORD                          ((uint32_t)0x53535353)
#define RTC_WRITR_UNPROTECT_WORD                        ((uint32_t)0xACACACAC)
static INT16U RTC_Set_Calendar(rtc_Calendar Calendar_val)
{
    RTC->RTCWE = RTC_WRITR_UNPROTECT_WORD;

    RTC->BCDYEAR = Calendar_val.year;
    RTC->BCDDATE = Calendar_val.day;
    RTC->BCDMONTH = Calendar_val.month;
    RTC->BCDWEEK = Calendar_val.week;	
    RTC->BCDHOUR = Calendar_val.hour;
    RTC->BCDMIN = Calendar_val.min;
    RTC->BCDSEC = Calendar_val.sec;

    RTC->RTCWE = RTC_WRITR_PROTECT_WORD;

    return  TRUE;
}

/*"*********************************************************"*/
/*"Function:   RTC_Write"*/   
/*"Description: Write Real Time"*/
/*"Author: chenkai"*/
/*"Input:"*/ 
/*"Output:"*/ 
/*"Return:"*/ 
/*"*********************************************************"*/
INT8U INRTC_WriteTime(INT8U *t)
{
    INT8U rValue = FALSE;
    INT8U t1[DATE_NUM];
    rtc_Calendar rtctime;

    LIB_MemCpy(&t[0], &t1[0], DATE_NUM);

    if (t1[WEEK] == 0x07) /*"'0'表示周日"*/
    {
        t1[WEEK] = 0x00; 
    }
    if(t1[HOUR] > 0x11)
        t1[HOUR] += 0x40;

    /* Set counter values */
    rtctime.year = (INT16U) t1[YEAR] &0x00FF;
    rtctime.month = t1[MON] ;
    rtctime.day = t1[DAY];
    rtctime.hour = t1[HOUR];
    rtctime.min = t1[MINU];
    rtctime.sec = t1[SEC];
    rtctime.week = t1[WEEK];
    rValue = RTC_Set_Calendar(rtctime);
    return rValue;
}

void Enable_RTC_1HZ(void)
{
	GPIO_ConfigPin(RTC_EN, OUTPUT, NORMAL_OUT);
	GPIO_SetMulti(RTC_EN, GPIO_Mode_OUT);
	GPIO_OutputPin(RTC_EN,LOW);

	//GPIO_ConfigPin(RTC_1HZ, OUTPUT, NORMAL_OUT);
	//GPIO_OutputPin(RTC_1HZ, HIGH);
	GPIO_SetMulti(RTC_1HZ, GPIO_Mode_DIG);//mcu output second pluse
	
}

void Disable_RTC_1HZ(void)
{
	GPIO_ConfigPin(RTC_EN, OUTPUT, NORMAL_OUT);
	GPIO_SetMulti(RTC_EN, GPIO_Mode_OUT);
	GPIO_OutputPin(RTC_EN,HIGH);

	GPIO_ConfigPin(RTC_1HZ, OUTPUT, NORMAL_OUT);//mcu output low
	GPIO_OutputPin(RTC_1HZ, LOW);
	//close_exrtc_sec();
}

/*"*********************************************************"*/
/*"Function:   RTC_OpenSec    "*/
/*"Description: Open Second Pulse"*/
/*"Author: "*/
/*"Input: "*/
/*"Output:"*/ 
/*"Return: "*/
/*"*********************************************************"*/
void RTC_OpenSec(void)
{ 
	Enable_RTC_1HZ();
	
	
	//Disable_RTC_1HZ();
	#if 0
	if(mode==0)//inRTC
	{
    	GPIO_SetMulti(RTC_1HZ, GPIO_Mode_DIG);
	}
	else if(mode==1)//exRTC
	{
		GPIO_ConfigPin(RTC_1HZ, INPUT, HIGH_IMPED_IN);
    	//GPIO_OutputPin(RTC_1HZ_OUT, LOW);
	}
	#endif
			
}
/*"*********************************************************"*/
/*"Function:   RTC_CloseSec    "*/
/*"Description: Close Second Pulse"*/
/*"Author: "*/
/*"Input: "*/
/*"Output:"*/ 
/*"Return: "*/
/*"*********************************************************"*/
void RTC_CloseSec(void)
{
	Disable_RTC_1HZ();
	#if 0
	GPIO_ConfigPin(RTC_1HZ, INPUT, HIGH_IMPED_IN);
	#endif
}

static void RTC_SecAdd(void)
{
    gsecCnt++;
}

static void RTC_SecPd(void)
{
    gsecCnt++;
    gSecFlag = TRUE;
}

