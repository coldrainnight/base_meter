#ifndef MS_H
#define MS_H

#include "PlatForm.h"

#define   W_MODE   1         /*"数据输出到数据中心"*/
#define   R_MODE   0         /*"从数据中心获取数据"*/

#define   MS_SUCCESS         (0)  /*"数据交互操作成功"*/
#define   MS_OBIS_ERR        (-1)  /*"数据交互对象标识错误"*/
#define   MS_LEN_ERR         (-2)  /*"数据交互数据长度错误"*/
#define   MS_BUFF_OVERFLOW   (-3)  /*"数据交互数据缓存溢出"*/
#define   MS_LIST_NUM_OVRE   (-4)  /*"数据交互LIST数目超限"*/
#define   MS_W_AUTH_ERR      (-5)  /*"数据交互写权限错误"*/
#define   MS_ACTION_AUTH_ERR (-6)  /*"数据交互操作权限错误"*/

#define   MS_OTHER_ERR         (-7)  /*"数据交互操作失败"*/

#define   MS_YEARZONE (-17) /*"年时区数超"*/
#define   MS_DAILYSLOT (-18) /*"日时段数超"*/
#define   MS_TRIFF (-19) /*"费率数超"*/


#define   MAX_ID_LIST_NUM    (10) /*"LIST抄读时支持的最大对象数"*/

