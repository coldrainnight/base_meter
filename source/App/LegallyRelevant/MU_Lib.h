#ifndef _MU_H_
#define _MU_H_

#include "App\LegallyRelevant\LRM_Config.h"

/*"----------------------------------"*/
/*"--------�궨��--------   ------------"*/
/*"----------------------------------"*/


/*"--------����ģ��汾      --------"*/
#define VERSION_NUM            20180507/*" �����汾��"*/

/*"--------����ģ������ͷ�ļ�         --------"*/
#define STATE_MODE                  1   /*"����ģʽ"*/
#define UNSTATE_MODE                0   /*"�ǹ���ģʽ"*/

/*"--------������--------"*/
#define MEASUREMODE_L               0   /*" ���߼���"*/
#define MEASUREMODE_LN1             1   /*" ���߼���+���߲���"*/
#define MEASUREMODE_LN2             2   /*" ˫��·����"*/

#define NOW_CHANNEL_E1  		    0	/*" ˫��·��������ͨ��"*/
#define NOW_CHANNEL_E2  		    1	/*" ˫��·��������ͨ��"*/


#define L_CHANNEL                   0   /*" ����"*/
#define N_CHANNEL                   1   /*" ����"*/

#define L_POWERP                    0   /*" �����й�����"*/
#define N_POWERP                    1   /*" �����й�����"*/
#define L_REAL_POWERP               2   /*" ����ԭʼ�й�����"*/
#define N_REAL_POWERP               3   /*" ����ԭʼ�й�����"*/
#define C_POWERQ                    4   /*" ����ͨ���޹�����"*/

#define AMEND_MODE                  0   /*" �޵�ģʽ 1�� 0�ر�"*/
#define CHIPCAL_MODE                0   /*" һ��У׼ģʽ 1�� 0�ر�"*/
#define LN2_MODE                    0   /*" ˫��·����ģʽ 1�� 0�ر�"*/


#define NORMAL_READ_MODE            0   /*"��ͨ�����ȡģʽ"*/
#define FAST_READ_MODE              1   /*"���ٴ����ȡģʽ"*/

#define	CT_WIDE_RANGE               1   /*" CT��Χ���� "*/
#define EDT_MODE                    1   /*" ���Ȳ�������"*/
#define OUTDATA_MODE                1   /*" ԭʼ�����������,���ڸ���ʶ����"*/

#define RE_PWR_EN                   1   /*" �޹�����"*/
#define VA_ANGLE_EN 				1

/*"--------ram reg ���ݳ��ȿ���--------"*/
#define RAM_DATA_TYPE              2
#define REG_DATA_TYPE              2

#define RAM_DATA_LENGTH8           sizeof(RAM_ADJ_DATA)/*" RAM_ADJ_DATA �ֽ���"*/
#define RAM_DATA_LENGTH            (RAM_DATA_LENGTH8/RAM_DATA_TYPE)
#define RAM_DATA_LENGTH16          (RAM_DATA_LENGTH8/2)/*" 21//RAM_ADJ_DATA ����"*/
#define RAM_DATA_LENGTH32          (RAM_DATA_LENGTH8/4)
#define REG_DATA_LENGTH            sizeof(reg_addr)/*" ram_data_length16-7//�Ĵ�������"*/

