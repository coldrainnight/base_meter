/*"
 * showlcd_abstract.c
 *
 *  Created on: 2012-1-19
 *      Author: ganjp
 "*/

#include "Driver\Hal\DIS\DIS.h"
#include "Lib\LIB_func.h"

/* 定义字符和数字的组成*/
#define _0     (SEG7_A + SEG7_B + SEG7_C + SEG7_D + SEG7_E + SEG7_F)
#define _1     (      	  SEG7_B + SEG7_C     )
#define _2     (SEG7_A + SEG7_B +		     SEG7_D + SEG7_E +		   SEG7_G )
#define _3     (SEG7_A + SEG7_B + SEG7_C + SEG7_D +                  		   SEG7_G  )
#define _4     (         	  SEG7_B + SEG7_C +                   		  SEG7_F + SEG7_G)
#define _5     (SEG7_A +                 SEG7_C + SEG7_D +                SEG7_F + SEG7_G) 
#define _6     (SEG7_A +                 SEG7_C + SEG7_D + SEG7_E + SEG7_F + SEG7_G)  
#define _7     (SEG7_A + SEG7_B + SEG7_C           )
#define _8     (SEG7_A + SEG7_B + SEG7_C + SEG7_D + SEG7_E + SEG7_F + SEG7_G )
#define _9     (SEG7_A + SEG7_B + SEG7_C + SEG7_D +                 SEG7_F + SEG7_G )

#define _A     (SEG7_A + SEG7_B + SEG7_C +                 SEG7_E + SEG7_F + SEG7_G  )
#define _b     (                                SEG7_C + SEG7_D + SEG7_E + SEG7_F + SEG7_G )
#define _C     (SEG7_A +                                 SEG7_D + SEG7_E + SEG7_F      )
//#define _c     (                            			      SEG7_D + SEG7_E +                SEG7_G  )
#define _d     (         	  SEG7_B + SEG7_C + SEG7_D + SEG7_E +                SEG7_G  )
#define _E     (SEG7_A +                   		      SEG7_D + SEG7_E + SEG7_F + SEG7_G   )
#define _e     (SEG7_A + SEG7_B +          	      SEG7_D + SEG7_E + SEG7_F + SEG7_G )
#define _F     (SEG7_A +                              			 SEG7_E + SEG7_F + SEG7_G)
#define _g     (SEG7_A + SEG7_B + SEG7_C + SEG7_D +         	  SEG7_F + SEG7_G     )
//#define _G     (SEG7_A +                 SEG7_C + SEG7_D + SEG7_E  + SEG7_F                  )
//#define _H     (         	  SEG7_B + SEG7_C +         		 SEG7_E + SEG7_F + SEG7_G   )
#define _h     (                 		    SEG7_C +          	 SEG7_E + SEG7_F + SEG7_G  )
//#define _I      (                                  				 SEG7_E + SEG7_F        )
#define _i      (                                  				 SEG7_E                            )
#define _J      (       	  SEG7_B + SEG7_C + SEG7_D             )
#define _k     (         	  SEG7_B + SEG7_C +         		 SEG7_E + SEG7_F + SEG7_G   )
#define _L      (                        			      SEG7_D + SEG7_E + SEG7_F   )
#define _M     (SEG7_A + SEG7_B + SEG7_C +         	 SEG7_E + SEG7_F     )
//#define _N     (SEG7_A + SEG7_B + SEG7_C +         		 SEG7_E + SEG7_F  )
#define _n     (                		    SEG7_C +          	 SEG7_E +          	    SEG7_G )
//#define _O     (SEG7_A + SEG7_B + SEG7_C + SEG7_D + SEG7_E + SEG7_F    )
#define _o     (                  		    SEG7_C + SEG7_D + SEG7_E +          	    SEG7_G  )
#define _P     (SEG7_A + SEG7_B +                    		 SEG7_E + SEG7_F + SEG7_G )
#define _q     (SEG7_A + SEG7_B + SEG7_C +                  		   SEG7_F +SEG7_G)
#define _r      (                                    				 SEG7_E +                SEG7_G  )
#define _S     (SEG7_A +          	    SEG7_C + SEG7_D +          	   SEG7_F + SEG7_G)
//#define _T     (SEG7_A +                            			 SEG7_E + SEG7_F   )
#define _t      (                          			      SEG7_D + SEG7_E + SEG7_F + SEG7_G )
//#define _U     (         	  SEG7_B + SEG7_C + SEG7_D + SEG7_E + SEG7_F)
#define _u     (               		    SEG7_C + SEG7_D + SEG7_E)
#define _v     (                SEG7_B + 		                                      SEG7_F + SEG7_G    )
#define _W  0x00                            
#define _X  0x00                            
#define _y     (         	  SEG7_B + SEG7_C + SEG7_D +         	  SEG7_F + SEG7_G    )
#define _z  0x00                            
#define _MINUS   (                                                    					   SEG7_G )
#define _SPACE  0x00                            
//#define _ii     (                		    SEG7_C+ 			 SEG7_E   )



#define _DIS_LCD_NUM_REG    24//LCD_RAM_SIZE/*"虚拟液晶数据缓冲区大小"*/
static INT8U  mShowRam[_DIS_LCD_NUM_REG];
/*"状态显示位数组，按状态提示符顺序按位表示状态提示符是否显示，1：显示  0：不显示"*/
static INT8U mStatusShow[_DIS_STA_NUM];
static INT8U mStatusTarfStep[2];
/*"状态显示位数组，按状态提示符顺序按位表示状态提示符是否闪烁，1：闪烁  0：不闪烁"*/
static INT8U mStatusFlash[_DIS_STA_NUM];
static ST_SHOW_CTRL mstDisCtrl;
#ifdef DIS_USER_CODE
static ST_USERCODE_SHOW mstUserCode;/*"用户代码信息"*/
#endif
 
