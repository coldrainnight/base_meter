#ifndef LRM_CONFIG_H
#define LRM_CONFIG_H

/*"--------�������ݿ��ƺ궨��--------"*/


//#define  QUAD_ACTIVE_ENGY_EN   0  /*"�й������޼���ʹ��"*/

#if defined (THREE_PHASE)
//#define REACTIVE_ENGY_EN  1    /*"�޹�����������ʹ��"*/
//#define APPARENT_ENGY_EN  0    /*"���ڵ���������ʹ��"*/
//#define PHASE_ENGY_EN     1    /*"�������������ʹ��"*/
//#define TARIFF_NUM 8
//#define PWR_DECIMAL_POINT   (-6)/*"����С��λ��"*/
//#define VOLT_DECIMAL_POINT   (-2)/*"��ѹС��λ��"*/
//#define CURR_DECIMAL_POINT   (-4)/*"����С��λ��"*/
//#define FREQ_DECIMAL_POINT   (-6)/*"Ƶ��С��λ��"*/
//#define FACTOR_DECIMAL_POINT (-6)/*"��������С��λ��"*/
//#define AMP_HOUR_EN            /*"��ʱֵ����ʹ�ܿ���"*/
#define HARM_DECIMAL_POINT   (-2)/*"г��������С��λ��"*/

#elif defined (SINGLE_PHASE)
#define REACTIVE_ENGY_EN  1
//#define APPARENT_ENGY_EN  0
#define TARIFF_NUM 0
//#define AMP_HOUR_EN          /*"��ʱֵ����ʹ�ܿ���"*/
#define PWR_DECIMAL_POINT   (-4)/*"����С��λ��"*/
#define VOLT_DECIMAL_POINT   (-1)/*"��ѹС��λ��"*/
#define CURR_DECIMAL_POINT   (-4)/*"����С��λ��"*/
#define FREQ_DECIMAL_POINT   (-4)/*"Ƶ��С��λ��"*/
#define FACTOR_DECIMAL_POINT (-4)/*"��������С��λ��"*/
#define HARM_DECIMAL_POINT   (-2)/*"г��������С��λ��"*/
#endif

#ifdef  IEC62056
#if (REACTIVE_ENGY_EN == 1)
//#define  REACTIVE_ENGY_COM
#endif
#endif

/*"----˲ʱ������ ���ƺ궨��------"*/
#if defined (SINGLE_PHASE)
#define MU_ELEMENT_NUM  1       /*"��������Ԫ��"*/
#elif defined (THREE_PHASE)
#define MU_ELEMENT_NUM  3       /*"��������Ԫ��"*/
#endif

#define HARMONIC_MODE           1       /*" г�����ܿ���"*/

#define MU_HARMONIC_NUM         15      /*"г����������"*/

//#define  ENGY_REGISTER_ACTIVE_FUN   /*"��ʱ���ʵ����Ĵ�������ܺ꿪��"*/
#endif
