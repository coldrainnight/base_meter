#ifndef _MU_H_
#define _MU_H_

#include "App\LegallyRelevant\LRM_Config.h"

/*"----------------------------------"*/
/*"--------宏定义--------   ------------"*/
/*"----------------------------------"*/


/*"--------计量模块版本      --------"*/
#define VERSION_NUM            20180507/*" 计量版本号"*/

/*"--------计量模块驱动头文件         --------"*/
#define STATE_MODE                  1   /*"国网模式"*/
#define UNSTATE_MODE                0   /*"非国网模式"*/

/*"--------计量宏--------"*/
#define MEASUREMODE_L               0   /*" 火线计量"*/
#define MEASUREMODE_LN1             1   /*" 火线计量+零线测量"*/
#define MEASUREMODE_LN2             2   /*" 双回路计量"*/

#define NOW_CHANNEL_E1  		    0	/*" 双回路计量火线通道"*/
#define NOW_CHANNEL_E2  		    1	/*" 双回路计量零线通道"*/


#define L_CHANNEL                   0   /*" 火线"*/
#define N_CHANNEL                   1   /*" 零线"*/

#define L_POWERP                    0   /*" 火线有功功率"*/
#define N_POWERP                    1   /*" 零线有功功率"*/
#define L_REAL_POWERP               2   /*" 火线原始有功功率"*/
#define N_REAL_POWERP               3   /*" 零线原始有功功率"*/
#define C_POWERQ                    4   /*" 计量通道无功功率"*/

#define AMEND_MODE                  0   /*" 修调模式 1打开 0关闭"*/
#define CHIPCAL_MODE                0   /*" 一键校准模式 1打开 0关闭"*/
#define LN2_MODE                    0   /*" 双回路计量模式 1打开 0关闭"*/


#define NORMAL_READ_MODE            0   /*"普通分项读取模式"*/
#define FAST_READ_MODE              1   /*"快速打包读取模式"*/

#define	CT_WIDE_RANGE               1   /*" CT宽范围开关 "*/
#define EDT_MODE                    1   /*" 自热补偿开关"*/
#define OUTDATA_MODE                1   /*" 原始数据输出开关,用于负荷识别功能"*/

#define RE_PWR_EN                   1   /*" 无功功率"*/
#define VA_ANGLE_EN 				1

/*"--------ram reg 数据长度控制--------"*/
#define RAM_DATA_TYPE              2
#define REG_DATA_TYPE              2

#define RAM_DATA_LENGTH8           sizeof(RAM_ADJ_DATA)/*" RAM_ADJ_DATA 字节数"*/
#define RAM_DATA_LENGTH            (RAM_DATA_LENGTH8/RAM_DATA_TYPE)
#define RAM_DATA_LENGTH16          (RAM_DATA_LENGTH8/2)/*" 21//RAM_ADJ_DATA 字数"*/
#define RAM_DATA_LENGTH32          (RAM_DATA_LENGTH8/4)
#define REG_DATA_LENGTH            sizeof(reg_addr)/*" ram_data_length16-7//寄存器数量"*/

/*" 													"*/
#define EEP_PAR_IS_INIT        0x56/*" 内卡参数已初始化,非默认参数,ram中也为非默认参数"*/
#define EEP_PAR_IS_DEF         0xAB/*" 内卡参数已初始化,ram中为默认参数"*/
#define EEP_PAR_NOT_INIT       0xAC/*" 内卡参数未初始化"*/
#define NEED_INIT              0xAB/*" 计量芯片需复位初始化标志"*/
#define NOT_NEED_INIT          0x56/*" 计量芯片不需复位初始化标志"*/
#define NEED_DELAY             0xAB
#define NOT_NEED_DELAY         0x56
#define ERROR                  0xAB
#define NO_ERROR               0x56
#define	NEED_RESET             0xAB
#define	NOT_NEED_RESET         0x56
#define EEP_RAM_TIME240        240/*" 秒"*/
#define LOW_VOLTAGE            1275000
#define LOW_VOLTAGE_LIMIT      500000/*" 最低电压下限"*/
#define CAL_STATUS_START       1
#define CAL_STATUS_STOP        0
#define FREQUENCE_CONST        44742500
#define CURRENT_GATE_BASE      960//5A电流对应的电流阀值是16MA  为防止EMC 静电提高 3倍至96MA
#define POWER_GATE_BASE        104//5A电流对应的功率阀值