/*" 													"*/
#define EEP_PAR_IS_INIT        0x56/*" �ڿ������ѳ�ʼ��,��Ĭ�ϲ���,ram��ҲΪ��Ĭ�ϲ���"*/
#define EEP_PAR_IS_DEF         0xAB/*" �ڿ������ѳ�ʼ��,ram��ΪĬ�ϲ���"*/
#define EEP_PAR_NOT_INIT       0xAC/*" �ڿ�����δ��ʼ��"*/
#define NEED_INIT              0xAB/*" ����оƬ�踴λ��ʼ����־"*/
#define NOT_NEED_INIT          0x56/*" ����оƬ���踴λ��ʼ����־"*/
#define NEED_DELAY             0xAB
#define NOT_NEED_DELAY         0x56
#define ERROR                  0xAB
#define NO_ERROR               0x56
#define	NEED_RESET             0xAB
#define	NOT_NEED_RESET         0x56
#define EEP_RAM_TIME240        240/*" ��"*/
#define LOW_VOLTAGE            1275000
#define LOW_VOLTAGE_LIMIT      500000/*" ��͵�ѹ����"*/
#define CAL_STATUS_START       1
#define CAL_STATUS_STOP        0
#define FREQUENCE_CONST        44742500
#define CURRENT_GATE_BASE      960//5A������Ӧ�ĵ�����ֵ��16MA  Ϊ��ֹEMC ������� 3����96MA
#define POWER_GATE_BASE        104//5A������Ӧ�Ĺ��ʷ�ֵ

#define NEGA_STATUS            1/*" ���ʷ����� "*/
#define ACTI_STATUS            0/*" ���ʷ������� "*/
#define MEA_DIR_NUM            3/*" ���ʷ����ۼ���"*/

#define _MU_W_PARA              0x20
#define _MU_W_CMD               0x21
#define _MU_R_PARA              0x23
#define _MU_R_DATA              0x25
#define _MU_R_REG               0x26
#define _MU_W_AMEND             0x27
#define _MU_R_AMEND             0x28
#define _RU_W_PARA              0x29
#define _RU_R_PARA              0x30
#define _RU_W_CMD               0x31

/*"--------��������--------"*/
#define COMM_ERR_NOERR     0x00    /*"�޴���"*/
#define COMM_ERR_OTHERS    0x01    /*"��������"*/
#define COMM_ERR_NODATA    0x02    /*"����������"*/
#define COMM_ERR_PASSWORD  0x04    /*"Ȩ�޴���"*/

#define NO_INIT    __no_init
#define INIT_MODE  0
#define RUN_MODE   1

/*"--------�����������----"*/
#define RAM_REGION_A    0
#define EEP_REGION_A    1
#define EEP_REGION_B    2

/*"--------�����������EngyId--------"*/
#define L_ENGY_P        0x00/*"�����й�����"*/
#define N_ENGY_P        0x01/*"�����й�����"*/

#define L_ENGY_Q        0x10/*"�����޹�����"*/
#define N_ENGY_Q        0x11/*"�����޹�����"*/

#define L_ENGY_S        0x20/*"������������"*/
#define N_ENGY_S        0x21/*"������������"*/
/*"--------�����������EngyId--------"*/
#define A_ENGY_P        0x30/*"A���й�����"*/
#define B_ENGY_P        0x31/*"B���й�����"*/
#define C_ENGY_P        0x32/*"C���й�����"*/
#define T_ENGY_P        0x33/*"�����й�����"*/

#define A_ENGY_Q        0x40/*"A���޹�����"*/
#define B_ENGY_Q        0x41/*"B���޹�����"*/
#define C_ENGY_Q        0x42/*"C���޹�����"*/
#define T_ENGY_Q        0x43/*"�����޹�����"*/

#define A_ENGY_S        0x50/*"A����������"*/
#define B_ENGY_S        0x51/*"B����������"*/
#define C_ENGY_S        0x52/*"C����������"*/
#define T_ENGY_S        0x53/*"������������"*/

/*" Add 2019/04/26 "*/
/*"--------���߼�������д������о״̬��������--------"*/
/*"     register           address         R/W     length  reset_value     function"*/

