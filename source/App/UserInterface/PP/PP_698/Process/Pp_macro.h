/*"***********************************************************		
FileName: pp_macro.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: Э�鴦��궨��ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/
#ifndef _PP_MACRO_H
#define _PP_MACRO_H
/********************************************************/
#include "App\UserInterface\ProtocolStack\PS_698\DL_698.h"


#define CHANNEL 3      /*ͨ����*/

#define APP_NEXT_FRAME_NO_ACCESS    1  /*Ӧ�ò����ݳ���2K����ܾ���д*/
//#define ENABLE_SAFE_MODE   1
#define DEBUG_PARAMENT_SINGLE_OBIS  0  /*�������OBIS����������������ʱ��һ�����ֱ���*/

#define DEBUG_PP_698_CODE    0  /*698Э����Դ���*/

#define ADD_TIME_SET_0  1      /*�ۼ�ʱ���Ƿ���0�����ǻ�NULL*/

/********************************************************/

#define OIL  0
#define OIH  1

typedef struct
{
	UN_OI Oi;
	INT8U MTD;
	INT8U MOD;
}_ST_OMD;

/*���ʽ��DARö�ٶ���*/
typedef enum
{
    DAR_0_OK             = 0,   /*"�ɹ�"*/
    DAR_1_HW_FAIL        = 1,   /*"Ӳ��ʧЧ"*/
    DAR_2_TEM_FAIL       = 2,   /*"��ʱʧЧ"*/
    DAR_3_NOACESS        = 3,   /*"�ܾ���д"*/
    DAR_4_OANODEF        = 4,  /*"����δ����"*/
    DAR_5_OANOADAPT      = 5,  /*"����ӿ��಻����"*/
    DAR_6_OANOEXIST      = 6,  /*"���󲻴���"*/
    DAR_7_TYPENOADAPT    = 7,  /*"���Ͳ�ƥ��"*/
    DAR_8_OVERFLOW       = 8,  /*"Խ��"*/
    DAR_9_DBNOVAILD      = 9,  /*"���ݿ鲻����"*/
    DAR_10_FRAMECANCEL   = 10, /*"��֡������ȡ��"*/
    DAR_11_NOFRAMWSTA    = 11, /*"�����ڷ�֡����״̬"*/
    DAR_12_BWCANCEL      = 12, /*"��дȡ��"*/
    DAR_13_NOBWEXIST     = 13, /*"�����ڿ�д״̬"*/
    DAR_14_DBIDNOVAILD   = 14, /*"���ݿ������Ч"*/
    DAR_15_PSWERR        = 15, /*"�����/δ��Ȩ"*/
    DAR_16_BAUDERR       = 16, /*"ͨ�����ʲ��ܸ���"*/
    DAR_17_YEARZONE      = 17, /*"��ʱ������"*/
    DAR_18_DAILYSLOT     = 18, /*"��ʱ������"*/
    DAR_19_TRIFF         = 19, /*"��������"*/
    DAR_20_NO_RIGHT      = 20, /*"��ȫ��֤��ƥ�� "*/
    DAR_21_REPRECHARGE   = 21, /*"�ظ���ֵ"*/
    DAR_22_ESAMCHKFAIL   = 22, /*"ESAM��֤ʧ��"*/
    DAR_23_AUTHFAIL      = 23, /*"��ȫ��֤ʧ��"*/
    DAR_24_USERNO        = 24, /*"�ͻ���Ų�ƥ��"*/
    DAR_25_BUYTIMES      = 25, /*"��ֵ��������"*/
    DAR_26_HOARD         = 26, /*"���糬�ڻ�"*/
    DAR_27_ADDRERR       = 27, /*"��ַ�쳣"*/
    DAR_28_DECRYPTERR    = 28, /*"�Գƽ��ܴ���"*/
    DAR_29_DECRYPTERR    = 29, /*"�ǶԳƽ��ܴ���"*/
    DAR_30_SIGNERR       = 30, /*"ǩ������"*/
    DAR_31_HANGUP        = 31, /*"���ܱ����"*/
    DAR_32_TIMETAGERR    = 32, /*"ʱ���ǩ��Ч    "*/
    DAR_33_TIMEOUT       = 33, /*"����ʱ   "*/
    DAR_34_P1P2ERR       = 34, /*"ESAM��P1P2����ȷ*/
    DAR_35_LCERR         = 35, /*"ESAM��LC����*/
    DAR_FE_ABORNMAL      = 254,/*�쳣���� �Զ����*/
    DAR_FF_OTHERERR      = 255,/*"��������"*/
}ENUM_DAR;

