#ifndef _SYS_DEFINE_H_
#define _SYS_DEFINE_H_


#define NOPREPAYMETER          	0               /*"��Ԥ���ѱ���ͨ��"*/
#define TELMETER               	1               /*"Զ�̱�"*/
#define LOCALMETER             	2               /*"���ر�"*/
#define SMARTMETER             	4               /*"���ܱ�"*/

#define METERTPYE               0x0F  			/*"����ģʽ��,����λ"*/

#define ENMWRELAY			   	0x80  /*"������������բ��ʽ"*/
#define HOLDTYPE1E              0x40  /*"1E��բ��ʽ"*/
#define HOLDTYPE1B3A            0x20  /*"1B��3A��բ��ʽ"*/

#define _TARIFF_PRC_NUM 		32    /*"���ʵ����"*/
#define _STEP_NUM 				6     /*"�ݶ���"*/
#define _STEP_PRC_NUM 			7     /*"�ݶȵ����"*/

#define RLYTYPE                 0x03  /*"�̵�������,����λ"*/
#define RLY_INSIDE              0                  /*"���ü̵���"*/
#define RLY_OUTSIDE             1                  /*"���õ�ƽ�̵���"*/
#define RLY_PULSEOUTSIDE        2                  /*"��������̵���"*/

#define MON_BILL_DATE_NUM    	3  /*"�½����ո���"*/
#define YEAR_BILL_DATE_NUM   	4  /*"������ո���"*/

/*"Э������"*/
#define DLT698     	/*"DL698Э��"*/
//#define IEC62056 	/*"62056Э��"*/
//#define DLT645     	/*"DL645Э��"*/
#define DM_DLT645
//#define DM_DLT698

#define REPORT645

#define RELIABILITY 	/*"�����ɿ��Լ��"*/


/*"������ʾģʽ"*/
#define PD_DISP_SLEEP   /*"���������δ����ʱΪ���糣��"*/


//#define PM_MODLE
#define MNT_MODLE
#define UI_MODLE
#define RCRD_MODLE
#define AUTO_MODLE


#define APP_DEMO   				0    /*"����demoʱ��1��������0"*/

#define SEASON_SLOT_NUM     	(14) /*"��ʱ��/������"*/
#define DAY_TBL_NUM         	(8)  /*"��ʱ�α���"*/
#define DAY_SLOT_NUM        	(14) /*"��ʱ����"*/
#define SPEC_DAY_NUM        	(30) /*"�����ڼ�����/ ��������"*/
#define DEFAULT_SEASON_TBL_NUM  (2)/*"Ĭ��2��ʱ����"*/
#define DEFAULT_DAY_TBL_NUM   	(4)/*"Ĭ��4��ʱ�α�"*/


/*"--------------------------------------------------------------"*/
/*"  Meter Configure Type defined            "*/
/*"--------------------------------------------------------------"*/ 

#define SINGLE_PHASE     /*"���Ͷ��壬�����2ѡ1"*/
//#define THREE_PHASE      /*"���Ͷ��壬�����2ѡ1"*/


#define CYC_DISP_MAX  	(99) /*"ѭ����������"*/
#ifdef THREE_PHASE
#define KEY_DISP_MAX  	(99)//(120) /*"������������"*/
#else
#define KEY_DISP_MAX  	(99) /*"������������"*/
#endif

/*"��ȫģʽ��������"*/
#ifdef THREE_PHASE
#define MAX_OI_NUM_SAFE_MODE   32   
#else
#define MAX_OI_NUM_SAFE_MODE   16   
#endif

/*"���������ѡ��"*/
#if defined (SINGLE_PHASE) 
    #if defined (THREE_PHASE)
        "err only one of "SINGLE_PHASE" or "THREE_PHASE" shoulud be selected"
    #endif
#endif

#ifndef SINGLE_PHASE
    #ifndef THREE_PHASE
        "err one of "SINGLE_PHASE" or "THREE_PHASE" be selected"
    #endif
#endif


/*"--------------------------------------------------------------"*/
/*"  typedef for data type            "*/
/*"--------------------------------------------------------------"*/ 
/***************************typedef for data type******************************/
typedef  unsigned char          BOOL;       /*8-bit  oolean or logical  */
typedef  unsigned char          INT8U;      /*8-bit  unsigned integer   */
typedef    signed char          INT8S;      /*8-bit    signed integer   */                            
typedef  unsigned short         INT16U;     /*16-bit unsigned integer   */                       
typedef    signed short         INT16S;     /*16-bit   signed integer   */
typedef  unsigned long          INT32U;     /*32-bit unsigned integer   */                           
typedef    signed long          INT32S;     /*32-bit   signed integer   */                           
typedef  unsigned long long     INT64U;     /*64-bit unsigned integer   */                          
typedef    signed long long     INT64S;     /*64-bit   signed integer   */                           
typedef  float                  FP32;       /*32-bit floating point     */                          
typedef  double                 FP64;       /*64-bit floating point     */             

