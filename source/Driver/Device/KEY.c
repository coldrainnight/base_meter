/*"***********************************************************"*/		
/*"FileName: key.c"*/		
/*"Author: YinLing"*/       
/*"Version : 1.0       Date: 2017.03.03"*/	
/*"Description: �ײ㰴������"*/
/*"Function List:"*/	
/*"<FunctionName>"*/
/*"<author> <time> <version > <desc>"*/		
	
/*"***********************************************************"*/	

#include "Driver\Device\KEY.h"
#include "Driver\MCU\MCU.h"
#include "Driver\MCU\GPIO.h"
#include "Lib\LIB_func.h"


#define  KEYVAL_MAX_ID   (8)/*"һ�������������ɶ�Ӧ���ܼ�ֵID��"*/
#ifdef THREE_PHASE
#define  MAX_PD_KEY_NUM  (2)/*"�����͹��ļ��2������"*/
#else
#define  MAX_PD_KEY_NUM  (1)/*"��������̱�͹��ļ��һ������"*/
#endif

#define  NO_KEY       (0)/*"�ް���"*/
#define  SGL_KEY      (1)/*"����"*/
#define  COMB_KEY   (2)/*"��ϼ�"*/

#define  COMB_TO_SGL ((1 == mKeyFlg.bits.comb) && (SGL_KEY == mKeyFlg.bits.curr)) /*"��ϼ�ת����"*/

static INT16U mCurKeyVal;/*"��ǰ����ֵ"*/
static INT16U mOutKeyVal;/*"������������ֵ"*/
static INT16U mKeyTick;/*"������������ʱ"*/
static INT8U mLastKeyID;/*"�ϴ�����Ĺ��ܼ�ֵID"*/
static ST_KEY_UP_INFO mstKeyUp;/*"����������Ĺ��ܼ�ֵ��Ϣ"*/
#ifdef KEY_KEYPAD
static INT8U mList;/*"������ļ�ֵID"*/
static INT8U mKeyIdBuff[KEY_ID_MAX];
static INT8U mRdIndex;
static INT8U mKeyIdCnt;
#else
static INT8U mList[KEYVAL_MAX_ID];/*"������ļ�ֵID�б�"*/
static INT8U mKeyId;
#endif
static INT8U mCnt;/*"��¼������ļ�ֵID����"*/
static UN_KEY_FLAG mKeyFlg;/*"��ǰ����״̬��־"*/
static INT8U mKeyStaPD[MAX_PD_KEY_NUM];/*"�͹��İ������״ֵ̬"*/


