/*"************************************************************"*/
/*"FileName: DM.c"*/
/*"Author: 尹凌  Version : 1.0       Date: 2017-11-05"*/
/*" Description: 显示按键模块"*/
/*" Function List: "*/
/*" History:"*/
/*" <author> <time> <version > <desc>"*/
/*"************************************************************"*/


#include "app\UserInterface\UI.h"
#include "System\Task\task.h"
#include "Driver\HAL\FS\FS_EepConfig.h"
extern ST_7TIME_FORM stSysTime;

#define ERR_CODE_LEN    (2)

typedef struct /*"错误显示代码表"*/
{
    EN_MTR_STATUS staIndex;
    INT8U errCode[ERR_CODE_LEN];
}ST_SHOW_ERR_CODE;

typedef struct
{
    INT8U keyId;/*"功能键值ID"*/
    On_Key_Function keyFunc;/*"功能响应函数指针"*/
}ST_KEY_FUNCTION;

typedef struct
{
    ST_DISP_OBJ dispObj;
    INT16U dataObj;
}ST_SPEC_DISP_OBJ;


#ifdef THREE_PHASE
INT8U mPdKeyWakeUpFlg;
INT8U mPdIrComFlg; /*"低功耗红外通信完成标志"*/
#endif
INT8U mShowErrFlg;

//ST_DISP_PARA stDispPara;/*"显示配置参数"*/ 
static ST_DM_RUN_DATA mstRunData;/*"显示运行数据"*/ 
static ST_SHOW_DATA mstShowData;/*"显示数据，传给虚拟液晶的数据"*/ 
static ST_SHOW_ERR mstShowErr;/*"显示运行错误信息"*/
static INT32U mShowVersion;/*"显示版本"*/
static INT8U mPdUpateFlg;/*"掉电模式刷新标志"*/
#if 0
static ST_DM_PD_DATA mPdShowList[PD_DISP_NUM];/*"低功耗显示数据缓存区"*/
#endif
static ST_PD_OBJ_LIST mPdDispObjList;/*"低功耗显示对象列表"*/
#ifdef SINGLE_PHASE
static INT8U mOverdraftFlg;
#endif


const INT8U mErrStr[MAX_SHOWDATA] = 
{
    DIS_Space, DIS_E, DIS_r, DIS_r, DIS_Minus, DIS_Space, DIS_Space, DIS_Space

};

const ST_SHOW_ERR_CODE  mShowErrTab[] = 
{
    {E_CLK_BAT_LOSS, {0,4}},/*"电池电压低"*/
    {E_PWR_REVERSE, {5,6}},/*"有功功率反向"*/       
    {E_RELAY_ERR, {0,1}},/*"控制回路错误"*/   
    {E_ESAM_ERR,{0,2}},/*"ESAM故障"*/ 
    {E_FIRST_PWR_ON,{0,3}},/*"电能表初始化故障"*/ 
    {E_EEP_ERR,{0,6}},/*"EEPROM故障"*/ 
    {E_CLK_ERR,{0,8}},/*"时钟故障"*/ 
    {E_MU_IC_ERR,{0,7}},/*"计量芯片故障"*/  
    //{    {8,1}},/*"表型模式字参数和硬件检测不匹配"*/
    //{    {8,2}},/*"继电器类型参数和硬件检测不匹配"*/
    {E_OSCILLATOR_ERR,{8,3}},/*"RTC未校准(晶振未起震)"*/        

};

const ST_DISP_LIST_INFO mstDispListInfo[MAX_LIST_TYPE] = 
{
    {CYC_OBJ_LIST_SHIFT, CYC_DISP_MAX}, 
    {KEY_OBJ_LIST_SHIFT, KEY_DISP_MAX}
};

typedef enum
{
    DISP_SOFT_VER = 0,
    DISP_ALL, 
    DISP_NONE,

    MAX_SPEC_DISP
}EN_SPEC_DISP;



#if defined(DM_DLT698)/*"698协议显示对象相关定义"*/

#define OBJ_IS_OAD      (0)
#define OBJ_IS_ROAD    (1)

const ST_DISP_OBJ mstSpecDisp[MAX_SPEC_DISP] = 
{
    {{0xffff,0x02,0x03},{0x0000,0x00,0x00},0}, //DISP_SOFT_VER  /*"软件版本号"*/
    {{0xffff,0x02,0x01},{0x0000,0x00,0x00},0}, // DISP_ALL           /*"全屏"*/
    {{0xffff,0x02,0x02},{0x0000,0x00,0x00},0} // DISP_NONE         /*"黑屏"*/
};

const ST_DISP_OBJ mstPDInitDisp = /*"掉电常显项"*/
{
    {0x0000,0x02,0x01},{0x0000,0x00,0x00},0 /*"当前总电量"*/
};

const ST_SPEC_DISP_OBJ mstSpecObj[] = 
{
    {{{0x202C,0x02,0x01},{0x0000,0x00,0x00},0}, E_REMAIN_MONEY},/*"当前剩余金额"*/
    {{{0x202D,0x02,0x00},{0x0000,0x00,0x00},0}, E_OVERDRAFT_MONEY},/*"当前透支金额"*/
    {{{0x3028,0x02,0x01},{0x202C,0x22,0x01},0}, E_REMAIN_BEFORE},/*"购电前剩余金额"*/
};

const ST_DISP_OBJ mstDefaultObj = {{0x0000,0x02,0x01},{0x0000,0x00,0x00},0};


#elif defined(DM_DLT645)/*"645协议显示对象相关定义"*/
const ST_DISP_OBJ mstSpecDisp[MAX_SPEC_DISP] = 
{
    {0x0e200020,0}, //DISP_SOFT_VER  /*"软件版本号"*/
    {0xffffffff,0}, // DISP_ALL           /*"全屏"*/
    {0xffffff01,0} // DISP_NONE         /*"黑屏"*/
};

const ST_DISP_OBJ mstPDInitDisp = /*"掉电常显项"*/
{
    0x00000000,0 /*"当前总电量"*/
};

#ifdef PM_MODLE
const ST_SPEC_DISP_OBJ mstSpecObj[] = 
{
    {{0x02010100,0}, E_REMAIN_MONEY},/*"当前剩余金额"*/
    {{0x02010100,0}, E_OVERDRAFT_MONEY},/*"当前透支金额"*/
};
#endif

const ST_DISP_OBJ mstDefaultObj = 
{
                    0x00000000,0/*"当前总"*/ 
};
#else
#endif


const ST_DISP_MODE_INFO mstDispModeInfo[MAX_SHOW_MODE] =
{
    {&stDispPara.cycleShowNum, (INT32U)CYC_OBJ_LIST_SHIFT},
    {&stDispPara.keyShowNum, (INT32U)KEY_OBJ_LIST_SHIFT},
    {&stDispPara.cycleShowNum, (INT32U)CYC_OBJ_LIST_SHIFT},
    {&stDispPara.keyShowNum, (INT32U)KEY_OBJ_LIST_SHIFT},
    {&mstRunData.initShowNum, RAM + (INT32U)&mstSpecDisp},
    {NULL, 0},
#ifdef PD_DISP_SLEEP
    {NULL, 0},
#else
    {&mstRunData.initShowNum, RAM + (INT32U)&mstPDInitDisp},/*"掉电常显用"*/
#endif
};

const ST_PD_DISP_INFO mstPdDispInfo[] = /*"掉电显示对象列表缓存信息"*/
{
    {PD_CYC_SHOW_MODE, PD_DISP_CYC_NUM, &mstRunData.pdCycNum, &mPdDispObjList.cyc[0]},
    {PD_KEY_SHOW_MODE, PD_DISP_KEY_NUM, &mstRunData.pdKeyNum, &mPdDispObjList.key[0]},

};

#ifdef THREE_PHASE
#define KEY_FUNC_BIT  (0x40)/*"该位为1时表示下翻键递增屏数，为0时递减"*/
#endif
#define COMB_SHOW_BIT (0x20)/*"该位为1时组合有功电量需显示组合字符"*/

static void DM_SetMode(INT8U mode);
static void DM_KeyProcess(INT8U keyId);
static void DM_TimerDec(void);
static INT8U DM_GetShowData(void);
#ifdef DIS_USER_CODE
static void DM_GetUserCode(void);
#endif

static void DM_OtherInfoDeal(ST_DISP_OBJ *pDispObj);
static void DM_GetShowErr(void);
static void DM_GetNextScr(void);
static void DM_GetLastScr(void);
static void DM_GetScr(void);
#ifdef SINGLE_PHASE
static INT8U DispMnyChk(void);
#endif
static void ShowNumBCD(ST_SHOW_DATA *pShow);
#ifdef DIS_REC_TIME
void SubMouth(INT8U MonthNum,INT8U *outTime);
#endif
#if 0
static void ShowBCD(ST_SHOWDATA_OUTPUT *pShow);
#endif
static INT8U GetHalf(INT8U inIndex, INT8U *inpCh);
static void InitSetLed(void);
static void InitClrLed(void);
static void SetBacklight(INT16U time);
#if 0
static void DM_DispParaChk(void);
#endif
static INT8S ShowSpecScr(ST_DISP_OBJ *pDispObj, INT16U time, EN_SHOW_PRIORITY ShowPrio);
static void GetShowErrIndex(void);
#ifdef THREE_PHASE
static void showQuad(INT8U  comMode);
static void SetKeyFunc(INT8U keyFlag);
#endif

#ifdef DM_DLT698
static INT8U ChkPhaseEvtRecord(ST_OAD *pEvtOad);
#endif

static void OnKeyDisp_CycMode(void);
static void OnKeyDisp_KeyMode(void);
static void OnKeyDisp_InitMode(void);
static void OnKeyDisp_PDCycMode(void);
static void OnKeyDisp_PDKeyMode(void);
static void OnKeyDisp_PDInitMode(void);
static void OnKeyRlyCls(void);
static void OnKeyInvalid_CycMode(void);
static void OnKeyInvalid_KeyMode(void);
static void OnKeyInvalid_InitMode(void);
static void OnKeyInvalid_PDCycMode(void);
static void OnKeyInvalid_PDKeyMode(void);
static void OnKeyInvalid_SpecMode(void);
static void OnKeyDisp_SpecMode(void);
#ifdef THREE_PHASE
static void OnKeyDisp2_KeyMode(void);
static void OnKeyDisp2_PDKeyMode(void);
static void OnKeyFacCheck(void);
#endif

#define KEY_ACT_NUM  (MAX_KEY_ID + 1)

#ifdef THREE_PHASE
#define  DISP_KEY_DOWN   (0)
#define  DISP_KEY_UP        (1)
#endif


