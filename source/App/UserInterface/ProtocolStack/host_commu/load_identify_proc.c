#include "app\UserInterface\UI.h"
#include "App\UserInterface\ProtocolStack\ProtocolStack.h"
#include "App\UserInterface\ProtocolStack\PS_645\Dlt645.h"
#include "App\UserInterface\ProtocolStack\PS_698\DL_698.h"
#include "App\UserInterface\PP\PP_645\PP_ProtocolTree.h"
#include "Lib\LIB_func.h"
#include "system\Sys_def.h"
#include "PlatForm.h"
#include "App\UserInterface\ProtocolStack\host_commu\host_commu.h"
#if 0
#define LD_PWR_ON_STATUS 0X00
#define LD_SYNC_TX_STATUS 0X01
#define LD_SYNC_OK_STATUS 0X02
#define LD_CFG_TX_STATUS 0X03
#define LD_RD_TX_STATUS 0x04
#define LD_CFG_OK_STATUS 0X05
#endif

#define LD_PWR_ON_STATUS 0X00
#define LD_SYNC_STATUS 			10
#define LD_GET_CFGPARA_STATUS 	11
#define LD_SET_CFGPARA_STATUS 	12
#define LD_SET_TIME_STATUS 		13
#define LD_CFG_OK_STATUS 		14

#define LD_FST_PWR_ON_STATUS 0XFF

#define ID_LD_SYNC_PKT 0x04FB0003/*��������*/
#define ID_LD_CFG_PKT 0x04FB0004
#define ID_LD_TIME_PKT 0x0400010C /*��ʶģ���ʱ����*/
#define ID_LD_RD_PKT  0x07000000//test

volatile INT8U has_ld_req;

INT8U ld_identify_status;

INT16U ld_model_tick;
INT32U currDI;

static void reset_LDID(void);


/*" ����645��ʽ��ʱ�� �����YYMMDDWWhhmmss"*/
INT8U get_sys_tm_645_fmt(INT8U *tmp)
{
    INT8U buf[10];
    
    GetSingle(E_SYS_TIME, tmp);
    //LIB_BcdToCharNByte(tmp, 7);
	LIB_CharToBcdNByte(tmp, 7);/*���ʱ����hex����*/
    buf[0] = tmp[6]; //WW
    buf[1] = tmp[3]; //hh
    buf[2] = tmp[4]; //mm
    buf[3] = tmp[5]; //ss
    
    tmp[3] = buf[0];
    tmp[4] = buf[1];
    tmp[5] = buf[2];
    tmp[6] = buf[3];

    LIB_RvsSelf(tmp, 7);

    return 7;
}

INT8U cfg_set_time_pkt_data(INT8U *data)
{
    INT8U tmp[10];
    
    get_sys_tm_645_fmt(tmp);

    LIB_MemSet(0x00, data, 8);
    data[0] = 0x04; /*" 4������ "*/
    LIB_MemCpy(tmp, data + 8, 7);
    return 8 + 7;
}

void tx_time_pkt_to_ld(void)
{
	UN_ID645 id;
	INT8U len,buf[15];//10->8+7
    {
        len = cfg_set_time_pkt_data(buf);
        id.asLong = ID_LD_TIME_PKT;
		currDI=ID_LD_TIME_PKT;
        tx_pkt_to_peer(1, 0x14, id, buf, len); /*" TODO, ģ��Ӧ���� "*/
		ld_model_tick = MCU_GetNonZeroTick();
	}
}


void tx_get_cfg_pkt_from_bm(void)
{
    UN_ID645 id;
        
    id.asLong = ID_GET_LD_CFG_ADDR;
    tx_pkt_to_peer(0, 0x11, id, NULL, 0);
    ld_model_tick = MCU_GetNonZeroTick();
}

void ld_model_rx_get_pkt_proc(INT8U *data)
{
    ld_identify_status = LD_SET_CFGPARA_STATUS;//LD_SYNC_TX_STATUS;
	//ld_identify_status = LD_CFG_TX_STATUS;//TODO  jump to 04FB0004
    /*" TODO ���渺��ģ�����ò��� "*/
    FS_WriteFile(E_FILE_UI_PARA_NO_BACKUP, LD_IDENTIFY_PARA_SHIFT, data, 29);
}
const INT8U defaultPara[64]={0x00,0x20,0x01,0x03,0x08,0x0C,0x54,0x00,
							 0x00,0x78,0x15,0x00,0x00,0x00,0x00,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,
							 0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00
							};
