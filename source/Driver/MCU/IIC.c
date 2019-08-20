/*"***********************************************************		
FileName: IIC.c		
Author: 邹杨       
Version : 1.0       Date: 2016.12.22	
Description: IIC驱动实现
Function List: 		
<FunctionName>		
<author> <time> <version > <desc>		
	
***********************************************************"*/	
#include "Driver\MCU\IIC.h"
#include "Driver\MCU\GPIO.h"
#include "Driver\MCU\MCU.h"

const INT8U SCL[4]={MEM_SCL,LCD_SCL,TH_SCL,RTC_SCL};
const INT8U SDA[4]={MEM_SDA,LCD_SDA,TH_SDA,RTC_SDA};
const INT8U buildBugSCL[4]={0x0E,0xFF,0x24,0x62};

#define IIC_WAIT_US     (5)
#define POLL_ACK_TIMES  (15)

//#define HARDWARE_IIC
#ifndef HARDWARE_IIC
    #define SOFTWARE_IIC
#endif


#ifdef HARDWARE_IIC
  

#endif 
/****************对外接口**********************/
void IIC_Init(INT8U Iic_Id);
void IIC_Close(INT8U Iic_Id);
void IIC_Start (INT8U Iic_Id);
void IIC_Stop(INT8U Iic_Id);
INT8U  IIC_SendChar(INT8U IicId,INT8U * Buff,INT16U Len);
INT8U  IIC_ReadChar(INT8U IicId,INT8U * Buff,INT16U Len);

/***************内部函数**********************/
#ifdef SOFTWARE_IIC
static void Set_Sda_Dir(INT8U IIC_ID,INT8U Dir);
static void Set_Scl_Dir(INT8U IIC_ID,INT8U Dir);
static void Set_Scl_Val(INT8U IIC_ID,INT8U Val);
static void Set_Sda_Val(INT8U IIC_ID,INT8U Val);
static INT8U Get_Sda_Val(INT8U IIC_ID);
#else 
static void R_IICA0_Create(INT8U mode);
static void R_IICA0_Stop(void);
static void R_IICA0_Set_PowerOff(void);
#endif

/*********************************************************/
#ifdef SOFTWARE_IIC
static void Set_Sda_Dir(INT8U IIC_ID,INT8U Dir)
{
	if(Dir == INPUT)
	{
	    GPIO_ConfigPin(SDA[IIC_ID], Dir,NULL_IN);
	}
	else
	{
	    GPIO_ConfigPin(SDA[IIC_ID], Dir,OPENDRAIN_OUT);
	}	
	
	#if 0
    if(IIC_ID == IIC_ID_EEP)
    {

		if(Dir == INPUT)
	    {
	        GPIO_ConfigPin(MEM_SDA, Dir,NULL_IN);
	    }
	    else
	    {
	        GPIO_ConfigPin(MEM_SDA, Dir,OPENDRAIN_OUT);
	    }	
		
    }
    else if(IIC_ID == IIC_ID_LCD)
    {
		if(Dir == INPUT)
	    {
	        GPIO_ConfigPin(LCD_SDA, Dir,NULL_IN);
	    }
	    else
	    {
	        GPIO_ConfigPin(LCD_SDA, Dir,NORMAL_OUT);
	    }
    }
	else if(IIC_ID == IIC_ID_ERTC)
    {
		if(Dir == INPUT)
	    {
	        GPIO_ConfigPin(RTC_SDA, Dir,NULL_IN);
	    }
	    else
	    {
	        GPIO_ConfigPin(RTC_SDA, Dir,NORMAL_OUT);
	    }
    }
    else
    {

    }
	#endif
}

