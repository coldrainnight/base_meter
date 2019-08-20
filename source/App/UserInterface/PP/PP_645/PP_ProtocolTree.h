#ifndef _PROTOCOL_TREE_H
#define _PROTOCOL_TREE_H

#include "app\UserInterface\display\dm.h"
#include "App\UserInterface\ProtocolStack\PS_645\Dlt645.h"
#include "App\UserInterface\PP\PP_645\645Data.h"

/*"各类事件记录记录文件记录次数"*/
#ifdef SINGLE_PHASE
#define _TARIFF_NUM 5         /*总+4费率*/
#else
#define _TARIFF_NUM 9         /*总+8费率*/
#endif

#define _NUM_PD             (10)        /*"掉电事件记录次数"*/
#define _NUM_PWR_ERR        (10)        /*"电源异常事件记录次数"*/
#define _NUM_LOAD_SW_ERR    (10)        /*"负荷开关误动作次数"*/
#define _NUM_OVER_CURR      (10)        /*"过流事件记录"*/
#define _NUM_OPEN_CVR       (10)        /*" 开表盖次数 "*/
#define _NUM_TRIP_SW        (10)        /*" 跳闸记录 "*/
#define _NUM_CLS_SW         (10)        /*" 合闸记录 "*/
#define _NUM_PRG            (10)        /*" 编程 "*/
#define _NUM_JUST_TIME      (10)        /*" 校时 "*/
#define _NUM_CLR_MTR        (10)        /*" 电表清零 "*/
#define _NUM_CLR_EVT        (10)        /*" 事件清零 "*/
#define _NUM_PRG_AEMODE     (10)        /*" 有功组合方式编程 "*/
#define _NUM_PRG_BILL       (10)        /*" 结算日编程 "*/
#define _NUM_PRG_DSLOT      (2)         /*" 时段表编程 "*/
#define _NUM_PRG_YSLOT      (2)         /*" 时区表编程 "*/
#define _NUM_PRG_WEEK       (2)         /*" 周休日编程 "*/
#define _NUM_KEY_UPDATE     (10)        /*" 密钥更新 "*/
#define _NUM_RESET          (10)        /*" 复位事件记录"*/
#define _NUM_BRD_JUST_TIME  (100)        /*" 广播校时 "*/
#define _NUM_NONLEG_REP		(10)		/*" 管理单元更换事件记录"*/
#define _NUM_NONLEG_UPD		(10)		/*" 管理单元升级事件记录"*/
#define _NUM_MGMT_COMM_ERR		(10)		/*" 管理单元通信异常事件记录"*/

#define _NUM_PRG_TARIFF     (10)        /*" 费率表编程 "*/
#define _NUM_PRG_STEP       (10)        /*" 阶梯表编程 "*/
#define _NUM_CARD_ERR       (10)        /*" 异常插卡 "*/
#define _NUM_BUY_ENGY       (10)        /*" 购电记录 "*/
#define _NUM_REFUND         (10)        /*" 退费记录 "*/

#define _YEARSLOT_NUM 14        /*"年时区数"*/
#define _DLYSLOT_TAB_NUM 8        /*"日时段表数"*/
#define _DAILYSLOT_NUM 14        /*"日时段数"*/
#define _HOLIDAYSLOT_NUM 254        /*"公共节假日数"*/
//#define _TARIFF_PRC_NUM 12    /*"费率电价数"*/

#define _YEARBILL_NUM 6    /*"年结算日个数"*/
#define _STEP_TAB_NUM 4 /*"阶梯表个数"*/
#define _STEP_NUM 6    /*"梯度数"*/
#define _STEP_PRC_NUM 7    /*"梯度电价数"*/
#define _STEP_YEARSLOT_NUM 4    /*"阶梯时区表个数"*/

#define    _NUM_MSR_RCD              (2160)   /*"自监测数据点数"*/

#define _CYC_DISP_NUM  99
#define _KEY_DISP_NUM  99

#define _EACH_SLOT_SIZE  3        /*"年时区表，日时段表的成员个数"*/
#define _EACH_HOLI_SIZE  4        /*"公共节假日的成员个数"*/

#ifdef SINGLE_PHASE /*"显示项目数据的成员个数"*/
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
#define _DAILYSLOT_NUM 14        /*"日时段数"*/
#endif


typedef struct       /*"冻结模式字 单字节HEX参数"*/
{
    INT8U  prdFrzIntval;        /*"周期冻结时间间隔（分钟）"*/
/*"以下5个参数协议树中有压缩，顺序请勿随意改动"*/
    UN_BYTE_DEF  timeFrzMode;      /*"定时冻结模式字"*/
    UN_BYTE_DEF  mmtFrzMode;        /*"瞬时冻结模式字"*/
    UN_BYTE_DEF  aptFrzMode;        /*"约定冻结模式字"*/  
    UN_BYTE_DEF  prdFrzMode;        /*"周期冻结模式字"*/  
    UN_BYTE_DEF  dayFrzMode;        /*"日冻结模式字"*/  
/*"----------------------------------------------------------------------"*/
}ST_FRZPARA_1HEX;



