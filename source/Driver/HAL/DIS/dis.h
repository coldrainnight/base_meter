/*
 * showlcd_abstract.h
 *
 *  Created on: 2012-1-19
 *      Author: Administrator
 */

#ifndef _DIS_H
#define _DIS_H

#include "driver\MCU\LCD.h"


//#define DIS_RELAY_ICON_COMB /*"������: �̵�������բ����ռ��2λstatus"*/
//#define DIS_USER_CODE /*"������: �û�������ʾ"*/
//#define DIS_QUAD_INFO /*"������: ������ʾ(�͹�����ʾʹ��)"*/
#define DIS_REC_TIME/*"������:��ʾ������¼����.�¶������ϼ���"*/
#define DIS_SCR_NO  /*"��ʾ�����"*/


#define MAX_CHAR_BYTE       ((MAX_BYTE_CHAR >> 3) + 1)/*"�����ַ��ֽ������ֵ������ඨ��һ���ֽ��Ǹ��·���ʾ��"*/
#define MAX_POINT_NUM       (5)        /*"����������С����8�ֵ������"*/
#define MAX_TPOINT_NUM     (2)        /*"����������ʱ���ʽð�������"*/
#define MAX_SHOWDATA        (10)
#define MAX_SCHAR_NUM      (5)
#define MAX_UINT_NUM      (15)   /*"����ʾ�ĵ�λ����"*/
#define MAX_LCD_UINT_NUM      (7)   /*"Һ���ϵ�λ�Ķ���ĸ���"*/
#ifdef DIS_USER_CODE
#define MAX_USECODE_NUM  (8)       /*"�û������������"*/
#define MAX_LEAFNO_NUM    (2) 
#endif
#ifdef DIS_QUAD_INFO
#define MAX_SUB_CHAR_NUM  (6)
#else
#define MAX_SUB_CHAR_NUM  (5)
#endif

/*"********�ַ���λ�ýṹ����**************"*/
typedef struct _CHAR_SHOW_POS
{
    INT8U X_Pos;/*"X_Pos��ʾ�ַ������ĸ����ֽ�"*/
    INT8U Y_Pos;/*"Y_Pos��ʾ�ַ����ڶ��ֽ���λ"*/
}ST_CHAR_POS;

typedef struct _BIT
{
	INT8U b0 :1;
	INT8U b1 :1;
	INT8U b2 :1;
	INT8U b3 :1;
	INT8U b4 :1;
	INT8U b5 :1;
	INT8U b6 :1;
	INT8U b7 :1;
} BIT;

/*"��ʾȡ�������Ϣ"*/
typedef struct
{
    ST_FMT_ATTR stShowFormat;/*"���ݸ�ʽ"*/
    INT8U schar[MAX_SCHAR_NUM];/*"��ʾ�ַ���Ϣ"*/
}ST_SHOW_TABLE; 

typedef struct
{
    INT8U showDataBuff[MAX_SHOWDATA]; /*"��ʾ���ݽ��ջ���"*/
    ST_SHOW_TABLE  stShowTable; /*"��ʾ��Ϣ�������Ϣ"*/
}ST_SHOWDATA_OUTPUT;

typedef struct
{
    ST_SHOWDATA_OUTPUT Protc;/*"����ʾ��������Ϣ"*/
    INT8U charAdd[MAX_SUB_CHAR_NUM];/*"0_b0-b3:�·�;b4-b7:����"*/
                                                              /*"1_b0-b3:ʱ���ʽ�����ݳ���;b4-b7:������Ϣ"*/
                                                             /*"2_����� HEX��"*/
                                                             /*"3_ʱ��:�� BCD��"*/
                                                            /*"4_ʱ��:�� BCD��"*/  
                                                            /*"5_b0-b3: I~IV����;b4: 0_����/ 1_��˸��b5-b7:����"*/                                                              
}ST_SHOW_DATA;

#ifdef DIS_USER_CODE
typedef struct
{
    INT8U obis[MAX_USECODE_NUM]; /*"�û�������ʾ����ʾ����"*/
    INT8U offset; /*"�û������offsetƫ�Ƶ���������ʼ��ʾ����һ���ӵ�һ�����ֿ�ʼ��"*/
    INT8U len; /*"�û�������ʾ���ȣ�8�ָ����� ��ȡֵ��Χ1~8"*/
    UN_BYTE_DEF pointFlg; /*"BIT0~BIT7�ֱ��ʾ��1~8��8�ֺ����'.'�Ƿ������0������  1����"*/
    INT8U leafNo[MAX_LEAFNO_NUM]; /*"Ҷ�Ӻ�"*/
}ST_USERCODE_SHOW;
#endif

