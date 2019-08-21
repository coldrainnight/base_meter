#include "app\UserInterface\UI.h"
#include "App\UserInterface\ProtocolStack\ProtocolStack.h"
#include "App\UserInterface\ProtocolStack\PS_645\Dlt645.h"
#include "App\UserInterface\ProtocolStack\PS_698\DL_698.h"
//#include "App\UserInterface\PP\PP_645\PP_ProtocolTree.h"
#include "Lib\LIB_func.h"
#include "system\Sys_def.h"
//#include "Driver\Device\spi_master.h"
#include "PlatForm.h"
#include "host_commu.h"

INT32U stop_for_init;

INT16U rx_tms;
INT16U rx_fault_tms;
INT16U rx_fault1_tms;

//INT16U rx_err_tms;
//INT16U rx_ok_tms;
INT16U ans_user_tms;
INT16U user_pkt_loss;
INT16U user_timeout;

INT16U tx_sec_tms;
INT16U rx_sec_tms;


extern INT16U rx_ok_tms;
extern INT16U rx_err_tms;



ST_7TIME_FORM poweron_rtc_tm;

struct pkt_queue mst_queue[MAX_MASTER_TX_COM_DEV];


static ST_COMMU_DEVICE mstChipDevice[MAX_MASTER_TX_COM_DEV];/*"�豸������Ϣ"*/
//static INT8U ChipCommuMinTimeFlg[MAX_MASTER_TX_COM_DEV];/*"������������С�����־"*/

static INT8U gChip_Buff_Rx[MASTER_TX_COM_BUF_RX_SZ];
static INT8U gChip_Buff_Tx[MASTER_TX_COM_BUF_RX_SZ];

static INT8U reply_pkt[MASTER_TX_COM_BUF_RX_SZ];


const INT8U master_uard_id[MAX_MASTER_TX_COM_DEV] = 
{
        E_UART_ID_MASTER_TX, 
#if (MAX_MASTER_TX_COM_DEV > 1)
        E_UART_ID_FH
#endif
};


void init_queue(struct pkt_queue *q)
{
    q->head = 0;
    q->tail = 0;
    q->qlen = 0;
    q->retrans_tms = 0;
    q->status = CH_IDLE_STATUS;
}

INT8U queue_is_empty(struct pkt_queue *q)
{
    return (q->qlen == 0);
}

INT8U queue_is_full(struct pkt_queue *q)
{
    return (q->qlen == MAX_PKT_NUM);
}

INT8U pkt_inqueue(struct pkt_queue *q, INT8U *data, INT8U len, INT8U src)
{
    INT8U idx;

    if(stop_for_init == STOP_FOR_INIT_FLG)
    {
        return FALSE;
    }
    if(queue_is_full(q) || (len > (HOST_COMMU_PKT_LEN - PKT_DATA_OFF)))
    {
        return FALSE;
    }
    else
    {
        idx = q->tail;
        q->pkt[idx][PKT_SRC_OFF] = src;
        q->pkt[idx][PKT_LEN_OFF] = len;
        LIB_MemCpy(data, &q->pkt[idx][PKT_DATA_OFF], len);
        q->tail = (q->tail + 1) % MAX_PKT_NUM;
        q->qlen++;
        return TRUE;
    }
}

INT8U pkt_inqueue_head(struct pkt_queue *q, INT8U *data, INT8U len, INT8U src)
{
    INT8U idx;
    INT8U end;
    INT8U next;
    INT8U cur_len;
    
    if(queue_is_full(q) || (len > (HOST_COMMU_PKT_LEN - PKT_DATA_OFF)))
    {
        return FALSE;
    }
    else if(q->qlen <= 1)
    {
        idx = q->tail;
        q->pkt[idx][PKT_SRC_OFF] = src;
        q->pkt[idx][PKT_LEN_OFF] = len;
        LIB_MemCpy(data, &q->pkt[idx][PKT_DATA_OFF], len);
        q->tail = (q->tail + 1) % MAX_PKT_NUM;
        q->qlen++;
        return TRUE;
    }
    else
    {
        if(q->tail > 0)
        {
            end = q->tail - 1;
        }
        else
        {
            end = MAX_PKT_NUM - 1;
        }
        while(end != q->head)
        {
            next = (end + 1) % MAX_PKT_NUM;
            cur_len = q->pkt[end][PKT_LEN_OFF] + 2;
            LIB_MemCpy(&q->pkt[end][0], &q->pkt[next][0], cur_len);
            if(end > 0)
            {
                end = end - 1;
            }
            else
            {
                end = MAX_PKT_NUM - 1;
            }
        }
        
        idx = (q->head + 1)% MAX_PKT_NUM;
        q->pkt[idx][PKT_SRC_OFF] = src;
        q->pkt[idx][PKT_LEN_OFF] = len;
        LIB_MemCpy(data, &q->pkt[idx][PKT_DATA_OFF], len);
        q->tail = (q->tail + 1) % MAX_PKT_NUM;
        q->qlen++;
        return TRUE;
    }
}

INT8U pkt_outqueue(struct pkt_queue *q)
{
    if(queue_is_empty(q))
    {
        return FALSE;
    }
    else
    {
        q->retrans_tms = 0;
        q->head = (q->head + 1) % MAX_PKT_NUM;
        q->qlen--;
        return TRUE;
    }    
}

INT8U cfg_645_pkt(INT8U cmd, UN_ID645 id, INT8U *data, INT8U len, INT8U *pkt)
{
    INT8U i;
    INT8U nCheckSum;
    INT8U len_data;
    INT8U has_id;

    if(cmd == 0x11|| cmd == 0x91|| cmd == 0xB1 || cmd == 0x12 || cmd == 0x14
        || cmd == 0x06|| cmd == 0x86)
    {
        len_data = len + 4;
        has_id = 1;
    }
    else
    {
        len_data = len;
        has_id = 0;
    }
    pkt[0] = 0x68;
    if(id.asLong  != ID_SET_BM_ADDR)
    {
        LIB_Reverse(mMtrAddr, pkt + 1, 6);
    }
    else
    {
        LIB_MemSet(0x99, pkt + 1, 6);
    }
	pkt[7] = 0x68;
	pkt[8] = cmd;
	pkt[9] = len_data;
    if(has_id)
    {
        LIB_MemCpy(&id.asBytes[0], &pkt[10], 4);
    }
    if(data != NULL && len != 0)
    {
        if(has_id)
        {
            LIB_MemCpy(data, &pkt[14], len);
        }
        else
        {
            LIB_MemCpy(data, &pkt[10], len);
        }
    }

    if(cmd != 0x05) /*"05�����33"*/
    {
        for(i = 0; i < len_data; i++)
        {
            pkt[10 + i] += 0x33;
        }
    }
	nCheckSum = 0;
	for(i = 0; i< (len_data + 10); i++)
	{		
        nCheckSum += pkt[i];
	}
	pkt[10 + len_data] = nCheckSum;
	pkt[11 + len_data] = 0x16;
    
	return (len_data + 12); 
}

INT8U cfg_retrans_645_pkt(INT8U *pkt)
{
    INT8U nCheckSum;
    INT8U len_data;  

    len_data = pkt[9];
    pkt[12] += 1; /*" DI2 "*/
    nCheckSum = pkt[10 + len_data];
    nCheckSum += 1;
    pkt[10 + len_data] = nCheckSum;
    return 1;
}

