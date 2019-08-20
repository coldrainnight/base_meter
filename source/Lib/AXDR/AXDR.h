#ifndef _AXDR_H
#define _AXDR_H

#include "System\Sys_def.h"
typedef enum
{
    /*基础部分数据*/
    null_data = 0,
    array = 1,				
    structure = 2,
    bool_t = 3,				
    bitstring =4,
    doublelong = 5,
    doublelongunsigned = 6,
    octetstring = 9,
    visiblestring	= 10,
    UTF8string = 12,
    integer =	15,
    long_t	= 16,
    unsigned_t = 17,
    longunsigned	=	18,
    long64		=   20,
    long64unsigned = 	21,
    enum_t	= 22,
    float32 = 23,
    float64 = 24,

    /*扩展复合部分*/

    
    date_time = 25,
    date = 26,
    time = 27,
    date_time_s = 28,

    /*698扩展部分*/
    OI_t = 80,
    OAD_t	= 81,
    ROAD_t = 82,
    OMD_t = 83,
    TI_t  = 84,
    TSA	= 85,
    MAC	= 86,
    RN	= 87,
    Region = 	88,
    Scaler_Unit = 89,
    RSD   = 90,
    CSD	= 91,
    MS	= 92,
    SID	= 93,
    SID_MAC = 94,
    COMDCB  = 95,
    RCSD    = 96

}ENUM_DATATYPE;

/*CSD编码choice*/
#define CHOICE_OAD  (0)
#define CHOICE_ROAD (1)


/*编码信息结构*/
typedef struct
{
    ENUM_DATATYPE       Type;/*类型*/
    INT8U       Len; /*长度*/
}ST_ENCODE_INFO;

#define DECODE_ERR_TYPE (-1)
#define DECODE_ERR_LEN  (-2)
#define DECODE_NO_SUPPORT  (-3)

#define BASE_ID_PP_698   200

typedef enum
{
    /*基础部分数据*/
    SID_ADDDATA_LEN  = 128,
    DIS_OBJ_NUM_CYC  = 129,
    EN_LEN_DSLOT_NUM = 130,

    E_SEASON_SLOT_ID = BASE_ID_PP_698, /*年时区数*/
    E_DAY_TBL_ID,               /*日时段表数*/ 
    E_DAY_SLOT_ID,              /*日时段数(每日切换数)*/
    E_TARIFF_ID,                /*总及费率数*/
    E_SPECIAL_DAY_ID,           /*公共假日数*/    
    
    E_PHASE_VARIBLE_ID,         /*分相变量ID*/
    E_PHASE_VARIBLE_SUM_ID,     /*总及分相变量ID*/
    E_TRIFF_ID_PHASE,           /*费率，不包括总*/
    E_RELATION_OBJECT_ID,       /*事件关联对象个数*/
    E_RELATION_FREEZE_OBJECT_ID,/*冻结关联对象个数*/
    E_BILL_DATE_ID,             /*结算日，也要改成块*/
    
    UNFIXED_LEN = 255  /*非固定长度数据*/

}ENUM_DATALEN_ID;

/*特殊的获取长度钩子函数*/
/*有匹配ID则返回>=0，无匹配返回<0*/
typedef INT16S (*ArrStrLenHook)(ENUM_DATALEN_ID Id);

extern void   AXDR_DataLenGetInit(ArrStrLenHook HookFun,INT8U Mod);

extern INT16U EnCode_LenArea(INT16U Len,INT8U *pDst);
extern INT16U EnCode_Base(INT8U *pSrc,ENUM_DATATYPE Type, INT16U SrcLen,INT8U *pDst);
extern INT16U EnCode_Simple(INT8U *pSrc,ST_ENCODE_INFO * pInfo,INT8U *pDst);
extern INT16U EnCode_Data(ST_ENCODE_INFO * pInfo,INT8U *pSrc,INT8U *pDst,INT8U DataNum);
extern INT8U  EnCode_Option(INT8U Flag,INT8U *pDst); 

extern INT16U DeCode_LenArea(INT8U *pSrc,INT8U *DataOffset);
extern INT16S DeCode_Base(INT8U *pSrc,ENUM_DATATYPE Type,INT8U *pDst);
extern INT16S DeCode_Simple(INT8U *pSrc,ST_ENCODE_INFO * pInfo,INT8U *pDst);
extern INT16S DeCode_Data(ST_ENCODE_INFO * pInfo,INT8U *pSrc,INT8U *pDst,INT8U DataNum);
extern INT16S DeCode_Data_Len(ST_ENCODE_INFO * pInfo,INT8U *pSrc,INT8U DataNum,INT16U * pSrcLen);
extern INT8U  DeCode_Option(INT8U *pSrc); 




#endif

