#ifndef _H_BM_PKT_PROC
#define _H_BM_PKT_PROC


#define CHIP_BUFF_RX_SIZE 263     /*"cm通信模块通道接收缓冲区"*/	
#define CHIP_BUFF_TX_SIZE 263     /*"cm通信模块通道发送缓冲区"*/

#define CHIP_COMMU_OCTET_TIMEOUT (30)/*"字节间超时"*/
#define CHIP_COMMU_INACTIVITY_TIMEOUT (1000)/*"无活动超时 (帧超时)"*/

#define MAX_SLAVE_DEV  1

#define CHIP_COMMU_OK       0x11
#define CHIP_COMMU_FAULT    0xaa

#define BM_UART2_RX_TIMEOUT (5000)/*" 基表UART2接收超时时间，多长时间未收到合法报文认为超时 "*/


typedef struct                
{  	
    INT32U sec_eng;
    ST_MMT_DATA data;
}ST_MU_BASE_DATA;

typedef struct
{
INT8U highByte;
INT8U lowByte;/*采样点个数*/
INT8U meterType;
INT8U dataLen;/*每个采样数据长度*/
INT8U point;/*小数点*/
INT8U Au[4];
INT8U Ai[4];
INT8U resvCoeff[16];
}ST_LD_PARA;

extern void Slave_Commu_Init(INT8U is_pwr_on);

#endif