/*698��λö�ٶ���*/
typedef enum
{
    UNIT_YEAR     = 1,     /*��*/
    UNIT_MONTH    = 2,     /*��*/    
    MIN           = 6,     /*��*/
    PHASEANGLE    = 8,     /*��Ƕ���*/
    TOC           = 9,
    MONEY         = 10,
    UNIT_W        = 27,    /*��*/
    KW            = 28,
    VA            = 29,
    KVA           = 30,
    var           = 31,
    kvar          = 32,
    KWH           = 33,
    kvarh         = 35,
    AMP           = 36,
    VOLTAGE       = 38,
    HZ            = 47,    
    UNIT_H        = 51,    /*% �ٷֱ� */
    UPRICE        = 54,
    AH            = 55,    /*��ʱ*/
    NOUNIT        = 255
}ENUM_UNIT;

/*TI��λö�ٶ���*/
typedef enum
{
	EN_TI_UNIT_SEC   = 0,  /*TI��λ:��*/
	EN_TI_UNIT_MIN   = 1,  /*TI��λ:��*/
	EN_TI_UNIT_HOUR  = 2,  /*TI��λ:ʱ*/
	EN_TI_UNIT_DAY   = 3,  /*TI��λ:��*/
	EN_TI_UNIT_MONTH = 4,  /*TI��λ:��*/
	EN_TI_UNIT_YEAR  = 5   /*TI��λ:��*/
}ENUM_TI_UNIT;

/*ʱ��Դö�ٶ���*/
typedef enum
{ 
	EN_CLR_SRC_INSIDE    = 0,  /*�ڲ���0��*/
	EN_CLR_SRC_RTC       = 1,  /*ʱ��оƬ��1��*/
	EN_CLR_SRC_INTERNET  = 2,  /*������ʱ�ӣ�2��*/
	EN_CLR_SRC_SATELLITE = 3,  /*����ʱ�ӣ�3��*/
	EN_CLR_SRC_LONG_WAVE = 4,  /*����ʱ�ӣ�4��*/	
}ENUM_CLR_SRC;

/*��ENUM_DATATYPEһ������ENUM_DATATYPE����ע�ͺ�ɾ��*/
typedef enum
{
   TNULL        = 0,
   TARRAY       = 1,      /*����*/
   TSTRUCT      = 2,      /*structure*/
   TBOOL        = 3,
   BITSTRING    = 4,
   CHAR4        = 5,      /* double-long*/
   UCHAR4       = 6,      /* double-long-unsigned*/
   OCTSTRING    = 9,
   VISIBLESTRING = 10,
   UTF8STRING    = 12,
   TBCD          = 13,
   CHAR1         = 15,
   CHAR2         = 16,
   UCHAR1        = 17,
   UCHAR2        = 18,
   CHAR8         = 20,
   UCHAR8        = 21,
   TENUM         = 22,
   FLOAT4        = 23,
   FLOAT8        = 24,
   DATEANDTIME   = 25,
   TDATE         = 26,
   TTIME         = 27,
   DATETIMEBCD   = 28,
   DATETIMEBCDH  = 29,
   DATETIMEBCDS  = 30,
   TDateBCD      = 31,
   TOI           = 80,
   TOAD          = 81,
   TROAD         = 82,
   TOMD          = 83,
   T_TI          = 84,
   TTSA          = 85,
   TMAC          = 86,
   TRN           = 87,
   TREGION       = 88,
   TScaler_Unit  = 89,
   TRSD          = 90,
   TCSD          = 91,
   TSID          = 93,      //SID 0x5d
   TSIDMAC       = 94,      //SID_MAC 0x5e
   TCOMDCB       = 95,
   SPEBITSTRING  = 243,   
   TVISIBLESTRING= 245,    /*�䳤�����ַ���*/
   SPEPROCESS    = 246,
   DEMANDTYPE_R  = 247,
   SPEARRAY      = 248,     /*�䳤����*/
   DEMANDTYPE    = 249,
   MAX_TYPE_698_DATA = 250
}EN_TYPE_698_DATA;

