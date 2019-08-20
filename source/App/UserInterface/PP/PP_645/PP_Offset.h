/*"***********************************************************		
FileName: PP_Offset.h		
Author: ����       
Version : 1.0       Date: 2014.04.24	
Description: ���ں궨��Э������ �����е�Դ����ƫ�� 
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>			
***********************************************************"*/
#ifndef _PP_OFFSET_H
#define _PP_OFFSET_H

#include "App\UserInterface\PP\PP_645\PP_ProtocolTree.h"

#define TOTAL_OFFSET     (2)    /*"(��1)����й�~(��12)����й�"*/
#define POACT_OFFSET     (7)      /*"(��1)�����й�~(��12)����й�"*/
#define NEACT_OFFSET     (12)       /*"(��1)�����й�~(��12)����й�"*/
#define BILL_LAST_OFFSET (17)
/*"����ʱ��"*/
#define BILLTIME_OFFSET  (1)    /*"(��1~12)�� ����ʱ��"*/

/*"��1�¶�����й����ۼ��õ���"*/
#define MONBILL_POACT_OFFSET  (2)    /*"�¶Ƚ��������й�ƫ��"*/
#define MONBILL_NEACT_OFFSET  (3)    /*"�¶Ƚ��㷴���й�ƫ��"*/


/*"�����"*/
#define YEARBILL_TIME_OFFSET  (1)    /*"�����ʱ��"*/
#define YEARBILL_TOTAL_OFFSET  (2)    /*"������ۼ��õ���"*/



/*"�¼���¼����������-----����Դƫ��"*/

/*"���ڼ��п�ʼ���н������¼��������¼���Ҫ�ڶ����ʶǰ��0x80ƫ����ʾ����"*/
#define END_OFFSET  (0x80)

/*"�����¼�"*/
#define _PD_BEGIN_OFFSET     (1)        /*"���翪ʼʱ��ƫ��"*/
/*������¼���*/
#define _PD_BPO_OFFSET       (2)        /*"��ʼǰ�������"*/
#define _PD_BNE_OFFSET  (3)        /*"��ʼǰ�������"*/

#define _PD_END_OFFSET       (1)+END_OFFSET       /*"�������ʱ��ƫ��"*/
/*������¼���*/
#define _PD_EPO_OFFSET       (2)+END_OFFSET        /*"����ǰ�������"*/
#define _PD_ENE_OFFSET        (3)+END_OFFSET        /*"����ǰ�������"*/


/*"����¼�"*/
#define _PRG_YMD_OFFSET      (1)        /*"��̿�ʼ������ƫ��"*/
#define _PRG_HMS_OFFSET      (1)        /*"��̿�ʼʱ����ƫ��"*/
#define _PRG_OPER_OFFSET     (2)        /*"��̲����ߴ���ƫ��"*/
#define _PRG_ID_OFFSET       (3)        /*"���ǰ10�����ݴ���ƫ��"*/


/*"��������¼�"*/
#define _CLRMTR_YMD_OFFSET   (1)        /*"����������ƫ��"*/
#define _CLRMTR_HMS_OFFSET   (1)        /*"����ʱ����ƫ��"*/
#define _CLRMTR_OPER_OFFSET  (2)        /*"��������ߴ���ƫ��"*/
#define _CLRMTR_POACT_OFFSET (3)        /*"����������ƫ��"*/
#define _CLRMTR_NEACT_OFFSET (4)        /*"���㷴����ƫ��"*/
#define _CLRMTR_R_ENG_1_OFF    (5)                   /*"��ʼǰ�޹�1ƫ��"*/
#define _CLRMTR_R_ENG_2_OFF    (6)                   /*"��ʼǰ�޹�2ƫ��"*/
#define _CLRMTR_R_ENG_3_OFF    (7)                   /*"��ʼǰ�޹�3ƫ��"*/
#define _CLRMTR_R_ENG_4_OFF    (8)                   /*"��ʼǰ�޹�4ƫ��"*/


