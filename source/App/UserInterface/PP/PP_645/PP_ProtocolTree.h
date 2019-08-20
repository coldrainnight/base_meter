#ifndef _PROTOCOL_TREE_H
#define _PROTOCOL_TREE_H

#include "app\UserInterface\display\dm.h"
#include "App\UserInterface\ProtocolStack\PS_645\Dlt645.h"
#include "App\UserInterface\PP\PP_645\645Data.h"

/*"�����¼���¼��¼�ļ���¼����"*/
#ifdef SINGLE_PHASE
#define _TARIFF_NUM 5         /*��+4����*/
#else
#define _TARIFF_NUM 9         /*��+8����*/
#endif

#define _NUM_PD             (10)        /*"�����¼���¼����"*/
#define _NUM_PWR_ERR        (10)        /*"��Դ�쳣�¼���¼����"*/
#define _NUM_LOAD_SW_ERR    (10)        /*"���ɿ�����������"*/
#define _NUM_OVER_CURR      (10)        /*"�����¼���¼"*/
#define _NUM_OPEN_CVR       (10)        /*" ����Ǵ��� "*/
#define _NUM_TRIP_SW        (10)        /*" ��բ��¼ "*/
#define _NUM_CLS_SW         (10)        /*" ��բ��¼ "*/
#define _NUM_PRG            (10)        /*" ��� "*/
#define _NUM_JUST_TIME      (10)        /*" Уʱ "*/
#define _NUM_CLR_MTR        (10)        /*" ������� "*/
#define _NUM_CLR_EVT        (10)        /*" �¼����� "*/
#define _NUM_PRG_AEMODE     (10)        /*" �й���Ϸ�ʽ��� "*/
#define _NUM_PRG_BILL       (10)        /*" �����ձ�� "*/
#define _NUM_PRG_DSLOT      (2)         /*" ʱ�α��� "*/
#define _NUM_PRG_YSLOT      (2)         /*" ʱ������ "*/
#define _NUM_PRG_WEEK       (2)         /*" �����ձ�� "*/
#define _NUM_KEY_UPDATE     (10)        /*" ��Կ���� "*/
#define _NUM_RESET          (10)        /*" ��λ�¼���¼"*/
#define _NUM_BRD_JUST_TIME  (100)        /*" �㲥Уʱ "*/
#define _NUM_NONLEG_REP		(10)		/*" ����Ԫ�����¼���¼"*/
#define _NUM_NONLEG_UPD		(10)		/*" ����Ԫ�����¼���¼"*/
#define _NUM_MGMT_COMM_ERR		(10)		/*" ����Ԫͨ���쳣�¼���¼"*/

#define _NUM_PRG_TARIFF     (10)        /*" ���ʱ��� "*/
#define _NUM_PRG_STEP       (10)        /*" ���ݱ��� "*/
#define _NUM_CARD_ERR       (10)        /*" �쳣�忨 "*/
#define _NUM_BUY_ENGY       (10)        /*" �����¼ "*/
#define _NUM_REFUND         (10)        /*" �˷Ѽ�¼ "*/

#define _YEARSLOT_NUM 14        /*"��ʱ����"*/
#define _DLYSLOT_TAB_NUM 8        /*"��ʱ�α���"*/
#define _DAILYSLOT_NUM 14        /*"��ʱ����"*/
#define _HOLIDAYSLOT_NUM 254        /*"�����ڼ�����"*/
//#define _TARIFF_PRC_NUM 12    /*"���ʵ����"*/

#define _YEARBILL_NUM 6    /*"������ո���"*/
#define _STEP_TAB_NUM 4 /*"���ݱ����"*/
#define _STEP_NUM 6    /*"�ݶ���"*/
#define _STEP_PRC_NUM 7    /*"�ݶȵ����"*/
#define _STEP_YEARSLOT_NUM 4    /*"����ʱ�������"*/

#define    _NUM_MSR_RCD              (2160)   /*"�Լ�����ݵ���"*/

#define _CYC_DISP_NUM  99
#define _KEY_DISP_NUM  99

#define _EACH_SLOT_SIZE  3        /*"��ʱ������ʱ�α�ĳ�Ա����"*/
#define _EACH_HOLI_SIZE  4        /*"�����ڼ��յĳ�Ա����"*/

#ifdef SINGLE_PHASE /*"��ʾ��Ŀ���ݵĳ�Ա����"*/
#define _DISP_CODE_SIZE 1    
#else
#define _DISP_CODE_SIZE 5        
#endif

#ifdef SINGLE_PHASE
#define _ENERGY_SIZE 4
#else 
#define _ENERGY_SIZE 6

#endif


