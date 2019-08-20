/*"****************************************************************************
Description:自监测数据存储和读取接口
Function List:
    1:MSR_Init()                     自监测初始化函数
    2:MSR_Main()                    自监测主处理函数
    3:MSR_GetRecord()            自监测查询函数
    4:MSR_SaveRecord()             自监测数据记录函数
Author:YWJ
Date:2018-05-14
*****************************************************************************"*/
#include "MSR.h"
#include "system\Sys_def.h"
#include "APP\message\MS.h"
#include "DataCenterSoftware\Code\message\MS_OBIS.h"
#include "APP\message\FRMT_def.h"
#include "Driver\device\FLASH.h"
#include "Driver\hal\fs\FS_EepConfig.h"
#include "Driver\hal\fs\FS_FlashConfig.h"
//#include "app\userinterface\PP\PP_698\Class\class_comm.h"
//#include "app\userinterface\PP\PP_698\Process\Pp_obis.h"
//#include "app\userinterface\PP\PP_698\Process\Pp_macro.h"
//#include "app\userinterface\PP\PP_698\Process\Pp_process.h"
#include "App\UserInterface\PP\PP_645\PP_ProtocolTree.h"

extern INT8U mSureManagerFrame22byte[Frame_LENGTH22];   /*在线监测645 南网协议数据*/
//#include "RCRD.h"
//#include "LR.h"
//#include "RCRD_dataProcess.h"
typedef struct
{
INT32U startaddr;
INT16U ValidNum;
}ST_MSRCD_INDEX;

typedef struct
{
ST_5TIME_FORM time;
INT8U data[16];
}ST_MSRCD_DATA;

typedef struct
{
INT8U hour;
INT8U day;
}ST_MSRCD_DAYHOUR;

typedef struct
{
ST_5TIME_FORM time;
INT8U Version[1];
INT8U AI_Event[2];
INT8U BI_Event[2];
INT8U AV_Event[2];
INT8U AI_CC[2];
INT8U AI_Cert[1];
INT8U BI_CC[2];
INT8U BI_Cert[1];
INT8U AV_CC[2];
INT8U AV_Cert[1];
}ST_MSRSRC_DATA;

typedef struct
{
ST_5TIME_FORM time;
INT8U Version[1];
INT8U AV_Event[2];
INT8U AI_Event[2];
INT8U RSV1[2];
INT8U AV_CC[2];
INT8U AV_Cert[1];
INT8U RSV2[2];
INT8U AI_CC[2];
INT8U AI_Cert[1];
INT8U RSV3[2];
}ST_MSRDATA_PHASEL;

typedef struct
{
ST_5TIME_FORM time;
INT8U Version[1];
INT8U RSV1[2];
INT8U NI_Event[2];
INT8U RSV2[2];
INT8U RSV3[2];
INT8U RSV4[1];
INT8U RSV5[2];
INT8U NI_CC[2];
INT8U NI_Cert[1];
INT8U RSV6[2];
}ST_MSRDATA_PHASEN;

typedef struct
{
INT8U* SoureAddr;
INT8U* DestAddr;
INT8U  Datalen;
}ST_DATACONV_TABLE;

ST_MSRCD_INDEX mstMsrIndex;
ST_MSRSRC_DATA  mstMsrData;
ST_MSRDATA_PHASEL mstMsrTempL;
ST_MSRDATA_PHASEN mstMsrTempN;
INT32U mstMsrTempAddr;
INT8U mstACKNum;
INT8U mstACKNumNext;
INT8U mstStartNum;
INT8U mstNextNum;
INT8U mstOverFlag;
const ST_DATACONV_TABLE ConverTable1[7] =
{
	{(INT8U *)&mstMsrData.time.min, (INT8U *)&mstMsrTempL.time.min, 5},
	{mstMsrData.AI_CC, mstMsrTempL.AI_CC, 2},
	{mstMsrData.AI_Event, mstMsrTempL.AI_Event, 2},
	{mstMsrData.AV_CC, mstMsrTempL.AV_CC, 2},
	{mstMsrData.AV_Event, mstMsrTempL.AV_Event, 2},
	{mstMsrData.AI_Cert, mstMsrTempL.AI_Cert, 1},
	{mstMsrData.AV_Cert, mstMsrTempL.AV_Cert, 1}
};
const ST_DATACONV_TABLE ConverTable2[4] =
{
	{(INT8U *)&mstMsrData.time.min, (INT8U *)&mstMsrTempN.time.min, 5},
	{mstMsrData.BI_CC, mstMsrTempN.NI_CC, 2},
	{mstMsrData.BI_Event, mstMsrTempN.NI_Event, 2},
	{mstMsrData.BI_Cert, mstMsrTempN.NI_Cert, 1}
};

void MsrClear(void);
void MsrInit(void);
void MsrMain(void);
void MsrSaveRecord(INT8U *data);
INT8U MsrSetTime(INT8U *time);
static INT8U MsrFindTime(INT8U *Intime, INT8U *Starttime, INT8U *Endtime,INT8U *outoffset);
static void MsrCalAddr( INT16U num);
// todo static INT16U MsrABS(INT16S data);
static void MsrDataConv(void);