#ifdef KEY_KEYPAD
const ST_KEY_ID_INFO mstKeyIdInfo[MAX_KEY_ID] = 
{
    {KEY_VAL_NUM_0, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 0
    {KEY_VAL_NUM_1, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 1
    {KEY_VAL_NUM_2, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 2
    {KEY_VAL_NUM_3, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 3
    {KEY_VAL_NUM_4, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 4
    {KEY_VAL_NUM_5, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 5
    {KEY_VAL_NUM_6, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 6
    {KEY_VAL_NUM_7, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 7
    {KEY_VAL_NUM_8, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 8
    {KEY_VAL_NUM_9, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 9
    {KEY_VAL_BACKSPACE, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 10
    {KEY_VAL_ENTER, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 11

    //{KEY_VAL_ENTER, FUNC2_TIME, DOWN_CHK, FALSE}, // 12
    //{KEY_VAL_ENTER, TEST_MODE_TIME, UP_CHK, FALSE}, // 13
    //{KEY_VAL_ENTER, FUNC1_TIME, DOWN_CHK, TRUE}, // 14

    {KEY_VAL_ENTER_0, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 15
    {KEY_VAL_ENTER_1, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 16
    {KEY_VAL_ENTER_2, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 17
    {KEY_VAL_ENTER_3, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 18
    {KEY_VAL_ENTER_4, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 19
    {KEY_VAL_ENTER_5, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 20
    {KEY_VAL_ENTER_6, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 21
    {KEY_VAL_ENTER_7, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 22
    {KEY_VAL_ENTER_8, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 23
    {KEY_VAL_ENTER_9, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 24

    {KEY_VAL_BACKSPACE_1, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 25
    {KEY_VAL_BACKSPACE_2, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 26
    {KEY_VAL_BACKSPACE_3, KEY_SHORT_TIME, DOWN_CHK, FALSE}, // 27

};

const INT8U mRow[]=
{
    ROWA,
    ROWB,
    ROWC,
    ROWD,
};

const INT8U mCol[]=
{
    ROW0,
    ROW1,
    ROW2,
};

const INT8U mKeyPort[MAX_PD_KEY_NUM] = 
{
    ROW2,
};


#define ROW_MAX    (ARRAY_SIZE(mRow))
#define COL_MAX     (ARRAY_SIZE(mCol))

#define ROW_SCAN     (0)
#define COL_SCAN      (1)

static void KeyPadPortConfig(INT8U mode);
static INT16U KeypadScan(void);

#else

const ST_KEY_ID_INFO mstKeyIdInfo[MAX_KEY_ID] = 
{
    {KEY_VAL_DISP1, KEY_SHORT_TIME, DOWN_CHK, FALSE},
#ifdef THREE_PHASE
    {KEY_VAL_DISP2, KEY_SHORT_TIME, DOWN_CHK, FALSE},
#endif
    {KEY_VAL_DISP1, RLY_CLS_TIME, DOWN_CHK, FALSE},
#ifdef THREE_PHASE
    {KEY_VAL_DISP2, RLY_CLS_TIME, DOWN_CHK, FALSE},
    {KEY_VAL_COMB, FAC_CHECK_TIME, DOWN_CHK, FALSE},
    {KEY_VAL_DISP1, SW_MULT_FUNC_TIME,DOWN_CHK,FALSE},
#endif    

};

const INT8U mKeyPort[MAX_KEY_NUM] = 
{
    DISP_KEY,
#ifdef THREE_PHASE
    DISP2_KEY,
#endif
};

static INT16U KeyScan(void);

#endif

#define KEY_PRESS   (HIGH) /*"������״̬"*/
#define KEY_NONE    (LOW) /*"������״̬"*/

static void POKeyISR(void);
static void PDKeyISR(void);
static INT8U ChkCombKey(INT16U keyVal);



/*"************************************************************"*/
/*"Function:    KEY_Init"*/
/*"Description: ������ʼ��"*/
/*"Input:       Mode����ʼ��ģʽ"*/
/*"                 MCU_RUNMODE���ϵ�ģʽ"*/
/*"                 MCU_LPMODE_MID���͹���ģʽ"*/
/*"Output:      ��"*/
/*"Return:      ��"*/
/*"Others:           "*/
/*"************************************************************"*/
void KEY_Init(INT8U Mode)
{
    ST_GPIO_EXTICONF TmpExtiConf[MAX_PD_KEY_NUM];
    INT8U i;

    if (MCU_RUNMODE != Mode)
    {
#ifdef KEY_KEYPAD
        GPIO_ConfigPin(ROW2, INPUT, NULL_IN);/*"���ENTER��"*/
#endif
        
        for(i = 0; i < MAX_PD_KEY_NUM; i++)
        {
            GPIO_ConfigPin(mKeyPort[i], INPUT, NULL_IN);

            /*"�͹����°����������ж�"*/
            TmpExtiConf[i].PinId = mKeyPort[i];
            TmpExtiConf[i].TrigModule = GPIO_EXTI_EDGE_RISING;
            TmpExtiConf[i].IntFuction= PDKeyISR;
            GPIO_EXTI_Enable(&TmpExtiConf[i]);
        }

        LIB_MemSet(KEY_NONE, mKeyStaPD, MAX_PD_KEY_NUM);
    }
    else
    {
#ifdef KEY_KEYPAD
        KeyPadPortConfig(ROW_SCAN);

        mList = KEY_ID_INVALID;
        LIB_MemSet(KEY_ID_INVALID, mKeyIdBuff, KEY_ID_MAX);
        mRdIndex = 0;
        mKeyIdCnt = 0;

#else
        for(i = 0; i < MAX_KEY_NUM; i++)
        {
            GPIO_ConfigPin(mKeyPort[i], INPUT, NULL_IN);

            TmpExtiConf[i].PinId = mKeyPort[i];
            TmpExtiConf[i].TrigModule = GPIO_EXTI_EDGE_RISING;
            TmpExtiConf[i].IntFuction= POKeyISR;
            GPIO_EXTI_Enable(&TmpExtiConf[i]);
        }
		GPIO_ConfigPin(KEY_VCC,OUTPUT,NORMAL_OUT);
		GPIO_OutputPin(KEY_VCC, HIGH);

        LIB_MemSet(KEY_ID_INVALID, mList, KEYVAL_MAX_ID);
        mKeyId = KEY_ID_INVALID;
#endif
        mCurKeyVal = 0;
        mOutKeyVal = 0;
        mKeyTick = 0;
        mLastKeyID = KEY_ID_INVALID;
        mstKeyUp.keyID = KEY_ID_INVALID;
        mstKeyUp.combFlg = NO_KEY;
        mCnt = 0;
        mKeyFlg.byte = 0;
     }
}

#ifdef KEY_KEYPAD
/*"************************************************************"*/
/*"Function:    KeyPadPortConfig"*/
/*"Description: ����ɨ��˿�����"*/
/*"Input:       Mode��ɨ��ģʽ"*/
/*"                 ROW_SCAN����ɨ��ֱ��õͣ���ȡ�����ŵ�ֵ"*/
/*"                 COL_SCAN����ɨ��ֱ��õͣ���ȡ�����ŵ�ֵ"*/
/*"Output:      ��"*/
/*"Return:      ��"*/
/*"Others:           "*/
/*"************************************************************"*/
static void KeyPadPortConfig(INT8U mode)
{
    INT8U i, outMax, inMax;
    const INT8U *pOut, *pIn;

    if(COL_SCAN == mode)/*"��ɨ��"*/
    {
        outMax = COL_MAX;
        pOut = &mCol[0];
        inMax = ROW_MAX;
        pIn = &mRow[0];
    }
    else/*"��ɨ��"*/
    {
        outMax = ROW_MAX;
        pOut = &mRow[0];
        inMax = COL_MAX;
        pIn = &mCol[0];
    }

    for(i = 0; i < outMax; i++)
    {
        GPIO_OutputPin(pOut[i], HIGH);
        GPIO_ConfigPin(pOut[i], OUTPUT, NORMAL_OUT);
    }
    for(i = 0; i < inMax; i++)
    {
        GPIO_ConfigPin(pIn[i], INPUT, NULL_IN); 
    }
}

/*"************************************************************"*/
/*"Function:    KeypadScan"*/
/*"Description: ���̱���ɨ����"*/
/*"Input:       ��"*/
/*"Output:      INT16U: ����ɨ�����"*/
/*"Return:      ��"*/
/*"Others:           "*/
/*"************************************************************"*/
static INT16U KeypadScan(void)
{
    INT16U keypadVal;
    INT8U rowNum, colNum;

    keypadVal = 0;

    KeyPadPortConfig(ROW_SCAN);

    for(rowNum = 0; rowNum < ROW_MAX; rowNum++)   
    {
        GPIO_OutputPin(mRow[rowNum], LOW);/*"��ɨ��ֱ��õ�"*/

        for(colNum = 0; colNum < COL_MAX; colNum++)
        {
            if(LOW == GPIO_ReadPin(mCol[colNum]))/*"��ȡ�����ŵ�ֵ"*/
            {/*"�а�������"*/
                keypadVal |= (1 << (rowNum * COL_MAX + colNum));
            }
        }
        GPIO_OutputPin(mRow[rowNum], HIGH);
    }

    KeyPadPortConfig(COL_SCAN);
    
    for(colNum = 0; colNum < COL_MAX; colNum++)   
    {
        GPIO_OutputPin(mCol[colNum], LOW);

        for(rowNum = 0; rowNum < ROW_MAX; rowNum++)
        {
            if(LOW == GPIO_ReadPin(mRow[rowNum]))
            {
                keypadVal |= (1 << (rowNum * COL_MAX + colNum));
            }
        }
        GPIO_OutputPin(mCol[colNum], HIGH);
    }

    return keypadVal;
}

#else

/*"************************************************************"*/
/*"Function:    KeyScan"*/
/*"Description: �Ǽ��̱���ɨ����(��ʾ����̼���) "*/
/*"Input:       ��"*/
/*"Output:      INT16U: ����ɨ�����"*/
/*"Return:      ��"*/
/*"Others:           "*/
/*"************************************************************"*/
static INT16U KeyScan(void)
{
    INT16U keyVal;
    INT8U i;

    keyVal = 0;
    for(i = 0; i < MAX_KEY_NUM; i++)
    {
        if(KEY_PRESS == GPIO_ReadPin(mKeyPort[i]))
        {
            keyVal |= (1 << i);
        }
    }

    return keyVal;
}

#endif

static void PDKeyISR(void)/*"�͹����°����жϴ�������"*/
{
    INT8U i;
    
    for(i = 0; i < MAX_PD_KEY_NUM; i++)
    {
        if(KEY_PRESS == GPIO_ReadPin(mKeyPort[i]))
        {
            mKeyStaPD[i] = KEY_PRESS;
        }
    }
}

extern void PushKeyMsg(INT8U key);
static void POKeyISR(void)/*"�����жϴ�������"*/
{
    INT8U i;
    
    for(i = 0; i < MAX_PD_KEY_NUM; i++)
    {
        if(KEY_PRESS == GPIO_ReadPin(mKeyPort[i]))
        {
            PushKeyMsg(0x01);
        }
    }
}



/*"************************************************************"*/
/*"Function:    ChkCombKey"*/
/*"Description: ��⵱ǰ��ֵ�Ƿ���ϼ�"*/
/*"Input:        ��"*/
/*"Output:      ��"*/
/*"Return:      COMB_KEY / SGL_KEY / NO_KEY "*/
/*"Others:           "*/
/*"************************************************************"*/
static INT8U ChkCombKey(INT16U keyVal)
{
    INT8U i, result;

    result = NO_KEY;

    if(keyVal > 0)
    {
        for(i = 0; i < MAX_KEY_NUM; i++)
        {
            if(keyVal == (INT16U)(1 << i))
            {
                break;
            }
        }
        if(i >=  MAX_KEY_NUM)
        {
            result = COMB_KEY;
        }
        else
        {
            result = SGL_KEY;
        }
    }

    return result;
}

/*"************************************************************"*/
/*"Function:    KEY_Process"*/
/*"Description: ����������ת��Ϊ���ܰ���ID"*/
/*"Input:        ��"*/
/*"Output:      ��"*/
/*"Return:      ��"*/
/*"Others:           "*/
/*"************************************************************"*/
void KEY_Process (void)

{
    INT16U keyVal;
    INT16U chkTick;
    INT8U i, result;
#ifndef KEY_KEYPAD
    INT8U j;
#endif

    result = KEY_ID_INVALID;
    chkTick = KEY_SHORT_TIME;/*"Ĭ��(��������)���ʱ��������ʱ"*/

#ifdef KEY_KEYPAD
    keyVal = KeypadScan();
    //keyVal = KEY_GetKeyPD();//test
#else
    keyVal = KeyScan();
#endif

    if(keyVal != mCurKeyVal)
    {
        mCurKeyVal = keyVal;
#ifdef KEY_KEYPAD
        mKeyTick = 0;
#else
        mKeyTick = MCU_GetTick();
#endif
        mKeyFlg.bits.curr = ChkCombKey(mCurKeyVal);
    }
    else
    {
#ifdef KEY_KEYPAD
        mKeyTick++;
#endif

        for(i = 0; i < MAX_KEY_ID; i++)
        {
            if(mCurKeyVal == mstKeyIdInfo[i].keyIndex)
            {/*"�а����������б��м�ֵID����"*/
                chkTick = mstKeyIdInfo[i].time;/*"�������¼��ʱ�������ܼ�ֵ����ʱ��"*/
            }
            else if((0 == mCurKeyVal) && /*"��������"*/
            ((i == mstKeyUp.keyID) || (KEY_ID_INVALID != mLastKeyID))) /*"�ǵ������֮ǰ��Ч���ĵ���"*/
            {
            }
            else
            {
                continue;
            }

#ifdef KEY_KEYPAD
            if(mKeyTick >= chkTick)
#else
            if(MCU_CmpTick(mKeyTick, chkTick))
#endif
            {
                if(mCurKeyVal > 0)/*"��������"*/
                {
#ifdef KEY_KEYPAD
                    if((TRUE != mstKeyIdInfo[i].conFlg) && (i == mList))/*"��ǰ��ֵID�����"*/
                    {/*"��ǰ��ֵID���������1��"*/
                        continue;/*"��ǰ��ֵID�����ظ����"*/
                    }
#else
                    if((TRUE != mstKeyIdInfo[i].conFlg) && (mCnt > 0))
                    {/*"��ǰ��ֵID���������1��"*/
                        for(j = 0; j < mCnt; j++)
                        {
                            if(i == mList[j])/*"��ǰ��ֵID�����"*/
                            {
                                break;
                            }
                        }
                        if(j < mCnt)
                        {
                            continue;/*"��ǰ��ֵID�����ظ����"*/
                        }
                    }
#endif

                    if(DOWN_CHK == mstKeyIdInfo[i].chkSta)/*"�����¼��"*/
                    {
                        result = i;
                    }
                    else/*"��������"*/
                    {
                        if(0 == COMB_TO_SGL)
                        {/*"��ϼ������⣬��ϼ�ת����ʱ�����ԭ��ϼ�����״̬"*/
                            mstKeyUp.keyID = i;
                            mstKeyUp.combFlg = mKeyFlg.bits.curr;
                        }
                    }

                   /*"���̱����(����)"*/
               

                }
                else if(i == mstKeyUp.keyID)/*"�����������Ǽ�������"*/
                {
                    result = mstKeyUp.keyID;
                    mstKeyUp.keyID = KEY_ID_INVALID;
                }
                else
                {
                }

                if(COMB_TO_SGL)
                {/*"��ϼ�֮��ת����������ⵥ���İ��º͵���"*/
                    if(SGL_KEY == mstKeyUp.combFlg)
                    {
                        mstKeyUp.keyID = KEY_ID_INVALID;                    
                    }
                    result = KEY_ID_INVALID;
                }

                if(mOutKeyVal != mCurKeyVal)
                {
                    if(0 == mCurKeyVal)/*"ȫ����������"*/
                    {
                        mKeyFlg.bits.comb = 0;
                    }
                    else/*"��ǰ�м�����"*/
                    {
                        if((1 != mKeyFlg.bits.comb) && (COMB_KEY == mKeyFlg.bits.curr))
                        {
                            mKeyFlg.bits.comb = 1;
                        }

                        if(0 != mOutKeyVal)/*"��1�μ�״̬Ҳ���м�����"*/
                        {
                            if(COMB_TO_SGL && (DOWN_CHK == mstKeyIdInfo[i].chkSta))/*"��ϼ�ת�������������ϼ��ĵ����״̬"*/
                            {/*"������ϼ����ǰ��¼�⣬Ҫ���֮ǰ�ĵ��������״̬"*/
                                mstKeyUp.keyID = KEY_ID_INVALID;
                            }
                        }
                    }

#ifdef KEY_KEYPAD
                    mList = KEY_ID_INVALID;
#else
                    mCnt = 0;//170303
                    LIB_MemSet(KEY_ID_INVALID, mList, KEYVAL_MAX_ID);
#endif
                    mOutKeyVal = mCurKeyVal;
                }

                if(KEY_ID_INVALID != result)
                {/*"�����¼���������⣬result���ؼ�ֵ"*/
                    if(DOWN_CHK == mstKeyIdInfo[i].chkSta)
                    {
                        mLastKeyID = result;
                    }

#ifdef KEY_KEYPAD
                    if((mCurKeyVal > 0) && (KEY_ID_INVALID == mList))
                    {/*"��ǰ�ǰ������£����Ѳ�����ֵID"*/
                        mList = result;/*"�����µļ�ֵID"*/
                    }/*"mListȡ���б�ʽ��������ͬһ����������Ӧ��ͬ��ֵID���"*/
#else
                    if((mCurKeyVal > 0) && (mCnt < KEYVAL_MAX_ID))
                    {/*"��ǰ�ǰ������£����Ѳ�����ֵID"*/
                        for(j = 0; j < mCnt; j++)
                        {
                            if(mList[j] == result)/*"��ֵID�Ѵ����б�"*/
                            {/*"ͬһ�������������ܶ�Ӧ��ͬ��ֵID(��ͬ����ʱ)����Ҫ�б���"*/
                                break;
                            }
                        }
                        if(j >= mCnt)/*"�µļ�ֵID�����б�"*/
                        {
                            mList[mCnt] = result;
                            mCnt++;
                        }
                    }
#endif

                    break;
                }
                else if(0 == mCurKeyVal)
                {/*"��ǰ�ǰ������������1�μ�ֵID"*/
                    mLastKeyID = KEY_ID_INVALID;/*"�����¼��ļ��ڵ���ʱ������ʱ��������ֵID"*/
                    break;
                }
                else
                {
                }
            }
        }
    }

#ifdef KEY_KEYPAD
    mKeyIdBuff[mKeyIdCnt++] = result;
    if(mKeyIdCnt >= KEY_ID_MAX)
    {
        mKeyIdCnt = 0;
    }
#else
    mKeyId = result;
#endif
}

/*"************************************************************"*/
/*"Function:    KEY_GetKey"*/
/*"Description: ��ȡ���ܰ���ID"*/
/*"Input:        ��"*/
/*"Output:      ��"*/
/*"Return:      ��ֵID"*/
/*"Others:           "*/
/*"************************************************************"*/
INT8U KEY_GetKey (void)
{
    INT8U result;

#ifdef KEY_KEYPAD
    result = mKeyIdBuff[mRdIndex];    

    mKeyIdBuff[mRdIndex++] = KEY_ID_INVALID;
    if(mRdIndex >= KEY_ID_MAX)
    {
        mRdIndex = 0;
    }

#else
    KEY_Process();

    result = mKeyId;
    mKeyId = KEY_ID_INVALID;
#endif

    return result;    
}

/*"************************************************************"*/
/*"Function:    KEY_GetKeyPD"*/
/*"Description: �͹�������ʱ�������(���ENTER��)"*/
/*"Input:       ��"*/
/*"Output:     INT8U: ��������ID"*/
/*"Return:     ��"*/
/*"Others:           "*/
/*"************************************************************"*/
INT8U KEY_GetKeyPD(void)
{
    INT8U result, keyNum;
#ifdef KEY_KEYPAD
    INT8U i;
#endif

    result = KEY_ID_INVALID;

    for(keyNum = 0; keyNum < MAX_PD_KEY_NUM; keyNum++)
    {
        if(KEY_PRESS == mKeyStaPD[keyNum])
        {
            mKeyStaPD[keyNum] = KEY_NONE;

#ifdef KEY_KEYPAD
            for(i = 0; i < 4; i++)/*"�͹��İ�������40ms"*/
            {
                if(LOW != GPIO_ReadPin(ROW2))
                {
                    break;                
                }
                MCU_DelayMs(10);/*"10ms"*/
            }
            if(i >= 4)
            {
                result = KEY_ID_ENTER;
            }
#else/*"������򻯵͹��İ�����⣬���͹���20180123"*/
            if(KEY_NONE != GPIO_ReadPin(mKeyPort[keyNum]))
            {/*"������򻯵͹��İ�����⣬���͹���20180123"*/
                result = keyNum;
                break;
            }
#endif
        }
    }

    return result;
}

void KEY_Disable(INT8U KeyIndex)
{
#ifndef KEY_KEYPAD
    INT8U i;
#endif

#ifdef KEY_KEYPAD

    INT_Dis(E_INT_ID_INTP0);

    GPIO_OutputPin(ROW2, HIGH);
    GPIO_ConfigPin(ROW2, OUTPUT, NORMAL_OUT);

#else

    if(MAX_KEY_NUM == KeyIndex)
    {
        for(i = 0; i < KeyIndex; i++)
        {
            GPIO_ConfigPin(mKeyPort[i], INPUT, NULL_IN);
        }
    }
    else if(KeyIndex < MAX_KEY_NUM)
    {
        GPIO_ConfigPin(mKeyPort[KeyIndex], INPUT, NULL_IN);
    }
    else
    {
    }

#endif

}