#define DEMANDTYPELENGTH 11


/*�������ݴ����DAR����DATA*/
typedef enum
{
   E_RESULT_CHOICE_DAR     = 0, /*��������ΪDAR�����������DAR*/
   E_RESULT_CHOICE_DATA    = 1, /*��������ΪDATA,�����������������*/
}EN_RESULT_CHOICE;


#define NO_SERVICE_TYPE   0x0  
/*6.3.4��Ӧ�ò�Э�����ݵ�Ԫ Ӧ�ò����  CHOICE����*/
typedef enum
{
   EN_CONNECTREQUEST     = 2,    /*��������*/
   EN_DISCONNECTREQUEST  = 3,    /*�Ͽ�����*/
   EN_GETREQUEST         = 5,    /*������*/
   EN_SETREQUEST         = 6,    /*��������*/
   EN_ACTIONREQUEST      = 7,    /*��������*/
   EN_SECURITYREQUEST    = 0x10, /*��ȫ����*/
   EN_ABNORMAL_REPONSE   = 238   /*�쳣��Ӧ*/
}EN_SERVICE_TYPE;

/*��ȡ��choice����*/
typedef enum
{
   EN_GET_NORMAL         = 1,  /*��ȡһ����������*/
   EN_GET_NORMAL_LIST    = 2,  /*��ȡ���ɸ���������*/
   EN_GET_RECORD         = 3,  /*��ȡһ����¼�Ͷ�������*/
   EN_GET_RECORD_LIST    = 4,  /*��ȡ���ɸ���¼�Ͷ�������*/
   EN_GET_NEXT           = 5,  /*��ȡ��֡��Ӧ����һ�����ݿ�*/
   EN_GET_MD5            = 6   /*��ȡһ���������Ե�MD5ֵ*/
}EN_GET_CHOICE;


/*������choice����*/
typedef enum
{
   EN_SET_NORMAL                = 1,  /*����һ����������*/
   EN_SET_NORMAL_LIST           = 2,  /*�������ɸ���������*/
   EN_SET_THEN_GET_NORMAL_LIST  = 3,  /*���ú��ȡ���ɸ���������*/
}EN_SET_CHOICE;

/*������choice����*/
typedef enum
{
   EN_ACTION_NORMAL                = 1,  /*����һ����������*/
   EN_ACTION_NORMAL_LIST           = 2,  /*�������ɸ���������*/
   EN_ACTION_THEN_GET_NORMAL_LIST  = 3,  /*�������ȡ���ɸ���������*/
}EN_ACTION_CHOICE;

/*Э��ջ���壬�������ɾ��*/
typedef enum
{
   SIDMACDATA       = 0,       /*SID_MAC ������֤��*/
   RNDATA           = 1,       /*�����*/
   RNMACDATA        = 2,       /*�����+����MAC*/
   SIDDATA          = 3,       /*��ȫ��ʶ*/
   MAX_ENCRYPT_TYPE = 4
}EN_ENCRYPT_TYPE;

/*RSDѡ�񷽷�*/
typedef enum
{
   EN_SELECTOR1 = 1,
   EN_SELECTOR2 = 2,
   EN_SELECTOR9 = 9,
}EN_SELECTOR;