void MsrClear(void)
{
	mstMsrIndex.startaddr = _MSR_DATAL_ADDR;
	mstMsrIndex.ValidNum = 0;
	 mstACKNum = 0;
     mstACKNumNext = 0;
	 mstNextNum = 0;
	 //Flash_Write(_MSR_INDEX_ADDR,  (INT8U *)&mstMsrIndex.startaddr, sizeof(ST_MSRCD_INDEX));
	EEP_Write(_MSR_INDEX_ADDR, (INT8U *)&mstMsrIndex.startaddr,sizeof(ST_MSRCD_INDEX));
}
void MsrInit(void)
{
	mstACKNum = 0;
	mstACKNumNext = 0;
	mstNextNum = 0;
	mstOverFlag =0;
	//Flash_Read(_MSR_INDEX_ADDR,  (INT8U *)&mstMsrIndex.startaddr, sizeof(ST_MSRCD_INDEX));
	EEP_Read(_MSR_INDEX_ADDR, (INT8U *)&mstMsrIndex.startaddr,sizeof(ST_MSRCD_INDEX));
}
void MsrDataConv(void)
{
	INT8U i;
	INT16U temp;
	LIB_MemSet(0xFF, (INT8U *)&mstMsrTempL.time.min, sizeof(ST_MSRDATA_PHASEL));
	LIB_MemSet(0xFF, (INT8U *)&mstMsrTempN.time.min, sizeof(ST_MSRDATA_PHASEN));
	mstMsrTempL.Version[0] = 1;
    mstMsrTempN.Version[0] = 1;
	for(i= 0; i< 7; i++)
	{
		LIB_MemCpy(ConverTable1[i].SoureAddr, ConverTable1[i].DestAddr, ConverTable1[i].Datalen);

	}
	for(i= 0; i< 4; i++)
	{
		LIB_MemCpy(ConverTable2[i].SoureAddr, ConverTable2[i].DestAddr, ConverTable2[i].Datalen);

	}
	for(i= 0; i< 4; i++)
		{
		    if((i ==1)||(i ==3))
		    	{
					LIB_MemCpy(ConverTable1[i].DestAddr, (INT8U *)&temp, 2);
					temp = temp*5;
					LIB_MemCpy((INT8U *)&temp, ConverTable1[i].DestAddr, 2);
		    	}
		}
	for(i= 0; i< 2; i++)
	{
	    if(i ==1)
	    	{
				LIB_MemCpy(ConverTable2[i].DestAddr, (INT8U *)&temp, 2);
				temp = temp*5;
				LIB_MemCpy((INT8U *)&temp, ConverTable2[i].DestAddr, 2);
	    	}
	}
}

void MsrMain(void)
{
   if(mSureManagerFrame22byte[0] == 1) 
 	{
	 mSureManagerFrame22byte[0] = 0;

	 LIB_MemCpy((INT8U *)&mSureManagerFrame22byte[1], (INT8U *)&mstMsrData.time.min,21);
	 
	 MsrDataConv();
	 MsrSaveRecord(_NULL);

    }
}