/*"�¼������¼�"*/
#define _CLREVT_TIME_OFFSET   (1)        /*"����ʱ��ƫ��"*/
#define _CLREVT_OPER_OFFSET   (2)        /*"��������ߴ���ƫ��"*/
#define _CLREVT_ID_OFFSET     (3)        /*"����IDƫ��"*/

/*"����Ԫ�����¼�"*/
#define _REPNONLEG_YMD_OFFSET   (1)        /*"�������������ƫ��"*/
#define _REPNONLEG_HMS_OFFSET   (1)        /*"�������ʱ����ƫ��"*/
#define _REPNONLEG_OPER_OFFSET  (2)        /*"������������ߴ���ƫ��"*/
#define _REPNONLEG_POACT_OFFSET (3)        /*"�������������ƫ��"*/
#define _REPNONLEG_NEACT_OFFSET (4)        /*"�������������ƫ��"*/
#define _REPNONLEG_R_ENG_1_OFF    (5)                   /*"��ʼǰ�޹�1ƫ��"*/
#define _REPNONLEG_R_ENG_2_OFF    (6)                   /*"��ʼǰ�޹�2ƫ��"*/
#define _REPNONLEG_R_ENG_3_OFF    (7)                   /*"��ʼǰ�޹�3ƫ��"*/
#define _REPNONLEG_R_ENG_4_OFF    (8)                   /*"��ʼǰ�޹�4ƫ��"*/

#define _REPNONLEG_EPO_OFFSET    (3+END_OFFSET)        /*"���������й�ƫ��"*/
#define _REPNONLEG_ENE_OFFSET    (4+END_OFFSET)        /*"���������й�ƫ��"*/
#define _REPNONLEG_E_R_ENG_1_OFF    (5+END_OFFSET)                   /*"�޹�1ƫ��"*/
#define _REPNONLEG_E_R_ENG_2_OFF    (6+END_OFFSET)                   /*"�޹�2ƫ��"*/
#define _REPNONLEG_E_R_ENG_3_OFF    (7+END_OFFSET)                   /*"�޹�3ƫ��"*/
#define _REPNONLEG_E_R_ENG_4_OFF    (8+END_OFFSET)                   /*"�޹�4ƫ��"*/

/*"����Ԫ�����¼�"*/
#define _UPDNONLEG_YMD_OFFSET   (1)        /*"������ƫ��"*/
#define _UPDNONLEG_HMS_OFFSET   (1)        /*"ʱ����ƫ��"*/
#define _UPDNONLEG_OPER_OFFSET  (2)        /*"�����ߴ���ƫ��"*/
#define _UPDNONLEG_POACT_OFFSET (3)        /*"������ƫ��"*/
#define _UPDNONLEG_NEACT_OFFSET (4)        /*"������ƫ��"*/
#define _UPDNONLEG_VER_OFFSET   (5)        /*" �汾��"*/

#define _UPDNONLEG_EPO_OFFSET    (3+END_OFFSET)        /*"���������й�ƫ��"*/
#define _UPDNONLEG_ENE_OFFSET    (4+END_OFFSET)        /*"���������й�ƫ��"*/
#define _UPDNONLEG_E_VER_OFFSET    (5+END_OFFSET)        /*" �汾��"*/

//#define _UPDNONLEG_E_R_ENG_1_OFF    (5+END_OFFSET)                   /*"�޹�1ƫ��"*/
//#define _UPDNONLEG_E_R_ENG_2_OFF    (6+END_OFFSET)                   /*"�޹�2ƫ��"*/
//#define _UPDNONLEG_E_R_ENG_3_OFF    (7+END_OFFSET)                   /*"�޹�3ƫ��"*/
//#define _UPDNONLEG_E_R_ENG_4_OFF    (8+END_OFFSET)                   /*"�޹�4ƫ��"*/


/*"����ͨ���쳣�¼�"*/
#define _MGMT_COMM_ERR_YMD_OFFSET   (1)        /*"������ƫ��"*/
#define _MGMT_COMM_ERR_HMS_OFFSET   (1)        /*"ʱ����ƫ��"*/
#define _MGMT_COMM_ERR_POACT_OFFSET (2)        /*"������ƫ��"*/
#define _MGMT_COMM_ERR_NEACT_OFFSET (3)        /*"������ƫ��"*/