void check_ld_model_cfg_para(void)
{
    INT8U data[29];
    //FS_WriteFile(E_FILE_UI_PARA_NO_BACKUP, LD_IDENTIFY_PARA_SHIFT, (INT8U *)&defaultPara[0], 64);
    //ld_identify_status = LD_SYNC_TX_STATUS;
	//ld_identify_status = LD_CFG_TX_STATUS;
    /*" ��ȡ����ģ�����ò��� "*/
    FS_ReadFile(E_FILE_UI_PARA_NO_BACKUP, LD_IDENTIFY_PARA_SHIFT, data, 29);
}
void tx_sync_pkt_to_ld_model(void)
{
    UN_ID645 id;
        
    id.asLong = ID_LD_SYNC_PKT;
	currDI = ID_LD_SYNC_PKT;
    tx_pkt_to_peer(1, 0x11, id, NULL, 0);
    ld_model_tick = MCU_GetNonZeroTick();
}

void ld_get_para_from_bm_proc(void)
{
    ld_identify_status = LD_GET_CFGPARA_STATUS;//LD_CFG_TX_STATUS;
	
}

INT8U get_ld_cfg_para(INT8U *data)
{
	INT8U tmp[64];
    
    //get_sys_tm_645_fmt(tmp);
	FS_ReadFile(E_FILE_UI_PARA_NO_BACKUP, LD_IDENTIFY_PARA_SHIFT, tmp, 29);/*���ò���29�ֽ�*/
	
    LIB_MemSet(0x00, data, 8);
    data[0] = 0x04; /*" 4������ "*/
    LIB_MemCpy(tmp, data + 8, 29);
    return 8 + 29;
}


void tx_cfg_pkt_to_ld_model(void)
{
    UN_ID645 id;
	INT8U len;
	INT8U data[72];
        
    id.asLong = ID_LD_CFG_PKT;
	currDI=ID_LD_CFG_PKT;
	len=get_ld_cfg_para(data);
    tx_pkt_to_peer(1, 0x14, id, data, len);
    ld_model_tick = MCU_GetNonZeroTick();
}

void tx_read_pkt_from_ld_model(void)
{
    UN_ID645 id;
	INT8U len;
	INT8U data[8];
        
    id.asLong = ID_LD_RD_PKT;
	data[0]=1;
	len=1;
    tx_pkt_to_peer(1, 0x11, id, data, len);
	
    ld_model_tick = MCU_GetNonZeroTick();
}

void ld_model_rx_cfg_pkt_proc(void)
{
	ld_identify_status =LD_SET_TIME_STATUS;
}
void ld_model_rx_rd_pkt_proc(void)
{
	ld_identify_status = LD_CFG_OK_STATUS;//the next status of rd cmd is ok
}

/*" �ϵ����������ѭ������ɶԸ���ģ������� "*/
void ld_identify_run_cfg(void)
{
	
    if(ld_identify_status == LD_CFG_OK_STATUS)
    {
        return;
    }
    if(MCU_CmpTick(ld_model_tick, PEER_REPLY_TIMEOUT) != TRUE)
    {
        return;
    }
    
    switch(ld_identify_status)
    {
    
	case LD_FST_PWR_ON_STATUS:
	case LD_SYNC_STATUS:
		tx_sync_pkt_to_ld_model();/*��������Ƿ���������жϸ���ʶ��ģ���״̬?*/
        break;
    case LD_GET_CFGPARA_STATUS:
		tx_get_cfg_pkt_from_bm();
        break;
    case LD_SET_CFGPARA_STATUS:
        tx_cfg_pkt_to_ld_model();
        break;
	case LD_SET_TIME_STATUS:
		//tx_read_pkt_from_ld_model();
		tx_time_pkt_to_ld();
		break;
		
    case LD_CFG_OK_STATUS:
        break;
    }
}