static void Set_Scl_Dir(INT8U IIC_ID,INT8U Dir)
{
	
        if(Dir == INPUT)
        {
            GPIO_ConfigPin(SCL[IIC_ID], Dir,NULL_IN);
        }
        else
        {
            GPIO_ConfigPin(SCL[IIC_ID], Dir,OPENDRAIN_OUT);
        }
    

	#if 0
    if(IIC_ID == IIC_ID_EEP)
    {
        if(Dir == INPUT)
        {
            GPIO_ConfigPin(MEM_SCL, Dir,NULL_IN);
        }
        else
        {
            GPIO_ConfigPin(MEM_SCL, Dir,OPENDRAIN_OUT);
        }
    }
    else if(IIC_ID == IIC_ID_LCD)
    {
        if(Dir == INPUT)
        {
            GPIO_ConfigPin(LCD_SCL, Dir,NULL_IN);
        }
        else
        {
            GPIO_ConfigPin(LCD_SCL, Dir,NORMAL_OUT);
        }
    }
	else if(IIC_ID == IIC_ID_ERTC)
    {
        if(Dir == INPUT)
        {
            GPIO_ConfigPin(RTC_SCL, Dir,NULL_IN);
        }
        else
        {
            GPIO_ConfigPin(RTC_SCL, Dir,NORMAL_OUT);
        }
    }
    else
    {
        if(Dir == INPUT)
        {
            //GPIO_ConfigPin(TH_SENSOR_SCL, Dir,NULL_IN);
        }
        else
        {
            //GPIO_ConfigPin(TH_SENSOR_SCL, Dir,OPENDRAIN_OUT);
        }
    }
	#endif


}

static void Set_Scl_Val(INT8U IIC_ID,INT8U Val)
{
	GPIOx_Type* GPIOi;
	INT16U reg; 

	//BUILD_BUG_ON(SCL[IIC_ID] != buildBugSCL[IIC_ID]);/*"指针的加法"*/
	GPIOi=GPIOA+((SCL[IIC_ID]>>4)&0x0F);
	reg=(SCL[IIC_ID]&0x0F);

    if(Val == HIGH)
    {
        GPIOi->BSETH = (INT32U)(0x01UL<<reg);//0x4000;
    }
    else
    {
        GPIOi->BRESETL = (INT32U)(0x01UL<<reg);//0x4000;
	}
    #if 0
    if(IIC_ID == IIC_ID_EEP)
    {
        //GPIO_OutputPin(MEM_SCL, Val);
        BUILD_BUG_ON(MEM_SCL != 0x0E);
        if(Val == HIGH)
        {
            GPIOA->BSETH = 0x4000;
        }
        else
        {
            GPIOA->BRESETL = 0x4000;

        }
    }
    else if(IIC_ID == IIC_ID_LCD)
    {
        //GPIO_OutputPin(LCD_SCL, Val);
        //BUILD_BUG_ON(LCD_SCL != 0x1F);
        if(Val == HIGH)
        {
            GPIOB->BSETH = 0x8000;
        }
        else
        {
            GPIOB->BRESETL = 0x8000;

        }

    }
	else if(IIC_ID == IIC_ID_ERTC)
    {
        //GPIO_OutputPin(LCD_SCL, Val);
        BUILD_BUG_ON(RTC_SCL != 0x62);
        if(Val == HIGH)
        {
            GPIOG->BSETH = 0x0004;
        }
        else
        {
            GPIOG->BRESETL = 0x0004;

        }

    }
    else
    {
        //GPIO_OutputPin(TH_SENSOR_SCL, Val);
        //BUILD_BUG_ON(TH_SENSOR_SCL != 0x62);
        if(Val == HIGH)
        {
            GPIOG->BSETH =   0x0004;
        }
        else
        {
            GPIOG->BRESETL = 0x0004;

        }
    }
	#endif
}

static void Set_Sda_Val(INT8U IIC_ID,INT8U Val)
{
	GPIOx_Type* GPIOi;
	INT16U reg; 

	//BUILD_BUG_ON(SCL[IIC_ID] != buildBugSCL[IIC_ID]);
	GPIOi=GPIOA+((SDA[IIC_ID]>>4)&0x0F);/*"指针的加法"*/
	reg=(SDA[IIC_ID]&0x0F);

    if(Val == HIGH)
    {
        GPIOi->BSETH = (0x01UL<<reg);//0x8000;
    }
    else
    {
        GPIOi->BRESETL = (0x01UL<<reg);//0x8000;
	}


	#if 0
    if(IIC_ID == IIC_ID_EEP)
    {
        //GPIO_OutputPin(MEM_SDA, Val);
        BUILD_BUG_ON(MEM_SDA != 0x0F);
        if(Val == HIGH)
        {
            GPIOA->BSETH = 0x8000;
        }
        else
        {
            GPIOA->BRESETL = 0x8000;
        }
    }
    else if(IIC_ID == IIC_ID_LCD)
    {
        //GPIO_OutputPin(LCD_SDA, Val);
        //BUILD_BUG_ON(LCD_SDA != 0x1E);
        if(Val == HIGH)
        {
            GPIOB->BSETH = 0x4000;
        }
        else
        {
            GPIOB->BRESETL = 0x4000;
        }

    }
    else
    {
        //GPIO_OutputPin(TH_SENSOR_SCL, Val);
        //BUILD_BUG_ON(TH_SENSOR_SDA != 0x63);
        if(Val == HIGH)
        {
            GPIOG->BSETH = 0x0008;
        }
        else
        {
            GPIOG->BRESETL = 0x0008;
        }

    }
	#endif
}