/*"********状态提示符段位置列表  顺序需与状态提示符定义顺序一致*******"*/
/*"********if the X and Y are all 0xff, the show seg is not exist"*/
const ST_CHAR_POS mstTabStatus[] = {  
    {0xff, 0xff}, /*"阶梯1"*/
    {0xff, 0xff} , /*"阶梯2"*/
    {0xff, 0xff} , /*"阶梯3"*/
    {0xff, 0xff} , /*"阶梯4"*/
    {20,COM2},/*"低功耗电池"*/  
    {20,COM5},/*"公钥"*/
    {20,COM1},/*"通信电话符"*/  
    {20,COM6},/*"模块通信符"*/ 
    {20,COM1},/*"模块上行"*/
    {20,COM6},/*"模块下行"*/  
    {20,COM7},/*"蓝牙"*/   
    {0xff, 0xff},/*"闭锁"*/   
    {0xff, 0xff},/*"升级"*/     
    {20,COM3},/*"请购电"*/  
    {0xff, 0xff},/*"拉闸"*/  
    {0xff, 0xff},/*"透支"*/    
    
    {21,COM1},/*"尖 费率"*/  
    {21,COM2},/*"峰 费率"*/
    {21,COM3},/*"平 费率"*/    
    {21,COM4},/*"谷 费率"*/   
    {0xff, 0xff},/*"第一套时段"*/     
    {0xff, 0xff},/*"第二套时段"*/
    {0xff, 0xff},/*"第一套阶梯"*/     
    {0xff, 0xff},/*"第二套阶梯"*/      
    {20,COM4},/*"功率反向 "*/        
    
};
#define _STA_MINUS_ADDR 8 /*"负号SEG位置 "*/  
#define _STA_MINUS_BIT  COM8 /*"负号COM位置 "*/  
/*"主显示区信息提示符位置列表，顺序需与Char_SegBit中字符定义顺序一致"*/
const ST_CHAR_POS mstTabChar[] = {

    {0xff, 0xff},/*"组合"*/
    {0xff, 0xff},/*"上X月"*/
    {0xff, 0xff},/*"N"*/          
    {2,COM2},/*"号"*/ 
    {0xff, 0xff},/*"当前"*/    
    {0xff, 0xff},/*"上"*/   
    {0xff, 0xff},/*"L"*/          
    {0xff, 0xff},/*"B"*/ 

    {0xff, 0xff},/*"日期"*/ 
    {0,COM3},/*"向"*/
    {0xff, 0xff},/*"剩余"*/    
    {0xff, 0xff},/*"阶梯"*/ 
    {0,COM2},/*"反"*/    
    {0,COM1},/*"正"*/    
    {0xff, 0xff},/*"COSΦ"*/    
    {0xff, 0xff},/*"赊"*/         

    {0, COM7},  /*"Ⅰ"*/                            
    {0, COM8},  /*"Ⅱ"*/   
    {0xff, 0xff},/*"量"*/    
    {1,COM6},/*"电"*/ 
    {0, COM5},  /*"有"*/ 
    {0, COM6},  /*"功"*/     
    {0xff, 0xff},  /*"需"*/   
    {1, COM3},  /*"费率"*/    

    {0xff, 0xff},/*"常数"*/ 
    {2,COM1},/*"户"*/
    {0xff, 0xff},  /*"压"*/         
    {0xff, 0xff},  /*"失"*/   
    {1, COM2},  /*"V"*/       
    {1,COM5},/*"总"*/   
    {0xff, 0xff},/*"金额"*/
    {0xff, 0xff},/*"价"*/         

    {1,COM7},/*"表"*/  
    {0xff, 0xff},/*"段"*/
    {0xff, 0xff},/*"间"*/
    {0xff, 0xff},/*"时"*/        
    {0xff, 0xff},/*"用"*/       
    {0xff, 0xff},  /*"透支"*/        
    {0xff, 0xff},/*"支"*/      //CHAR_LIU,  /*"流"*/   
    {0xff, 0xff},  /*"功率"*/       
    
    {0xff, 0xff},/*"上N月预留"*/
    {0xff, 0xff},/*"上N月预留"*/
    {0xff, 0xff},/*"上N月预留"*/
    {0xff, 0xff},/*"上N月预留"*/
    {0xff, 0xff},/*"谷"*/   
    {0xff, 0xff},/*"平"*/    
    {0xff, 0xff},/*"峰"*/     
    {0xff, 0xff},/*"尖"*/
    {0xff, 0xff}/*"1"*/      
};

/*"********主显示区8字小数点字符段位置列表，按8字顺序排列*******"*/
const ST_CHAR_POS mstTabPoint[MAX_POINT_NUM] = {
        {17,COM8},   /*"dp2"*/
        {16,COM8},   /*"dp3"*/
        {15,COM8},   /*"dp5"*/
        {14,COM8},   /*"dp6"*/  
        {3,COM8},    /*"dp1"*/
         
};
#if DIS_USER_CODE
/*"********用户显示代码区8字后小数点字符段位置列表，按8字顺序排列*******"*/
const ST_CHAR_POS mstUserTabPoint[MAX_USECODE_NUM] = {
        {0xff, 0xff},   /*""*/
        { 3, _COM1},   /*"p10"*/
        { 2, _COM1},   /*"p9"*/
        { 1, _COM1},   /*"p8"*/
        { 0, _COM1},   /*"p7"*/
};
#endif
/*"********主显示区时间格式小数点字符段位置列表，按时间数据顺序排列*******"*/
const ST_CHAR_POS mstTabTPoint[MAX_TPOINT_NUM] = {
    {11,COM8},   /*"dP4"*/
    {10,COM8},   /*"dP7"*/
};

/*"用一个字节表示单位字段，单位配置只需配置该字节即可"*/
/*" * b0-----kW"*/
/*"   b1-----元"*/
/*"   b2-----V"*/
/*"   b3-----A"*/
/*"   b4-----h"*/


/*"********单位配置列表，按enum SHOW_UNIT结构定义顺序排列"*/
/*"************ 为0标识该液晶不支持该单位显示*******"*/
const INT8U mTabDecodeUnit[MAX_UINT_NUM] = {
        0	,	/*"	E_NONE_UNIT,	"*/
        0x0f	,	/*"	E_KWH_UNIT,	"*/
        0x0e	,	/*"	E_WH_UNIT,	"*/
        0x07	,	/*"	E_KW_UNIT,	"*/
        0x06	,	/*"	E_W_UNIT,	"*/
        0x02	,	/*"	E_V_UNIT,	"*/
        0x10	,	/*"	E_A_UNIT,	"*/
        0	,	/*"	E_SECOND_UNIT,	"*/
        0	,	/*"	E_MINUTE_UNIT,	"*/
        0	,	/*"	E_HOUR_UNIT,	"*/
        0	,	/*"	E_DAY_UNIT,	"*/
        0	,	/*"	E_DEGREE_UNIT,	"*/
        0	,	/*"	E_HZ_UNIT,	"*/
        0x00	,	/*"	E_YUAN_UNIT,	"*/
        0x60	,	/*"    E_KVARH_UNIT,	"*/


        
};


/*"********单位段码位位置列表，Decode_Unit_list中定义位顺序排列*******"*/
const ST_CHAR_POS mstTabUnit[MAX_LCD_UINT_NUM] = {
    {18,COM4},   /*"k "*/  
    {18,COM5},   /*"V1 "*/         
    {18,COM6},  /*"V2"*/    
    {18,COM8},   /*"h"*/   
    
    {18,COM7},   /*"A"*/
    {19,COM2},   /*"h"*/
    {19,COM1},   /*"Kvar"*/

    
};

/*将HEX码映射到具体字符上*/
const INT8U mDigital[DIS_MAX] =
{
    _0,_1,_2,_3,_4,_5,_6,_7,_8,_9,
    _A,_b,_C,_d,_E,_F,_g,_h,_i,_J,_k,_L,_M,_n,_o,_P,_q,_r,_S,_t,_u,_v,_W,_X,_y,_z,_SPACE,_MINUS
};

typedef struct
{
    INT8U addr ;
    INT8U bit ;
}ST_NUM_POS;

