/*******************************************************************************
FILE NAME  	: BaseDef.C
DESCRIPTION	: Base function code

Copyright   : 2010  Wasion Group Ltd.
*******************************************************************************/

/*******************************************************************************
Revision History
DD.MM.YYYY   Description                                        Modefied By
13.10.2010   First Release                                        Yuhao.cui
13.04.2011   修改了时间类型转换函数中的 bcd及Hex之间的转换        Yuhao.cui

*******************************************************************************/
#include "BaseDef.h"
#include "Lib\LIB_func.h"


//#include "Display_task.h"
//#include "Class01_Data.h"
//#include "Class03_Register.h"
//#include "app_math.h"
//#include "AppTaskCOSEM.h"
//#include "AppTaskLoopSecFun.h"
//#include "IEC62056.h"

/* float format 
The single format is:
widths : 1     8            23
des    : s     e            f
order  : msb                lsb

where:
- s is the sign bit
- e is the exponent; it is 8 bits wide and the exponent bias is + 127
- f is the fraction, it is 23 bits

with this, the value is (if 0 < e < 255):
		v = (-1)^s * 2 ^(e-127) * (1.f)

example: the coding of the decimal value "1" including the tag of data
type is(all value are hexadeciaml) : 17 3f 80 00 00
*/
//#define FLOAT_ENCODE_MODE_INVERED

//MeteringPortType MeterPort;    /* 实例化计量接口                */


/************************************************************************* 
                属性权限的枚举列表   
*************************************************************************/
//#pragma  sectaddress  _RighList_FE, ROMDATA  0xD9500
//#pragma section  rom  _RighList
const Right_Type  AttriRighList[] =   /* 最高级别的权限固定为 可读写 */
{  //PC  FC PRCC MC  FT  
    {NO, NO, NO, NO, WR},   // 0
    {NO, NO, NO, RO, WR},   // 1
    {NO, NO, NO, WR, WR},   // 2
    {NO, NO, RO, RO, WR},   // 3
    {NO, NO, RO, WR, WR},   // 4
    {NO, NO, WR, WR, WR},   // 5
    {NO, RO, RO, RO, WR},   // 6
    {NO, RO, RO, WR, WR},   // 7
    {NO, RO, WR, WR, WR},   // 8
    {NO, WR, WR, WR, WR},    // 9
	{RO, RO, RO, RO, WR},   // 10   added  6.8  
    {RO, RO, RO, WR, WR},   // 11   modified by yy 11.09.28
	{NO, WO, WO, WO, WR},   //12
	{NO, NO, WO, WO, WR},   //13
	{NO, NO, NO, WO, WR},   //14
	{RO, RO, WR, WR, WR},   // 15 added by lin 20120622
	//---------------------------
	{RO, NO, NO, RO, WR},//{RO, NO, RO, RO, WR},	//16 add by slx
	{NO, WO, NO, WO, WR},	//17 add by slx
	{RO, NO, NO, RO, WR},	//18 add by slx
	{NO, RO, NO, RO, WR},//{NO, RO, NO, NO, WR},	//19 add by slx
	{NO, WO, NO, NO, WR},	//20 add by slx
	{RO, NO, RO, WR, WR},   //21 add by SLX 
	{NO, WR, NO, WR, WR},//{NO, WR, NO, NO, WR},   //22
	{RO, NO, NO, WR, WR},//23
};	
const Right_Type  MethodRighList[] =   /* 最高级别的权限固定为 可读写 */
{
    {NO, NO, NO, NO, YES},   // 0
    {NO, NO, NO, YES, YES},   // 1
    {NO, NO, YES, YES, YES},   // 2
    {NO, YES, YES, YES, YES},   // 3
	{YES, YES, YES, YES, YES},   // 4     
	{NO, NO, YES, NO, YES},   //5
	{NO, YES, YES, NO, YES},   //6
	{YES, YES, YES, NO, YES},   //7
};
/************************************************************************* 
              IEC62056 时间类型 定义及转换函数     
*************************************************************************/
/* 计算星期 几 */
//#pragma SECTION program programD_Basedef_CalculateWeek
INT8U CalculateWeek(INT16U y, INT8U m, INT8U d)
{
	if((m == 1) || (m == 2))
	{
	     m += 12;
	     y -= 1;
	}
	// 基姆拉尔森计算公式 W= (d+2*m+3*(m+1)/5+y+y/4-y/100+y/400) mod 7
	// 在公式中d表示日期中的日数，m表示月份数，y表示年数。  算出的为 星期1到星期7
	return ((d+2*m+3*(m+1)/5+y+y/4-y/100+y/400) % 7 )+ 1;
}


// TimeTypeToHMS
//#pragma SECTION program programD_Basedef_TimeTypeToHMS
void  TimeTypeToHMS(INT8U * Time , INT8U* HMS)   
{
	TimeType * Tm = (TimeType*)Time;
	HMS[0] = Tm->Time.Hour;   /* HH */
	HMS[1] = Tm->Time.Min;    /* MM */
	HMS[2] = Tm->Time.Sec;    /* SS */
}

// HMSToTimeType
//#pragma SECTION program programD_Basedef_HMSToTimeType
void  HMSToTimeType (INT8U* HMS , INT8U * Time )
{
	TimeType * Tm = (TimeType*)Time;
	Tm->Time.Hour = HMS[0];   /* HH */
	Tm->Time.Min = HMS[1];    /* MM */
	Tm->Time.Sec = HMS[2];    /* SS */
	Tm->Time.HunredSecS = 0;  /* hundred */
}

// YMDToDateType
//#pragma SECTION program programD_Basedef_YMDToDateType
void  YMDToDateType(INT8U* YMD , INT8U * Date)
{
	DateType * DateT = (DateType *)Date;
	
	if(YMD[0] != 0xFF)
	{
	  DateT->Date.Year_H = (YMD[0] + 2000) >> 8;
	  DateT->Date.Year_L = (YMD[0] + 2000) & 0x00FF;
	  DateT->Date.Week = CalculateWeek(YMD[0] + 2000, YMD[1], YMD[2]);
	}
	else
	{
	  DateT->Date.Year_H = 0xFF;
	  DateT->Date.Year_L = 0xFF;
	  DateT->Date.Week = 0xFF;
	}
	DateT->Date.Month = YMD[1];
	DateT->Date.Day = YMD[2];
}

//DateTypeToYMD
//#pragma SECTION program programD_Basedef_DateTypeToYMD
void  DateTypeToYMD (INT8U* Date , INT8U * YMD )
{
	DateType * DateT = (DateType *)Date;
	if((DateT->Date.Year_H != 0xFF) && (DateT->Date.Year_L != 0xFF))
	{
	  YMD[0] = (DateT->Date.Year_H * 256L + DateT->Date.Year_L - 2000);
        }
        else
        {
          YMD[0] = 0xFF;
        }
	YMD[1] = DateT->Date.Month;
	YMD[2] = DateT->Date.Day;
	YMD[3] = DateT->Date.Week;
}

// DateTimeTypeToYMDHMS
//#pragma SECTION program programD_Basedef_DateTimeTypeToYMDHMS
void  DateTimeTypeToYMDHMS (INT8U* DateTime, INT8U * YMDHMS )
{
	DateTimeType * DataTm = (DateTimeType *)DateTime;
	
	DateTypeToYMD((INT8U *)&(DataTm->DateTime.Date),YMDHMS);
	TimeTypeToHMS((INT8U *)&(DataTm->DateTime.Time),&YMDHMS[4]);
}

#if 0
void  YMDHMSToDateTimeType (INT8U* YMDHMS, INT8U * DateTime)
{
	YMDToDateType(&YMDHMS[0], DateTime);
	HMSToTimeType(&YMDHMS[4], &DateTime[5]);
    EEPROMRead((INT8U *)&DateTime[9],CLOCK_STATUS_ADDR,3);     //读取在写clock时存储Dev_h,Dev_L,Clock 
}

void  YMDHMSToDateTimeType (INT8U* Deviation, INT8U* YMDHMS, INT8U * DateTime)
{
    YMDToDateType(&YMDHMS[0], DateTime);
    HMSToTimeType(&YMDHMS[4], &DateTime[5]);
    LIB_MemCpy(Deviation,(INT8U *)&DateTime[9],3);     //读取在写clock时存储Dev_h,Dev_L,Clock 
}
#endif
/* 定义一个指针数组， 通过指针数组来访问时间类型转换函数 */
//#pragma  sectaddress  _TimeTypeChg_FE, ROMDATA  0xD9550
//#pragma section  rom  _TimeTypeChg
const TypeChange TimeTypeChg[] = 
{
   TimeTypeToHMS,
   HMSToTimeType,
   YMDToDateType,
   DateTypeToYMD,
   DateTimeTypeToYMDHMS,
//   YMDHMSToDateTimeType
};