#define	_05_W_I_AMPSTD	    0xC0	/*"     W       4	    -----       д�Լ�������·��ֵ�ο�"*/
#define	_05_W_I_PHSSTD	    0xC1	/*"     W       4	    -----       д�Լ�������·��λ�ο�"*/
#define	_05_W_U400_AMPSTD	0xC2	/*"     W       4	    -----       д�Լ���ѹ��·��ֵ�ο�"*/
#define	_05_W_U400_PHSSTD	0xC3	/*"     W       4	    -----       д�Լ���ѹ��·��λ�ο�"*/
#define	_05_W_U10_AMPSTD	0xC4	/*"     W       4	    -----       д�Լ���ѹ��·��ֵ�ο�1"*/
#define	_05_W_U10_PHSSTD	0xC5	/*"     W       4	    -----       д�Լ���ѹ��·��λ�ο�1"*/
#define	_05_W_U10_AMPSTD2	0xC6	/*"     W       4	    -----       д�Լ���ѹ��·��ֵ�ο�2"*/
#define	_05_W_U10_PHSSTD2	0xC7	/*"     W       4	    -----       д�Լ���ѹ��·��λ�ο�2"*/


#define	_05_R_I_AMPSTD	    0xC0	/*"     R       4	    -----       ���Լ�������·��ֵ�ο�"*/
#define	_05_R_I_PHSSTD	    0xC1	/*"     R       4	    -----       ���Լ�������·��λ�ο�"*/
#define	_05_R_U400_AMPSTD	0xC2	/*"     R       4	    -----       ���Լ���ѹ��·��ֵ�ο�"*/
#define	_05_R_U400_PHSSTD	0xC3	/*"     R       4	    -----       ���Լ���ѹ��·��λ�ο�"*/
#define	_05_R_U10_AMPSTD	0xC4	/*"     R       4	    -----       ���Լ���ѹ��·��ֵ�ο�1"*/
#define	_05_R_U10_PHSSTD	0xC5	/*"     R       4	    -----       ���Լ���ѹ��·��λ�ο�1"*/
#define	_05_R_U10_AMPSTD2	0xC6	/*"     R       4	    -----       ���Լ���ѹ��·��ֵ�ο�2"*/
#define	_05_R_U10_PHSSTD2	0xC7	/*"     R       4	    -----       ���Լ���ѹ��·��λ�ο�2"*/


#define	_05_R_I_AMP	        0xC0	/*"     R       4	    -----       ���Լ�������·��ֵ����ֵ"*/
#define	_05_R_I_PHS	        0xC1	/*"     R       4	    -----       ���Լ�������·��λ����ֵ"*/
#define	_05_R_U400_AMP   	0xC2	/*"     R       4	    -----       ���Լ���ѹ��·��ֵ����ֵ"*/
#define	_05_R_U400_PHS  	0xC3	/*"     R       4	    -----       ���Լ���ѹ��·��λ����ֵ"*/
#define	_05_R_U10_AMP	    0xC4	/*"     R       4	    -----       ���Լ���ѹ��·��ֵ����ֵ1"*/
#define	_05_R_U10_PHS	    0xC5	/*"     R       4	    -----       ���Լ���ѹ��·��λ����ֵ1"*/
#define	_05_R_U10_AMP2	    0xC6	/*"     R       4	    -----       ���Լ���ѹ��·��ֵ����ֵ2"*/
#define	_05_R_U10_PHS2	    0xC7	/*"     R       4	    -----       ���Լ���ѹ��·��λ����ֵ2"*/
#define	_05_R_I_AMP_ERR	    0xC8	/*"     R       4	    -----       ���Լ�������·��ֵ���ֵ"*/
#define	_05_R_I_PHS_ERR     0xC9	/*"     R       4	    -----       ���Լ�������·��λ���ֵ"*/
#define	_05_R_U400_AMP_ERR	0xCA	/*"     R       4	    -----       ���Լ���ѹ��·��ֵ���ֵ"*/
#define	_05_R_U400_PHS_ERR  0xCB	/*"     R       4	    -----       ���Լ���ѹ��·��λ���ֵ"*/
#define	_05_R_U10_AMP_ERR   0xCC	/*"     R       4	    -----       ���Լ���ѹ��·��ֵ���ֵ1"*/
#define	_05_R_U10_PHS_ERR   0xCD	/*"     R       4	    -----       ���Լ���ѹ��·��λ���ֵ1"*/
#define	_05_R_U10_AMP_ERR2  0xCE	/*"     R       4	    -----       ���Լ���ѹ��·��ֵ���ֵ2"*/
#define	_05_R_U10_PHS_ERR2  0xCF	/*"     R       4	    -----       ���Լ���ѹ��·��λ���ֵ2"*/
#define	_05_R_P_AMP_ERR	    0xF0	/*"     R       4	    -----       ���Լ�������·��ֵ���ֵ"*/
#define	_05_R_P_PHS_ERR     0xF1	/*"     R       4	    -----       ���Լ�������·��λ���ֵ"*/