typedef union /*"��ʾ���Ʊ�־"*/
{
    struct
    {
        INT8U   flash: 1; /*"��˸"*/
        INT8U   code:1;    /*"�û�������ʾ"*/
    }bits;
    INT8U   byte;
}UN_DIS_FLAG;

/*"��ʾ���ȼ����壬�ɵ͵��߷�4�����ȼ���1����ͣ�4�����"*/
typedef enum  
{
    IDLE = 0,
    LEVEL_1,
    LEVEL_2,
    LEVEL_3,
    LEVEL_4
}EN_SHOW_PRIORITY;

typedef struct 
{
    INT16U lastTime; /*"��ʾҪ�����ʱ��"*/
    EN_SHOW_PRIORITY currPrior;/*"��ǰ��ʾ���ȼ�"*/
    UN_DIS_FLAG flag;/*"��ʾ���Ʊ�־"*/
}ST_SHOW_CTRL;



/*"********************״̬��ʾ������*******************************"*/
/*" *********����û�г���255����ʶ�����壬���ֻ����1BYTE ****"*/
/*"**void Set_StatusBit_Show(INT8U SegBit_name);********"*/
/*"*****void Clear_StatusBit_Show(INT8U SegBit_name);********"*/
/*"*****void Set_StatusBit_Flash(INT8U SegBit_name);********"*/
/*"*****void Clear_StatusBit_Flash(INT8U SegBit_name);********"*/
/*"*****����Ĳ�������ֵΪUINT8�ͣ�ע�ⲻҪ����Χ********"*/
typedef enum  
{
    STA_STEP_1 = 0, /*"����1"*/
    STA_STEP_2 , /*"����2"*/
    STA_STEP_3 , /*"����3"*/
    STA_STEP_4 , /*"����4"*/
    STA_BAT ,/*"�͹��ĵ��"*/  
    STA_KEY,/*"��Կ"*/
    STA_PHONE,/*"ͨ�ŵ绰��"*/  
    STA_WAVE,/*"ģ��ͨ�ŷ�"*/ 
    STA_WAVE_UP,/*"ģ������"*/
    STA_WAVE_DOWN,/*"ģ������"*/  
    STA_BLUETOOTH,/*"����"*/   
    STA_LOCK,/*"����"*/   
    STA_UPGRADE,/*"����"*/     
    STA_RECHARGE,/*"�빺��"*/  
    STA_RELAY_OPEN,/*"��բ"*/  
    STA_OVERDRAFT,/*"͸֧"*/    
    
    STA_TARF_JIAN,/*"�� ����"*/  
    STA_TARF_FENG,/*"�� ����"*/
    STA_TARF_PING,/*"ƽ ����"*/    
    STA_TARF_GU,/*"�� ����"*/   
    STA_TARF_1_SET,/*"��һ��ʱ��"*/     
    STA_TARF_2_SET,/*"�ڶ���ʱ��"*/
    STA_STEP_1_SET,/*"��һ�׽���"*/     
    STA_STEP_2_SET,/*"�ڶ��׽���"*/      
    STA_REVERSE,/*"���ʷ��� "*/    

    MAX_BYTE_STA
  }Status_SegBit;

#define _DIS_STA_NUM  ((MAX_BYTE_STA>>3) + 1)