#define   SPEC_DAY_SET_SIZE  (6)    /*公共假日长度*/  
/*"----------电能表运行状态定义---------"*/
typedef enum
{
    E_FAC = 0,/*"工厂状态"*/
    E_PRIVATE, /*"私钥状态"*/
    E_PRGEVENT_START,/*"编程状态"*/
    E_FIRST_PWR_ON,/*"初次上电"*/
    E_PLM_WORKING,/*"载波模块正常通信/空闲"*/
    E_OPEN_COVER,/*"开表盖"*/
    E_OPEN_TAIL, /*"开端盖"*/
    E_CLK_ERR, /*"时钟故障"*/
    
    E_CLK_BAT_LOSS, /*"时钟电池欠压"*/
    E_DIS_BAT_LOSS, /*"低功耗电池欠压"*/
    E_MAGNET, /*"磁场干扰"*/
    E_MTR_RST,/*"电表复位"*/
    E_LRM_PD_DATA_ERR,/*"法定掉电存数数据不合法"*/
    E_OSCILLATOR_ERR,/*"晶振故障"*/
    E_RAM_VALID,/*"RAM数据维持合法"*/
    E_MTR_INIT,/*"电表初始化"*/
    
    E_MU_JUST_MTR,/*"校表状态 0 未校表  1已校表"*/
    E_RTC_CALIB, /*"RTC补偿校准状态 0 未校准  1已校准"*/
    E_LOW_PWR,/*"低功耗状态"*/
    E_MON_STEP,/*"月阶梯"*/
    E_YEAR_STEP,/*"年阶梯"*/
    E_TIME_SLOT_NOW,/*"当前运行时段  0第一套 1第二套"*/
    E_STEP_SET_NOW,/*"当前运行阶梯  0第一套 1第二套"*/
    E_TARIFF_SET_NOW,/*"当前运行费率电价  0第一套 1第二套"*/
    
    E_OVERDRAFT,/*"透支"*/
    E_ESAM_ERR,/*"ESAM故障"*/
    E_EEP_ERR,/*"EEPROM故障"*/
    E_FLASH_ERR,/*"外部FLASH存储故障"*/
    E_PREPAY,/*"预付费表   0非预付费   1预付费"*/
    E_PREPAY_ENGY,/*"预付电量表   1预付电量   0 非预付电量"*/
    E_PREPAY_MONEY,/*"预付电费表   1预付电费   0 非预付电费"*/
    E_MU_IC_ERR,/*"计量芯片故障"*/
    E_MU_DATA_READY,/*"计量数据准备就绪状态  1就绪  0未就绪"*/
    
    E_DEMAND_MODE,/*"需量计算模式  1 区间式   0 滑差式 "*/
    E_AUX_PWR,  /*"辅助电源供电  1 辅助电源供电    0 市电供电"*/
    E_BAT_PWR, /*"电池供电  1 电池供电    0 主电源供电"*/
    E_RELAY_CMD_STATUS,/*"继电器命令状态  1 跳闸    0 合闸"*/
    E_RELAY_ALARM,/*"继电器预跳闸状态  1 报警    0 无报警"*/
    E_RELAY_PROTECT,/*"保电状态  1 保电    0 非保电"*/
    E_AUTH_STATUS,/*"安全认证  1 有效    0 失效"*/
    E_IRAUTH_STATUS,/*"红外认证有效状态0无效 1有效"*/
    
    E_LOCAL_CREDIT,/*"本地开户  1 未开户    0 开户"*/
    E_REMOTE_CREDIT,/*"远程开户  1 未开户    0 开户"*/
    E_VOLT_NORMAL,/*"电压高于电表工作电压（临界电压）"*/
    E_VOLT_RELAY_OK,/*"电压高于继电器动作电压"*/
    E_VOLT_ICCARD_OK,/*"电压高于IC卡动作电压"*/
    E_VOLT_LCD_OK,/*"电压高于显示背光工作电压"*/
    E_VOLT_COM_OK,/*"电压高于通信工作电压"*/
    E_CURR_PROTECT,/*"电流大于保护电流（不允许跳闸）"*/
    
    E_PWR_DOWN, /*"掉电"*/
    E_RELAY_ERR, /*"继电器误动作"*/
    E_RELAY_OFF_SUCCESS,/*"继电器跳闸成功"*/
    E_RELAY_ON_SUCCESS,/*"继电器合闸成功"*/
    E_RELAY_STATUS,/*"继电器状态通过反馈信号和功率判断继电器通断  1 断 0 通"*/
    E_PWR_Q_LA_DIR,/*"A相无功功率方向0 正向   1  反向 （下同）"*/
    E_PWR_Q_LB_DIR,/*"B相无功功率方向"*/
    E_PWR_Q_LC_DIR,/*"C相无功功率方向"*/
    
    E_PWR_Q_DIR,   /*"总无功功率方向"*/
    E_PWR_P_LA_DIR,/*"A相有功功率方向"*/
    E_PWR_P_LB_DIR,/*"B相有功功率方向"*/
    E_PWR_P_LC_DIR,/*"C相有功功率方向"*/
    E_PWR_P_DIR,   /*"总有功功率方向 "*/
    E_LOSS_VOLT_LA, /*"A相失压"*/
    E_LOSS_VOLT_LB, /*"B相失压"*/
    E_LOSS_VOLT_LC, /*"C相失压"*/
    
    E_LOSS_VOLT_ALL,/*"全失压"*/
    E_LOSS_CURR_LA, /*"A相失流"*/
    E_LOSS_CURR_LB, /*"B相失流"*/
    E_LOSS_CURR_LC, /*"C相失流"*/
    E_BRK_PHASE_LA,/*"A相断相"*/
    E_BRK_PHASE_LB,/*"B相断相"*/
    E_BRK_PHASE_LC,/*"C相断相"*/
    E_OVER_CURR_LA, /*"A相过流"*/
    
    E_OVER_CURR_LB, /*"B相过流"*/
    E_OVER_CURR_LC, /*"C相过流"*/
    E_CUT_CURR_LA, /*"A相断流"*/
    E_CUT_CURR_LB, /*"B相断流"*/
    E_CUT_CURR_LC, /*"C相断流"*/
    E_OVER_VOLT_LA, /*"A相过压"*/
    E_OVER_VOLT_LB, /*"B相过压"*/
    E_OVER_VOLT_LC, /*"C相过压"*/
    
    E_UNDER_VOLT_LA, /*"A相欠压"*/
    E_UNDER_VOLT_LB, /*"B相欠压"*/
    E_UNDER_VOLT_LC, /*"C相欠压"*/
    E_PWR_REVERSE, /*"潮流反向"*/
    E_PWR_REVERSE_LA,/*"A相功率反向"*/
    E_PWR_REVERSE_LB,/*"B相功率反向"*/
    E_PWR_REVERSE_LC,/*"C相功率反向"*/
    E_OVER_LOAD_LA, /*"A相过载"*/
    
    E_OVER_LOAD_LB, /*"B相过载"*/
    E_OVER_LOAD_LC, /*"C相过载"*/
    E_OVER_DEMAND,/*"需量超限"*/
    E_OVER_DEMAND_PA,/*"正向有功需量超限"*/
    E_OVER_DEMAND_NA,/*"反向有功需量超限"*/
    E_OVER_DEMAND_I, /*"I象限无功需量超限"*/
    E_OVER_DEMAND_II,/*"II象限无功需量超限"*/
    E_OVER_DEMAND_III,/*"III象限无功需量超限"*/
    
    E_OVER_DEMAND_IV, /*"IV象限无功需量超限"*/
    E_VOLT_IMBAL, /*"电压不平衡"*/
    E_CURR_IMBAL, /*"电流不平衡"*/
    E_CURR_BADLY_IMBAL, /*"电流严重不平衡"*/
    E_OVER_FACTOR, /*"总功率因数超下限"*/
    E_OVER_FACTOR_LA, /*"A相功率因数超下限"*/
    E_OVER_FACTOR_LB, /*"B相功率因数超下限"*/
    E_OVER_FACTOR_LC, /*"C相功率因数超下限"*/
    
    E_VOLT_REVERSE, /*"电压逆相序"*/
    E_CURR_REVERSE, /*"电流逆相序"*/
    E_PERMISS_LIMIT,/*限制权限*/
    E_MGMT_COMM_ERR, /*" 双芯通信故障"*/
    
    MAX_MTR_STA,
    E_STA_NO_DEF=0xFF/*未定义*/
}EN_MTR_STATUS;