#define DATA_ADDR INT32U

/*"--------------------------------------------------------------"*/
/*"  Common body definition of BYTE(8bit) type                   "*/
/*"--------------------------------------------------------------"*/ 
typedef union
{
    INT8U byte;
    struct
	{
        INT8U b0 :1;
        INT8U b1 :1;
        INT8U b2 :1;
        INT8U b3 :1;
        INT8U b4 :1;
        INT8U b5 :1;
        INT8U b6 :1;
        INT8U b7 :1;
    } bit;
} UN_BYTE_DEF; 

/*"--------------------------------------------------------------"*/
/*"  Common body definition of WORD(16bit) type                  "*/
/*"----------------------------------------------------------- --"*/
typedef union WORD_DEF
{
    INT16U word;
    INT8U byte[2];
    struct
    {
        INT8U b0 :1;
        INT8U b1 :1;
        INT8U b2 :1;
        INT8U b3 :1;
        INT8U b4 :1;
        INT8U b5 :1;
        INT8U b6 :1;
        INT8U b7 :1;
        INT8U b8 :1;
        INT8U b9 :1;
        INT8U b10 :1;
        INT8U b11 :1;
        INT8U b12 :1;
        INT8U b13 :1;
        INT8U b14 :1;
        INT8U b15 :1;
    } bit;
}UN_WORD_DEF; 

/*"---------------------------------------------------------------"*/
/*"  Common body definition of DWORD(32bit) type                  "*/
/*"----------------------------------------------------------- ---"*/
typedef union
{
    INT32U dword;
    INT16U word[2];
    INT8U byte[4];
    struct
    {
        INT8U b0 :1;
        INT8U b1 :1;
        INT8U b2 :1;
        INT8U b3 :1;
        INT8U b4 :1;
        INT8U b5 :1;
        INT8U b6 :1;
        INT8U b7 :1;
        INT8U b8 :1;
        INT8U b9 :1;
        INT8U b10 :1;
        INT8U b11 :1;
        INT8U b12 :1;
        INT8U b13 :1;
        INT8U b14 :1;
        INT8U b15 :1;
        INT8U b16 :1;
        INT8U b17 :1;
        INT8U b18 :1;
        INT8U b19 :1;
        INT8U b20 :1;
        INT8U b21 :1;
        INT8U b22 :1;
        INT8U b23 :1;
        INT8U b24 :1;
        INT8U b25 :1;
        INT8U b26 :1;
        INT8U b27 :1;
        INT8U b28 :1;
        INT8U b29 :1;
        INT8U b30 :1;
        INT8U b31 :1;
    } bit;
}UN_DWORD_DEF;

typedef struct        /*"7�ֽ�ʱ���ʽ"*/
{
    INT8U  year;
    INT8U  month;
    INT8U  day;
    INT8U  hour;
    INT8U  min;
    INT8U  sec;
    INT8U  week;
}ST_7TIME_FORM;


typedef struct        /*"6�ֽ�ʱ���ʽ"*/
{
    INT8U  year;
    INT8U  month;
    INT8U  day;
    INT8U  hour;
    INT8U  min;
    INT8U  sec;
}ST_6TIME_FORM;


typedef struct        /*"5�ֽ�ʱ���ʽ"*/
{
    INT8U  year;
    INT8U  month;
    INT8U  day;
    INT8U  hour;
    INT8U  min;
}ST_5TIME_FORM;


typedef struct        /*"2�ֽ�ʱ���ʽ"*/
{
    INT8U  hour;
    INT8U  min;
}ST_2TIME_FORM;

typedef struct        /*"3�ֽ�ʱ���ʽ"*/
{
    INT8U  month;
    INT8U  day;
    INT8U  hour;
}ST_3TIME_FORM;


typedef struct        /*"4�ֽ�ʱ���ʽ"*/
{
    INT8U  month;
    INT8U  day;
    INT8U  hour;
    INT8U  min;
}ST_4TIME_FORM;


typedef struct        /*"2�ֽ�ʱ���ʽ"*/
{
    INT8U  day;
    INT8U  hour;
}ST_DAY_HOUR;


typedef struct 
{
  INT8U format;   /*"���ݸ�ʽ"*/
  INT8U unit;     /*"���ݵ�λ"*/
  INT8S exp;      /*"����С��λ��"*/
}ST_FMT_ATTR;

typedef union /*"645 id"*/
{
    INT32U asLong;
    INT16U asWord[2];
    INT8U asBytes[4];
} UN_ID645;

/*OI�����嶨��*/
typedef union
{
    INT16U asInt;
    INT8U  asByte[2];
}UN_OI;

/*OAD����*/
typedef struct
{
    UN_OI Oi;/*"OAD�����ʶ"*/
    INT8U AId;/*"���Ա�ʶ"*/
    INT8U IId;/*"����Ԫ������"*/
}ST_OAD;


