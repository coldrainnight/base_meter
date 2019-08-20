/*"************************************************************"*/
/*"FileName: TR.h                                              "*/
/*"Author: 尹凌                                                "*/
/*"Version : 1.0       Date: 2017.11.05                        "*/
/*"Description: 显示按键模块              "*/
/*"Function List:                                              "*/
/*"<FunctionName>                                              "*/
/*"<author> <time> <version > <desc>                           "*/
/*"************************************************************"*/

#ifndef _DM_H_
#define _DM_H_

#include "system\Sys_def.h"
#include "Driver\Hal\DIS\DIS.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include"app\userinterface\PP\PP_698\Process\Pp_macro.h"
#include "App\MtrParamDefault\MtrParamDef.h"


typedef enum
{
    CYC_SHOW_MODE = 0,/*"循显"*/
    KEY_SHOW_MODE,/*"按显"*/
    //TEST_SHOW_MODE,/*"测试"*/
    PD_CYC_SHOW_MODE,/*"低功耗循显"*/
    PD_KEY_SHOW_MODE,/*"低功耗按显"*/
    INIT_SHOW_MODE,/*"上电初始化显示"*/
    SPEC_MODE,/*"液晶查看"*/
    PD_INIT_SHOW_MODE,/*"掉电初始化显示"*/

    MAX_SHOW_MODE
}EN_SHOW_MODE;/*"显示模式列表"*/

typedef enum
{
    DISP_VER_ID = 0,
    DISP_ALL_ID,

    MAX_INIT_NUM
}EN_INIT_ITEM;/*"上电显示项目列表"*/

typedef enum
{
    CYC_LIST = 0,
    KEY_LIST,

    MAX_LIST_TYPE
}EN_DISP_LIST;/*"含显示列表的最大模式ID"*/

typedef enum
{
    RUN_MODE_PD = 0,
    RUN_MODE_INIT,
    RUN_MODE_NORMAL,

    MAX_RUN_MODE
}EN_DISP_RUN_MODE;/*"工作模式"*/



#define DISP_OBJ_SIZE    sizeof(ST_DISP_OBJ)

#ifndef DM_DLT698
#define SHOW_ALL_TIME    (5)/*"全屏时间"*/
#endif
#define SHOW_VER_TIME    (3)/*"显示版本号时间"*/
#define SHOW_INTERVAL   (500) /*"显示刷新间隔500ms"*/

#define PD_SHOW_TIME     (604800) /*"掉电常显时间7*24*3600"*/

#ifdef THREE_PHASE
#define PD_DISP_DATA_NUM (40)/*"低功耗显示数据缓存最大项目个数"*/
#define PD_DISP_CYC_NUM (21)/*"低功耗循显对象最大项目个数"*/
#define PD_DISP_KEY_NUM (40)/*"低功耗按显对象最大项目个数"*/
#else
#define PD_DISP_DATA_NUM (25)/*"低功耗显示数据缓存最大项目个数"*/
#define PD_DISP_CYC_NUM (7)/*"低功耗循显对象最大项目个数"*/
#define PD_DISP_KEY_NUM (27)/*"低功耗按显对象最大项目个数"*/
#endif

#define DM_BLK_LEFT_SHIFT  (7)

#ifdef THREE_PHASE
#define PD_KEY_WAKE_UP  (0xAA)/*"低功耗按键唤醒标志"*/
#define PD_IR_COMM          (0xAA)/*"低功耗红外通信结束"*/
#define PD_IR_SHOW          (0x55)/*"低功耗红外唤醒显示延时"*/
#endif

#if defined(DM_DLT698)/*"698协议显示对象相关定义"*/

/*"698协议显示对象标识等于时钟"*/
#define DISP_CLOCK_OBJ   ((mstRunData.curScrCode.Oad.Oi.asInt == 0x4000) ? 1 : 0)

typedef struct
{
    INT8U Csd;
    INT8U Oad[4];	
    INT8U scrNo;
}ST_DISP_OBJ1;

typedef struct
{
    INT8U Csd;
    INT8U Oad[4];	
    INT8U cnt;
    INT8U releOad[4];
    INT8U scrNo;
}ST_DISP_OBJ2;

typedef union
{
    ST_DISP_OBJ1 item1;
    ST_DISP_OBJ2 item2;
}UN_DISP_OBJ;

typedef UN_DISP_OBJ  ST_DISP_PARA_OBJ;

#elif defined(DM_DLT645)/*"645协议显示对象相关定义"*/

#define DISP_CLOCK_OBJ   ((mstRunData.curScrCode.unId645.asLong == 0x04000101) || (mstRunData.curScrCode.unId645.asLong == 0x04000102))

#define DISP_CHECK_TIME   (10)

typedef  struct
{
    INT8U id645[4];
    INT8U leafNum;  /*"显示数据叶子号"*/
}ST_DISP_PARA_OBJ;

#elif defined(DM_IEC62056)/*"62056协议显示对象相关定义"*/

#define DISP_CLOCK_OBJ   (0)

typedef struct    
{
    INT8U  ClassID;    /*"类标识"*/
    INT8U  OBIS[6];    /*"捕获对象OBIS码"*/
    INT8U  AttriID;    /*"属性标识"*/
    INT8U  index;      /*"属性标识"*/
    INT8U  DisObis[8];    /*"用户定义显示OBIS"*/
} ST_DISP_OBJ; /*"62056协议显示对象定义"*/

