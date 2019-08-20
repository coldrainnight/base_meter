/*"***********************************************************"*/		
/*"FileName: WDT.c "*/		
/*"Author: "*/     
/*"Version : 1.0       Date: 2017.01.5"*/	
/*"Description: ?��??1��"*/
/*"Function List: "*/		
/*"<FunctionName>	"*/	
/*"<author> <time> <version > <desc>"*/			
/*"***********************************************************"*/	
#include "Driver\MCU\MCU.h"

static void GPIO_Config_Dir(INT8U PinId, INT8U Dir);
static void GPIO_Config_PullFunc(INT8U PinId, INT8U Mode);
static void GPIO_Config_OD(INT8U PinId, INT8U Mode);
static void Set2bitValue(INT8U Value,INT32U *SetDataP,INT8U BitPos);
static void GPIO_EXTI_ISR(void);

void GPIO_ConfigPin(INT8U PinId, INT8U Dir, INT8U Mode);
void GPIO_OutputPin(INT8U PinId, INT8U OutPut);
INT8U GPIO_ReadPin(INT8U PinId);
void GPIO_SetMulti(INT8U PinId, GPIOFun_TypeDef Mode);
void GPIO_EXTI_Enable(ST_GPIO_EXTICONF *EXTI_GpioConf);
void GPIO_EXTI_Disable(INT8U PinId);
void GPIO_Init(INT8U mode);


/*"********************************************************************"*/
/*"IO��ID�ţ���0x00��ʾPA0��"*/
/*"********************************************************************"*/