const ST_KEY_FUNCTION mstKeyFunc[MAX_SHOW_MODE][KEY_ACT_NUM] = 
{/*"按键动作列表"*/
#ifndef KEY_KEYPAD

    {/*"CYC_SHOW_MODE"*/
        {KEY_ID_INVALID, OnKeyInvalid_CycMode},/*"无按键"*/
        {KEY_ID_DISP1, OnKeyDisp_CycMode},/*"显示下翻键短按"*/
#ifdef THREE_PHASE
        {KEY_ID_DISP2, OnKeyDisp_CycMode},/*"显示上翻键短按"*/
#endif
        {KEY_ID_DISP1_RLY_CLS, OnKeyRlyCls},/*"显示下翻键长按3秒检测合闸"*/
#ifdef THREE_PHASE
        {KEY_ID_DISP2_RLY_CLS, OnKeyRlyCls},/*"显示上翻键长按3秒检测合闸"*/
        {KEY_ID_FAC_CHECK, OnKeyFacCheck},/*"两个显示键同时按下"*/
#endif
        {KEY_ID_SW_MULT_FUNC, NULL},/*"显示下翻键长按5秒切换多功能口输出"*/

    },

    {/*"KEY_SHOW_MODE"*/
        {KEY_ID_INVALID, OnKeyInvalid_KeyMode},
        {KEY_ID_DISP1, OnKeyDisp_KeyMode},
#ifdef THREE_PHASE
        {KEY_ID_DISP2, OnKeyDisp2_KeyMode},
#endif
        {KEY_ID_DISP1_RLY_CLS, OnKeyRlyCls},
#ifdef THREE_PHASE
        {KEY_ID_DISP2_RLY_CLS, OnKeyRlyCls},
        {KEY_ID_FAC_CHECK, OnKeyFacCheck},
#endif
        {KEY_ID_SW_MULT_FUNC, NULL},/*"显示下翻键长按5秒切换多功能口输出"*/

    },

    {/*"PD_CYC_SHOW_MODE"*/
        {KEY_ID_INVALID, OnKeyInvalid_PDCycMode},
        {KEY_ID_DISP1, OnKeyDisp_PDCycMode},
#ifdef THREE_PHASE
        {KEY_ID_DISP2, OnKeyDisp_PDCycMode},
#endif
        {KEY_ID_DISP1_RLY_CLS, NULL},
#ifdef THREE_PHASE
        {KEY_ID_DISP2_RLY_CLS, NULL},
        {KEY_ID_FAC_CHECK, NULL},
#endif
        {KEY_ID_SW_MULT_FUNC, NULL},

    },

    {/*"PD_KEY_SHOW_MODE"*/
        {KEY_ID_INVALID, OnKeyInvalid_PDKeyMode},
        {KEY_ID_DISP1, OnKeyDisp_PDKeyMode},
#ifdef THREE_PHASE
        {KEY_ID_DISP2, OnKeyDisp2_PDKeyMode},
#endif
        {KEY_ID_DISP1_RLY_CLS, NULL},
#ifdef THREE_PHASE
        {KEY_ID_DISP2_RLY_CLS, NULL},
        {KEY_ID_FAC_CHECK, NULL},
#endif
        {KEY_ID_SW_MULT_FUNC, NULL},

    },

    {/*"INIT_SHOW_MODE"*/
        {KEY_ID_INVALID, OnKeyInvalid_InitMode},
        {KEY_ID_DISP1, OnKeyDisp_InitMode},
#ifdef THREE_PHASE
        {KEY_ID_DISP2, OnKeyDisp_InitMode},
#endif
        {KEY_ID_DISP1_RLY_CLS, OnKeyRlyCls},
#ifdef THREE_PHASE
        {KEY_ID_DISP2_RLY_CLS, OnKeyRlyCls},
        {KEY_ID_FAC_CHECK, OnKeyFacCheck},
#endif
        {KEY_ID_SW_MULT_FUNC, NULL},/*"显示下翻键长按5秒切换多功能口输出"*/

    },

    {/*"SPEC_MODE"*/
        {KEY_ID_INVALID, OnKeyInvalid_SpecMode},
        {KEY_ID_DISP1, OnKeyDisp_SpecMode},
#ifdef THREE_PHASE
        {KEY_ID_DISP2, OnKeyDisp_SpecMode},
#endif
        {KEY_ID_DISP1_RLY_CLS, OnKeyRlyCls},
#ifdef THREE_PHASE
        {KEY_ID_DISP2_RLY_CLS, OnKeyRlyCls},
        {KEY_ID_FAC_CHECK, OnKeyFacCheck},
#endif
        {KEY_ID_SW_MULT_FUNC, NULL},/*"显示下翻键长按5秒切换多功能口输出"*/

    },

    {/*"PD_INIT_SHOW_MODE"*/
        {KEY_ID_INVALID, NULL},
        {KEY_ID_DISP1, OnKeyDisp_PDInitMode},
#ifdef THREE_PHASE
        {KEY_ID_DISP2, OnKeyDisp_PDInitMode},
#endif
        {KEY_ID_DISP1_RLY_CLS, NULL},
#ifdef THREE_PHASE
        {KEY_ID_DISP2_RLY_CLS, NULL},
        {KEY_ID_FAC_CHECK, NULL},
#endif
        {KEY_ID_SW_MULT_FUNC, NULL},

    },

#endif

};



/*"************************************************"*/
/*"Function:    DM_Init"*/
/*"Description: 显示初始化"*/
/*"Input: mode 上电or 掉电运行模式"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
void DM_Init(INT8U mode)
{
    INT8U i;
    LIB_MemSet(0, (INT8U *)&mstShowErr, sizeof(ST_SHOW_ERR));
    LIB_MemSet(0, (INT8U *)&mstRunData, sizeof(ST_DM_RUN_DATA));
    mstRunData.refreshTick = MCU_GetTick();
        
#ifdef THREE_PHASE
    mPdKeyWakeUpFlg = 0;
    mPdIrComFlg = 0;
#endif
    mShowErrFlg = 0;

    DIS_init();/*"初始化液晶"*/  

    if((MCU_RUNMODE == mode) || (FIRST_PWR_ON == mode))
    {
        mstRunData.runMode = RUN_MODE_INIT;
        mstRunData.showRet = TRUE;
        mstRunData.initShowNum = MAX_INIT_NUM;

        mShowVersion = 0;

        for(i = 0; i < 8; i++)
        {
            mShowVersion |= ((INT32U)(ShowVersion[i] - 0x30) << (7 - i) * 4);
        }

        InitSetLed();  

        DM_SetMode(INIT_SHOW_MODE);
   
        DM_GetShowData();

        DIS_PrintLcdLine1(&mstShowData, 0, LEVEL_1);/*"显示刷新"*/  

    }
    else
    {
        mstRunData.runMode = RUN_MODE_PD;

        LIB_MemSet(0x0, (INT8U *)&mPdDispObjList, sizeof(ST_PD_OBJ_LIST));/*"清低功耗显示数据缓存区"*/
#if 0
        LIB_MemSet(0xFF, (INT8U *)&mPdShowList[0], sizeof(mPdShowList));/*"清低功耗显示数据缓存区"*/
#endif
        mPdUpateFlg = FALSE;

#ifndef PD_DISP_SLEEP
        LCD_Init(MCU_LPMODE_DEEP);

        mstRunData.initShowNum = 1;/*"常显1屏"*/  
        mstRunData.pdShowTime = PD_SHOW_TIME;
#endif

        DM_SetMode(PD_INIT_SHOW_MODE);

    }
}

#if 0
static void DM_DispParaChk(void)
{
    CRC_DATA_TP  crc;
    ST_UI_PARA_WITH_BACKUP tmpPara;
    
    crc = CRC16((INT8U*)&StUIParaWithRamBackup, FIND(ST_UI_PARA_WITH_BACKUP, CrcValue));
    if(crc != StUIParaWithRamBackup.CrcValue)
    {
        FS_ReadFile(E_FILE_UI_PARA_WITH_BACKUP, 0, (INT8U *)&tmpPara, sizeof(ST_UI_PARA_WITH_BACKUP));

        crc = CRC16((INT8U*)&tmpPara, FIND(ST_UI_PARA_WITH_BACKUP, CrcValue));
        if(crc != tmpPara.CrcValue)
        {
            UI_LoadDefaultUiPara();
        }
        else
        {
            LIB_MemCpy((INT8U *)&tmpPara.StDmParaWithRamBackup.DispPara, (INT8U *)&stDispPara, sizeof(ST_DISP_PARA));
        }
    }
}
#endif

static void InitSetLed(void)
{
    INT16U time;

#ifdef DM_DLT698
    time = (INT16U)stDispPara.pwrUpFullShowT;
#else
    time = SHOW_ALL_TIME;
#endif

    SetBacklight(time);
    OC_LedOnSingle(E_ID_LED_RELAY, E_LED_ALWAYS_MODE, time);
}

static void InitClrLed(void)
{
    OC_LedOff(E_ID_LED_BGD, E_LED_SC_SNG);
    OC_LedOff(E_ID_LED_RELAY, E_LED_SC_SNG);
}

/*"*****************************************************************************"*/
/*"  Function:       SetBacklight"*/
/*"  Description:    控制背光点亮接口"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          time 背光点亮时间"*/
/*"  Output          "*/
/*"  Return:         无"*/
/*"  Others:         code size  byte"*/
/*"*****************************************************************************"*/
static void SetBacklight(INT16U time)
{
#if 0
    if((TRUE != ChkStatus(E_VOLT_LCD_OK)) && (TRUE != ChkStatus(E_FAC)))
    {
        return;
    }
#endif
    OC_LedOnSingle(E_ID_LED_BGD, E_LED_ALWAYS_MODE, time);
}

/*"************************************************"*/
/*"Function:    DM_ShowSpec"*/
/*"Description: 指定显示对象显示 ( 目前仅预付费模块调用)"*/
/*"Input: pdin  特殊屏参数"*/
/*"Output: pdout  无"*/
/*"Return: DAR"*/
/*"Others: "*/
/*"************************************************"*/
INT8S DM_ShowSpec(void *pdin, void *pdout)
{
    
    if(NULL == pdin)
    {
        return MS_OTHER_ERR;
    }
#ifdef PM_MODLE
    pSpec = (ST_SHOW_SPEC *)pdin;
    for(i = 0; i < ARRAY_SIZE(mstSpecObj); i++)
    {
        if(mstSpecObj[i].dataObj == pSpec->obj)
        {
            ShowSpecScr((ST_DISP_OBJ *)&mstSpecObj[i].dispObj, pSpec->time, LEVEL_3/*pSpec->ShowPrio*/);
            return MS_SUCCESS;
        }
    }
#endif
    return MS_OBIS_ERR;
}

/*"************************************************"*/
/*"Function:    DM_DLT698ShowSpec"*/
/*"Description: 698协议显示查看"*/
/*"Input: pdin  显示查看参数"*/
/*"Output: pdout  无"*/
/*"Return: DAR"*/
/*"Others: "*/
/*"************************************************"*/
INT8S DM_DLT698ShowSpec(void *pdin, void *pdout)
{
#ifdef DM_DLT698
    INT16U len, time;
    INT8U *pData;
    ST_DISP_OBJ showData;
    INT32U paraAddr;
    INT8S result;

    if(NULL == pdin)
    {
        return MS_OTHER_ERR;
    }

    pData = (INT8U *)pdin + sizeof(INT16U);// ST_ACTION_DATa
    LIB_MemCpy(pData, (INT8U *)&len, sizeof(INT16U));
    if(len > sizeof(INT16U))
    {
        len -= sizeof(INT16U);    
        if((len == sizeof(ST_DISP_OBJ1)) || (len == sizeof(ST_DISP_OBJ2)))
        {
            pData += sizeof(INT16U);
            LIB_MemCpy(pData, (INT8U *)&paraAddr, sizeof(INT32U));/*"取参数地址"*/
            pData = (INT8U *)paraAddr;

            time = ((INT16U)pData[len + 1] << 8) + (INT16U)pData[len];
#if 0
            if((0 == time) || (time > 60))
            {/*"液晶查看时间1-60s，超出时默认5s"*/
                time = 5;
            }
#endif
            if(0 != time)
            {
                LIB_MemSet(0, (INT8U *)&showData, sizeof(ST_DISP_OBJ));
                if(len == sizeof(ST_DISP_OBJ1))
                {
                    LIB_MemCpy((pData + 1), (INT8U *)&showData.Oad, sizeof(ST_OAD));
                    showData.scrNo = *(pData + 5);
                }
                else
                {
                    LIB_MemCpy((pData + 1), (INT8U *)&showData.Oad, sizeof(ST_OAD));
                    LIB_MemCpy((pData + 6), (INT8U *)&showData.releOad, sizeof(ST_OAD));
                    showData.scrNo = *(pData + 10);
                }

                result = ShowSpecScr(&showData, time, LEVEL_1);/*"数据需要在主循环刷新，优先级必须为1"*/
            }
            else
            {
                result = MS_OTHER_ERR;
            }

            return result;
        }
    }
#endif
    return MS_LEN_ERR;
}