#define _MGMT_COMM_ERR_EPO_OFFSET    (2+END_OFFSET)        /*"���������й�ƫ��"*/
#define _MGMT_COMM_ERR_ENE_OFFSET    (3+END_OFFSET)        /*"���������й�ƫ��"*/


/*"Уʱ�¼�"*/
#define _JUSTTIME_OPER_OFFSET   (1)      /*"�����ߴ���ƫ��ƫ��"*/
#define _JUSTTIME_BEFORE_OFFSET (2)      /*"Уʱǰʱ��ƫ��"*/
#define _JUSTTIME_AFTER_OFFSET  (1 + END_OFFSET)      /*"Уʱ��ʱ��ƫ��"*/

/*"��ʱ�α���"*/
#define _PRGDSLOT_TIME_OFFSET   (1)        /*"ʱ��ƫ��"*/
#define _PRGDSLOT_OPER_OFFSET   (2)        /*"�����ߴ���ƫ��"*/
#define _PRGDSLOT_TAB1_OFFSET   (3)        /*"�����ߴ���ƫ��"*/
#define _PRGDSLOT_TAB2_OFFSET   (11)        /*"�����ߴ���ƫ��"*/

/*"��ʱ������"*/
#define _PRGYSLOT_TIME_OFFSET   (1)        /*"ʱ��ƫ��"*/
#define _PRGYSLOT_OPER_OFFSET   (2)        /*"�����ߴ���ƫ��"*/
#define _PRGYSLOT_TAB1_OFFSET   (3)        /*"���ǰ��һ��ƫ��"*/
#define _PRGYSLOT_TAB2_OFFSET   (4)       /*"���ǰ�ڶ���ƫ��"*/

/*"�����ձ��"*/
#define _PRGWEEK_TIME_OFFSET   (1)        /*"ʱ��ƫ��"*/
#define _PRGWEEK_OPER_OFFSET   (2)        /*"�����ߴ���ƫ��"*/
#define _PRGWEEK_BEFORE_OFFSET (3)        /*"���ǰʱ�α�ƫ��"*/

/*"�й���Ϸ�ʽ���"*/
#define _PRGAEMODE_TIME_OFFSET   (1)        /*"ʱ��ƫ��"*/
#define _PRGAEMODE_OPER_OFFSET   (2)        /*"�����ߴ���ƫ��"*/
#define _PRGAEMODE_BEFORE_OFFSET (3)        /*"���ǰʱ�α�ƫ��"*/


/*"�����ձ��"*/
#define _PRGBILL_TIME_OFFSET   (1)        /*"ʱ��ƫ��"*/
#define _PRGBILL_OPER_OFFSET   (2)        /*"�����ߴ���ƫ��"*/
#define _PRGBILL_BEFORE1_OFFSET (3)        /*"���ǰ��һ������ƫ��"*/
#define _PRGBILL_BEFORE2_OFFSET (4)        /*"���ǰ�ڶ�������ƫ��"*/
#define _PRGBILL_BEFORE3_OFFSET (5)        /*"���ǰ����������ƫ��"*/


/*"����"*/
#define _OPENCVR_BYMD_OFFSET   (1)                   /*"��ʼ������ƫ��"*/
#define _OPENCVR_BHMS_OFFSET   (1)                   /*"��ʼʱ����ƫ��"*/
#define _OPENCVR_EYMD_OFFSET   (1+END_OFFSET)        /*"����������ƫ��"*/
#define _OPENCVR_EHMS_OFFSET   (1+END_OFFSET)        /*"����ʱ����ƫ��"*/
#define _OPENCVR_BPO_OFFSET    (2)                   /*"��ʼǰ�����й�ƫ��"*/
#define _OPENCVR_BNE_OFFSET    (3)                   /*"��ʼǰ�����й�ƫ��"*/
#define _OPENCVR_R_ENG_1_OFF    (4)                   /*"��ʼǰ�޹�1ƫ��"*/
#define _OPENCVR_R_ENG_2_OFF    (5)                   /*"��ʼǰ�޹�2ƫ��"*/
#define _OPENCVR_R_ENG_3_OFF    (6)                   /*"��ʼǰ�޹�3ƫ��"*/
#define _OPENCVR_R_ENG_4_OFF    (7)                   /*"��ʼǰ�޹�4ƫ��"*/

