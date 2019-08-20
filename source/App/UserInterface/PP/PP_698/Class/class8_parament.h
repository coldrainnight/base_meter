/*"***********************************************************		
FileName: CLASS8_PARAMENT.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: �α�����ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS8_PARAMENT_H_
#define _CLASS8_PARAMENT_H_
/*"******************************************************"*/ 
#include "class_comm.h"
/*"-------------------------------------------------------"*/

/*8��������б�ֻ���Ϳ�д�б�*/
typedef struct
{
   INT16U OI;             /*OI����*/   
   INT8U  MAX_IID;        /*MAX_IID,���Ƿ��б�Ҫ����*/ 
   INT8U  Seq_Num;        /*�б����кţ�ͬʱҲ�ճ�4�ֽڶԽ�����Ҫ��Ϊ��CLASS8 ST_OAD_OBIS_INFO�б������*/

   const INT8U *Pointer;  /*����ָ��*/   
}ST_Parament_Object;    /*����Ԫ��*/

typedef struct
{
   INT16U OI;             /*OI����*/   
   INT8U  AID;            /*AIDֵ*/
   INT8U  Seq_Num;        /*�б����к�*/
   
   INT8U  MAX_IID;        /*MAX_IID,���Ƿ��б�Ҫ����*/
   INT8U  Right_RW;       /*��дȨ��,0:ֻ����FF:�ɶ�д*/
   //INT8U  Reser[2];
   const INT8U *Pointer;  /*����ָ��*/   
}ST_Parament_SEPECIAL_Object;    /*����Ԫ��*/

