/*"***********************************************************"*/		
/*"文件名: tca.c	"*/	
/*"作者:       "*/ 
/*"版本: 1.0       日期:  "*/	
/*"文件描述: 双芯认证"*/
/*"函数列表: "*/		
/*"<函数名>	"*/	
/*"***********************************************************"*/	
#include "..\..\..\PlatForm.h"
#include "Tca.h"
#include "app\UserInterface\UI.h"
#include "App\UserInterface\ProtocolStack\host_commu\host_commu.h"
#include "APP\message\MS.h"
#include"DataCenterSoftware\Code\message\MS_OBIS.h"

INT16U tcauthtime; /*双芯认证剩余时间min 24h内没有认证则做限制*/
NO_INIT INT16U authallowkeep;	/*允许认证保持计时，插拔管理芯后，有24H的允许时间*/
TCA_AuthPara authparam;	/*双芯认证参数*/
TCA_AuthFactor authfactor;	/*当前认证因子，每次上电后取备份区的值*/
static INT8U TcaNowDay;/*"该变量用于判断是否跨天，来决定是否解锁"*/ 

INT8U TCA_Auth_Init_rx(ST_CMD03 *pPkg);

void	TCA_ReadBackupFactor(INT8U offset)
{

	offset %=3;		/*避免>=3的情况*/
	switch(offset)
	{
		case 0x0:
			FS_ReadFile(E_FILE_UI_DATA , TCA_AUTHFACTOR1_SHIFT, (INT8U *)authfactor.asBytes, sizeof(TCA_AuthFactor));
			break;
		case 0x1:
			FS_ReadFile(E_FILE_UI_DATA , TCA_AUTHFACTOR2_SHIFT, (INT8U *)authfactor.asBytes, sizeof(TCA_AuthFactor));
			break;
		case 0x2:
			FS_ReadFile(E_FILE_UI_DATA , TCA_AUTHFACTOR3_SHIFT, (INT8U *)authfactor.asBytes, sizeof(TCA_AuthFactor));
			break;
		default:
			break;
	}
}

void	TCA_WriteBackupFactor(INT8U offset,TCA_AuthFactor factor)
{

	offset %=3;		/*避免>=3的情况*/
	switch(offset)
	{
		case 0x0:
			FS_WriteFile(E_FILE_UI_DATA , TCA_AUTHFACTOR1_SHIFT, (INT8U *)factor.asBytes, sizeof(TCA_AuthFactor));
			break;
		case 0x1:
			FS_WriteFile(E_FILE_UI_DATA , TCA_AUTHFACTOR2_SHIFT, (INT8U *)factor.asBytes, sizeof(TCA_AuthFactor));
			break;
		case 0x2:
			FS_WriteFile(E_FILE_UI_DATA , TCA_AUTHFACTOR3_SHIFT, (INT8U *)factor.asBytes, sizeof(TCA_AuthFactor));
			break;
		default:
			break;
	}
}



void TCA_Init(INT8U mode)
{
	ST_6TIME_FORM pdTime;
	INT8U BackOffset,writetemp[3];
	LIB_MemSet(0x00,(INT8U*)&authparam,sizeof(TCA_AuthPara));
	if (mode == FIRST_PWR_ON)
	{
		authparam.AuthAllow =TRUE;
		authparam.LimtPermiss = FALSE;
		BackOffset = 0xFF;
		writetemp[0] = authparam.AuthAllow;
		writetemp[1] = authparam.LimtPermiss;
		writetemp[2] = BackOffset;
		FS_WriteFile(E_FILE_UI_DATA , TCA_AUTHALLOW_SHIFT, writetemp, 3);
		tcauthtime = 24*60;
		authallowkeep = 0;
	}
	else if (mode  == MCU_RUNMODE)
	{
		FS_ReadFile(E_FILE_UI_DATA , TCA_AUTHALLOW_SHIFT, writetemp, 3);
		authparam.AuthAllow = writetemp[0];
		authparam.LimtPermiss = writetemp[1];
		BackOffset = writetemp[2];
		if (BackOffset == 0xFF)
		{
			authfactor.asLong = 0x0000FFFF;
		}
		else
		{
			TCA_ReadBackupFactor(BackOffset);
		}
		if (tcauthtime > 24*60)
		{
			tcauthtime = 24*60;
		}
		GetSingle(E_PD_TIME,(INT8U *)&pdTime);	
		TcaNowDay = pdTime.day;	
		tcauthtime = 0;		/*重新上电后，直接做限制*/
	}
	else
	{

	}
		
	
}