#define	_05_R_READ_ERR_RECORD	        0x80	/*"     R       1	    -----       ����оƬ�����ݴ����¼������"*/
#define	_05_R_INIT_ERR_RECORD	        0x81	/*"     R       1	    -----       ����оƬ��ʼ�������¼������"*/
#define	_05_R_DELAYREAD_FLAG	    	0x82	/*"     R       1	    -----       ����оƬ��ʱ����־��"*/
#define	_05_R_INIT_FLAG				  	0x83	/*"     R       1	    -----       ����оƬ��ʼ����־��"*/
#define	_05_R_READ_ERR				    0x84	/*"     R       1	    -----       ����оƬ����־��"*/
#define	_05_R_RST_FLAG	  			    0x85	/*"     R       1	    -----       Ӳ����λ��־��"*/
#define	_05_R_DELAYREAD_RECORD		    0x86	/*"     R       1	    -----       ����оƬ��ʱ��������"*/
#define	_05_R_READ_EFFECT_TIMER		    0x87	/*"     R       1	    -----       ����оƬ����Чֵ��ʱ��"*/
#define	_05_R_READ_INTERVALERR_RECORD	0x88	/*"     R       1	    -----       ����оƬ����Ъ�����¼������"*/
#define	_05_R_PULSE_FLAG			    0x89	/*"     R       1	    -----       �����������"*/

/*"�����EDT������"*/
#define		EDT_PARA_NUM		6U
#define		EDT_STD_POS_IN_ST	34U
#define		EDT_ALLP_CMD_R_LEN	58U

/* ������ֵת�� */
#define     ADD_IF_SIGNED_VALUE           80000000
#define     IF_SIGNED_BIT_SET             0x80000000

#if HARMONIC_MODE
#define	nPhs			1
#define NUM_FFT 	    128 	// ˲ʱ���ݵ���
#define	nHarmonic	    MU_HARMONIC_NUM		// ����г������,0Ϊ����
#endif

/*"----------------------------------"*/
/*"--------���ݽṹ�����ݱ���--------"*/
/*"----------------------------------"*/
typedef enum
{
   #ifdef SINGLE_PHASE
   E_TEMP_LIN=0,    /*"���߽����¶�"*/
   E_TEMP_LOUT,     /*"���߳����¶�"*/
   E_TEMP_NIN,       /*"���߽����¶�"*/
   E_TEMP_NOUT,    /*"���߳����¶�"*/
   #else
   #endif
   E_TEMP_MAX
}E_TEMP_TYPE;

typedef struct
{
  INT16U  Ram_V_Coefficient;
  INT16U  Ram_Ia_Coefficient;
  INT16U  Ram_Pa_Coefficient;
  INT16U  Ram_GPQA;
  INT16U  Ram_PhsA;
  INT16U  Ram_APOSA;

  INT16U  Ram_Ib_Coefficient;
  INT16U  Ram_Pb_Coefficient;
  INT16U  Ram_GPQB;
  INT16U  Ram_PhsB;
  INT16U  Ram_APOSB;

  INT16U  Ram_hfconst;
  INT16U  Ram_PStart;
  INT16U  Ram_QStart;
  INT16U  Ram_MeterCfg;/*" ����������"*/
  INT16U  Ram_MeterCfg2;/*" ����������2"*/
  INT16U  Ram_ecconst;
}RAM_ADJ_DATA;/*" 	���ӡ�ɾ����Ա��Ҫ�޸Ķ�Ӧ�ڿ��ռ��С	"*/