typedef enum
{
    E_UN_ENCRYPT_TYPE      = 0,   /*����*/
    E_UN_ENCRYPT_MAC_TYEP  = 1,   /*����+MAC*/
    E_ENCRYPT_TYPE         = 2,   /*����*/
    E_ENCRYPT_MAC_TYPE     = 3    /*����+MAC*/
}EN_CRYPT_TYPE_SAFE_MODE;    

/**************************************************/
/*��Э��ջ�ӿ�*/
typedef struct 
{
  EN_SERVICE_TYPE  servicetype; /*��������*/
  EN_ADDRTYPE      AddrType;   /*"��ַ����"*/

  INT8U  Crypt_Type;           /*���ܼ��� 0�����ģ�1������+MAC��2�����ģ�3������+MAC*/
  INT16U Len_data;             /*���ܺ��APDU���ȣ��ͱ��������ݳ���*/ 

  //INT8U  PortNum;     /*ͨ�Ŷ˿�*/
  //INT8U  res[3];      /*�ֽڶ���,Ԥ��*/

  INT8U *pdata;       /*Դ���ݣ��Լ����ر���������*/
}ST_OBJECT;

typedef struct
{
    INT16U rcrd_obis;/*" OBIS "*/
    INT8U  mode; /*" �ϱ�ģʽ "*/
}ST_RPT_ACTION;

typedef struct 
{
  INT8U Year;
  INT8U Month;
  INT8U Day;
  INT8U Hour;
  INT8U Min;
  INT8U Sec;
}ST_DATA_TIME;

/*ɸѡ����1*/
typedef struct{
	ST_DATA_TIME Time;       /*ɸѡ��ʱ��*/
}_ST_RSD_SELECT1;


/*ɸѡ����2*/
typedef struct{ 
	ST_DATA_TIME Time_begin;  /*"��ʼʱ��"*/
	ST_DATA_TIME Time_end;    /*"����ʱ��"*/
	INT8U  Period[3];	          /*"ʱ����[0]Ϊ�֡�ʱѡ��"*/
}_ST_RSD_SELECT2;  //15byte?

#define LEN_SELECT2_DATA   sizeof(_ST_RSD_SELECT2)    //15byte?


/*���������Ľ�������¼ģ��ṹ��*/
typedef struct
{       
    INT16U Line_start;   /*ɸѡ�Ŀ�ʼ��*/
    INT16U Line_end;     /*ɸѡ�Ľ�����  0xffff��ʾ��һ��ɸѡ�������������*/
    
    INT8U  NumOad;       /*ɸѡ��OAD����*/ 
    INT8U  Rer[3];       /*Ϊ�ֽڶ��룬Ԥ��3�ֽ�*/
    
    ST_OAD *Poadlist;    /*ɸѡ��OAD�б�*/
    void   *pDdataFilter;/*ɸѡ����������*/
}ST_Filter_Record_In;  

/*���������Ľ�������¼ģ��ṹ��*/
typedef struct
{   
    INT16U NumRecord;   /*��¼���� ��Ҫ�ǵ�һ��ɸѡʱ����*/
    INT8U  Reser[2];    /*���� �ֽڶ���*/
    INT8U *pleng;       /*�����������:����Ϊ0����ʾ������*/
    INT8U *pbuf;        /*���ݻ����ַ�����δ洢*/
}ST_Filter_Record_Out;  
/**************************************************/
/*ʱ���ǩ ��ʱע���ֽڶ���*/
typedef struct
{
  INT8U OptByte;         /*ѡ���ֽ�0:�ޣ�1:��ʱ���ǩ*/  
  
  /*����ʱ��*/
  INT16U year;
  INT8U  month;
  INT8U  day;
  INT8U  hour;
  INT8U  minute;
  INT8U  second;

  /*��������ʱʱ��*/
  INT8U   unit;         /*ENUMERATED,��λ:��:0,��:1,ʱ:2,��:3,��:4,��,5*/
  INT16U  interval;     /*���ֵ  long-unsigned*/
}ST_TIME_TAG ;

#define LEN_TIME_TAG    (11) // sizeof(ST_TIME_TAG)

