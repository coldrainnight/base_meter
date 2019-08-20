#ifndef _H_HOST_COMMU
#define _H_HOST_COMMU
#include "App\UserInterface\ProtocolStack\PS_645\Dlt645.h"

#define STOP_FOR_INIT_FLG 0xaafacedb

#define HOST_USE_UART   1 /*" �ڲ�ͨ����UART "*/

//#define MAIN_METER  1
//#define BASE_METER  1

#define DL645_READ_LIST 1 /*" ��������� "*/
#define DL645_FLOW_REPORT 1 /*" 645�����ϱ�����"*/

#define ID_SHOW_PUSH     0xFF000001  /*" ÿ������Ӧ�� "*/
#define ID_AO_CTRL      0xFF000002  /*" AO���� "*/
#define ID_SET_SHOW_PARA     0xFF000003  /*" ʵʱ״̬����ʾ��˸Ƶ������ "*/
#define ID_RTC_SYNC     0xFF000103  /*" ����RTCʱ��ͬ��  "*/
#define ID_CLR_MTR      0xFF000004  /*" �������  "*/
#define ID_CLR_EVT      0xFF000005  /*" �¼�����  "*/
#define ID_INIT_MTR     0xFF00AC06  /*" �ڿ���ʼ��  "*/
#define ID_SET_BM_TM    0xFF000007  /*" ���û���ʱ�� "*/
#define ID_SET_BM_ADDR  0xFF000008  /*" ���û����ַ "*/
#define ID_GET_LD_CFG_ADDR  0xFF000009  /*" ��ȡ����ʶ��ģ�����ò��� "*/

#define ID_BM_DATA_SYNC     0xFF000010  /*" ������Ԫ����ͬ��"*/
#define ID_BM_MMT_DATA      0xFF000011  /*" ������Ԫ˲ʱ��ˢ��"*/
#define ID_BM_SELF_MONITOR  0xFF000012  /* ����Լ������*/
#define ID_BM_KEY_DATA      0xFF000013  /*  ��������*/
#define ID_BM_FRZ_DATA      0xFF000014  /*  ��������*/


#define MASTER_TX_COM_OCTET_TIMEOUT (30)/*"�ֽڼ䳬ʱ"*/
#define MASTER_TX_COM_INACTIVITY_TIMEOUT (1000)/*"�޻��ʱ (֡��ʱ)"*/


#define MAX_MASTER_TX_COM_DEV  1

#define CH_IDLE_STATUS 0
#define CH_BUSY_STATUS 1
#define CH_FAULT_STATUS 2

#define MAX_RETRANS_TMS   3 /*" Ӧ��ʱ���Դ��� "*/

#define PEER_REPLY_TIMEOUT (3000)/*" �û������ĵȴ�����Ӧ��ʱʱ�� ,����ӵ����Ӧ��"*/
#define WAIT_ANS_TIMEOUT (1000) /*" ���͵����յĳ�ʱʱ�� "*/
#define WAIT_LD_PKT_TX_TIMEOUT (50) /*" ����ʶ��ģ����������ı��ĵ�Ӧ�������ʱ�� "*/

#define PKT_SRC_OFF 0
#define PKT_LEN_OFF 1
#define PKT_DATA_OFF 2

/*" Դ�˿ڷ��� "*/
#define INTER_MU_PKT 0x10 /*" �ڲ����������Ԫ���� "*/
#define INTER_ANS_PKT 0x15 /*"�ڲ� Ӧ�������Ԫ����ı��� "*/

#define LD_REPLY_PKT 0xDD /*" ����ʶ��ģ����������ı��ĵ�Ӧ�� "*/
#define _LDIDResetDelay  20

#define MAX_PKT_NUM  5
#define HOST_COMMU_PKT_LEN  200
struct pkt_queue
{
    INT8U head;
    INT8U tail;
    INT8U qlen;
    INT8U tx;
    INT8U pkt[MAX_PKT_NUM][HOST_COMMU_PKT_LEN];
    
    INT8U status;
    INT8U retrans_tms;
    INT16U user_reply_tick[E_MAX_COM_ID]; /*" �û������ĵȴ�����Ӧ��tick "*/
    INT16U send_tick; /*"UART2���ͱ���tick�� �������ʱ��ֵ "*/
    
	INT16U retry_tick;
};