#define NEGA_STATUS            1/*" 功率方向反向 "*/
#define ACTI_STATUS            0/*" 功率方向正向 "*/
#define MEA_DIR_NUM            3/*" 功率反向累计数"*/

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

/*"--------错误类型--------"*/
#define COMM_ERR_NOERR     0x00    /*"无错误"*/
#define COMM_ERR_OTHERS    0x01    /*"其它错误"*/
#define COMM_ERR_NODATA    0x02    /*"无数据请求"*/
#define COMM_ERR_PASSWORD  0x04    /*"权限错误"*/

#define NO_INIT    __no_init
#define INIT_MODE  0
#define RUN_MODE   1

/*"--------参数存放区域----"*/
#define RAM_REGION_A    0
#define EEP_REGION_A    1
#define EEP_REGION_B    2

/*"--------单相电量类型EngyId--------"*/
#define L_ENGY_P        0x00/*"火线有功能量"*/
#define N_ENGY_P        0x01/*"零线有功能量"*/

#define L_ENGY_Q        0x10/*"火线无功能量"*/
#define N_ENGY_Q        0x11/*"零线无功能量"*/

#define L_ENGY_S        0x20/*"火线视在能量"*/
#define N_ENGY_S        0x21/*"零线视在能量"*/
/*"--------三相电量类型EngyId--------"*/
#define A_ENGY_P        0x30/*"A相有功能量"*/
#define B_ENGY_P        0x31/*"B相有功能量"*/
#define C_ENGY_P        0x32/*"C相有功能量"*/
#define T_ENGY_P        0x33/*"合相有功能量"*/

#define A_ENGY_Q        0x40/*"A相无功能量"*/
#define B_ENGY_Q        0x41/*"B相无功能量"*/
#define C_ENGY_Q        0x42/*"C相无功能量"*/
#define T_ENGY_Q        0x43/*"合相无功能量"*/

#define A_ENGY_S        0x50/*"A相视在能量"*/
#define B_ENGY_S        0x51/*"B相视在能量"*/
#define C_ENGY_S        0x52/*"C相视在能量"*/
#define T_ENGY_S        0x53/*"合相视在能量"*/

/*" Add 2019/04/26 "*/
/*"--------在线监测参数读写及计量芯状态读功能码--------"*/
/*"     register           address         R/W     length  reset_value     function"*/

#define	_05_W_I_AMPSTD	    0xC0	/*"     W       4	    -----       写自监测电流回路幅值参考"*/
#define	_05_W_I_PHSSTD	    0xC1	/*"     W       4	    -----       写自监测电流回路相位参考"*/
#define	_05_W_U400_AMPSTD	0xC2	/*"     W       4	    -----       写自监测电压回路幅值参考"*/
#define	_05_W_U400_PHSSTD	0xC3	/*"     W       4	    -----       写自监测电压回路相位参考"*/
#define	_05_W_U10_AMPSTD	0xC4	/*"     W       4	    -----       写自监测电压回路幅值参考1"*/
#define	_05_W_U10_PHSSTD	0xC5	/*"     W       4	    -----       写自监测电压回路相位参考1"*/
#define	_05_W_U10_AMPSTD2	0xC6	/*"     W       4	    -----       写自监测电压回路幅值参考2"*/
#define	_05_W_U10_PHSSTD2	0xC7	/*"     W       4	    -----       写自监测电压回路相位参考2"*/


#define	_05_R_I_AMPSTD	    0xC0	/*"     R       4	    -----       读自监测电流回路幅值参考"*/
#define	_05_R_I_PHSSTD	    0xC1	/*"     R       4	    -----       读自监测电流回路相位参考"*/
#define	_05_R_U400_AMPSTD	0xC2	/*"     R       4	    -----       读自监测电压回路幅值参考"*/
#define	_05_R_U400_PHSSTD	0xC3	/*"     R       4	    -----       读自监测电压回路相位参考"*/
#define	_05_R_U10_AMPSTD	0xC4	/*"     R       4	    -----       读自监测电压回路幅值参考1"*/
#define	_05_R_U10_PHSSTD	0xC5	/*"     R       4	    -----       读自监测电压回路相位参考1"*/
#define	_05_R_U10_AMPSTD2	0xC6	/*"     R       4	    -----       读自监测电压回路幅值参考2"*/
#define	_05_R_U10_PHSSTD2	0xC7	/*"     R       4	    -----       读自监测电压回路相位参考2"*/


