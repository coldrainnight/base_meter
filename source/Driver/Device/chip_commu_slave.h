#ifndef _H_BM_PKT_PROC
#define _H_BM_PKT_PROC


#define CHIP_BUFF_RX_SIZE 263     /*"cmͨ��ģ��ͨ�����ջ�����"*/	
#define CHIP_BUFF_TX_SIZE 263     /*"cmͨ��ģ��ͨ�����ͻ�����"*/

#define CHIP_COMMU_OCTET_TIMEOUT (30)/*"�ֽڼ䳬ʱ"*/
#define CHIP_COMMU_INACTIVITY_TIMEOUT (1000)/*"�޻��ʱ (֡��ʱ)"*/

#define MAX_SLAVE_DEV  1

#define CHIP_COMMU_OK       0x11
#define CHIP_COMMU_FAULT    0xaa

#define BM_UART2_RX_TIMEOUT (5000)/*" ����UART2���ճ�ʱʱ�䣬�೤ʱ��δ�յ��Ϸ�������Ϊ��ʱ "*/


typedef struct                
{  	
    INT32U sec_eng;
    ST_MMT_DATA data;
}ST_MU_BASE_DATA;

typedef struct
{
INT8U highByte;
INT8U lowByte;/*���������*/
INT8U meterType;
INT8U dataLen;/*ÿ���������ݳ���*/
INT8U point;/*С����*/
INT8U Au[4];
INT8U Ai[4];
INT8U resvCoeff[16];
}ST_LD_PARA;

extern void Slave_Commu_Init(INT8U is_pwr_on);

#endif