/*"************************************************"*/
/*"Function:    ShowSpecScr"*/
/*"Description: 指定显示对象显示 ( 液晶查看or 指定对象)"*/
/*"Input: pDispObj 显示对象；time 显示时长；ShowPrio 优先级"*/
/*"Output: 无"*/
/*"Return: DAR"*/
/*"Others: "*/
/*"************************************************"*/
static INT8S ShowSpecScr(ST_DISP_OBJ *pDispObj, INT16U time, EN_SHOW_PRIORITY ShowPrio)
{
    INT16U ledTime;

    LIB_MemCpy((INT8U *)pDispObj, (INT8U *)&mstRunData.curScrCode, sizeof(ST_DISP_OBJ));

    mstRunData.showRet = DM_GetShowData();
    if(TRUE != mstRunData.showRet)
    {
        return MS_OTHER_ERR;
    }

#ifdef PM_MODLE
    if(EQUAL == LIB_MemCmp((INT8U *)&mstSpecObj[2].dispObj, (INT8U *)pDispObj, sizeof(ST_DISP_OBJ)))
    {/*"购电前剩余金额去掉'上1'显示"*/
        mstShowData.Protc.stShowTable.schar[0] &= 0xDF;
        mstShowData.charAdd[0] &= 0xF0;
    }
#endif    

    mstRunData.scrTimer = time;

#ifdef DIS_USER_CODE
    if(0 != mstRunData.showUserCodeFlg)
    {
        DM_GetUserCode();
        DIS_PrintLcdLine2(&mstRunData.userCode);/*"显示用户代码"*/  
    }
#endif

    DIS_PrintLcdLine1(&mstShowData, time, ShowPrio);/*"显示刷新"*/  

    if(ShowPrio == LEVEL_1)/*"液晶查看"*/
    {
#if defined(DM_DLT698)
        ledTime = stDispPara.DisChecklightTime;
#elif defined(DM_DLT645)
        ledTime = DISP_CHECK_TIME;
#else
#endif

        DM_SetMode(SPEC_MODE);
        SetBacklight(ledTime);

        if(EQUAL == LIB_MemCmp((INT8U *)&mstSpecDisp[DISP_ALL], (INT8U *)pDispObj, sizeof(ST_DISP_OBJ)))/*"全屏时点亮RELAY LED"*/
        {
            OC_LedOnSingle(E_ID_LED_RELAY, E_LED_ALWAYS_MODE, ledTime);
        }
    }

    return MS_SUCCESS;
}

extern INT8U chip_commu_ok(void);
extern void show_mb_data(void);
extern void main_key_up(void);
/*"************************************************"*/
/*"Function:    DM_Task"*/
/*"Description: 上电显示任务"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
ST_SHOW_DATA DisDataLib=
{
       {{1,2,3,4,5,6,7,8},{{FMT_STRING,E_KWH_UNIT,2}, {0x11,0x00,0xBC,0x00,0x00}}},
    	{0,0,0,0,0}	
};

INT8U mgmt_ok;

void SetShowData(INT8U *buf)
{
    //LIB_MemCpy(buf, (INT8U *)&DisDataLib, sizeof(DisDataLib));
    DIS_PrintLcdLine3(buf, 10, LEVEL_3);
    mgmt_ok = 0xff;
}


void DM_Task(void)
{
    INT8U keyId, refFlg;

    refFlg = FALSE;
	if((ChkStatus(E_MGMT_COMM_ERR) == FALSE) && (mgmt_ok == 0xff)) /*"管理芯通讯正常，使用管理芯数据"*/
	{
		//DM_freshDisData((ST_SHOW_DATA *)&DisDataLib);
	}
	else
	{
		DIS_ClrPriority();
		if(TRUE == Task_ChkTsk_1s(MAINTASK_DM))/*"秒任务"*/
		{
			DM_GetShowErr(); /*"获取错误显示"*/
			
			DM_TimerDec();/*"显示时间递减"*/ 
	
			DIS_Main();   
		}
	
#if 0/*"放入UI模块分钟任务处理"*/
    if(TRUE == Task_ChkTsk_1min(MAINTASK_DM))/*"分钟任务"*/
    {
        DM_DispParaChk();
    }
#endif

    if(MCU_CmpTick(mstRunData.refreshTick, SHOW_INTERVAL))/*"刷新时间到"*/
    {
        mstRunData.refreshTick = MCU_GetTick();
        refFlg = TRUE;

        //DIS_Main();   /*"虚拟液晶闪烁频率同屏刷新频率时需增加接口"*/
    }

    keyId = KEY_GetKey();/*"获取功能键值ID"*/

    DM_KeyProcess(keyId);

    if((TRUE == refFlg) || (FALSE == mstRunData.showRet))/*"显示取数"*/
    {
        mstRunData.showRet = DM_GetShowData();

        if(TRUE == mstRunData.showRet)
        {
#ifdef DIS_USER_CODE
            if(0 != mstRunData.showUserCodeFlg)
            {
                DM_GetUserCode();
                DIS_PrintLcdLine2(&mstRunData.userCode);/*"显示用户代码"*/  
            }
#endif

            DIS_PrintLcdLine1(&mstShowData, 0, LEVEL_1);/*"显示刷新"*/  
        }
    }
	}
}

/*"************************************************"*/
/*"Function:    DM_GetNextScr"*/
/*"Description: 取得下一屏显示代码"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
static void DM_GetNextScr(void)
{
    INT8U *pNum;

    if((KEY_SHOW_MODE == mstRunData.showMode) && (0 == mstShowErr.index))
    {
        mstShowErr.index= 0xFF;
    }

    if(0x55 == mShowErrFlg)/*"故障代码可插入显示"*/
    {
        mShowErrFlg = 0;
        if(0xFF == mstShowErr.index)
        {
            mstShowErr.index = 0;
        }
    }

    if((mstShowErr.runErr == 0) || (mstShowErr.index > ARRAY_SIZE(mShowErrTab)))
    {/*"没有运行错误，或者已经轮显完毕错误代码"*/
        pNum = mstDispModeInfo[mstRunData.showMode].pDispNum;
        if(NULL != pNum)
        {
           // mstRunData.curNum = *pNum;
            mstRunData.curNum = 5;
            mstRunData.index ++;/*"显示项目索引值增加"*/
            if(mstRunData.index >= mstRunData.curNum)/*"显示项目已轮显完毕"*/
            {
                mstRunData.index  = 0;/*"显示索引值清0"*/
                mstShowErr.index = 0;/*"错误显示索引值清0"*/
                mstRunData.validFlg = FALSE;

                if(INIT_SHOW_MODE == mstRunData.showMode)
                {
                    DM_SetMode(CYC_SHOW_MODE);
                }
            }
        }
    }
    else
    {
    }

    DM_GetScr();
}

static void GetShowErrIndex(void)
{
    INT8S i;

#ifdef THREE_PHASE
    if(DISP_KEY_UP == mstRunData.keyFlg)
    {
        if((0 != mstShowErr.index) && (0xFF != mstShowErr.index))
        {/*"有运行错误，并且当前正在轮显错误代码"*/
            if(mstShowErr.index > 1)
            {
                for(i = mstShowErr.index - 2; i >= 0; i--)/*"找到下一屏需显示的错误代码"*/
                {                        
                    mstShowErr.index--;
                    if(mstShowErr.runErr & (1 << i))
                    {
                        break;
                    } 
                    else
                    {
                        if(1 == mstShowErr.index)
                        {
                            mstShowErr.index = 0;
                        }
                    }
                }
            }
            else
            {
                mstShowErr.index--;
            }

            if(0 == mstShowErr.index)/*"错误代码已经显示完毕"*/
            {
                mstRunData.index = mstRunData.curNum - 1;/*"从第一屏开始显示"*/
            }
        }
    }
    else
#endif
    {
        if(mstShowErr.index <= ARRAY_SIZE(mShowErrTab))
        {/*"有运行错误，并且当前正在轮显错误代码"*/
            for(i = mstShowErr.index; i <= ARRAY_SIZE(mShowErrTab); i++)/*"找到下一屏需显示的错误代码"*/
            {
                mstShowErr.index++;     /*"错误代码显示索引值增加"*/       
                if(mstShowErr.runErr & (1 << i))
                {
                    break;
                } 
            }
            if(mstShowErr.index > ARRAY_SIZE(mShowErrTab))/*"错误代码已经显示完毕"*/
            {
                mstRunData.index = 0;/*"从第一屏开始显示"*/
            }
        }
    }
}

/*"************************************************"*/
/*"Function:    DM_GetScr"*/
/*"Description: 取得显示对象代码"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
static void DM_GetScr(void)
{
    INT8U i, readEEFlg;
    INT16U offset;
    INT8U *pRam, *pNum;
    INT32U dataAddr;

    if(0 != mstShowErr.runErr)
    {
        GetShowErrIndex();
    }

    switch(mstRunData.showMode)
    {
    case CYC_SHOW_MODE:
    case PD_CYC_SHOW_MODE:
        mstRunData.scrTimer = stDispPara.cycleShowTime;
        mstRunData.showRet = FALSE;/*"准备取显示数据"*/
        break;
    case KEY_SHOW_MODE:
        mstRunData.scrTimer = stDispPara.keyRetTime;
        mstRunData.showRet = FALSE;
        break;
    case PD_KEY_SHOW_MODE:
        mstRunData.scrTimer = stDispPara.LPKeyDisTime;
        mstRunData.showRet = FALSE;
        break;
    case INIT_SHOW_MODE:
        if(DISP_VER_ID == mstRunData.index)/*"显示版本号"*/
        {
            if(TRUE == ChkStatus(E_FAC))/*"厂内状态显示版本号"*/
            {
                mstRunData.scrTimer = SHOW_VER_TIME;
            }
            else
            {
                mstRunData.index ++;
            }
        }

        if(DISP_ALL_ID == mstRunData.index)/*"全屏显示"*/
        {
#ifdef DM_DLT698
            mstRunData.scrTimer = (INT16U)stDispPara.pwrUpFullShowT;
#else
            mstRunData.scrTimer = SHOW_ALL_TIME;
#endif
        }
        break;
    case PD_INIT_SHOW_MODE:
        LIB_ConstMemCpy((INT8U *)&mstPDInitDisp, (INT8U *)&mstRunData.curScrCode, sizeof(ST_DISP_OBJ));
        break;

    default:
        break;
    }

    pNum = mstDispModeInfo[mstRunData.showMode].pDispNum;
    dataAddr = mstDispModeInfo[mstRunData.showMode].dispListAddr;
    if(NULL != pNum)
    {
        /*"提取下一屏显示对象"*/
        if(dataAddr >= RAM)/*"RAM address"*/
        {
            offset = (INT16U)mstRunData.index * sizeof(ST_DISP_OBJ);
            pRam = (INT8U *)((dataAddr - RAM) + offset);

            LIB_MemCpy(pRam, (INT8U *)&mstRunData.curScrCode, sizeof(ST_DISP_OBJ));
        }
        else/*"EEPROM address"*/
        {
            readEEFlg = 0x55;

            if(RUN_MODE_PD == mstRunData.runMode)
            {
                for(i = 0; i < ARRAY_SIZE(mstPdDispInfo); i++)
                {
                    if(mstPdDispInfo[i].showMode != mstRunData.showMode)
                    {
                        continue;
                    }

                    if(mstRunData.index < mstPdDispInfo[i].maxObjNum)
                    {
                        pRam = (INT8U *)(mstPdDispInfo[i].pDispObj + mstRunData.index);
                        if(EQUAL != LIB_ChkValue(pRam, sizeof(ST_DISP_OBJ), 0x0))
                        {
                            LIB_MemCpy(pRam, (INT8U *)&mstRunData.curScrCode, sizeof(ST_DISP_OBJ));
                            readEEFlg = 0;
                        }
                    }
                    break;
                }
            }

            if(0x55 == readEEFlg)
            {
                offset = (INT16U)mstRunData.index * sizeof(ST_DISP_OBJ);
                FS_ReadFile(E_FILE_UI_PARA_NO_BACKUP, ((INT16U)dataAddr + offset), (INT8U *)&mstRunData.curScrCode, sizeof(ST_DISP_OBJ));

                if(RUN_MODE_PD == mstRunData.runMode)
                {
                    for(i = 0; i < ARRAY_SIZE(mstPdDispInfo); i++)
                    {
                        if((mstPdDispInfo[i].showMode == mstRunData.showMode) &&
                        (*mstPdDispInfo[i].pCurrNum < mstPdDispInfo[i].maxObjNum))
                        {
                            LIB_MemCpy((INT8U *)&mstRunData.curScrCode, (INT8U *)(mstPdDispInfo[i].pDispObj + mstRunData.index), sizeof(ST_DISP_OBJ));
                            (*mstPdDispInfo[i].pCurrNum)++;
                            break;
                        }
                    }
                }
            }
        }

        mPdUpateFlg = TRUE;
    }
}    