typedef struct
{
    INT32U Voltage;         /*" ��ѹ"*/
    INT32U CurrentA;        /*" ���� "*/
    INT32U CurrentB;        /*" ���� B"*/
    INT32U PowerPA;         /*" ����"*/
    INT32U RealPowerPA;     /*" ������ʵ�й���ֵ(δ��Ǳ��)"*/
    INT32U PowerPB;         /*" ���߹���"*/
    INT32U RealPowerPB;     /*" ������ʵ�й���ֵ(δ��Ǳ��)"*/
    INT32U PowerQ;          /*" ��ǰͨ���޹�����"*/
    INT32U PowerQA;         /*" �����޹�����"*/
    INT32U PowerQB;         /*" �����޹�����"*/
    INT32U PowerSA;         /*" �������ڹ���"*/
    INT32U PowerSB;         /*" �������ڹ���"*/
    INT32U EnergyP;
    INT32U EnergyQ;
    INT32U EnergyS;
    INT32U Frequency;       /*" Ƶ��"*/
    INT32U FactorA;         /*" ��������"*/
    INT32U FactorB;         /*" ��������"*/
    INT32U DrA;             /*" ���ʷ���"*/
    INT32U DrB;             /*" ���ʷ���"*/
    INT32U DrqA;            /*" �޹����ʷ���"*/
    INT32U DrqB;            /*" �޹����ʷ���"*/
    INT32S gTmnlTmp[E_TEMP_MAX];  /*"�������¶�"*/
}ST_MU_INTERDATA;

typedef struct
{
  INT16U Voltage;       /*" ��ѹ"*/
  INT16U Current;       /*" ���� "*/
  INT16U Power;         /*" ����"*/
  INT8U Factor;         /*" ��������"*/
}AMEND_OUTDATA;

typedef struct
{
    INT32U I_Amp;
    INT32U I_Phs;
    INT32U U400_Amp;
    INT32U U400_Phs;
    INT32U U10_Amp;
    INT32U U10_Phs;
    INT32U U10_Amp2;
    INT32U U10_Phs2;
    INT32S I_Amp_Err;
    INT32S I_Phs_Err;
    INT32S U400_Amp_Err;
    INT32S U400_Phs_Err;
    INT32S U10_Amp_Err;
    INT32S U10_Phs_Err;
    INT32S U10_Amp_Err2;
    INT32S U10_Phs_Err2;
    INT32S I_Err;
    INT32S U_Err;
    INT32S P_AMP_Err;
    INT32S P_PHS_Err;
} ST_EDT_ERRDATA;

typedef struct 
{
    INT16U  HarmonicpercentU[nPhs][nHarmonic];
    INT16U	HarmonicpercentI[nPhs][nHarmonic];	//---г��������--NNN.N6
    INT32U	HarmonicUrms[nPhs];		// ����г����Чֵ
    INT32U	HarmonicIrms[nPhs];
}sHarmonicData_TypeDef;



