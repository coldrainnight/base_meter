/*"***********************************************************"*/
/*"FileName: RTC_8025T.c                                      "*/
/*"Author:                                                    "*/
/*"Version : 1.0       Date: 2016.02.17                       "*/
/*"Description: ����RTC�豸����                                                                                     "*/
/*"Function List:                                             "*/
/*"<FunctionName>                                             "*/
/*"<author> <time> <version > <desc>                          "*/
/*"***********************************************************"*/
#include "Driver\Device\RTC_8025T.h"
#include "Driver\MCU\MCU.h"
#include "Driver\MCU\IIC.h"
#include "Lib\LIB_func.h"

#define IICREDOTIMES	2      /*"8025TоƬ�����쳣�ظ�����"*/

/*"*********************************************************
ʱ��оƬ�Ĵ�������
*********************************************************"*/
#define R8025DEV    0X64        /*"8025t device address 01100100"*/

#define R8025SECA   0x00        /*"8025t SECOND register address"*/

#define R8025TCONA  0x0d        /*"8025t externsion register address"*/
#define R8025TCON1  0x00        /*"externsion register set "*/
                                /*"TEST   WADA  USEL   TE  FSEL1  FSEL0  TSEL1 TSEL0"*/
#define R8025TCON2  0x00        /*"flag register set "*/
                                /*"  o      o    UF    TF    AF     o     VLF  VDET"*/
#define R8025TCON3  0xe0        /*"control register set "*/
                                /*"CSEL1  CSEL0  UIE   TIE  AIE     o      o   RESET"*/
                                /*" CSEL1  CSEL0  �������  1 1 30��     1 0  10��     0 1  2�루Ĭ�ϣ�   0 0 0.5��"*/
                                /*" UIE  �����ж�ʹ��λ"*/

//static UN_BYTE_DEF    mRtcErr;/*"8025t�쳣״̬��־  b0  8025TоƬʧ���־��λ"*/
const INT8U DefaultTime[]={0x30,0x59,0x23,0x03,0x30,0x04,0x17};
const INT8U contorl[3] = {R8025TCON1, R8025TCON2, R8025TCON3};

volatile INT32U exSecCnt;
static INT8U Write8025T(INT8U adr,INT8U len,INT8U *pbuff);
static INT8U Read8025T(INT8U adr,INT8U len,INT8U *pbuff);
static INT8U WeekChange(INT8U Week,INT8U kind);

/*"*****************************************************************************"*/
/*"  Function:       Write8025T"*/
/*"  Description:    8025Tд���ݽӿ�"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          adr       �Ĵ�����ַ  "*/
/*"                  len       д���ݳ���"*/
/*"                  pBuff     д�����ݻ����ַָ��"*/
/*"  Output:         "*/
/*"  Return:         TRUE      д�ɹ�     FALSE  дʧ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U Write8025T(INT8U adr,INT8U len,INT8U *pbuff)
{
    INT8U result;
    INT8U dev;
    INT8U addrtmp;
    INT8U i,iic_id;

    dev = R8025DEV;
	iic_id=IIC_ID_ERTC;
    addrtmp = adr;
    result = FALSE;
    for(i = 0; ((i < IICREDOTIMES) && (FALSE == result)); i++)
    {
        IIC_Stop(iic_id);
        IIC_Start(iic_id);

        result = IIC_SendChar(iic_id, (INT8U *)&dev, 1);/*"send 8025t device id"*/
        if(TRUE == result)
        {
            result = IIC_SendChar(iic_id, (INT8U *)&addrtmp, 1);/*"send 8025t register address"*/
            if(TRUE == result)
            {
                result = IIC_SendChar(iic_id, pbuff, len);/*"write data"*/
            }
            else
            {
            }
        }
        else
        {
        }
    }

    IIC_Stop(iic_id);

    return result;
}