/*"----��������С��λ���������ֽ�������----"*/
#define  ENGY_DECIMAL_POINT   (-4)
#if defined (SINGLE_PHASE)
    #if(ENGY_DECIMAL_POINT >= -3)
    #define ENERGY_SIZE 4
    #elif(ENGY_DECIMAL_POINT >= -5)
    #define ENERGY_SIZE 5
    #else
    "err  define of "ENGY_DECIMAL_POINT"out of range"
    #endif
#elif defined (THREE_PHASE)
    #if(ENGY_DECIMAL_POINT >= -2)
    #define ENERGY_SIZE 4
    #elif(ENGY_DECIMAL_POINT >= -4)
    #define ENERGY_SIZE 5
    #elif(ENGY_DECIMAL_POINT >= -6)
    #define ENERGY_SIZE 6
    #else
    "err  define of "ENGY_DECIMAL_POINT"out of range"
    #endif
#endif

#if(ENERGY_SIZE > 4)
#define ENGY_TYPE_U   INT64U
#define ENGY_TYPE_S   INT64S
#else
#define ENGY_TYPE_U   INT32U
#define ENGY_TYPE_S   INT32S
#endif
/*"===================================================================
    MACRO : Bool flag
==================================================================="*/
#define  FALSE		    (0)
#define	 TRUE			(1)

#ifndef NULL     
#define NULL   			(0)
#endif

#define SYS_TICK_MS     (10) /*ʱ�ӵδ���*/

#define  FAR_POINTER   //__data20
#define  TABLE_MEM     //__data20
//===================================================================
//  MACRO Get shift of struct element
//===================================================================
#define FIND(str,element)  (INT32U)(&(((str*)(0))->element))

/*"��ȡ�����г�Ա�ĸ���"*/
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

/**
 * BUILD_BUG_ON - break compile if a condition is true.
 * @condition: the condition which the compiler should know is false.
 *
 * If you have some code which relies on certain constants being equal, or
 * other compile-time-evaluated condition, you should use BUILD_BUG_ON to
 * detect if someone changes it.
 *
 * The implementation uses gcc's reluctance to create a negative array, but
 * gcc (as of 4.4) only emits that error for obvious cases (eg. not arguments
 * to inline functions).  So as a fallback we use the optimizer; if it can't
 * prove the condition is false, it will cause a link error on the undefined
 * "__build_bug_on_failed".  This error message can be harder to track down
 * though, hence the two different methods.
 */
#define BUILD_BUG_ON(condition) ((void)sizeof(char[1 - 2*!!(condition)]))

/* �õ��ṹ���Ա�����Ĵ�С   */
#define SIZEOF_ELEMENT(str,element)     (sizeof(((str*)0)->element))   


#define MCU_RUNMODE      (0)  /*�ϵ�����ģʽ*/
#define MCU_LPMODE_LIGHT (1)  /*��ȵ�������ģʽ*/
#define MCU_LPMODE_MID   (2)  /*�жȵ�������ģʽ*/
#define MCU_LPMODE_DEEP  (3)  /*��ȵ�������ģʽ*/
#define FIRST_PWR_ON     (4)  /*��һ���ϵ�ģʽ*/

/*��С�˶���*/
//#define BIG_ENDIAN
#ifndef BIG_ENDIAN
#define LITTLE_ENDIAN
#endif

/*"---���ڸ�λ����Ҫ���ֵ�RAM�ĺ궨��----"*/
#define NO_INIT __no_init


/*"����汾��"*/
#define SOF0  	('D')
#define SOF1  	('D')
#define SOF2  	('Z')
#define SOF3  	('Y')
#define SOF4  	('1')
#define SOF5  	('0')
#define SOF6  	('2')
#define SOF7  	('C')
#define SOF8  	('-')
#define SOF9  	('Z')
#define SOF10  	('/')
#define SOF11  	('K')
#define SOF12  	('6')
#define SOF13  	('6')
#define SOF14  	('(')
#define SOF15  	('6')
#define SOF16  	('9')
#define SOF17  	('8')
#define SOF18  	(')')
#define SOF19  	('-')
#define SOF20  	('6')
#define SOF21  	('-')
#define SOF22  	('0')
#define SOF23  	(0x0)
#define SOF24  	(0x0)
#define SOF25  	(0x0)
#define SOF26  	(0x0)
#define SOF27  	(0x0)
#define SOF28  	(0x0)
#define SOF29  	(0x0)
#define SOF30  	(0x0)
#define SOF31  	(0x0)

/*�汾����*/
#define Y4  ('0')
#define Y3  ('0')
#define Y2  ('1')
#define Y1  ('9')
#define M2  ('0')
#define M1  ('8')
#define D2  ('2')
#define D1  ('0')


#define  NEW_EEP_FLAG       0xE3610409    /*"���ڿ���ʶ"*/


#define  HEX_FRMT     1   /*"�궨��HEXʱ���ʽ"*/
#define  BCD_FRMT     2   /*"�궨��BCDʱ���ʽ"*/
#define  APP_TIME_FRMT      HEX_FRMT

#endif