/*"数字的位置和偏移"*/
/*" 数字在液晶排列的位置 每个数字占2位 从最右边开始 依次向左,左上角的小8字为最后2位"*/
#define  DIS_NUM_CNT  (17)
#define DIS_NUM_TARF_ADDR (16) /*"第几费率"*/
#define DIS_NUM_STEP_ADDR (15) /*"第几阶梯"*/
#define DIS_YEAR_ADDR (10) /*"年"*/
#define DIS_MONTH_ADDR (12) /*"月"*/
#define DIS_INDEX_ADDR (12)  /*"屏序号"*/
#define DIS_TARF_ADDR  (14) /*"屏显提示第几费率"*/
const ST_CHAR_POS mstTabNum[(DIS_NUM_CNT*7)] =
{
    {	8,COM1},  /*"A"*/                /*"0"*/
    {	8,COM2}, /*"B"*/
    {	8,COM3}, /*"C"*/
    {	8,COM4}, /*"D"*/
    {	8,COM5}, /*"E"*/
    {	8,COM6},   /*"F"*/
    {	8,COM7}, /*"G"*/   

    {	9,COM1}, /*"A"*/   /*"1"*/
    {	9,COM2},/*"B"*/
    {	9,COM3},/*"C"*/
    {	9,COM4},/*"D"*/
    {	9,COM5},/*"E"*/
    {	9,COM6},/*"F"*/
    {	9,COM7},/*"G"*/ 

    {	10,COM1},  /*"A"*/                /*"2"*/
    {	10,COM2}, /*"B"*/
    {	10,COM3}, /*"C"*/
    {	10,COM4}, /*"D"*/
    {	10,COM5}, /*"E"*/
    {	10,COM6},   /*"F"*/
    {	10,COM7}, /*"G"*/   

    {	11,COM1}, /*"A"*/   /*"3"*/
    {	11,COM2},/*"B"*/
    {	11,COM3},/*"C"*/
    {	11,COM4},/*"D"*/
    {	11,COM5},/*"E"*/
    {	11,COM6},/*"F"*/
    {	11,COM7},/*"G"*/ 

    {	12,COM1}, /*"A"*/                /*"4"*/
    {	12,COM2},/*"B"*/
    {	12,COM3},/*"C"*/
    {	12,COM4},/*"D"*/
    {	12,COM5},/*"E"*/
    {	12,COM6},/*"F"*/
    {	12,COM7},/*"G"*/   


    {	13,COM1},/*"	A	"*/	/*"5"*/
    {	13,COM2},/*"	B	"*/	
    {	13,COM3},/*"	C	"*/	
    {	13,COM4},/*"	D	"*/	
    {	13,COM5},/*"	E	"*/	
    {	13,COM6},/*"	F	"*/	
    {	13,COM7},/*"	G	"*/	


    {	14,COM1},/*"	A	"*/	/*"6"*/
    {	14,COM2},/*"	B	"*/	
    {	14,COM3},/*"	C	"*/	
    {	14,COM4},/*"	D	"*/	
    {	14,COM5},/*"	E	"*/	
    {	14,COM6},/*"	F	"*/	
    {	14,COM7},/*"	G	"*/	

    {	15,COM1}	,/*"	A	"*/	/*"7"*/
    {	15,COM2}	,/*"	B	"*/	
    {	15,COM3},/*"	C	"*/	
    {	15,COM4}	,/*"	D	"*/	
    {	15,COM5},/*"	E	"*/	
    {	15,COM6},/*"	F	"*/	
    {	15,COM7},/*"	G	"*/	

    {	16,COM1}	,/*"	A	"*/	/*"8"*/
    {	16,COM2}	,/*"	B	"*/	
    {	16,COM3},/*"	C	"*/	
    {	16,COM4}	,/*"	D	"*/	
    {	16,COM5},/*"	E	"*/	
    {	16,COM6},/*"	F	"*/	
    {	16,COM7},/*"	G	"*/	
    
    {	17,COM1}	,/*"	A	"*/	/*"9"*/
    {	17,COM2}	,/*"	B	"*/	
    {	17,COM3},/*"	C	"*/	
    {	17,COM4}	,/*"	D	"*/	
    {	17,COM5},/*"	E	"*/	
    {	17,COM6},/*"	F	"*/	
    {	17,COM7},/*"	G	"*/	


    {	3,COM1},/*"	A	"*/	/*"10"*/
    {	3,COM2},/*"	B	"*/	
    {	3,COM3},/*"	C	"*/	
    {	3,COM4},/*"	D	"*/	
    {	3,COM5},/*"	E	"*/	
    {	3,COM6},/*"	F	"*/	
    {	3,COM7},/*"	G	"*/	

    {	4,COM1},/*"	A	"*/	/*"11"*/
    {	4,COM2},/*"	B	"*/	
    {	4,COM3},/*"	C	"*/	
    {	4,COM4},/*"	D	"*/	
    {	4,COM5},/*"	E	"*/	
    {	4,COM6},/*"	F	"*/	
    {	4,COM7},/*"	G	"*/	

    {	5,COM1},/*"	A	"*/	/*"12"*/
    {	5,COM2},/*"	B	"*/	
    {	5,COM3},/*"	C	"*/	
    {	5,COM4},/*"	D	"*/	
    {	5,COM5},/*"	E	"*/	
    {	5,COM6},/*"	F	"*/	
    {	5,COM7},/*"	G	"*/	

    {	6,COM1},/*"	A	"*/	/*"13"*/
    {	6,COM2},/*"	B	"*/	
    {	6,COM3},/*"	C	"*/	
    {	6,COM4},/*"	D	"*/	
    {	6,COM5},/*"	E	"*/	
    {	6,COM6},/*"	F	"*/	
    {	6,COM7},/*"	G	"*/	

    {	7,COM1},/*"	A	"*/	/*"14"*/    
    {	7,COM2},/*"	B	"*/	
    {	7,COM3},/*"	C	"*/	
    {	7,COM4},/*"	D	"*/	
    {	7,COM5},/*"	E	"*/	
    {	7,COM6},/*"	F	"*/	
    {	7,COM7},/*"	G	"*/	

    {	22,COM1},/*"	A	"*/	/*"15"*/
    {	22,COM2},/*"	B	"*/	
    {	22,COM3},/*"	C	"*/	
    {	22,COM4},/*"	D	"*/	
    {	22,COM5},/*"	E	"*/	
    {	22,COM6},/*"	F	"*/	
    {	22,COM7},/*"	G	"*/	

    {	23,COM1},/*"	A	"*/	/*"16"*/
    {	23,COM2},/*"	B	"*/	
    {	23,COM3},/*"	C	"*/	
    {	23,COM4},/*"	D	"*/	
    {	23,COM5},/*"	E	"*/	
    {	23,COM6},/*"	F	"*/	
    {	23,COM7},/*"	G	"*/	

};