/*"*****************************************************************************"*/
/*"  Function:       Read8025T"*/
/*"  Description:    8025T�����ݽӿ�"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          adr       �Ĵ�����ַ  "*/
/*"                  len       �����ݳ���"*/
/*"  Output          pBuff     �������ݻ����ַָ��"*/
/*"  Return:         TRUE      д�ɹ�     FALSE  дʧ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U Read8025T(INT8U adr,INT8U len,INT8U *pbuff)
{
    INT8U result;
    INT8U dev;
    INT8U addrtmp;
    INT8U i,iic_id;

    dev = R8025DEV;
	iic_id=IIC_ID_ERTC;
    addrtmp = adr;
    result = FALSE;
    for(i = 0; ((i < IICREDOTIMES) && (FALSE == result)); i++)
    {
        IIC_Stop(iic_id);
        IIC_Start(iic_id);

        result = IIC_SendChar(iic_id, (INT8U *)&dev, 1);/*"send 8025t device id"*/
        if(TRUE == result)
        {
            result = IIC_SendChar(iic_id, (INT8U *)&addrtmp, 1);/*"send 8025t register address"*/
            if(TRUE == result)
            {
                IIC_Start(iic_id);
                dev |= 0x01;
                result = IIC_SendChar(iic_id, (INT8U *)&dev, 1);/*"send 8025t device id"*/
                if(TRUE == result)
                {
                    result = IIC_ReadChar(iic_id, pbuff, len);
                }
                else
                {
                }
            }
            else
            {
            }
        }
        else
        {
        }
    }

    IIC_Stop(iic_id);

    return result;
}

static void EXRTC_INT_ISR(void)
{
    exSecCnt++;
}

void En_1HZ_INT(void)
{
	ST_GPIO_EXTICONF TmpExtiConf;
	
    GPIO_ConfigPin(RTC_1HZ, INPUT, NULL_IN);
    TmpExtiConf.PinId = RTC_1HZ;
    TmpExtiConf.TrigModule = GPIO_EXTI_EDGE_RISING;
    TmpExtiConf.IntFuction= EXRTC_INT_ISR;
    GPIO_EXTI_Enable(&TmpExtiConf);
}

/*"*****************************************************************************"*/
/*"  Function:       EXRTC_Init"*/
/*"  Description:    ����ʱ��оƬ��ʼ��"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          initMode:INIT_MODE0_8025T,��ʼ��ʱ��Ϳ��ƼĴ���  "*/
/*"                           INIT_MODE1_8025T,��ʼ�����ƼĴ���"*/
/*"                  secPulse:SEC_PULSE_ENABLE_8025T  ʹ��8025T���������"*/
/*"                           SEC_PULSE_DISABLE_8025T ʧ��8025T���������"*/
/*"  Output          "*/
/*"  Return:         TRUE      ��ʼ���ɹ�       FALSE    ��ʼ��ʧ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U EXRTC_Init(INT8U initMode)
{
    INT8U uni = 0;
    INT8U result;
    INT8U count;
    INT8U contrlTmp[3];
    INT8U DateWeekTime[7];

	exSecCnt=0;/**/
	En_1HZ_INT();/*PG6 as input */
    result = TRUE;
    //mRtcErr.byte = 0;
    result = Read8025T(R8025TCONA + 1, sizeof(count),(INT8U *)&count); /*"�ж�оƬʧ���־"*/
    if(TRUE == result)
    {      
        count &= 0x02;
        if(count)
        {
            //mRtcErr.bit.b0 = 1;
        }
        else
        {
        }

        for(uni = 0; uni < sizeof(contrlTmp); uni++)
        {
            contrlTmp[uni] = contorl[uni];
        }
        result = Write8025T(R8025TCONA, sizeof(contrlTmp),contrlTmp);

        if(TRUE == result)
        {
            if(INIT_MODE0_8025T == initMode)
            {
                for(uni = 0; uni < sizeof(ST_7TIME_FORM); uni++)
                {
                    DateWeekTime[uni] = DefaultTime[uni];
                }
                result = Write8025T(R8025SECA, sizeof(DateWeekTime), DateWeekTime);
            }
            else
            {
            }
        }
        else
        {
        }
    }
	

    return (result);
}