/*"************************************************"*/
/*"Function:    DM_SetMode"*/
/*"Description: 显示模式切换控制"*/
/*"Input: mode  将更新的显示模式"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
static void DM_SetMode(INT8U mode)
{
    switch(mode)
    {
        case INIT_SHOW_MODE:
            break;
        case PD_INIT_SHOW_MODE:/*"掉电初始化显示"*/
#ifdef PD_DISP_SLEEP
            LCD_Close();/*"掉电关闭液晶"*/
#endif
            break;
        case CYC_SHOW_MODE:/*"循显"*/      
        case KEY_SHOW_MODE:/*"按键"*/  
        case SPEC_MODE:
            if(RUN_MODE_INIT == mstRunData.runMode)
            {
                InitClrLed();
                mstRunData.runMode = RUN_MODE_NORMAL;
            }
        case PD_CYC_SHOW_MODE:/*"低功耗循显"*/      
        case PD_KEY_SHOW_MODE:/*"低功耗按键"*/  
        default:
            break;
    }

    mstRunData.showMode = mode;
    mstRunData.index = 0;
    mstRunData.validFlg = FALSE;
    mstShowErr.index = 0;

    if(SPEC_MODE != mode)
    {
        DM_GetScr();
    }

#ifndef PD_DISP_SLEEP
    if(PD_INIT_SHOW_MODE == mode)
    {
        DM_GetShowData();

#ifdef DIS_USER_CODE
        DM_GetUserCode();
        DIS_PrintLcdLine2(&mstRunData.userCode);/*"显示用户代码"*/  
#endif

        DIS_PrintLcdLine1(&mstShowData, 0, LEVEL_1);/*"显示刷新"*/  
    }
#endif

}

/*"************************************************"*/
/*"Function:    DM_GetShowData"*/
/*"Description: 根据显示对象代码取得显示数据"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/

#if defined(DM_DLT698)
const ST_DISP_OBJ mstDispMny[] = 
{
    {{0x202C, 2, 1}, {0x0000, 0, 0}, 0},/*"剩余电量"*/
    {{0x202D, 2, 1}, {0x0000, 0, 0}, 0},/*"透支电量"*/
};
#elif defined(DM_DLT645)
const ST_DISP_OBJ mstDispMny[] = 
{
    {0x00900200, 0},/*"剩余电量"*/
    {0x00900201, 0},/*"透支电量"*/
};
#else
#endif

static INT8U DM_GetShowData(void)
{
    INT8U len, ret;
    INT8U format, *pShowFmt;
    INT8U index;

    ret = TRUE;   
    LIB_MemSet(0x00, (INT8U *)&mstShowData, sizeof(ST_SHOW_DATA));
    format = FMT_STRING;/*"显示格式默认为 字符串"*/  
#ifdef DIS_USER_CODE
    mstRunData.showUserCodeFlg = 0;
#endif

    if(EQUAL == LIB_MemCmp((INT8U *)&mstSpecDisp[DISP_ALL], (INT8U *)&mstRunData.curScrCode, sizeof(ST_DISP_OBJ)))
    {/*"全屏"*/  
        mstShowData.Protc.showDataBuff[7] = _SHOW_ALL_;         
    }
    else if(EQUAL == LIB_MemCmp((INT8U *)&mstSpecDisp[DISP_SOFT_VER], (INT8U *)&mstRunData.curScrCode, sizeof(ST_DISP_OBJ)))
    {/*"显示版本"*/  
        LIB_MemCpy((INT8U *)&mShowVersion, &mstShowData.Protc.showDataBuff[0], sizeof(INT32U));        
        mstShowData.Protc.stShowTable.stShowFormat.format = E_ASCII_8_LENGTH;
        ShowNumBCD(&mstShowData);  
        //ShowBCD(&mstShowData.Protc);  
    }
    /*"有运行错误 &&  当前处于错误轮显 &&（循显或者按键显示）"*/     
    else if((mstShowErr.runErr != 0) && (mstShowErr.index <= ARRAY_SIZE(mShowErrTab)) && (mstShowErr.index > 0))
    {
        LIB_ConstMemCpy((INT8U *)&mErrStr[0], &mstShowData.Protc.showDataBuff[0], MAX_SHOWDATA);        
        LIB_ConstMemCpy((INT8U *)&mShowErrTab[(mstShowErr.index - 1) ].errCode[0], &mstShowData.Protc.showDataBuff[5], ERR_CODE_LEN);/*"将错误代码装填显示缓冲区"*/  
    }
    else/*"其他能从协议树取出的数据"*/  
    {  
        len = PP_ShowGetData(&mstRunData.curScrCode, &mstShowData.Protc);  

        if((0 == len) &&
        (mstRunData.index >= (*mstDispModeInfo[mstRunData.showMode].pDispNum - 1)) &&
        (FALSE == mstRunData.validFlg))
        {/*"当前显示模式无有效显示对象，显示默认屏为当前总电量"*/
            LIB_ConstMemCpy((INT8U *)&mstDefaultObj, (INT8U *)&mstRunData.curScrCode, sizeof(ST_DISP_OBJ));
            len = PP_ShowGetData(&mstRunData.curScrCode, &mstShowData.Protc);  
        }

        if(0 == len)
        {
            ret = FALSE;
        }
        else
        {
            mstRunData.validFlg = TRUE;
            pShowFmt = &mstShowData.Protc.stShowTable.stShowFormat.format;

#ifdef SINGLE_PHASE //(defined(DLT698) || defined(DM_DLT645))
            mOverdraftFlg = 0;
            index = DispMnyChk();
            if(0xFF != index)
            {
                

                
#ifdef PM_MODLE                
                GetSingle(E_OVERDRAFT_MONEY, (INT8U *)&tempMny);/*"取透支金额"*/
                if(0 != tempMny)/*"透支状态"*/
                {
                    if(0 == index)/*"剩余"*/
                    {
                        len = PP_ShowGetData((ST_DISP_OBJ *)&mstDispMny[1], &mstShowData.Protc);              
                        mstShowData.Protc.stShowTable.schar[1] |= 0x04;
                        mOverdraftFlg = 1;
                    }
    
                    //*pShowFmt = E_BCD_4_BYTE;
                    //mstShowData.Protc.showDataBuff[3] |= 0x80;
                }
#endif                
            }
#endif

            DM_OtherInfoDeal(&mstRunData.curScrCode);/*"单独处理协议树不给出的数据（尖峰平谷，上XX月）"*/  

            ret = TRUE;

            if((*pShowFmt > E_TIME_FORMAT) && (*pShowFmt < E_STRING))
            {
                LIB_RvsSelf(&mstShowData.Protc.showDataBuff[0], len);

                if(E_HHMMSS == *pShowFmt)
                {
                    format = FMT_TIME;
                }
                else
                {
                    format = FMT_DATE;
                }
            }
            else
            {
                format = FMT_STRING;
            }

            ShowNumBCD(&mstShowData);  
        }        

#ifdef DIS_USER_CODE
        mstRunData.showUserCodeFlg = 0x55;
#endif
    mstShowData.Protc.stShowTable.stShowFormat.format = format;
#if 0
        if(0 == mstRunData.index)
        {
            LIB_MemSet(DIS_Space, mstShowData.Protc.showDataBuff, MAX_SHOWDATA);
            mstShowData.Protc.showDataBuff[7] = DIS_0;
            mstShowData.Protc.showDataBuff[6] = DIS_0;        
            mstShowData.Protc.showDataBuff[5] = DIS_0;        
            LIB_MemSet(0, mstShowData.Protc.stShowTable.schar, MAX_SCHAR_NUM);   
            mstShowData.Protc.stShowTable.schar[2] |= 0x08;        
            mstShowData.Protc.stShowTable.schar[3] |= 0x20;
            mstShowData.Protc.stShowTable.stShowFormat.exp = -2;
            mstShowData.Protc.stShowTable.stShowFormat.format = FMT_STRING;
            mstShowData.Protc.stShowTable.stShowFormat.unit = 1;
            LIB_MemSet(0, mstShowData.charAdd, MAX_SUB_CHAR_NUM);        
            ret = TRUE;
        }
        else if(1 == mstRunData.index)
        {        
            LIB_MemSet(DIS_Space, mstShowData.Protc.showDataBuff, MAX_SHOWDATA);
            mstShowData.Protc.showDataBuff[2] = ((LIB_CharToBcd(stSysTime.year) & 0xf0)>>4);
            mstShowData.Protc.showDataBuff[3] = (LIB_CharToBcd(stSysTime.year) & 0x0f);
            mstShowData.Protc.showDataBuff[4] = ((LIB_CharToBcd(stSysTime.month) & 0xf0)>>4);
            mstShowData.Protc.showDataBuff[5] = (LIB_CharToBcd(stSysTime.month) & 0x0f);
            mstShowData.Protc.showDataBuff[6] = ((LIB_CharToBcd(stSysTime.day) & 0xf0)>>4);
            mstShowData.Protc.showDataBuff[7] = (LIB_CharToBcd(stSysTime.day) & 0x0f);
     
            LIB_MemSet(0, mstShowData.Protc.stShowTable.schar, MAX_SCHAR_NUM);   
            mstShowData.Protc.stShowTable.stShowFormat.exp = 0;
            mstShowData.Protc.stShowTable.stShowFormat.format = FMT_DATE;
            mstShowData.Protc.stShowTable.stShowFormat.unit = 0;
            LIB_MemSet(0, mstShowData.charAdd, MAX_SUB_CHAR_NUM);    
            mstShowData.charAdd[1] = 6;
            ret = TRUE;
        }
        else if(2 == mstRunData.index)
        {        
            LIB_MemSet(DIS_Space, mstShowData.Protc.showDataBuff, MAX_SHOWDATA);
            mstShowData.Protc.showDataBuff[2] = ((LIB_CharToBcd(stSysTime.hour)&0xf0)>>4);
            mstShowData.Protc.showDataBuff[3] = (LIB_CharToBcd(stSysTime.hour) & 0x0f);
            mstShowData.Protc.showDataBuff[4] = ((LIB_CharToBcd(stSysTime.min) & 0xf0)>>4);
            mstShowData.Protc.showDataBuff[5] = (LIB_CharToBcd(stSysTime.min) & 0x0f);
            mstShowData.Protc.showDataBuff[6] = ((LIB_CharToBcd(stSysTime.sec) & 0xf0)>>4);
            mstShowData.Protc.showDataBuff[7] = (LIB_CharToBcd(stSysTime.sec) & 0x0f);
     
            LIB_MemSet(0, mstShowData.Protc.stShowTable.schar, MAX_SCHAR_NUM);   
            mstShowData.Protc.stShowTable.stShowFormat.exp = 0;
            mstShowData.Protc.stShowTable.stShowFormat.format = FMT_TIME;
            mstShowData.Protc.stShowTable.stShowFormat.unit = 0;
            LIB_MemSet(0, mstShowData.charAdd, MAX_SUB_CHAR_NUM);        
            ret = TRUE;
        }     
        else if(3 == mstRunData.index)
        {        
            LIB_MemSet(DIS_Space, mstShowData.Protc.showDataBuff, MAX_SHOWDATA);
            mstShowData.Protc.showDataBuff[7] = DIS_0;    
            mstShowData.Protc.showDataBuff[6] = DIS_0;  
            mstShowData.Protc.showDataBuff[5] = DIS_0;   
            mstShowData.Protc.showDataBuff[4] = DIS_0;   
            
            LIB_MemSet(0, mstShowData.Protc.stShowTable.schar, MAX_SCHAR_NUM);   
            mstShowData.Protc.stShowTable.stShowFormat.exp = 0;
            mstShowData.Protc.stShowTable.stShowFormat.format = FMT_STRING;
            mstShowData.Protc.stShowTable.stShowFormat.unit = 0;
            LIB_MemSet(0, mstShowData.charAdd, MAX_SUB_CHAR_NUM);        
            ret = TRUE;
        }             
        else if(4 == mstRunData.index)
        {        
            LIB_MemSet(DIS_0, mstShowData.Protc.showDataBuff, MAX_SHOWDATA);
            mstShowData.Protc.showDataBuff[7] = DIS_1;    
            LIB_MemSet(0, mstShowData.Protc.stShowTable.schar, MAX_SCHAR_NUM);   
            mstShowData.Protc.stShowTable.stShowFormat.exp = 0;
            mstShowData.Protc.stShowTable.stShowFormat.format = FMT_STRING;
            mstShowData.Protc.stShowTable.stShowFormat.unit = 0;
            LIB_MemSet(0, mstShowData.charAdd, MAX_SUB_CHAR_NUM);        
            ret = TRUE;
        }
        mstShowData.charAdd[2] = mstRunData.index+1;
#endif
    }



    
    return ret;
}

