#ifndef  _KEY_H_
#define  _KEY_H_

#include "system\sys_def.h"


/*"(������)  ������Ӧ����:   ���̱�/ �Ǽ��̱�"*/
//#define KEY_KEYPAD  /*" ���̱�"*/
 


#define KEY_SHORT_TIME    (20)/*"����ʱ�������Ƚ�ʱ����20ms"*/

#ifdef KEY_KEYPAD
#define KEY_ID_MAX   (3)
#else
#define RLY_CLS_TIME     (3000)/*"��բ������ʱ3s"*/
#define SW_MULT_FUNC_TIME    (5000)/*"�л��๦������ڹ���"*/
//#define CLR_DU_TIME      (3000)/*"������������ʱ3s"*/
//#define TEST_MODE_TIME  (3000)/*"�л�������ģʽ��ʱ3s"*/
#endif
#ifdef THREE_PHASE
#define FAC_CHECK_TIME   (50)/*"50ms  ���ڼ��(�����Զ�������)"*/
#endif

#define UP_CHK           (0)/*"����������"*/
#define DOWN_CHK     (1)/*"�������¼��"*/

#define KEY_ID_INVALID (0xFF)/*"����Ч����"*/


typedef struct
{
    INT16U keyIndex;/*"��������ֵ"*/
    INT16U time;/*"��ֵ������ʱ��"*/
    INT8U   chkSta;/*"���״̬Ϊ�������"*/
    INT8U   conFlg;/*"�����ʱ�Ƿ��������"*/
}ST_KEY_ID_INFO;

typedef struct/*"����������Ĺ��ܼ�ֵ��Ϣ"*/
{
    INT8U keyID;/*"��ֵID"*/
    INT8U combFlg;/*"��ֵ����:SGL_KEY ������COMB_KEY ��ϼ�"*/
}ST_KEY_UP_INFO;

typedef union /*"��ǰ����״̬"*/
{
    struct
    {
        INT8U   curr: 2; /*"��ǰ״̬: 0_�ް���; 1_����; 2_��ϼ�"*/
        INT8U   comb:1;    /*"�Ѳ�����ϼ�"*/
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
    /*"����ID"*/
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

    /*"��ϼ�ID"*/
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
    KEY_ID_DISP1_RLY_CLS,  /*"3�밴����բ"*/
#ifdef THREE_PHASE
    KEY_ID_DISP2_RLY_CLS,  
    KEY_ID_FAC_CHECK,  
#endif
    KEY_ID_SW_MULT_FUNC,/*"�๦�ܿ��л�"*/
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