static INT8U Get_Sda_Val(INT8U IIC_ID)
{
    INT8U GPIO_PinId;
	
	GPIO_PinId = SDA[IIC_ID];
	return GPIO_ReadPin(GPIO_PinId);
	
	#if 0
    if(IIC_ID == IIC_ID_EEP)
    {
        GPIO_PinId = MEM_SDA;
    }
    else if(IIC_ID_LCD)
    {
        GPIO_PinId = LCD_SDA;
    }
	else if(IIC_ID_ERTC)
    {
        GPIO_PinId = RTC_SDA;
    }
    else
    {
        //GPIO_PinId = TH_SENSOR_SDA;
    }
	return GPIO_ReadPin(GPIO_PinId);
	#endif
    
}

#endif

/*"************************************************"*/		
/*"Function: 	IIC_Init		"*/
/*"Description: IIC初始化"*/
/*"Input:  IicId ：IIC总线编号"*/
/*"Output: 无	"*/
/*"Return: 无"*/
/*"Others: 		"*/
/*"************************************************"*/
void IIC_Init(INT8U IicId)
{
    GPIO_ConfigPin(FLASH_PWR,OUTPUT,NORMAL_OUT);
    GPIO_OutputPin(FLASH_PWR,LOW);
	#ifdef SOFTWARE_IIC
    GPIO_ConfigPin(SCL[IicId],INPUT,NULL_IN);
    GPIO_ConfigPin(SDA[IicId],INPUT,NULL_IN);
	if(IIC_ID_EEP == IicId)
    {
    	GPIO_ConfigPin(MEM_PWR,OUTPUT,NORMAL_OUT);
		GPIO_OutputPin(MEM_PWR, HIGH);
    }
    #endif
		
	
    #ifdef HARDWARE_IIC
	if(IIC_ID_EEP == IicId)
	{
        R_IICA0_Create(PWRMODE_UP);
	}
    #endif

        
	#if 0
    if(IIC_ID_EEP == IicId)
    {
        #ifdef HARDWARE_IIC
        R_IICA0_Create(PWRMODE_UP);
        #endif

        #ifdef SOFTWARE_IIC
        GPIO_ConfigPin(MEM_SCL,INPUT,NULL_IN);
        GPIO_ConfigPin(MEM_SDA,INPUT,NULL_IN);
		GPIO_ConfigPin(MEM_PWR,OUTPUT,NORMAL_OUT);
		GPIO_OutputPin(MEM_PWR, HIGH);
        #endif
    }
    else if(IIC_ID_LCD)
    {
        #ifdef HARDWARE_IIC
        #endif

        #ifdef SOFTWARE_IIC
        GPIO_ConfigPin(LCD_SCL,INPUT,NULL_IN);
        GPIO_ConfigPin(LCD_SDA,INPUT,NULL_IN);
        #endif
    }
	 else if(IIC_ID_ERTC)
    {
        #ifdef HARDWARE_IIC
        #endif

        #ifdef SOFTWARE_IIC
        GPIO_ConfigPin(RTC_SCL,INPUT,NULL_IN);
        GPIO_ConfigPin(RTC_SDA,INPUT,NULL_IN);
        #endif
    }
    else
    {
        #ifdef SOFTWARE_IIC
        GPIO_ConfigPin(TH_SCL,INPUT,NULL_IN);
        GPIO_ConfigPin(TH_SDA,INPUT,NULL_IN);
        #endif
    }
    #endif
}