typedef struct                
{  	
    INT8U CurrentDeviceId; /*"��ǰ�豸��Ӳ���˿ں�"*/
       
	INT8U *RxBuff;/*"���ջ�����"*/
	INT8U *TxBuff;/*"���ͻ�����"*/	
	
	INT16U RxLen;/*"���ճ���"*/
	INT16U TxLen;/*"���ͳ���"*/
    INT16U tx_off;

    INT16U send_end_tick;

	INT16U InactivityTimer;/*�޻��ʱʱ�䣬�������һ��ʱ����û�л��Ͽ�����*/
	INT16U InterOctetTimer;	/*"�ֽڼ䳬ʱʱ��"*/
	
	INT8U InterruptClose:1;/*"�жϹرգ�ֻ���жϹرգ����ܽ����ڽ��յ���������ӵ����ջ�����"*/	
	INT8U TxWaiting:1;     /*"���͵ȴ�"*/
	INT8U InterOctetTimeout:1;	/*�ֽڼ䳬ʱ��־λ��1λ��*/	
	INT8U InactivityTimeout:1;  /*֡��ʱ��־λ��1λ��*/	
	INT8U TxOver:1;     /*"���ͽ���"*/
	INT8U frmType:2;  /*"����֡����: 01_698; 02_645; else_invalid"*/
	INT8U Saved:1;

}ST_COMMU_DEVICE;

typedef struct
{
    INT8U bmstatus[2];
    INT8U samplestatus[2];
    INT8U tmp_l_in[2];
    INT8U tmp_l_out[2];
    INT8U tmp_n_in[2];
    INT8U tmp_n_out[2];
    INT8U mtr_tmp[2];
    INT8U mtr_humidity[2];
    INT8U clk_bat_volt[2];
    INT8U n_curr[4];
    INT8U volt[2];
    INT8U curr[4];
    INT8U ac_pwr[4];
    INT8U r_ac_pwr[4];
    INT8U factor[4];
    INT8U freq[2];

    INT8U pa_eng[5];
    INT8U na_eng[5];
    INT8U r_eng1[4][5];
} ST_BM_SEC_DATA;


typedef struct
{
    INT8U id;
    INT8U cmd;
    INT16U tms;
}AO_CMD;

typedef struct
{
    INT16U obis;
    INT8U dst_fmt;
    INT8S exp;
}COMM_OBIS;



extern INT32U stop_for_init;
extern ST_7TIME_FORM poweron_rtc_tm;
extern struct pkt_queue mst_queue[MAX_MASTER_TX_COM_DEV];

extern INT8U is_peer_data(INT8U dev, INT8U com, ST_FRAME645 *res);
extern INT8U pkt_outqueue(struct pkt_queue *q);
extern INT16U MCU_GetNonZeroTick(void);
extern void Master_TX_Com_Init(INT8U dev);
extern INT8U tx_pkt_to_peer(INT8U dev, INT8U cmd, UN_ID645 id, INT8U *data, INT8U len);
extern INT8U tx_pkt_to_peer_now(INT8U dev, INT8U cmd, UN_ID645 id, INT8U *data, INT8U len);
extern void Master_TX_Com_StartSend(INT8U dev, INT8U *data,INT16U len);
extern void Master_TX_Com_Task(void);
extern INT8U cfg_645_pkt(INT8U cmd, UN_ID645 id, INT8U *data, INT8U len, INT8U *pkt);
extern INT8U is_bm_para_data(INT32U id);
extern INT8U is_only_bm_para_data(INT32U id);
extern void Com_Send_data(INT8U com, INT8U *pkt, INT8U len);
extern void Host_Commu_Sec_Proc(void);
extern INT8S poweron_get_bm_info(void *pdin, void *pdout);
void Load_identify_init(INT8U Mode);
extern INT8U user_req_peer_data_proc(INT8U dev, INT8U com, INT8U *frm, INT8U frmlen);
extern INT8U Master_TX_Com_AnswerCmd(INT8U *pdataBuff, INT8U len, INT8U dev);
extern INT8U Master_TX_Com_ProcessRecBytes(INT8U *pDataBuff, INT16U length);
extern void user_req_pkt_outqueue(void);
extern void Host_Commu_Bm_Data_Sync(INT8U type);


/******************************************************************************/

#endif