static INT8U DispMnyChk(void)
{
    INT8U i, ret;

    ret = 0xFF;
    for(i = 0; i < ARRAY_SIZE(mstDispMny); i++)
    {
        if(EQUAL == LIB_MemCmp((INT8U *)&mstRunData.curScrCode, (INT8U *)&mstDispMny[i], sizeof(ST_DISP_OBJ)))
        {
            ret = i;
            break;
        }
    }

    return ret;
}

#if 0
static void ShowBCD(ST_SHOWDATA_OUTPUT *pShow)
{
    INT8S i;
    INT8U *pData;
    INT8U temp[MAX_SHOWDATA];

    pData = (INT8U *)&pShow->showDataBuff[0];
    for (i = (MAX_SHOWDATA - 1); i >= 0; i--) 
    {
        temp[i] = GetHalf(i, pData);
    }
    LIB_Reverse(temp, pData, MAX_SHOWDATA);
}
#endif

const INT16U showObis[] = 
{
    0x2001, 0x2004, 0x2005, 0x2006, 0x200A /*"电流、功率、功率因数"*/
};

static void ShowNumBCD(ST_SHOW_DATA *pShow)
{
    INT8S i;
    INT8U NeedShow, frmt;
    INT8U t, exp, *pData;
    INT8U minus, signAddr;
    INT8U temp[MAX_SHOWDATA];
    INT8U len, flag;

    if(pShow->Protc.stShowTable.stShowFormat.exp == -1)
    {
        exp = 1;
    }
    else if(pShow->Protc.stShowTable.stShowFormat.exp == -2)
    {
        exp = 2;
    }
    else if(pShow->Protc.stShowTable.stShowFormat.exp == -3)
    {
        exp = 3;
    }
    else if(pShow->Protc.stShowTable.stShowFormat.exp == -4)
    {
        exp = 4;
    }        
    else
    {
        exp = 0;
    }

    minus = 0;
    frmt = pShow->Protc.stShowTable.stShowFormat.format;
    len = (INT8U)(FRMT_sizeofFormat(frmt) - 1);
    pData = (INT8U *)&pShow->Protc.showDataBuff[0];

    switch(frmt)
    {
    case E_BCD_1_BYTE:
    case E_BCD_2_BYTE:
    case E_BCD_3_BYTE:
    case E_BCD_4_BYTE:
    case E_ENGY_H_INT:
    case E_ENGY_H_DECIMAL:
        if(*(pData + len) & 0x80)/*"有负号"*/
        {
            *(pData + len) &= 0x7F;
            minus = 1;
        }
        break;
    default: break;
    }

#ifdef SINGLE_PHASE
    if(mOverdraftFlg)
    {
        minus = 1;
        mOverdraftFlg = 0;
    }
#endif

    flag = 0;
    len += 1;
    if((frmt >= E_ASCII_2_LENGTH) && (frmt <= E_ASCII_8_LENGTH) && (len <= MAX_SHOWDATA))
    {/*"ASCII字符"*/
        flag = 0x55;
    }
    else if((frmt > E_TIME_FORMAT) && (frmt <= E_ID_CODE_4_FORMAT) && (len <= (MAX_SHOWDATA >> 1)))
    {
        len <<= 1;
        flag = 0x55;

        if((frmt > E_TIME_FORMAT) && (frmt < E_STRING))
        {/*"时间、时段、结算日等格式"*/
            pShow->charAdd[1] |= (len & 0x0F);
        }
    }
    else
    {
    }

    signAddr = MAX_SHOWDATA;
    NeedShow = GetHalf((MAX_SHOWDATA - 1), pData);
    for (i = (MAX_SHOWDATA - 1); i >= 0; i--) 
    {
        t = GetHalf(i, pData);
        NeedShow = (INT8U)(NeedShow||(t != 0)||(exp  >= i));
        if((NeedShow > 0) || ((0x55 == flag) && (i < len)))
        {
            temp[i] = t;
        }
        else
        {
            temp[i] = DIS_Space;
            signAddr = i;
        }
    }
    LIB_Reverse(temp, pData, MAX_SHOWDATA);

    if(minus)
    {
        for(i = 0; i < ARRAY_SIZE(showObis); i++)
        {
#ifdef DM_DLT698
            if((mstRunData.curScrCode.Oad.Oi.asInt == showObis[i]) ||
            (mstRunData.curScrCode.releOad.Oi.asInt == showObis[i]))
            {
                minus = 0;
#ifdef THREE_PHASE
                //pShow->Protc.stShowTable.schar[1] |= 0x12;/*"反向"*/
                pShow->Protc.stShowTable.schar[CHAR_FAN / 8] |= 1 << (CHAR_FAN % 8);
                pShow->Protc.stShowTable.schar[CHAR_XIANG / 8] |= 1 << (CHAR_XIANG % 8);

#endif
                break;
            }
#endif
        }
    }
    
    if(minus)
    {
        pShow->charAdd[1] |= (signAddr << 4);
    }
}

static INT8U GetHalf(INT8U inIndex, INT8U *inpCh)
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

#ifdef DIS_USER_CODE
static void DM_GetUserCode(void)
{
    INT8S i;
    INT8U temp[4];

    LIB_Reverse((INT8U *)&mstRunData.curScrCode.Oad, temp, sizeof(ST_OAD));
    LIB_RvsSelf((temp + 2), 2);

    for (i = (MAX_USECODE_NUM - 1); i >= 0; i--) 
    {
        mstRunData.userCode.obis[i] = GetHalf(i, temp);
    }

    mstRunData.userCode.offset = 0;
    mstRunData.userCode.len = MAX_USECODE_NUM;
    mstRunData.userCode.pointFlg.byte = 0x14; /*"0x14: 0001 0100 ,  0x54: 0101 0100"*/

    LIB_Reverse((INT8U *)&mstRunData.curScrCode.scrNo, temp, sizeof(INT8U));
    for (i = (MAX_LEAFNO_NUM - 1); i >= 0; i--) 
    {
        mstRunData.userCode.leafNo[i] = GetHalf(i, temp);
    }
}
#endif

INT8S DM_ShowFlash(void *pdin, void *pdout)
{
    INT8U rsult;

#ifdef DIS_USER_CODE
    DM_GetUserCode();
    DIS_PrintLcdLine2(&mstRunData.userCode);/*"显示用户代码"*/  
#endif

    rsult = DIS_PrintLcdLine1(&mstShowData, 0, LEVEL_1);/*"显示刷新"*/  
    if(TRUE == rsult)
    {
        return MS_SUCCESS;
    }
    else
    {
        return MS_OTHER_ERR;
    }
}

/*"************************************************"*/
/*"Function:    DM_OtherInfoDeal"*/
/*"Description: 单独处理协议树不给出的数据（尖峰平谷，上XX月）"*/
/*"Input: pDispObj : 显示对象地址 "*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
#define TARIFF_ID_1   (1)
#define TARIFF_ID_2   (2)
#define TARIFF_ID_3   (3)
#define TARIFF_ID_4   (4)
#define STEP_NUM          (6)     /*"梯度数"*/
#define STEP_PRC_NUM  (7)     /*"梯度电价数"*/