void IIC_Close(INT8U Iic_Id)
{
	
#ifdef HARDWARE_IIC
	if(IIC_ID_EEP == Iic_Id)
    {
        R_IICA0_Stop();
        R_IICA0_Set_PowerOff();
        GPIO_ConfigPin(MEM_SCL,INPUT,NULL_IN);
        GPIO_ConfigPin(MEM_SDA,INPUT,NULL_IN);
	}
#endif

#ifdef SOFTWARE_IIC
        GPIO_ConfigPin(SCL[Iic_Id],INPUT,NULL_IN);
        GPIO_ConfigPin(SDA[Iic_Id],INPUT,NULL_IN);
#endif
        
	
	#if 0
    if(IIC_ID_EEP == Iic_Id)
    {
#ifdef HARDWARE_IIC
        R_IICA0_Stop();
        R_IICA0_Set_PowerOff();
        GPIO_ConfigPin(MEM_SCL,INPUT,NULL_IN);
        GPIO_ConfigPin(MEM_SDA,INPUT,NULL_IN);
#endif

#ifdef SOFTWARE_IIC
        GPIO_ConfigPin(MEM_SCL,INPUT,NULL_IN);
        GPIO_ConfigPin(MEM_SDA,INPUT,NULL_IN);
#endif
        
    }
    else if(IIC_ID_LCD == Iic_Id)
    {
        #ifdef HARDWARE_IIC
        #endif
        
        #ifdef SOFTWARE_IIC
        GPIO_ConfigPin(LCD_SCL,INPUT,NULL_IN);
        GPIO_ConfigPin(LCD_SDA,INPUT,NULL_IN);
        #endif
    }
	else if(IIC_ID_ERTC == Iic_Id)
    {
        #ifdef HARDWARE_IIC
        #endif
        
        #ifdef SOFTWARE_IIC
        GPIO_ConfigPin(RTC_SCL,INPUT,NULL_IN);
        GPIO_ConfigPin(RTC_SDA,INPUT,NULL_IN);
        #endif
    }
    else
    {
        #ifdef HARDWARE_IIC
        #endif
        
        #ifdef SOFTWARE_IIC
        //GPIO_ConfigPin(TH_SENSOR_SCL,INPUT,NULL_IN);
        //GPIO_ConfigPin(TH_SENSOR_SDA,INPUT,NULL_IN);
        #endif
    }
	#endif
}

/*"************************************************"*/			
/*"Function: 	IIC_Start		"*/
/*"Description: IIC开始信号"*/
/*"Input:  IIC_ID ：IIC总线编号"*/
/*"Output: 无	"*/
/*"Return: 无"*/
/*"Others: 		"*/
/*"************************************************"*/
void IIC_Start (INT8U IicId)
{   
    #ifdef HARDWARE_IIC

    #endif

    #ifdef SOFTWARE_IIC
        Set_Scl_Val(IicId, LOW);
        Set_Scl_Dir(IicId, OUTPUT);

        Set_Sda_Val(IicId, HIGH);
        Set_Sda_Dir(IicId, OUTPUT);

        Set_Scl_Val(IicId, HIGH);
        MCU_DelayUs(IIC_WAIT_US);
        Set_Sda_Val(IicId, LOW);    
        MCU_DelayUs(IIC_WAIT_US);
        Set_Scl_Val(IicId, LOW); 
    #endif

}

/*"************************************************"*/
/*"Function: 	IIC_Stop		"*/
/*"Description: IIC停止信号"*/
/*"Input:  IIC_ID ：IIC总线编号"*/
/*"Output: 无	"*/
/*"Return: 无"*/
/*"Others: 		"*/
/*"************************************************"*/
void IIC_Stop(INT8U IicId)
{

    #ifdef HARDWARE_IIC

    #endif

    #ifdef SOFTWARE_IIC
        Set_Scl_Val(IicId,  LOW);
        Set_Scl_Dir(IicId,  OUTPUT);

        Set_Sda_Val(IicId, LOW);
        Set_Sda_Dir(IicId, OUTPUT);

        Set_Scl_Val(IicId, HIGH);
        MCU_DelayUs(IIC_WAIT_US);
        Set_Sda_Val(IicId, HIGH);
        MCU_DelayUs(IIC_WAIT_US);

        Set_Scl_Dir(IicId, INPUT);
        Set_Sda_Dir(IicId, INPUT);
    #endif


}


/*"************************************************"*/
/*"Function: 	IIC_SendChar		"*/
/*"Description: IIC发送一串字节"*/
/*"Input:  IicId ：IIC总线编号，
            Buff :发送数据指针,如果为NULL，代表发送数据为全0
            Len ： 发送的字节数"*/