/*"-----------˲ʱ�����ݽṹ�嶨��-----------"*/
typedef struct
{
    INT16U volt[MU_ELEMENT_NUM];/*"��ѹ"*/
    INT32S curr[MU_ELEMENT_NUM];/*"����"*/
    INT32S actPwr[MU_ELEMENT_NUM + 1];/*"�й�����"*/
    #if (RE_PWR_EN == 1)
    INT32S reactPwr[MU_ELEMENT_NUM + 1];/*"�޹�����"*/
    #endif
    #if (AP_PWR_EN == 1)
    INT32S apparentPwr[MU_ELEMENT_NUM + 1];/*"���ڹ���"*/
    #endif
    INT32U freq;/*"Ƶ��"*/
    INT32S factor[MU_ELEMENT_NUM + 1];/*"��������"*/
    INT32U calNcurr;/*"���ߵ���������ֵ��"*/
	INT32U residualCurr;/*"ʣ�����"*/
    #if (VOLT_ANGLE_EN == 1)
    INT32U VoltAngle[3];/*"��ѹ���"*/
    #endif
    #if (VA_ANGLE_EN == 1)
    INT32U VoltCurrAngle[4];/*"����������"*/
    #endif
    INT8U Quad[MU_ELEMENT_NUM + 1];/*"��ǰ����ֵ    0~3 I/II/III/IV����,   MU_ELEMENT_NUM + 1 ��ʾ��ABC "*/
    INT8U PowerDir;
    /*"***************************************************"*/
    /*"���ʷ���bPowerDir��bitλ����"*/
    /*"8��bit�ֱ��ʾTABC�й����޹��Ĺ��ʷ���bitֵΪ0��ʾ����Ϊ1��ʾ����
    enum
    {
    POWER_DIR_REACT_A,
    POWER_DIR_REACT_B,
    POWER_DIR_REACT_C,
    POWER_DIR_REACT_SUM,
    POWER_DIR_ACT_A,
    POWER_DIR_ACT_B,
    POWER_DIR_ACT_C,
    POWER_DIR_ACT_SUM
    };
    ***************************************************"*/
    INT32S gTmnlTmp[E_TEMP_MAX];  /*"�������¶�"*/
    #if (HARMONIC_MODE == 1)	
    INT16U HRVolt[MU_ELEMENT_NUM][nHarmonic];     /*��ѹг��,��λ0.01%,HRVolt[0][0]��ѹTHD*/
    INT16U HRCurr[MU_ELEMENT_NUM][nHarmonic];     /*����г��,��λ0.01%,HRCurr[0][0]����THD*/
    INT32U HRUrms[MU_ELEMENT_NUM];		          /*��ѹ��г����Чֵ0.0001V*/
	INT32U HRIrms[MU_ELEMENT_NUM];                /*������г����Чֵ0.0001A*/
    #endif
} ST_MMT_DATA;

typedef struct
{
    INT32U I_AmpStd;
    INT32U I_PhsStd;
    INT32U U400_AmpStd;
    INT32U U400_PhsStd;
    INT32U U10_AmpStd;
    INT32U U10_PhsStd;
    INT32U U10_AmpStd2;
    INT32U U10_PhsStd2;
} ST_EDT_STDDATA;


typedef struct
{              					 /*�����ӵ�У���� "*/
    INT32S Ib_80Un_1;        				 /*��80%un 5%IB  1.0�¾�������ֵ"*/
    INT32S Ib_80Un_5L;        				 /*��80%un 5%IB  1.0�¾�������ֵ"*/
    INT32S Ib_90Un_1;      					 /*��90%un 5%IB  1.0�¾�������ֵ"*/
    INT32S Ib_90Un_5L;       				 /*���������� "*/
    INT32S Ib_110Un_1;       				 /*��110%un 5%IB  1.0�¾�������ֵ"*/
    INT32S Ib_110Un_5L;       				 /*���������� "*/
    INT32S Ib_115Un_1;     					 /*��1150%un 5%IB  1.0�¾�������ֵ"*/
    INT32S Ib_115Un_5L;     				/*��1150%un 5%IB  1.0�¾�������ֵ"*/
    INT32S Up_Down_0_05Ib;     			/*���������� "*/
    INT32S Up_Down_Ib;     				/*���������� "*/
    INT32S Up_Down_Imax;     				 /*���������� "*/
    INT32S Imax_10min_1;     				 /*���������� "*/
    INT32S Imax_50min_1;    				 /*��IMAX��50���� 1.0�¾�������ֵ"*/
    INT32S Imax_50min_5L;     				 /*��IMAX   50��0.5L�¾�������ֵ "*/
    INT32S Imax_50min_8C;   				 /*��IMAX 50����0.8C�¾�������ֵ "*/
    INT32S Harmonic;						 /*��5��г���޵�ֵ "*/
}ST_AMEND_VALUE;