/*�����¼��ϱ�����1,2,0x33,0x20,0x02,0x01,0x01,TOAD OAD(4) 20150200 01 BITSTRING  4 4BYTE*/
/*01 02 33 20 02 01 01 51 00 00 00 00 20 15 02 00 01 04 20 00 00 00 00*/
#define MAX_LEN_FOLLOW_REPORT  23

#define LEN_RESERVE    8       /*Ԥ��8�ֽڣ����Խ����*/


/*����ʱ����������date_time_s ��ʱע���ֽڶ���*/
#if 0
typedef struct
{
  INT16U year;
  INT8U  month;
  INT8U  day;
  
  INT8U  hour;
  INT8U  minute;
  INT8U  second;
}ST_DATE_TIME_S ;
#endif

#define LEN_DATE_TIME_S     (7)  //sizeof(ST_DATE_TIME_S)


/*ʱ����TI��������*/
typedef struct
{
  INT8U   unit;         /*ENUMERATED,��λ:��:0,��:1,ʱ:2,��:3,��:4,��,5*/
  INT16U  interval;     /*���ֵ  long-unsigned*/
}ST_TI;



/*��ȫģʽ�����ṹ�嶨��*/
typedef struct
{
	UN_OI Oi;
	INT8U Rigth2;   /*�ο� 698Э�� ��¼��F.2����ȫģʽ����*/
	INT8U Rigth3;
}ST_F101_SAFE_MODE;

/*8����������Ժͷ���ö��*/
typedef enum
{
    ATTRIBUTE_2_SET_PARAMENT  = 2,             /*����2�����ò���)*/
    ATTRIBUTE_3_STATUS_RELAY  = 3,             /*����3���̵������״̬��ֻ��)*/
    ATTRIBUTE_4_STATUS_ALARM  = 4,             /*����4(�澯״̬��ֻ��)*/
    ATTRIBUTE_5_STATUS_CMD    = 5,             /*����5������״̬��ֻ��)*/
    METHOD_127_ALARM          = 127,           /*����127�������澯��������������=NULL*/
    METHOD_128_CANCEL_ALARM   = 128,           /*����128�������������������=NULL*/
    METHOD_129_OPEN           = 129,           /*����129����բ��������*/
    METHOD_130_CLOSE          = 130,           /*����130�����ĺ�բ��������*/
    METHOD_131_CLOSE          = 131,           /*����131�����������բ��������*/
}EN_ATT_METHOD_REMOTE_698;
/*-----------------------------------------------*/
#define LEN_RN         16      /*���������*/
#define LEN_MAC_DATA   7       /*01 00 04 4byte mac*/


#define LEN_GET_DAR      6    /*get dar�Ĵ������*/
#define LEN_SET_DAR      5    /*set dar�Ĵ������*/
#define LEN_ACTION_DAR   6    /*action dar�Ĵ������*/

#define LEN_ATT1_ENCODE  8    /*���ATT1����*/

#define LEN_LOGIC_NAME   4    /*�߼���*/

#define LEN_SCALE_UNIT   3    /*���㵥λ*/

/*bit5��bit7�����ʾ�������������������Ƕ���ͬһ�������ڲ�ͬ���ջ�����ȡֵģʽ��ȡֵ0��7�����������ھ�����������������*/
#define MASK_AID_SELECT     0x1F   /*AID ����*/

#define MASK_AID_SELECT_B5_B7   0xE0   /**/

/*�˳��ú꣬�ĳ�λ��*/
#define DIV_2(num)       ((num)>>1)  /*��2*/
#define DIV_4(num)       ((num)>>2)
#define DIV_8(num)       ((num)>>3)
#define DIV_16(num)      ((num)>>4)
#define DIV_32(num)      ((num)>>5)
#define DIV_64(num)      ((num)>>6)
#define DIV_128(num)     ((num)>>7)
#define DIV_256(num)     ((num)>>8)