/* define one array of unit_scaler */
#define UnitNum   197
//#pragma  sectaddress  _UnitSList_FE, ROMDATA  0xD9570
//#pragma section  rom  _UnitSList
const UnitScaler_Type  UnitSList[UnitNum] = 
{
    {None_Unit, 0},
 /* 功率：有功, 无功,视在 1~ 27 */  
    {U_W, 4},   {U_W, 3},   {U_W, 2},   {U_W, 1},   {U_W, 0},   {U_W, -1},   {U_W, -2},   {U_W, -3},   {U_W, -4},    // 有功功率    
    {U_Var, 4}, {U_Var, 3}, {U_Var, 2}, {U_Var, 1}, {U_Var, 0}, {U_Var, -1}, {U_Var, -2}, {U_Var, -3}, {U_Var, -4},  // 无功功率 
    {U_VA, 4},  {U_VA, 3},  {U_VA, 2},  {U_VA, 1},  {U_VA, 0},  {U_VA, -1},  {U_VA, -2},  {U_VA, -3},  {U_VA, -4},   // 视在功率 
 /* 电量:  有功, 无功,视在 28~54 */   
    {U_Wh, 4},  {U_Wh,  3}, {U_Wh,   2},{U_Wh, 1},  {U_Wh, 0},  {U_Wh, -1},  {U_Wh, -2},  {U_Wh, -3},  {U_Wh, -4},   // 有功电量 
    {U_Varh, 4},{U_Varh, 3},{U_Varh, 2},{U_Varh, 1},{U_Varh, 0},{U_Varh, -1},{U_Varh, -2},{U_Varh, -3},{U_Varh, -4}, // 无功电量 
    {U_VAh, 4}, {U_VAh, 3}, {U_VAh, 2}, {U_VAh, 1}, {U_VAh, 0}, {U_VAh, -1}, {U_VAh, -2}, {U_VAh, -3}, {U_VAh, -4},  // 视在电量 

  /* 金额:  当地货币 55~69 */          
    {U_Currency, 7},   {U_Currency, 6},   {U_Currency, 5},   {U_Currency, 4},    {U_Currency, 3},   {U_Currency, 2},   {U_Currency, 1},   // 1   
    {U_Currency, 0}, {U_Currency, -1}, {U_Currency, -2}, {U_Currency, -3}, {U_Currency, -4}, {U_Currency, -5}, {U_Currency, -6}, {U_Currency, -7},   // -7   
 /* 电压 V:    70~74 */               
    {U_V, 2},    {U_V, 1},   {U_V, 0},   {U_V, -1},   {U_V, -3},  
 /* 电流 A    75~83 */               
    {U_A, 4},   {U_A, 3},  {U_A, 2}, {U_A, 1},  {U_A, 0},   {U_A, -1},   {U_A, -2},  {U_A, -3}, {U_A, -4},  
 /* 功率因数                */
    {None_Unit, 4},   // 84   
    {None_Unit, 3},   // 85   
    {None_Unit, 2},   // 86   
    {None_Unit, 1},   // 87   
    {None_Unit, 0},   // 88   
    {None_Unit, -1},  // 89   
    {None_Unit, -2},  // 90    
    {None_Unit, -3},  // 91   
    {None_Unit, -4},  // 92    
 /* 角度fai                 */
    {U_Degree, 4},    // 93   
    {U_Degree, 3},    // 94   
    {U_Degree, 2},    // 95   
    {U_Degree, 1},    // 96   
    {U_Degree, 0},    // 97   
    {U_Degree, -1},   // 98   
    {U_Degree, -2},   // 99    
    {U_Degree, -3},   // 100   
    {U_Degree, -4},   // 101  
 /* 频率 hz               */
    {U_Hz, 2},        // 102  
    {U_Hz, 1},        // 103 
    {U_Hz, 0},        // 104  
    {U_Hz, -1},       // 105    
    {U_Hz, -2},       // 106 
    {U_Hz, -3},       // 107 
  /* s , m  , h 时间      */
    {U_S, 4},         // 108 
    {U_S, 3},         // 109 
    {U_S, 2},         // 110 
    {U_S, 1},         // 111 
    {U_S, 0},         // 112 
    {U_S, -1},        // 113 
    {U_S, -2},        // 114 
    {U_S, -3},        // 115 
    {U_S, -4},        // 116 
    {U_Min, 2},       // 117 
    {U_Min, 1},       // 118 
    {U_Min, 0},       // 119 
    {U_Min, -1},      // 120 
    {U_Min, -2},      // 121 
    {U_h, 2},         // 122 
    {U_h, 1},         // 123 
    {U_h, 0},         // 124 
    {U_h, -1},        // 125 
    {U_h, -2},        // 126 

  /* 温度 */            
    {U_Temper, 2},         // 127 
    {U_Temper, 1},         // 128 
    {U_Temper, 0},         // 129 
    {U_Temper, -1},        // 130 
    {U_Temper, -2},        // 131 

  // A2h, V2h           
    {U_A2h, 4},       // 132   
    {U_A2h, 3},       // 133   
    {U_A2h, 2},       // 134   
    {U_A2h, 1},       // 135   
    {U_A2h, 0},       // 136   
    {U_A2h, -1},      // 137   
    {U_A2h, -2},      // 138    
    {U_A2h, -3},      // 139   
    {U_A2h, -4},      // 140      

    {U_V2h, 4},       // 141  
    {U_V2h, 3},       // 142  
    {U_V2h, 2},       // 143   
    {U_V2h, 1},       // 144  
    {U_V2h, 0},       // 145  
    {U_V2h, -1},      // 146   
    {U_V2h, -2},      // 147    
    {U_V2h, -3},      // 148   
    {U_V2h, -4},      // 149  
      
 /* 体积 */              
    {U_M3, 4},        // 150   
    {U_M3, 3},        // 151   
    {U_M3, 2},        // 152   
    {U_M3, 1},        // 153   
    {U_M3, 0},        // 154   
    {U_M3, -1},       // 155   
    {U_M3, -2},       // 156    
    {U_M3, -3},       // 157   
    {U_M3, -4},       // 158   
 
 /*  流量 m3/h  m3/d  m3/s  */
    {U_M3h, 2},       // 159   
    {U_M3h, 1},       // 160   
    {U_M3h, 0},       // 161  
    {U_M3h, -1},      // 162  
    {U_M3h, -2},      // 163      

    {U_M3d, 2},       // 164   
    {U_M3d, 1},       // 165  
    {U_M3d, 0},       // 166   
    {U_M3d, -1},      // 167   
    {U_M3d, -2},      // 168  
 
 /* 热表               */
    {U_J, 2},         // 169   
    {U_J, 1},         // 170   
    {U_J, 0},         // 171   
    {U_J, -1},        // 172   
    {U_J, -2},        // 173      

    {U_Jh, 2},        // 174   
    {U_Jh, 1},        // 175 
    {U_Jh, 0},        // 176 
    {U_Jh, -1},       // 177  
    {U_Jh, -2},       // 178   
    
  //  count           
    {U_Count, 4},     // 179 
    {U_Count, 3},     // 180 
    {U_Count, 2},     // 181 
    {U_Count, 1},     // 182 
    {U_Count, 0},     // 183 
    {U_Count, -1},    // 184  
    {U_Count, -2},    // 185   
    {U_Count, -3},    // 186  
    {U_Count, -4},     // 187  
  // percentage %
    {U_Percentage,0} ,  //188
	
    {U_Per_Wh,  -5},   //189/*  1/wh  */
    {U_Per_Varh,-5},   //190/*  1/varh  */
    {U_Size_KByte ,0}, //191
	
  /* day 时间      */
    {U_Day, 2},         // 192 
    {U_Day, 1},         // 193 
    {U_Day, 0},         // 194 
    {U_Day, -1},        // 195 
    {U_Day, -2},        // 196 
};



/* 	Set_UnitScaler, return the new index in the unitscaler list, if retrun 0xFF, means set fault*/
//#pragma SECTION program programD_Basedef_Set_UnitScaler
INT8U Set_UnitScaler(UnitScaler_Type * NewUnitScaler)
{
	INT8U Cnt;
	for(Cnt = 0; Cnt < UnitNum; Cnt++)
	{
		if((UnitSList[Cnt].Unit == NewUnitScaler->Unit)
		          && (UnitSList[Cnt].Scaler == NewUnitScaler->Scaler))
	    {
			return Cnt;
		}
	}
	return 0xFF;
}

/*	Get_UnitScaler */
//#pragma SECTION program programD_Basedef_Get_UnitScaler
BOOL Get_UnitScaler(INT8U Index, UnitScaler_Type * ResultUnitScaler)
{
	/* 索引越界 */
	if(Index >= UnitNum)
	   return false;
	
	ResultUnitScaler->Unit = UnitSList[Index].Unit;
	ResultUnitScaler->Scaler = UnitSList[Index].Scaler;
	return true;
}

/************************************************************************* 
             各个接口类任务ID列表
*************************************************************************/


/************************************************************************* 
        IEC62056 基本数据类型的编码函数    统一返回编码报文的长度 
*************************************************************************/
// DeCode Lenth of Data
//#pragma SECTION program programD_Basedef_DeCodeLength
BOOL DeCodeLength(INT8U *Offset, INT16U *Length, INT8U *lpSrc)
{
	INT8U OctetNum;
	INT8U i;
	
	*Length = 0;
	
	if(((*lpSrc) & 0x80) > 0)//length long code
	{		
		OctetNum = (*lpSrc++) & 0x7F;	
		if(OctetNum > 4)
		{
			return FALSE;
		}	

		for(i = 0; i < OctetNum; i++)
		{
			*Length = (*Length << 8) + *lpSrc++;
		}	
		*Offset = (OctetNum + 1);
		
	}
	else//length short code
	{
		*Length = *lpSrc;
		*Offset = 1;
	}
	return TRUE;
}
//#pragma SECTION program programD_Basedef_EnNull
INT16U EnNull(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)  //modified by lin 20121107
{
//	while(DataLenth--)
      *EnStartAddr= 0;
	return 1;
}

//#pragma SECTION program programD_Basedef_EnConstNull
INT16U EnConstNull(INT8U * EnStartAddr,  INT16U DataLenth)
{
	
	while(DataLenth--)
      *EnStartAddr++= 0;
	return 1;
	
}
//#pragma SECTION program programD_Basedef_EnSelectConstNull
INT16U EnSelectConstNull(INT8U * EnStartAddr,  INT16U DataLenth)
{
    //  *EnStartAddr++= 0x01;
	  *EnStartAddr++= 0x01;
	  *EnStartAddr++= 0x02;
	  *EnStartAddr++= 0x0F;
	  *EnStartAddr++= 0x01;
	  *EnStartAddr++= 0x0F;
	  *EnStartAddr++= 0x02;
	  
	return DataLenth;	
}