#define _OPENCVR_EPO_OFFSET    (2+END_OFFSET)        /*"���������й�ƫ��"*/
#define _OPENCVR_ENE_OFFSET    (3+END_OFFSET)        /*"���������й�ƫ��"*/
#define _OPENCVR_E_R_ENG_1_OFF    (4+END_OFFSET)                   /*"�޹�1ƫ��"*/
#define _OPENCVR_E_R_ENG_2_OFF    (5+END_OFFSET)                   /*"�޹�2ƫ��"*/
#define _OPENCVR_E_R_ENG_3_OFF    (6+END_OFFSET)                   /*"�޹�3ƫ��"*/
#define _OPENCVR_E_R_ENG_4_OFF    (7+END_OFFSET)                   /*"�޹�4ƫ��"*/

/*"���ʱ���"*/
#define _PRGTARIFF_TIME_OFFSET   (1)                        /*"ʱ��ƫ��"*/
#define _PRGTARIFF_OPER_OFFSET   (2)                        /*"�����ߴ���ƫ��"*/
#define _PRGTARIFF_TAB1_OFFSET   (3)                        /*"��1����ƫ��"*/
#define _PRGTARIFF_TAB2_OFFSET   (_PRGTARIFF_TAB1_OFFSET + _TARIFF_PRC_NUM)      /*"��2����ƫ��"*/


/*"���ݱ���"*/
#define _PRGSTEP_TIME_OFFSET   (1)         /*"ʱ��ƫ��"*/
#define _PRGSTEP_OPER_OFFSET   (2)         /*"�����ߴ���ƫ��"*/

#define _PRGSTEP_T11S1_OFFSET   (3)         /*"��1�׵�1���ݵ�1���ݴ���ƫ��"*/
#define _PRGSTEP_T11S2_OFFSET   (4)         /*"��1�׵�1���ݵ�2���ݴ���ƫ��"*/
#define _PRGSTEP_T11S3_OFFSET   (5)         /*"��1�׵�1���ݵ�3���ݴ���ƫ��"*/
#define _PRGSTEP_T11S4_OFFSET   (6)         /*"��1�׵�1���ݵ�4���ݴ���ƫ��"*/
#define _PRGSTEP_T11S5_OFFSET   (7)         /*"��1�׵�1���ݵ�5���ݴ���ƫ��"*/
#define _PRGSTEP_T11S6_OFFSET   (8)         /*"��1�׵�1���ݵ�6���ݴ���ƫ��"*/

#define _PRGSTEP_T11P1_OFFSET   (15)          /*"��1�׵�1���ݵ��1����ƫ��"*/
#define _PRGSTEP_T11P2_OFFSET   (16)         /*"��1�׵�1���ݵ��2����ƫ��"*/
#define _PRGSTEP_T11P3_OFFSET   (17)         /*"��1�׵�1���ݵ��3����ƫ��"*/
#define _PRGSTEP_T11P4_OFFSET   (18)         /*"��1�׵�1���ݵ��4����ƫ��"*/
#define _PRGSTEP_T11P5_OFFSET   (19)         /*"��1�׵�1���ݵ��5����ƫ��"*/
#define _PRGSTEP_T11P6_OFFSET   (20)         /*"��1�׵�1���ݵ��6����ƫ��"*/
#define _PRGSTEP_T11P7_OFFSET   (21)         /*"��1�׵�1���ݵ��7����ƫ��"*/