const ST_CHAR_POS mstTabLcd[192] =
{
    {LCD_SEG26,COM1},/*"正"*/    
    {LCD_SEG25,COM1},/*"反"*/ 
    {LCD_SEG24,COM1},/*"向"*/
    {LCD_SEG23, COM1},  /*"无"*/ 
    {LCD_SEG22, COM1},  /*"有"*/ 
    {LCD_SEG21, COM1},  /*"功"*/ 
    {LCD_SEG20, COM1},  /*"Ⅰ"*/     
    {LCD_SEG19, COM1},  /*"Ⅱ"*/   
    
    {LCD_SEG18, COM1},  /*"III"*/     
    {LCD_SEG17, COM1},  /*"V"*/         
    {LCD_SEG15, COM1},  /*"费"*/    
    {LCD_SEG14, COM1},  /*"率"*/    
    {LCD_SEG6,COM3},/*"总"*/   
    {LCD_SEG6,COM4},/*"电量"*/ 
    {LCD_SEG6,COM5},/*"表"*/  
    {LCD_SEG3, COM1},/*"1"*/  
    
    {LCD_SEG7,COM5},/*"户"*/
    {LCD_SEG7,COM4},/*"号"*/ 
    {0xff, 0xff},
    {0xff, 0xff},
    {0xff, 0xff},
    {0xff, 0xff},
    {0xff, 0xff},
    {0xff, 0xff},

    {	LCD_SEG8,COM1	},/*"	A	"*/	/*"10"*/
    {	LCD_SEG9,COM3	},/*"	B	"*/	
    {	LCD_SEG9,COM4},/*"	C	"*/	
    {	LCD_SEG8,COM4},/*"	D	"*/	
    {	LCD_SEG8,COM3},/*"	E	"*/	
    {	LCD_SEG7,COM3},/*"	F	"*/	
    {	LCD_SEG8,COM2},/*"	G	"*/	
    {LCD_SEG13,COM6},    /*"dp1"*/


    {	LCD_SEG9,COM1},/*"	A	"*/	/*"11"*/
    {	LCD_SEG10,COM1},/*"	B	"*/	
    {	LCD_SEG13,COM4},/*"	C	"*/	
    {	LCD_SEG10,COM4	},/*"	D	"*/	
    {	LCD_SEG10,COM3},/*"	E	"*/	
    {	LCD_SEG9,COM2},/*"	F	"*/	
    {	LCD_SEG10,COM2},/*"	G	"*/	
    {0xff, 0xff},
    

    {	LCD_SEG13,COM1},/*"	A	"*/	/*"12"*/
    {	LCD_SEG12,COM3},/*"	B	"*/	
    {	LCD_SEG12,COM5},/*"	C	"*/	
    {	LCD_SEG12,COM6	},/*"	D	"*/	
    {	LCD_SEG13,COM5},/*"	E	"*/	
    {	LCD_SEG13,COM2},/*"	F	"*/	
    {	LCD_SEG13,COM3},/*"	G	"*/	
    {0xff, 0xff},

    {	LCD_SEG12,COM1},/*"	A	"*/	/*"13"*/
    {	LCD_SEG11,COM1},/*"	B	"*/	
    {	LCD_SEG11,COM6},/*"	C	"*/	
    {	LCD_SEG11,COM5	},/*"	D	"*/	
    {	LCD_SEG11,COM3},/*"	E	"*/	
    {	LCD_SEG12,COM2},/*"	F	"*/	
    {	LCD_SEG11,COM2},/*"	G	"*/	
    {0xff, 0xff},

    {	LCD_SEG4,COM1},/*"	A	"*/	/*"14"*/    
    {	LCD_SEG5,COM1},/*"	B	"*/	
    {	LCD_SEG7,COM2},/*"	C	"*/	
    {	LCD_SEG6,COM2},/*"	D	"*/	
    {	LCD_SEG6,COM1},/*"	E	"*/	
    {	LCD_SEG5,COM2},/*"	F	"*/	
    {	LCD_SEG7,COM1},/*"	G	"*/
    {0xff, 0xff},

    {LCD_SEG29, COM3},/*"0"*/ 
    {LCD_SEG28, COM4},
    {LCD_SEG28, COM6},
    {LCD_SEG29, COM6},
    {LCD_SEG29, COM5},
    {LCD_SEG29, COM4},
    {LCD_SEG28, COM5},
    {LCD_SEG1, COM4},/*"负号"*/

    {LCD_SEG27, COM3},/*"1"*/ 
    {LCD_SEG26, COM2},
    {LCD_SEG26, COM4},
    {LCD_SEG27, COM6},
    {LCD_SEG27, COM5},
    {LCD_SEG27, COM4},
    {LCD_SEG26, COM3},
    {LCD_SEG22, COM2},    /*"P10"*/ 

    {	LCD_SEG25,COM2},  /*"A"*/                /*"2"*/
    {	LCD_SEG24,COM2}, /*"B"*/
    {	LCD_SEG24,COM4}, /*"C"*/
    {	LCD_SEG25,COM5}, /*"D"*/
    {	LCD_SEG25,COM4}, /*"E"*/
    {	LCD_SEG25,COM3},   /*"F"*/
    {	LCD_SEG24,COM3}, /*"G"*/   
    {LCD_SEG18,COM2},    /*"P9"*/     

    {	LCD_SEG23,COM2}, /*"A"*/   /*"3"*/
    {	LCD_SEG22,COM3},/*"B"*/
    {	LCD_SEG22,COM5},/*"C"*/
    {	LCD_SEG23,COM5},/*"D"*/
    {	LCD_SEG23,COM4},/*"E"*/
    {	LCD_SEG23,COM3},/*"F"*/
    {	LCD_SEG22,COM4},/*"G"*/ 
    {LCD_SEG14,COM2},    /*"P8"*/     

    {	LCD_SEG21,COM2},  /*"A"*/                /*"4"*/
    {	LCD_SEG20,COM2}, /*"B"*/
    {	LCD_SEG20,COM4}, /*"C"*/
    {	LCD_SEG21,COM5}, /*"D"*/
    {	LCD_SEG21,COM4}, /*"E"*/
    {	LCD_SEG21,COM3},/*"F"*/
    {	LCD_SEG20,COM3}, /*"G"*/   
    {LCD_SEG22, COM6},    /*"P7"*/ 

    {	LCD_SEG19,COM2}, /*"A"*/                /*"5"*/
    {	LCD_SEG18,COM3},/*"B"*/
    {	LCD_SEG18,COM5},/*"C"*/
    {	LCD_SEG19,COM5},/*"D"*/
    {	LCD_SEG19,COM4},/*"E"*/
    {	LCD_SEG19,COM3},/*"F"*/
    {	LCD_SEG18,COM4},/*"G"*/   
    {LCD_SEG20,COM5},    /*"P6"*/ 

    {	LCD_SEG17,COM2}, /*"A"*/                /*"6"*/
    {	LCD_SEG16,COM2},/*"B"*/
    {	LCD_SEG16,COM4},/*"C"*/
    {	LCD_SEG17,COM5},/*"D"*/
    {	LCD_SEG17,COM4},/*"E"*/
    {	LCD_SEG17,COM3},/*"F"*/
    {	LCD_SEG16,COM3},/*"G"*/   
    {LCD_SEG18,COM6},    /*"P5"*/ 

    {	LCD_SEG15,COM2},/*"	A	"*/	/*"7"*/
    {	LCD_SEG14,COM3},/*"	B	"*/	
    {	LCD_SEG14,COM5},/*"	C	"*/	
    {	LCD_SEG15,COM5},/*"	D	"*/	
    {	LCD_SEG15,COM4},/*"	E	"*/	
    {	LCD_SEG15,COM3},/*"	F	"*/	
    {	LCD_SEG14,COM4},/*"	G	"*/	
    {LCD_SEG16, COM1},    /*"P4"*/ 

    {	LCD_SEG2,COM1	},/*"	A	"*/	/*"8"*/
    {	LCD_SEG3,COM2	},/*"	B	"*/	
    {	LCD_SEG3,COM4},/*"	C	"*/	
    {	LCD_SEG2,COM4	},/*"	D	"*/	
    {	LCD_SEG2,COM3	},/*"	E	"*/	
    {	LCD_SEG2,COM2	},/*"	F	"*/	
    {	LCD_SEG3,COM3	},/*"	G	"*/	
    {LCD_SEG14,COM6},    /*"P3"*/ 
    
    {	LCD_SEG4,COM2		},/*"	A	"*/	/*"9"*/
    {	LCD_SEG5,COM3		},/*"	B	"*/	
    {	LCD_SEG5,COM5	},/*"	C	"*/	
    {	LCD_SEG4,COM5		},/*"	D	"*/	
    {	LCD_SEG4,COM4	},/*"	E	"*/	
    {	LCD_SEG4,COM3	},/*"	F	"*/	
    {	LCD_SEG5,COM4	},/*"	G	"*/	
    {LCD_SEG3,COM5},  /*"P2"*/     

    {LCD_SEG4, COM6},    /*"°"*/         
    {LCD_SEG5, COM6},    /*"C"*/       
    {LCD_SEG6, COM6},    /*"OSΦ"*/     
    {LCD_SEG7,COM6},   /*"k "*/  
    {LCD_SEG8,COM5},   /*"V1  W"*/     
    {LCD_SEG8, COM6},   /*"V2  "*/     
    {LCD_SEG9,COM5},   /*"A"*/
    {LCD_SEG9,COM6},   /*"h"*/

    {LCD_SEG10,COM6},   /*"Kvar "*/
    {LCD_SEG10, COM5},   /*"h"*/
    {0xff, 0xff},
    {0xff, 0xff},
    {0xff, 0xff},
    {0xff, 0xff},
    {0xff, 0xff},
    {0xff, 0xff},    

    {LCD_SEG24,COM5},/*"模块下行"*/  
    {LCD_SEG23,COM6},/*"低功耗电池"*/  
    {LCD_SEG3,COM6},/*"请购电"*/  
    {LCD_SEG26,COM6},/*"功率反向 "*/     
    {LCD_SEG26,COM5},/*"公钥"*/
    {LCD_SEG25,COM6},/*"模块上行"*/
    {LCD_SEG24,COM6},/*"蓝牙"*/   
    {0xff, 0xff},

    {LCD_SEG1,COM2},/*"尖 费率"*/  
    {LCD_SEG1,COM3},/*"峰 费率"*/
    {LCD_SEG1,COM5},/*"平 费率"*/    
    {LCD_SEG1,COM6},/*"谷 费率"*/   
    {LCD_SEG1, COM1},/*"L"*/   
    {LCD_SEG21, COM6},/*"T"*/       
    {LCD_SEG20, COM6},/*"T2"*/  
    {0xff, 0xff},

    {LCD_SEG28, COM1},/*"15"*/ 
    {LCD_SEG27, COM1},
    {LCD_SEG27, COM2},
    {LCD_SEG28, COM3},
    {LCD_SEG29, COM2},
    {LCD_SEG29, COM1},   
    {LCD_SEG28, COM2},
    {0xff, 0xff},   
    
    {LCD_SEG15, COM6},/*"16"*/ 
    {LCD_SEG2, COM5},
    {LCD_SEG2, COM6},
    {LCD_SEG19, COM6},
    {LCD_SEG17, COM6},
    {LCD_SEG16, COM5},   
    {LCD_SEG16, COM6},
    {0xff, 0xff},   
    
};
    