// EncodeLenth
//#pragma SECTION program programD_Basedef_EncodeLenth
INT16U EncodeLenth(INT8U * EnStartAddr,  INT16U DataLenth)
{
    INT8U *lptmp = EnStartAddr;
    if(DataLenth <= 127)
    {
	    *lptmp++ = DataLenth; 
    }
    else if(DataLenth > 255)
    {
		*lptmp++ = 0x82;
		*lptmp++ = DataLenth >> 8;
	    *lptmp++ = DataLenth & 0x00FF;    
	}
	else
	{
	    *lptmp++ = 0x81;
	    *lptmp++ = DataLenth & 0x00FF;   	
	}  
	
    return lptmp - EnStartAddr;
}

// EnArray
//#pragma SECTION program programD_Basedef_EnArray
INT16U EnArray(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)
{
    INT16U TempData;
    LIB_MemCpy(Data, (INT8U *)&TempData, DataLenth);
    *EnStartAddr = 0x01; 
    return EncodeLenth(EnStartAddr+1, DataLenth) + 1;
}

// EnStruct
//#pragma SECTION program programD_Basedef_EnStruct
INT16U EnStruct(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)
{
	 INT16U TempData;
	 LIB_MemCpy(Data, (INT8U *)&TempData, DataLenth);
     *EnStartAddr = 0x02;
	 
     return EncodeLenth(EnStartAddr+1, DataLenth) + 1;	
}

// EnBOOL
//#pragma SECTION program programD_Basedef_EnBOOL
INT16U EnBOOL(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)
{   
	INT16U TempData;
	INT8U *lptmp = EnStartAddr;
	LIB_MemCpy(Data, (INT8U *)&TempData, DataLenth);
	
    *lptmp++=0x03;
	if(*Data==FALSE)
	{
		*lptmp++=0x00;
	}
	else
	{
	    *lptmp++=0xFF;
	}
	return lptmp - EnStartAddr;
}

// EnBitStr   added by sdh  5.6
//#pragma SECTION program programD_Basedef_EnBitStr
INT16U EnBitStr(INT8U * EnStartAddr, INT8U *Data, INT16U BitStringLenth)
{	
	INT16U DataLength;
	INT8U *lp = EnStartAddr;
	*EnStartAddr++ = 0x04;
	EnStartAddr += EncodeLenth(EnStartAddr,  BitStringLenth);
	
	DataLength = (BitStringLenth / 8);
	if((BitStringLenth % 8) == 0) 
	{
		LIB_MemCpy(Data, EnStartAddr, DataLength);
        EnStartAddr += DataLength;
	}
	else
	{
		DataLength += 1;
		LIB_MemCpy(Data, EnStartAddr, DataLength-1);
        EnStartAddr += DataLength - 1;
		*EnStartAddr++ = (Data[DataLength-1]<<(8-(BitStringLenth % 8)));
	}
	return EnStartAddr - lp;
}

// EnINT
//#pragma SECTION program programD_Basedef_EnINT
INT16U EnINT(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)
{
	switch(DataLenth)
	{
	case 1:  EnStartAddr[0] = 15;
	         EnStartAddr[1] = Data[0];
	        return 2;
	case 2:  EnStartAddr[0] = 16;
	         EnStartAddr[1] = Data[1];
			 EnStartAddr[2] = Data[0];
	        return 3;
	case 4:  EnStartAddr[0] = 5;
	         EnStartAddr[1] = Data[3];
			 EnStartAddr[2] = Data[2];
			 EnStartAddr[3] = Data[1];
			 EnStartAddr[4] = Data[0];
	       return 5;
	case 8:  EnStartAddr[0] = 20;
	         EnStartAddr[1] = Data[7];
			 EnStartAddr[2] = Data[6];
			 EnStartAddr[3] = Data[5];
			 EnStartAddr[4] = Data[4];
			 EnStartAddr[5] = Data[3];
			 EnStartAddr[6] = Data[2];
			 EnStartAddr[7] = Data[1];
			 EnStartAddr[8] = Data[0];
	       return 9;
	default: return 0;
	}
//	return 0;
}



// EnUnsgned
//#pragma SECTION program programD_Basedef_EnUnsgned
INT16U EnUnsgned(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)
{
	switch(DataLenth)
	{
	case 1:  EnStartAddr[0] = 17;
	         EnStartAddr[1] = Data[0];
	        return 2;
	case 2:  EnStartAddr[0] = 18;
	         EnStartAddr[1] = Data[1];
			 EnStartAddr[2] = Data[0];
	        return 3;
	case 4:  EnStartAddr[0] = 6;
	         EnStartAddr[1] = Data[3];
			 EnStartAddr[2] = Data[2];
			 EnStartAddr[3] = Data[1];
			 EnStartAddr[4] = Data[0];
	       return 5;
	case 8:  EnStartAddr[0] = 21;
	         EnStartAddr[1] = Data[7];
			 EnStartAddr[2] = Data[6];
			 EnStartAddr[3] = Data[5];
			 EnStartAddr[4] = Data[4];
			 EnStartAddr[5] = Data[3];
			 EnStartAddr[6] = Data[2];
			 EnStartAddr[7] = Data[1];
			 EnStartAddr[8] = Data[0];
	       return 9;
	default: return 0;
	}
	//return 0;	
}

INT16U EnConstINT(INT8U * EnStartAddr, const INT8U *Data, INT16U DataLenth)
{
	switch(DataLenth)
	{
	case 1:  EnStartAddr[0] = 15;
	         EnStartAddr[1] = Data[0];
	        return 2;
	case 2:  EnStartAddr[0] = 16;
	         EnStartAddr[1] = Data[1];
			 EnStartAddr[2] = Data[0];
	        return 3;
	case 4:  EnStartAddr[0] = 5;
	         EnStartAddr[1] = Data[3];
			 EnStartAddr[2] = Data[2];
			 EnStartAddr[3] = Data[1];
			 EnStartAddr[4] = Data[0];
	       return 5;
	case 8:  EnStartAddr[0] = 20;
	         EnStartAddr[1] = Data[7];
			 EnStartAddr[2] = Data[6];
			 EnStartAddr[3] = Data[5];
			 EnStartAddr[4] = Data[4];
			 EnStartAddr[5] = Data[3];
			 EnStartAddr[6] = Data[2];
			 EnStartAddr[7] = Data[1];
			 EnStartAddr[8] = Data[0];
	       return 9;
	default: return 0;
	}
	//return 0;
}

// EnConstUnsgned
//#pragma SECTION program programD_Basedef_EnConstUnsgned
INT16U EnConstUnsgned(INT8U * EnStartAddr, const INT8U *Data, INT16U DataLenth)
{
	switch(DataLenth)
	{
	case 1:  EnStartAddr[0] = 17;
	         EnStartAddr[1] = Data[0];
	        return 2;
	case 2:  EnStartAddr[0] = 18;
	         EnStartAddr[1] = Data[1];
			 EnStartAddr[2] = Data[0];
	        return 3;
	case 4:  EnStartAddr[0] = 6;
	         EnStartAddr[1] = Data[3];
			 EnStartAddr[2] = Data[2];
			 EnStartAddr[3] = Data[1];
			 EnStartAddr[4] = Data[0];
	       return 5;
	case 8:  EnStartAddr[0] = 21;
	         EnStartAddr[1] = Data[7];
			 EnStartAddr[2] = Data[6];
			 EnStartAddr[3] = Data[5];
			 EnStartAddr[4] = Data[4];
			 EnStartAddr[5] = Data[3];
			 EnStartAddr[6] = Data[2];
			 EnStartAddr[7] = Data[1];
			 EnStartAddr[8] = Data[0];
	       return 9;
	default: return 0;
	}
	//return 0;	
}




// EnOctStr
//#pragma SECTION program programD_Basedef_EnOctStr
INT16U EnOctStr(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth) 
{
	INT8U *Temp = EnStartAddr;
	*Temp++ = 0x09;
	Temp += EncodeLenth(Temp, DataLenth);
	LIB_MemCpy(Data, Temp, DataLenth);
	return Temp - EnStartAddr + DataLenth;
}


// EnOctStr
//#pragma SECTION program programD_Basedef_EnConstOctStr
INT16U  EnConstOctStr( INT8U * EnStartAddr, const INT8U * Data, INT16U DataLenth) 
{

	INT8U *Temp = EnStartAddr;
	*Temp++ = 0x09;
	Temp += EncodeLenth(Temp, DataLenth);
	ConstStrCopy((const INT8U *)Data, Temp, DataLenth);
    return Temp - EnStartAddr + DataLenth;
}

// EnVisibleStr
//#pragma SECTION program programD_Basedef_EnVisibleStr
INT16U EnVisibleStr(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth) 
{
	INT8U *Temp = EnStartAddr;
	*Temp++ = 0x0a;
	Temp += EncodeLenth(Temp, DataLenth);
	LIB_MemCpy(Data, Temp, DataLenth);
	return Temp - EnStartAddr + DataLenth;
}
// EnBCD
//#pragma SECTION program programD_Basedef_EnBCD
INT16U EnBCD(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)
{
	INT16U TempData;
	LIB_MemCpy(Data, (INT8U *)&TempData, DataLenth);
    *EnStartAddr = 0;
	return 0;
}

// EnEnum
//#pragma SECTION program programD_Basedef_EnEnum
INT16U EnEnum(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)
{
	while(DataLenth--)
	EnStartAddr[0] = 0x16;
	EnStartAddr[1] = *Data;
	return 2;
}

INT16U EnConstEnum(INT8U * EnStartAddr, const INT8U *Data, INT16U DataLenth)
{
	while(DataLenth--)
	EnStartAddr[0] = 0x16;
	EnStartAddr[1] = *Data;
	return 2;
}