const ST_GPIO_CONFIG mstGPIOConfig[] =
{
    /*"GPIO PA�� 16"*/
    {0x00, INPUT, HIGH_IMPED_IN, LOW},
    {0x01, INPUT, HIGH_IMPED_IN, LOW},
    {0x02, INPUT, HIGH_IMPED_IN, LOW},
    {0x03, INPUT, HIGH_IMPED_IN, LOW},
    {0x04, INPUT, HIGH_IMPED_IN, LOW},
    {0x05, INPUT, HIGH_IMPED_IN, LOW},
    {0x06, INPUT, HIGH_IMPED_IN, LOW},
    {0x07, INPUT, HIGH_IMPED_IN, LOW},
    {0x08, INPUT, HIGH_IMPED_IN, LOW},
    {0x09, INPUT, HIGH_IMPED_IN, LOW},
    {0x0A, INPUT, HIGH_IMPED_IN, LOW},
    {0x0B, INPUT, HIGH_IMPED_IN, LOW},
    {0x0C, INPUT, HIGH_IMPED_IN, LOW},
    {0x0D, INPUT, HIGH_IMPED_IN, LOW},
    {0x0E, INPUT, HIGH_IMPED_IN, LOW},
    {0x0F, INPUT, HIGH_IMPED_IN, LOW},

    /*"GPIO PB�� 16"*/
    {0x10, INPUT, HIGH_IMPED_IN, LOW},
    {0x11, INPUT, HIGH_IMPED_IN, LOW},
    {0x12, INPUT, HIGH_IMPED_IN, LOW},
    {0x13, INPUT, HIGH_IMPED_IN, LOW},
    {0x14, INPUT, HIGH_IMPED_IN, LOW},
    {0x15, INPUT, HIGH_IMPED_IN, LOW},
    {0x16, INPUT, HIGH_IMPED_IN, LOW},
    {0x17, INPUT, HIGH_IMPED_IN, LOW},
    {0x18, INPUT, HIGH_IMPED_IN, LOW},
    {0x19, INPUT, HIGH_IMPED_IN, LOW},
    {0x1A, INPUT, HIGH_IMPED_IN, LOW},
    {0x1B, INPUT, HIGH_IMPED_IN, LOW},
    {0x1C, INPUT, HIGH_IMPED_IN, LOW},
    {0x1D, INPUT, HIGH_IMPED_IN, LOW},
    {0x1E, INPUT, HIGH_IMPED_IN, LOW},
    {0x1F, INPUT, HIGH_IMPED_IN, LOW},

    /*"GPIO PC��"*/
    {0x22, INPUT, HIGH_IMPED_IN, LOW},
    {0x23, INPUT, HIGH_IMPED_IN, LOW},
    {0x24, INPUT, HIGH_IMPED_IN, LOW},
    {0x25, INPUT, HIGH_IMPED_IN, LOW},
    {0x26, INPUT, HIGH_IMPED_IN, LOW},
    {0x27, INPUT, HIGH_IMPED_IN, LOW},
    {0x28, INPUT, HIGH_IMPED_IN, LOW},
    {0x29, INPUT, HIGH_IMPED_IN, LOW},
    {0x2A, INPUT, HIGH_IMPED_IN, LOW},
    {0x2B, INPUT, HIGH_IMPED_IN, LOW},
    {0x2C, INPUT, HIGH_IMPED_IN, LOW},
    {0x2D, INPUT, HIGH_IMPED_IN, LOW},
    {0x2E, INPUT, HIGH_IMPED_IN, LOW},
    {0x2F, INPUT, HIGH_IMPED_IN, LOW},

    /*"GPIO PD��"*/
    {0x30, INPUT, HIGH_IMPED_IN, LOW},
    {0x31, INPUT, HIGH_IMPED_IN, LOW},
    {0x32, INPUT, HIGH_IMPED_IN, LOW},
    {0x33, INPUT, HIGH_IMPED_IN, LOW},
    {0x34, INPUT, HIGH_IMPED_IN, LOW},
    {0x35, INPUT, HIGH_IMPED_IN, LOW},
    {0x36, INPUT, HIGH_IMPED_IN, LOW},
    {0x37, INPUT, HIGH_IMPED_IN, LOW},

    /*"GPIO PE��"*/
    {0x42, INPUT, HIGH_IMPED_IN, LOW},
    {0x43, INPUT, HIGH_IMPED_IN, LOW},
    {0x44, INPUT, HIGH_IMPED_IN, LOW},

    /*"GPIO PF��"*/
    {0x53, INPUT, HIGH_IMPED_IN, LOW},
    {0x54, INPUT, HIGH_IMPED_IN, LOW},
    {0x55, INPUT, HIGH_IMPED_IN, LOW},
    {0x56, INPUT, HIGH_IMPED_IN, LOW},
    {0x5B, INPUT, HIGH_IMPED_IN, LOW},
    {0x5C, INPUT, HIGH_IMPED_IN, LOW},
    {0x5D, INPUT, HIGH_IMPED_IN, LOW},
    {0x5E, INPUT, HIGH_IMPED_IN, LOW},
    {0x5F, INPUT, HIGH_IMPED_IN, LOW},

    /*"GPIO PG��"*/
    {0x62, INPUT, HIGH_IMPED_IN, LOW},
    {0x63, INPUT, HIGH_IMPED_IN, LOW},
    {0x67, INPUT, HIGH_IMPED_IN, LOW},
    //{0x68, INPUT, HIGH_IMPED_IN, LOW},/*SWD*/
    //{0x69, INPUT, HIGH_IMPED_IN, LOW},/*SWD*/
};