#define MULTI_2(num)     ((num)<<1)   /*��2*/
#define MULTI_4(num)     ((num)<<2)
#define MULTI_8(num)     ((num)<<3)
#define MULTI_16(num)    ((num)<<4)
#define MULTI_32(num)    ((num)<<5)
#define MULTI_64(num)    ((num)<<6)
#define MULTI_128(num)   ((num)<<7)
#define MULTI_256(num)   ((num)<<8)
/*�˳��ú꣬�ĳ�λ�� end*/



#define MAXGETLISTNUMBER 		 64    /* get list ���������  */
#define MAXGETRECORDLISTNUMBER   3     /* getrecordlist��������� */
#define MAXCSD                   55    /*���ﲻӦ�����ư�*/
#define MAXSETLISTNUMBER 		 10
#define MAXSETTHENGETLISTNUMBER  5
#define MAXACTIONLISTNUMBER      10
#define MAXESAMGETDATALEN        32   /*ESAM GET��󷽷�����*/

#define MAXACTIONTHENLISTNUMBER MAXSETTHENGETLISTNUMBER

/*�Ƿ�����ϱ�*/
#define  FLAG_FOLLOW_REPORT_NUM     mEvtPrtStatus.OadNum[Report_Channal()]
#define  FLAG_FOLLOW_REPORT_STATUS  mEvtPrtStatus.rptFlg[Report_Channal()]           


#define   COMM_SET_CHANGE_FORMAT    1  /*�Ƿ���ת��ʽ*/

#define MAX_IID_2014   7     /*���ܱ�����״̬��*/

#define MAX_IID_DAILY_SLOT_TABLE 8  /*��ʱ�α������IID*/
#define MAX_IID_YEALY_SLOT_TABLE 14 /*ʱ�������IID*/
#define MAX_IID_TRIFF 63            /*���ʵ�����IID*/


#ifdef THREE_PHASE
#define MAX_CLASS2_LIST_NUM    10
#define MAX_CLASS3_LIST_NUM    5
#define MAX_CLASS4_LIST_NUM    6
#define MAX_CLASS6_LIST_NUM    29
#else
#define MAX_CLASS3_LIST_NUM    2
#define MAX_CLASS4_LIST_NUM    2
#define MAX_CLASS6_LIST_NUM    18
#endif


#define MAX_DATA_LEN_ACTION     312   /*������������ݳ���*/
#define MAX_ONE_OAD_DATA_LEN    384   /*һ��OAD�����ݳ���*/

#ifdef THREE_PHASE
#define MAX_ONE_RECORD_DATA_LEN 1000   /*һ����¼�����ݳ���*/
#else
#define MAX_ONE_RECORD_DATA_LEN 512   /*һ����¼�����ݳ���*/
#endif


#define MAX_CODE_CMP_LEN        (1024+16+2) /*1042�ռ��һ�αȽ�4096�ֽ�*/
//#if THREE_PHASE
//#define MAX_CODE_CMP_LEN        540
//#else
//#define MAX_CODE_CMP_LEN        300
//#endif

#define MAX_DATA_LEN_SET        MAX_DATA_LEN_ACTION  

#define MAX_OBIS_NUM_ONE_OAD    MAX_ID_LIST_NUM

/*�¼�ȫ�壬��һ�������OBIS����*/
#define CLR_ALL_EVENT_OBIS    0xfffe
#define CLR_METER_OBIS        0xffff    /*�����0 OBIS*/

#define E_OBIS_NULL             0xfff0    /*��Ч��OBIS*/

#define DAR_READ_FUN_DIRECT_DATA  0xAA     /*��ǰ������ֱ�ӷ�������*/


/*����������������ģ�����*/
extern INT16S PP_process(ST_OBJECT *pObject);
extern INT8U PP_OadToObis(ST_OAD *pOad, INT16U *pObis, INT16U *pOffset, ST_FMT_ATTR *pFormat);
extern INT8U PP_ObisToOad(INT16U Obis,ST_OAD *pOad);
extern void Init_Safe_Mode(INT8U mode);

extern ST_OAD CurOad;
extern ST_OAD Program_Oad[10]; 
/********************************************************/
#endif