#define  MTR_STA_BYTE    ((MAX_MTR_STA >> 3) + 1)
#define     DC_BUF_SIZE  64

#define MSADDR_IN_RAM(MSAddr,Result) {if(MSAddr>RAM){Result=TRUE;}else{Result=FALSE;}}

/*数据中心配置的地址转文件系统的参数*/
#define MSADDR_TO_FILEINFO(Addr,FileName,Offset) {FileName = (ENUM_FS_ID)(Addr>>16);Offset=(INT16U)Addr;}

/*数据中心配置地址转RAM地址*/
#define MSADDR_TO_RAMADDR(MSAddr,RamAddr)  {RamAddr = MSAddr - RAM;}

extern INT8U DCDataBuf[DC_BUF_SIZE];/*"数据中心记录文件数据缓冲区，用于事件记录"*/
extern INT8U oprateCode[4];
#ifdef THREE_PHASE
/*"三相表电表清零宏定义"*/
#define CLR_MTR()    {\
                     SaveRcrd(E_MTR_CLR_EVNT_RCRD, E_EVTLST_CLRMTR);\
                     Action(E_LRM_CLR, NULL, NULL);\
                     DCDataBuf[0] = 0xFF; \
                     DCDataBuf[1] = 0xFF;\
                     Action(E_MNT_EVNT_CLR, &DCDataBuf[0], NULL);\
                     Action(E_RCRD_CLR, &DCDataBuf[0], NULL);\
                     Action(E_UIDATA_CLR, NULL, NULL);\
                     Action(E_MAX_DEMAND_CLR, NULL, NULL);\
                     }
#else
/*"单相表电表清零宏定义"*/
#define CLR_MTR()    {\
                     SaveRcrd(E_MTR_CLR_EVNT_RCRD, E_EVTLST_CLRMTR);\
                     Action(E_LRM_CLR, NULL, NULL);\
                     DCDataBuf[0] = 0xFF; \
                     DCDataBuf[1] = 0xFF;\
                     Action(E_MNT_EVNT_CLR, &DCDataBuf[0], NULL);\
                     Action(E_RCRD_CLR, &DCDataBuf[0], NULL);\
                     Action(E_UIDATA_CLR, NULL, NULL);\
                     }
#endif


/*"电表数据初始化宏定义"*/
#define  MTR_INIT() {\
                     if(TRUE != FS_Init(FIRST_PWR_ON)){\
                        MCU_Reset();\
                     };\
                     LRM_Init(FIRST_PWR_ON);\
                     MNT_Init(FIRST_PWR_ON);\
                     UI_Init(FIRST_PWR_ON);\
                     RCRD_Init(FIRST_PWR_ON);\
        			 Action(E_FIRST_PWR_UP_OVER, NULL, NULL);\
        			 Action(E_RESET_FAC_REMAIN_TIME, NULL, NULL);\
        			 CLR_MTR()\
        			 LCD_Close();\
        			 Ans_MtrClr();\
                     MCU_Reset();\
                    }
#define  FRMT_NUM  (sizeof(stFormatList)/sizeof(ST_FMT_ATTR))
extern INT16S GetSingle(INT16U id, INT8U *pBuff);
extern INT16S GetList (INT16U *pIdList, INT8U IdNum, INT8U *pBuff, INT16U BuffSize);
extern INT8S Set (INT16U *pIdList, INT8U IdNum, INT8U *pData, INT8U *pLenList);
extern INT8S Action (INT16U id, void *pdata, void *pbuff);
extern INT16S GetRcrd (INT16U id, void *pdin, void *pdout, INT16U len);
extern INT8S SaveRcrd(INT16U file, INT16U ListObis);
extern INT8U GetFrmt(INT16U id, ST_FMT_ATTR *pformat);
extern void Push(EN_MTR_STATUS StaId, INT8U Status);
extern INT8U ChkStatus(EN_MTR_STATUS StaId);

extern INT8U FRMT_sizeofFormat(INT8U format);
extern INT8U FRMT_DataChange (INT8U *srcData, INT8U *destData, ST_FMT_ATTR inFormat, ST_FMT_ATTR outFormat);
extern INT16U FRMT_NdataChange(INT8U *srcData, INT8U *destData, ST_FMT_ATTR inFormat, ST_FMT_ATTR outFormat,INT16U Len);
#endif