/*CLASS8��Ӧ�����к�*/
typedef enum
{
  E_SEQ_CUR_PAY_FILE   = 1,     /*(��ǰ��Ǯ���ļ� */
  E_SEQ_DATE_TIME,              /*����ʱ��  */
  E_SEQ_ADDR,                   /*ͨ�ŵ�ַ  */
  E_SEQ_METER_ID,               /*���*/
  E_SEQ_USERID,                 /*�ͻ����*/
  E_SEQ_DEVICE_ADDR,            /*�豸����λ��*/
  E_SEQ_GROUP_ADDR,             /*���ַ  */
  E_SEQ_CLK_SRC,                /*ʱ��Դ  */
  E_SEQ_LCD_PARA,               /*LCD����  */
  E_SEQ_YEAR_SLOT_SWITCH_TIME,  /*����ʱ�����л�ʱ��  */
  E_SEQ_DAILY_SLOT_SWITCH_TIME, /*������ʱ���л�ʱ��  */
  E_SEQ_TRIFF_SWITCH_TIME,      /*���׷�ʱ�����л�ʱ��  */
  E_SEQ_STEP_SWITCH_TIME,       /*���׽��ݵ���л�ʱ��  */
  E_SEQ_SLOT_NUM,               /*ʱ��ʱ����  */
  E_SEQ_STEP_NUM,               /*������  */
  //E_SEQ_PASS_NUM,               /*��Կ������  ������ں���ֻ���� */
  E_SEQ_METER_NUM,              /*����Ԫ����  */
  E_SEQ_HOLIDAY_NUM,            /*�������� ����������<=254 */
  E_SEQ_WEEK_TYPE,              /*������������  */
  E_SEQ_WEEK_SLOT_NUM,          /*��������õ���ʱ�α��  */

  E_SEQ_BAK_YEAR_SLOT_TABLE,    /*������ʱ����  ʱ������<=14*/
  E_SEQ_BAK_DAILY_SLOT_TABLE,   /*��������ʱ�α� ��ʱ�α�����q��8)*/
  E_SEQ_BAK_TRIFF_PRICE_TABLE,  /*�����׷��ʵ��  ��������k��63��*/
  E_SEQ_BAK_STEP_PRICE_TABLE,   /*�����׽��ݵ��  ������<=6  */
  E_SEQ_CT,                     /*�������������*/
  E_SEQ_PT,                     /*��ѹ���������*/ 
  E_SEQ_ALARM_MONEY_LIMIT,      /*���������ֵ  */
  E_SEQ_OTHER_MONEY_LIMIT,      /*���������ֵ  */
  //E_SEQ_ALARM_POWER_LIMIT,      /*����������ֵ  ������ֵ�ǲ��ǲ�����?*/
  //E_SEQ_OTHER_POWER_LIMIT,      /*����������ֵ  */
  E_SEQ_SAFE_TIME,              /*��ȫ��֤��Чʱ�� ����Э��ɾ����? */

  E_SEQ_PULSE_WIDTH,            /*У��������  */
  E_SEQ_MANAGE_CODE,            /*�ʲ��������  */
  E_SEQ_METER_RUN_TYPE,         /*���ܱ�����������1  */
  E_SEQ_ACTIVE_TYPE,            /*�й���Ϸ�ʽ������  */
  E_SEQ_BILL_DAY,               /*������       */
  E_SEQ_PASS,                   /*��֤����  �Ƚ����⣬����ֻ��д�����ܶ�     */
  E_SEQ_REMOTE,                 /*ң��        */

  E_SEQ_VOLT_RATE_PARA,         /*��ѹ�ϸ��ʲ���*/
  E_SEQ_MAX_DEMAND_TIME,        /*�����������*/
  E_SEQ_SKIP_TIME,              /*���ʻ���ʱ��  */
  E_SEQ_R1_ACTION_TYPE,         /*�޹���Ϸ�ʽ1������  */
  E_SEQ_R2_ACTION_TYPE,         /*�޹���Ϸ�ʽ2������  */
  E_SEQ_INTERVAL_DEMAND_TIME,   /*�ڼ�������������*/
  E_SEQ_SIGNLE,                 /*�ź�ǿ��      */
  
  
  #ifdef THREE_PHASE
  E_SEQ_DEMAND_TEMP_TIME ,      /*������ʱ���ݣ���ǰ��������������*/
  E_SEQ_DEMAND_DATA,            /*������ʱ���ݣ���ǰƽ������ֵ����*/
  E_SEQ_DEMAND_TEMP_VALUE,      /*������ʱ���ݣ������й���ʱ����*/
  #endif
 
  E_SEQ_SAFE_MODE_PARA,         /*��ȫģʽ����*/
  E_SEQ_FLOWER_REPORT_STATUS = 100,  /*���ܱ�����ϱ�״̬��*/
  E_SEQ_EVENT_START_TIME,       /*�¼�����ʱ��*/	
  E_SEQ_EVENT_END_TIME,         /*�¼�����ʱ��*/
  E_SEQ_FREEZE_TIME,            /*���ݶ���ʱ��*/
  E_SEQ_EVENT_RECORD_NUM,       /*�¼���¼���*/
  E_SEQ_FREEZE_RECORD_NUM,      /*�����¼���  */
  E_SEQ_EVENT_SRC_NUM,          /*�¼�����Դ  */
  E_SEQ_EVENT_CUR_NUM,          /*�¼���ǰֵ  */

  E_SEQ_EVENT_REPORT_STATUS,    /*�¼��ϱ�״̬ ͳһ��NULL��*/	
  E_SEQ_ADD_EVENT_REPORT_LIST,  /*�����¼��б� MAX_IID�Ǹ��仯��*/ 
  //E_SEQ_EVENT_REPORT_LIST,      /*���ϱ��¼��б�*/ 
  
  E_SEQ_CUR_YEAR_SLOT_TABLE,    /*��ǰ��ʱ����   ʱ������<=14 */
  E_SEQ_CUR_DAILY_SLOT_TABLE,   /*��ǰ����ʱ�α� ��ʱ�α�����q��8�� */
  E_SEQ_CUR_TRIFF_PRICE_TABLE,  /*��ǰ�׷��ʵ�� ��������k��63��*/
  E_SEQ_CUR_STEP_PRICE_TABLE,   /*��ǰ�׽��ݵ�� ������<=6 */

  E_SEQ_REGU_VOLT,              /*���ѹ  */
  E_SEQ_REGU_CURRENT,           /*�������������  */
  E_SEQ_MAX_CURRENT,            /*������  */
  E_SEQ_ACT_ACCU_GRADE,         /*�й�׼ȷ�ȵȼ�  */
  E_SEQ_ACT_CONST,              /*���ܱ��й�����  */
  E_SEQ_METER_TYPE,             /*���ܱ��ͺ�  */
  E_SEQ_SOFT_RECORD_ID,         /*���������  */
  E_SEQ_HOLD_POWER,             /*���� */	


  E_SEQ_R_ACT_ACCU_GRADE,       /*�޹�׼ȷ�ȵȼ�  */
  E_SEQ_R_ACT_CONST,            /*���ܱ��޹�����  */
  
  E_SEQ_FLOW_REPORT_MODE,       /*���ܱ�����ϱ�ģʽ��*/
  E_SEQ_EVENT_NEED_REPORT_LIST, /*���ϱ��¼������б�ֻ��*/
  E_SEQ_JUST_TIME_MODE,         /*4000 Уʱģʽ*/
  E_SEQ_ALARM_STATUS,           /*�澯״̬��ֻ��*/
  E_SEQ_CMD_STATUS,             /*����״̬��ֻ��*/
  E_SEQ_MAX_NO_COMM_TIME,       /*��������վ�����ͨ��ʱ�������ӣ���0��ʾ���Զ�����*/
  E_SEQ_AUTO_HOLD_TIME,         /*�ϵ��Զ�����ʱ�������ӣ���0��ʾ�ϵ粻�Զ�����*/
  E_SEQ_PRIO_SAFE_MODE_PARA,   /*��ʽ��ȫģʽ����*/  

  /*����Ϊ�Զ������*/   
  E_SEQ_DL_LENGTH,              /*��·�㳤��      */
  
  E_SEQ_METER_MODE,             /*����ģʽ��*/
  E_SEQ_RELAY_TYPE,             /*�̵�������*/
  E_SEQ_ALM_RELAY_OUT_MODE,     /*�����̵������ģʽ��*/
  E_SEQ_ALM_BL_MODE,            /*���ⱨ��ģʽ��*/  
  E_SEQ_WAI_RELAY_TIME,         /*���ü̵������巽ʽ��բά��ʱ�䣨��λ:ms��*/
  E_SEQ_WAI_RELAY_AGAIN,        /*���ü̵������巽ʽ�ٴ���բ��������λ:Wh��*/
  E_SEQ_SOFT_BAK_NUMBER ,       /*���������(�Զ���ı���)*/
  E_SEQ_EXTEND_MODE_1,          /*������չģʽ��1*/
  E_SEQ_EXTEND_MODE_2,          /*������չģʽ��2*/
  E_SEQ_EXTEND_MODE_3,          /*������չģʽ��3*/
  E_SEQ_EXTEND_MODE_4,          /*������չģʽ��4*/
  E_SEQ_EXTEND_MODE_5,          /*������չģʽ��5*/
  E_SEQ_EXTEND_MODE_6,          /*������չģʽ��6*/
  E_SEQ_EXTEND_MODE_7,          /*������չģʽ��7*/
  E_SEQ_EXTEND_MODE_8,          /*������չģʽ��8*/
  E_SEQ_EXTEND_MODE_9,          /*������չģʽ��9*/
  E_SEQ_EXTEND_MODE_10,         /*������չģʽ��10*/
  E_SEQ_DISP_VERSION,           /*�����ʾ�汾��*/        
  E_SEQ_SOFTWARE_VER,           /*����汾��(�Զ���)*/ 
  E_SEQ_PRODUCT_NUMBER_1,       /*����ʶ��Ψһ��1*/
  E_SEQ_PRODUCT_NUMBER_2,       /*����ʶ��Ψһ��2*/
  E_SEQ_PRODUCT_NUMBER_3,       /*����ʶ��Ψһ��3*/
  E_SEQ_CURRENT_TRIFF_NUM,      /*��ǰ���ʺ�*/
  E_SEQ_CURRENT_STEP_NUM,       /*��ǰ���ݺ�*/
  E_SEQ_STATUS_FACTOR,          /*���ܱ���״̬*/
  E_SEQ_TEST_RELAY,             /*�̵�����������*/
  E_SEQ_TEST_MULTI,             /*�๦�ܶ���Ͷ�в���*/
  E_SEQ_TEST_CHECK,             /*�Լ����*/
  E_SEQ_PARAMENT_CRC,           /*����CRCֵ */
  
  E_SEQ_SET_DEFAULT,            /*�ָ�Ĭ�ϲ��� */
  E_MAX_SEQ_PARAMENT            /*������к�*/

}EN_CLASS_8_SEQ_NUM;

/*"--------------------------------------------------------------"*/
/*�ⲿ�������������*/
extern INT8U Crypt_Type;


extern INT8U get_class8_object(SUB_OBJECT *pObject);
extern INT8S get_oi_right_type(INT8U ServiceType, SUB_OBJECT *pObject);
extern INT8U *Get_safe_mode(ST_OAD Oad, INT8U *pdata);
extern INT8U setLcdPara(INT8U *pdata);
extern INT8U GetLcdPara(INT8U *pdata);
extern INT8U get_class_parament_Obis(INT8U seq_num, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak,
                                    const ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_ser,INT8U list_num);
extern INT8U get_class8_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);
extern INT8U Action_safe_mode(INT8U AId,INT8U *Ioaddr,INT8U Iooffset);

/*"******************************************************"*/
#endif