/*"-----------------------PRIVATE FUNCTION------------------------"*/
static void DecodeStatus(void);
static void DecodeData(ST_SHOW_DATA *pShowDataL1);
static void DecodeChar(INT8U *pChar);
static void DecodeUnit(INT8U unit);
//static void DecodeMonth(INT8U inMonth);
static void ShowPoint(INT8U num);
static void ShowTPoint(INT8U num);
static void ClearShowRam(void);
static void ShowFigure(INT8U Addr, INT8U Num);
static void DisAll(void);
#ifdef DIS_USER_CODE
static void DecodeUserCode(void);
static void ShowUserPoint(INT8U num);
#endif
static void DIS_ClrAllStauSeg(void);
static void DecodeMinus(INT8U minus);
static void DecodeIndex(INT8U Index);
static void DecodeBillDate(INT8U *BillDate);
static void DecodeBCDByte(INT8U Addr, INT8U InBcd);
#ifdef DIS_QUAD_INFO
static void DecodeQuad(INT8U quad);
#endif
void DIS_ShowData(INT8U *inData, INT8U *outData);
INT8U DIS_PrintLcdLine3(INT8U *pShowData, INT16U LastTime, EN_SHOW_PRIORITY ShowPrio);

/*"*****************************************************************************"*/
/*"  Function:       DIS_init"*/
/*"  Description:    初始化虚拟液晶"*/
/*"  Calls:          "*/
/*"  Called By:      DM"*/
/*"  Input:          无"*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U tmpggIndex;
void DIS_init(void)
{
    ClearShowRam();
    DIS_ClrAllStauSeg();
    mstDisCtrl.currPrior = IDLE;
    mstDisCtrl.flag.byte = 0;
#ifdef DIS_USER_CODE
    LIB_MemSet(0, (INT8U *)&mstUserCode, sizeof(ST_USERCODE_SHOW));
#endif
tmpggIndex = 0;
}

/*"*****************************************************************************"*/
/*"  Function:       DIS_SetStauSeg"*/
/*"  Description:    状态显示字符显示置位，各功能模块字符显示函数接口"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块"*/
/*"  Input:          SegId  定义的状态显示符名称                  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void DIS_SetStauSeg(INT8U SegId)
{
#ifdef DIS_RELAY_ICON_COMB
    INT8U tmpSeg;
#endif

    if(SegId < MAX_BYTE_STA)
    {
        mStatusShow[SegId >> 3] |= (0x01 << (SegId & 0x07));
    }

#ifdef DIS_RELAY_ICON_COMB
    if((STA_SW_CLOSE == SegId) || (STA_SW_OPEN == SegId))
    {
        tmpSeg = STA_SW;
        mStatusShow[tmpSeg >> 3] |= (0x01 << (tmpSeg & 0x07));
    }
#endif
}

void DIS_SetStauTarfStep(INT8U SegId,INT8U Num)
{
    if((Num < 10)&&(SegId < 2))
    {
        DIS_SetStauSeg(SegId);
        mStatusTarfStep[SegId] = Num;
    }
}
void DIS_ClrStauTarfStep(INT8U SegId)
{
    if((SegId < 2))
    {
        DIS_ClrStauSeg(SegId);
        mStatusTarfStep[SegId] = 0;
    }
}
/*"*****************************************************************************"*/
/*"  Function:       Clear_StatusBit_Show"*/
/*"  Description:    状态显示字符显示清零，各功能模块字符显示函数接口"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块"*/
/*"  Input:          SegBit_name 定义的状态显示符名称                 "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#ifdef DIS_RELAY_ICON_COMB
#define  SEGID_CLOSE    (mStatusShow[STA_SW_CLOSE >> 3] & (0x01 << (STA_SW_CLOSE & 0x07)))
#define  SEGID_OPEN      (mStatusShow[STA_SW_OPEN >> 3] & (0x01 << (STA_SW_OPEN & 0x07)))
#endif

void DIS_ClrStauSeg(INT8U SegId)
{
#ifdef DIS_RELAY_ICON_COMB
    INT8U tmpSeg;
#endif

    if(SegId < MAX_BYTE_STA)
    {
        mStatusShow[SegId >> 3] &= ~(0x01 << (SegId & 0x07));
    }

#ifdef DIS_RELAY_ICON_COMB
    if(((STA_SW_CLOSE == SegId) && (0 == SEGID_OPEN)) ||
    ((STA_SW_OPEN == SegId) && (0 == SEGID_CLOSE)))
    {
        tmpSeg = STA_SW;
        mStatusShow[tmpSeg >> 3] &= ~(0x01 << (tmpSeg & 0x07));
    }
#endif
}

/*"*****************************************************************************"*/
/*"  Function:       DIS_ClrAllStauSeg"*/
/*"  Description:    清所有状态显示字符显示和闪烁标志"*/
/*"  Calls:"*/
/*"  Called By:      Send_ShowData_To_LCD，低功耗初始化接口函数"*/
/*"  Input:"*/
/*"  Return:"*/
/*"  Others:"*/
/*"*****************************************************************************"*/
static void DIS_ClrAllStauSeg(void)
{
    LIB_MemSet(0, mStatusShow, _DIS_STA_NUM);
    LIB_MemSet(0, mStatusFlash, _DIS_STA_NUM);
    LIB_MemSet(0, mStatusTarfStep, 2);    
}

/*"*****************************************************************************"*/
/*"  Function:       DIS_FlashStauSeg"*/
/*"  Description:    状态显示字符闪烁置位，各功能模块字符闪烁函数接口"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块"*/
/*"  Input:          SegBit_name 定义的状态显示符名称                "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
void DIS_FlashStauSeg(INT8U SegId)
{
#ifdef DIS_RELAY_ICON_COMB
    INT8U tmpSeg;
#endif

    if(SegId < MAX_BYTE_STA)
    {
        mStatusFlash[SegId >> 3] |= (0x01 << (SegId & 0X07));
    }

#ifdef DIS_RELAY_ICON_COMB
    if((STA_SW_CLOSE == SegId) || (STA_SW_OPEN == SegId))
    {
        tmpSeg = STA_SW;
        mStatusFlash[tmpSeg >> 3] |= (0x01 << (tmpSeg & 0x07));
    }
#endif
}

/*"*****************************************************************************"*/
/*"  Function:       DIS_ClrFlashStauSeg"*/
/*"  Description:    状态显示字符闪烁清零,各功能模块字符闪烁函数接口"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块"*/
/*"  Input:          SegBit_name 定义的状态显示符名称                     "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#ifdef DIS_RELAY_ICON_COMB
#define  SEGID_F_CLOSE    (mStatusFlash[STA_SW_CLOSE >> 3] & (0x01 << (STA_SW_CLOSE & 0x07)))
#define  SEGID_F_OPEN      (mStatusFlash[STA_SW_OPEN >> 3] & (0x01 << (STA_SW_OPEN & 0x07)))
#endif

void DIS_ClrFlashStauSeg(INT8U SegId)
{
#ifdef DIS_RELAY_ICON_COMB
    INT8U tmpSeg;
#endif

    if(SegId < MAX_BYTE_STA)
    {
        mStatusFlash[SegId >> 3] &= ~(0x01 << (SegId & 0X07));
    }
#ifdef DIS_RELAY_ICON_COMB
    if(((STA_SW_CLOSE == SegId) && (0 == SEGID_F_OPEN)) ||
    ((STA_SW_OPEN == SegId) && (0 == SEGID_F_CLOSE)))
    {
        tmpSeg = STA_SW;
        mStatusFlash[tmpSeg >> 3] &= ~(0x01 << (tmpSeg & 0X07));
    }
#endif
}

/*"*****************************************************************************"*/
/*"  Function:       DecodeStatus"*/
/*"  Description:    状态字符显示"*/
/*"  Calls:          "*/
/*"  Called By:      DIS_PrintLcdLine1"*/
/*"  Input:           "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void DecodeStatus(void)
{
    INT8U pos_x, pos_y;
    INT8U uni;

    for(uni = 0; uni < MAX_BYTE_STA; uni++)
    {
        pos_x = mstTabStatus[uni].X_Pos;
        pos_y = mstTabStatus[uni].Y_Pos;

        if((0xff != pos_x) && (0xff != pos_y))
        {
            /*"检查某段位是否需要闪烁，如果需要闪烁，不能进行显示处理"*/
            if(mStatusFlash[uni >> 3] & (INT8U)(0x01 << (uni & 0X07)))
            {
                if(mstDisCtrl.flag.bits.flash == 0)
                {
                    mShowRam[pos_x] |=  pos_y;
                }
                else
                {
                    mShowRam[pos_x] &= ~pos_y;
                }
            }
            else
            {
                if(mStatusShow[uni >> 3] & (INT8U)(0x01 << (uni & 0X07)))
                {
                    mShowRam[pos_x] |=  pos_y;
                }
                else
                {
                    mShowRam[pos_x] &= ~pos_y;
                }
            }
        }
    }        
}
    