#define	_05_R_I_AMP	        0xC0	/*"     R       4	    -----       读自监测电流回路幅值计算值"*/
#define	_05_R_I_PHS	        0xC1	/*"     R       4	    -----       读自监测电流回路相位计算值"*/
#define	_05_R_U400_AMP   	0xC2	/*"     R       4	    -----       读自监测电压回路幅值计算值"*/
#define	_05_R_U400_PHS  	0xC3	/*"     R       4	    -----       读自监测电压回路相位计算值"*/
#define	_05_R_U10_AMP	    0xC4	/*"     R       4	    -----       读自监测电压回路幅值计算值1"*/
#define	_05_R_U10_PHS	    0xC5	/*"     R       4	    -----       读自监测电压回路相位计算值1"*/
#define	_05_R_U10_AMP2	    0xC6	/*"     R       4	    -----       读自监测电压回路幅值计算值2"*/
#define	_05_R_U10_PHS2	    0xC7	/*"     R       4	    -----       读自监测电压回路相位计算值2"*/
#define	_05_R_I_AMP_ERR	    0xC8	/*"     R       4	    -----       读自监测电流回路幅值误差值"*/
#define	_05_R_I_PHS_ERR     0xC9	/*"     R       4	    -----       读自监测电流回路相位误差值"*/
#define	_05_R_U400_AMP_ERR	0xCA	/*"     R       4	    -----       读自监测电压回路幅值误差值"*/
#define	_05_R_U400_PHS_ERR  0xCB	/*"     R       4	    -----       读自监测电压回路相位误差值"*/
#define	_05_R_U10_AMP_ERR   0xCC	/*"     R       4	    -----       读自监测电压回路幅值误差值1"*/
#define	_05_R_U10_PHS_ERR   0xCD	/*"     R       4	    -----       读自监测电压回路相位误差值1"*/
#define	_05_R_U10_AMP_ERR2  0xCE	/*"     R       4	    -----       读自监测电压回路幅值误差值2"*/
#define	_05_R_U10_PHS_ERR2  0xCF	/*"     R       4	    -----       读自监测电压回路相位误差值2"*/
#define	_05_R_P_AMP_ERR	    0xF0	/*"     R       4	    -----       读自监测电流回路幅值误差值"*/
#define	_05_R_P_PHS_ERR     0xF1	/*"     R       4	    -----       读自监测电流回路相位误差值"*/


#define	_05_R_READ_ERR_RECORD	        0x80	/*"     R       1	    -----       计量芯片读数据错误记录计数器"*/
#define	_05_R_INIT_ERR_RECORD	        0x81	/*"     R       1	    -----       计量芯片初始化错误记录计数器"*/
#define	_05_R_DELAYREAD_FLAG	    	0x82	/*"     R       1	    -----       计量芯片延时读标志字"*/
#define	_05_R_INIT_FLAG				  	0x83	/*"     R       1	    -----       计量芯片初始化标志字"*/
#define	_05_R_READ_ERR				    0x84	/*"     R       1	    -----       计量芯片读标志字"*/
#define	_05_R_RST_FLAG	  			    0x85	/*"     R       1	    -----       硬件复位标志字"*/
#define	_05_R_DELAYREAD_RECORD		    0x86	/*"     R       1	    -----       计量芯片延时读计数器"*/
#define	_05_R_READ_EFFECT_TIMER		    0x87	/*"     R       1	    -----       计量芯片读有效值计时器"*/
#define	_05_R_READ_INTERVALERR_RECORD	0x88	/*"     R       1	    -----       计量芯片读间歇错误记录计数器"*/
#define	_05_R_PULSE_FLAG			    0x89	/*"     R       1	    -----       计量输出数据"*/

/*"打包读EDT参数宏"*/
#define		EDT_PARA_NUM		6U
#define		EDT_STD_POS_IN_ST	34U
#define		EDT_ALLP_CMD_R_LEN	58U

/* 负数数值转换 */
#define     ADD_IF_SIGNED_VALUE           80000000
#define     IF_SIGNED_BIT_SET             0x80000000

#if HARMONIC_MODE
#define	nPhs			1
#define NUM_FFT 	    128 	// 瞬时数据点数
#define	nHarmonic	    MU_HARMONIC_NUM		// 计算谐波次数,0为基波
#endif