INT8U identify_pkt_queue_rx_proc(ST_FRAME645 *res, INT8U len, INT8U dev)
{
    INT8U com;
    INT8U pkt_len;
    struct pkt_queue *queue;
    INT8U tmp[10];

    queue = &mst_queue[dev];

    if(res->command == 0x11 || res->command == 0x13) /*"  ����ʶ��ģ����������ĳ������� "*/
    {
        /*" TODO ��ȡ���� ����Ӧ�� "*/
        pkt_len = 0;
        if(res->command == 0x11)
        {
            if(res->dataId.asLong == 0x0400010c)
            {
                get_sys_tm_645_fmt(tmp);

                tx_pkt_to_peer(1, 0x91, res->dataId, tmp, 7);
            }
        }
        if(res->command == 0x13)
        {
            GetSingle(E_COMM_ADDR, tmp);
            tx_pkt_to_peer(1, 0x93, res->dataId, tmp, 6);
        }
        
        return 1;
    }

    switch(res->dataId.asLong)
    {
        default: /*" ֱ��͸��Ӧ����û�ͨ�ſ� "*/
            pkt_len = len;
            if(currDI == ID_LD_SYNC_PKT)
            {
                //FrameDataDec0x33(res);
                if(res->command == 0x91) /*" �ɹ�ͬ�� "*/
                {
                    ld_get_para_from_bm_proc();
                }
            }
			/*��ȡ���Ͷ���ͷ�������ݱ�ʶ�������жϺ�ȷ����һ����*/
			//if(res->dataId.asLong == ID_LD_CFG_PKT)
            if(currDI== ID_LD_CFG_PKT)
            {
                //FrameDataDec0x33(res);
                if(res->command == 0x94) /*" �ɹ����� "*/
                {
                    ld_model_rx_cfg_pkt_proc();
                }
            }
			 if(currDI== ID_LD_TIME_PKT)
            {
                if(res->command == 0x94) /*" �ɹ����� "*/
                {
                   //do nothing 
                   ld_identify_status = LD_CFG_OK_STATUS;//LD_CFG_TX_STATUS;
                   currDI=0;
                }
            }
			if(res->dataId.asLong == ID_LD_RD_PKT)
            {
                //FrameDataDec0x33(res);
                if(res->command == 0x91) 
                {
                    ld_model_rx_rd_pkt_proc();
                }
            }
            com = queue->pkt[queue->head][PKT_SRC_OFF];
            if((com < E_MAX_COM_ID) && (queue->user_reply_tick[com] != 0))
            {
                queue->user_reply_tick[com] = 0;
                Com_Send_data(com, res->beginData, pkt_len);
            }
            break;
    }
    pkt_outqueue(queue);
    queue->send_tick = 0;
    queue->status = CH_IDLE_STATUS;
    return 1;
}

void Load_identify_Req(void)	
{
	if(LD_CFG_OK_STATUS==ld_identify_status)
    {
    	if(has_ld_req)	/*COM-RQ interupt*/
    	{
        	has_ld_req = 0;
			reset_LDID();
			ld_identify_status = LD_SET_CFGPARA_STATUS;//LD_FST_PWR_ON_STATUS;
    	}
    }
}

void Load_identify_main(void)
{
    Load_identify_Req();

    ld_identify_run_cfg();
}

static void LoadComRqISR(void)/*"���ɼ�¼ģ��REQ�����ж�����"*/
{
    if(HIGH == GPIO_ReadPin(LD_COM_RQ))
    {
        has_ld_req = 1;
    }
}

void Nilm_power_on(void)
{
	GPIO_OutputPin(LDID_PWR,LOW);
    GPIO_ConfigPin(LDID_PWR,OUTPUT,OPENDRAIN_OUT);
}

void Load_identify_init(INT8U Mode)
{
    ST_GPIO_EXTICONF ExtiConf;

    ld_identify_status = 0;
    if (MCU_RUNMODE == Mode || FIRST_PWR_ON == Mode)
    {
        has_ld_req = 1;/*��ʼ��ʱ����һ�ζ�ʱ*/
        Nilm_power_on();
        GPIO_ConfigPin(LD_COM_RQ, INPUT, NULL_IN);

        /*"����ʶ��REQ�����ж�"*/
        ExtiConf.PinId = LD_COM_RQ;
        ExtiConf.TrigModule = GPIO_EXTI_EDGE_RISING;
        ExtiConf.IntFuction= LoadComRqISR;
        GPIO_EXTI_Enable(&ExtiConf);
        ld_identify_status = LD_PWR_ON_STATUS;
        if(FIRST_PWR_ON == Mode)
        {
        	/*ʹ��Ĭ�ϵĸ���ʶ������ò���*/
        	FS_WriteFile(E_FILE_UI_PARA_NO_BACKUP, LD_IDENTIFY_PARA_SHIFT, (INT8U *)&defaultPara[0], 29);
            //ld_identify_status = LD_FST_PWR_ON_STATUS;
        }
        else
        {
            check_ld_model_cfg_para();
        }
		ld_identify_status = LD_FST_PWR_ON_STATUS;
    }
}

void reset_LDID(void)
{	
	if(has_ld_req)
		{
	 	GPIO_ConfigPin(LDID_RST,OUTPUT,NORMAL_OUT);
     	GPIO_OutputPin(LDID_RST,LOW); /*"��ͨ��ģ�鸴λ������0"*/
     	MCU_DelayMs(_LDIDResetDelay); /*"��ʱ200ms"*/
     	GPIO_OutputPin(LDID_RST,HIGH);/*"��ͨ��ģ�鸴λ������1"*/
		}
}