INT16U MsrGetRecord(INT8U *indata,INT8U *outdata,INT8U type)
{
  INT8U *pDatain,*pDataout;
  INT8U Num,i,tempbuff[200],convbuf[200];
  INT16U AddrOffest,Result;
  ST_6TIME_FORM BeginTime,RcdTimeStart,RcdTimesEnd;
  INT32U InMinutes,StartMinutes,EndMinutes,TempaddrBegin,TempaddrEnd;
  pDatain = indata;
  pDataout = outdata;
  Result = 0;
  TempaddrBegin = 0;
  TempaddrEnd = 0;
 
  if((type == 0)||(type == 1)||(type == 10))
	{
	  LIB_MemSet(0,&BeginTime.sec,6);
	  LIB_MemSet(0,&RcdTimeStart.sec,6);
	  LIB_MemSet(0,&RcdTimesEnd.sec,6);
	  //LIB_MemCpy(pDatain, (INT8U *)&Num,1);
	  LIB_BcdToHex(1,pDatain, (INT8U *)&Num);
	  LIB_MemCpy(pDatain+1, (INT8U *)&BeginTime.min,5);
	  if((LIB_ChkDateTime((ST_6TIME_FORM *)&BeginTime.sec)== FALSE)||(0 == mstMsrIndex.ValidNum))
	  {
		Result = 0;
		return Result;
	  }
	  Flash_Read(mstMsrIndex.startaddr, (INT8U *)&RcdTimesEnd.min, 5);
	  //EEP_Read(mstMsrIndex.startaddr, (INT8U *)&RcdTimesEnd.min, 5);
	  MsrCalAddr(mstMsrIndex.ValidNum);
	  Flash_Read(mstMsrTempAddr, (INT8U *)&RcdTimeStart.min, 5);
	  //EEP_Read(mstMsrTempAddr, (INT8U *)&RcdTimeStart.min, 5);
	  BeginTime.min = 0;
	  RcdTimeStart.min = 0;
	  RcdTimesEnd.min = 0;
	  InMinutes = LIB_MToStdDate((ST_6TIME_FORM *)&BeginTime.sec);
	  StartMinutes = LIB_MToStdDate((ST_6TIME_FORM *)&RcdTimeStart.sec);
	  EndMinutes = LIB_MToStdDate((ST_6TIME_FORM *)&RcdTimesEnd.sec);
		if(InMinutes > EndMinutes)
		{
		mstOverFlag = FALSE;
			Result = 0;
			return Result;
		}
		else if((InMinutes < StartMinutes)||(InMinutes == StartMinutes))
		{
			//mstMsrTempAddr = mstMsrIndex.startaddr;
			mstOverFlag = TRUE;
		    if(mstMsrIndex.ValidNum < Num)
			 {
				mstACKNum = mstMsrIndex.ValidNum;
			 }
			 else	
			 {
				mstACKNum = Num;
			 }
		}
		else if(InMinutes == EndMinutes)
		{
		  mstACKNum = 1;
		  mstStartNum = 1;
		  mstOverFlag = FALSE;
		}
		else 
		{
		 mstOverFlag = FALSE;
		  Result = MsrFindTime((INT8U *)&InMinutes, (INT8U *)&StartMinutes, (INT8U *)&EndMinutes, (INT8U *)&AddrOffest);
	      if(TRUE == Result)
	      {
			  MsrCalAddr(AddrOffest);
			  mstStartNum = AddrOffest;

		   if(AddrOffest < Num)
			 {
				mstACKNum = AddrOffest;
			 }
			 else	
			 {
				mstACKNum = Num;
			 }
		  }
		}
		if((mstACKNum > 4)&&(type == 10))
	  	{
			if(mstOverFlag == TRUE)
				{
					 
				 TempaddrBegin = mstMsrTempAddr;
				 MsrCalAddr(mstMsrIndex.ValidNum-4);
				 TempaddrEnd = mstMsrTempAddr;

				}
			else
				{
				  //MsrCalAddr(mstACKNum);
				  TempaddrBegin = mstMsrTempAddr;
				  MsrCalAddr(mstStartNum-4);
				  TempaddrEnd = mstMsrTempAddr;
				}
			  if( TempaddrBegin < TempaddrEnd)
				 {
		          for(i = 0; i < 4; i++)
					{
					  Flash_Read(TempaddrBegin , (INT8U *)&tempbuff[2*i*(sizeof(ST_MSRDATA_PHASEL))], sizeof(ST_MSRDATA_PHASEL));
					  Flash_Read(TempaddrBegin + MSR_DATA_OFFSET, (INT8U *)&tempbuff[2*i*(sizeof(ST_MSRDATA_PHASEL)) + sizeof(ST_MSRDATA_PHASEL)], sizeof(ST_MSRDATA_PHASEN));
					  TempaddrBegin +=   sizeof(ST_MSRDATA_PHASEL);

					}
				  
					  //EEP_Read(TempaddrBegin,tempbuff,9*sizeof(ST_MSRCD_DATA));
				 }
			  else
				 {

					Flash_Read(TempaddrBegin, tempbuff, (_MSR_DATAL_END -TempaddrBegin));
					Flash_Read(MSR_DATAL_ADDR,(INT8U *)&tempbuff[MSR_DATAL_END -TempaddrBegin], 4*sizeof(ST_MSRDATA_PHASEL)-(_MSR_DATAL_END - TempaddrBegin));
					Flash_Read(TempaddrBegin + MSR_DATA_OFFSET, (INT8U *)&tempbuff[100], (MSR_DATAN_END -TempaddrBegin - MSR_DATA_OFFSET));
					Flash_Read(MSR_DATAN_ADDR,(INT8U *)&tempbuff[100 + MSR_DATAN_END -TempaddrBegin - MSR_DATA_OFFSET], 4*sizeof(ST_MSRDATA_PHASEN)-(MSR_DATAN_END -TempaddrBegin- MSR_DATA_OFFSET));
					/*这里前提是ST_MSRDATA_PHASEL == ST_MSRDATA_PHASEN*/
					LIB_MemCpy(tempbuff, convbuf, 200);
					for(i = 0; i < 4; i++)
					{
						LIB_MemCpy((INT8U *)&convbuf[100 + i*sizeof(ST_MSRDATA_PHASEL)], (INT8U *)&tempbuff[2*i*sizeof(ST_MSRDATA_PHASEL)+ sizeof(ST_MSRDATA_PHASEL)], sizeof(ST_MSRDATA_PHASEL));
						LIB_MemCpy((INT8U *)&convbuf[2*i*sizeof(ST_MSRDATA_PHASEL)], (INT8U *)&tempbuff[2*i*sizeof(ST_MSRDATA_PHASEL)], sizeof(ST_MSRDATA_PHASEL));

					}
				  //EEP_Read(TempaddrBegin,tempbuff,(_MSR_DATA_END -TempaddrBegin));
				  //EEP_Read(_MSR_DATA_ADDR,(INT8U *)&tempbuff[_MSR_DATA_END -TempaddrBegin],(9*sizeof(ST_MSRCD_DATA)) - (_MSR_DATA_END -TempaddrBegin));
		  
				 }
				  LIB_MemCpy(tempbuff, pDataout, 8*sizeof(ST_MSRDATA_PHASEL));
				  Result = 8*sizeof(ST_MSRDATA_PHASEL);

			  Result |= 0x8000;
			  mstACKNumNext = mstACKNum - 4;
			  mstStartNum = mstStartNum -4;
		    }
		   else if((mstACKNum > 9)&&((type == 0)||(type == 1)))
		  	{
			   if(mstOverFlag == TRUE)
			   {
					
				TempaddrBegin = mstMsrTempAddr;
				MsrCalAddr(mstMsrIndex.ValidNum-4);
				TempaddrEnd = mstMsrTempAddr;
   
			   }
		   else
			   {

			  //MsrCalAddr(mstACKNum);
			  TempaddrBegin = mstMsrTempAddr;
			  MsrCalAddr(mstStartNum-9);
			  TempaddrEnd = mstMsrTempAddr;
		   	}
			  if( TempaddrBegin < TempaddrEnd)
				 {
				 if(type == 0)
				 	{
					  Flash_Read(TempaddrBegin, tempbuff, 9*sizeof(ST_MSRDATA_PHASEL));
				 	}
				 if(type == 1)
				 	{
					  Flash_Read(TempaddrBegin + MSR_DATA_OFFSET, tempbuff, 9*sizeof(ST_MSRDATA_PHASEL));
				 	}

				  //EEP_Read(TempaddrBegin,tempbuff,9*sizeof(ST_MSRCD_DATA));
				 }
			  else
				 {
				  if(type == 0)
					 {
						Flash_Read(TempaddrBegin, tempbuff, (_MSR_DATAL_END -TempaddrBegin));
						Flash_Read(_MSR_DATAL_ADDR,(INT8U *)&tempbuff[_MSR_DATAL_END -TempaddrBegin], 9*sizeof(ST_MSRDATA_PHASEL)-(_MSR_DATAL_END - TempaddrBegin));
					 }
				  if(type == 1)
					 {
						Flash_Read(TempaddrBegin + MSR_DATA_OFFSET, tempbuff, (MSR_DATAN_END -TempaddrBegin - MSR_DATA_OFFSET));
						Flash_Read(MSR_DATAN_ADDR,(INT8U *)&tempbuff[MSR_DATAN_END -TempaddrBegin - MSR_DATA_OFFSET], 9*sizeof(ST_MSRDATA_PHASEL)-(_MSR_DATAN_END -TempaddrBegin- MSR_DATA_OFFSET));
					 }


				  //EEP_Read(TempaddrBegin,tempbuff,(_MSR_DATA_END -TempaddrBegin));
				  //EEP_Read(_MSR_DATA_ADDR,(INT8U *)&tempbuff[_MSR_DATA_END -TempaddrBegin],(9*sizeof(ST_MSRCD_DATA)) - (_MSR_DATA_END -TempaddrBegin));
		  
				 }
				  LIB_MemCpy(tempbuff, pDataout, 9*sizeof(ST_MSRDATA_PHASEL));
				  Result = 9*sizeof(ST_MSRDATA_PHASEL);

			  Result |= 0x8000;
			  mstACKNumNext = mstACKNum - 9;
			  mstStartNum = mstStartNum -9;
		    }
		  else
		  	{
			  if(mstOverFlag == TRUE)
			 {
				  
			  
 
			 }
		 else
			 {

			 MsrCalAddr(mstStartNum);
		 	}
			 if(mstMsrTempAddr <= mstMsrIndex.startaddr)
			 	{
					if(type == 0)
					{
						Flash_Read(mstMsrTempAddr, tempbuff, mstACKNum*sizeof(ST_MSRDATA_PHASEL));
					}
					if(type == 1)
					 {
						Flash_Read(mstMsrTempAddr + MSR_DATA_OFFSET, tempbuff, mstACKNum*sizeof(ST_MSRDATA_PHASEL));
					 }
					if(type == 10)
					{
						for(i = 0; i < mstACKNum; i++)
						{
							Flash_Read(mstMsrTempAddr , (INT8U *)&tempbuff[2*i*(sizeof(ST_MSRDATA_PHASEL))], sizeof(ST_MSRDATA_PHASEL));
							Flash_Read(mstMsrTempAddr + MSR_DATA_OFFSET, (INT8U *)&tempbuff[2*i*(sizeof(ST_MSRDATA_PHASEL)) + sizeof(ST_MSRDATA_PHASEL)], sizeof(ST_MSRDATA_PHASEN));
							mstMsrTempAddr += sizeof(ST_MSRDATA_PHASEL);

						}
					}
				 //EEP_Read(mstMsrTempAddr,tempbuff,mstACKNum*sizeof(ST_MSRCD_DATA));
			    }
			 else
			 	{
					if(type == 0)
					{
						 Flash_Read(mstMsrTempAddr, tempbuff, (_MSR_DATAL_END -mstMsrTempAddr));
						 Flash_Read(_MSR_DATAL_ADDR,(INT8U *)&tempbuff[_MSR_DATAL_END -mstMsrTempAddr], mstACKNum*sizeof(ST_MSRDATA_PHASEL)-(_MSR_DATAL_END - mstMsrTempAddr));
					}
					if(type == 1)
					{
						 Flash_Read(mstMsrTempAddr + MSR_DATA_OFFSET, tempbuff, (MSR_DATAN_END -mstMsrTempAddr - MSR_DATA_OFFSET));
						 Flash_Read(MSR_DATAN_ADDR,(INT8U *)&tempbuff[MSR_DATAN_END -mstMsrTempAddr - MSR_DATA_OFFSET], mstACKNum*sizeof(ST_MSRDATA_PHASEL)-(MSR_DATAN_END -mstMsrTempAddr- MSR_DATA_OFFSET));
					}
					if(type == 10)
					{
						Flash_Read(mstMsrTempAddr, tempbuff, (_MSR_DATAL_END -mstMsrTempAddr));
						Flash_Read(MSR_DATAL_ADDR,(INT8U *)&tempbuff[_MSR_DATAL_END -mstMsrTempAddr], mstACKNum*sizeof(ST_MSRDATA_PHASEL)-(_MSR_DATAL_END - mstMsrTempAddr));
						Flash_Read(mstMsrTempAddr + MSR_DATA_OFFSET, (INT8U *)&tempbuff[100], (MSR_DATAN_END -mstMsrTempAddr - MSR_DATA_OFFSET));
						Flash_Read(MSR_DATAN_ADDR,(INT8U *)&tempbuff[100 + MSR_DATAN_END -mstMsrTempAddr - MSR_DATA_OFFSET], mstACKNum*sizeof(ST_MSRDATA_PHASEN)-(MSR_DATAN_END -mstMsrTempAddr- MSR_DATA_OFFSET));
						/*这里前提是ST_MSRDATA_PHASEL == ST_MSRDATA_PHASEN*/
						LIB_MemCpy(tempbuff, convbuf, 200);
						for(i = 0; i < mstACKNum; i++)
						{
							LIB_MemCpy((INT8U *)&convbuf[100 + i*sizeof(ST_MSRDATA_PHASEL)], (INT8U *)&tempbuff[2*i*sizeof(ST_MSRDATA_PHASEL)+ sizeof(ST_MSRDATA_PHASEL)], sizeof(ST_MSRDATA_PHASEL));
							LIB_MemCpy((INT8U *)&convbuf[2*i*sizeof(ST_MSRDATA_PHASEL)], (INT8U *)&tempbuff[2*i*sizeof(ST_MSRDATA_PHASEL)], sizeof(ST_MSRDATA_PHASEL));

						}

					}
				 //EEP_Read(mstMsrTempAddr,tempbuff,(_MSR_DATA_END -mstMsrTempAddr));
				 //EEP_Read(_MSR_DATA_ADDR,(INT8U *)&tempbuff[_MSR_DATA_END -mstMsrTempAddr],(mstACKNum*sizeof(ST_MSRCD_DATA)) - (_MSR_DATA_END -mstMsrTempAddr));

				}
				 if(type == 10)
			 	{
					 LIB_MemCpy(tempbuff, pDataout, 2*mstACKNum*sizeof(ST_MSRDATA_PHASEL));
					 Result = 2*mstACKNum*sizeof(ST_MSRDATA_PHASEL);
			 	}
				 else
			 	{
					 LIB_MemCpy(tempbuff, pDataout, mstACKNum*sizeof(ST_MSRDATA_PHASEL));
					 Result = mstACKNum*sizeof(ST_MSRDATA_PHASEL);
			 	}
		    }
	}
	else if((type == 2)||(type == 3)||(type == 12))
	{
		

		if((mstACKNumNext > 9)&&((type == 2)||(type == 3)))
		  {
			   mstNextNum = (*indata)*9;
				if(mstOverFlag == TRUE)
			   {
					MsrCalAddr(mstMsrIndex.ValidNum-mstNextNum);
					TempaddrBegin = mstMsrTempAddr;
					MsrCalAddr(mstMsrIndex.ValidNum-mstNextNum-9);
					TempaddrEnd = mstMsrTempAddr;

			   }
		   else
			   {

				MsrCalAddr(mstStartNum);
				TempaddrBegin = mstMsrTempAddr;
				MsrCalAddr(mstStartNum - 9);
				TempaddrEnd = mstMsrTempAddr;

			   }
			if( TempaddrBegin < TempaddrEnd)
			 {
				 if(type == 2)
				 	{
					  Flash_Read(TempaddrBegin, tempbuff, 9*sizeof(ST_MSRDATA_PHASEL));
				 	}
				 if(type == 3)
				 	{
					  Flash_Read(TempaddrBegin + MSR_DATA_OFFSET, tempbuff, 9*sizeof(ST_MSRDATA_PHASEL));
				 	}

				  //EEP_Read(TempaddrBegin,tempbuff,9*sizeof(ST_MSRCD_DATA));
			 }
			else
			 {
				  if(type == 2)
					 {
						Flash_Read(TempaddrBegin, tempbuff, (_MSR_DATAL_END -TempaddrBegin));
						Flash_Read(_MSR_DATAL_ADDR,(INT8U *)&tempbuff[_MSR_DATAL_END -TempaddrBegin], 9*sizeof(ST_MSRDATA_PHASEL)-(_MSR_DATAL_END - TempaddrBegin));
					 }
				  if(type == 3)
					 {
						Flash_Read(TempaddrBegin + MSR_DATA_OFFSET, tempbuff, (MSR_DATAN_END -TempaddrBegin - MSR_DATA_OFFSET));
						Flash_Read(MSR_DATAN_ADDR,(INT8U *)&tempbuff[MSR_DATAN_END -TempaddrBegin - MSR_DATA_OFFSET], 9*sizeof(ST_MSRDATA_PHASEL)-(MSR_DATAN_END -TempaddrBegin- MSR_DATA_OFFSET));
					 }


				  //EEP_Read(TempaddrBegin,tempbuff,(_MSR_DATA_END -TempaddrBegin));
				  //EEP_Read(_MSR_DATA_ADDR,(INT8U *)&tempbuff[_MSR_DATA_END -TempaddrBegin],(9*sizeof(ST_MSRCD_DATA)) - (_MSR_DATA_END -TempaddrBegin));

			 }
			  LIB_MemCpy(tempbuff, pDataout, 9*sizeof(ST_MSRDATA_PHASEL));
			  Result = 9*sizeof(ST_MSRDATA_PHASEL);

			Result |= 0x8000;

			mstACKNumNext = mstACKNumNext - 9;
			mstStartNum =mstStartNum -9;
		}
	  else if((mstACKNumNext > 4)&&(type == 12))
	  	{
		  if(mstOverFlag == TRUE)
		 {
			 mstNextNum = (*indata)*4;
			  MsrCalAddr(mstMsrIndex.ValidNum-mstNextNum);
			  TempaddrBegin = mstMsrTempAddr;
			  MsrCalAddr(mstMsrIndex.ValidNum-mstNextNum-4);
			  TempaddrEnd = mstMsrTempAddr;

		 }
	 else
		 {

			MsrCalAddr(mstStartNum);
			TempaddrBegin = mstMsrTempAddr;
			MsrCalAddr(mstStartNum - 4);
			TempaddrEnd = mstMsrTempAddr;
	 	}
			if( TempaddrBegin < TempaddrEnd)
			 {
				  for(i = 0; i < 4; i++)
					{
					  Flash_Read(TempaddrBegin , (INT8U *)&tempbuff[2*i*(sizeof(ST_MSRDATA_PHASEL))], sizeof(ST_MSRDATA_PHASEL));
					  Flash_Read(TempaddrBegin + MSR_DATA_OFFSET, (INT8U *)&tempbuff[2*i*(sizeof(ST_MSRDATA_PHASEL)) + sizeof(ST_MSRDATA_PHASEL)], sizeof(ST_MSRDATA_PHASEN));
						TempaddrBegin +=   sizeof(ST_MSRDATA_PHASEL);
					}
				  
					  //EEP_Read(TempaddrBegin,tempbuff,9*sizeof(ST_MSRCD_DATA));
			 }
		  else
			 {
			  
					Flash_Read(TempaddrBegin, tempbuff, (_MSR_DATAL_END -TempaddrBegin));
					Flash_Read(_MSR_DATAL_ADDR,(INT8U *)&tempbuff[_MSR_DATAL_END -TempaddrBegin], 4*sizeof(ST_MSRDATA_PHASEL)-(_MSR_DATAL_END - TempaddrBegin));
					Flash_Read(TempaddrBegin + MSR_DATA_OFFSET, (INT8U *)&tempbuff[100], (MSR_DATAN_END -TempaddrBegin - MSR_DATA_OFFSET));
					Flash_Read(MSR_DATAN_ADDR,(INT8U *)&tempbuff[100 + MSR_DATAN_END -TempaddrBegin - MSR_DATA_OFFSET], 4*sizeof(ST_MSRDATA_PHASEN)-(MSR_DATAN_END -TempaddrBegin- MSR_DATA_OFFSET));
					/*这里前提是ST_MSRDATA_PHASEL == ST_MSRDATA_PHASEN*/
					LIB_MemCpy(tempbuff, convbuf, 200);
					for(i = 0; i < 4; i++)
					{
						LIB_MemCpy((INT8U *)&convbuf[100 + i*sizeof(ST_MSRDATA_PHASEL)], (INT8U *)&tempbuff[2*i*sizeof(ST_MSRDATA_PHASEL)+ sizeof(ST_MSRDATA_PHASEL)], sizeof(ST_MSRDATA_PHASEL));
						LIB_MemCpy((INT8U *)&convbuf[2*i*sizeof(ST_MSRDATA_PHASEL)], (INT8U *)&tempbuff[2*i*sizeof(ST_MSRDATA_PHASEL)], sizeof(ST_MSRDATA_PHASEL));
					}
					  //EEP_Read(TempaddrBegin,tempbuff,(_MSR_DATA_END -TempaddrBegin));
					  //EEP_Read(_MSR_DATA_ADDR,(INT8U *)&tempbuff[_MSR_DATA_END -TempaddrBegin],(9*sizeof(ST_MSRCD_DATA)) - (_MSR_DATA_END -TempaddrBegin));
				  
			 }
				  LIB_MemCpy(tempbuff, pDataout, 8*sizeof(ST_MSRDATA_PHASEL));
				  Result = 8*sizeof(ST_MSRDATA_PHASEL);
		  
				  Result |= 0x8000;
				  mstACKNumNext = mstACKNumNext - 4;
				  mstStartNum  = mstStartNum -4;

		}
		else
		{
			if(mstOverFlag == TRUE)
			{
				 
				if(type == 12)
				{ 

				MsrCalAddr(mstMsrIndex.ValidNum-mstNextNum-4);

				}
				else
				{
				MsrCalAddr(mstMsrIndex.ValidNum-mstNextNum-9);
				}
				

	
			}
		else
			{

				MsrCalAddr(mstStartNum);
			}
			if(mstMsrTempAddr <= mstMsrIndex.startaddr)
			{
				if(type == 2)
				{
					Flash_Read(mstMsrTempAddr, tempbuff, mstACKNumNext*sizeof(ST_MSRDATA_PHASEL));
				}
				if(type == 3)
				 {
					Flash_Read(mstMsrTempAddr + MSR_DATA_OFFSET, tempbuff, mstACKNumNext*sizeof(ST_MSRDATA_PHASEL));
				 }
				if(type == 12)
				{
					for(i = 0; i < mstACKNumNext; i++)
					{
						Flash_Read(mstMsrTempAddr , (INT8U *)&tempbuff[2*i*(sizeof(ST_MSRDATA_PHASEL))], sizeof(ST_MSRDATA_PHASEL));
						Flash_Read(mstMsrTempAddr + MSR_DATA_OFFSET, (INT8U *)&tempbuff[2*i*(sizeof(ST_MSRDATA_PHASEL)) + sizeof(ST_MSRDATA_PHASEL)], sizeof(ST_MSRDATA_PHASEN));
						mstMsrTempAddr += sizeof(ST_MSRDATA_PHASEL);

					}
				}
			 //EEP_Read(mstMsrTempAddr,tempbuff,mstACKNum*sizeof(ST_MSRCD_DATA));
			}
			else
			{
				if(type == 2)
				{
					 Flash_Read(mstMsrTempAddr, tempbuff, (_MSR_DATAL_END -mstMsrTempAddr));
					 Flash_Read(_MSR_DATAL_ADDR,(INT8U *)&tempbuff[_MSR_DATAL_END -mstMsrTempAddr], mstACKNumNext*sizeof(ST_MSRDATA_PHASEL)-(_MSR_DATAL_END - mstMsrTempAddr));
				}
				if(type == 3)
				{
					 Flash_Read(mstMsrTempAddr + MSR_DATA_OFFSET, tempbuff, (MSR_DATAN_END -mstMsrTempAddr - MSR_DATA_OFFSET));
					 Flash_Read(MSR_DATAN_ADDR,(INT8U *)&tempbuff[MSR_DATAN_END -mstMsrTempAddr - MSR_DATA_OFFSET], mstACKNumNext*sizeof(ST_MSRDATA_PHASEL)-(MSR_DATAN_END -mstMsrTempAddr- MSR_DATA_OFFSET));
				}
				if(type == 12)
				{
					Flash_Read(mstMsrTempAddr, tempbuff, (_MSR_DATAL_END -mstMsrTempAddr));
					Flash_Read(_MSR_DATAL_ADDR,(INT8U *)&tempbuff[_MSR_DATAL_END -mstMsrTempAddr], mstACKNumNext*sizeof(ST_MSRDATA_PHASEL)-(_MSR_DATAL_END - mstMsrTempAddr));
					Flash_Read(mstMsrTempAddr + MSR_DATA_OFFSET, (INT8U *)&tempbuff[100], (MSR_DATAN_END -mstMsrTempAddr - MSR_DATA_OFFSET));
					Flash_Read(MSR_DATAN_ADDR,(INT8U *)&tempbuff[100 + MSR_DATAN_END -mstMsrTempAddr - MSR_DATA_OFFSET], mstACKNumNext*sizeof(ST_MSRDATA_PHASEN)-(MSR_DATAN_END -mstMsrTempAddr- MSR_DATA_OFFSET));
					/*这里前提是ST_MSRDATA_PHASEL == ST_MSRDATA_PHASEN*/
					LIB_MemCpy(tempbuff, convbuf, 200);
					for(i = 0; i < mstACKNumNext; i++)
					{
						LIB_MemCpy((INT8U *)&convbuf[100 + i*sizeof(ST_MSRDATA_PHASEL)], (INT8U *)&tempbuff[2*i*sizeof(ST_MSRDATA_PHASEL)+ sizeof(ST_MSRDATA_PHASEL)], sizeof(ST_MSRDATA_PHASEL));
						LIB_MemCpy((INT8U *)&convbuf[2*i*sizeof(ST_MSRDATA_PHASEL)], (INT8U *)&tempbuff[2*i*sizeof(ST_MSRDATA_PHASEL)], sizeof(ST_MSRDATA_PHASEL));

					}

				}
			 //EEP_Read(mstMsrTempAddr,tempbuff,(_MSR_DATA_END -mstMsrTempAddr));
			 //EEP_Read(_MSR_DATA_ADDR,(INT8U *)&tempbuff[_MSR_DATA_END -mstMsrTempAddr],(mstACKNum*sizeof(ST_MSRCD_DATA)) - (_MSR_DATA_END -mstMsrTempAddr));

			}
			 if(type == 12)
			{
				 LIB_MemCpy(tempbuff, pDataout, 2*mstACKNumNext*sizeof(ST_MSRDATA_PHASEL));
				 Result = 2*mstACKNumNext*sizeof(ST_MSRDATA_PHASEL);
			}
			 else
			{
				 LIB_MemCpy(tempbuff, pDataout, mstACKNumNext*sizeof(ST_MSRDATA_PHASEL));
				 Result = mstACKNumNext*sizeof(ST_MSRDATA_PHASEL);
			}

	  }
 
	}
	else if((type == 4)||(type == 5)||(type == 11))
  	{
		//todo loadondemandData();
		LIB_MemCpy((INT8U *)&mSureManagerFrame22byte[1], (INT8U *)&mstMsrData.time.min,21);

		MsrDataConv();
	   //LIB_MemCpy((INT8U *)&mSureManagerFrame22byte[1], pDataout, 21);
	   if(type == 4)
	   	{
		   LIB_MemCpy((INT8U *)&mstMsrTempL.time.min, pDataout, 22);
		   Result = 22;

	    }
   	   if(type == 5)
	   	{
		   LIB_MemCpy((INT8U *)&mstMsrTempN.time.min, pDataout, 22);
		   Result = 22;

	    }
   	   if(type == 11)
	   	{
		   LIB_MemCpy((INT8U *)&mstMsrTempL.time.min, pDataout, 22);
		   LIB_MemCpy((INT8U *)&mstMsrTempN.time.min, pDataout + 22, 22);
		   Result = 44;

	    }

	}
	else
	{

	}
  return Result;
}
INT8U MsrSetTime(INT8U *time)
{
	//+ INT8U *pData;
	INT8U Result;//Num
	INT16U AddrOffest;
	ST_6TIME_FORM BeginTime,RcdTimeStart,RcdTimesEnd;
	INT32U InMinutes,StartMinutes,EndMinutes;
	Result = TRUE;
	#if 1//+
    LIB_MemCpy(time, (INT8U *)&BeginTime.sec,6);
	EEP_Read(mstMsrIndex.startaddr, (INT8U *)&RcdTimesEnd.min, 5);
	MsrCalAddr(mstMsrIndex.ValidNum);
	EEP_Read(mstMsrTempAddr, (INT8U *)&RcdTimeStart.min, 5);
	InMinutes = LIB_MToStdDate((ST_6TIME_FORM *)&BeginTime.sec);
	StartMinutes = LIB_MToStdDate((ST_6TIME_FORM *)&RcdTimeStart.sec);
	EndMinutes = LIB_MToStdDate((ST_6TIME_FORM *)&RcdTimesEnd.sec);
	if(InMinutes > EndMinutes)
	{
		MsrClear();
	}
	else if((InMinutes < StartMinutes)||(InMinutes == StartMinutes))
	{
	}
    else
	{
	  Result = MsrFindTime((INT8U *)&InMinutes, (INT8U *)&StartMinutes, (INT8U *)&EndMinutes, (INT8U *)&AddrOffest);
      if(TRUE == Result)
      {

		mstMsrIndex.ValidNum = mstMsrIndex.ValidNum-AddrOffest;
		mstMsrIndex.startaddr = mstMsrTempAddr;
		EEP_Write(_MSR_INDEX_ADDR, (INT8U *)&mstMsrIndex.startaddr,sizeof(ST_MSRCD_INDEX));

	  }
	}
	#endif
	MsrClear();
	return Result;

}

