/*"***********************************************************		
FileName: CLASS22_INPUT_OUTPUT.h		
Author: XYT       
Version : 1.0       Date: 2017.11	
Description: ���������ͷ�ļ�		 	
<author> <time> <version > <desc>		
***********************************************************"*/	
#ifndef _CLASS22_INPUT_OUTPUT_H_
#define _CLASS22_INPUT_OUTPUT_H_
/*"******************************************************"*/ 
/*"-------------------------------------------------------"*/
#define METHOD_CLASS22_127         127         /*22��ֻ�з���127��128����*/
#define METHOD_CLASS22_128         128     

typedef struct
{
   INT16U OI;             /*OI����*/   
   INT8U  MAX_IID;        /*MAX_IID,���Ƿ��б�Ҫ����*/ 
   INT8U  Seq_Num;        /*�б����кţ�ͬʱҲ�ճ�4�ֽڶԽ�����Ҫ��Ϊ��CLASS8 ST_OAD_OBIS_INFO�б������*/
   const INT8U *Pointer;  /*����ָ��*/   
}ST_Class22_ObjectList;    /*����Ԫ��*/


typedef struct
{ 
    INT8U  Seq_Num;   /*�б����к� ��ST_Class22_ObjectList�е�һ��*/
    INT8U  num;       /*"OAD��ӦOBIS��Ԫ�ظ���"*/
    INT16U Obis;      /*"�������Ķ���  <3000,����ֱ�Ӷ�ȡ����������OBIS�б�"*/

    INT8U  Offset;    /*"�����������ݵ�ƫ�� ���̶�Ϊ0���ȱ��������濴������پ����Ƿ�ȥ��"*/
    INT8U  FormatId;  /*"ͨ�š���ʾ��ʽID"*/
    INT8U  FunctionId;/*"��д����ID"*/
    INT8U  ShowinfoId;/*"��ʾ���ñ�ID"*/
}ST_Class22_OAD_OBIS_INFO;

/****************************************************************************/
/*������ 300bps��0����   600bps��1����     1200bps��2����2400bps��3����  4800bps��4����    7200bps��5����
9600bps��6����  19200bps��7����   38400bps��8����57600bps��9���� 115200bps��10���� ����Ӧ��255��*/
typedef struct
{
    INT8U commrate;    /*������ */
    INT8U check_bit;   /*У��λ  ENUMERATED {��У�飨0������У�飨1����żУ�飨2��*/
    INT8U data_bit;    /*����λ  ENUMERATED {5��5����6��6����7��7����8��8��*/
    INT8U stop_bit;    /*ֹͣλ  ENUMERATED {1��1����2��2��*/
    INT8U flow_ctr;    /*����    ENUMERATED {��(0)��Ӳ��(1)�����(2)*/
}STR_COMDCB;

#define PORT_DESCRIPT_LEN_6    6   /*���6���ֽ�*/
#define PORT_DESCRIPT_LEN_5    5   /*���5���ֽ�*/

/*RS232\485 ��������豸������ʶ ����*/
typedef struct
{
    ST_OAD    OAD;                /*OAD*/
    STR_COMDCB port_com_dcb;       /*�˿ڲ���   COMDCB*/
    INT8U      port_fun;           /*�˿ڹ���   enum{����ͨ�ţ�0��������1����������2����ͣ�ã�3��*/
}STR_RS_DEVICE_SET;


/*�̵������*/
#define MAX_BYTE_RELAY_DESCRIPT   1    /*���1���ֽ� 0,�ͼ칫��д����0���ֵֹ�*/
typedef struct
{
   INT8U relay_descript[MAX_BYTE_RELAY_DESCRIPT];     /*������ visible-string*/
   INT8U current_status;                              /*��ǰ״̬  enum{δ�����0���������1��*/
   INT8U method;                                      /* ��������  enum{����ʽ��0��������ʽ��1��*/
   INT8U connect_status;                              /*����״̬  enum{���루0����δ���루1)*/
}STR_RELAY_DEVICE;


/*"--------------------------------------------------------------"*/
/*�ⲿ�������������*/
extern INT8U get_class22_input_output_object(SUB_OBJECT *pObject);
extern INT8U get_class22_Obis(SUB_OBJECT *pObject, ST_PARAMENT_OAD_OBIS_INFO *pObisInfo_bak);
extern INT16S class22_input_output_get(ST_OAD Oad, INT8U *pdata);
extern INT8U class22_input_output_set(ST_OAD Oad, INT8U *pdata);
extern INT8U class22_input_output_action(ST_OAD Oad, INT8U *pdata);

/*"******************************************************"*/
#endif
