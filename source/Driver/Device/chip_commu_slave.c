#include "APP\message\MS.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "APP\message\FRMT_def.h"
#include "App\UserInterface\ProtocolStack\ProtocolStack.h"
#include "App\UserInterface\ProtocolStack\PS_645\Dlt645.h"
#include "App\UserInterface\ProtocolStack\PS_698\DL_698.h"
#include "App\UserInterface\ProtocolStack\host_commu\host_commu.h"
#include "app\userinterface\UI.h"
#include "APP\LegallyRelevant\MU_LIB.h"
#include "Lib\LIB_func.h"
#include "system\Sys_def.h"
#include "PlatForm.h"
#include "chip_commu_slave.h"
//#include "pkt_proc.h"


INT16U wait_master_tx;
INT16U uart2_rx_tick;
INT8U uart2_fault;
INT8U uart2_retry_tms;

static INT8U poweron_rtc_flg;

static ST_COMMU_DEVICE mstSlaveDevice[MAX_SLAVE_DEV];/*"�豸������Ϣ"*/

static INT8U gSlave_Buff_Rx[CHIP_BUFF_RX_SIZE];
static INT8U gSlave_Buff_Tx[CHIP_BUFF_RX_SIZE];


INT8U cfg_poweron_bm_info_pkt(INT8U *pkt)
{
    ST_7TIME_FORM now;
    UN_ID645 id;

    id.asLong = ID_RTC_SYNC;
    GetSingle(E_SYS_TIME, (INT8U *)&now);/*"ȡϵͳʱ��"*/
    LIB_CharToBcdNByte((INT8U *)&now, 7);
    return cfg_645_pkt(0x11, id, (INT8U *)&now, 7, pkt);

}


void ChipComm_Slave_Powron_RX_INT(void)
{
    INT8U byte;
    ST_COMMU_DEVICE *pDevice;

    pDevice = &mstSlaveDevice[0];

    byte = UART_Receive1Byte(E_UART_ID_SLAVE);
    if(byte == 0xAB)
    {
        poweron_rtc_flg++;
    }
    else
    {
        poweron_rtc_flg = 0;
    }
    if(poweron_rtc_flg >= 1 && pDevice->tx_off == 0xff)
    {
        pDevice->tx_off = 0;
        pDevice->TxLen = cfg_poweron_bm_info_pkt(pDevice->TxBuff);
        UART_Send1Byte(E_UART_ID_SLAVE, pDevice->TxBuff[0]);
        pDevice->tx_off++;
        pDevice->TxLen--;
    }   
}

void ChipComm_Slave_Powron_TX_INT(void)
{
    ST_COMMU_DEVICE *pDevice;

    pDevice = &mstSlaveDevice[0];
    
    if(pDevice->TxLen > 0)
    {
        UART_Send1Byte(E_UART_ID_SLAVE, pDevice->TxBuff[pDevice->tx_off]);
        pDevice->tx_off++;
        pDevice->TxLen--;
    }
    else
    {
        pDevice->tx_off = 0xff;
        poweron_rtc_flg = 0;
    }
}