typedef enum  
{

    CHAR_COMB = 0,/*"���"*/
    CHAR_LAST_MON,/*"��X��"*/
    CHAR_N,/*"N"*/          
    CHAR_NUM,/*"��"*/ 
    CHAR_CURR,/*"��ǰ"*/    
    CHAR_LAST,/*"��"*/   
    CHAR_L,/*"L"*/          
    CHAR_B,/*"B"*/ 

    CHAR_DATE,/*"����"*/ 
    CHAR_XIANG,/*"��"*/
    CHAR_REMAIN,/*"ʣ��"*/    
    CHAR_STEP,/*"����"*/ 
    CHAR_FAN,/*"��"*/    
    CHAR_PO,/*"��"*/    
    CHAR_COS,/*"COS��"*/    
    CHAR_SHE,/*"��"*/         

    CHAR_FIRST,  /*"��"*/                            
    CHAR_SEC,  /*"��"*/   
    CHAR_LIANG,/*"��"*/    
    CHAR_DIAN,/*"��"*/ 
    CHAR_YOU,  /*"��"*/ 
    CHAR_GONG,  /*"��"*/     
    CHAR_XU,  /*"��"*/   
    CHAR_TARIFF,  /*"����"*/    

    CHAR_CONST,/*"����"*/ 
    CHAR_HU,/*"��"*/
    CHAR_YA,  /*"ѹ"*/         
    CHAR_SHI,  /*"ʧ"*/   
    CHAR_V,  /*"V"*/       
    CHAR_SUM,/*"��"*/   
    CHAR_MONEY,/*"���"*/
    CHAR_JIA,/*"��"*/         

    CHAR_METER,/*"��"*/  
    CHAR_SEG,/*"��"*/
    CHAR_JIAN,/*"��"*/
    CHAR_SHI2,/*"ʱ"*/        
    CHAR_YONG,/*"��"*/       
    CHAR_OVERDRAW,  /*"͸֧"*/        
    CHAR_ZHI,/*"֧"*/      //CHAR_LIU,  /*"��"*/   
    CHAR_POWER,  /*"����"*/       
    
    CHAR_MONTH1,/*"��N��Ԥ��"*/
    CHAR_MONTH2,/*"��N��Ԥ��"*/
    CHAR_MONTH3,/*"��N��Ԥ��"*/
    CHAR_MONTH4,/*"��N��Ԥ��"*/
    CHAR_TARF_GU,/*"��"*/   
    CHAR_TARF_PING,/*"ƽ"*/    
    CHAR_TARF_FENG,/*"��"*/     
    CHAR_TARF_JIAN,/*"��"*/
    CHAR_1,/*"1"*/      

    MAX_BYTE_CHAR
}Char_SegBit;

typedef enum  
{
    DIS_0 = 0,
    DIS_1,
    DIS_2,
    DIS_3,
    DIS_4,
    DIS_5,
    DIS_6,
    DIS_7,
    DIS_8,
    DIS_9,
    DIS_A,
    DIS_b,
    DIS_C,
    DIS_d,
    DIS_E,
    DIS_F,
    DIS_G,
    DIS_h,
    DIS_i,
    DIS_J,
    DIS_k,
    DIS_L,
    DIS_M,
    DIS_n,
    DIS_o,
    DIS_P,
    DIS_q,
    DIS_r,
    DIS_S,
    DIS_t,
    DIS_u,
    DIS_v,
    DIS_W,//
    DIS_X,//
    DIS_y,
    DIS_z,//
    DIS_Space,
    DIS_Minus,

    DIS_MAX,
}EN_DISP_CHAR;


extern void DIS_SetStauSeg(INT8U SegId);
extern void DIS_SetStauTarfStep(INT8U SegId,INT8U Num);
extern void DIS_ClrStauTarfStep(INT8U SegId);
extern void DIS_ClrStauSeg(INT8U SegId);
extern void DIS_FlashStauSeg(INT8U SegId);
extern void DIS_ClrFlashStauSeg(INT8U SegId);
extern INT8U DIS_PrintLcdLine1(ST_SHOW_DATA *pShowDataL1, INT16U LastTime, EN_SHOW_PRIORITY ShowPrio);
extern INT8U DIS_PrintLcdLine3(INT8U *pShowData, INT16U LastTime, EN_SHOW_PRIORITY ShowPrio);
extern void DIS_init(void);
extern void DIS_Main(void);
#ifdef DIS_USER_CODE
extern INT8U DIS_PrintLcdLine2 (ST_USERCODE_SHOW *pUseCode);
#endif
#if 0
extern void DIS_Char_K(void);
extern void DIS_ClrAllStauSeg(void);
#endif
extern void Dis_ShowAll(void);
extern INT8U set_show_data(INT8U *buf);

#define COM1 0X01
#define COM2 0X02
#define COM3 0X04
#define COM4 0X08
#define COM5 0x10
#define COM6 0x20
#define COM7 0x40
#define COM8 0x80

/* �����������ĸ��bit λ*/
#define SEG7_A 0x01
#define SEG7_B 0x02
#define SEG7_C 0x04
#define SEG7_D 0x08
#define SEG7_E 0x10
#define SEG7_F 0x20
#define SEG7_G 0x40




#define _SHOW_ALL_  0x5a
#define _SHOW_NONE_ 0x5b


/*"��ʾ���ݸ�ʽ"*/
typedef enum
{
    FMT_STRING = 0,  /*"�ַ���"*/
    FMT_DATE,  /*"����"*/
    FMT_TIME,  /*"ʱ��"*/

    FMT_NONE = 0xff
}EN_Data_Format;
extern void Dis_test(void);
extern void DIS_ClrPriority(void);
#endif