// EnDateTime
//#pragma SECTION program programD_Basedef_EnDateTime
INT16U EnDateTime(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)
{
	return EnOctStr(EnStartAddr, Data, DataLenth);
}
//#pragma SECTION program programD_Basedef_EnDateTime2
INT8U EnDateTime2(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)
{
	EnStartAddr[0] = 25;
	LIB_MemCpy(Data, EnStartAddr + 1, 12);
	return 13;
}
//#pragma SECTION program programD_Basedef_EnDate
INT16U EnDate(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)
{
	EnStartAddr[0] = 26;
	LIB_MemCpy(Data, EnStartAddr + 1, 5);
	return 6;
}
//#pragma SECTION program programD_Basedef_EnTime
INT16U EnTime(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)
{
	EnStartAddr[0] = 27;
	LIB_MemCpy(Data, EnStartAddr + 1, 4);
	return 5;
}
//#pragma SECTION program programD_Basedef_EnFloat32
INT16U EnFloat32(INT8U * EnStartAddr, INT8U *Data, INT16U DataLenth)
{
	EnStartAddr[0] = 23;
	//#ifdef FLOAT_ENCODE_MODE_INVERED 
	EnStartAddr[1] = Data[3];
	EnStartAddr[2] = Data[2];
	EnStartAddr[3] = Data[1];
	EnStartAddr[4] = Data[0];
	//#else
	//EnStartAddr[1] = Data[0];
	//EnStartAddr[2] = Data[1];
	//EnStartAddr[3] = Data[2];
	//EnStartAddr[4] = Data[3];
	//#endif

	return 5;
}

/* declare encode function list */
//#pragma  sectaddress  _Encode_FE, ROMDATA  0xD9720
//#pragma section  rom  _Encode
const EncodeFunType Encode[29] = 
{
  EnNull,            // 0
  EnArray,         // 1 Array       
  EnStruct,        // 2 Structure   
  EnBOOL,          // 3 bool        
  EnBitStr,        // 4 Bit str     
  EnINT,           // 5 Tdouble_long
  EnUnsgned,       // 6 Tdouble_long_unsigned  
  EnNull,            // 7            
  EnNull,            // 8            
  EnOctStr,        // 9 octet-string 
  EnVisibleStr,        // 10 Vistr        
  EnNull,            // 11             
  EnNull,            // 12
  EnBCD,           // 13 Bcd
  EnNull,            // 14
  EnINT,           // 15 integer8
  EnINT,           // 16 integer16
  EnUnsgned,       // 17 unsigned8
  EnUnsgned,       // 18 unsigned16
  EnNull,            // 19
  EnINT,           // 20 integer64
  EnUnsgned,       // 21 unsigned64
  EnEnum,          // 22 enum
  EnFloat32,//EnNull,            // 23 float32
  EnNull,            // 24 float64
  EnDateTime,      // 25 date time
  EnDate,      // 26 date
  EnTime,      // 27 time
  EnNull
};      




/************************************************************************* 
                             其他的基本函数
*************************************************************************/
#if 0
/* 字符串比较 */
//#pragma SECTION program programD_Basedef_StrCmp
__root Bool StrCmp(INT8U * Src, INT8U * Dest, INT16U Lenth ) 
{
	while(Lenth--)
	{
		if((*Src++) != (*Dest++))
		{
			return false;
		}
	}
	return true;	
}

//#pragma SECTION program programD_Basedef_StrConstCmp
__root Bool  StrConstCmp(const INT8U * Src, const INT8U * Dest, INT16U Lenth )  /* 字符串比较 */
{
	while(Lenth--)
	{
		if((*Src++) != (*Dest++))
		{
			return false;
		}
	}
	return true;	
}


/* 字符串复制 */
//#pragma SECTION program programD_Basedef_StrCopy
__root void  StrCopy(INT8U * Src, INT8U * Dest, INT16U Lenth )
{
	if(Dest <= Src)
	{
		while(Lenth--)
	    {
		   (*Dest++) = (*Src++);
		}
	}
	else
	{
		Dest+= Lenth-1;
		Src+= Lenth-1;
		while(Lenth--)
	    {
		   (*Dest--) = (*Src--);
		}
	}	
}
#endif
/* 字符串复制 */
//#pragma SECTION program programD_Basedef_ConstStrCopy
void  ConstStrCopy(const INT8U * Src, INT8U * Dest, INT16U Lenth )
{
	if(Dest <= Src)
	{
		while(Lenth--)
	    {
		   (*Dest++) = (*Src++);
		}
	}
	else
	{
		Dest+= Lenth-1;
		Src+= Lenth-1;
		while(Lenth--)
	    {
		   (*Dest--) = (*Src--);
		}
	}	
}

#if 0
/* 字符串清空 */
//#pragma SECTION program programD_Basedef_StrClean
__root void  StrClean( INT8U * Dest, INT16U Lenth )
{
//	INT8U * ddd = Dest;
	while(Lenth--)
	{
		*Dest++ = 0;
	}	
}      
#endif

/* 求10的N次方*/
//#pragma  sectaddress  _Exp_10_FE, ROMDATA  0xD97B0
//#pragma section  rom  _Exp_10
const INT32U Exp_10[8] = {1L, 10L, 100L, 1000L, 10000L, 100000L, 1000000L, 10000000L};
//#pragma SECTION program programD_Basedef_Power10
INT32U  Power10(INT8U Exp)
{
    return Exp_10[Exp & 0x07];
}

/* 小数点平移 */
//#pragma SECTION program programD_Basedef_CheckScaler
INT32U  CheckScaler(INT8S S_Scaler, INT8S D_Scaler, INT32U Data)
{
    INT8S Slr = D_Scaler - S_Scaler;
    if(Slr >= 0)
    {
        return Data/Exp_10[(Slr)&0x07];
    }
    else
        return Data*Exp_10[(-1)*Slr&0x07];
       
}

/* 十六进制转十进制 */
//#pragma SECTION program programD_Basedef_HexToDec
void HexToDec(INT8U *Data, INT8U Lenth)
{
	while(Lenth--)
	{
		if(*Data <= 0x63)
		{
		    *Data = (((*Data) / 10) << 4) + ((*Data) % 10);
		}
		Data++;
	}
}

/* 十进制转十六进制 */
//#pragma SECTION program programD_Basedef_DecToHex
void DecToHex(INT8U *Data, INT8U Lenth)
{
	while(Lenth--)
	{
		*Data = (((*Data) >> 4) * 10) + ((*Data) & 0x0F);
		Data++;
	}
}

/* COSEM type 类型数据拷贝 */
//#pragma SECTION program programD_Basedef_CopyCosemMssag
void CopyCosemMsg(COSEM_MessageType *Source, COSEM_MessageType *Dest)
{
	LIB_MemCpy((INT8U *)Source, (INT8U *)Dest, sizeof(COSEM_MessageType));  
}

#if 0
/* 从Epprom 中读出某一个结构体数据 */
//#pragma SECTION program programD_Basedef_LoadStructData
__root Bool LoadStructData(INT8U *StructAddr, INT16U EpprommAddr, INT16U Size)
{
    INT16U CRC;
    INT16U Scrc = *(INT16U*)(&StructAddr[Size-2]);   /* 结构体的最后两个字节为CRC校验字 */
    *(INT16U*)(&StructAddr[Size-2]) = 0;           /* 清CRC */
    
    CRC = CRCString((INT8U *)&StructAddr, Size-2);
    if(CRC != Scrc)
    {   /* 从EPPROM中读出 */
        if(!LoadData(StructAddr, EpprommAddr, Size))
        {
            return false;
        }
    } 
    return SUCCESS;  
}
#endif
//#pragma SECTION program programD_Basedef_TimeComp
INT8U TimeComp(INT8U *pr, INT8U *ps, INT8U Size)
{ 
	  
	if(Size==0)
	    return 1;	  	 
	if((*pr)>(*ps))
	    return 1;
	 else if((*pr)<(*ps))
	    return 0;
     else return TimeComp(pr++,ps++,Size-1);

}

//#pragma SECTION program programD_Basedef_bswap16
INT16U bswap16(INT16U x)
{
    return (x & 0x00ff) << 8 | (x & 0xff00) >> 8 ;
}
//#pragma SECTION program programD_Basedef_bswap32
INT32U bswap32(INT32U x)
{
    return ( x & 0xff000000 ) >>24 | ( x & 0x00ff0000 ) >>8 | ( x & 0x0000ff00 ) <<8 | ( x & 0x000000ff ) << 24 ; 
}

/* convertion from big-endian to little-endian */
INT32U chSwap32(INT8U *temp)
{
    return (((INT32U)(temp[0]&0x000000FF)<<24) | \
            ((INT32U)(temp[1]&0x000000FF)<<16) | \
            ((INT32U)(temp[2]&0x000000FF)<< 8)| \
            ((INT32U)(temp[3]&0x000000FF)));
}

INT16U chSwap16(INT8U *temp)
{
    return (((INT16U)(temp[0]&0x00FF)<<8) | \
            ((INT16U)(temp[1]&0x00FF)));
}

#if 0
INT32U TimeDelta(DateTimeType DateTimeA, DateTimeType DateTimeB)
{
    INT32U  SecondA;
    INT32U  SecondB;
    s_DateTime tmpA;
    s_DateTime tmpB;
        
    DateTimeTypeToYMDHMS((INT8U *)&DateTimeA, (INT8U *)&tmpA);
    DateTimeTypeToYMDHMS((INT8U *)&DateTimeB, (INT8U *)&tmpB);
    SecondA = TIME_IDEX((INT8U *)&tmpA.Year) * 60 + tmpA.Second;    //A与2000年相差的秒数
	SecondB = TIME_IDEX((INT8U *)&tmpB.Year) * 60 + tmpB.Second;    //B与2000年相差的秒数
	return ((SecondB >= SecondA)?(SecondB-SecondA):(SecondA-SecondB));
}
#endif
//---------------------------------------------------------------------------
/* 日期比较函数  表示大于 */
//#pragma SECTION program programD_Basedef_SysDateCmpr
INT8U SysDateCmpr( DateType *Fdate,  DateType *Ldate)
{	                                            
  INT32U fd, ld;
  
  if( (Fdate->Date.Year_H  != 0xFF) && (Fdate->Date.Year_L != 0xFF)     /* 是否为通配, 例如 xx年5月1日 */
       && (Ldate->Date.Year_H  != 0xFF) && (Ldate->Date.Year_L  != 0xFF) )
  {
     fd = Fdate->Date.Year_H  * 256L + Fdate->Date.Year_L;
     ld = Ldate->Date.Year_H  * 256L + Ldate->Date.Year_L;
     if(fd > ld)             /* 年 */
     {
       return LATE;
     }
     else if(fd < ld)
       return EARLY;
  }
  if((Fdate->Date.Month != 0xFF) && (Ldate->Date.Month != 0xFF))
  {  
      if(Fdate->Date.Month > Ldate->Date.Month)  /* 月 */
      {
        return LATE;
      }
      else if(Fdate->Date.Month < Ldate->Date.Month)
        return EARLY;
  }
  if((Fdate->Date.Day != 0xFF) && (Ldate->Date.Day != 0xFF))
  {
	    
    if(Fdate->Date.Day > Ldate->Date.Day)      /* 日  */
     {
       return LATE;
     }
    else if(Fdate->Date.Day < Ldate->Date.Day)
     return EARLY;
  }

  return EQU;
}