#define _PRGSTEP_T12S1_OFFSET   (9)         /*"��1�׵�2���ݵ�1���ݴ���ƫ��"*/
#define _PRGSTEP_T12S2_OFFSET   (10)        /*"��1�׵�2���ݵ�2���ݴ���ƫ��"*/
#define _PRGSTEP_T12S3_OFFSET   (11)        /*"��1�׵�2���ݵ�3���ݴ���ƫ��"*/
#define _PRGSTEP_T12S4_OFFSET   (12)        /*"��1�׵�2���ݵ�4���ݴ���ƫ��"*/
#define _PRGSTEP_T12S5_OFFSET   (13)        /*"��1�׵�2���ݵ�5���ݴ���ƫ��"*/
#define _PRGSTEP_T12S6_OFFSET   (14)        /*"��1�׵�2���ݵ�6���ݴ���ƫ��"*/

#define _PRGSTEP_T12P1_OFFSET   (22)          /*"��1�׵�2���ݵ��1����ƫ��"*/
#define _PRGSTEP_T12P2_OFFSET   (23)         /*"��1�׵�2���ݵ��2����ƫ��"*/
#define _PRGSTEP_T12P3_OFFSET   (24)         /*"��1�׵�2���ݵ��3����ƫ��"*/
#define _PRGSTEP_T12P4_OFFSET   (25)         /*"��1�׵�2���ݵ��4����ƫ��"*/
#define _PRGSTEP_T12P5_OFFSET   (26)         /*"��1�׵�2���ݵ��5����ƫ��"*/
#define _PRGSTEP_T12P6_OFFSET   (27)         /*"��1�׵�2���ݵ��6����ƫ��"*/
#define _PRGSTEP_T12P7_OFFSET   (28)         /*"��1�׵�2���ݵ��7����ƫ��"*/


#define _PRGSTEP_T1B1_OFFSET   (29)        /*"��1�׵�1�����մ���ƫ��"*/
#define _PRGSTEP_T1B2_OFFSET   (30)        /*"��1�׵�2�����մ���ƫ��"*/
#define _PRGSTEP_T1B3_OFFSET   (31)        /*"��1�׵�3�����յ�۴���ƫ��"*/
#define _PRGSTEP_T1B4_OFFSET   (32)        /*"��1�׵�4�����մ���ƫ��"*/
#define _PRGSTEP_T1B5_OFFSET   (33)        /*"��1�׵�5�����յ�۴���ƫ��"*/
#define _PRGSTEP_T1B6_OFFSET   (34)        /*"��1�׵�6�����մ���ƫ��"*/


#define _PRGSTEP_T21S1_OFFSET   (35)        /*"��2�׵�1����1����ƫ��"*/
#define _PRGSTEP_T21S2_OFFSET   (36)        /*"��2�׵�1����2����ƫ��"*/
#define _PRGSTEP_T21S3_OFFSET   (37)        /*"��2�׵�1����3����ƫ��"*/
#define _PRGSTEP_T21S4_OFFSET   (38)        /*"��2�׵�1����4����ƫ��"*/
#define _PRGSTEP_T21S5_OFFSET   (39)        /*"��2�׵�1����5����ƫ��"*/
#define _PRGSTEP_T21S6_OFFSET   (40)        /*"��2�׵�1����6����ƫ��"*/

#define _PRGSTEP_T21P1_OFFSET   (47)       /*"��2�׵�1���ݵ��1����ƫ��"*/
#define _PRGSTEP_T21P2_OFFSET   (48)        /*"��2�׵�1���ݵ��2����ƫ��"*/
#define _PRGSTEP_T21P3_OFFSET   (49)        /*"��2�׵�1���ݵ��3����ƫ��"*/
#define _PRGSTEP_T21P4_OFFSET   (50)        /*"��2�׵�1���ݵ��4����ƫ��"*/
#define _PRGSTEP_T21P5_OFFSET   (51)        /*"��2�׵�1���ݵ��5����ƫ��"*/
#define _PRGSTEP_T21P6_OFFSET   (52)        /*"��2�׵�1���ݵ��6����ƫ��"*/
#define _PRGSTEP_T21P7_OFFSET   (53)        /*"��2�׵�1���ݵ��7����ƫ��"*/