/*"*****************************************************************************"*/
/*"  Function:       tx_pkt_to_peer "*/
/*"  Description:    ����������Ľӿ� "*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          id:645���ݱ�־"*/
/*"                  data: Ϊ��������ָ�룻 len: Ϊ�������ݳ��� "*/
/*"  Output:         �� "*/
/*"  Return:         �ɹ�����TRUE    ʧ�ܷ���FALSE       "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U tx_pkt_to_peer(INT8U dev, INT8U cmd, UN_ID645 id, INT8U *data, INT8U len)
{
    INT8U pkt[MASTER_TX_COM_BUF_RX_SZ];
    INT8U pkt_len;
    INT8U ret;
    INT8U src;

    pkt_len = cfg_645_pkt(cmd, id, data, len, pkt);

    src = INTER_MU_PKT;
    ret = pkt_inqueue(&mst_queue[dev], pkt, pkt_len, src);
    return ret;
}

INT16U tx_col_tms;
INT16U tx_reply_tick;

void fastoutput_pkt_to_peer(INT8U *pkt, INT8U len)
{
    if(reply_pkt[0] == 0xAB)
    {
    	tx_col_tms++;

    }
    reply_pkt[0] = 0xAB;
    reply_pkt[1] = len;
    LIB_MemCpy(pkt, &reply_pkt[2], len);
}

INT8U tx_reply_pkt_to_peer(INT8U dev, INT8U cmd, UN_ID645 id, INT8U *data, INT8U len)
{
    INT8U pkt[MASTER_TX_COM_BUF_RX_SZ];
    INT8U pkt_len;

#if 1
    pkt_len = cfg_645_pkt(cmd, id, data, len, pkt);
    fastoutput_pkt_to_peer(pkt, pkt_len);

    return TRUE;

#if 0
    if(mstChipDevice[0].TxLen == 0)
    {
        if(MCU_CmpTick(mstChipDevice[0].send_end_tick, 10) != TRUE)
        {
           // MCU_DelayMs(15);
        }
    }
    else
    {
        tx_col_tms++;
        //MCU_DelayMs(50);
    }
    MCU_DelayMs(100);
    tx_reply_tick = MCU_GetTick();
    Master_TX_Com_StartSend(dev, pkt, pkt_len);
    return TRUE;
#endif

#else
    INT8U ret;
    INT8U src;

    pkt_len = cfg_645_pkt(cmd, id, data, len, pkt);

    src = INTER_ANS_PKT;
    ret = pkt_inqueue(&mst_queue[dev], pkt, pkt_len, src);
    return ret;
#endif
}





/*"*****************************************************************************"*/
/*"  Function:       tx_pkt_to_peer_now "*/
/*"  Description:    ����������Ľӿ� "*/
/*"  Calls:          "*/
/*"  Called By:      ������ģ�麯��"*/
/*"  Input:          id:645���ݱ�־"*/
/*"                  data: Ϊ��������ָ�룻 len: Ϊ�������ݳ��� "*/
/*"  Output:         �� "*/
/*"  Return:         �ɹ�����TRUE    ʧ�ܷ���FALSE       "*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U tx_pkt_to_peer_now(INT8U dev, INT8U cmd, UN_ID645 id, INT8U *data, INT8U len)
{
    INT8U pkt[MASTER_TX_COM_BUF_RX_SZ];
    INT8U pkt_len;

	MCU_DelayMs(10);
    pkt_len = cfg_645_pkt(cmd, id, data, len, pkt);

    Master_TX_Com_StartSend(dev, pkt, pkt_len);
    MCU_DelayMs(10);
    return TRUE;
}


/*" �ж��Ƿ��ǶԶ˵����� "*/
INT8U is_peer_data(INT8U dev, INT8U com, ST_FRAME645 *res)
{
    INT8U di3,di0;
    INT8U ret;
    INT32U id;
    INT32U id3_1;

    ret = 0;
    id = res->dataId.asLong;
    di3 = res->dataId.asBytes[3];
	di0 = res->dataId.asBytes[0];
    id3_1 = (id & 0xFFFFFF00);
    if(dev == 0)
    {
        if( res->command == 0x15) /*" ȡ��ַ ���ַ"*/   //res->command == 0x13 ||
        {
            ret = 1;
        }
        else if(res->command == 0x1A || res->command == 0x1B)
        {/*" 05У��; 1A �������; 1B �¼����� "*/
            ret = 2;
        }
        else if(id == 0x04000101 || id == 0x04000102 || id == 0x0400010C /*" ʱ��"*/
            || id == 0x04000401 || (di3 == 0x08) /*" ��ַ���������"*/
            //|| (di3 == 0x02 && id <= 0x0280000A) /*" ˲ʱ��"*/
            /*" ����/����/Уʱ"*/
            || (id3_1 == 0x03300d00) || (id3_1 == 0x03110000) || (id3_1 == 0x03300400) 
            || (0x0E180000 == (id & 0xffff0000)) || (id == 0x0E200040)) 
        {
            ret = 1;
        }

        if(res->authority >= T2_CLASS)
        {
            ret = 0;
        }
        else
        {
#ifdef MAIN_METER
            if(res->command == 0x14)
            {
                if(id == 0x04000101 || id == 0x04000102 || id == 0x0400010C /*" ʱ��"*/
                    || id == 0x04000401) /*" ��ַ "*/
                {
                    ret = 0; /*" Ҫ�����õ����ˣ������õ��Զ� "*/
                }
            }
            if(res->command == 0x15 /*" ���ַ "*/
                || res->command == 0x1A || res->command == 0x1B) /*" 1A �������; 1B �¼����� "*/
            {
                ret = 0; /*" Ҫ�����õ����ˣ������õ��Զ� "*/
            }
#endif
        }
		
#if DL645_FLOW_REPORT
		if((reportComBegin[com] == 1) && (res->command == 0x12))
		{
			ret = 1;
		}
#endif
#ifdef MAIN_METER
        return ret;
#else
        return !ret;
#endif
    }
    else
    { /*" �Ǹ���ʶ������� "*/
        if(((id3_1 == 0x07000000)&&(di0<=0x02)) || id3_1 == 0x04FB0000 || (res->dataLen == 0Xa6 && res->command == 0x05))
        {
            ret = 1;
        }
        return ret;
    }
}

INT16U MCU_GetNonZeroTick(void)
{
    INT16U tick;

    tick = MCU_GetTick();
    if(tick == 0)
    {
        tick = 1;
    }
    return tick;
}




extern INT8U Check_Address(INT8U *rpAddress);
extern void ParseFrame(INT8U *rpFrame, ST_FRAME645 *rpResult);

/*" ����û�������ı����Ƿ���Ҫ����������ǲ�����������Ӧ�� "*/
INT8U user_req_peer_data_proc(INT8U dev, INT8U com, INT8U *frm, INT8U frmlen)
{
    INT8U ret;

    ret = pkt_inqueue(&mst_queue[dev], frm, frmlen, com);
    if(ret == TRUE)
    {
        /*" ��ȡ����Ӧ��ʱʱ��tick "*/
        mst_queue[dev].user_reply_tick[com] = MCU_GetNonZeroTick();
    }
    else
    {
		if(dev == 0)
        {
            user_pkt_loss++;
        }
    }
    return 1;
}