/*"*****************************************************************************"*/
/*"  Function:       DIS_PrintLcdLine1"*/
/*"  Description:    第一行数字显示区数字显示（虚拟液晶）"*/
/*"  Calls:          "*/
/*"  Called By:      各功能模块"*/
/*"  Input:       pShowDataL1：液晶主显示区显示内容指针 "*/
/*"                  LastTime：显示持续时间（单位s）"*/
/*"                  ShowPrio：显示优先级"*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U DIS_PrintLcdLine1(ST_SHOW_DATA *pShowDataL1, INT16U LastTime, EN_SHOW_PRIORITY ShowPrio)
{
    INT8U ret;

    ret = FALSE;
    if(NULL != pShowDataL1)
    {
        if((mstDisCtrl.currPrior <= ShowPrio) || (IDLE == ShowPrio))/*"请求显示优先级高于当前优先级"*/
        {
            mstDisCtrl.currPrior = ShowPrio;
            mstDisCtrl.lastTime = LastTime;
            /*"主显示屏显示字符时，屏幕上只显示字符，不显示其他任何内容"*/

            ClearShowRam();
            
            if(_SHOW_ALL_ == pShowDataL1->Protc.showDataBuff[7])
            {
                DisAll();
            }
            else if(_SHOW_NONE_ == pShowDataL1->Protc.showDataBuff[7])
            {
                ;
            }            
            else
            {
                DecodeData(pShowDataL1);
                DecodeMinus(pShowDataL1->charAdd[1] >> 4);

#ifdef DIS_USER_CODE
                if(mstDisCtrl.flag.bits.code)
                {
                    DecodeUserCode();
                    mstDisCtrl.flag.bits.code = 0;
                }
#endif
#ifdef DIS_QUAD_INFO
                DecodeQuad(pShowDataL1->charAdd[2] & 0x1F);     
#endif

                DecodeStatus();
#if 0
                DecodeMonth(pShowDataL1->charAdd[3]);     
#endif
                DecodeUnit(pShowDataL1->Protc.stShowTable.stShowFormat.unit); 
                DecodeChar(pShowDataL1->Protc.stShowTable.schar); 
                DecodeIndex(pShowDataL1->charAdd[2]);
                /*"当前屏显示费率"*/
                if(pShowDataL1->Protc.stShowTable.schar[(CHAR_TARIFF>>3)]&(1<<(CHAR_TARIFF%8)))
                {
                    ShowFigure(DIS_NUM_TARF_ADDR, (pShowDataL1->charAdd[0] >>4));
                }
                else if (pShowDataL1->Protc.stShowTable.schar[(CHAR_STEP>>3)]&(1<<(CHAR_STEP%8)))
                {
                    ShowFigure(DIS_NUM_STEP_ADDR, (pShowDataL1->charAdd[0] >>4));
                }
                if(pShowDataL1->charAdd[4] != 0)
                {
                    DecodeBillDate(&pShowDataL1->charAdd[3]);
                }
            }
            ret = TRUE;
        }
        else /*"请求显示优先级低于当前优先级"*/
        {
#if 0
            if((_SHOW_ALL_ != pShowDataL1->Protc.showDataBuff[7]) &&
            (_SHOW_NONE_ != pShowDataL1->Protc.showDataBuff[7]))
            {
                DecodeStatus();
                ret = TRUE;
            }      
#endif
        }
        //Dis_test();
        if(TRUE == ret)
        {
            //LIB_MemSet(0xFF,( INT8U *)&mShowRam[0], _DIS_LCD_NUM_REG);
            INT8U tmpShowRam[LCD_RAM_SIZE];
            LIB_MemSet(0, tmpShowRam,LCD_RAM_SIZE);
            if(    mShowRam[19] == 0xa5)
            {
                LIB_MemSet(0xff, tmpShowRam,LCD_RAM_SIZE);
            }
            else
            {
                DIS_ShowData(mShowRam,tmpShowRam);
            }

            
            LCD_Refresh(tmpShowRam);
        }
    }

    return ret;
}