void TCA_OnEvyMin(void)
{
	ST_7TIME_FORM  mSysTime;
	if(tcauthtime>0)
	{
		tcauthtime --;
	}
	else if(tcauthtime == 0)
	{
		if (authparam.LimtPermiss == FALSE)
		{
			authparam.LimtPermiss=TRUE;	/*计时结束后，限制*/
			Push(E_PERMISS_LIMIT,TRUE);
			FS_WriteFile(E_FILE_UI_DATA , TCA_LIMITPERMISS_SHIFT, &authparam.LimtPermiss, 1);
		}
	}
	else
	{

	}
	if (authallowkeep > 0)
	{
		authallowkeep -- ;
	}
	
	GetSingle(E_SYS_TIME,(INT8U *)&mSysTime);
	if(TcaNowDay != mSysTime.day)
        {
          	TcaNowDay = mSysTime.day;
		if (authparam.AuthAllow == FALSE)
		{
			authparam.AuthAllow = TRUE;	/*每过零点认证允许*/
			authparam.ErrCnt = 0;
			FS_WriteFile(E_FILE_UI_DATA , TCA_AUTHALLOW_SHIFT, &authparam.AuthAllow, 1);
		}	
        } 
        else
        {
        }
	#ifdef TCA_TEST		
	if (authparam.LimtPermiss == TRUE)
	{
		DIS_SetStauSeg(STA_LOCK);
		DIS_ClrStauSeg(STA_BAT);
	}
	#endif
}


TCA_AuthFactor TEST_temp_factor;

INT8U TCA_Auth(ST_CMD03 *pPkg)
{
	TCA_AuthFactor temp_factor,backup_factor[3];
	INT8U i,backcnt,offset[2],ret=RET_OTHERERR;

	LIB_MemCpy(pPkg->pData, (INT8U*) temp_factor.asBytes, 4);	
	TEST_temp_factor = temp_factor;
	
	if(authparam.AuthAllow == FALSE)
	{
		return RET_RIGHTERR;
	}
		
	if( authfactor.asLong +1 ==  temp_factor.asLong)	/*认证成功*/
	{
		authfactor.asLong = temp_factor.asLong;
		ret = RET_OK;
	}
	else 
	{
		FS_ReadFile(E_FILE_UI_DATA , TCA_FLAG_SHIFT, offset, 2);
		if (offset[0] == 1)
		{
			backcnt = 3;
			FS_ReadFile(E_FILE_UI_DATA , TCA_AUTHFACTOR1_SHIFT, backup_factor[0].asBytes, 3*sizeof(TCA_AuthFactor));
		}
		else 
		{
			backcnt = offset[1]%3 +1;
			FS_ReadFile(E_FILE_UI_DATA , TCA_AUTHFACTOR1_SHIFT, backup_factor[0].asBytes, backcnt*sizeof(TCA_AuthFactor));
		}
		for (i=0;i<backcnt;i++)
		{
			if(backup_factor[i].asLong + 1== temp_factor.asLong)	/*管理芯发送的备份加1*/
			{
				authfactor.asLong = temp_factor.asLong;
				#ifdef TCA_TEST
				/*TEST START*/
				if(i == 0)
					DIS_ClrStauSeg(STA_STEP_1);
				else if (i == 1)
					DIS_ClrStauSeg(STA_STEP_2);
				else 
					DIS_ClrStauSeg(STA_STEP_3);		
				#endif
				/*TEST END*/
				ret = RET_OK;
				break;
			}
		}
	}
	
	if(ret == RET_OK)
	{
		if (authparam.LimtPermiss == TRUE)
		{
			authparam.LimtPermiss = FALSE;
			Push(E_PERMISS_LIMIT,FALSE);
			FS_WriteFile(E_FILE_UI_DATA , TCA_LIMITPERMISS_SHIFT, & authparam.LimtPermiss, 1);
		}
		authparam.ErrCnt =0;
		tcauthtime = 24*60;
		#ifdef TCA_TEST
		tcauthtime = 2;
		DIS_SetStauSeg(STA_BAT);
		DIS_ClrStauSeg(STA_LOCK);
		#endif
	}
	else		/*认证失败*/
	{
		authparam.ErrCnt ++;		

		if (authparam.ErrCnt >=20)	/*连续认证失败超过20次*/
		{
			authparam.ErrCnt =0;		
//			if((0 == authallowkeep )&&(FALSE == ChkStatus(E_FAC)))/*在允许认证时间等于0时，禁止认证;厂内一直允许认证(?*/
			if(0 == authallowkeep )
			{
				authparam.AuthAllow = FALSE;
				authparam.LimtPermiss=TRUE;	
				Push(E_PERMISS_LIMIT,TRUE);
				FS_WriteFile(E_FILE_UI_DATA , TCA_AUTHALLOW_SHIFT, & authparam.AuthAllow, 2);
			}
			else
			{
				authparam.LimtPermiss=TRUE;	
				Push(E_PERMISS_LIMIT,TRUE);
				FS_WriteFile(E_FILE_UI_DATA , TCA_LIMITPERMISS_SHIFT, & authparam.LimtPermiss, 1);
			}	
		}
	}
	return ret;		
}