static void DM_OtherInfoDeal(ST_DISP_OBJ *pDispObj)
{
#if 0
    INT8U tariffNo;
#ifdef DIS_REC_TIME
    INT8U RecTimeIndex;
#endif
#ifdef DM_DLT698
    INT8U mode;
    ST_OAD tempOad;
    INT8U *pData;

    
    if((0x50 == pDispObj->Oad.Oi.asByte[1]) || /*"历史冻结数据"*/
    (0x30 == (pDispObj->Oad.Oi.asByte[1] & 0xF0))) /*"事件记录"*/
    {
        pData = (INT8U *)&(pDispObj->releOad);
    }
    else
    {
        pData = (INT8U *)&(pDispObj->Oad);
    }
    LIB_MemCpy(pData, (INT8U *)&tempOad, sizeof(ST_OAD));
#endif

    LIB_MemSet(0, &mstShowData.charAdd[0], MAX_SUB_CHAR_NUM);
#ifdef DIS_REC_TIME
    RecTimeIndex = 0;

#endif

#ifdef DM_DLT698
    if(mstShowData.Protc.stShowTable.schar[0] & 0x01)/*"组合"*/
    {
        GetSingle(E_EXT_MODE1, &mode);
        if((0 == (mode & COMB_SHOW_BIT)) &&
        (0x0000 == tempOad.Oi.asInt))/*"组合有功电量"*/
        {
            mstShowData.Protc.stShowTable.schar[0] &= 0xFE;/*"不显示组合字符"*/
        }
    }
#endif

/*"**************上几月信息**************************************"*/
    /*"协议树给的显示信息有显示 月 "*/
    if(((mstShowData.Protc.stShowTable.schar[0] & 0x02) == 0x02) ||/*"月"*/
    ((mstShowData.Protc.stShowTable.schar[0] & 0x20) == 0x20))/*"上"*/
    {  
#ifdef DM_DLT698
        if((0x50 == pDispObj->Oad.Oi.asByte[1]) || /*"历史冻结数据"*/
        ((0x30 == (pDispObj->Oad.Oi.asByte[1] & 0xF0)) && (2 == pDispObj->Oad.AId)) || /*"事件记录"*/
        (TRUE == ChkPhaseEvtRecord(&(pDispObj->Oad)))) /*"分项事件记录"*/
        {
            #ifdef DIS_REC_TIME
                RecTimeIndex = (INT8U)(pDispObj->Oad.IId & 0x0f);
            #else
                mstShowData.charAdd[0] = (INT8U)(pDispObj->Oad.IId & 0x0f);           
            #endif
        }
#else/*"645"*/
        if(pDispObj->unId645.asBytes[2] <= 0x02)
        {
            #ifdef DIS_REC_TIME
                RecTimeIndex = (INT8U)(pDispObj->unId645.asBytes[0] & 0x0f);
            #else
                mstShowData.charAdd[0] = (INT8U)(pDispObj->unId645.asBytes[0] & 0x0f);           
            #endif
        }
#endif
#ifndef DIS_REC_TIME
        if(0 == mstShowData.charAdd[0])/*"当前要显示当前1月"*/
        {
#ifdef THREE_PHASE
            if(0x2031 != tempOad.Oi.asInt)
#endif
            {
                mstShowData.charAdd[0] = 1;
            }
        }
#endif
    }
#ifdef DIS_REC_TIME
    if(0 != RecTimeIndex)
    {
       SubMouth(RecTimeIndex,&mstShowData.charAdd[3]);
    }
#endif
/*"***********费率号***************************************8"*/
    if(mstShowData.Protc.stShowTable.schar[2] & 0x80)/*"显示费率"*/
    {                
        tariffNo = 0;

#ifdef DLT698
        if((tempOad.IId > 1) ||/*"不是总电量"*/
        (0x201B == tempOad.Oi.asInt) ||/*"当前费率电价"*/
        (0x4018 == tempOad.Oi.asInt))/*"当前套费率电价"*/
        {
            if(0x201B == tempOad.Oi.asInt)
            {
                GetSingle(E_TARIFF_NOW, &tariffNo);
            }
            else if(0x4018 == tempOad.Oi.asInt)
            {
                tariffNo = tempOad.IId;
            }
            else /*"电量费率"*/
            {
                if(tempOad.IId > 1)
                {
                    tariffNo = tempOad.IId - 1;
                }
            }

//#else/*"645"*/
//        if(pDispObj->unId645.asBytes[1] != 0)/*"不是总数据"*/
//        {
//            tariffNo = pDispObj->unId645.asBytes[1];

#endif
            if(tariffNo > 8)
            {
                mstShowData.charAdd[0] &= 0x0f;
            }
            else
            {
                mstShowData.charAdd[0] = ((tariffNo&0x0F)<<4);
            }

        }
        else/*"总"*/
        {
            mstShowData.Protc.stShowTable.schar[3] |= 0x20;
        }
    }
    #if 0
#ifdef DIS_SCR_NO
    if(pDispObj->scrNo)
    {
         mstShowData.charAdd[2] = pDispObj->scrNo;
    }
#endif
#endif
    /*"******************显示阶梯号****************************"*/
#ifdef PM_MODLE    
    if(((0x401A == tempOad.Oi.asInt) || (0x401B == tempOad.Oi.asInt)) && /*"阶梯电价表"*/
    (0 != pDispObj->scrNo) && (pDispObj->scrNo <= STEP_PRC_NUM)) 
    {
        if(tariffNo > 8)
        {
            mstShowData.charAdd[0] &= 0x0f;
        }
        else
        {
            mstShowData.charAdd[0] = ((pDispObj->scrNo&0x0F)<<4);
        }
    }
    else
    {
    }
#endif

/*"********************象限***************************"*/
#ifdef DIS_QUAD_INFO
    INT32S sCurr[3];
    INT8U nowQuad, engMode;
    UN_OI intOad;

    intOad.asInt = tempOad.Oi.asInt & 0xFFF0;
    if((0x0030 == intOad.asInt) || (0x0040 == intOad.asInt) ||/*"组合无功电量"*/
    (0x1030 == intOad.asInt) || (0x1040 == intOad.asInt))/*"组合无功需量"*/
    {
        if(0x30 == intOad.asByte[0])
        {
            GetSingle(E_R_ENGY_COM_TYPE1, &engMode);
        }
        else
        {
            GetSingle(E_R_ENGY_COM_TYPE2, &engMode);
        }
        showQuad(engMode);
    }
    else
    {
        GetSingle(E_CURR_BLK, (INT8U *)&sCurr[0]);
        if(((sCurr[0] != 0) || (sCurr[1] != 0) || (sCurr[2]!= 0)) &&
        (RUN_MODE_PD != mstRunData.runMode))
        {
            GetSingle(E_QUAD_TOTAL, &nowQuad);
            switch(nowQuad)
            {
            case 0:
                mstShowData.charAdd[2] |= 0x01; 
                break;
            case 1:
                mstShowData.charAdd[2] |= 0x02; 
                break;
            case 2:
                mstShowData.charAdd[2] |= 0x04; 
                break;
            case 3:
                mstShowData.charAdd[2] |= 0x08; 
                break;
            default:
                break;
            }
        }
    }
#endif
#endif
}

#ifdef DIS_QUAD_INFO
static void showQuad(INT8U  comMode)
{
    if(comMode & 0x01)/*"I"*/
    {
        mstShowData.charAdd[2] |= 0x01; 
    }
    if(comMode & 0x04)/*"II"*/
    {
        mstShowData.charAdd[2] |= 0x02; 
    }
    if(comMode & 0x10)/*"III"*/
    {
        mstShowData.charAdd[2] |= 0x04; 
    }
    if(comMode & 0x40)/*"IV"*/
    {
        mstShowData.charAdd[2] |= 0x08; 
    }

    if((RUN_MODE_PD != mstRunData.runMode) &&
    (0 != (mstShowData.charAdd[2] & 0x0F)))
    {
        mstShowData.charAdd[2] |= 0x10; 
    }
}
#endif

/*"************************************************"*/
/*"Function:    DM_PDMain"*/
/*"Description: 掉电显示主接口"*/
/*"Input: pPdShowList  显示取数缓存"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
void DM_PDMain(ST_DM_PD_DATA *pPdShowList)
{
    INT8U i;
    INT8U keyId;
    ST_DM_PD_DATA *pData;

    if(NULL == pPdShowList)
    {
        return;
    }

    keyId = KEY_GetKeyPD();

    DM_KeyProcess(keyId);

#ifdef THREE_PHASE
    if(PD_KEY_WAKE_UP == mPdIrWakeUpFlg)
    {
        OnKeyDisp_PDInitMode();
        mPdIrWakeUpFlg = 0;
        mPdKeyWakeUpFlg = PD_KEY_WAKE_UP;
        mPdIrComFlg = 0;
    }
    else if(PD_IR_COMM == mPdIrComFlg)
    {
        if(PD_CYC_SHOW_MODE == mstRunData.showMode)
        {
            mstRunData.pdIrTimer = stDispPara.LPKeyDisTime;                
            mPdIrComFlg = PD_IR_SHOW;
        }
        else
        {
            mPdIrComFlg = 0;
            mstRunData.pdIrTimer = stDispPara.LPKeyDisTime;
        }
    }
    else
    {
    }
#endif

    if((TRUE == mPdUpateFlg) && (PD_INIT_SHOW_MODE != mstRunData.showMode))
    {
        if(DISP_CLOCK_OBJ)
        {/*"当前时间显示，每秒刷新"*/
            i = 0;
            //mPdUpateFlg = TRUE;
        }
        else
        {
            mPdUpateFlg = FALSE;
            pData = pPdShowList;

            for(i = 0; i < mstRunData.pdObjNum; i++)/*"检查是否ram已经保存了显示数据"*/
            {
                if(EQUAL == LIB_MemCmp((INT8U *)&mstRunData.curScrCode, (INT8U *)&(pData[i].item), sizeof(ST_DISP_OBJ)))
                {
                    mstShowData = pData[i].data;/*"把找到的数据放入显示数据全局变量"*/
                    mstRunData.showRet = TRUE;
                    break;
                }
            }

        }

        //if((i >= mstRunData.pdObjNum) || (TRUE == mPdUpateFlg))/*"没有找到RAM保存的显示数据"*/
        {                    
            mstRunData.showRet = DM_GetShowData();

            if(TRUE == mstRunData.showRet)
            {
                if((mstRunData.pdObjNum < PD_DISP_DATA_NUM ) && (FALSE == mPdUpateFlg))
                {/*"显示数据缓冲区没有存满，则继续保存数据"*/
                    pData[mstRunData.pdObjNum].data = mstShowData;
                    LIB_MemCpy((INT8U *)&mstRunData.curScrCode, (INT8U *)&(pData[mstRunData.pdObjNum].item), sizeof(ST_DISP_OBJ));
                    mstRunData.pdObjNum++;
                }     
            }
        }
        //else
        {
            //DM_OtherInfoDeal(&mstRunData.curScrCode);
        }

        if(TRUE == mstRunData.showRet)
        {
#ifdef DIS_USER_CODE
            DM_GetUserCode();
            DIS_PrintLcdLine2(&mstRunData.userCode);/*"显示用户代码"*/  
#endif

            DIS_PrintLcdLine1(&mstShowData, 0, LEVEL_1);
        }

#if 0
        if(TRUE == mPdUpateFlg)
        {/*"时间需要每秒刷新，下个秒中断重新取数"*/  
            mstRunData.showRet = FALSE;
        }
#endif
    } 
}

void DM_PD1s(void)
{
    if(PD_INIT_SHOW_MODE != mstRunData.showMode)    
    {
        DM_TimerDec();

#ifdef SINGLE_PHASE
        if((0 == (mstRunData.scrTimer % 5)) && 
        (TRUE == mstRunData.showRet))/*"当前屏取数成功"*/
#endif
        {/*"防止液晶驱动被静电打乱，需5S刷新一次显示"*/
#ifdef DIS_USER_CODE
            DM_GetUserCode();
            DIS_PrintLcdLine2(&mstRunData.userCode);/*"显示用户代码"*/  
#endif

            DIS_PrintLcdLine1(&mstShowData, 0, LEVEL_1);
        }
    }
    if(2 == mstRunData.index)
    {
        mPdUpateFlg = TRUE;
    }
}