#define _PRGSTEP_T22S1_OFFSET   (41)        /*"��2�׵�2����1����ƫ��"*/
#define _PRGSTEP_T22S2_OFFSET   (42)        /*"��2�׵�2����2����ƫ��"*/
#define _PRGSTEP_T22S3_OFFSET   (43)        /*"��2�׵�2����3����ƫ��"*/
#define _PRGSTEP_T22S4_OFFSET   (44)        /*"��2�׵�2����4����ƫ��"*/
#define _PRGSTEP_T22S5_OFFSET   (45)        /*"��2�׵�2����5����ƫ��"*/
#define _PRGSTEP_T22S6_OFFSET   (46)        /*"��2�׵�2����6����ƫ��"*/

#define _PRGSTEP_T22P1_OFFSET   (54)       /*"��2�׵�1���ݵ�۴���ƫ��"*/
#define _PRGSTEP_T22P2_OFFSET   (55)        /*"��2�׵�2���ݵ�۴���ƫ��"*/
#define _PRGSTEP_T22P3_OFFSET   (56)        /*"��2�׵�3���ݵ�۴���ƫ��"*/
#define _PRGSTEP_T22P4_OFFSET   (57)        /*"��2�׵�4���ݵ�۴���ƫ��"*/
#define _PRGSTEP_T22P5_OFFSET   (58)        /*"��2�׵�5���ݵ�۴���ƫ��"*/
#define _PRGSTEP_T22P6_OFFSET   (59)        /*"��2�׵�6���ݵ�۴���ƫ��"*/
#define _PRGSTEP_T22P7_OFFSET   (60)        /*"��2�׵�7���ݵ�۴���ƫ��"*/

#define _PRGSTEP_T2B1_OFFSET   (61)        /*"��2�׵�1�����մ���ƫ��"*/
#define _PRGSTEP_T2B2_OFFSET   (62)        /*"��2�׵�2�����մ���ƫ��"*/
#define _PRGSTEP_T2B3_OFFSET   (63)        /*"��2�׵�3�����յ�۴���ƫ��"*/
#define _PRGSTEP_T2B4_OFFSET   (64)        /*"��2�׵�4�����մ���ƫ��"*/
#define _PRGSTEP_T2B5_OFFSET   (65)        /*"��2�׵�5�����մ���ƫ��"*/
#define _PRGSTEP_T2B6_OFFSET   (66)        /*"��2�׵�6�����մ���ƫ��"*/


/*"��Կ�����¼�"*/
#define _KEYUPDATE_TIME_OFFSET  (1)        /*"ʱ��ƫ��"*/
#define _KEYUPDATE_OPER_OFFSET  (2)        /*"�����ߴ���ƫ��"*/
#define _KEYUPDATE_NUM_OFFSET   (3)        /*"������������ƫ��"*/
#define _KEYUPDATE_STA_OFFSET   (4)        /*"����ǰ״̬ƫ��"*/

/*"�쳣�忨"*/
#define _CARDERR_TIME_OFFSET   (1)        /*"ʱ��ƫ��"*/
#define _CARDERR_SERNUM_OFFSET (2)        /*"������"*/
#define _CARDERR_INFO_OFFSET   (3)        /*"������Ϣ"*/
#define _CARDERR_HEAD_OFFSET   (4)        /*"��������ͷ"*/
#define _CARDERR_STA_OFFSET    (5)        /*"������Ӧ״̬"*/
#define _CARDERR_TIMES_OFFSET  (6)       /*"������"*/
#define _CARDERR_REM_OFFSET    (7)          /*"ʣ����"*/
#define _CARDERR_PO_OFFSET     (8)         /*"�����й���"*/
#define _CARDERR_NE_OFFSET     (9)        /*"�����й���"*/


/*"�˷��¼�"*/
#define _REFUND_TIME_OFFSET  (1)         /*"ʱ��ƫ��"*/
#define _REFUND_TIMES_OFFSET  (2)        /*"�˷�ǰ�������ƫ��"*/
#define _REFUND_MONEY_OFFSET  (3)        /*"�˷ѽ��ƫ��"*/
#define _REFUND_REMB_OFFSET  (4)        /*"�˷�ǰʣ����ƫ��"*/
#define _REFUND_REMA_OFFSET  (5)        /*"�˷Ѻ�ʣ����ƫ��"*/