#ifndef    NUM_ENGY_BILL
#define    NUM_ENGY_BILL            (12)   /*" 月结算 "*/
#endif

#define    _NUM_TIME_FRZ             (60)   /*" 定时冻结 "*/
#define    _NUM_MMT_FRZ              (3)    /*" 瞬时冻结 "*/
#define    _NUM_YSLOT_SW_FRZ         (2)    /*" 两套时区表切换冻结 "*/
#define    _NUM_DSLOT_SW_FRZ         (2)    /*" 两套日时段切换冻结 "*/
#define    _NUM_DAY_FRZ              (62)   /*" 日冻结 "*/
#define    _NUM_PERIOD_FRZ           (254)  /*" 整点冻结 "*/
#define    _NUM_ENGY_BILL            (12)   /*" 结算 "*/
#define    _NUM_MON_BILL             (10)   /*"月累计电量结算"*/

#define    _NUM_TARIFF_SW_FRZ        (2)    /*" 费率电价切换冻结 "*/
#define    _NUM_STEP_SW_FRZ          (2)    /*" 阶梯电价切换冻结 "*/
#define    _NUM_PMMODE_SW_FRZ        (2)    /*" 南网费控模式切换冻结"*/
#define    _NUM_STEPYSLOT_SW_FRZ     (2)    /*" 南网两套阶梯时区表切换冻结"*/
#define    _NUM_PREPAY_FRZ           (62)   /*" 剩余金额日冻结 "*/
#define    _NUM_YEAR_BILL            (10)   /*"年累计电量结算"*/

/*"电表操作类事件结构定义顺序，指针、累计次数都需按这个顺序定义结构"*/
typedef enum  
{
    OPEN_METER_COVER = 0,   /*"开表盖"*/
    PROGRAM,        /*"编程"*/
    METER_CLR,      /*"电表清零"*/
    EVENT_CLR,      /*"事件清零"*/
    DAILYSLOT_PROG,    /*"时段表编程"*/
    YEARSLOT_PROG,     /*"时区表编程"*/
    WEEKEND_PROG,      /*"周休日编程"*/
    AEMODE_PROG,       /*"有功组合方式编程"*/

    BILLDAY_PROG,     /*"结算日编程"*/
    AJUST_CLK,       /*"校时"*/
    BUY_ENGY,   /*"购电事件"*/
    TRIP_SW,        /*"跳闸记录"*/
    CLS_SW,      /*"合闸记录"*/
    PRG_TARIFF,      /*"费率电价表编程"*/

    PRG_STEP,    /*"阶梯电价表编程"*/
    KEY_UPDATE,     /*"密钥更新"*/
    CARD_ERR,      /*"异常插卡"*/
    REFUND,       /*"退费"*/
    LOAD_SW_ERR,   /*"负荷开关误动作"*/   
    PMMODE_PROG, /*"费控模式编程"*/
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
	INT8U Tree_addr[2];     /*"叶子地址"*/   /*"下级节点地址/叶子对象地址"*/
	INT8U Revise[2];        /*"地址偏移量  用于压缩叶子 "*/
}TREE_LEAF_LIST;




/*"节点属性解析"*/
typedef union _CHECK_NODE_ID_RET
{
	struct CHECK_NODE_BIT
	{
		INT8U low:1;     /*"1下级节点     0  本节点"*/
		INT8U zip1:1;    /*"1 一级压缩    0  无一级压缩"*/
		INT8U zip2:1;    /*"1 二级压缩    0  无二级压缩"*/
		INT8U num1 :2;   /*"0-3 一级压缩n压缩号（DI(n)）"*/
		INT8U num2 :2;   /*"0-3 二级压缩n压缩号（DI(n)）"*/
		INT8U error:1;   /*"1查找错误      0正确·"*/
	}bit;
	
	INT8U	byte;
}CHECK_NODE_RET;

typedef union _TCommError/*"通信错误类型数据结构定义"*/
{
    struct
    {
        INT8U errOther :1; /*"其他错误"*/
        INT8U errObis :1; /*"无请求数据"*/
        INT8U errPassword :1; /*"密码错/未授权"*/
        INT8U errBaud :1; /*"通信速率不能更改"*/
        INT8U errYearZone :1; /*"年时区数超"*/
        INT8U errDailySlot :1; /*"日时段数超"*/
        INT8U errTriff :1; /*"费率数超"*/
        INT8U :1;  /*"保留"*/
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

/*"************写前、写后、读前处理函数*********************"*/
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