#ifdef DM_DLT698
static INT8U ChkPhaseEvtRecord(ST_OAD *pEvtOad)
{
    if((((pEvtOad->Oi.asInt >= 0x3000) && (pEvtOad->Oi.asInt <= 0x3008)) ||
    (pEvtOad->Oi.asInt == 0x300B)) &&
    ((pEvtOad->AId >= 6/*7*/) && (pEvtOad->AId <= 9)))
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
#endif

/*"************************************************"*/
/*"Function:    DM_GetDisPara"*/
/*"Description: 显示项目读取"*/
/*"Input:   obisId    显示对象标识"*/
/*"            index     块对象元素索引"*/
/*"            *pBuff   返回数据地址"*/
/*"Output: 无"*/
/*"Return: DAR"*/
/*"Others: "*/
/*"************************************************"*/
INT16S DM_GetDisPara(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U *pBuff)
{
#ifdef DM_DLT698
    ST_DISP_OBJ showData;
    INT16U offset;
    INT8U cls, phaseEvt;;
#endif
    ST_DISP_PARA_OBJ disp;
    INT8U len, kind;


    if((NULL == BlkAttr) || (NULL == pBuff))
    {
        return MS_OTHER_ERR;
    }

    len = 0;
    kind = (BlkAttr->obis - E_CYCLE_DIS_OBJ_LIST) >> DM_BLK_LEFT_SHIFT;
    if((kind < MAX_LIST_TYPE) && (0 != index))
    {
#ifdef DM_DLT698
        offset = mstDispListInfo[kind].shift + ((INT16U)(index - 1) * sizeof(ST_DISP_OBJ));
        FS_ReadFile(E_FILE_UI_PARA_NO_BACKUP, offset, (INT8U *)&showData, sizeof(ST_DISP_OBJ));

        cls = showData.Oad.Oi.asByte[1] >> 4;
        phaseEvt = 0;

#ifdef THREE_PHASE
        if(TRUE == ChkPhaseEvtRecord(&showData.Oad))
        {
            phaseEvt = 0x55;
        }

#endif

        if((((3 == cls) || (5 == cls)) && (2 == showData.Oad.AId)) ||/*"记录数据"*/
        (0x55 == phaseEvt))
        {
            len = sizeof(ST_DISP_OBJ2);
            disp.item2.Csd = OBJ_IS_ROAD;
            LIB_MemCpy((INT8U *)&showData.Oad, disp.item2.Oad, sizeof(ST_OAD));
            disp.item2.cnt = 1;
            LIB_MemCpy((INT8U *)&showData.releOad, disp.item2.releOad, sizeof(ST_OAD)); 
            disp.item2.scrNo = showData.scrNo;
        }
        else
        {
            len = sizeof(ST_DISP_OBJ1);
            disp.item1.Csd = OBJ_IS_OAD;
            LIB_MemCpy((INT8U *)&showData.Oad, disp.item1.Oad, sizeof(ST_OAD));
            disp.item1.scrNo = showData.scrNo;
        }
#endif
        LIB_MemCpy((INT8U *)&disp, pBuff, len);

        return (INT16S)len;
    }
    else
    {
        return MS_OBIS_ERR;
    }
}

/*"************************************************"*/
/*"Function:    DM_SetDisPara"*/
/*"Description: 显示项目读取"*/
/*"Input: pIdList   显示对象标识 "*/
/*"           IdNum  访问对象数"*/
/*"           *pdata  返回数据地址"*/
/*"           len        设置数据长度"*/
/*"Output: 无"*/
/*"Return: DAR"*/
/*"Others: "*/
/*"************************************************"*/
#define DISP_LIST_MAX  ((CYC_DISP_MAX > KEY_DISP_MAX) ? CYC_DISP_MAX : KEY_DISP_MAX)

INT16S DM_SetDisPara(ST_BLK_DATA_ATTR* BlkAttr, INT8U index, INT8U setLen, INT8U *pData)
{
#ifdef DM_DLT698
    INT16S result;
    INT8U *pItem;
    INT8U num, kind, i, cnt;
    INT16S dataLen;
    ST_DISP_OBJ dispItem;
    ST_DISP_PARA_OBJ disp;
    INT8U lenList[DISP_LIST_MAX];
    INT16U offset;

    if((NULL == BlkAttr) || (NULL == pData))
    {
        return MS_OTHER_ERR;
    }

    kind = (BlkAttr->obis - E_CYCLE_DIS_OBJ_LIST) >> DM_BLK_LEFT_SHIFT;
    if(kind >= MAX_LIST_TYPE)
    {
        result = MS_OBIS_ERR;
    }
    else if(0 == setLen)
    {
        result = MS_LEN_ERR;
    }
    else
    {
        result = MS_SUCCESS;
    }

    if(MS_SUCCESS == result)
    {
        dataLen = setLen;
        pItem = pData;
        num = 0;

#ifdef DM_DLT698
        do
        {/*"显示对象长度检查"*/
            if(OBJ_IS_OAD == *pItem)
            {
                lenList[num] = sizeof(ST_DISP_OBJ1);
            }
            else if(OBJ_IS_ROAD == *pItem)
            {
               lenList[num] = sizeof(ST_DISP_OBJ2);
            }
            else
            {
                break;
            }

            pItem += lenList[num];
            dataLen -= lenList[num];

            num++;
            if(num >= mstDispListInfo[kind].num)
            {
                break;
            }
        }while(dataLen > 0);

        if((0 != dataLen) || ((0 != index) && (1 != num)))/*"长度错误 or  设置单个显示项,项目数不为1"*/
        {
            result = MS_LEN_ERR;
        }
        else
        {/*"显示对象参数保存"*/
            if(0 == index)
            {/*"显示对象块设时，索引从1开始"*/
                cnt = 0;
            }
            else
            {
                cnt = index - 1;
            }

            pItem = pData;
            for(i = 0; i < num; i++)
            {
                LIB_MemCpy(pItem, (INT8U *)&disp, sizeof(UN_DISP_OBJ));
                if(lenList[i] == sizeof(ST_DISP_OBJ2))/*"记录数据"*/
                {
                    LIB_MemCpy(disp.item2.Oad, (INT8U *)&dispItem.Oad, sizeof(ST_OAD));
                    LIB_MemCpy(disp.item2.releOad, (INT8U *)&dispItem.releOad, sizeof(ST_OAD));
                    dispItem.scrNo = disp.item2.scrNo;
                }
                else
                {
                    LIB_MemSet(0x00, (INT8U *)&dispItem, sizeof(ST_DISP_OBJ));
                    LIB_MemCpy(disp.item1.Oad, (INT8U *)&dispItem.Oad, sizeof(ST_OAD));
                    dispItem.scrNo = disp.item1.scrNo;
                }

                offset = mstDispListInfo[kind].shift + ((INT16U)cnt * sizeof(ST_DISP_OBJ));
                FS_WriteFile(E_FILE_UI_PARA_NO_BACKUP, offset, (INT8U *)&dispItem, sizeof(ST_DISP_OBJ));

                pItem += lenList[i];
                cnt++;
            }

            if(0 == index)/*"块设时，剩余显示对象参数清零"*/
            {
                LIB_MemSet(0, (INT8U *)&dispItem, sizeof(ST_DISP_OBJ));

                for(i = num; i < mstDispListInfo[kind].num; i++)
                {
                    offset = mstDispListInfo[kind].shift + ((INT16U)i * sizeof(ST_DISP_OBJ));
                    FS_WriteFile(E_FILE_UI_PARA_NO_BACKUP, offset, (INT8U *)&dispItem, sizeof(ST_DISP_OBJ));
                }
            }
        }
#endif
    }

    return result;
#else
    return 0;
#endif
}

/*"************************************************"*/
/*"Function:    DM_GetShowErr"*/
/*"Description: 取得需要显示报警的错误"*/
/*"Input:  无"*/
/*"Output:  无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
static void DM_GetShowErr(void)
{
    INT8U i;
    INT16U errStatus;
    INT16U oldRunErr;

    if(RUN_MODE_NORMAL == mstRunData.runMode)
    {/*"上电初始化显示结束后允许显示错误代码"*/
        oldRunErr = mstShowErr.runErr;

        if(TRUE != ChkStatus(E_FAC))/*"厂外"*/
        {
            mstShowErr.runErr = 0;
        }
        else/*"厂内显示报警错误代码"*/
        {
            errStatus = 0;

            for(i = 0; i < ARRAY_SIZE(mShowErrTab); i++)
            {
                if(TRUE == ChkStatus(mShowErrTab[i].staIndex))
                {
                    errStatus |= (0x01 << i);
                }
            }

            mstShowErr.runErr = errStatus;
        }

        if(mstShowErr.runErr == 0)
        {
            mstShowErr.index = 0;
        }
        else
        {
            if(0 == oldRunErr)
            {
                mShowErrFlg = 0x55;
            }
        }
    }
}

/*"************************************************"*/
/*"Function:    DM_ShowAll"*/
/*"Description: 全屏显示"*/
/*"Input: pdin   参见ST_ACTION_DATa"*/
/*"Output: 无"*/
/*"Return: DAR"*/
/*"Others: 698协议方法"*/
/*"************************************************"*/
INT8S DM_ShowAll(void *pdin, void *pdout)
{
    INT16U time;
    INT16U len;
    INT8U *pData;
    INT32U paraAddr;

    if(NULL == pdin)
    {
        return MS_OTHER_ERR;
    }

    pData = (INT8U *)pdin + sizeof(INT16U);
    LIB_MemCpy(pData, (INT8U *)&len, sizeof(INT16U));/*"取参数长度"*/
    if(sizeof(INT16U) == len)
    {
        pData += len;
        LIB_MemCpy(pData, (INT8U *)&paraAddr, sizeof(INT32U));/*"取参数地址"*/
        pData = (INT8U *)paraAddr;
        LIB_MemCpy(pData, (INT8U *)&time, sizeof(INT16U));
        if(0 != time)
        {
            ShowSpecScr((ST_DISP_OBJ *)&mstSpecDisp[DISP_ALL], time, LEVEL_1);/*"全屏时可切按显"*/
        }
        return MS_SUCCESS;
    }
    else
    {
        return MS_LEN_ERR;
    }
}

/*"************************************************"*/
/*"Function:    DM_GetLastScr"*/
/*"Description: 取得上一屏显示代码"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
static void DM_GetLastScr(void)
{
    INT8U *pNum;

    pNum = mstDispModeInfo[mstRunData.showMode].pDispNum;
    if((NULL != pNum) && (*pNum >= 1))
    {
        mstRunData.curNum = *pNum;
    }
    else
    {
        mstRunData.curNum = 1;
    }

    if(mstShowErr.index == (ARRAY_SIZE(mShowErrTab) + 1))
    {
        mstShowErr.index= 0xFF;
    }

    if(0x55 == mShowErrFlg)/*"故障代码可插入显示"*/
    {
        mShowErrFlg = 0;
        if((0xFF == mstShowErr.index) || (0 == mstShowErr.index))
        {
            mstShowErr.index = ARRAY_SIZE(mShowErrTab) + 1;
        }
    }

    if((0 == mstShowErr.runErr) || (0 == mstShowErr.index) || (0xFF == mstShowErr.index))
    {/*"没有运行错误，或者已经轮显完毕错误代码"*/
        if(mstRunData.index > 0)/*"显示项目轮显中"*/
        {
            mstRunData.index --;/*"显示项目索引值递减"*/
        }
        else/*"轮显结束"*/
        {
            if(mstShowErr.runErr == 0)  /*"没有运行错误"*/
            {
                mstRunData.index = mstRunData.curNum - 1;
            }

            mstShowErr.index = ARRAY_SIZE(mShowErrTab) + 1;/*"错误显示索引值"*/
            mstRunData.validFlg = FALSE;
        }
    }
    else
    {
    }

    DM_GetScr();
}

INT8S DM_ShowNext(void *pdin, void *pdout)
{
    if(KEY_SHOW_MODE == mstRunData.showMode)/*"当前为按键显示"*/ 
    {
#ifdef THREE_PHASE
        SetKeyFunc(DISP_KEY_DOWN);
#else
        DM_GetNextScr();/*"提取下一屏显示代码"*/
#endif

        mstRunData.showRet = DM_GetShowData();
        SetBacklight(stDispPara.BacklightTime);/*"点亮背光"*/ 
    }
    else
    {
        OnKeyDisp_CycMode();
    }

    return MS_SUCCESS;
}

INT8S DM_ShowLast(void *pdin, void *pdout)
{
    if(KEY_SHOW_MODE == mstRunData.showMode)/*"当前为按键显示"*/ 
    {
#ifdef THREE_PHASE
        SetKeyFunc(DISP_KEY_UP);
#else
        DM_GetLastScr();/*"提取上一屏显示代码"*/
#endif

        mstRunData.showRet = DM_GetShowData();
        SetBacklight(stDispPara.BacklightTime);/*"点亮背光"*/ 
    }
    else
    {
        OnKeyDisp_CycMode();
    }

    return MS_SUCCESS;
}