#else
#endif

typedef struct/*"特殊屏显示参数定义"*/
{
    INT16U obj;/*"数据对象标识"*/
    INT16U time;/*"显示时长"*/
    //INT8U ShowPrio;
}ST_SHOW_SPEC;

#if 0
typedef struct
{
    ST_DM_PD_DATA data[PD_DISP_DATA_NUM]; /*"掉电显示数据缓存"*/
    ST_DISP_OBJ cyc[PD_DISP_CYC_NUM];/*"循显对象"*/
    ST_DISP_OBJ key[PD_DISP_KEY_NUM];/*"按显对象"*/
/*"单相max/default: 28*25 = 700 / 10*7=70 / 10*27=270   total: 1040"*/
/*"三相max: 30*97 = 2910 / 10*21=210 / 10*99=990   total: 4110"*/
/*"三相default: 30*40 = 1200 / 10*21=210 / 10*40=400   total: 1810"*/
}ST_PD_SHOW_DATA;
#endif

typedef struct
{
    ST_SHOW_DATA data; /*"掉电显示数据缓存区"*/
    ST_DISP_OBJ item;/*"显示对象"*/
}ST_DM_PD_DATA;

typedef struct
{
    ST_DISP_OBJ cyc[PD_DISP_CYC_NUM];/*"循显对象"*/
    ST_DISP_OBJ key[PD_DISP_KEY_NUM];/*"按显对象"*/
}ST_PD_OBJ_LIST;/*"低功耗显示对象列表"*/

typedef struct
{
    INT8U showMode;/*"显示模式"*/
    INT8U maxObjNum;/*"最大显示对象缓存个数"*/
    INT8U *pCurrNum;/*"当前显示对象读取个数数据地址"*/
    ST_DISP_OBJ *pDispObj;/*"显示对象指针"*/
}ST_PD_DISP_INFO;

typedef struct
{
    INT16U runErr;/*"运行异常错误代码,最多16个错误代码,参考mShowErrTab"*/
    INT8U index;/*"显示错误索引"*/
}ST_SHOW_ERR;

typedef struct
{
    ST_DISP_OBJ curScrCode;/*"当前屏显示代码"*/
#ifdef DIS_USER_CODE
    ST_USERCODE_SHOW userCode;/*"显示用户代码"*/
#endif

    INT8U showMode;/*"当前显示模式"*/
    INT8U runMode;/*"当前运行模式0_掉电/ 1_上电初始化/ 2_上电运行"*/
    INT8U index;/*"显示索引值"*/
    INT8U curNum;/*"当前显示状态屏数"*/
    INT8U showRet;/*显示成功标志*/
    INT8U pdObjNum;/*"低功耗显示数据缓存区项目个数"*/
    INT8U initShowNum;/*"初始化显示项个数"*/
    INT8U pdCycNum;/*"低功耗循显当前显示对象读取个数"*/
    INT8U pdKeyNum;/*"低功耗按显当前显示对象读取个数"*/
    INT8U validFlg;/*"当前显示模式下有效显示对象存在标志"*/
#ifdef THREE_PHASE
    INT8U keyFlg;/*"0_下翻；1_上翻"*/
    INT8U pdIrTimer;/*"低功耗红外通信唤醒延时"*/
#endif
#ifdef DIS_USER_CODE
    INT8U showUserCodeFlg;/*"显示用户代码标志"*/
#endif
    INT16U scrTimer;/*"当前屏显示时长"*/
    INT16U refreshTick;/*刷新tick*/
    //INT16U refreshInterval;/*刷新间隔*/

    INT32U pdShowTime;/*"低功耗常显总时间"*/
}ST_DM_RUN_DATA;


typedef struct
{
    INT8U *pDispNum;
    INT32U dispListAddr;
}ST_DISP_MODE_INFO;

typedef void  (* On_Key_Function)(void);

typedef struct
{
    INT16U shift;
    INT8U num;
}ST_DISP_LIST_INFO;





extern void DM_Init(INT8U mode);
extern void DM_Task(void);
extern void DM_PDMain(ST_DM_PD_DATA *pPdShowList);
extern void DM_PD1s(void);

/*"通信模块(用户接口) 调用接口"*/
extern INT16S DM_GetDisPara(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U *pBuff);
extern INT16S DM_SetDisPara(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U setLen, INT8U *pData);


/*"外部模块调用方法"*/
extern INT8S DM_ShowFlash(void *pdin, void *pdout);
extern INT8S DM_ShowSpec(void *pdin, void *pdout);
extern INT8S DM_ShowAll(void *pdin, void *pdout);
extern INT8S DM_ShowNext(void *pdin, void *pdout);
extern INT8S DM_ShowLast(void *pdin, void *pdout);
extern INT8S DM_DLT698ShowSpec(void *pdin, void *pdout);


#define stDispPara   (StUIParaWithRamBackup.StDmParaWithRamBackup.DispPara)

#ifdef THREE_PHASE
extern INT8U mPdKeyWakeUpFlg;
extern INT8U mPdIrComFlg;
#endif

extern void DM_freshDisData(ST_SHOW_DATA *inData);
#endif