//---------------------------------------------------------------------------

/********************************************************* 

单动作的时间判断单独处理 SLN 161012	 
	
*********************************************************/
INT8U SysTimeCmpr(  TimeType *Ftime,  TimeType *Ltime)
{	    
	/* 时 */
	if( (Ftime->Time.Hour != 0xFF)&& (Ltime->Time.Hour != 0xFF))            
	{
		if(Ftime->Time.Hour > Ltime->Time.Hour)             
			return LATE;
		else if(Ftime->Time.Hour < Ltime->Time.Hour)
			return EARLY;
	}
	/* 分 */
	if((Ftime->Time.Min != 0xFF) && (Ltime->Time.Min != 0xFF))
	{  
		if(Ftime->Time.Min > Ltime->Time.Min)  
			return LATE;
		else if(Ftime->Time.Min < Ltime->Time.Min)
			return EARLY;
	}
	/* 秒 */
	if((Ftime->Time.Sec != 0xFF) && (Ltime->Time.Sec != 0xFF))
	{	    
		if(Ftime->Time.Sec > Ltime->Time.Sec)      
			return LATE;
		else if(Ftime->Time.Sec < Ltime->Time.Sec)
			return EARLY;
  	}

	return EQU;
}

//----------------------------------------------------------------------------
/* 日期时间比较 */
//#pragma SECTION program programD_Basedef_SysDtTmCmpr
INT8U SysDtTmCmpr( DateTimeType *Fdate,  DateTimeType *Ldate)
{	                                            
  INT8U Result = SysDateCmpr( &(Fdate->DateTime.Date), &(Ldate->DateTime.Date) );
  
  if( Result == EQU )
  {
     Result = SysTimeCmpr( &(Fdate->DateTime.Time), &(Ldate->DateTime.Time) );
  }
  return Result;
}

/* 时间比较函数  */
//#pragma SECTION program programD_Basedef_SysTimeCmprNoSec
INT8U SysTimeCmprNoSec(  TimeType *Ftime,  TimeType *Ltime)
{	    
//	INT32U test1,test2;    
	                                    
  INT32U ff = Ftime->Time.Hour * 60L * 60L + Ftime->Time.Min * 60L ;
  INT32U ll = Ltime->Time.Hour * 60L * 60L + Ltime->Time.Min * 60L;
 /* test1 = Ftime->Time.Hour * 60;
  test1 = test1*60;
  test2 = Ftime->Time.Min * 60;
  test1 +=test2;
  
  test1 = Ltime->Time.Hour * 60;
  test1 = test1*60;
  test2 = Ltime->Time.Min * 60;
  test1 +=test2;*/
  if(ff > ll)
    return LATE;
  else if(ff < ll)
    return EARLY;
    
  return EQU;
}

/* 日期时间比较 */
//#pragma SECTION program programD_Basedef_SysDtTmCmprNoSec
INT8U SysDtTmCmprNoSec( DateTimeType *Fdate,  DateTimeType *Ldate)
{	                                            
  INT8U Result = SysDateCmpr( &(Fdate->DateTime.Date), &(Ldate->DateTime.Date) );
  
  if( Result == EQU )
  {
     Result = SysTimeCmprNoSec( &(Fdate->DateTime.Time), &(Ldate->DateTime.Time) );
  }
  return Result;
}

//#pragma SECTION program programD_Basedef_BasetimeCmpr
INT8U BasetimeCmpr(s_DateTime *Systime, DateTimeType *SeasonTime)
{ /* modified by lin 2011-5-5  需将s_DateTime类型数据转化为十六进制(BcdtoHex)再进行相应处理 */
    INT16U temp;
    if(( SeasonTime->DateTime.Date.Date.Year_H != 0xFF) && (SeasonTime->DateTime.Date.Date.Year_L != 0xFF ))
    {
        temp = SeasonTime->DateTime.Date.Date.Year_L + SeasonTime->DateTime.Date.Date.Year_H * 256;
        if( (Systime->Year + 2000) > temp)
        {
            return LATE;
        }
        else if( (Systime->Year + 2000) < temp)
        {
            return EARLY;
        }
    }
    
    /* 比较 月 */
    if(SeasonTime->DateTime.Date.Date.Month != 0xFF)
    {
        if( Systime->Month  > SeasonTime->DateTime.Date.Date.Month)
        {
            return LATE;
        }
        else if( Systime->Month  < SeasonTime->DateTime.Date.Date.Month)
        {
            return EARLY;
        }
    }
    /* 比较 日 */
    if(SeasonTime->DateTime.Date.Date.Day != 0xFF)
    {
        if(Systime->Day  > SeasonTime->DateTime.Date.Date.Day)
        {
            return LATE;
        }
        else if(Systime->Day  < SeasonTime->DateTime.Date.Date.Day)
        {
            return EARLY;
        }
    }
     /* 比较 时 */
    if(SeasonTime->DateTime.Time.Time.Hour != 0xFF)
    {
        if( Systime->Hour  > SeasonTime->DateTime.Time.Time.Hour)
        {
            return LATE;
        }
        else if(Systime->Hour  < SeasonTime->DateTime.Time.Time.Hour)
        {
            return EARLY;
        }   
    }
    
    /* 比较 分 */
    if(SeasonTime->DateTime.Time.Time.Min != 0xFF)
    {
        if( Systime->Minute  > SeasonTime->DateTime.Time.Time.Min)
        {
            return LATE;
        }
        else if( Systime->Minute  < SeasonTime->DateTime.Time.Time.Min)
        {
            return EARLY;
        }   
    }
    /* 比较 秒 */
    if(SeasonTime->DateTime.Time.Time.Sec != 0xFF)
    {
        if( Systime->Second  > SeasonTime->DateTime.Time.Time.Sec)
        {
            return LATE;
        }
        else if( Systime->Second  < SeasonTime->DateTime.Time.Time.Sec)
        {
            return EARLY;
        }   
    }
    
    return EQU;
}

/* 检测0数据 若全为0 则返回true */
//#pragma SECTION program programD_Basedef_CheckConstZero
BOOL CheckConstZero(const INT8U *Addr, INT16U Lenth)
{
    while(Lenth--)
    {
        if(*Addr++ != 0)
          return false;
    }
    return true;
}

/* 检测0数据 若全为0 则返回true */
//#pragma SECTION program programD_Basedef_CheckZero
BOOL CheckZero(INT8U *Addr, INT16U Lenth)
{
    while(Lenth--)
    {
        if(*Addr++ != 0)
          return false;
    }
    return true;
}


/* 检测0xff数据 若全为0xff 则返回true */
//#pragma SECTION program programD_Basedef_CheckWildcards
BOOL CheckWildcards(INT8U *Addr, INT16U Lenth)
{
    while(Lenth--)
    {
        if(*Addr++ != 0xff)
          return false;
    }
    return true;
}


/* 时间调整函数，时间前调 shift time 单位为 秒 */
//#pragma SECTION program programD_Basedef_DateTimeShiftBack
void DateTimeShiftBack(DateTimeType * CurDateTime, INT32U ShiftTime)
{
    INT32U Sec; 
    INT32U Min;
    INT32U Hour;
    
    Sec = ShiftTime % 60;
    Min = ShiftTime / 60;
    Hour = Min / 60;
    Min = Min % 60;
    
    if(CurDateTime->DateTime.Time.Time.Sec >= Sec)    
    {
       CurDateTime->DateTime.Time.Time.Sec -= Sec;
    }
    else
    {
       CurDateTime->DateTime.Time.Time.Sec = 60 + CurDateTime->DateTime.Time.Time.Sec - Sec;
       Min += 1;
    }
    
    if(CurDateTime->DateTime.Time.Time.Min >= Min)    
    {
       CurDateTime->DateTime.Time.Time.Min -= Min;
    }
    else
    {
       CurDateTime->DateTime.Time.Time.Min = 60 + CurDateTime->DateTime.Time.Time.Min - Min;
       Hour += 1;
    }   
    
    if(CurDateTime->DateTime.Time.Time.Hour >= Hour)    
    {
       CurDateTime->DateTime.Time.Time.Hour -= Hour;
    }
    else
    {
       CurDateTime->DateTime.Time.Time.Hour = 24 + CurDateTime->DateTime.Time.Time.Hour - Hour;
       
       CurDateTime->DateTime.Date.Date.Day -= 1;
       if( !CurDateTime->DateTime.Date.Date.Day )
       {
           CurDateTime->DateTime.Date.Date.Month -= 1;
           switch(CurDateTime->DateTime.Date.Date.Month)
           {
           case 0: CurDateTime->DateTime.Date.Date.Year_L -= 1;
                   if(CurDateTime->DateTime.Date.Date.Year_L == 0xFF)
                   {
                       CurDateTime->DateTime.Date.Date.Year_H -= 1;
                   }
                   CurDateTime->DateTime.Date.Date.Month = 12;
           case 1:
           case 3:
           case 5:
           case 7:
           case 8:
           case 10:
               CurDateTime->DateTime.Date.Date.Day = 31;
               break;
           case 4:
           case 6:
           case 9:
           case 11:
               CurDateTime->DateTime.Date.Date.Day = 30;
               break;
           case 2:if( CurDateTime->DateTime.Date.Date.Year_L & 0x03)
                      CurDateTime->DateTime.Date.Date.Day = 28;
                  else
                      CurDateTime->DateTime.Date.Date.Day = 29;
               break;
           default:
               break;
           }
       }
    }       
	CurDateTime->DateTime.Date.Date.Week = CalculateWeek(CurDateTime->DateTime.Date.Date.Year_H*256L+CurDateTime->DateTime.Date.Date.Year_L, 
	                                                     CurDateTime->DateTime.Date.Date.Month, CurDateTime->DateTime.Date.Date.Day);        
}