/*"Output: 无	"*/
/*"Return: TRUE : 发送成功 ， _FALSE:发送失败"*/
/*"Others: 		"*/
/*"************************************************"*/
INT8U  IIC_SendChar(INT8U IicId,INT8U * Buff,INT16U Len)
{
    INT8U result;
    INT16U SendCnt;
#ifdef SOFTWARE_IIC
    INT8U BitCnt;
    INT8U i;
#endif
    INT8U InCh;
    

    #ifdef HARDWARE_IIC
       
    #endif
    
    #ifdef SOFTWARE_IIC
        result = TRUE;
        InCh = 0 ;
        for(SendCnt=0; (SendCnt<Len) && (result== TRUE); SendCnt++)
        {
            Set_Sda_Dir(IicId, OUTPUT);
            if(Buff != NULL)
            {
                InCh = *(Buff+SendCnt);
            }
            else
            {
            
            }
            for(BitCnt = 0;BitCnt < 8;BitCnt++)
            {
                Set_Scl_Val(IicId, LOW);
                MCU_DelayUs(IIC_WAIT_US);
                if(InCh&0x80)
                {
                    Set_Sda_Val(IicId, HIGH);
                }
                else
                {
                    Set_Sda_Val(IicId, LOW);
                }
                InCh <<=1;
                Set_Scl_Val(IicId, HIGH);
                MCU_DelayUs(IIC_WAIT_US);
            }
            
            Set_Scl_Val(IicId, LOW);

            Set_Sda_Dir(IicId, INPUT);	
        	    
            Set_Scl_Val(IicId, HIGH);   
            MCU_DelayUs(IIC_WAIT_US);
            for(i = 0; i < POLL_ACK_TIMES; i++)
            {
                if(Get_Sda_Val(IicId) == HIGH)
                {
                    result = FALSE;
                }
                else
                {
                    result = TRUE;
                    break;  /*"检测到应答信号，退出循环"*/
                }
            }
            Set_Scl_Val(IicId, LOW);
      
        }

        Set_Sda_Dir(IicId, OUTPUT);
        Set_Sda_Val(IicId, LOW);	
        
        return result;
    #endif


}

/*"************************************************"*/
/*"Function: 	IIC_ReadChar		"*/
/*"Description: IIC发送一串字节"*/
/*"Input:  IicId ：IIC总线编号，
            Buff :读取数据指针
            Len ： 读取的字节数"*/
/*"Output: 无	"*/
/*"Return: TRUE : 发送成功 ， _FALSE:发送失败"*/
/*"Others: 		"*/
/*"************************************************"*/
INT8U  IIC_ReadChar(INT8U IicId,INT8U * Buff,INT16U Len)
{



        #ifdef HARDWARE_IIC
           
        #endif
        
        #ifdef SOFTWARE_IIC
            INT8U  mReadChar;
            INT16U ReadCnt;
            INT8U i;
            
            for(ReadCnt=0; ReadCnt<Len; ReadCnt++)
            {
                mReadChar = 0;
                Set_Sda_Dir(IicId, INPUT);
                Set_Scl_Val(IicId, LOW);
                MCU_DelayUs(IIC_WAIT_US);
                for(i = 0;i < 8;i++)
                { 
                    Set_Scl_Val(IicId, HIGH);
                    mReadChar <<= 1;
                    MCU_DelayUs(IIC_WAIT_US);
                    if(Get_Sda_Val(IicId)==HIGH)
                    {
                        mReadChar |= 0x01;
                    }
                    else
                    {
                        mReadChar &= 0xFE;
                    } 
                    Set_Scl_Val(IicId, LOW);
                    MCU_DelayUs(IIC_WAIT_US);
                }
                *(Buff+ReadCnt) = mReadChar;
                
                if(ReadCnt == (Len-1)) /*最后一个字节*/
                {
                    Set_Sda_Val(IicId, HIGH);
                }
                else
                {
                    Set_Sda_Val(IicId, LOW);
                }
                Set_Sda_Dir(IicId, OUTPUT);
                
                Set_Scl_Val(IicId, HIGH);
                MCU_DelayUs(IIC_WAIT_US);
                Set_Scl_Val(IicId, LOW);

            }

            Set_Sda_Val(IicId, LOW);
            return  TRUE;
        
        #endif


}