typedef struct{
    INT16U Amend_Imax_Time;				 /*��Imax�����ļ��ʱ�䣬Ϊ3200��xl,20110907 "*/
    INT8U Amend_Imax_LoseTime; 			 /*��Imax��������µ�����������ʱ�䣬Ϊ5����xl,20110907 "*/
    INT8U  Amend_Complete;				 /*��δ������ʱ��Ϊ0��������ʱ���������״̬Ϊ��ͬ��ֵ0x00-0x19  xl,20110907 "*/
    INT8U  Amend_Setchange;				 /*����У�����б仯ʱΪ1.�ޱ仯ʱΪ0   xl,20110907 "*/
    INT8U  Amend_Resume; 				 /*������������ʱΪ1��û���޸�����ʱΪ0    xl,20110907 "*/
    INT16U Amend_Judge_Timer;			 /*����������ж�ʱ��"*/
    INT8U Amend_UpDownIbFlg;			 /*���������Ib���жϱ�־"*/
    INT8U Amend_UpDownImaxFlg;			 /*���������Imax���жϱ�־"*/
    INT8U Amend_UpDownFlg;				 /*��������������޵���־"*/
    INT8U Amend_UpImax_Timer;			 /*���������Imax��������ʱ��"*/
    INT8U Amend_UpIb_Timer;				 /*���������Ib��������ʱ��"*/
    INT8U Amend_Up0_05Ib_Timer;			 /*���������5%Ib��������ʱ�� "*/
    INT16U Amend_Down_0_05Ib_Timer;	 /*��5%IB�����������ʵ�����ʱ��"*/
}ST_AMEND_TIMER;

typedef struct
{
  INT8U Vreflow;
  INT8U Nopld;
  INT8U Noqld;
  INT8U Revp;
  INT8U ChksumBusy;
  INT16U Chksum;
}ST_MU_EMUStatus;

typedef struct
{
  INT8U MeasureChip_Read_Error_Record;       /*"����оƬ�����ݴ����¼������"*/
  INT8U MeasureChip_Initialize_Error_Record;      /*"����оƬ��ʼ�������¼������"*/
  INT8U MeasureChip_DelayRead_Flag;      /*"����оƬ��ʱ����־��"*/
  INT8U MeasureChip_Initialize_Flag;       /*"����оƬ��ʼ����־��"*/
  INT8U MeasureChip_Read_Error;       /*"����оƬ����־��"*/
  INT8U MeasureChip_Reset_Flag;       /*"Ӳ����λ��־��"*/
  INT8U MeasureChip_DelayRead_Record;  /*"����оƬ��ʱ��������"*/
  INT8U Read_EffectValue_Timer;/*"����оƬ����Чֵ��ʱ��"*/
  INT8U MeasureChip_Read_IntervalError_Record;/*"����оƬ����Ъ�����¼������"*/
  INT8U MeasureChip_pluse_flag;
}ST_MU_FLAG;/*"�����������"*/

typedef struct
{
	INT16U MeaRun_State_Flag; /*"����оƬ����״̬�֣���ʾ"*/
	INT16U Measure_State_Flag; /*"��ǰ����״̬"*/
    INT16U MeaChannel_State_Flag;	/*"��ǰ����ͨ��״̬"*/
}ST_MU_STATE;