#ifndef _DAILYSLOT_NUM 
#define _DAILYSLOT_NUM 14        /*"��ʱ����"*/
#endif


typedef struct       /*"����ģʽ�� ���ֽ�HEX����"*/
{
    INT8U  prdFrzIntval;        /*"���ڶ���ʱ���������ӣ�"*/
/*"����5������Э��������ѹ����˳����������Ķ�"*/
    UN_BYTE_DEF  timeFrzMode;      /*"��ʱ����ģʽ��"*/
    UN_BYTE_DEF  mmtFrzMode;        /*"˲ʱ����ģʽ��"*/
    UN_BYTE_DEF  aptFrzMode;        /*"Լ������ģʽ��"*/  
    UN_BYTE_DEF  prdFrzMode;        /*"���ڶ���ģʽ��"*/  
    UN_BYTE_DEF  dayFrzMode;        /*"�ն���ģʽ��"*/  
/*"----------------------------------------------------------------------"*/
}ST_FRZPARA_1HEX;



#ifndef    NUM_ENGY_BILL
#define    NUM_ENGY_BILL            (12)   /*" �½��� "*/
#endif

#define    _NUM_TIME_FRZ             (60)   /*" ��ʱ���� "*/
#define    _NUM_MMT_FRZ              (3)    /*" ˲ʱ���� "*/
#define    _NUM_YSLOT_SW_FRZ         (2)    /*" ����ʱ�����л����� "*/
#define    _NUM_DSLOT_SW_FRZ         (2)    /*" ������ʱ���л����� "*/
#define    _NUM_DAY_FRZ              (62)   /*" �ն��� "*/
#define    _NUM_PERIOD_FRZ           (254)  /*" ���㶳�� "*/
#define    _NUM_ENGY_BILL            (12)   /*" ���� "*/
#define    _NUM_MON_BILL             (10)   /*"���ۼƵ�������"*/

#define    _NUM_TARIFF_SW_FRZ        (2)    /*" ���ʵ���л����� "*/
#define    _NUM_STEP_SW_FRZ          (2)    /*" ���ݵ���л����� "*/
#define    _NUM_PMMODE_SW_FRZ        (2)    /*" �����ѿ�ģʽ�л�����"*/
#define    _NUM_STEPYSLOT_SW_FRZ     (2)    /*" �������׽���ʱ�����л�����"*/
#define    _NUM_PREPAY_FRZ           (62)   /*" ʣ�����ն��� "*/
#define    _NUM_YEAR_BILL            (10)   /*"���ۼƵ�������"*/

/*"���������¼��ṹ����˳��ָ�롢�ۼƴ������谴���˳����ṹ"*/
typedef enum  
{
    OPEN_METER_COVER = 0,   /*"�����"*/
    PROGRAM,        /*"���"*/
    METER_CLR,      /*"�������"*/
    EVENT_CLR,      /*"�¼�����"*/
    DAILYSLOT_PROG,    /*"ʱ�α���"*/
    YEARSLOT_PROG,     /*"ʱ������"*/
    WEEKEND_PROG,      /*"�����ձ��"*/
    AEMODE_PROG,       /*"�й���Ϸ�ʽ���"*/

    BILLDAY_PROG,     /*"�����ձ��"*/
    AJUST_CLK,       /*"Уʱ"*/
    BUY_ENGY,   /*"�����¼�"*/
    TRIP_SW,        /*"��բ��¼"*/
    CLS_SW,      /*"��բ��¼"*/
    PRG_TARIFF,      /*"���ʵ�۱���"*/

    PRG_STEP,    /*"���ݵ�۱���"*/
    KEY_UPDATE,     /*"��Կ����"*/
    CARD_ERR,      /*"�쳣�忨"*/
    REFUND,       /*"�˷�"*/
    LOAD_SW_ERR,   /*"���ɿ�������"*/   
    PMMODE_PROG, /*"�ѿ�ģʽ���"*/
    OPER_EVT_SIZE
}EN_OPER_EVT;

/*******************************************/

#define MAX_LEAF_NUM   155
#define PROTOCOL_MAX_BLOCK_NUM  8
#define MAX_FUN_BUFF_SIZE	100

#if ALIGN_BYTE == 1
#pragma pack(1)
#elif ALIGN_BYTE == 2
#pragma pack(2)
#elif ALIGN_BYTE == 4
#pragma pack(4)
#endif

typedef struct  _TREE_LEAF_LIST
{
	INT8U Tree_addr[2];     /*"Ҷ�ӵ�ַ"*/   /*"�¼��ڵ��ַ/Ҷ�Ӷ����ַ"*/
	INT8U Revise[2];        /*"��ַƫ����  ����ѹ��Ҷ�� "*/
}TREE_LEAF_LIST;