/*"----------------------------------"*/
/*"--------数据结构、数据变量--------"*/
/*"----------------------------------"*/
typedef enum
{
   #ifdef SINGLE_PHASE
   E_TEMP_LIN=0,    /*"火线进线温度"*/
   E_TEMP_LOUT,     /*"火线出线温度"*/
   E_TEMP_NIN,       /*"零线进线温度"*/
   E_TEMP_NOUT,    /*"零线出线温度"*/
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
  INT16U  Ram_MeterCfg;/*" 计量配置字"*/
  INT16U  Ram_MeterCfg2;/*" 计量配置字2"*/
  INT16U  Ram_ecconst;
}RAM_ADJ_DATA;/*" 	增加、删除成员需要修改对应内卡空间大小	"*/

typedef struct
{
    INT32U Voltage;         /*" 电压"*/
    INT32U CurrentA;        /*" 电流 "*/
    INT32U CurrentB;        /*" 电流 B"*/
    INT32U PowerPA;         /*" 功率"*/
    INT32U RealPowerPA;     /*" 火线真实有功率值(未被潜动)"*/
    INT32U PowerPB;         /*" 零线功率"*/
    INT32U RealPowerPB;     /*" 零线真实有功率值(未被潜动)"*/
    INT32U PowerQ;          /*" 当前通道无功功率"*/
    INT32U PowerQA;         /*" 火线无功功率"*/
    INT32U PowerQB;         /*" 零线无功功率"*/
    INT32U PowerSA;         /*" 火线视在功率"*/
    INT32U PowerSB;         /*" 零线视在功率"*/
    INT32U EnergyP;
    INT32U EnergyQ;
    INT32U EnergyS;
    INT32U Frequency;       /*" 频率"*/
    INT32U FactorA;         /*" 功率因素"*/
    INT32U FactorB;         /*" 功率因素"*/
    INT32U DrA;             /*" 功率方向"*/
    INT32U DrB;             /*" 功率方向"*/
    INT32U DrqA;            /*" 无功功率方向"*/
    INT32U DrqB;            /*" 无功功率方向"*/
    INT32S gTmnlTmp[E_TEMP_MAX];  /*"端子排温度"*/
}ST_MU_INTERDATA;

typedef struct
{
  INT16U Voltage;       /*" 电压"*/
  INT16U Current;       /*" 电流 "*/
  INT16U Power;         /*" 功率"*/
  INT8U Factor;         /*" 功率因素"*/
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
    INT16U	HarmonicpercentI[nPhs][nHarmonic];	//---谐波含有率--NNN.N6
    INT32U	HarmonicUrms[nPhs];		// 各次谐波有效值
    INT32U	HarmonicIrms[nPhs];
}sHarmonicData_TypeDef;