/* 时间调整函数，时间后调 shift time 单位为 秒 */
void DateTimeShiftForward(DateTimeType * CurDateTime, INT32U ShiftTime)
{
    INT32U Sec; 
    INT32U Min;
    INT32U Hour;
    
    Sec = ShiftTime % 60;
    Min = ShiftTime / 60;
    Hour = Min / 60;
    Min = Min % 60;
    
    if(CurDateTime->DateTime.Time.Time.Sec + Sec < 60)    
    {
       CurDateTime->DateTime.Time.Time.Sec += Sec;
    }
    else
    {
       CurDateTime->DateTime.Time.Time.Sec = CurDateTime->DateTime.Time.Time.Sec + Sec - 60;
       Min += 1;
    }
    
    if(CurDateTime->DateTime.Time.Time.Min + Min <60)    
    {
       CurDateTime->DateTime.Time.Time.Min += Min;
    }
    else
    {
       CurDateTime->DateTime.Time.Time.Min =  CurDateTime->DateTime.Time.Time.Min + Min - 60;
       Hour += 1;
    }   
    
    if(CurDateTime->DateTime.Time.Time.Hour + Hour<24 )    
    {
       CurDateTime->DateTime.Time.Time.Hour += Hour;
    }
    else
    {
       CurDateTime->DateTime.Time.Time.Hour = CurDateTime->DateTime.Time.Time.Hour + Hour - 24;
     
        switch(CurDateTime->DateTime.Date.Date.Month)
           {
           case 12: if(CurDateTime->DateTime.Date.Date.Day ==31 )
	                  {
						   if(CurDateTime->DateTime.Date.Date.Year_L == 0xFF)
			                 {
			                    CurDateTime->DateTime.Date.Date.Year_H += 1;
			                 }
					       CurDateTime->DateTime.Date.Date.Year_L += 1;
                           CurDateTime->DateTime.Date.Date.Month = 1;
						   CurDateTime->DateTime.Date.Date.Day =1;
					  }
					  else
					    CurDateTime->DateTime.Date.Date.Day++;
					  break;
           case 1:
           case 3:
           case 5:
           case 7:
           case 8:
           case 10:
		       if(CurDateTime->DateTime.Date.Date.Day ==31 )
			     {
					 CurDateTime->DateTime.Date.Date.Month += 1;
                     CurDateTime->DateTime.Date.Date.Day = 1;
				 }
				else  CurDateTime->DateTime.Date.Date.Day++;
               break;
           case 4:
           case 6:
           case 9:
           case 11:
               if(CurDateTime->DateTime.Date.Date.Day ==30 )
			     {
					 CurDateTime->DateTime.Date.Date.Month += 1;
                     CurDateTime->DateTime.Date.Date.Day = 1;
				 }
				else  CurDateTime->DateTime.Date.Date.Day++;
               break;
           case 2:if( CurDateTime->DateTime.Date.Date.Year_L & 0x03)
		             {
						 if(CurDateTime->DateTime.Date.Date.Day ==28 )
				         {
						   CurDateTime->DateTime.Date.Date.Month += 1;
	                       CurDateTime->DateTime.Date.Date.Day = 1;
					     }
						 else CurDateTime->DateTime.Date.Date.Day++;
					 }
				   else
                      {
						 if(CurDateTime->DateTime.Date.Date.Day ==29 )
				         {
						   CurDateTime->DateTime.Date.Date.Month += 1;
	                       CurDateTime->DateTime.Date.Date.Day = 1;
					     }
						else CurDateTime->DateTime.Date.Date.Day++;
					 }
               break;
           default:
               break;
           }
    }  
	CurDateTime->DateTime.Date.Date.Week = CalculateWeek(CurDateTime->DateTime.Date.Date.Year_H*256L+CurDateTime->DateTime.Date.Date.Year_L, 
	                                                     CurDateTime->DateTime.Date.Date.Month, CurDateTime->DateTime.Date.Date.Day);   
}


/* 获取原始计量瞬时数据的小数点位数 added by cyh 5.8 */
//#pragma SECTION program programD_Basedef_GetBaseDataScaler
INT8S GetBaseDataScaler(INT8U C_Code)
{
    C_Code = (C_Code - 1) % 20 + 1;
    switch(C_Code)
    {
    case 1:
    case 2:
    case 15:
    case 16:
    case 17:
    case 18:
    case 19:
    case 20:
       return Actpower_SSclr;
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
       return ReActpower_SSclr;
    case 9:
    case 10:
       return Aprnpower_SSclr;
    case 11: 
       return Irms_SSclr;
    case 12:
       return Urms_SSclr;
    case 13:
       return PF_SSclr;
    case 14:
       return HZ_SSclr;
    default:
       break;
    }
    return 0;
}


/***********************************************************************/
/**************           计算每个月的最大天数：             ***************/
/***********************************************************************/
//#pragma SECTION program programD_Basedef_dayer_acu
INT8U dayer_acu(INT8U year,INT8U month)
 {
    INT8U  dayer; 
    
    switch(month)
        {
        case 1:
        case 3:
        case 5:
        case 7:
        case 8:
        case 10:
        case 12:
           dayer=31;
           break;
        case 2: 
            if( (year&0x03)==0x00 )
                {
                    dayer=29;
                }
            else
                {
                  dayer=28;
                }
             break;
        default:
            {
               dayer=30;  
            }
        }
    return dayer;
 }
    
	