/*"�궨��ʱ���ʽ���ݴ洢˳�򣬸���Ӧ�ò������˳��������ʱ���ʽ˳��"*/
#define SEC   5
#define MIN   4
#define HOUR  3
#define DAY   2
#define MON   1
#define YEAR  0
#define WEEK  6
/*"*****************************************************************************"*/
/*"  Function:       EXRTC_Read"*/
/*"  Description:    ������ʱ��оƬʱ��"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          "*/
/*"  Output          t:ʱ�ӽ������ݻ���ָ��  ���˳����ֽڵ����ֽ�Ϊ�� �� �� �ܴ� ʱ �� ��"*/
/*"  Return:         TRUE      ��ʱ�ӳɹ�       FALSE    ��ʱ��ʧ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U EXRTC_ReadTime(INT8U *t)
{
    //INT8U uni = 0;
    INT8U result;
    INT8U DateWeekTime[7];

    result = Read8025T(R8025SECA, sizeof(DateWeekTime), DateWeekTime);
    if(TRUE == result)
    {
        *(t + YEAR) = DateWeekTime[6];
        *(t + MON) = DateWeekTime[5] & 0x1f;
        *(t + DAY) = DateWeekTime[4] & 0x3f;
        *(t + HOUR) = DateWeekTime[2] & 0x3f;
        *(t + MIN) = DateWeekTime[1] & 0x7f;
        *(t + SEC) = DateWeekTime[0] & 0x7f;
        *(t + WEEK) = WeekChange(DateWeekTime[3], 1);
    }
    else
    {
    }

    return (result);
}

/*"*****************************************************************************"*/
/*"  Function:       EXRTC_Write"*/
/*"  Description:    д����ʱ��оƬʱ��"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          "*/
/*"  Output          t:ʱ��д���ݻ���ָ��  ���˳����ֽڵ����ֽ�Ϊ�� �� �� �ܴ� ʱ �� ��"*/
/*"  Return:         TRUE      дʱ�ӳɹ�       FALSE    дʱ��ʧ��"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
INT8U EXRTC_WriteTime(INT8U  *t)
{
    INT8U result;
    INT8U weekTmp;
    INT8U DateWeekTime[7];

    weekTmp = LIB_CalWeekBCD((ST_6TIME_FORM *)t);
    DateWeekTime[6] = *(t + YEAR);
    DateWeekTime[5] = *(t + MON);
    DateWeekTime[4] = *(t + DAY);
    DateWeekTime[2] = *(t + HOUR);
    DateWeekTime[1] = *(t + MIN);
    DateWeekTime[0] = *(t + SEC);
    DateWeekTime[3] = WeekChange(weekTmp, 0);
    result = Write8025T(R8025SECA, sizeof(DateWeekTime),DateWeekTime);

    return (result);
}

void close_exrtc_sec(void)
{
	INT8U count,result;
	INT8U ctrl_C0[3]={0x00,0x00,0xc0};	
	
	result = Read8025T(R8025TCONA + 1, sizeof(count),(INT8U *)&count); /*"�ж�оƬʧ���־"*/
    if(TRUE == result)
    {      
        count &= 0x02;
        if(count)
        {
            //mRtcErr.bit.b0 = 1;todo
        }
        else
        {
        }
		
       Write8025T(R8025TCONA, sizeof(ctrl_C0),ctrl_C0);
    }
}

/*"*****************************************************************************"*/
/*"  Function:       WeekChange"*/
/*"  Description:    �ܴθ�ʽת������"*/
/*"  Calls:          "*/
/*"  Called By:      "*/
/*"  Input:          Week �����ܴ�"*/
/*"                  kind ת������  0 softdate --> rtcdata   1 rtcdata  --> softdata"*/
/*"  Output          "*/
/*"  Return:         ����ת�����ܴθ�ʽ"*/
/*"  Others:         "*/
/*"*****************************************************************************"*/
static INT8U WeekChange(INT8U Week,INT8U kind)
{
    INT8U ret_week = 0;
    INT8U uni = 0;
    if(kind == 0) /*"write in to 8025T"*/
    {
        ret_week = 1;
        ret_week <<= Week;
    }
    else /*"read from 8025T"*/
    {
        for(uni = 0; uni < 7; uni++)
        {
            if((Week & 0x01) == 1)
            {
                ret_week = uni;
                break;
            }
            Week >>= 0x01;
        }
    }

    return (ret_week);
}