INT8U MsrFindTime(INT8U *Intime, INT8U *Starttime, INT8U *Endtime,INT8U *outoffset)
{
	INT32U intime,starttime,endtime,temptime;//LastOneRCD
	INT8U Result,DirFlag;
	INT16U number,offset,tempnum;//startaddr endaddr
	INT16U Startpot,Endpot;
	ST_6TIME_FORM BeginTime,RcdTimeStart,RcdTimesEnd;
	Result = TRUE;
	number = mstMsrIndex.ValidNum;
	LIB_MemSet(0,&BeginTime.sec,6);
	LIB_MemSet(0,&RcdTimeStart.sec,6);
	LIB_MemSet(0,&RcdTimesEnd.sec,6);
	LIB_MemCpy(Intime, (INT8U *)&intime, 4);
	LIB_MemCpy(Starttime, (INT8U *)&starttime, 4);
	LIB_MemCpy(Endtime, (INT8U *)&endtime, 4);
	tempnum = (endtime - intime )/60;/*计算输入时间和当前最近记录点之间的个数*/
	if(tempnum > mstMsrIndex.ValidNum)
	{
	 tempnum =  mstMsrIndex.ValidNum;/*计算的个数大于当前记录的有效个数时，取当前有效个数*/
	}
	else
	{

	}
	MsrCalAddr(tempnum);/*计算出来的个数点所处地址*/
	Flash_Read(mstMsrTempAddr, (INT8U *)&BeginTime.min, 5);/*读取计算个数点的时间标签*/
	BeginTime.min = 0;
	temptime = LIB_MToStdDate((ST_6TIME_FORM *)&BeginTime.sec);
	if (temptime == intime)/*读取计算个数点的时间标签和输入时间相等，则代表所有点已包括*/
	{
	 offset = tempnum;
	 LIB_MemCpy((INT8U *)&offset, outoffset, 2);

	 return Result;
	}
	else
	{
		if (temptime > intime)/*读取的时间标签大于输入时间，则代表该点在读取范围内，要继续往更靠前找*/
		{
		   number = tempnum + (mstMsrIndex.ValidNum - tempnum)/2;/*更靠以前时间找*/
		   DirFlag = TRUE;
		   Startpot = tempnum;
		   Endpot = mstMsrIndex.ValidNum;

		}
		else
		{
		   number  = tempnum/2;/*更靠现在时间找*/
		   Startpot = 0;
		   Endpot = tempnum;
		    DirFlag = FALSE;

		}

	  while(1)
	  {
		   MsrCalAddr(number);
		   Flash_Read(mstMsrTempAddr, (INT8U *)&BeginTime.min, 5);
		   BeginTime.min = 0;
		   temptime = LIB_MToStdDate((ST_6TIME_FORM *)&BeginTime.sec);
		   //+LastOneRCD = temptime;
		   if (temptime == intime)
		   {
			   offset = number;
			   LIB_MemCpy((INT8U *)&offset, outoffset, 2);

			   return Result;
		   }
		   if (temptime > intime)
		   	{
			   Startpot = number;

			   number = Startpot  +  (Endpot - Startpot)/2;/*更靠以前时间找*/
			   
			   DirFlag = TRUE;
			}
		   else
		   	{
		   	   Endpot = number;
			   
			   number = Startpot  +  (Endpot - Startpot)/2;/*更靠当前时间找*/
			   
			   DirFlag = FALSE;
		    }
		   	
		   if((Startpot == number)||(number == Endpot))
		   	{
		       if(DirFlag == TRUE)
		       	{
				   //number++; 
			    }
			   offset = number;
			   LIB_MemCpy((INT8U *)&offset, outoffset, 2);
			   
			   return Result;

		    }

	  }
 }
 
}