/*" �û��ڴ�������Զ�����Ӧ��Ĺ��̣���ʱ�ܾ��û��ں������� "*/
INT8U is_in_wait_peer_reply(INT8U com)
{
    INT8U dev;
    INT8U has;

    has = 0;
    for(dev=0;dev<MAX_MASTER_TX_COM_DEV;dev++)
    {
        if(mst_queue[dev].user_reply_tick[com] != 0)
        {
            has = 1;
        }
    }
    if(has != 0)
    {
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}
/*" �û������Ļ���Ӧ��ʱ���� "*/
void user_req_reply_timeout_proc(INT8U dev)
{
    INT8U com;

    for(com = 0; com < E_MAX_COM_ID; com++) /* todo */
    {
        if(mst_queue[dev].user_reply_tick[com] != 0)
        {
            if(TRUE == MCU_CmpTick(mst_queue[dev].user_reply_tick[com], PEER_REPLY_TIMEOUT))
            {
                /*" ����Ӧ�û���ת��Ϊʹ�ܽ��� QQQ ��תҲû�� "*/
                //COM_RxTx_Ctrl(com, TRANSTA_SC_RO);
                mst_queue[dev].user_reply_tick[com] = 0;
                mst_queue[dev].send_tick = 0;
				if(dev == 0)
                {
				    user_timeout++;
                }
            }
        }
    }
}


/* ͨ�����ϻָ�����*/
void Master_TX_Com_fault_recover(INT8U dev)
{
    if(mst_queue[dev].status == CH_FAULT_STATUS)
    {
        /*" �ڲ�ͨ�ſ�(SPI��UART)���³�ʼ�� "*/
        Master_TX_Com_Init(dev);
    }
}


INT16U tx_tms;

void pkt_queue_tx_task(INT8U dev)
{
    INT16U timeout;
    INT8U *pkt;
    INT8U len;
    INT8U src;
    struct pkt_queue *queue;

    queue = &mst_queue[dev];
    pkt = &queue->pkt[queue->head][PKT_DATA_OFF];
    len = queue->pkt[queue->head][PKT_LEN_OFF];
    src = queue->pkt[queue->head][PKT_SRC_OFF];

    if((mstChipDevice[0].send_end_tick != 0) && (MCU_CmpTick(mstChipDevice[0].send_end_tick, 150) == TRUE))
    {
        if(reply_pkt[0] == 0xAB)
        {
            reply_pkt[0] = 0;
            tx_reply_tick = MCU_GetTick();
            mstChipDevice[0].send_end_tick = 0;
            Master_TX_Com_StartSend(dev, &reply_pkt[2], reply_pkt[1]);
        }
    }
            
    if((queue->status == CH_IDLE_STATUS) && (queue_is_empty(queue) == 0)
        && (mstChipDevice[0].send_end_tick != 0) && (MCU_CmpTick(mstChipDevice[0].send_end_tick, 150) == TRUE))
    {
        mstChipDevice[0].send_end_tick = 0;
        /* todo �������� */
        Master_TX_Com_StartSend(dev, pkt, len);
        queue->send_tick = MCU_GetNonZeroTick();

        queue->status = CH_BUSY_STATUS;
    }
    else if(queue->status == CH_BUSY_STATUS)
    {
        //if( (src == LD_REPLY_PKT) && (queue->send_tick != 0) && (TRUE == MCU_CmpTick(queue->send_tick, WAIT_LD_PKT_TX_TIMEOUT)))
        //{
        //    pkt_outqueue(queue);
        //    queue->send_tick = 0;
        //    queue->status = CH_IDLE_STATUS;
        //     return;
        //}
        
        if(src == INTER_MU_PKT)
        {
            timeout = 1000;
        }
        else
        {
            timeout = WAIT_ANS_TIMEOUT;
        }
        if( (queue->send_tick != 0) && (TRUE == MCU_CmpTick(queue->send_tick, timeout)))
        {
            queue->retrans_tms++;
            if(queue->retrans_tms > MAX_RETRANS_TMS)
            {
                pkt_outqueue(queue);
            }
            queue->send_tick = 0;
            queue->status = CH_IDLE_STATUS;
            if(dev == 0)
            {
                rx_err_tms++;
            }
        }
    }
    else if((dev == 0) && (stop_for_init == STOP_FOR_INIT_FLG) && (queue_is_empty(queue) != 0))
    {
        MTR_INIT();
    }

    user_req_reply_timeout_proc(dev);

    Master_TX_Com_fault_recover(dev);
}


INT8U is_bm_para_data(INT32U id)
{
    if((id == 0x04000101) || (id == 0x04000102) || (id == 0x04000401))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

INT8U is_only_bm_para_data(INT32U id)
{
    if(id == 0x04000409) /*" ����й�����"*/
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/*" ����ֵ: 0��ʾ�����⴦��  ; ���򷵻��޸ĺ�ı��ĳ��� "*/
INT8U do_master_reply_spec_proc(ST_FRAME645 *res)
{
    INT8U new_len;
#if DL645_READ_LIST
    INT8U buf[200];
    ST_ANSWER_DATA ans;
    
    INT8U cmd;
    INT8U ret;
#endif
#ifdef BASE_METER
    INT8U err;
    INT32U id;
#endif

    new_len = 0;
#if DL645_READ_LIST
    ans.Data_addr = buf;

    if((res->dataId.asLong & 0xFFFFFF00UL) == 0xEEEEEE00UL)
    {
        if(ListDataIsAllPeerData() == TRUE)
        {
            return 0;
        }
        FrameDataDec0x33(res);
        ret = ReplyUserIdData(res, &ans);
        if(ret == COMM_ERR_NOERR)
        {
            cmd = 0x11 + 0x80;
        }
        else
        {
            cmd = 0x11 + 0xC0;
        }
        new_len = cfg_645_pkt(cmd, res->dataId, buf, ans.Data_len.int_len, res->beginData);
        return new_len;
    }
#endif

#ifdef BASE_METER
    id = res->dataId.asLong;
    //dir = res->command & 0x80;
    cmd = res->command & 0x1F;
    err = res->command & 0x40;
    new_len = 0;
    if(err == 0) /*" Ϊ����Ӧ��֡ "*/
    { 
        switch(cmd)
        {
            case 0x1A: /*" ������� "*/
                CLR_MTR();
                break;
            case 0x1B: /*" �¼����� "*/
                buf[0] = 0xfe;
                buf[1] = 0xff;
                Action(E_RCRD_CLR, &buf[0], NULL);
                break;
            case 0x15: /*" д���ַ "*/
                FrameDataDec0x33(res);
                SetSingle(E_COMM_ADDR, res->data, 6);
                buf[0] = 0;
                new_len = cfg_645_pkt(0x95, res->dataId, buf, 1, res->beginData);
                break;
            case 0x14:
                if(is_bm_para_data(id))
                {
                    FrameDataDec0x33(res);
                    //setSysTimeYMDW(res->data); /*" HEX��ʽʱ�� "*/
                    ret = SetParaToMter(res);
                    if(ret == COMM_ERR_NOERR)
                    {
                        buf[0] = 0;
                        new_len = cfg_645_pkt(0x94, res->dataId, buf, 1, res->beginData);
                    }
                    else
                    {
                        buf[0] = COMM_ERR_OTHERS;
                        new_len = cfg_645_pkt(0xD4, res->dataId, buf, 1, res->beginData);
                    }
                }
                break;
        }
    }
#endif
    return new_len;
}

#ifdef HOST_USE_UART
/* ���յ�SlaveӦ����ʱ�����д���*/
void rx_slave_reply_outqueue(void)
{
    INT8U src;
    INT8U one;
    struct pkt_queue *queue;

    queue = &mst_queue[0];
    one = 0;
    while(queue->head != queue->tx) /* ������ͳ�ȥ�Ķ���Ӧ����*/
    {
        src = queue->pkt[queue->head][PKT_SRC_OFF];
        if(src != INTER_ANS_PKT && one == 1)
        {
            break;
        }
        else if(src != INTER_ANS_PKT)
        {
            one = 1;
        }
        pkt_outqueue(queue);
        queue->send_tick = 0;
        queue->user_reply_tick[0] = 0; /* ����E_COM_SPI ��TODO  */
        
    }
}

/*" ����û��Ӧ����ڲ�ͨ��Ӧ����ʱ����"*/
void tx_inter_ans_pkt_outqueue(void)
{
    INT8U src;
    struct pkt_queue *queue;

    queue = &mst_queue[0];
    while(queue->head != queue->tx) /* ������ͳ�ȥ�Ķ���Ӧ����*/
    {
        src = queue->pkt[queue->head][PKT_SRC_OFF];
        if(src != INTER_ANS_PKT)
        {
            break;
        }

        pkt_outqueue(queue);
        queue->send_tick = 0;
        queue->user_reply_tick[0] = 0; /* ����E_COM_SPI ��TODO  */
    }
}

INT16U lst_rx_tick;
INT8U lst_rx_idx;
INT16U rx_frm_err;

INT8U host_commu_ok(void)
{
    if((lst_rx_tick != 0) && (MCU_CmpTick(lst_rx_tick, 1000) != TRUE))
    {
        Push(E_MGMT_COMM_ERR, FALSE);
        return TRUE;
    }
    else
    {
        Push(E_MGMT_COMM_ERR, TRUE);
        return FALSE;
    }
}


void PushKeyMsg(INT8U key)
{
    UN_ID645 id;
    INT8U buf[20];
    //INT8U len;
    
    id.asLong = ID_BM_KEY_DATA;
    //len = get_show_data(buf);
    //len += GetSingle(E_DOWNDATA, buf + len);
    //len += get_clr_relay_cmd(buf + len);
    //len += get_clr_led_cmd(buf + len);
    //len = get_bm_lrm_data(buf);
    buf[0] = key;
    tx_pkt_to_peer(0, 0x06, id, buf, 1);
}

void key_msg_proc(void)
{
    INT8U keyId;
    
    keyId = KEY_GetKey();
    if(keyId == KEY_ID_INVALID)
    {
        return;
    }
    switch(keyId)
    { /*" 03Ϊ����3�룬04Ϊ����5�� "*/
        case KEY_ID_DISP1:
            break;
        case KEY_ID_DISP1_RLY_CLS:
            PushKeyMsg(0x03);
            break;
        case KEY_ID_SW_MULT_FUNC:
            PushKeyMsg(0x04);
            break;
        default:
            break;
    }
}


extern void SetShowData(INT8U *buf);

INT8U do_main_meter_ao_cmd(INT8U cmd_len, INT8U *buf)
{
    AO_CMD *cmd;
    LED_MODE_EN mode;
    INT8U rly_cmd;
    EN_OC_RELAYACT act;
    INT8U i;
    INT8U num;

    num = cmd_len/4;
    for(i = 0; i < num; i++)
    {
        cmd = (AO_CMD *)(buf + i*4);
        if(cmd->cmd == 1)
        {
            mode = E_LED_ALWAYS_MODE;
        }
        else if(cmd->cmd == 2)
        {
            mode = E_LED_TWINKLE_MODE;
        }
        switch(cmd->id)
        {
            case 1:/*" ����"*/
                OC_LedOnSingle(E_ID_LED_BGD, mode, cmd->tms);
                break;
            case 2:/*" ��բ��"*/
                OC_LedOnSingle(E_ID_LED_RELAY, mode, cmd->tms);
                break;
            case 3:/*" ��բ�̵���"*/
                rly_cmd = cmd->cmd >> 4;
                if(rly_cmd == 1)
                {
                    act = RELAY_ACT_TRIP;
                }
                else if(rly_cmd == 2)
                {
                    act = RELAY_ACT_CLOSE;
                }
                OC_RelayAct(E_RELAY_SC_PREPAY,E_RELAY_BREAK,E_RELAY_MODE_INSIDE, act, 400);
                break;
            default:
                break;
        }
    }
    return TRUE;
}



/*" UART2�������Ϸ�645����ʱ���� "*/
INT8U pkt_queue_rx_proc(ST_FRAME645 *res, INT8U len, INT8U dev)
{
    INT8U new_len;
    INT8U com;
    INT8U pkt_len;
    struct pkt_queue *queue;
    UN_ID645 id;
    //ST_7TIME_FORM now;

	lst_rx_tick = MCU_GetTick();
    queue = &mst_queue[dev];
    rx_tms++;
    if((res->command & 0x1F) == 0x06)
    {
        if(res->command & 0x80) /*"�յ� Ӧ����"*/
        {
            switch(res->dataId.asLong)
            {
                case ID_BM_MMT_DATA:
                    rx_sec_tms++;
                    break;
                case ID_BM_KEY_DATA:
                    SetShowData(res->data);
                    break;
            }
            //rx_slave_reply_outqueue();
            user_req_pkt_outqueue();
        }
        else /*" �����ģ���ȡ��������뷢�Ͷ���"*/
        {
            id.asLong = res->dataId.asLong;
            switch(res->dataId.asLong)
            {
                case ID_SHOW_PUSH:
                    //FrameDataDec0x33(res);
                    //set_show_data(res->data);
                    SetShowData(res->data);
                    tx_reply_pkt_to_peer(0, 0x86, id, NULL, 0);
                    break;
                case ID_AO_CTRL:
                    do_main_meter_ao_cmd(res->dataLen, res->data);
                    tx_reply_pkt_to_peer(0, 0x86, id, NULL, 0);
                    break;
                case ID_SET_SHOW_PARA:
                    tx_reply_pkt_to_peer(0, 0x86, id, NULL, 0);// need to do 
                    break;
                case ID_RTC_SYNC:
                    break;
                case ID_INIT_MTR:
                    MTR_INIT();
                    break;
            }
            
        }
        return 1;
    }

    //tx_inter_ans_pkt_outqueue();
    //user_req_pkt_outqueue();

    /*" �û��ڱ���͸��"*/
    new_len = do_master_reply_spec_proc(res);
    if(queue->head >= MAX_PKT_NUM)
    {
        return 1;
    }
    /*" ���������Ĵ�Դ�˿ڷ��ͳ�ȥ "*/
    com = queue->pkt[queue->head][PKT_SRC_OFF];
    if(new_len != 0)
    {

        pkt_len = new_len;
    }
    else
    {
        pkt_len = len;
    }
    if((com < E_MAX_COM_ID) && (queue->user_reply_tick[com] != 0))
    {
        queue->user_reply_tick[com] = 0;
#if DL645_FLOW_REPORT
		/*" TODO ����Ƕ�������û�к���֡��Ҫ�����޸����ϱ�"*/
        if(((res->command & 0x20) == 0) && (((res->command & 0x1F) == 0x11) || ((res->command & 0x1F) == 0x12)))
        {
            INT8U reportFlgTmp;
            UN_BYTE_DEF  mtrRunModeTmp; 

            /*�����ϱ�����֡*/
            GetSingle(E_MTR_RUN_MODE, &mtrRunModeTmp.byte);    	
            reportFlgTmp       = m645EvtPrtStatus.rptFlg;
            if((1==reportFlgTmp)&&(1==mtrRunModeTmp.bit.b2))
            {
                res->beginData[8] += 0x20; /*" ����֡"*/
                res->beginData[pkt_len - 2] += 0x20; /*" sum����"*/
                reportComBegin[com] = 1;
            }
        }
#endif
        Com_Send_data(com, res->beginData, pkt_len);
    }
            
    //rx_slave_reply_outqueue();
    user_req_pkt_outqueue();
    return 1;
}

#else

#endif

/*" �յ�������ԪӦ��󣬽����ĴӶ���ժ��"*/
void user_req_pkt_outqueue(void)
{
    struct pkt_queue *queue;

    queue = &mst_queue[0];
    pkt_outqueue(queue);
    queue->send_tick = 0;
    queue->status = CH_IDLE_STATUS;
    queue->user_reply_tick[0] = 0; /* ����E_COM_SPI ��TODO  */
}



void Master_TX_Com_RX_INT(void)
{
    ST_COMMU_DEVICE *pDevice;

    pDevice = &mstChipDevice[0];

    if(pDevice->RxLen >= MASTER_TX_COM_BUF_RX_SZ)
    {   
        pDevice->RxLen = 0; 
    }	
    pDevice->RxBuff[pDevice->RxLen] = UART_Receive1Byte(E_UART_ID_MASTER_TX); /*"���������ֽڸ������ջ�����"*/
    pDevice->RxLen++;                        /*"���յ��ĳ���+1"*/	
    
    pDevice->InactivityTimer = MCU_GetNonZeroTick();
    pDevice->InterOctetTimer = MCU_GetNonZeroTick();/*"���¸��ֽڼ䳬ʱʱ�丳ֵ"*/    
}

void Master_TX_Com_TX_INT(void)
{
    ST_COMMU_DEVICE *pDevice;

    pDevice = &mstChipDevice[0];
    
    if(pDevice->TxLen > 0)
    {
        UART_Send1Byte(E_UART_ID_MASTER_TX, pDevice->TxBuff[pDevice->tx_off]);
        pDevice->tx_off++;
        pDevice->TxLen--;
    }
    else
    {
        pDevice->send_end_tick = MCU_GetTick();
    }
}

#if (MAX_MASTER_TX_COM_DEV > 1)
void Master_LD_Com_RX_INT(void)
{
    ST_COMMU_DEVICE *pDevice;

    pDevice = &mstChipDevice[1];

    if(pDevice->RxLen >= MASTER_TX_COM_BUF_RX_SZ)
    {   
        pDevice->RxLen = 0; 
    }	
    pDevice->RxBuff[pDevice->RxLen] = UART_Receive1Byte(E_UART_ID_MASTER_LD); /*"���������ֽڸ������ջ�����"*/
    pDevice->RxLen++;                        /*"���յ��ĳ���+1"*/	
    
    pDevice->InactivityTimer = MCU_GetNonZeroTick();
    pDevice->InterOctetTimer = MCU_GetNonZeroTick();/*"���¸��ֽڼ䳬ʱʱ�丳ֵ"*/    
}

void Master_LD_Com_TX_INT(void)
{
    ST_COMMU_DEVICE *pDevice;

    pDevice = &mstChipDevice[1];
    
    if(pDevice->TxLen > 0)
    {
        UART_Send1Byte(E_UART_ID_MASTER_LD, pDevice->TxBuff[pDevice->tx_off]);
        pDevice->tx_off++;
        pDevice->TxLen--;
    }
    else
    {
        
    }
}
#endif

void Master_TX_Com_StartSend(INT8U dev, INT8U *data,INT16U len)
{
    ST_COMMU_DEVICE *pDevice;

    if(dev == 0)
    {
    tx_tms++;
    }

    pDevice = &mstChipDevice[dev];
    
    LIB_MemCpy(data, pDevice->TxBuff, len);
    pDevice->TxLen = len;
    pDevice->tx_off = 0;
    //Master_TX_Com_TX_INT();
    UART_Send1Byte(master_uard_id[dev], pDevice->TxBuff[0]);//dev
    pDevice->tx_off++;
    pDevice->TxLen--;
}

void Master_TX_Com_Init(INT8U dev)
{
    ST_UARTCONFIG UartConf;

    /*" ������� "*/
    init_queue(&mst_queue[dev]);
    
    LIB_MemSet(0, (INT8U *)&mstChipDevice[dev], sizeof(mstChipDevice));
    mstChipDevice[0].send_end_tick = 1;
    mstChipDevice[dev].RxBuff = gChip_Buff_Rx;
    mstChipDevice[dev].TxBuff = gChip_Buff_Tx;    

    UartConf.Baud = E_BAUD115200;/*E_BAUD115200*/
    UartConf.TranSta.byte = TRANSTA_SO_RO;
    if(dev == 0)
    {
        Push(E_MGMT_COMM_ERR, TRUE);
        UartConf.ReceiveIsr = Master_TX_Com_RX_INT;
        UartConf.SendIsr = Master_TX_Com_TX_INT;
    }
#if (MAX_MASTER_TX_COM_DEV > 1)
    else
    {
   		UartConf.Baud = E_BAUD9600;
        UartConf.ReceiveIsr = Master_LD_Com_RX_INT;
        UartConf.SendIsr = Master_LD_Com_TX_INT;
    }
#endif
    UART_Open(master_uard_id[dev], &UartConf);
}

/*"ʱ���ȽϺ�����ģ��task�е���һ��"*/
static void Master_TX_Com_TimeOut(void)
{
    INT8U src;
    INT8U DeviceIndex; 
    ST_COMMU_DEVICE *pDevice;
    struct pkt_queue *queue;

    queue = &mst_queue[0];
    
    src = queue->pkt[queue->head][PKT_SRC_OFF];
    if((mst_queue[0].send_tick != 0) && (TRUE == MCU_CmpTick(mst_queue[0].send_tick, 30))
        && (src == INTER_ANS_PKT))
    {
        user_req_pkt_outqueue();
        mst_queue[0].send_tick = 0;
        mst_queue[0].status = CH_IDLE_STATUS;
    }

    for(DeviceIndex = 0; DeviceIndex < MAX_MASTER_TX_COM_DEV; DeviceIndex++)
    {
        pDevice = &mstChipDevice[DeviceIndex];

        /*"�ֽڼ䳬ʱ"*/
        if( (pDevice->InterOctetTimer > 0))
        {	
            if(TRUE == MCU_CmpTick(pDevice->InterOctetTimer, MASTER_TX_COM_OCTET_TIMEOUT))
            {
                pDevice->InterOctetTimeout = TRUE;

                //pDevice->InterruptClose = TRUE;/*"���ô��ڵ��жϹرգ������ڽ��յ����ֽڲ��ܹ��ڸ���Device[i]�Ľ��ջ�����"*/						
                pDevice->InterOctetTimer = 0;       /*"���ֽڼ䳬ʱ��0"*/  
                //ChipCommuMinTimeFlg[DeviceIndex] = TRUE;
            }		
            else
            {
                //ChipCommuMinTimeFlg[DeviceIndex] = FALSE;
            }
        }	
        
        if(pDevice->InactivityTimer > 0)/*"֡��ʱ"*/
        {
            if(MCU_CmpTick(pDevice->InactivityTimer, MASTER_TX_COM_INACTIVITY_TIMEOUT) == TRUE)
            {		
                pDevice->InactivityTimer = 0;
                pDevice->InactivityTimeout = TRUE;			
            }	
        }
    }
}

#if 0
static INT8U Master_TX_Com_AnswerCmd(INT8U *pdataBuff, INT8U len, INT8U dev)
{
    ST_FRAME645 nResult;

    /*"����֡������Ϣ����"*/
    LIB_MemSet(0, (INT8U *)&nResult, sizeof(ST_FRAME645));
    ParseFrameNoMdf(pdataBuff, &nResult);

    /*" UART2���յ��Ļ���Ӧ�� "*/
    pkt_queue_rx_proc(&nResult, len, dev);

    /*" TODO ����յ����ǻ����ĵ�Ӧ��Ҫ���û���ת����ȥ "*/
    return CM_UNUSUAL;
}
#endif

/*"������յ�һ֡���ݣ��Ը�֡���д���"*/
INT8U Master_TX_Com_ProcessRecBytes(INT8U *pDataBuff, INT16U length)
{
    INT8U uni, nCheckSum, FrameLen, nFrameResult;
    INT8U i;
    //INT8U AddrType;

    nFrameResult = 0;

    for(i = 0; i <= (length - 12); i++)/*"12: ��data����ֽڳ���"*/
    {
        if((pDataBuff[i] == 0x68) && (pDataBuff[i + 7] == 0x68))
        {
            FrameLen = pDataBuff[i + 9] + 12;
            if((i + FrameLen) > length)
            {/*"�ѽ��յ����ݲ�������֡���ų������룬������֡ͷ"*/
                continue;
            }
            else if((FrameLen <= length) && (pDataBuff[i + FrameLen - 1] == 0x16))
            {
                nCheckSum = 0;
                for(uni = 0; uni < (FrameLen - 2); uni++)
                {
                    nCheckSum += pDataBuff[i + uni];
                }
                if(nCheckSum != pDataBuff[i + FrameLen - 2])/*"У��"*/
                {
                    continue;
                }
            }
            else
            {
                continue;
            }
            /*"�Ƚ�ͨ�ŵ�ַ���˴�ʡ��"*/
#if 0
            AddrType = Check_Address(&pDataBuff[i + 1]);
            if(ADDR_UNRESOLVED == AddrType)
            {
                continue;   /*"ͨ�ŵ�ַ��ʱ�˳���������֡ͷ"*/ 
            }
            else
            {
            }
#endif
            if(0 != i)
            {/*"����ʱ��֡ͷ������0��ʼ"*/
                LIB_MemCpy(&pDataBuff[i], &pDataBuff[0], FrameLen);
            }

            /*"��������"*/
            
            nFrameResult = FrameLen;
            
            break;
        }
    }

    return nFrameResult;
}


 extern INT8U CM_AnswerCmd(INT8U rCommPort, INT8U *pdataBuff, INT8U len);

 /*"�������Ϳ�����"*/
void Master_TX_Com_Task(void)
{
    INT8U buff[MASTER_TX_COM_BUF_RX_SZ];
    INT16U lenTmp;
    INT8U dev; 
    INT8U len;
    INT8U reslen;

    host_commu_ok();

    key_msg_proc();
    
    //mu_commu_task();
//#if (MAX_MASTER_TX_COM_DEV > 1)    
    //Load_identify_main();
    Master_TX_Com_TimeOut();
    for(dev=0;dev<MAX_MASTER_TX_COM_DEV;dev++)
    {
        pkt_queue_tx_task(dev);
        
        /*"ͨ���ܳ�ʱ"*/	
        if(TRUE == mstChipDevice[dev].InactivityTimeout)
        {	
            mstChipDevice[dev].TxWaiting = FALSE;  
            mstChipDevice[dev].InactivityTimeout = FALSE;	
            //SetComStatus(E_DLT645,mstChipDevice[DeviceIndex].CurrentDeviceId,COM_STATUS_RELEASE);
        }	
    	
        /*"�����ֽڼ䳬ʱ"*/		
        if((TRUE == mstChipDevice[dev].InterOctetTimeout))
        { 
            /*"�ȸ������ݣ��ٸ�λ�շ�������"*/	
            lenTmp = mstChipDevice[dev].RxLen;
            if(lenTmp < MASTER_TX_COM_BUF_RX_SZ)
            {
                LIB_MemCpy(mstChipDevice[dev].RxBuff, buff, lenTmp);
            }
            /*"���ջ���������"*/
            mstChipDevice[dev].InterOctetTimer = 0;
            mstChipDevice[dev].InterOctetTimeout = FALSE;
            mstChipDevice[dev].RxLen = 0;
            //mstChipDevice[DeviceIndex].InterruptClose = FALSE;	
            /*"�������֡,���ؽ������"*/
            len = Master_TX_Com_ProcessRecBytes(buff,lenTmp);
            if(len > 0)
            {
                reslen = CM_AnswerCmd(E_COM_SLAVE, buff, len);
                if(reslen != CM_UNUSUAL)
                {
                    fastoutput_pkt_to_peer(buff, reslen);
                }
            }
            else
            {
                rx_frm_err++;
            }
        }
    }
//#endif
}



extern INT16U get_bm_lrm_data(INT8U *buf);
extern INT16U get_bm_frz_data(INT8U *buf);
#if 1
COMM_OBIS sec_data[] =
{
    {E_METER_SAMPLE_STATUS, E_U_2_BYTE, 0},
    {E_SAMPLE_STATUS, E_U_2_BYTE, 0},
    {E_TMP_LINE_IN, E_BCD_2_BYTE, -1},
    {E_TMP_LINE_OUT, E_BCD_2_BYTE, -1},
    {E_TMP_N_IN, E_BCD_2_BYTE, -1},
    {E_TMP_N_OUT, E_BCD_2_BYTE, -1},
    {E_MNT_MTR_TMP, E_BCD_2_BYTE, -1},
    {E_MNT_MTR_HMDTY, E_UBCD_2_BYTE, -2},

    {E_CLK_BAT_VOLT, E_UBCD_2_BYTE, -2},
    {E_CURR_RESIDUAL, E_BCD_4_BYTE, -4},
    {E_VOLT_LA, E_UBCD_2_BYTE, -1},
    {E_CURR_LA, E_BCD_4_BYTE, -4},
    {E_ACTIVE_PWR, E_BCD_4_BYTE, -4},
    {E_REACTIVE_PWR, E_BCD_4_BYTE, -4},
    {E_FACTOR, E_BCD_4_BYTE, -4},
    {E_FREQ, E_UBCD_2_BYTE, -2},
    {E_PA_ENGY, E_UBCD_5_BYTE, -4},
    {E_NA_ENGY, E_UBCD_5_BYTE, -4},
    {E_R_ENGY_I, E_UBCD_5_BYTE, -4},
    {E_R_ENGY_II, E_UBCD_5_BYTE, -4},
    {E_R_ENGY_III, E_UBCD_5_BYTE, -4},
    {E_R_ENGY_IV, E_UBCD_5_BYTE, -4},
    //{E_A_VOLT_CONT_TOTAL, E_UBCD_2_BYTE, -2},
    //{E_A_CURR_CONT_TOTAL, E_UBCD_2_BYTE, -2},
};

//INT8U per_o_len[40];
extern void Lrm_test(void);

INT16U get_comm_data(INT8U *buf, COMM_OBIS *tbl, INT8U num)
{
    INT8U BuffTemp[100];
    INT8U i;
    INT16U obis;
    INT16S res;
    ST_FMT_ATTR src_fmt;
    ST_FMT_ATTR dst_fmt;
    INT8U per_dst_len;
    INT16U len;
    Lrm_test();

    len = 0;
    for(i=0; i<num; i++)
    {
        obis = tbl[i].obis;
        res = GetSingle(obis , BuffTemp);
        if(res < MS_SUCCESS)
        {
            break;
        }
        GetFrmt(obis, &src_fmt); /*" ԭʼ��ʽ "*/
        LIB_MemCpy((INT8U *)&src_fmt, (INT8U *)&dst_fmt, sizeof(ST_FMT_ATTR));
        dst_fmt.format = tbl[i].dst_fmt;
        dst_fmt.exp = tbl[i].exp;
        FRMT_DataChange(BuffTemp, buf + len, src_fmt, dst_fmt);
        per_dst_len = FRMT_sizeofFormat(dst_fmt.format);
        len += per_dst_len;
        //per_o_len[i] = per_dst_len;
    }
    return len;
}

INT32U engy=0;

INT16U get_bm_lrm_data(INT8U *buf)
{
    INT16U len;
    INT32U data;

    //Lrm_test();
    len = 0;
    GetSingle(E_METER_SAMPLE_STATUS, buf);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2;
    GetSingle(E_SAMPLE_STATUS, buf + len);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2;

    GetSingle(E_TMP_LINE_IN, buf + len);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2;
    
    GetSingle(E_TMP_LINE_OUT, buf + len);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2;
    GetSingle(E_TMP_N_IN, buf + len);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2;
    GetSingle(E_TMP_N_OUT, buf + len);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2;

    GetSingle(E_MNT_MTR_TMP, buf + len);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2;
    GetSingle(E_MNT_MTR_HMDTY, buf + len);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2;

    data=0;
    GetSingle(E_CLK_BAT_VOLT, (INT8U *)&data);
    data=data/10;
    LIB_MemCpy((INT8U *)&data, buf + len, 2);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2;

    GetSingle(E_CURR_RESIDUAL, buf + len);
	LIB_CharToBcdNByte(buf+len, 4);
    len += 4;
    
    GetSingle(E_VOLT_LA, buf + len);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2;
    GetSingle(E_CURR_LA, buf + len);
	INT32U curr=5000;
	LIB_MemCpy((INT8U *)&curr, buf + len, 4);
	LIB_CharToBcdNByte(buf+len,4);
    len += 4;   

    GetSingle(E_ACTIVE_PWR, buf + len);
	INT32U pwr=11000;
	LIB_MemCpy((INT8U *)&pwr, buf + len, 4);
	LIB_CharToBcdNByte(buf+len, 4);
    len += 4; 
    GetSingle(E_REACTIVE_PWR, buf + len); 
	LIB_CharToBcdNByte(buf+len, 4);
    len += 4; 

    GetSingle(E_FACTOR, buf + len);
	INT32U fact=10000;
	LIB_MemCpy((INT8U *)&fact, buf + len, 4);
	LIB_CharToBcdNByte(buf+len, 4);
    len += 4; 

    data = 0;
    GetSingle(E_FREQ, (INT8U *)&data);
    data /= LIB_Pow10(-2 - CURR_DECIMAL_POINT);
    LIB_MemCpy((INT8U *)&data, buf + len, 2);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2; 

    GetSingle(E_PA_ENGY, buf + len);
	engy+=1111;
	LIB_MemCpy((INT8U *)&engy, buf + len, 5);
	LIB_CharToBcdNByte(buf+len, 5);
    len += 5; 
	GetSingle(E_NA_ENGY, buf + len);
	LIB_CharToBcdNByte(buf+len, 5);
    len += 5;
    #if (REACTIVE_ENGY_EN == 1)
    /*"�������޹�����  "*/
    GetSingle(E_R_ENGY_I, buf + len);
	LIB_CharToBcdNByte(buf+len, 5);
    len += 5;
    GetSingle(E_R_ENGY_II, buf + len);
	LIB_CharToBcdNByte(buf+len, 5);
    len += 5;
    GetSingle(E_R_ENGY_III, buf + len);
	LIB_CharToBcdNByte(buf+len, 5);
    len += 5;
    GetSingle(E_R_ENGY_IV, buf + len);
	LIB_CharToBcdNByte(buf+len, 5);
    len += 5;
    #endif

#if (HARMONIC_MODE == 1)
    /*"A���ѹ��������г��������"*/
#if 0
    GetSingle(E_A_VOLT_CONT_TOTAL, buf + len);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2;
    GetSingle(E_A_CURR_CONT_TOTAL, buf + len);
	LIB_CharToBcdNByte(buf+len, 2);
    len += 2;
	#endif
#endif
    
    return len;
}

INT32U engyMin;

INT16U get_bm_frz_data(INT8U *buf)
{
    INT16U len;
    INT32U data;
    //INT8U data[32];

    len = 0;
    GetSingle(E_VOLT_LA, buf + len);
    len += 2;
    GetSingle(E_CURR_LA, buf + len);
    INT32U curr=10000;
    LIB_MemCpy((INT8U *)&curr, buf + len, 4);
    LIB_CharToBcdNByte(buf+len,4);
    len += 4;   

    GetSingle(E_ACTIVE_PWR, buf + len);
    INT32U pwr=22000;
    LIB_MemCpy((INT8U *)&pwr, buf + len, 4);
    LIB_CharToBcdNByte(buf+len, 4);
    len += 4; 
    GetSingle(E_REACTIVE_PWR, buf + len);
    len += 4; 

    GetSingle(E_FACTOR, buf + len);
    len += 4; 

    data = 0;
    GetSingle(E_FREQ, (INT8U *)&data);
    data /= LIB_Pow10(-2 - CURR_DECIMAL_POINT);
    LIB_MemCpy((INT8U *)&data, buf + len, 2);
    len += 2; 

    GetSingle(E_PA_ENGY, buf + len);
	engyMin+=2222*60;
	LIB_MemCpy((INT8U *)&engy, buf + len, 5);
	LIB_CharToBcdNByte(buf+len, 5);
    len += 5; 
	GetSingle(E_NA_ENGY, buf + len);
    len += 5;
    #if (REACTIVE_ENGY_EN == 1)
    /*"�������޹�����  "*/
    GetSingle(E_R_ENGY_I, buf + len);
    len += 5;
    GetSingle(E_R_ENGY_II, buf + len);
    len += 5;
    GetSingle(E_R_ENGY_III, buf + len);
    len += 5;
    GetSingle(E_R_ENGY_IV, buf + len);
    len += 5;
    #endif
    return len;
}

#else

INT16U get_bm_lrm_data(INT8U *buf)
{
    ST_BASE_CAL_ENGY bm_eng;
    INT16U len;

    BUILD_BUG_ON((sizeof(ST_BASE_CAL_ENGY) + sizeof(ST_MMT_DATA) != 120));
    //Lrm_test();
    LIB_MemSet(0, (INT8U *)&bm_eng, sizeof(ST_BASE_CAL_ENGY));
    GetSingle(E_PA_ENGY,(INT8U *)&bm_eng.ComP[0]);
	GetSingle(E_NA_ENGY,(INT8U *)&bm_eng.ComP[1]);
    #if (REACTIVE_ENGY_EN == 1)
    /*"�������޹����� todo "*/
    GetSingle(E_R_ENGY_I,(INT8U *)&bm_eng.Q[0]);
    GetSingle(E_R_ENGY_II,(INT8U *)&bm_eng.Q[1]);
    GetSingle(E_R_ENGY_III,(INT8U *)&bm_eng.Q[2]);
    GetSingle(E_R_ENGY_IV,(INT8U *)&bm_eng.Q[3]);
    #endif

    len = 0;
    LIB_MemCpy((INT8U *)&bm_eng, buf, sizeof(ST_BASE_CAL_ENGY)); /*" 48 "*/
    len = sizeof(ST_BASE_CAL_ENGY);
    LIB_MemCpy((INT8U *)&stMmtData, buf + len, sizeof(ST_MMT_DATA));/*" 36 "*/
    len += sizeof(ST_MMT_DATA);
    //len += get_bm_status(buf + len);/* 4 */
    len += GetSingle(E_UPDATA, buf + len); /* 2 */
    return len; /*" 90�ֽ� "*/
}
#endif


void Host_Commu_Sec_Proc(void)
{
    UN_ID645 id;
    INT8U buf[200];
    INT8U len=0;
    
    id.asLong = ID_BM_MMT_DATA;
    //len = get_show_data(buf);
    //len += GetSingle(E_DOWNDATA, buf + len);
    //len += get_clr_relay_cmd(buf + len);
    //len += get_clr_led_cmd(buf + len);
    //len = get_comm_data(buf, sec_data, ARRAY_SIZE(sec_data));
    GetSingle(E_SYS_TIME, buf);/*"ȡϵͳʱ��"*/  
    LIB_CharToBcdNByte(buf, 7);
	len +=7;
	LIB_RvsSelf(buf,len);
	
    len += get_bm_lrm_data(buf+len);
    tx_pkt_to_peer(0, 0x06, id, buf, len);
}

void Host_Commu_Frz_Data_Push(void)
{
    UN_ID645 id;
    INT8U buf[200];
    INT8U len=0;
    
    id.asLong = ID_BM_FRZ_DATA;
    GetSingle(E_SYS_TIME, buf);/*"ȡϵͳʱ��"*/  
    LIB_CharToBcdNByte(buf , 7);
    len +=7;
    LIB_RvsSelf(buf,len);
	
    len += get_bm_frz_data(buf);
    tx_pkt_to_peer(0, 0x06, id, buf, len);
}

void Host_Commu_Bm_Data_Sync(INT8U type)
{
    UN_ID645 id;
    INT8U buf[200];
    INT8U len;
    
    id.asLong = ID_BM_DATA_SYNC;
    buf[0] = type;
    len = 1;
    switch(type)
    {
        case 1: /*"�ϵ�ͬ����һ�׶�����"*/
            LIB_Reverse(mMtrAddr, buf + len, 6);
            len += 6;
            GetSingle(E_SYS_TIME, buf + len);/*"ȡϵͳʱ��"*/  
            LIB_CharToBcdNByte(buf + len, 7);
            //LIB_RvsSelf(buf + len, 7);
            len += 7;
            break;
        case 2: /*"�ϵ�ͬ���ڶ��׶�����"*/
            GetSingle(E_SYS_TIME, buf + len);/*"ȡϵͳʱ��"*/  
            LIB_CharToBcdNByte(buf + len, 7);
            //LIB_RvsSelf(buf + len, 7);
            len += 7;
            
            len += get_bm_lrm_data(buf + len);
            break;
        case 3: /*" ����ʱͬ��"*/
            GetSingle(E_SYS_TIME, buf + len);/*"ȡϵͳʱ��"*/  
            LIB_CharToBcdNByte(buf + len, 7);
            //LIB_RvsSelf(buf + len, 7);
            len += 7;
            break;
    };

    if(type == 1)
    {
        tx_pkt_to_peer_now(0, 0x06, id, buf, len);
    }
    else
    {
        tx_pkt_to_peer(0, 0x06, id, buf, len);
    }
}

INT8U get_tms = 0xff;

void poweron_set_bm_info(void)
{
    INT8U buf[MASTER_TX_COM_BUF_RX_SZ];
    //INT8U pkt[200];
    INT8U rxlen;
    ST_FRAME645 nResult;
    INT16U tms;
    INT8U i;
    INT8U get;
    //UN_ID645 id;
    
#ifdef HOST_USE_UART
    //ST_7TIME_FORM now;

    ST_COMMU_DEVICE *pDevice;
    pDevice = &mstChipDevice[0];
#endif

    Master_TX_Com_Init(0);
    MCU_DelayMs(10);

    tms = 0;
    get = 0;
    for(tms=0; tms<25; tms++)
    {
        get_tms = tms;
        Host_Commu_Bm_Data_Sync(1);
        rxlen = 0;
        for(i=0; i<50; i++)/*" 5*10ms��ѯӦ�� "*/
        {
            MCU_DelayMs(1);
            
            //Master_TX_Com_TimeOut();
            //if(TRUE == pDevice->InterOctetTimeout) 
            if(pDevice->RxLen >= 16) 
            {
                if(pDevice->RxLen < MASTER_TX_COM_BUF_RX_SZ)
                {
                    LIB_MemCpy(pDevice->RxBuff, buf, pDevice->RxLen);
                    rxlen = Master_TX_Com_ProcessRecBytes(buf, pDevice->RxLen);
                }
            }

            if(rxlen > 0)
            {
                LIB_MemSet(0, (INT8U *)&nResult, sizeof(ST_FRAME645));
                ParseFrame(buf, &nResult);
                switch(nResult.dataId.asLong)
                {
                    case ID_SHOW_PUSH:/*" ��ʾ����Ӧ�� "*/
                        /*" ���ø�ģ��ӿڱ����������� "*/
                        /*" �������������ɵ���ģ���ṩ��������״̬�����������״̬���ģ���ṩ��������"*/
                        break;
                    case ID_AO_CTRL:/*" ����AOӦ�� "*/
                        break;
                    case ID_RTC_SYNC:/*" ��������RTCӦ�� "*/
                        break;
                    case ID_BM_DATA_SYNC:

                        get = 1;
                        break;
                    default:
                        break;
                }
                if(get)
                {
                    break;
                }
            }
        }
        if(get)
        {
            break;
        }
    }
	if(get)
	{
    	return ;
	}
	else
	{
        //SaveRcrd(E_RTC_ERR_EVNT_RCRD, E_EVTLST_RTC_ERR_S);
        //Push(E_CLK_ERR, TRUE);
		return	;
	}
}