const ST_GPIO_CONFIG mstGPIOConfigPD[] =
{
    /*"GPIO PA��"*/
    {0x00, INPUT, HIGH_IMPED_IN, LOW},
    {0x01, INPUT, HIGH_IMPED_IN, LOW},
    {0x02, INPUT, HIGH_IMPED_IN, LOW},
    {0x03, INPUT, HIGH_IMPED_IN, LOW},
    {0x04, INPUT, HIGH_IMPED_IN, LOW},
    {0x05, INPUT, HIGH_IMPED_IN, LOW},
    {0x06, INPUT, HIGH_IMPED_IN, LOW},
    {0x07, INPUT, HIGH_IMPED_IN, LOW},
    {0x08, INPUT, HIGH_IMPED_IN, LOW},
    {0x09, INPUT, HIGH_IMPED_IN, LOW},
    {0x0A, INPUT, HIGH_IMPED_IN, LOW},
    {0x0B, INPUT, HIGH_IMPED_IN, LOW},
    {0x0C, INPUT, HIGH_IMPED_IN, LOW},
    {0x0D, INPUT, HIGH_IMPED_IN, LOW},
    {0x0E, INPUT, HIGH_IMPED_IN, LOW},
    {0x0F, INPUT, HIGH_IMPED_IN, LOW},

    /*"GPIO PB��"*/
    {0x10, INPUT, HIGH_IMPED_IN, LOW},
    {0x11, INPUT, HIGH_IMPED_IN, LOW},
    {0x12, INPUT, HIGH_IMPED_IN, LOW},
    {0x13, INPUT, HIGH_IMPED_IN, LOW},
    {0x14, INPUT, HIGH_IMPED_IN, LOW},
    {0x15, INPUT, HIGH_IMPED_IN, LOW},
    {0x16, INPUT, HIGH_IMPED_IN, LOW},
    {0x17, INPUT, HIGH_IMPED_IN, LOW},
    {0x18, INPUT, HIGH_IMPED_IN, LOW},
    {0x19, INPUT, HIGH_IMPED_IN, LOW},
    {0x1A, INPUT, HIGH_IMPED_IN, LOW},
    {0x1B, INPUT, HIGH_IMPED_IN, LOW},
    {0x1C, INPUT, HIGH_IMPED_IN, LOW},
    {0x1D, INPUT, HIGH_IMPED_IN, LOW},
    {0x1E, INPUT, HIGH_IMPED_IN, LOW},
    {0x1F, INPUT, HIGH_IMPED_IN, LOW},

    /*"GPIO PC��"*/
    {0x22, INPUT, HIGH_IMPED_IN, LOW},
    {0x23, INPUT, HIGH_IMPED_IN, LOW},
    {0x24, INPUT, HIGH_IMPED_IN, LOW},
    {0x25, INPUT, HIGH_IMPED_IN, LOW},
    {0x26, INPUT, HIGH_IMPED_IN, LOW},
    {0x27, INPUT, HIGH_IMPED_IN, LOW},
    {0x28, INPUT, HIGH_IMPED_IN, LOW},
    {0x29, INPUT, HIGH_IMPED_IN, LOW},
    {0x2A, INPUT, HIGH_IMPED_IN, LOW},
    {0x2B, INPUT, HIGH_IMPED_IN, LOW},
    {0x2C, INPUT, HIGH_IMPED_IN, LOW},
    {0x2D, INPUT, HIGH_IMPED_IN, LOW},
    {0x2E, INPUT, HIGH_IMPED_IN, LOW},
    {0x2F, INPUT, HIGH_IMPED_IN, LOW},

    /*"GPIO PD��"*/
    {0x30, INPUT, HIGH_IMPED_IN, LOW},
    {0x31, INPUT, HIGH_IMPED_IN, LOW},
    {0x32, INPUT, HIGH_IMPED_IN, LOW},
    {0x33, INPUT, HIGH_IMPED_IN, LOW},
    {0x34, INPUT, HIGH_IMPED_IN, LOW},
    {0x35, INPUT, HIGH_IMPED_IN, LOW},
    {0x36, INPUT, HIGH_IMPED_IN, LOW},
    {0x37, INPUT, HIGH_IMPED_IN, LOW},

    /*"GPIO PE��"*/
    {0x42, INPUT, HIGH_IMPED_IN, LOW},
    {0x43, INPUT, HIGH_IMPED_IN, LOW},
    {0x44, INPUT, HIGH_IMPED_IN, LOW},

    /*"GPIO PF��"*/
    {0x53, INPUT, HIGH_IMPED_IN, LOW},
    {0x54, INPUT, HIGH_IMPED_IN, LOW},
    {0x55, INPUT, HIGH_IMPED_IN, LOW},
    {0x56, INPUT, HIGH_IMPED_IN, LOW},
    {0x5B, INPUT, HIGH_IMPED_IN, LOW},
    {0x5C, INPUT, HIGH_IMPED_IN, LOW},
    {0x5D, INPUT, HIGH_IMPED_IN, LOW},
    {0x5E, INPUT, HIGH_IMPED_IN, LOW},
    {0x5F, INPUT, HIGH_IMPED_IN, LOW},

    /*"GPIO PG��"*/
    {0x62, INPUT, HIGH_IMPED_IN, LOW},
    {0x63, INPUT, HIGH_IMPED_IN, LOW},
    {0x67, INPUT, HIGH_IMPED_IN, LOW},
    //{0x68, INPUT, HIGH_IMPED_IN, LOW},/*SWD*/
    //{0x69, INPUT, HIGH_IMPED_IN, LOW},/*SWD*/
};