/*双芯认证初始化(明文厂内)*/
INT8U TCA_Auth_Init(ST_CMD03 *pPkg)
{
	INT8U BackOffset[2];
	authfactor.asLong =0 ;
	LIB_MemCpy(pPkg->pData, (INT8U*) authfactor.asBytes, 4);	
	authparam.ErrCnt = 0;
	authparam.AuthAllow = TRUE;
	authparam.LimtPermiss = FALSE;		/*需要整合一下赋值*/
	Push(E_PERMISS_LIMIT,FALSE);
	FS_WriteFile(E_FILE_UI_DATA , TCA_AUTHALLOW_SHIFT, & authparam.AuthAllow, 2);
	BackOffset[0] = 0;
	BackOffset[1] = 0xFF;
	FS_WriteFile(E_FILE_UI_DATA , TCA_FLAG_SHIFT, BackOffset, 2);
	tcauthtime = 24*60;
	return RET_OK;
}




INT8U TCA_Backup(ST_CMD03 *pPkg)
{
	INT8U temp,offset[2];
	TCA_AuthFactor tempfactor;
	LIB_MemCpy(pPkg->pData ,(INT8U *)tempfactor.asBytes ,sizeof(TCA_AuthFactor));
	FS_ReadFile(E_FILE_UI_DATA , TCA_FLAG_SHIFT, offset, 2);
	temp =offset[1];
	offset[1] = offset[1]+1;
	offset[1] %=3;
	TCA_WriteBackupFactor(offset[1],tempfactor);
	if((offset[0] == 0)&&(temp == 2))	
	{
		offset[0] = 1;
		FS_WriteFile(E_FILE_UI_DATA , TCA_FLAG_SHIFT, offset, 2);
	}
	else
	{
		FS_WriteFile(E_FILE_UI_DATA , TCA_BACKOFFSET_SHIFT, &offset[1], 1);
	}
				#ifdef TCA_TEST
				/*TEST START*/
				if(offset[1] == 0)
					DIS_SetStauSeg(STA_STEP_1);
				else if (offset[1] == 1)
					DIS_SetStauSeg(STA_STEP_2);
				else if(offset[1] == 2)
					DIS_SetStauSeg(STA_STEP_3);		
				else
					;
				#endif
				/*TEST END*/
	return RET_OK;
}


 INT8U TCA_AuthHandle(ST_CMD03 *pPkg)
{
	INT8U cmd,RetVal = 0xFF;
	cmd = pPkg->pData[0];
	pPkg->pData ++;
	switch(cmd)
    	{
		case TCA_CMD2_AUTH:
			RetVal = TCA_Auth(pPkg);
			break;
		case TCA_CMD2_INIT:
			RetVal = TCA_Auth_Init(pPkg);
			break;
		case TCA_CMD2_BACKUP:
			RetVal = TCA_Backup(pPkg);
			break;
		default:
			break;
    	}
	if (RetVal == RET_OK)
	{
        	pPkg->len = 9;
	}
	else
	{
		pPkg->len = 5;
	}
		
    return RetVal;
}