typedef struct
{
	INT16U Phase_measure_mode;/*"������������"*/
	INT16U Phase_i_base;/*"���������������"*/
	INT16U Phase_i_gate;/*"������Ǳ����ֵ������ "*/
    INT16U Phase_p_gate;
	INT16U Phase_iL_Nopld;/*"���߸���Ǳ����־��"*/
	INT16U Phase_iN_Nopld;/*"���߸���Ǳ����־��"*/
	INT16U Phase_Enable_AMEND;/*"�����޵�"*/
}ST_MU_MODE;

typedef struct
{
	INT16U Time_cnt1;/*"4���Ӽ�����"*/
  	INT16U Time_cnt2;/*"5���Ӽ�����"*/
	INT16U EMU_ERR_CNT;/*"EMUSTATUS�����������"*/
  	INT16U EMU_ERR_CNT1;/*"EMUSTATUS�ۼӺʹ��������"*/
	INT16U NEGA_Time;/*"�����ۼ�ʱ��"*/
    INT16U ACTI_Time;/*"�����ۼ�ʱ��"*/
    INT16U NEGAB_Time;/*"��ͨ�������ۼ�ʱ��"*/
    INT16U ACTIB_Time;/*"��ͨ�������ۼ�ʱ��"*/
}ST_MU_COUNT;

typedef struct
{
	INT8U Fatal_err_flag;/*"����д����оƬʧ��"*/
  	INT8U Fatal_err_flag1;/*"������λʧ��"*/
  	INT8U Fatal_err_flag2;/*"����������оƬУ���ʧ��   "*/
}ST_MU_ERR;

typedef struct
{
	INT32U VoltageReg_Data;
	INT32U CurrentReg_Data;
	INT32U CurrentBReg_Data;
	INT32U PowerReg_Data;
	INT32U PowerBReg_Data;
}ST_MU_REGDATA;

typedef struct
{
    INT16U TxDataInterval;
}
ST_OUT_DATA;               /*" ����ʶ�����"*/

typedef struct
{
    INT16U MC_DefPara_Flag;             /*"����оƬĬ�ϲ�����־"*/
    RAM_ADJ_DATA ram_adj_data;          /*" ����оƬ����"*/
    #if EDT_MODE
    ST_EDT_STDDATA edt_stddata;         /*" edt����"*/
    #endif
    #if AMEND_MODE
    ST_AMEND_VALUE AmendValue;          /*" �����޵�����"*/
    #endif
    #if OUTDATA_MODE
    ST_OUT_DATA out_data;               /*" ����ʶ�����"*/
    #endif
    INT16U CRC_Val;                     /*" CRCУ��"*/
}ST_MU_PARA;

typedef struct
{
	INT8U UnBalanceTimer1;						/*"ͨ����ƽ�������1"*/
	INT8U UnBalanceTimer2;						/*"ͨ����ƽ�������2"*/
	INT8U Init_Channel_Status;					/*"ͨ����ʼ��״̬"*/
	INT8U Init_Channel_flag;					/*"ͨ����ʼ����־"*/
	INT8U Low_Current_flag;						/*"ͨ��1��2������С��5%��־"*/
}ST_MU_LN2PARA;



/*"----------------------------------"*/
/*"--------�ⲿ����------   ------------"*/
/*"----------------------------------"*/

/*" MU����ӿڱ���"*/
extern ST_MU_PARA      gMU_Para;
extern ST_MU_FLAG      gMea_Flag;/*"����оƬ״̬��־λ"*/
extern ST_MMT_DATA     stMmtData;
extern ST_MU_STATE     gMea_State;/*"�ṩ��Ӧ�ò�ο�״̬"*/
#if EDT_MODE
extern ST_EDT_ERRDATA  gEDT_Errdata;/*"���߼������"*/
#endif


/*" MU����ӿں���"*/
extern void	MU_Init(INT8U mode);
extern void	MU_Main();
extern INT8U  MU_JustMtr(INT8U *pData);
extern INT8U MU_EngyGet(INT16U EngyId, INT32U *pData);
extern void MU_OpenPulse(void);
extern void  MU_ReadInstantData(INT16U reg);


#endif