#define GPIO_NUM    ARRAY_SIZE(mstGPIOConfig)

/*"************************************************			
Function: 	GPIO_Config_Dir		
Description: GPIO�ķ�������
Input:  GPIO_PinId 
        DIR  ����
Output: ��	
Return: 
Others: 		
************************************************"*/	
static void GPIO_Config_Dir(INT8U GPIO_PinId, INT8U Dir)
{
    INT32U *PinAddr;
    INT8U tmp;
    INT8U Cnt ;

    Cnt = (INT8U)(GPIO_PinId & 0x0F);
    tmp = (INT8U)(GPIO_PinId & 0xF0); /*"��ȡIO������"*/

    if(0x00 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOA->INEN);
    }
    else if(0x10 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOB->INEN);
    }
    else if(0x20 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOC->INEN);
    }
    else if(0x30 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOD->INEN);
    }
    else if(0x40 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOE->INEN);
    }
    else if(0x50 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOF->INEN);
    }
    else if(0x60 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOG->INEN);
    }
    else/*"IO�ڷǷ�����"*/
    {
        return;
    }

    if(INPUT == Dir)
    {
        *PinAddr |= 0x01 << Cnt;/*����ʹ��*/
    }
    else
    {
        *PinAddr &= ~(0x01 << Cnt);/*��ʹ������*/
    }
}



/*"************************************************			
Function: 	GPIO_Config_PullFunc		
Description: GPIO����������
Input:  GPIO_PinId 
        Mode GPIO ģʽ
Output: ��	
Return: 
Others: 		
************************************************"*/	
void GPIO_Config_PullFunc(INT8U GPIO_PinId, INT8U Mode)
{

    INT32U *PinAddr;
    INT8U tmp;
    INT8U Cnt ;

    Cnt = (INT8U)(GPIO_PinId & 0x0F);
    tmp = (INT8U)((GPIO_PinId) & 0xF0); /*"��ȡIO������"*/

    if(0x00 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOA->PUEN);
    }
    else if(0x10 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOB->PUEN);
    }
    else if(0x20 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOC->PUEN);
    }
    else if(0x30 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOD->PUEN);
    }
    else if(0x40 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOE->PUEN);
    }
    else if(0x50 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOF->PUEN);
    }
    else if(0x60 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOG->PUEN);
    }
    else/*"IO�ڷǷ�����"*/
    {
        return;
    }
#pragma diag_suppress=Pa134
    if((PULLUP_OUT & Mode) || (PULL_UP_IN & Mode))
#pragma diag_default=Pa134
    {
        *PinAddr |= (0x01 << Cnt);
    }
    else
    {
        *PinAddr &= ~(0x01 << Cnt);
    }

}


/*"************************************************			
Function: 	GPIO_Config_OD		
Description: GPIO ��©����
Input:  GPIO_PinId 
        Mode GPIO ģʽ
Output: ��	
Return: 
Others: 		
************************************************"*/	
static void GPIO_Config_OD(INT8U GPIO_PinId, INT8U Mode)
{
    INT32U *PinAddr;
    INT8U tmp;
    INT8U Cnt ;

    Cnt = (INT8U)(GPIO_PinId & 0x0F);
    tmp = (INT8U)((GPIO_PinId) & 0xF0); /*"��ȡIO������"*/

    if(0x00 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOA->ODEN);
    }
    else if(0x10 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOB->ODEN);
    }
    else if(0x20 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOC->ODEN);
    }
    else if(0x30 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOD->ODEN);
    }
    else if(0x40 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOE->ODEN);
    }
    else if(0x50 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOF->ODEN);
    }
    else if(0x60 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOG->ODEN);
    }
    else/*"IO�ڷǷ�����"*/
    {
        return;
    }

    if(OPENDRAIN_OUT & Mode)
    {
        *PinAddr |= (0x01 << Cnt);
    }
    else
    {
        *PinAddr &= ~(0x01 << Cnt);
    }
}