INT8U is_retrans_645_pkt(ST_FRAME645 *frm)
{
    if((frm->dataId.asLong > 0xFF000000) && (frm->dataId.asBytes[2] == 1)) /*" DI2 "*/
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

INT8U chip_commu_ok(void)
{
    if(uart2_fault == CHIP_COMMU_OK)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/*���ɸ���ʶ�����ò���*/
INT8U PrepareLdPara(INT8U *pdata)
{
	ST_LD_PARA ldPara;
	INT32U Au,Ai;
	INT16U num;

	LIB_MemSet(0x00, (INT8U *)&ldPara, sizeof(ST_LD_PARA));
	//num=128/(gMU_Para.out_data.TxDataInterval+1);
	num=32;
	ldPara.highByte=num>>8;;
	ldPara.lowByte=(num&0x00ff);
	ldPara.meterType=0x01;
	ldPara.dataLen=0x03;
	ldPara.point=8;/*С����*/
	Au=(200000000UL)/gMU_Para.ram_adj_data.Ram_V_Coefficient;
	//Au=0x540C;//magic number
	LIB_MemCpy((INT8U *)&Au,(INT8U *)&ldPara.Au, 4);
	Ai=(200000000UL)/gMU_Para.ram_adj_data.Ram_Ia_Coefficient;
	//Ai=0x1578;
	LIB_MemCpy((INT8U *)&Ai, (INT8U *)&ldPara.Ai,4);
	LIB_MemCpy((INT8U *)&ldPara, (INT8U *)pdata, sizeof(ST_LD_PARA));
    return 1;
}

INT32U frm_cnt;
INT16U mb_tx_tms;
INT16U mb_rx_ok_tms;
INT16U mb_rx_err_tms;
INT16U mb_err_tms;


INT8U set_show_data(INT8U *buf)
{
    LIB_MemCpy(buf, (INT8U *)&frm_cnt, 4);
    LIB_MemCpy(buf + 4, (INT8U *)&mb_tx_tms, 2);
    LIB_MemCpy(buf + 6, (INT8U *)&mb_rx_ok_tms, 2);
    LIB_MemCpy(buf + 8, (INT8U *)&mb_rx_err_tms, 2);
    mb_err_tms = mb_tx_tms - mb_rx_ok_tms;
    return 10;
}

extern INT16U get_bm_lrm_data(INT8U *buf);
/*" UART2�������Ϸ�645����ʱ���� "*/
INT8U bm_slave_rx_proc(ST_FRAME645 *frm, INT16U *ans_len)
{
    ST_7TIME_FORM now;
    //INT8U len;
    INT8U buf[16];
    //ST_MU_BASE_DATA data;
    
    uart2_rx_tick = MCU_GetTick();
    if(is_retrans_645_pkt(frm)) /*" ����Ϊ�ش�����,��DI2Ϊ01��ʾ�ش� "*/
    {
        frm->dataId.asBytes[2] = 0;
        uart2_retry_tms++;
        if(uart2_retry_tms >= (MAX_RETRANS_TMS - 1))
        {
            uart2_fault = CHIP_COMMU_FAULT; /*" �ﵽN���ش�����ͨ������ "*/
            return CM_UNUSUAL;
        }
    }
    else
    {
        uart2_retry_tms = 0;
    }
    
    switch(frm->dataId.asLong)
    {
        case ID_SHOW_PUSH: /*" ��ʾ����Ӧ�� "*/
            uart2_fault = CHIP_COMMU_OK; 
            /*" ��ʾ���ݱ��浽��ʾģ�� "*/
            //len = set_show_data(frm->data);
            set_show_data(frm->data);
            Action(E_DOWNDATA_GET, frm->data + 10, NULL);
            /*" �������ذ�������̵��� "*/
            //len += do_main_meter_relay_cmd(frm->data + len);
            /*" �������ذ������LED  "*/
            //do_main_meter_led_cmd(frm->data + len);
            /*" �������������ɵ���ģ���ṩ��������״̬�����������״̬���ģ���ṩ��������"*/

            *ans_len = get_bm_lrm_data(frm->data);
            break;
        case ID_AO_CTRL:/*" ����AOӦ�� "*/
            
            break;
        case ID_RTC_SYNC:/*" ��������RTCӦ�� "*/
            GetSingle(E_SYS_TIME, (INT8U *)&now);/*"ȡϵͳʱ��"*/  
            LIB_MemCpy((INT8U *)&now, frm->data, 7);
            LIB_CharToBcdNByte(frm->data, 7);
            *ans_len = 7;
            break;
        case ID_CLR_MTR:
            CLR_MTR();
            frm->data[0] = 0;
            *ans_len = 1;
            break;
        case ID_CLR_EVT:
            buf[0] = 0xfe;
            buf[1] = 0xff;
            Action(E_RCRD_CLR, &buf[0], NULL);
            frm->data[0] = 0;
            *ans_len = 1;
            break;
        case ID_INIT_MTR:
            MTR_INIT();
            break;
        case ID_SET_BM_TM: /*" дʱ�� "*/
            BeforeFunSetSystemTime(frm->data); /*" HEX��ʽʱ�� "*/
            SetSingle(E_SYS_TIME, frm->data, 7);
            frm->data[0] = 0;
            *ans_len = 1;
            break;
        case ID_SET_BM_ADDR: /*" д���ַ "*/
            SetSingle(E_COMM_ADDR, frm->data, 6);
            frm->data[0] = 0;
            *ans_len = 1;
            break;
        case ID_GET_LD_CFG_ADDR://todo
            PrepareLdPara((INT8U *)&frm->data[0]);
            *ans_len = sizeof(ST_LD_PARA);
            break;
        default:
            break;
    }
    return COMM_ERR_NOERR;
}






/*" UART2��·������ά�� "*/
void bm_uart2_rx_period_proc(void)
{
    if(TRUE == MCU_CmpTick(uart2_rx_tick, BM_UART2_RX_TIMEOUT))
    {
        uart2_fault = CHIP_COMMU_FAULT;
    }
}


void Chip_Commu_RX_INT(void)
{
    ST_COMMU_DEVICE *pDevice;

    pDevice = &mstSlaveDevice[0];

    if(pDevice->RxLen >= CHIP_BUFF_RX_SIZE)
    {   
        pDevice->RxLen = 0; 
    }	
    pDevice->RxBuff[pDevice->RxLen] = UART_Receive1Byte(E_UART_ID_SLAVE); /*"���������ֽڸ������ջ�����"*/
    pDevice->RxLen++;                        /*"���յ��ĳ���+1"*/
}

void Chip_Commu_TX_INT(void)
{
    ST_COMMU_DEVICE *pDevice;

    pDevice = &mstSlaveDevice[0];
    
    if(pDevice->TxLen > 0)
    {
        UART_Send1Byte(E_UART_ID_SLAVE, pDevice->TxBuff[pDevice->tx_off]);
        pDevice->tx_off++;
        pDevice->TxLen--;
    }
    else
    {
        
    }
}

void Chip_Commu_StartSend(INT8U *date,INT16U len)
{
    ST_COMMU_DEVICE *pDevice;

    pDevice = &mstSlaveDevice[0];
    
    LIB_MemCpy(date, pDevice->TxBuff, len);
    pDevice->TxLen = len;
    pDevice->tx_off = 0;
    Chip_Commu_TX_INT();
}

/*" ֻ���ϵ�����в����� "*/
void Slave_Commu_Init(INT8U is_pwr_on)
{
    ST_UARTCONFIG UartConf;
    
    LIB_MemSet(0, (INT8U *)&mstSlaveDevice, sizeof(mstSlaveDevice));

    mstSlaveDevice[0].RxBuff = gSlave_Buff_Rx;
    mstSlaveDevice[0].TxBuff = gSlave_Buff_Tx;    

    UartConf.Baud = E_BAUD460800;/*E_BAUD115200*/
    UartConf.TranSta.byte = TRANSTA_SO_RO;
    if(is_pwr_on == 1)
    {
        UartConf.ReceiveIsr = ChipComm_Slave_Powron_RX_INT;
        UartConf.SendIsr = ChipComm_Slave_Powron_TX_INT;
        mstSlaveDevice[0].tx_off = 0xff;
        poweron_rtc_flg = 0;
    }
    else
    {
        UartConf.ReceiveIsr = ChipComm_Slave_Powron_RX_INT;
        UartConf.SendIsr = ChipComm_Slave_Powron_TX_INT;
    }
    UART_Open(E_UART_ID_SLAVE, &UartConf);
}