/*"���ɿ�������"*/
#define _LOADERR_TIMEB_OFFSET  (1)                      /*"��ʼʱ��ƫ��"*/
#define _LOADERR_TIMEE_OFFSET  (1+END_OFFSET)           /*"����ʱ��ƫ��"*/
#define _LOADERR_STA_OFFSET    (2)                      /*"������״̬ƫ��"*/
#define _LOADERR_POB_OFFSET    (3)                      /*"��ʼǰ�����й�ƫ��"*/
#define _LOADERR_NEB_OFFSET    (4)                      /*"��ʼ�����й�ƫ��"*/
#define _LOADERR_POE_OFFSET    (2+END_OFFSET)           /*"����ʱ�����й�ƫ��"*/
#define _LOADERR_NEE_OFFSET    (3+END_OFFSET)           /*"����ʱ�����й�ƫ��"*/

/*"��Դ�쳣"*/
#define _PWRERR_TIMEB_OFFSET  (1)                   /*"��ʼʱ��ƫ��"*/
#define _PWRDERR_TIMEE_OFFSET  (1+END_OFFSET)       /*"����ʱ��ƫ��"*/
#define _PWRERR_PO_OFFSET  (2)                      /*"�����й�ƫ��"*/
#define _PWRERR_NE_OFFSET  (3)                      /*"�����й�ƫ��"*/

/*"�����¼�"*/
#define _BUYENGY_TIME_OFFSET  (1)         /*"ʱ��ƫ��"*/
#define _BUYENGY_TIMES_OFFSET  (2)         /*"������ܴ���ƫ��"*/
#define _BUYENGY_BUY_OFFSET  (3)         /*"������ƫ��"*/
#define _BUYENGY_REMB_OFFSET  (4)         /*"����ǰʣ��ƫ��"*/
#define _BUYENGY_REMA_OFFSET  (5)         /*"�����ʣ��ƫ��"*/
#define _BUYENGY_TOTAL_OFFSET  (6)         /*"�����ۼƹ�����ƫ��"*/

/*"��բ�¼�"*/
#define _TRIPSW_TIME_OFFSET  (1)         /*"ʱ��ƫ��"*/
#define _TRIPSW_OPER_OFFSET  (2)         /*"�����ߴ���ƫ��"*/
#define _TRIPSW_PO_OFFSET  (3)           /*"�����й���ƫ��"*/
#define _TRIPSW_NE_OFFSET  (4)         /*"�����й���ƫ��"*/


/*"��բ�¼�"*/
#define _CLSSW_TIME_OFFSET  (1)         /*"ʱ��ƫ��"*/
#define _CLSSW_OPER_OFFSET  (2)         /*"�����ߴ���ƫ��"*/
#define _CLSSW_PO_OFFSET  (3)           /*"�����й���ƫ��"*/
#define _CLSSW_NE_OFFSET  (4)         /*"�����й���ƫ��"*/


/*"�����¼�"*/
#define _OVERCURR_BTIME_OFFSET  (1)        /*"��ʼʱ��ƫ��"*/
#define _OVERCURR_BPO_OFFSET  (2)          /*"��ʼʱ�����й���ƫ��"*/
#define _OVERCURR_BNE_OFFSET  (3)          /*"��ʼʱ�����й���ƫ��"*/
#define _OVERCURR_BV_OFFSET  (4)           /*"��ʼʱ��ѹƫ��"*/
#define _OVERCURR_BCURR_OFFSET  (5)        /*"��ʼʱ����ƫ��"*/
#define _OVERCURR_BPW_OFFSET  (6)          /*"��ʼʱ����ƫ��"*/
#define _OVERCURR_BFACT_OFFSET  (7)          /*"��ʼʱ��������ƫ��"*/
#define _OVERCURR_ETIME_OFFSET  (1+END_OFFSET)        /*"����ʱ��ƫ��"*/
#define _OVERCURR_EPO_OFFSET  (2 +END_OFFSET)         /*"����ʱ�����й���ƫ��"*/
#define _OVERCURR_ENE_OFFSET  (3 +END_OFFSET)        /*"����ʱ�����й���ƫ��"*/