void DIS_ShowData(INT8U *inData, INT8U *outData)
{
    INT8U i,j;

    for(i = 0; i < _DIS_LCD_NUM_REG;i++)
    {
        for(j = 0;j<8;j++)
        {
            if(inData[i]&(0x01 << j))
            {
                outData[mstTabLcd[i*8+j].X_Pos] |= mstTabLcd[i*8+j].Y_Pos;
            }
        }
    }

}
INT8U DIS_PrintLcdLine3(INT8U *pShowData, INT16U LastTime, EN_SHOW_PRIORITY ShowPrio)
{
    INT8U ret,i;
    INT8U tmpShowRam[LCD_RAM_SIZE];
    ret = FALSE;
    if(NULL != pShowData)
    {
        if((mstDisCtrl.currPrior <= ShowPrio) || (IDLE == ShowPrio))/*"请求显示优先级高于当前优先级"*/
        {
            mstDisCtrl.currPrior = ShowPrio;
            mstDisCtrl.lastTime = LastTime;
            LIB_MemSet(0,mShowRam, _DIS_LCD_NUM_REG);
            LIB_MemCpy(pShowData, mShowRam, 20);
            for(i=0;i<8;i++)
            {
                if(pShowData[21]&(0x01 << i))
                {
                    if(mstDisCtrl.flag.bits.flash)
                    {
                        mShowRam[20] |= (0x01 << i);
                    }
                    else
                    {
                        ;
                    }
                }
                else if(pShowData[20]&(0x01 << i))
                {
                    mShowRam[20] |= (0x01 << i);
                }
            }
            LIB_MemCpy(&pShowData[22], &mShowRam[21], 3);

            LIB_MemSet(0, tmpShowRam,LCD_RAM_SIZE);
            DIS_ShowData(mShowRam,tmpShowRam);
            
            LCD_Refresh(tmpShowRam);
        }
    }

    return ret;
}
void Dis_test(void)
{

           
    ClearShowRam();
    DIS_SetStauSeg(STA_STEP_1);


    ShowFigure(0,8);
    //mShowRam[0] = 0x0f;
    //ShowFigure(2,2);

    ShowFigure(1,8);
    ShowFigure(2,8);
    ShowFigure(3,8);
    ShowFigure(4,8);
    ShowFigure(5,8);
    ShowFigure(6,8);   
    ShowFigure(7,8);
    ShowFigure(8,8);
    ShowFigure(9,8);
    ShowFigure(10,8);
    ShowFigure(11,8);
    ShowFigure(12,8);
    ShowFigure(13,8);
    ShowFigure(14,8);
    ShowFigure(15,8);

DIS_SetStauSeg(STA_WAVE);
DIS_SetStauSeg(STA_WAVE_UP);
//DIS_SetStauSeg(STA_WAVE_MID);
DIS_SetStauSeg(STA_WAVE_DOWN);
DIS_SetStauSeg(STA_BLUETOOTH);

DecodeStatus();    

    LCD_Refresh((INT8U *)mShowRam);    


 
    
}
/*"*****************************************************************************"*/
/*"  Function:       DecodeShowData"*/
/*"  Description:    按照要求显示格式，将显示数据进行格式解码，解码结果保存在主数据"*/
/*"                  区数据缓存中"*/
/*"  Calls:          "*/
/*"  Called By:      DIS_PrintLcdLine1"*/
/*"  Input:          pData  需要显示数据内容地址指针"*/
/*"                  format 需要显示数据的显示格式"*/
/*"                  len    需要显示数据长度"*/
/*"  Return:        "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void DecodeData(ST_SHOW_DATA *pShowDataL1)
{
    INT8U uni, exp, len;

    if(pShowDataL1->Protc.stShowTable.stShowFormat.exp == -1)
    {
        exp = 1;
    }
    else if(pShowDataL1->Protc.stShowTable.stShowFormat.exp == -2)
    {
        exp = 2;
    }
    else if(pShowDataL1->Protc.stShowTable.stShowFormat.exp == -3)
    {
        exp = 3;
    }
    else if(pShowDataL1->Protc.stShowTable.stShowFormat.exp == -4)
    {
        exp = 4;
    }        
    else
    {
        exp = 0;
    }

    if(exp > 0)
    {
        ShowPoint((INT8U)(exp-1));     
    }

    for(uni = 0; uni < 10; uni++)
    {
        ShowFigure(uni, pShowDataL1->Protc.showDataBuff[uni]);
    }    

    switch(pShowDataL1->Protc.stShowTable.stShowFormat.format)
    {
    case FMT_TIME:
        ShowTPoint(0);
        ShowTPoint(1);

        ShowPoint(1);
        ShowPoint(3);
        break;
    case FMT_DATE:
        len = (pShowDataL1->charAdd[1] & 0x0F) >> 1;

        for(uni = 2; uni <= 4; uni++)
        {
            if(len >= uni)
            {
                ShowPoint(((uni - 2) << 1) + 1);
            }
            else
            {
                break;
            }
        }

        break;
    case FMT_STRING:
    default:
        break;
    }

}

/*"*****************************************************************************"*/
/*"  Function:       DecodeShowChar"*/
/*"  Description:    将显示字符提示信息解码到显示缓存区"*/
/*""*/
/*"  Calls:          "*/
/*"  Called By:      DIS_PrintLcdLine1"*/
/*"  Input:          pChar  需要显示字符提示内容地址指针  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void DecodeChar(INT8U *pChar)
{
    INT8U i, j;
    INT8U pos_x, pos_y;

    for(i = 0; i < MAX_CHAR_BYTE; i++)
    {
        for(j = 0; j < 8; j++)
        {
            pos_x = mstTabChar[i * 8 + j].X_Pos;
            pos_y = mstTabChar[i * 8 + j].Y_Pos;
            if((0xff != pos_x) && (0xff != pos_y))
            {
                if((pChar[i] >> j) & 0x01)
                {
                   
                    mShowRam[pos_x] |=  pos_y;
                }
            }
        }   
    }
}
static void DecodeIndex(INT8U inIndex)
{
    INT8U tmpindex;

    tmpindex = LIB_CharToBcd(inIndex);
    DecodeBCDByte(DIS_INDEX_ADDR,tmpindex);
    
}
static void DecodeBCDByte(INT8U Addr, INT8U InBcd)
{

    ShowFigure(Addr,(InBcd&0xf0)>>4);
    ShowFigure((Addr + 1),(InBcd&0x0f));    
}
static void DecodeBillDate(INT8U *BillDate)
{
    DecodeBCDByte(DIS_YEAR_ADDR,BillDate[0]);  
    DecodeBCDByte(DIS_MONTH_ADDR,BillDate[1]); 
    ShowPoint(4);
}

/*"*****************************************************************************"*/
/*"  Function:       DecodeUnit"*/
/*"  Description:    根据相应的单位标识，将显示单位信息解码到显示缓存区"*/
/*""*/
/*"  Calls:          "*/
/*"  Called By:      Decode_ShowChar"*/
/*"  Input:          unit  单位标识              "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void DecodeUnit(INT8U unit)
{
    INT8U i;
    INT8U temp;
    INT8U pos_x, pos_y;
    
    temp = mTabDecodeUnit[unit];
    
    for(i = 0; i < MAX_LCD_UINT_NUM; i++)
    {
        pos_x = mstTabUnit[i].X_Pos;
        pos_y = mstTabUnit[i].Y_Pos;
        if((0xff != pos_x) && (0xff != pos_y))
        {
            if((temp >> i) & 0x01)
            {
                mShowRam[pos_x] |= pos_y;
            }
        }                
    }    
}

/*"*****************************************************************************"*/
/*"  Function:       Decode_Month_data"*/
/*"  Description:    根据相应的单位标识，将显示单位信息解码到显示缓存区"*/
/*""*/
/*"  Calls:          "*/
/*"  Called By:      Decode_ShowChar"*/
/*"  Input:          unit  单位标识                            "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
#if 0
static void DecodeMonth(INT8U inMonth)
{

    INT8U tmpData;

    tmpData = (INT8U)(inMonth & 0x0f);
    if(tmpData)
    {
        if(tmpData >= 10)
        {
            mShowRam[mstTabChar[CHAR_1].X_Pos] |=  mstTabChar[CHAR_1].Y_Pos;//Set the status char
            tmpData -= 10;
        }

        ShowFigure(DIS_NUM_ADDR4, tmpData);   
    }

}
#endif
#if 0
static void DecodeMonth(INT8U inMonth)
{

    INT8U tmpData;

    tmpData = (INT8U)(inMonth & 0x0f);
    if(tmpData)
    {
        if(tmpData >= 10)
        {
            mShowRam[mstTabChar[CHAR_1].X_Pos] |=  mstTabChar[CHAR_1].Y_Pos;//Set the status char
            tmpData -= 10;
        }

        ShowFigure(DIS_NUM_ADDR4, tmpData);   
    }

}
#endif
static void DecodeMinus(INT8U minus)
{
    if(8 == minus)
    {
        mShowRam[_STA_MINUS_ADDR] |= _STA_MINUS_BIT; /*"负号"*/
    }
    else if(minus > 0)
    {
        ShowFigure(minus, DIS_Minus);
    }
    else
    {
    }
}


void show_mb_data(void)
{
    LCD_Refresh(mShowRam);
}