void MsrCalAddr(INT16U num)
{ 
   if(num == 0)
   	{
   	   mstMsrTempAddr = mstMsrIndex.startaddr;
   	}
   else
   	{
		if(((mstMsrIndex.startaddr - _MSR_DATAL_ADDR)/ sizeof(ST_MSRDATA_PHASEL)) >= (num-1)) 
		{
			mstMsrTempAddr = mstMsrIndex.startaddr - sizeof(ST_MSRDATA_PHASEL)*(num-1);

		}
		else
		{
			mstMsrTempAddr = mstMsrIndex.startaddr + sizeof(ST_MSRDATA_PHASEL)*(_NUM_MSR_RCD - num+1);

		}
   	}
}
void MsrSaveRecord(INT8U *data)
{
  //+INT16U ADDR;
	if(mstMsrIndex.ValidNum < _NUM_MSR_RCD)
	{
		mstMsrIndex.ValidNum++;

	}
	if(((mstMsrIndex.startaddr + sizeof(ST_MSRDATA_PHASEL)) > _MSR_DATAL_END)||(mstMsrIndex.ValidNum ==1))
	{
		mstMsrIndex.startaddr = _MSR_DATAL_ADDR;
	}
	else
	{
		mstMsrIndex.startaddr += sizeof(ST_MSRDATA_PHASEL);/*注意这里是以ST_MSRDATA_PHASEL=ST_MSRDATA_PHASEN的长度计算的*/

	}
	EEP_Write(_MSR_INDEX_ADDR, (INT8U *)&mstMsrIndex.startaddr,sizeof(ST_MSRCD_INDEX));
	//Flash_Write(_MSR_INDEX_ADDR,  (INT8U *)&mstMsrIndex.startaddr, sizeof(ST_MSRCD_INDEX));
	Flash_Write(mstMsrIndex.startaddr, (INT8U *)&mstMsrTempL.time.min, sizeof(ST_MSRDATA_PHASEL));
	Flash_Write(mstMsrIndex.startaddr + MSR_DATA_OFFSET, (INT8U *)&mstMsrTempN.time.min, sizeof(ST_MSRDATA_PHASEN));/*注意这里是以ST_MSRDATA_PHASEL=ST_MSRDATA_PHASEN的长度计算的*/
	//EEP_Write(_MSR_INDEX_ADDR, (INT8U *)&mstMsrIndex.startaddr,sizeof(ST_MSRCD_INDEX));
	//EEP_Write (mstMsrIndex.startaddr,data,sizeof(ST_MSRCD_DATA));

}
//todo 
#if 0
INT16U MsrABS(INT16S data)
{
    INT16U abs;
    if(data < 0)
    {
        abs = 0 - data;
    }
    else
    {
        abs = data;
    }

    return abs;

}
#endif