/*"************************************************			
Function: 	GPIO_ConfigPin		
Description: GPIO �������������ģʽ,
             !!! �Ὣ����ģʽҲ���ó���������ģʽ!!!
Input:  GPIO_PinId 
        Dir ����
        Mode GPIO ģʽ
Output: ��	
Return: 
Others: 		
************************************************"*/	
void GPIO_ConfigPin(INT8U GPIO_PinId, INT8U Dir, INT8U Mode)
{
    INT8U  Tmp;
    
    if(Dir == INPUT)
    {
        if(Mode != HIGH_IMPED_IN)
        {
            Tmp = INPUT;
        }
        else/*����ģʽ��ʹ������*/
        {
            Tmp = OUTPUT;
        }
        GPIO_Config_Dir(GPIO_PinId, Tmp);
        GPIO_SetMulti(GPIO_PinId, GPIO_IN);/*����Ϊ���빦��*/
        GPIO_Config_PullFunc(GPIO_PinId, Mode);
    }
    else
    {
        GPIO_Config_Dir(GPIO_PinId, OUTPUT);
        GPIO_SetMulti(GPIO_PinId, GPIO_Mode_OUT);/*����Ϊ���빦��*/
        GPIO_Config_PullFunc(GPIO_PinId, Mode);
        GPIO_Config_OD(GPIO_PinId, Mode);
    }
}


/*"************************************************			
Function: 	GPIO_OutputPin		
Description: ���������ƽ��ʹ�����������λ�͸�λ�Ĵ���
Input:  GPIO_PinId 
        OutPut 
Output: ��	
Return: 
Others: 		
************************************************"*/	
void GPIO_OutputPin(INT8U GPIO_PinId, INT8U OutPut)
{
    INT32U *PinAddrSetH;
    INT32U *PinAddrSetL;

    INT8U tmp;
    INT8U Cnt ;

    Cnt = (INT8U)(GPIO_PinId & 0x0F);
    tmp = (INT8U)((GPIO_PinId) & 0xF0); /*"��ȡIO������"*/

    if(0x00 == tmp)
    {
        PinAddrSetH = (INT32U *)(&GPIOA->BSETH);
        PinAddrSetL = (INT32U *)(&GPIOA->BRESETL);
    }
    else if(0x10 == tmp)
    {
        PinAddrSetH = (INT32U *)(&GPIOB->BSETH);
        PinAddrSetL = (INT32U *)(&GPIOB->BRESETL);
    }
    else if(0x20 == tmp)
    {
        PinAddrSetH = (INT32U *)(&GPIOC->BSETH);
        PinAddrSetL = (INT32U *)(&GPIOC->BRESETL);
    }
    else if(0x30 == tmp)
    {
        PinAddrSetH = (INT32U *)(&GPIOD->BSETH);
        PinAddrSetL = (INT32U *)(&GPIOD->BRESETL);
    }
    else if(0x40 == tmp)
    {
        PinAddrSetH = (INT32U *)(&GPIOE->BSETH);
        PinAddrSetL = (INT32U *)(&GPIOE->BRESETL);
    }
    else if(0x50 == tmp)
    {
        PinAddrSetH = (INT32U *)(&GPIOF->BSETH);
        PinAddrSetL = (INT32U *)(&GPIOF->BRESETL);
    }
    else if(0x60 == tmp)
    {
        PinAddrSetH = (INT32U *)(&GPIOG->BSETH);
        PinAddrSetL = (INT32U *)(&GPIOG->BRESETL);
    }
    else/*"IO�ڷǷ�����"*/
    {
        return ;
    }


    if(OutPut == HIGH)
    {
        *PinAddrSetH  = (INT32U)(0x01UL << Cnt);
    }
    else
    {
        *PinAddrSetL  = (INT32U)(0x01UL << Cnt);
    }
}