INT8U Specified_Day_Calculate(INT8U y,INT8U m ,INT8U d ,INT8U w) 
{
     INT8U week,temp_day,value,w_count=0;
	 
	 if(d!=0xfd&&d!=0xfe)
	 {
	  w_count = (d%7==0)?(d/7):(d/7 +1) ;
	  week=CalculateWeek(y+2000,m,0x01);
	  if(week<=w)
	    value = w-week + 1 + 7*(w_count-1);
	  else
	    value = 7-(week-w) +1 + 7*(w_count-1); 
	 }
	 else
	 { 
	  if(d==0xfe)
	     w_count= 1;
	  else if(d==0xfd)
	     w_count= 2;
	  temp_day=dayer_acu(y,m);
	  week=CalculateWeek(y+2000,m,temp_day);
	  if(week<w)
	    value =temp_day-(7-(w-week))-7*(w_count-1);
	  else
	    value = temp_day - (week-w)-7*(w_count-1);
	 }

	 return value;		
}
//#if 0
INT8U  Calendar_Calculate(DateTimeType * S_DateTime, DateTimeType *D_DateTime,INT8U Year,INT8U flag)
{
	 INT8U temp_day,yearflag=0;
	 s_DateTime  TempDateTime;
	 DateTimeType ToCmpEndTime;
	 LIB_MemCpy((INT8U *)S_DateTime,(INT8U *)D_DateTime,sizeof(DateTimeType));
    // if(CheckWildcards((INT8U *)S_DateTime,8))
     //    return ;
	 //LIB_MemCpy((INT8U *)&NormalDateTime,(INT8U *)&TempDateTime,sizeof(s_DateTime));
	 //YMDHMSToDateTimeType((INT8U *)&(NormalDateTime),(INT8U*)&(ToCmpEndTime));

      //wildcard year 
	  if(D_DateTime->DateTime.Date.Date.Year_H==0xff&&D_DateTime->DateTime.Date.Date.Year_L==0xff) 
	  {
	      //Daylight_Savings_Begin time is after Daylight_Savings_End time 
	      //current datetime is after Daylight_Savings_Begin time
		  if(Year&&(flag==DLS_END)&&(SysDtTmCmpr(&ToCmpEndTime,S_DateTime))!=EARLY)
	        TempDateTime.Year++;//DSL end year is next year
          //Daylight_Savings_Begin time is after Daylight_Savings_End time
		  //current datetime is before Daylight_Savings_Begin time
	      else if(Year&&(flag==DLS_END)&&(SysDtTmCmpr(&ToCmpEndTime,S_DateTime))==EARLY) 
	        yearflag=1;//report: DSL start year is last year
          if(Year&&(flag==DLS_BEGIN))
	        TempDateTime.Year--;//DSL start year is last year
		  D_DateTime->DateTime.Date.Date.Year_H = (TempDateTime.Year + 2000) >> 8;
	      D_DateTime->DateTime.Date.Date.Year_L = (TempDateTime.Year + 2000) & 0x00FF;
	  }
	  else
	   {
		 TempDateTime.Year = D_DateTime->DateTime.Date.Date.Year_H * 256L +D_DateTime->DateTime.Date.Date.Year_L - 2000;
	   }   
	   
	
	 if(S_DateTime->DateTime.Date.Date.Month==0xff)
	  {
		   D_DateTime->DateTime.Date.Date.Month = TempDateTime.Month;
		   if(S_DateTime->DateTime.Date.Date.Day==0xfe)
		   {
			  if(S_DateTime->DateTime.Date.Date.Week==0xff) //ok
			    {
					temp_day=dayer_acu(TempDateTime.Year,TempDateTime.Month);
				    D_DateTime->DateTime.Date.Date.Day=temp_day;
				}
			  else //ok
			    {
					temp_day=Specified_Day_Calculate(TempDateTime.Year,TempDateTime.Month,S_DateTime->DateTime.Date.Date.Day,S_DateTime->DateTime.Date.Date.Week);
				    D_DateTime->DateTime.Date.Date.Day=temp_day;
				}
		   
		   }
		   else if(S_DateTime->DateTime.Date.Date.Day==0xfd)
		   {
			  if(S_DateTime->DateTime.Date.Date.Week==0xff) //ok
			   {
			      temp_day=dayer_acu(TempDateTime.Year,TempDateTime.Month);
				  D_DateTime->DateTime.Date.Date.Day=temp_day-1; 
			   }
			  else //ok
			   {
			     temp_day=Specified_Day_Calculate(TempDateTime.Year,TempDateTime.Month,S_DateTime->DateTime.Date.Date.Day,S_DateTime->DateTime.Date.Date.Week);
			     D_DateTime->DateTime.Date.Date.Day=temp_day;
			   } 
		   }
		  else if(S_DateTime->DateTime.Date.Date.Day==0xff)
		   {
			  if(S_DateTime->DateTime.Date.Date.Week==0xff) 
			      YMDToDateType((INT8U *)&TempDateTime,(INT8U *)D_DateTime); 
			   else 
			  {
			    if(S_DateTime->DateTime.Date.Date.Week==CalculateWeek(TempDateTime.Year+2000,TempDateTime.Month,TempDateTime.Day))
			      YMDToDateType((INT8U *)&TempDateTime,(INT8U *)D_DateTime);
				else 
				   {
						TempDateTime.Day = 0x32;
					    YMDToDateType((INT8U *)&TempDateTime,(INT8U *)D_DateTime);
				   }
			  }
		   }
		   else
		   {
			    if(S_DateTime->DateTime.Date.Date.Week==0xff) 
			    {
					D_DateTime->DateTime.Date.Date.Week = CalculateWeek(TempDateTime.Year+2000,TempDateTime.Month,S_DateTime->DateTime.Date.Date.Day);
				
				}
			  else 
			    {
			       temp_day=Specified_Day_Calculate(TempDateTime.Year,TempDateTime.Month,S_DateTime->DateTime.Date.Date.Day,S_DateTime->DateTime.Date.Date.Week);
			       D_DateTime->DateTime.Date.Date.Day=temp_day;
				}
		   }
	 }
	 else 
	 { 
		  if(S_DateTime->DateTime.Date.Date.Day==0xfe)
		   {
			  if(S_DateTime->DateTime.Date.Date.Week==0xff) 
			     {
					 temp_day=dayer_acu(TempDateTime.Year,S_DateTime->DateTime.Date.Date.Month);
				     D_DateTime->DateTime.Date.Date.Day=temp_day;
				 }
			  else //ok
			    {
			        temp_day=Specified_Day_Calculate(TempDateTime.Year,S_DateTime->DateTime.Date.Date.Month,S_DateTime->DateTime.Date.Date.Day,S_DateTime->DateTime.Date.Date.Week);
				    D_DateTime->DateTime.Date.Date.Day=temp_day;
				}
		   }
		   else if(S_DateTime->DateTime.Date.Date.Day==0xfd)
		   {
			  if(S_DateTime->DateTime.Date.Date.Week==0xff) 
			    {
					temp_day=dayer_acu(TempDateTime.Year,S_DateTime->DateTime.Date.Date.Month);
				    D_DateTime->DateTime.Date.Date.Day=temp_day-1; 
				}
			  else //ok
			   {
			       temp_day=Specified_Day_Calculate(TempDateTime.Year,S_DateTime->DateTime.Date.Date.Month,S_DateTime->DateTime.Date.Date.Day,S_DateTime->DateTime.Date.Date.Week);
				   D_DateTime->DateTime.Date.Date.Day=temp_day;
			   }   
		   }
		  else if(S_DateTime->DateTime.Date.Date.Day==0xff)
		   {
			  if(S_DateTime->DateTime.Date.Date.Week==0xff) 
			      D_DateTime->DateTime.Date.Date.Day=TempDateTime.Day;
			   else 
				{
					if(TempDateTime.Month==D_DateTime->DateTime.Date.Date.Month)
					{
						if(S_DateTime->DateTime.Date.Date.Week==CalculateWeek(TempDateTime.Year+2000,TempDateTime.Month,TempDateTime.Day))
				          YMDToDateType((INT8U *)&TempDateTime,(INT8U *)D_DateTime);
						else 
						   {
							   TempDateTime.Day = 0x32;
					           YMDToDateType((INT8U *)&TempDateTime,(INT8U *)D_DateTime);
						   }
					}
				}
		   }
	       else 
		   {
			  if(S_DateTime->DateTime.Date.Date.Week==0xff) 
			    {
					D_DateTime->DateTime.Date.Date.Week = CalculateWeek(TempDateTime.Year+2000,S_DateTime->DateTime.Date.Date.Month,S_DateTime->DateTime.Date.Date.Day);
				
				}
			  else if(S_DateTime->DateTime.Date.Date.Year_L==0xff&&S_DateTime->DateTime.Date.Date.Year_H==0xff)//ok
			    {
			       temp_day=Specified_Day_Calculate(TempDateTime.Year,S_DateTime->DateTime.Date.Date.Month,S_DateTime->DateTime.Date.Date.Day,S_DateTime->DateTime.Date.Date.Week);
			       

	/*年为通配符，日和周确定值，表示每年每月的第N个星期几
	（N=INT(日/7)+1，当日能被7整除时，N=日/7，
	如果该月没有第五个多少周次,以日确定值执行）。*/
				   if(temp_day>dayer_acu(TempDateTime.Year,S_DateTime->DateTime.Date.Date.Month))
			       {
			          D_DateTime->DateTime.Date.Date.Week=CalculateWeek(TempDateTime.Year,S_DateTime->DateTime.Date.Date.Month,S_DateTime->DateTime.Date.Date.Day);
			       }
                   else
                   {
                      D_DateTime->DateTime.Date.Date.Day=S_DateTime->DateTime.Date.Date.Day;
                   }
				}
		   }
	 }
	 return yearflag;
}
//#endif
void Time_Convert_WildCard(INT8U *S_DateTime,INT8U *D_DateTime)
{

	 if(S_DateTime[0]!=0xff&&S_DateTime[1]!=0xff)
	   D_DateTime[0] = S_DateTime[0]*256L+S_DateTime[1]-2000;
	 else 
	   D_DateTime[0] = 0xff;
	 D_DateTime[1] = S_DateTime[2];
	 D_DateTime[2] = S_DateTime[3];
	 D_DateTime[3] = S_DateTime[4];
	 D_DateTime[4] = S_DateTime[5];
	 D_DateTime[5] = S_DateTime[6];
	 D_DateTime[6] = S_DateTime[7];
}

/****************************************************
 comment:judge the validity of the timer
 Timer_Valid(unsigned char *p, unsigned char type)
 type=0  表示 时钟时间检查
    =1 表示 含有通配符 0XFF 的检查     
****************************************************/
//#pragma SECTION program programD_Basedef_Timer_Valid
INT8U Timer_Valid(INT8U *p, INT8U type)  //return 1 means valid 
{        
    INT8U Valid_Flag=1;
    INT8U day_temp;
//    INT8U week_temp;
    if( ((*p)>99) && (!(type && ((*p)==0xff))))   
    {
        Valid_Flag=0;
    }	
    
    if( (((*(p+1))>12) || ((*(p+1))==0)) && (!(type && ((*(p+1))==0xff))))  //month
    {
        Valid_Flag=0;
    }
   
//	if((*(p+2))==0xff&&(*(p+3))!= 0xff)
	   // Valid_Flag=0;
		
    day_temp = dayer_acu( *p,(*(p+1)) );
    
    if(  (((*(p+2))>day_temp) || ((*(p+2))==0)) &&  ((!(type &&((*(p+2))==0xff)))&&(!(type &&((*(p+2))==0xfe)))&&(!(type &&((*(p+2))==0xfd)))) )  //day
    {
        Valid_Flag=0;
    }
//	if(((*p)!=0xff)&&((*(p+1))!=0xff)&&((*(p+2))!=0xff)&&((*(p+2))!=0xfe)&&((*(p+2))!=0xfd)&&(!(type && ((*(p+3))== 0xff))) )
//      {
//		  week_temp = CalculateWeek(((*p + 2000)), (*(p+1)),( *(p+2)));
//		  if((*(p+3)) != week_temp)
//		     Valid_Flag=0;
//	  }
    
//    if( ((*(p+3))> 7) && (!(type && ((*(p+3))== 0xff))) )//星期日是7         //week
//    {
//        Valid_Flag=0;
//    }
    
    if(  ((*(p+4)) > 23) && (!(type && ((*(p+4))==0xff)))  )               //hour
   //if(!( (((*(p+4)>=0) && (*(p+4)<=23))  ||((*(p+4))==0xFF) ) )
    {
        Valid_Flag=0;
    }
    
    if( ((*(p+5)) > 59) && (!(type &&((*(p+5))==0xff))) )     //minute
 // if(!( (((*(p+5)>=0) && (*(p+5)<=59))  ||((*(p+5))==0xFF) ) )
    {
        Valid_Flag=0;
    }
    
    if( ((*(p+6)) > 59) && (!(type&& ((*(p+6))==0xff))) )    //second
   //if(!( (((*(p+6)>=0) && (*(p+6)<=59))  ||((*(p+6))==0xFF) ) )
    {
        Valid_Flag=0;
    }
        
    return  Valid_Flag;
}

//#pragma SECTION program programD_Basedef_TimeTransfer
void TimeTransfer(s_DateTime *Time,s_DateTime *TempTime)
{
	Time->Year 	=	TempTime->Year; 
	Time->Month	=	TempTime->Month;
	Time->Day	=	TempTime->Day;
	Time->Week	=	TempTime->Week;
	Time->Hour	=	TempTime->Hour;
 	Time->Minute=	TempTime->Minute;
    Time->Second=	TempTime->Second;
}

