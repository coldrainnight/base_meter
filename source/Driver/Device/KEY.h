#ifndef  _KEY_H_
#define  _KEY_H_

#include "system\sys_def.h"


/*"(配置项)  按键对应表型:   键盘表/ 非键盘表"*/
//#define KEY_KEYPAD  /*" 键盘表"*/
 


#define KEY_SHORT_TIME    (20)/*"短延时按键检测比较时基差20ms"*/

#ifdef KEY_KEYPAD
#define KEY_ID_MAX   (3)
#else
#define RLY_CLS_TIME     (3000)/*"合闸允许延时3s"*/
#define SW_MULT_FUNC_TIME    (5000)/*"切换多功能输出口功能"*/
//#define CLR_DU_TIME      (3000)/*"按键清需量延时3s"*/
//#define TEST_MODE_TIME  (3000)/*"切换至测试模式延时3s"*/
#endif
#ifdef THREE_PHASE
#define FAC_CHECK_TIME   (50)/*"50ms  厂内检测(用于自动化生产)"*/
#endif

#define UP_CHK           (0)/*"按键弹起检测"*/
#define DOWN_CHK     (1)/*"按键按下检测"*/

#define KEY_ID_INVALID (0xFF)/*"无有效按键"*/


typedef struct
{
    INT16U keyIndex;/*"按键输入值"*/
    INT16U time;/*"键值检测持续时间"*/
    INT8U   chkSta;/*"检测状态为弹起或按下"*/
    INT8U   conFlg;/*"键检出时是否持续返回"*/
}ST_KEY_ID_INFO;

typedef struct/*"按键弹起检测的功能键值信息"*/
{
    INT8U keyID;/*"键值ID"*/
    INT8U combFlg;/*"键值属性:SGL_KEY 单键，COMB_KEY 组合键"*/
}ST_KEY_UP_INFO;

typedef union /*"当前按键状态"*/
{
    struct
    {
        INT8U   curr: 2; /*"当前状态: 0_无按键; 1_单键; 2_组合键"*/
        INT8U   comb:1;    /*"已产生组合键"*/
    }bits;
    INT8U   byte;
}UN_KEY_FLAG;


#ifdef KEY_KEYPAD
typedef enum 
{
    KEY_NUM_1 = 0,
    KEY_NUM_2,
    KEY_NUM_3,
    KEY_NUM_4,
    KEY_NUM_5,
    KEY_NUM_6,
    KEY_NUM_7,
    KEY_NUM_8,
    KEY_NUM_9,
    KEY_BCAKSPACE,
    KEY_NUM_0,
    KEY_ENTER,

    MAX_KEY_NUM
}EN_KEY_INDEX;


typedef enum 
{
    /*"单键ID"*/
    KEY_ID_NUM_0 = 0,
    KEY_ID_NUM_1,
    KEY_ID_NUM_2,
    KEY_ID_NUM_3,
    KEY_ID_NUM_4,
    KEY_ID_NUM_5,
    KEY_ID_NUM_6,
    KEY_ID_NUM_7,
    KEY_ID_NUM_8,
    KEY_ID_NUM_9,
    KEY_ID_BCAKSPACE,
    KEY_ID_ENTER,

    //KEY_ID_ENTER_T1,
    //KEY_ID_ENTER_T2,
    //KEY_ID_ENTER_T3,

    /*"组合键ID"*/
    KEY_ID_ENTER_0,
    KEY_ID_ENTER_1,
    KEY_ID_ENTER_2,
    KEY_ID_ENTER_3,
    KEY_ID_ENTER_4,
    KEY_ID_ENTER_5,
    KEY_ID_ENTER_6,
    KEY_ID_ENTER_7,
    KEY_ID_ENTER_8,
    KEY_ID_ENTER_9,
    KEY_ID_BACKSPACE_1,
    KEY_ID_BACKSPACE_2,
    KEY_ID_BACKSPACE_3,

    MAX_KEY_ID
}EN_KEY_ID;

#define KEY_VAL_NUM_0   (1 << KEY_NUM_0)
#define KEY_VAL_NUM_1   (1 << KEY_NUM_1)
#define KEY_VAL_NUM_2   (1 << KEY_NUM_2)
#define KEY_VAL_NUM_3   (1 << KEY_NUM_3)
#define KEY_VAL_NUM_4   (1 << KEY_NUM_4)
#define KEY_VAL_NUM_5   (1 << KEY_NUM_5)
#define KEY_VAL_NUM_6   (1 << KEY_NUM_6)
#define KEY_VAL_NUM_7   (1 << KEY_NUM_7)
#define KEY_VAL_NUM_8   (1 << KEY_NUM_8)
#define KEY_VAL_NUM_9   (1 << KEY_NUM_9)
#define KEY_VAL_BACKSPACE  (1 << KEY_BCAKSPACE)
#define KEY_VAL_ENTER   (1 << KEY_ENTER)

#define KEY_VAL_ENTER_0   (KEY_VAL_ENTER | KEY_VAL_NUM_0)
#define KEY_VAL_ENTER_1   (KEY_VAL_ENTER | KEY_VAL_NUM_1)
#define KEY_VAL_ENTER_2   (KEY_VAL_ENTER | KEY_VAL_NUM_2)
#define KEY_VAL_ENTER_3   (KEY_VAL_ENTER | KEY_VAL_NUM_3)
#define KEY_VAL_ENTER_4   (KEY_VAL_ENTER | KEY_VAL_NUM_4)
#define KEY_VAL_ENTER_5   (KEY_VAL_ENTER | KEY_VAL_NUM_5)
#define KEY_VAL_ENTER_6   (KEY_VAL_ENTER | KEY_VAL_NUM_6)
#define KEY_VAL_ENTER_7   (KEY_VAL_ENTER | KEY_VAL_NUM_7)
#define KEY_VAL_ENTER_8   (KEY_VAL_ENTER | KEY_VAL_NUM_8)
#define KEY_VAL_ENTER_9   (KEY_VAL_ENTER | KEY_VAL_NUM_9)

#define KEY_VAL_BACKSPACE_1   (KEY_VAL_BACKSPACE | KEY_VAL_NUM_1)
#define KEY_VAL_BACKSPACE_2   (KEY_VAL_BACKSPACE | KEY_VAL_NUM_2)
#define KEY_VAL_BACKSPACE_3   (KEY_VAL_BACKSPACE | KEY_VAL_NUM_3)

#else

typedef enum 
{
    KEY_DISP1 = 0,
#ifdef THREE_PHASE
    KEY_DISP2,
#endif

    MAX_KEY_NUM
}EN_KEY_INDEX;

typedef enum 
{
    KEY_ID_DISP1 = 0,
#ifdef THREE_PHASE
    KEY_ID_DISP2,
#endif
    KEY_ID_DISP1_RLY_CLS,  /*"3秒按键合闸"*/
#ifdef THREE_PHASE
    KEY_ID_DISP2_RLY_CLS,  
    KEY_ID_FAC_CHECK,  
#endif
    KEY_ID_SW_MULT_FUNC,/*"多功能口切换"*/
    MAX_KEY_ID
}EN_KEY_ID;

#define KEY_VAL_DISP1   (1 << KEY_DISP1)
#ifdef THREE_PHASE
#define KEY_VAL_DISP2   (1 << KEY_DISP2)
#define KEY_VAL_COMB   (KEY_VAL_DISP1 | KEY_VAL_DISP2)
#endif

#endif

extern void KEY_Init(INT8U Mode);
extern INT8U KEY_GetKey(void);
extern INT8U KEY_GetKeyPD(void);
extern void KEY_Disable(INT8U KeyIndex);

#endif