/*"************************************************			
Function: 	GPIO_ReadPin		
Description: ��ȡ���ŵ�ƽ
Input:  GPIO_PinId 
Output: ��ƽ	
Return: 
Others: 		
************************************************"*/	
INT8U GPIO_ReadPin(INT8U GPIO_PinId)
{
    INT32U PinVal;
    INT8U tmp;
    INT8U Cnt ;
    INT32U RetVal ;

    Cnt = (INT8U)(GPIO_PinId & 0x0F);
    tmp = (INT8U)((GPIO_PinId) & 0xF0); /*"��ȡIO������"*/

    if(0x00 == tmp)
    {
        PinVal = (GPIOA->IDR);
    }
    else if(0x10 == tmp)
    {
        PinVal = (GPIOB->IDR);
    }
    else if(0x20 == tmp)
    {
        PinVal = (GPIOC->IDR);
    }
    else if(0x30 == tmp)
    {
        PinVal = (GPIOD->IDR);
    }
    else if(0x40 == tmp)
    {
        PinVal = (GPIOE->IDR);
    }
    else if(0x50 == tmp)
    {
        PinVal = (GPIOF->IDR);
    }
    else if(0x60 == tmp)
    {
        PinVal = (GPIOG->IDR);
    }
    else/*"IO�ڷǷ�����"*/
    {
        return 0xFF;
    }

    RetVal = PinVal & (0x01UL << Cnt);
    if(RetVal)
    {
        return HIGH;
    }
    else
    {
        return  LOW;
    }

}

static void Set2bitValue(INT8U Value,INT32U *SetDataP,INT8U BitPos)
{
    (*SetDataP) &=  ~(0x03 << (BitPos));/*����*/
    (*SetDataP) |=   (INT32U)(Value << BitPos);/*����ΪVAL*/
    
}

/*"************************************************			
Function: 	GPIO_SetMulti		
Description: ���ö๦��ģʽ
Input:  GPIO_PinId 
        GPIOFun_TypeDef
Output: 	
Return: 
Others: 		
************************************************"*/	
void GPIO_SetMulti(INT8U GPIO_PinId, GPIOFun_TypeDef Mode)
{
    INT32U *PinAddr;
    INT8U tmp;
    INT8U Cnt ;

    Cnt = (INT8U)(GPIO_PinId & 0x0F);
    tmp = (INT8U)((GPIO_PinId) & 0xF0); /*"��ȡIO������"*/

    if(0x00 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOA->FCR);
    }
    else if(0x10 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOB->FCR);
    }
    else if(0x20 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOC->FCR);
    }
    else if(0x30 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOD->FCR);
    }
    else if(0x40 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOE->FCR);
    }
    else if(0x50 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOF->FCR);
    }
    else if(0x60 == tmp)
    {
        PinAddr = (INT32U *)(&GPIOG->FCR);
    }
    else/*"IO�ڷǷ�����"*/
    {
        return;
    }

    Set2bitValue(Mode,PinAddr,(Cnt*2));
}