/*"*****************************************************************************"*/
/*"  Function:       Show_Point_Line1"*/
/*"  Description:    显示主数据区指定8字后面的小数点"*/
/*"                                       "*/
/*"  Calls:          "*/
/*"  Called By:      Decode_ShowData"*/
/*"  Input:          num  指定8字号"*/
/*"                  Show_buffer 显示缓存区地址指针        "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void ShowPoint(INT8U num)
{
    INT8U pos_x, pos_y;
    
    if(num < MAX_POINT_NUM)
    {
        pos_x = mstTabPoint[num].X_Pos;
        pos_y = mstTabPoint[num].Y_Pos;
        
        if((0xff != pos_x) && (0xff != pos_y))
        {
            mShowRam[pos_x] |= pos_y;
        }
    }    
}

/*"*****************************************************************************"*/
/*"  Function:       Show_TPoint_Line1"*/
/*"  Description:    显示主数据区指定时间格式后面的上点，时间格式支持5字节YYMMDDhhmm"*/
/*"                                         "*/
/*"  Calls:          "*/
/*"  Called By:      Decode_ShowData"*/
/*"  Input:          num  指定时间数据编号"*/
/*"                  Show_buffer 显示缓存区地址指针     "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void ShowTPoint(INT8U num)
{
    INT8U pos_x, pos_y;
    
    if(num < MAX_TPOINT_NUM)
    {
        pos_x = mstTabTPoint[num].X_Pos;
        pos_y = mstTabTPoint[num].Y_Pos;
        
        if((0xff != pos_x) && (0xff != pos_y))
        {
            mShowRam[pos_x] |= pos_y;
        }
    }    
}

/*"*****************************************************************************"*/
/*"  Function:       Clear_Show_Ram"*/
/*"  Description:    清显示中间缓存区内容"*/
/*"  Calls:          "*/
/*"  Called By:      Send_ShowData_To_LCD中当主显示区显示字符内容时调用"*/
/*"  Input:                  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void ClearShowRam(void)
{
    LIB_MemSet(0, (INT8U *)mShowRam, _DIS_LCD_NUM_REG);
}

#if 0
/*"************************************************"*/
/*"Function:    GetHalfChar"*/
/*"Description: 取半个字符"*/
/*"Input: pCode ：*inIndex 需取的第几个半字符  *inpCh  数据指针"*/
/*"Output: 无"*/
/*"Return: 取得的字符"*/
/*"Others: "*/
/*"************************************************"*/
static INT8U GetHalfChar(INT8U inIndex, INT8U *inpCh)
{
    if (inIndex & 0x01)
    {
        return (INT8U)(inpCh[inIndex >> 1] >> 4);
    }
    else
    {
        return (INT8U)(inpCh[inIndex >> 1] & 0x0f);
    }
}
#endif
/*"************************************************"*/
/*"Function:    ShowFigure"*/
/*"Description: 显示单个字符"*/
/*"Input: pCode ：Addr 显示的位置  Num 显示字符的索引值"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
static void ShowFigure(INT8U Addr, INT8U Num)
{
    INT8U i;

    for(i = 0; i < 7; i++)
    {
        if(mDigital[Num] & (1 << i ))
        {
            mShowRam[mstTabNum[(Addr*7)+i].X_Pos] |=  mstTabNum[((Addr*7)+i)].Y_Pos;
        }
    }

}

/*"*****************************************************************************"*/
/*"  Function:       DisAll"*/
/*"  Description:    全显"*/
/*"  Calls:          "*/
/*"  Input:          "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/    
static void DisAll(void)
{
    //LIB_MemSet(0xff, mShowRam, _DIS_LCD_NUM_REG);  
    mShowRam[19] = 0xa5;
}

/*"*****************************************************************************"*/
/*"  Function:       Dis_ShowAll"*/
/*"  Description:    上电全屏显示接口"*/
/*"  Calls:          "*/
/*"  Input:          "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/    
void Dis_ShowAll(void)
{
    DisAll();
    LCD_Refresh(mShowRam);
}


/*"*****************************************************************************"*/
/*"  Function:       DIS_Main"*/
/*"  Description:    抽象层秒任务处理接口"*/
/*"  Calls:          "*/
/*"  Input:          "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/    
void DIS_Main(void)
{
//    if(TRUE == ST_ChkTsk_1s(DIS))
    {
        /*"当前屏显示时间到"*/
        if(mstDisCtrl.lastTime > 0)
        {
            mstDisCtrl.lastTime --;       
        }
        else
        {
            mstDisCtrl.currPrior = IDLE;
        }    

        /*"每秒更新闪烁标志"*/
        if(mstDisCtrl.flag.bits.flash)
        {
            mstDisCtrl.flag.bits.flash = 0;
        }
        else
        {
            mstDisCtrl.flag.bits.flash = 1;
        }
    }
}

#ifdef DIS_USER_CODE
/*"*****************************************************************************"*/
/*"  Function:       DIS_PrintLcdLine2"*/
/*"  Description:    用户代码区数据显示"*/
/*"  Calls:          "*/
/*"  Input:          pUseCode: 用户代码数据结构指针"*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/    
INT8U DIS_PrintLcdLine2 (ST_USERCODE_SHOW *pUseCode)
{
    LIB_MemCpy((INT8U *)pUseCode, (INT8U *)&mstUserCode, sizeof(ST_USERCODE_SHOW));
    mstDisCtrl.flag.bits.code = 1;

    return TRUE;
}

/*"*****************************************************************************"*/
/*"  Function:       DecodeUserCode"*/
/*"  Description:    按用户代码格式要求显示数据"*/
/*"  Calls:          "*/
/*"  Input:          "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/    
#define  USECODE_ADDR   (8)       /*"用户代码数字区起始位置"*/
static void DecodeUserCode(void)
{
    INT8U i, offset, len;
    INT8U pointMask;

    offset = mstUserCode.offset;
    len = mstUserCode.len;

    if((offset < MAX_USECODE_NUM) && ((INT8U)(offset + len) <= MAX_USECODE_NUM))
    {
        for(i = offset; i < (INT8U)(offset + len); i++)
        {
            ShowFigure((INT8U)(USECODE_ADDR + i), mstUserCode.obis[i - offset]);
        }
    }

    pointMask = mstUserCode.pointFlg.byte;
    for(i = 0; i < MAX_USECODE_NUM; i++)
    {
        if(((pointMask >> i) & 0x01) != 0)
        {
            ShowUserPoint(i);
        }
    }
}

/*"*****************************************************************************"*/
/*"  Function:       ShowUserPoint"*/
/*"  Description:    显示用户代码数据区指定8字后面的小数点"*/
/*"                                       "*/
/*"  Calls:          "*/
/*"  Called By:      Decode_ShowData"*/
/*"  Input:          num  指定8字号"*/
/*"                  Show_buffer 显示缓存区地址指针        "*/
/*"  "*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void ShowUserPoint(INT8U num)
{
    INT8U pos_x, pos_y;

    if(num < MAX_USECODE_NUM)
    {
        pos_x = mstUserTabPoint[num].X_Pos;
        pos_y = mstUserTabPoint[num].Y_Pos;

        if((0xff != pos_x) && (0xff != pos_y))
        {
            mShowRam[pos_x] |= pos_y;
        }
    }
}
#endif

#if 0
void DIS_Char_K(void)
{
    INT8U temp;

    temp = 0x3C;
    DecodeChar(&temp);
}
#endif

#ifdef DIS_QUAD_INFO
/*"*****************************************************************************"*/
/*"  Function:       DecodeQuad"*/
/*"  Description:    提示信息中的象限显示"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          quad  象限信息"*/
/*"  Return:         "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static void DecodeQuad(INT8U quad)
{
    DIS_ClrStauSeg(STA_XY);
    DIS_ClrFlashStauSeg(STA_I);
    DIS_ClrFlashStauSeg(STA_II);
    DIS_ClrFlashStauSeg(STA_III);
    DIS_ClrFlashStauSeg(STA_IV);

    DIS_ClrStauSeg(STA_I);
    DIS_ClrStauSeg(STA_II);
    DIS_ClrStauSeg(STA_III);
    DIS_ClrStauSeg(STA_IV);

    if(0 == (quad & 0x0F))
    {
        return;
    }

    DIS_SetStauSeg(STA_XY);
    if(quad & 0x10)
    {
        if(quad & 0x01)
        {
            DIS_FlashStauSeg(STA_I);
        }
        if(quad & 0x02)
        {
            DIS_FlashStauSeg(STA_II);
        }
        if(quad & 0x04)
        {
            DIS_FlashStauSeg(STA_III);
        }
        if(quad & 0x08)
        {
            DIS_FlashStauSeg(STA_IV);
        }
    }
    else
    {
        if(quad & 0x01)
        {
            DIS_SetStauSeg(STA_I);
        }
        if(quad & 0x02)
        {
            DIS_SetStauSeg(STA_II);
        }
        if(quad & 0x04)
        {
            DIS_SetStauSeg(STA_III);
        }
        if(quad & 0x08)
        {
            DIS_SetStauSeg(STA_IV);
        }
    }
}
#endif
void DIS_ClrPriority(void)
{
	if(LEVEL_4 == mstDisCtrl.currPrior)
	{
		mstDisCtrl.currPrior = IDLE;
	}
}