/*"��ʤ��չ�¼���¼"*/
#define _RESET_TIME_OFFSET  (1)         /*"ʱ��ƫ��"*/


/*"��������������----"*/

/*"��ʱ����"*/
#define TIMEFRZ_TIME_OFFSET   (1)    /*"ʱ��ƫ��"*/
#define TIMEFRZ_PO_OFFSET     (2)    /*"�����й�ƫ��"*/
#define TIMEFRZ_NE_OFFSET     (7)    /*"�����й�ƫ��"*/
#define TIMEFRZ_PW_OFFSET     (12)    /*"���й�����ƫ��"*/

/*"˲ʱ����"*/
#define MMTFRZ_TIME_OFFSET   TIMEFRZ_TIME_OFFSET   
#define MMTFRZ_PO_OFFSET     TIMEFRZ_PO_OFFSET                   
#define MMTFRZ_NE_OFFSET     TIMEFRZ_NE_OFFSET   
#define MMTFRZ_PW_OFFSET     TIMEFRZ_PW_OFFSET

    
/*"������ʱ�����л�����"*/
#define YSLOTSWFRZ_TIME_OFFSET   TIMEFRZ_TIME_OFFSET   
#define YSLOTSWFRZ_PO_OFFSET     TIMEFRZ_PO_OFFSET                   
#define YSLOTSWFRZ_NE_OFFSET     TIMEFRZ_NE_OFFSET   
#define YSLOTSWFRZ_PW_OFFSET     TIMEFRZ_PW_OFFSET

/*"������ʱ�α��л�����"*/
#define DSLOTSWFRZ_TIME_OFFSET   TIMEFRZ_TIME_OFFSET   
#define DSLOTSWFRZ_PO_OFFSET     TIMEFRZ_PO_OFFSET                   
#define DSLOTSWFRZ_NE_OFFSET     TIMEFRZ_NE_OFFSET   
#define DSLOTSWFRZ_PW_OFFSET     TIMEFRZ_PW_OFFSET

/*"���ڶ���"*/
#define PERIODFRZ_TIME_OFFSET   (1)   
#define PERIODFRZ_PO_OFFSET     (2)                   
#define PERIODFRZ_NE_OFFSET     (3)   

/*"��ʱ�����л�����"*/
#define TARIFFSWFRZ_TIME_OFFSET   TIMEFRZ_TIME_OFFSET   
#define TARIFFSWFRZ_PO_OFFSET     TIMEFRZ_PO_OFFSET                   
#define TARIFFSWFRZ_NE_OFFSET     TIMEFRZ_NE_OFFSET   
#define TARIFFSWFRZ_PW_OFFSET     TIMEFRZ_PW_OFFSET


/*"�ն���"*/
#define DAYFRZ_TIME_OFFSET   TIMEFRZ_TIME_OFFSET   
#define DAYFRZ_PO_OFFSET     TIMEFRZ_PO_OFFSET                   
#define DAYFRZ_NE_OFFSET     TIMEFRZ_NE_OFFSET   
#define DAYFRZ_PW_OFFSET     TIMEFRZ_PW_OFFSET

/*"���׽����л�����"*/
#define STEPSWFRZ_TIME_OFFSET   TIMEFRZ_TIME_OFFSET   
#define STEPSWFRZ_PO_OFFSET     TIMEFRZ_PO_OFFSET                   
#define STEPSWFRZ_NE_OFFSET     TIMEFRZ_NE_OFFSET   
#define STEPSWFRZ_PW_OFFSET     TIMEFRZ_PW_OFFSET


/*"ʣ�����ն���"*/
#define PREPAYFRZ_TIME_OFFSET    (1)
#define PREPAYFRZ_RM_OFFSET      (2)  /*"ʣ����"*/               
#define PREPAYFRZ_OD_OFFSET      (3)  /*"͸֧���"*/


#endif