#define EXTI0 (0)
#define EXTI1 (1)
#define EXTI2 (2)
/*"************************************************			
Function: 	PinToExtiInfo		
Description: ͨ��pin���Ų�ѯ��Ӧ��EXTI�Ĵ���λ�ü���Ҫ���õ�ֵ
Input:  GPIO_PinId 
        ExtiX �Ĵ����� EXTI0 EXTI1 EXTI2
        SelY  ����ΪSELX
        SelVal ��Ҫ���õ�ֵ
Output: 	
Return: 
Others: 		
************************************************"*/	
static void PinToExtiInfo(INT8U GPIO_PinId ,INT8U *ExtiX,INT8U *SelY,INT8U *SelVal)
{
    INT8U group;
    INT8U Cnt ;

    group = (INT8U)(GPIO_PinId & 0xF0);
    Cnt = (INT8U)(GPIO_PinId & 0x0F);
    *ExtiX = 0xFF;
    *SelY  = 0xFF;  
    *SelVal = 0xFF;
    /*GroupA|B�ڵ�һ��*/
    if((0x00 == group)||(0x10 == group))
    {
        *ExtiX = EXTI0;
        if(0x00 == group)
        {
            if(Cnt<8)
            {
                *SelY = Cnt;
                *SelVal = 0x00;
            }
            else
            {
                *SelY = Cnt-8;
                *SelVal = 0x01;
            }
        }
        else
        {
            if(Cnt<8)
            {
                *SelY = Cnt;
                *SelVal = 0x02;

            }
            else
            {
                *SelY = Cnt-8;
                *SelVal = 0x03;
            }
        }
    }
    /*GroupC|D|G�ڵڶ���*/
    else if((0x20 == group)||(0x30 == group)||(0x60 == group))
    {
        *ExtiX = EXTI1;
        if(0x20 == group)/*C group*/
        {
            if(Cnt<8)
            {
                *SelY = Cnt;
                *SelVal = 0x00;
            }
            else
            {
                *SelY = Cnt-8;
                *SelVal = 0x01;
            }
        }
        else if(0x30 == group)/*D group*/
        {
            if(Cnt<8)
            {
                *SelY = Cnt;
                *SelVal = 0x02;

            }
            else if(Cnt<11)/*PD8,PD9,PD10*/
            {
                *SelY = Cnt-8;
                *SelVal = 0x03;
            }
        }
        else if(0x60 == group)/*G group*/
        {
            if(Cnt==2)
            {
                *SelY = 3;
                *SelVal = 0x03;
            }
            else if((Cnt>4) && (Cnt<9))/*PG5,PG6,PG7,PG8*/
            {
                *SelY = Cnt-1;
                *SelVal = 0x03;
            }
        }
    }
    /*GroupE|F�ڵ�3��*/
    else if((0x40 == group) || (0x50 == group))
    {
        *ExtiX = EXTI2;
        if(0x40 == group)/*GROUPE*/
        {
            if((Cnt>1) && (Cnt<10))/*PE2~PE9*/
            {
                *SelY = (Cnt-2);
                *SelVal = 0x00;
            }

        }
        else if(0x50 == group)/*GROUPF*/
        {
            if(Cnt<7)/*PF1~PF6*/
            {
                *SelY = Cnt;
                *SelVal = 0x01;

            }
            else if ((Cnt>7) && (Cnt<11))/*PF8,PF9,PF10*/
            {
                *SelY = Cnt-4;
                *SelVal = 0x02;
            }
        }
    }
}


static  ISR_FNCT_VOID  ExtiIsrTab[3][8];

/*"************************************************			
Function: 	GPIO_EXTI_ISR		
Description: gpio�жϴ�����
Input: 
Output: 	
Return: 
Others: 		
************************************************"*/	
static void GPIO_EXTI_ISR(void)
{
    INT32U TmpIf[3];
    INT32U TmpData;
    INT8U Cnt;
    INT8U BitPos;

    TmpIf[0] = GPIO_common->EXTI0IF;
    TmpIf[1] = GPIO_common->EXTI1IF;
    TmpIf[2] = GPIO_common->EXTI2IF;

    GPIO_common->EXTI0IF = 0xFFFFFFFF;
    GPIO_common->EXTI1IF = 0xFFFFFFFF;
    GPIO_common->EXTI2IF = 0xFFFFFFFF;

    for(Cnt=0;Cnt<3;Cnt++)
    {
        TmpData = TmpIf[Cnt];

        if(TmpData != 0)
        {
            for(BitPos=0;BitPos<8;BitPos++)
            {
                if(TmpData & (0x01<<BitPos))
                {
                    if(ExtiIsrTab[Cnt][BitPos] != NULL)
                    {
                        ExtiIsrTab[Cnt][BitPos]();
                    }
                }
            }
        }
    }
   
}