static void DM_TimerDec(void)
{
    if(mstRunData.scrTimer > 0)/*"当前屏显示时间递减"*/ 
    {
        mstRunData.scrTimer --;
    }

    if(mstRunData.pdShowTime > 0)/*"当前屏显示时间递减"*/ 
    {
        mstRunData.pdShowTime --;
    }

#ifdef THREE_PHASE
    if(mstRunData.pdIrTimer > 0)/*"红外通信唤醒时间递减"*/
    {
        mstRunData.pdIrTimer --;
    }
#endif
}

#ifndef KEY_KEYPAD
static void OnKeyDisp_CycMode(void)
{   
    //trigger_main_key();
    
    DM_SetMode(KEY_SHOW_MODE);/*"进入按键显示"*/

    SetBacklight(stDispPara.BacklightTime);/*"点亮背光"*/ 

#ifdef THREE_PHASE
    Action(E_RLYALARM_KEYCLR, NULL, NULL);
#endif
}
extern INT8U tmpggIndex;
static void OnKeyDisp_KeyMode(void)
{
    //trigger_main_key();
tmpggIndex ++;
#ifdef THREE_PHASE
    SetKeyFunc(DISP_KEY_DOWN);
#else
    DM_GetNextScr();/*"提取下一屏显示代码"*/
#endif

    SetBacklight(stDispPara.BacklightTime);/*"点亮背光"*/ 

#ifdef THREE_PHASE
    Action(E_RLYALARM_KEYCLR, NULL, NULL);
#endif
}

#ifdef THREE_PHASE
static void OnKeyDisp2_KeyMode(void)
{
    SetKeyFunc(DISP_KEY_UP);

    SetBacklight(stDispPara.BacklightTime);/*"点亮背光"*/ 

    Action(E_RLYALARM_KEYCLR, NULL, NULL);
}
#endif

static void OnKeyDisp_InitMode(void)
{
    DM_SetMode(KEY_SHOW_MODE);/*"进入按键显示"*/

    SetBacklight(stDispPara.BacklightTime);/*"点亮背光"*/ 
}

static void OnKeyDisp_PDCycMode(void)
{
    DM_SetMode(PD_KEY_SHOW_MODE);/*"进入按键显示"*/
}

static void OnKeyDisp_PDKeyMode(void)
{
#ifdef THREE_PHASE
    SetKeyFunc(DISP_KEY_DOWN);
#else
    DM_GetNextScr();/*"提取下一屏显示代码"*/
#endif
}

#ifdef THREE_PHASE
static void OnKeyDisp2_PDKeyMode(void)
{
    SetKeyFunc(DISP_KEY_UP);
}

static void OnKeyFacCheck(void)
{
    INT8U InData;

    InData = 1;
    if(RUN_MODE_PD != mstRunData.runMode)
    {
        Action(E_AUTO_KEYSELFTEST, &InData, NULL);/*"自动化生产用"*/
    }
}
#endif

static void OnKeyDisp_PDInitMode(void)
{
    LCD_Init(MCU_LPMODE_DEEP);

    DM_SetMode(PD_CYC_SHOW_MODE);/*"按键唤醒后进入循显"*/
}

static void OnKeyRlyCls(void)
{
#ifdef PM_MODLE
    if(RUN_MODE_PD != mstRunData.runMode)
    {
        Action(E_CARD_RLYHOLD, NULL, NULL);/*"3秒按键合闸"*/
    }
#endif    

}
static void OnKeyInvalid_CycMode(void)
{
    if((0 == mstRunData.scrTimer) || (FALSE == mstRunData.showRet))/*"当前屏显示时间结束"*/ 
    {
        DM_GetNextScr();/*"提取下一屏显示代码"*/ 
    }
}

static void OnKeyInvalid_KeyMode(void)
{
    if(0 == mstRunData.scrTimer)/*"当前屏显示时间结束"*/ 
    {
        DM_SetMode(CYC_SHOW_MODE);
    }
    else if(FALSE == mstRunData.showRet)
    {
#ifdef THREE_PHASE
        if(DISP_KEY_UP == mstRunData.keyFlg)
        {
            DM_GetLastScr();
        }
        else
#endif
        {
            DM_GetNextScr();
        }
    }
    else
    {
    }
}

static void OnKeyInvalid_InitMode(void)
{
    if(0 == mstRunData.scrTimer)/*"当前屏显示时间结束"*/ 
    {
#if 0/*"版本号在全屏之后显示"*/
        if((TRUE != ChkStatus(E_FAC)) && (DISP_VER_ID == mstRunData.index))
        {/*"厂外模式不显示版本号"*/ 
            mstRunData.index += 1;
        }
#endif
        DM_GetNextScr();/*"提取下一屏显示代码"*/ 
    }
}

static void OnKeyInvalid_PDCycMode(void)
{
    if((0 == mstRunData.scrTimer) || /*"当前屏显示时间结束"*/ 
    ((FALSE == mstRunData.showRet) && (FALSE == mPdUpateFlg)))/*"取数失败时取下一屏"*/
    {
        DM_GetNextScr();/*"提取下一屏显示代码"*/ 

#ifdef THREE_PHASE
        if(((0 == mstRunData.index) && (0 == mPdIrComFlg)) ||
        ((PD_IR_SHOW == mPdIrComFlg) && (0 == mstRunData.pdIrTimer)))
        {
            DM_SetMode(PD_INIT_SHOW_MODE);

            mPdKeyWakeUpFlg = 0;
            mPdIrComFlg = 0;
        }        

#else
        if(0 == mstRunData.index)
        {/*"低功耗循显完一轮后进入掉电显示状态(关屏/常显)"*/ 
            DM_SetMode(PD_INIT_SHOW_MODE);
        }        
#endif
    }
    else
    {
    }
}

static void OnKeyInvalid_PDKeyMode(void)
{
#ifdef THREE_PHASE
    if((0 == mstRunData.scrTimer) && (0 == mstRunData.pdIrTimer))/*"当前屏显示时间结束且无红外通信"*/ 
#else
    if(0 == mstRunData.scrTimer)/*"当前屏显示时间结束"*/ 
#endif
    {
        DM_SetMode(PD_INIT_SHOW_MODE);

#ifdef THREE_PHASE
        mPdKeyWakeUpFlg = 0;
#endif
    }
    else if((FALSE == mstRunData.showRet) && (FALSE == mPdUpateFlg))
    {/*"取数失败时取下一屏"*/
#ifdef THREE_PHASE
        if(DISP_KEY_UP == mstRunData.keyFlg)
        {
            DM_GetLastScr();
        }
        else
#endif
        {
            DM_GetNextScr();
        }
    }
    else
    {
    }
}

static void OnKeyInvalid_SpecMode(void)
{
    if(0 == mstRunData.scrTimer)/*"当前屏显示时间结束"*/ 
    {
        DM_SetMode(CYC_SHOW_MODE);
    }
}

static void OnKeyDisp_SpecMode(void)
{
    DM_SetMode(KEY_SHOW_MODE);/*"进入按键显示"*/

    SetBacklight(stDispPara.BacklightTime);/*"点亮背光"*/ 
}

#endif

/*"************************************************"*/
/*"Function:    DM_KeyProcess"*/
/*"Description: 按键处理接口"*/
/*"Input: 无"*/
/*"Output: 无"*/
/*"Return: 无"*/
/*"Others: "*/
/*"************************************************"*/
static void DM_KeyProcess(INT8U keyId)
{
    INT8U index;

    if(KEY_ID_INVALID == keyId)
    {
        index = 0;
    }
    else if(keyId < MAX_KEY_ID)
    {
        index = keyId + 1;

#ifdef THREE_PHASE
        if(RUN_MODE_PD == mstRunData.runMode)
        {
            mPdKeyWakeUpFlg = PD_KEY_WAKE_UP;
            MCU_RunModeSet(MCU_LPMODE_LIGHT);         
        }
#endif
    }
    else
    {
        index = 0xFF;
    }

    if((index <= MAX_KEY_ID) && (NULL != mstKeyFunc[mstRunData.showMode][index].keyFunc))
    {
        mstKeyFunc[mstRunData.showMode][index].keyFunc();
    }
}

#ifdef THREE_PHASE
static void SetKeyFunc(INT8U keyFlag)
{
    INT8U mode;

    GetSingle(E_EXT_MODE1, &mode);

    if(((DISP_KEY_DOWN == keyFlag) && (mode & KEY_FUNC_BIT)) ||
    ((DISP_KEY_UP == keyFlag) && (0 == (mode & KEY_FUNC_BIT))))
    {
        mstRunData.keyFlg = DISP_KEY_DOWN;
        DM_GetNextScr();/*"提取下一屏显示代码"*/
    }
    else
    {
        mstRunData.keyFlg = DISP_KEY_UP;
        DM_GetLastScr();/*"提取上一屏显示代码"*/
    }
}
#endif

void SubMouth(INT8U MonthNum,INT8U *outTime)
{
    INT8U data[2];
    ST_7TIME_FORM nowTime;
    if(MonthNum > 12)
        return;
    
    GetSingle(E_SYS_TIME, (INT8U *)&nowTime);
   LIB_MemCpy((INT8U *)&nowTime.year, data, 2);
   #if 0
    for(i=0;i<2;i++)
    {
        #if   (APP_TIME_FRMT == HEX_FRMT)
        data[i]=*((INT8U *)nowTime+i);
        #else
        data[i]=LIB_BcdToChar(*((INT8U *)nowTime+i));
        #endif
    }
    #endif
    if( nowTime.month<= MonthNum )
    {
        data[1]+=12;
        data[0]=(nowTime.year-1)%100;        
    }
    data[1] -= MonthNum;

    LIB_MemCpy(data, outTime, 2);

}
void DM_freshDisData(ST_SHOW_DATA *inData)
{
	DIS_PrintLcdLine1(inData, 0, LEVEL_4);/*"显示刷新"*/	
}

#if 0
void DM_freshDisData(INT8U *inData)
{
	ST_SHOW_DATA tmpShowData;
	INT8U inPos,i,j;
	LIB_MemCpy((INT8U *)&inData[0],(INT8U *)&tmpShowData.Protc.stShowTable.schar[0],MAX_SCHAR_NUM);
	inPos = MAX_SCHAR_NUM;
	LIB_MemCpy((INT8U *)&inData[inPos],(INT8U *)&tmpShowData.charAdd[0],MAX_SUB_CHAR_NUM);
	inPos += MAX_SUB_CHAR_NUM;
	LIB_MemCpy((INT8U *)&inData[inPos],(INT8U *)&tmpShowData.Protc.showDataBuff[0],MAX_SHOWDATA);
	inPos += MAX_SHOWDATA;
	LIB_MemCpy((INT8U *)&inData[inPos],(INT8U *)&tmpShowData.Protc.stShowTable.stShowFormat.format,3);
	inPos += 3;
	for(j = 0;j<2;j++)
	{
		for(i = 0;i < 8;i++)
		{
			if(inData[inPos+j] & (0x01<<i))
			{
				DIS_SetStauSeg(i+j*8);
			}
		}		
	}
	inPos += 1;
	for(j = 0;j<2;j++)
	{
		for(i = 0;i < 8;i++)
		{
			if(inData[inPos+j] & (0x01<<i))
			{
				DIS_FlashStauSeg(i+j*8);
			}
		}		
	} 
	
	DIS_PrintLcdLine1(&tmpShowData, 0, LEVEL_4);/*"显示刷新"*/	

}
#endif