//#pragma SECTION program programD_Basedef_DecodeSetData
INT8U DecodeSetData(INT8U * EnStartAddr,INT8U *Data)  /*解码函数待完善 added by Lin 2011_5_19*/
{
    INT32U TempLong;
    INT16U TempInt;
    INT8U  Length,TempLength;
	
    switch(*EnStartAddr)
    {
    case T_VisStr:
        Length=*(++EnStartAddr);
        EnStartAddr++; 
        LIB_MemCpy(EnStartAddr,Data,Length);
        return Length;
    case T_OctStr:
        Length=*(++EnStartAddr);
        EnStartAddr++; 
        LIB_MemCpy(EnStartAddr,Data,Length);
        return Length;
    case T_DateTime:
        EnStartAddr++; 
        LIB_MemCpy(EnStartAddr,Data,12);
        return 12;
    case T_Date:
        EnStartAddr++; 
        LIB_MemCpy(EnStartAddr,Data,5);
        return 5;
    case T_Time:
        EnStartAddr++; 
        LIB_MemCpy(EnStartAddr,Data,4);
        return 4;
    case T_UN16: 
        EnStartAddr++;
        TempInt = chSwap16(EnStartAddr);
        LIB_MemCpy((INT8U *)&TempInt,Data,2);
        return 2;
    case T_UN32: 
        EnStartAddr++;
        TempLong = chSwap32(EnStartAddr);
        LIB_MemCpy((INT8U *)&TempLong,Data,4);
        return 4;
    case T_FLOAT32:  
        EnStartAddr++;
        TempLong = chSwap32(EnStartAddr);
        LIB_MemCpy((INT8U *)&TempLong,Data,4);
        return 4;
    case T_UN8:
    case T_Enum:
    case T_BOOL:
        EnStartAddr++;
        LIB_MemCpy(EnStartAddr,Data,1);
        return 1;
    case T_BitStr:
        Length=*(++EnStartAddr);
        TempLength=((Length%8==0)? Length/8:Length/8+1);
        EnStartAddr++;
        LIB_MemCpy(EnStartAddr,Data,TempLength);
        return Length;  ///*暂时如此处理*/
    }
    return 0;	
}

//#pragma SECTION program programD_Basedef_leap
char leap(int year)
{
 int leap;
 if(year%400L==0||(year%4L==0&&year%100L!=0))    /*if leap or not*/
  leap=1 ;
 else 
  leap=0;
 return leap;
}
//#pragma  sectaddress  _Rong_Year_FE, ROMDATA  0xDAC70
//#pragma section  rom  _Rong_Year
const INT16U Rong_Year[]={0,31,60,91,121,152,182,213,244,274,305,335,366};
//#pragma  sectaddress  _NoRong_Year_FE, ROMDATA  0xDACA0
//#pragma section  rom  _NoRong_Year
const INT16U NoRong_Year[]={0,31,59,90,120,151,181,212,243,273,304,334,365};
//#pragma SECTION program programD_Basedef_TIME_IDEX
unsigned long TIME_IDEX(unsigned char  *p)                                 //计算与2000年1月1日0时0分相差的分钟数
{
    unsigned long TotalMinInterval = 0;
    unsigned long a=1440;                                                  //a = 24 * 60 = 1440 <->一天的分钟数
//    unsigned long T_i_buffer[4];
//    unsigned long Temp;
    unsigned long numleapyear = 0; 
    unsigned int  i;	
	for(i = 0; i < *p; i++)                                        //判断和2000年之间有几个闰年
	{
        if(leap(2000+i))
		{
			numleapyear ++;
		}
	}
	TotalMinInterval =numleapyear;
	TotalMinInterval *=a;
	TotalMinInterval +=(*p)*525600;								//计算年的分钟数
	if(leap(*p+2000))												//计算月的分钟数
	{
		TotalMinInterval +=Rong_Year[*(p+1)-1]*a; 				//闰年
	}
	else
	{
		TotalMinInterval +=NoRong_Year[*(p+1)-1]*a; 				//非闰年
	}
	TotalMinInterval +=(*(p+2)-1)*a;								//计算天的分钟数
	TotalMinInterval +=(*(p+4))*60+*(p+5); 			//计算时、分的分钟数
	return  TotalMinInterval;
/*	以下程序存在的BUG是：没有考虑从 2000年至当前时间之间的闰年数；
	if(leap(BcdtoHex(*p)+2000))
	 {
      T_i_buffer[0]=BcdtoHex((*p))*527040;  
      T_i_buffer[1]=Rong_Year[BcdtoHex(*(p+1))-1]*a; 
      T_i_buffer[2]=(BcdtoHex(*(p+2))-1)*a;  
      T_i_buffer[3]=(BcdtoHex(*(p+4)))*60+BcdtoHex(*(p+5)); 
      Temp=T_i_buffer[0]+T_i_buffer[1]+T_i_buffer[2]+T_i_buffer[3];
      return  Temp;
	 }
	 else 
	 {
      T_i_buffer[0]=BcdtoHex((*p))*525600;  
      T_i_buffer[1]=NoRong_Year[BcdtoHex(*(p+1))-1]*a; 
      T_i_buffer[2]=(BcdtoHex(*(p+2))-1)*a;   
      T_i_buffer[3]=(BcdtoHex(*(p+4)))*60+BcdtoHex(*(p+5)); 
      Temp=T_i_buffer[0]+T_i_buffer[1]+T_i_buffer[2]+T_i_buffer[3];
      return  Temp;
	 }
*/	 
}	
//#pragma SECTION program programD_Basedef_U8ToBcd2
unsigned char U8ToBcd2(unsigned char *buff, unsigned char *dest)
{	
	INT8U ValueTmp;
	INT8U dividend;
	INT8S i;
	INT8U Value = *buff;
	INT8U *lptmp = dest;
	for(i = 1; i >=0; i --)
	{
		dividend = Power10(i);
		ValueTmp = Value/dividend;
		
		*lptmp++ = ValueTmp + 0x30;	
		Value = Value - ValueTmp * dividend;		
	}
	return lptmp - dest;
}


//#pragma SECTION program programD_Basedef_EncodeDateTime
unsigned char EncodeDateTime(unsigned char *buff, unsigned char *dest)
{
	//显示为时间格式
	unsigned char *lptmp = buff;
	unsigned int Year_16;
	unsigned char Year_8, Month, Day, Week, Hour, Minute, Second;
	Year_16 = *lptmp;
	Year_16 = (Year_16 << 8) + *(lptmp + 1);
	Year_16 = Year_16 - (Year_16/1000) * 1000;
	Year_8 = Year_16 - (Year_16/100) * 100;
	Month = *(lptmp + 2);
	Day = *(lptmp + 3);
	Week = *(lptmp + 4);
	Hour = *(lptmp + 5);
	Minute = *(lptmp + 6);	
	Second = *(lptmp + 7);
	
	lptmp = dest;
	lptmp += U8ToBcd2(&Year_8, lptmp);
	lptmp += U8ToBcd2(&Month, lptmp);
	lptmp += U8ToBcd2(&Day, lptmp);
	lptmp += U8ToBcd2(&Week, lptmp);
	lptmp += U8ToBcd2(&Hour, lptmp);
	lptmp += U8ToBcd2(&Minute, lptmp);
	lptmp += U8ToBcd2(&Second, lptmp);
	return lptmp - dest;
}

//#pragma SECTION program programD_Basedef_Calc_BCC
INT8U Calc_BCC(INT8U *lpSrc, unsigned int Length)  //lpSrc已经剔除了SOH和STX
{
	unsigned int i;
    INT8U BCC = *lpSrc++;
    for(i = 1; i < Length; i ++)
    {
        BCC ^= *lpSrc++;
    }
    return BCC;
}
//-------- add by slx
//参数：找到的buff的索引，起始地址，总buff数量
//返回：成功or失败
BOOL FindIdleMsgBuff(INT8U *MsgBuffIndex, COSEM_MessageType *lpMsg, INT8U MsgBuffCount)
{	
	for(*MsgBuffIndex = 0; *MsgBuffIndex < MsgBuffCount; (*MsgBuffIndex)++)
    {
        if(lpMsg->LockEn == FALSE)
        {
            break;
        }
		lpMsg++;
    }
    if(*MsgBuffIndex >= MsgBuffCount)  /* 没找到空缓冲区， 等待下次处理 */
    {
        return FALSE;
    }
	return TRUE;
}


void StrToHex(INT8U *Str, INT8U *Hex, INT16U StrLen)
{
    INT16U i=0;
    INT16U j=0;
        
    for(i=0; i<StrLen; i++)
    {
        if(('0'<=Str[i]) && (Str[i]<='9'))
        {
            Str[i] = Str[i]-'0';
        }
        else if(('a'<=Str[i]) && (Str[i]<='z'))
        {
            Str[i] = Str[i]-'a'+10;
        }
        else if(('A'<=Str[i]) && (Str[i]<='Z'))
        {
            Str[i] = Str[i]-'A'+10;
        }
    }     
    for(i=0; i<StrLen; )
    {
        Hex[j++] = (Str[i]<<4)|(Str[i+1]&0x0F);         
        i += 2;
    }
}

void HexToStr(INT8U *Str, INT8U *Hex, INT16U HexLen)
{
    INT16U i=0;
        
    for(i=0; i<HexLen; i++)
    {
        if(0x0a<=(Hex[i]&0x0f)) 
        {
            Str[(HexLen-i)*2-1] = 'a'+(Hex[i]&0x0f)-0x0a;
        }
	else
		Str[(HexLen-i)*2-1] = 0x30+(Hex[i]&0x0f);

	if(0x0a<=(Hex[i]>>4)) 
        {
            Str[(HexLen-i)*2-2] = 'a'+(Hex[i]>>4)-0x0a;
        }
	else
		Str[(HexLen-i)*2-2] = 0x30+(Hex[i]>>4);
	
    }     
}

INT8U ConstCompareString(const INT8U *ps,INT8U *pr,INT8S len)
{
	if(len>0)
	{
	while(len--)
		{
		if((*ps++)!=(*pr++)) return FALSE;
		}
	}else{
	while(len++)
		{
		if((*ps--)!=(*pr--)) return FALSE;
		}	
	}
return TRUE;
}