void GetCmpData(INT32U addr,INT16U len,INT8U *Output)
{
	 //INT8U __far * pFarAddr;
	INT8U  *pFarAddr;
	INT16U i,tmpLen;
    
	if(addr+len > _CODEEND)
    	{
		tmpLen = _CODEEND - addr + 1;
	}
	#if 0
    //else if((addr + len > _RSVSTART1) &&(addr + len < _RSVEND1))
	else if(addr + len < _RSVEND1)
	{
		tmpLen = _RSVSTART1 - addr;
	}
    //else if((addr + len > _RSVSTART2) &&(addr + len < _RSVEND2))
	else if(addr + len < _RSVEND2)
	{
		tmpLen = _RSVSTART2 - addr;
	}
	#endif
	else
	{
		tmpLen = len;
	}
    //pFarAddr = (INT8U __far *)(addr + 0x080000);
	pFarAddr = (INT8U *)(addr + 0x000000);
	if(tmpLen != len)
	{
		LIB_MemSet(0x00,Output,len);
		for(i=0;i < tmpLen;i++)
		{
			*(Output+i) = *(pFarAddr+i);
		}
		*(Output+i) = 0x80;
	}
	else
	{
		for(i=0;i<len;i++)
		{
			*(Output+i) = *(pFarAddr + i);
		}
	}
}
 
INT8U CodeCmp(const void *comString,INT8U *outString)
{
	INT8U j;
	INT16U i;
	ST_CODECMP *pstCodeCmp;

	pstCodeCmp = (ST_CODECMP *)comString;
	/*"检测比对因子起始地址的有效性"*/
	if(pstCodeCmp->FactorAddr.dword > _CODEEND)
	{
		return _CODEADDR_ERR;
	}
	else
	{
		if(pstCodeCmp->CodeAddr.dword >= _RSVSTART1)
		{
			return _CODEADDR_ERR;
		}
	}
    /*"检测比对数据起始地址的有效性"*/
	if(pstCodeCmp->CodeAddr.dword > _CODEEND)
	{
		return _CODEADDR_ERR;
	}
	else
	{
		if(pstCodeCmp->CodeAddr.dword >= _RSVSTART1)
		{
			return _CODEADDR_ERR;
		}
	}
	/*"提取比对因子，如不足16位时，进行补位操作"*/
	GetCmpData(pstCodeCmp->FactorAddr.dword,256,CARDBUFF1);
	#if 0
	/*比对因子中连续16个数据相同，返回地址错*/
   	for(i=0;i<(256-16);i++)
   	{
		if(LIB_MemCmp(CARDBUFF1+i,CARDBUFF1+1+i,15) == TRUE)
		{
			return _CODEADDR_ERR;
		}
	}
	#endif 
	for(j=1;j<4;j++)
	{
		for(i=0;i<64;i++)
		{
			CARDBUFF1[i] ^= CARDBUFF1[i+j*64];
		}
	}
	/*"提取比对数据，如不足256位时，进行补位操作"*/
	GetCmpData(pstCodeCmp->CodeAddr.dword,256,CARDBUFF2);
	for(j=1;j<4;j++)
    	{
        	for(i=0;i<64;i++)
        	{
			CARDBUFF2[i] ^= CARDBUFF2[i+j*64];
		}
	}
	for(i=0;i<64;i++)
	{
		CARDBUFF2[i] ^= CARDBUFF1[i];
	}
	LIB_Reverse(CARDBUFF2,CARDBUFF2,64);
	LIB_MemCpy(CARDBUFF2, outString, 64);
	return RET_OK;
}

INT8U FarCodeCmp(ST_CMD03 *pPkg)
{
	INT8U RetVal;

	if(pPkg->len != sizeof(ST_CODECMP) + 8)
	{
		return RET_OTHERERR;
	}
  	  else
	{
		pPkg->pData = pPkg->pData+4;
	    RetVal = CodeCmp(pPkg->pData,pPkg->DI+4);
	}

	if(RetVal == RET_OK)
	{
		pPkg->len = 64+4;/*"返回64字节的对比数据长度"*/
	}
	else
	{
		pPkg->len = 4;
	}
	return RetVal;	
}

INT8S TCA_AuthAllow_KeepS(void *pdin, void *pdout)
{
	authallowkeep = 24*60;	/*m任务S任务*/
	tcauthtime = 0;
	return	MS_SUCCESS;
}