/*"�ڵ����Խ���"*/
typedef union _CHECK_NODE_ID_RET
{
	struct CHECK_NODE_BIT
	{
		INT8U low:1;     /*"1�¼��ڵ�     0  ���ڵ�"*/
		INT8U zip1:1;    /*"1 һ��ѹ��    0  ��һ��ѹ��"*/
		INT8U zip2:1;    /*"1 ����ѹ��    0  �޶���ѹ��"*/
		INT8U num1 :2;   /*"0-3 һ��ѹ��nѹ���ţ�DI(n)��"*/
		INT8U num2 :2;   /*"0-3 ����ѹ��nѹ���ţ�DI(n)��"*/
		INT8U error:1;   /*"1���Ҵ���      0��ȷ��"*/
	}bit;
	
	INT8U	byte;
}CHECK_NODE_RET;

typedef union _TCommError/*"ͨ�Ŵ����������ݽṹ����"*/
{
    struct
    {
        INT8U errOther :1; /*"��������"*/
        INT8U errObis :1; /*"����������"*/
        INT8U errPassword :1; /*"�����/δ��Ȩ"*/
        INT8U errBaud :1; /*"ͨ�����ʲ��ܸ���"*/
        INT8U errYearZone :1; /*"��ʱ������"*/
        INT8U errDailySlot :1; /*"��ʱ������"*/
        INT8U errTriff :1; /*"��������"*/
        INT8U :1;  /*"����"*/
    } ST_ASBITS;
    INT8U asByte;
}UN_COMM_ERR;

typedef union _TREE_LEAFADDR{
	INT16U	Int_addr;
	INT8U   Byte_addr[2];
}TREE_LEAFADDR;

extern INT8U PP_CommGetData (ST_FRM_RESULT *framedata,INT16U Start_addr, INT16U MaxLen,ST_ANSWER_DATA * panswer_data);
extern INT8U PP_CommSetData (ST_FRM_RESULT *framedata,
                    ST_ANSWER_DATA *panswer_data);
#ifdef DM_DLT645
extern INT8U PP_ShowGetData (ST_DISP_OBJ *pdata_in,
                           ST_SHOWDATA_OUTPUT *pdata_out );
#endif

/*"************дǰ��д�󡢶�ǰ������*********************"*/
extern INT8U setSysTimeHMS(INT8U *pdata);
extern INT8U setSysTimeYMDW(INT8U *pdata);
extern INT8U setSysTimeAll(INT8U *pdata);
//extern INT8U ChkByteDataRange(INT8U *pdata);
//extern INT8U ChkWordDataRange(INT8U *pdata);
#if 0
extern INT8U setBillDay(INT8U *pdata);
extern INT8U setDailySlot(INT8U *pdata);
extern INT8U setYearSlot(INT8U *pdata);
extern INT8U setDayFrz(INT8U *pdata);
extern INT8U setPrdFrzBgnTime(INT8U *pdata);
extern INT8U setLrBgnTime(INT8U *pdata);
extern INT8U setHoliday(INT8U *pdata);
#endif
extern INT8U setBaudRate(INT8U *pdata);
//extern INT8U setTariffSWdate(INT8U *pdata);
extern INT8U setActEngyMode(INT8U *pdata);
extern INT8U setPulseConst(INT8U *pdata);
extern INT8U setCommAddr(INT8U *pdata);
extern INT8U setDis(INT8U *pdata);
extern INT8U readNosupportData(INT8U *pdata);
extern INT8U  readHRVoltCurr(INT8U *pdata);
extern INT8U readDis(INT8U *pdata);
extern INT8U ReadFrzBit0Data(INT8U *pdata);
extern INT8U ReadFrzBit1Data(INT8U *pdata);
extern INT8U ReadFrzBit7Data(INT8U *pdata);
//extern INT8U ReadNowBillEgy(INT8U *pdata);
//extern INT8U ReadLastBillEgy(INT8U *pdata);
//extern INT8U OutPut_BattVal(INT8U *pdata);
//extern INT8U readFrzBlk(INT8U *pdata);
extern INT8U check_is_lr645_id(UN_ID645 id, INT8U mode, INT8U data_len);
extern INT16U LR_GetRecord(ST_FRM_RESULT *pInBuff,ST_ANSWER_DATA *pOutBuff, INT8U mode);
extern void FrzObisUnzip(INT16U *pDataObis);
extern void FrzDataZipEnd(INT8U *pEndId);
extern INT8U readFrzBlk(INT8U *pdata);
extern INT8U PP_CommGetDataExt (UN_ID645 dataId,
		            INT16U Start_addr, INT16U MaxLen,
		            ST_ANSWER_DATA * panswer_data);
#endif