/*"-----------瞬时量数据结构体定义-----------"*/
typedef struct
{
    INT16U volt[MU_ELEMENT_NUM];/*"电压"*/
    INT32S curr[MU_ELEMENT_NUM];/*"电流"*/
    INT32S actPwr[MU_ELEMENT_NUM + 1];/*"有功功率"*/
    #if (RE_PWR_EN == 1)
    INT32S reactPwr[MU_ELEMENT_NUM + 1];/*"无功功率"*/
    #endif
    #if (AP_PWR_EN == 1)
    INT32S apparentPwr[MU_ELEMENT_NUM + 1];/*"视在功率"*/
    #endif
    INT32U freq;/*"频率"*/
    INT32S factor[MU_ELEMENT_NUM + 1];/*"功率因数"*/
    INT32U calNcurr;/*"零线电流（计算值）"*/
	INT32U residualCurr;/*"剩余电流"*/
    #if (VOLT_ANGLE_EN == 1)
    INT32U VoltAngle[3];/*"电压相角"*/
    #endif
    #if (VA_ANGLE_EN == 1)
    INT32U VoltCurrAngle[4];/*"功率因数角"*/
    #endif
    INT8U Quad[MU_ELEMENT_NUM + 1];/*"当前象限值    0~3 I/II/III/IV象限,   MU_ELEMENT_NUM + 1 表示总ABC "*/
    INT8U PowerDir;
    /*"***************************************************"*/
    /*"功率方向bPowerDir的bit位定义"*/
    /*"8个bit分别表示TABC有功和无功的功率方向，bit值为0表示正向，为1表示反向
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
    INT32S gTmnlTmp[E_TEMP_MAX];  /*"端子排温度"*/
    #if (HARMONIC_MODE == 1)	
    INT16U HRVolt[MU_ELEMENT_NUM][nHarmonic];     /*电压谐波,单位0.01%,HRVolt[0][0]电压THD*/
    INT16U HRCurr[MU_ELEMENT_NUM][nHarmonic];     /*电流谐波,单位0.01%,HRCurr[0][0]电流THD*/
    INT32U HRUrms[MU_ELEMENT_NUM];		          /*电压总谐波有效值0.0001V*/
	INT32U HRIrms[MU_ELEMENT_NUM];                /*电流总谐波有效值0.0001A*/
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
{              					 /*”增加调校命令 "*/
    INT32S Ib_80Un_1;        				 /*”80%un 5%IB  1.0下精度修正值"*/
    INT32S Ib_80Un_5L;        				 /*”80%un 5%IB  1.0下精度修正值"*/
    INT32S Ib_90Un_1;      					 /*”90%un 5%IB  1.0下精度修正值"*/
    INT32S Ib_90Un_5L;       				 /*”待定保留 "*/
    INT32S Ib_110Un_1;       				 /*”110%un 5%IB  1.0下精度修正值"*/
    INT32S Ib_110Un_5L;       				 /*”待定保留 "*/
    INT32S Ib_115Un_1;     					 /*”1150%un 5%IB  1.0下精度修正值"*/
    INT32S Ib_115Un_5L;     				/*”1150%un 5%IB  1.0下精度修正值"*/
    INT32S Up_Down_0_05Ib;     			/*”待定保留 "*/
    INT32S Up_Down_Ib;     				/*”待定保留 "*/
    INT32S Up_Down_Imax;     				 /*”待定保留 "*/
    INT32S Imax_10min_1;     				 /*”待定保留 "*/
    INT32S Imax_50min_1;    				 /*”IMAX，50分钟 1.0下精度修正值"*/
    INT32S Imax_50min_5L;     				 /*”IMAX   50分0.5L下精度修正值 "*/
    INT32S Imax_50min_8C;   				 /*”IMAX 50分钟0.8C下精度修正值 "*/
    INT32S Harmonic;						 /*”5次谐波修调值 "*/
}ST_AMEND_VALUE;

typedef struct{
    INT16U Amend_Imax_Time;				 /*”Imax电流的检测时间，为3200秒xl,20110907 "*/
    INT8U Amend_Imax_LoseTime; 			 /*”Imax电流情况下电流波动允许时间，为5秒钟xl,20110907 "*/
    INT8U  Amend_Complete;				 /*”未修正的时候为0，修正的时候各种修正状态为不同的值0x00-0x19  xl,20110907 "*/
    INT8U  Amend_Setchange;				 /*”调校参数有变化时为1.无变化时为0   xl,20110907 "*/
    INT8U  Amend_Resume; 				 /*”在修正区间时为1，没在修复区间时为0    xl,20110907 "*/
    INT16U Amend_Judge_Timer;			 /*”升降变差判断时间"*/
    INT8U Amend_UpDownIbFlg;			 /*”升降变差Ib点判断标志"*/
    INT8U Amend_UpDownImaxFlg;			 /*”升降变差Imax点判断标志"*/
    INT8U Amend_UpDownFlg;				 /*”进入升降变差修调标志"*/
    INT8U Amend_UpImax_Timer;			 /*”升降变差Imax电流持续时间"*/
    INT8U Amend_UpIb_Timer;				 /*”升降变差Ib电流持续时间"*/
    INT8U Amend_Up0_05Ib_Timer;			 /*”升降变差5%Ib电流持续时间 "*/
    INT16U Amend_Down_0_05Ib_Timer;	 /*”5%IB在做升降变差实验持续时间"*/
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
  INT8U MeasureChip_Read_Error_Record;       /*"计量芯片读数据错误记录计数器"*/
  INT8U MeasureChip_Initialize_Error_Record;      /*"计量芯片初始化错误记录计数器"*/
  INT8U MeasureChip_DelayRead_Flag;      /*"计量芯片延时读标志字"*/
  INT8U MeasureChip_Initialize_Flag;       /*"计量芯片初始化标志字"*/
  INT8U MeasureChip_Read_Error;       /*"计量芯片读标志字"*/
  INT8U MeasureChip_Reset_Flag;       /*"硬件复位标志字"*/
  INT8U MeasureChip_DelayRead_Record;  /*"计量芯片延时读计数器"*/
  INT8U Read_EffectValue_Timer;/*"计量芯片读有效值计时器"*/
  INT8U MeasureChip_Read_IntervalError_Record;/*"计量芯片读间歇错误记录计数器"*/
  INT8U MeasureChip_pluse_flag;
}ST_MU_FLAG;/*"计量输出数据"*/

typedef struct
{
	INT16U MeaRun_State_Flag; /*"计量芯片运行状态字，表示"*/
	INT16U Measure_State_Flag; /*"当前计量状态"*/
    INT16U MeaChannel_State_Flag;	/*"当前计量通道状态"*/
}ST_MU_STATE;

typedef struct
{
	INT16U Phase_measure_mode;/*"电表计量配置字"*/
	INT16U Phase_i_base;/*"电表电流规格配置字"*/
	INT16U Phase_i_gate;/*"电表电流潜动阀值配置字 "*/
    INT16U Phase_p_gate;
	INT16U Phase_iL_Nopld;/*"火线辅助潜动标志字"*/
	INT16U Phase_iN_Nopld;/*"零线辅助潜动标志字"*/
	INT16U Phase_Enable_AMEND;/*"允许修调"*/
}ST_MU_MODE;

typedef struct
{
	INT16U Time_cnt1;/*"4分钟计数器"*/
  	INT16U Time_cnt2;/*"5秒钟计数器"*/
	INT16U EMU_ERR_CNT;/*"EMUSTATUS读错误计数器"*/
  	INT16U EMU_ERR_CNT1;/*"EMUSTATUS累加和错误计数器"*/
	INT16U NEGA_Time;/*"正向累计时间"*/
    INT16U ACTI_Time;/*"反向累计时间"*/
    INT16U NEGAB_Time;/*"次通道正向累计时间"*/
    INT16U ACTIB_Time;/*"次通道正向累计时间"*/
}ST_MU_COUNT;

typedef struct
{
	INT8U Fatal_err_flag;/*"连续写计量芯片失败"*/
  	INT8U Fatal_err_flag1;/*"连续复位失败"*/
  	INT8U Fatal_err_flag2;/*"连续读计量芯片校验和失败   "*/
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
ST_OUT_DATA;               /*" 负荷识别参数"*/

typedef struct
{
    INT16U MC_DefPara_Flag;             /*"计量芯片默认参数标志"*/
    RAM_ADJ_DATA ram_adj_data;          /*" 计量芯片参数"*/
    #if EDT_MODE
    ST_EDT_STDDATA edt_stddata;         /*" edt参数"*/
    #endif
    #if AMEND_MODE
    ST_AMEND_VALUE AmendValue;          /*" 计量修调参数"*/
    #endif
    #if OUTDATA_MODE
    ST_OUT_DATA out_data;               /*" 负荷识别参数"*/
    #endif
    INT16U CRC_Val;                     /*" CRC校验"*/
}ST_MU_PARA;

typedef struct
{
	INT8U UnBalanceTimer1;						/*"通道不平衡计数器1"*/
	INT8U UnBalanceTimer2;						/*"通道不平衡计数器2"*/
	INT8U Init_Channel_Status;					/*"通道初始化状态"*/
	INT8U Init_Channel_flag;					/*"通道初始化标志"*/
	INT8U Low_Current_flag;						/*"通道1、2电流均小于5%标志"*/
}ST_MU_LN2PARA;



/*"----------------------------------"*/
/*"--------外部声明------   ------------"*/
/*"----------------------------------"*/

/*" MU对外接口变量"*/
extern ST_MU_PARA      gMU_Para;
extern ST_MU_FLAG      gMea_Flag;/*"计量芯片状态标志位"*/
extern ST_MMT_DATA     stMmtData;
extern ST_MU_STATE     gMea_State;/*"提供给应用层参考状态"*/
#if EDT_MODE
extern ST_EDT_ERRDATA  gEDT_Errdata;/*"在线监测数据"*/
#endif


/*" MU对外接口函数"*/
extern void	MU_Init(INT8U mode);
extern void	MU_Main();
extern INT8U  MU_JustMtr(INT8U *pData);
extern INT8U MU_EngyGet(INT16U EngyId, INT32U *pData);
extern void MU_OpenPulse(void);
extern void  MU_ReadInstantData(INT16U reg);


#endif