/*"************************************************			
Function: 	GPIO_EXTI_Enable		
Description: GPIO ʹ���ⲿ�ж�
Input:  EXTI_GpioConf
Output: ��	
Return: 
Others: 		
************************************************"*/	
void GPIO_EXTI_Enable(ST_GPIO_EXTICONF *EXTI_GpioConf)
{
    INT32U *SelAddr;
    INT8U ExtiX;
    INT8U SelY;
    INT8U SelVal;
    
    PinToExtiInfo(EXTI_GpioConf->PinId,&ExtiX,&SelY,&SelVal);
    
    ExtiIsrTab[ExtiX][SelY]= EXTI_GpioConf->IntFuction;
    
    /*�������˲��������Ҫ*/
    /*δʹ��*/

    /*����GPIOΪ����*/
    GPIO_ConfigPin(EXTI_GpioConf->PinId, INPUT, NULL_IN);

    /*����EXTI*/
    if(ExtiX == EXTI0)
    {
        MCU_PeriphClkSet(EXTI0_num,ENABLE);
        SelAddr = (INT32U *)(&GPIO_common->EXTI0_SEL);
    }
    else if(ExtiX == EXTI1)
    {
        MCU_PeriphClkSet(EXTI1_num,ENABLE);
        SelAddr = (INT32U *)(&GPIO_common->EXTI1_SEL);
    }
    else if(ExtiX == EXTI2)
    {
        MCU_PeriphClkSet(EXTI2_num,ENABLE);
        SelAddr = (INT32U *)(&GPIO_common->EXTI2_SEL);
    }
    else
    {
        return;
    }
    

    NOP;
    NOP;
    NOP;
    NOP;
    Set2bitValue(SelVal,SelAddr,SelY*2);/*����INT����*/  
    Set2bitValue( EXTI_GpioConf->TrigModule,SelAddr,SelY*2+16);/*����EDGE Mode*/

}

/*"************************************************			
Function: 	GPIO_EXTI_Disable		
Description: GPIO �ر��ⲿ�ж�
Input:  PinId
Output: ��	
Return: 
Others: 		
************************************************"*/	
void GPIO_EXTI_Disable(INT8U PinId)
{
    INT32U *SelAddr;
    INT8U ExtiX;
    INT8U SelY;
    INT8U SelVal;

    PinToExtiInfo(PinId,&ExtiX,&SelY,&SelVal);
    ExtiIsrTab[ExtiX][SelY]= NULL;

    /*����EXTI*/
    if(ExtiX == EXTI0)
    {
        SelAddr = (INT32U *)(&GPIO_common->EXTI0_SEL);
    }
    else if(ExtiX == EXTI1)
    {
        SelAddr = (INT32U *)(&GPIO_common->EXTI1_SEL);
    }
    else if(ExtiX == EXTI2)
    {
        SelAddr = (INT32U *)(&GPIO_common->EXTI2_SEL);
    }
    else
    {
        return;
    }
    
    Set2bitValue( GPIO_EXTI_EDGE_DISABLE,SelAddr,SelY*2+16);/*����EDGE Mode*/

}


/*"************************************************			
Function: 	GPIO_Init		
Description: GPIO ��ʼ��
Input:  PinId
Output: ��	
Return: 
Others: 		
************************************************"*/	
void GPIO_Init(INT8U mode)
{
    INT8U i;
    ST_GPIO_CONFIG *GPIOConfig;

    BUILD_BUG_ON(GPIO_NUM != 69);
    BUILD_BUG_ON(ARRAY_SIZE(mstGPIOConfigPD) != GPIO_NUM);

    MCU_PeriphClkSet(PDC_num,ENABLE);

    LIB_MemSet(NULL, (INT8U * )&ExtiIsrTab[0][0], sizeof(ExtiIsrTab));

    INT_VectSet(E_INT_ID_EXTI, GPIO_EXTI_ISR);
    INT_En(E_INT_ID_EXTI);
    
    GPIO_common->ANASEL = (INT32U)0x37;//B0011_0111;/*ģ�⹦��ѡ��Ĵ���*/

    GPIO_common->FOUTSEL = (INT32U)0x05;//B0000_0101;/*FOUT���RTCTM*/  
    if(mode == MCU_RUNMODE)
    {
        GPIOConfig = (ST_GPIO_CONFIG *)mstGPIOConfig;
    }
    else
    {
        GPIOConfig = (ST_GPIO_CONFIG *)mstGPIOConfigPD;  
    }

    for(i = 0; i < GPIO_NUM ; i ++)
    {
        GPIO_ConfigPin(GPIOConfig[i].PinID, GPIOConfig[i].Dir, GPIOConfig[i].Mode);
        if(OUTPUT == GPIOConfig[i].Dir)
        {
            GPIO_OutputPin(GPIOConfig[i].PinID, GPIOConfig[i].Output);
        }
    }

}


