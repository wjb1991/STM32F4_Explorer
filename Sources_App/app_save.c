#include "app_save.h"

#define N_MAX_SAVE   100   //最大变量个数
//---------------------------------变量地址---------------------------------//
static const MYSAVEPARA st_Save[N_MAX_SAVE]=
{
//存储地址|-----------------变量地址---------------|-----类型------|-长度-|-----限定方-----|---最小值--|---最大值---|--默认值---//
  
/*  0    3670个点的背景光谱  4096*4个点的空间预留  */  
START_ADD+0x0000,         (uint32_t)alf_BkSpectrum, KIND_FP64,    3648,    LIMIT_RANGE,      0,        65536,        0,

/*  4096    20个校准点数据 每个20字节 400字节  512个字节空间   */ 
START_ADD+0x8000+sizeof(CaliPoint_t)*0, (uint32_t)(&ast_CPortBuff[0]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*1, (uint32_t)(&ast_CPortBuff[1]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*2, (uint32_t)(&ast_CPortBuff[2]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*3, (uint32_t)(&ast_CPortBuff[3]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,
START_ADD+0x8000+sizeof(CaliPoint_t)*4, (uint32_t)(&ast_CPortBuff[4]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*5, (uint32_t)(&ast_CPortBuff[5]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*6, (uint32_t)(&ast_CPortBuff[6]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*7, (uint32_t)(&ast_CPortBuff[7]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*8, (uint32_t)(&ast_CPortBuff[8]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*9, (uint32_t)(&ast_CPortBuff[9]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*10, (uint32_t)(&ast_CPortBuff[10]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*11, (uint32_t)(&ast_CPortBuff[11]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*12, (uint32_t)(&ast_CPortBuff[12]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*13, (uint32_t)(&ast_CPortBuff[13]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*14, (uint32_t)(&ast_CPortBuff[14]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*15, (uint32_t)(&ast_CPortBuff[15]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*16, (uint32_t)(&ast_CPortBuff[16]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*17, (uint32_t)(&ast_CPortBuff[17]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
START_ADD+0x8000+sizeof(CaliPoint_t)*18, (uint32_t)(&ast_CPortBuff[18]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,
START_ADD+0x8000+sizeof(CaliPoint_t)*19, (uint32_t)(&ast_CPortBuff[19]),KIND_INT8U, sizeof(CaliPoint_t), LIMIT_NULL, 0, 255, 0,    
   
//存储地址|-----------------变量地址---------------|-----类型------|-长度-|-----限定方-----|---最小值--|---最大值---|--默认值---//
/*  4096 + 512  128个字节预留  */
START_ADD+0x8200 + 0x00, (uint32_t)(&alf_WorkLine[0][0]), KIND_FP32,   1,     LIMIT_NULL,       0,          99,        0,
START_ADD+0x8200 + 0x04, (uint32_t)(&alf_WorkLine[0][1]), KIND_FP32,   1,     LIMIT_NULL,       0,          99,        0,
START_ADD+0x8200 + 0x08, (uint32_t)(&alf_WorkLine[0][2]), KIND_FP32,   1,     LIMIT_NULL,       0,          99,        0,
START_ADD+0x8200 + 0x0c, (uint32_t)(&alf_WorkLine[0][3]), KIND_FP32,   1,     LIMIT_NULL,       0,          99,        0,
                                                                      
START_ADD+0x8200 + 0x10, (uint32_t)(&alf_WorkLine[1][0]), KIND_FP32,   1,     LIMIT_NULL,       0,          99,        0,
START_ADD+0x8200 + 0x14, (uint32_t)(&alf_WorkLine[1][1]), KIND_FP32,   1,     LIMIT_NULL,       0,          99,        0,
START_ADD+0x8200 + 0x18, (uint32_t)(&alf_WorkLine[1][2]), KIND_FP32,   1,     LIMIT_NULL,       0,          99,        0,
START_ADD+0x8200 + 0x1c, (uint32_t)(&alf_WorkLine[1][3]), KIND_FP32,   1,     LIMIT_NULL,       0,          99,        0,
                                                                      
START_ADD+0x8200 + 0x20, (uint32_t)(&alf_WorkLine[2][0]), KIND_FP32,   1,     LIMIT_NULL,       0,          99,        0,
START_ADD+0x8200 + 0x24, (uint32_t)(&alf_WorkLine[2][1]), KIND_FP32,   1,     LIMIT_NULL,       0,          99,        0,
START_ADD+0x8200 + 0x28, (uint32_t)(&alf_WorkLine[2][2]), KIND_FP32,   1,     LIMIT_NULL,       0,          99,        0,
START_ADD+0x8200 + 0x2c, (uint32_t)(&alf_WorkLine[2][3]), KIND_FP32,   1,     LIMIT_NULL,       0,          99,        0,
            

//存储地址|-----------------变量地址---------------|-----类型------|-长度-|-----限定方-----|---最小值--|---最大值---|--默认值---//
/*  4096 + 512  128个字节预留  */
START_ADD+0x9000 + 0x00, (uint32_t)(&USB4000.uch_ScansToAverage),   KIND_INT8U,   1,     LIMIT_RANGE,       1,          99,        10,
START_ADD+0x9000 + 0x04, (uint32_t)(&USB4000.uch_Boxcar),           KIND_INT8U,   1,     LIMIT_RANGE,       1,          99,        32,
START_ADD+0x9000 + 0x08, (uint32_t)(&USB4000.b_EdcEnable),          KIND_INT8U,   1,     LIMIT_RANGE,       0,          1,          1,
START_ADD+0x9000 + 0x0C, (uint32_t)(&USB4000.b_NlcEnable),          KIND_INT8U,   1,     LIMIT_RANGE,       0,          1,          0,
START_ADD+0x9000 + 0x10, (uint32_t)(&USB4000.ul_SetIntegralTime),   KIND_INT32U,  1,     LIMIT_RANGE,       1,      65000000,    100000,

}; 
 
//==================================================================================================
//| 函数名称 | InitParaFromEeprom()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 从Eeprom初始化参数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | uch_InitFlag: 初始化方式
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
void InitParaFromEeprom(INT8U uch_InitFlag)
{
	INT16U i,k;
	INT8U  uch_Temp=0; 
	INT16U uin_Temp;
	INT32U ul_Temp;
	FP32   f_Temp;
    FP64   lf_Temp;
	INT32U ul_SaveAddr;
	MYSAVEPARA *p;
	
	//找出对应地址
	for(i=0; i<N_MAX_SAVE; i++)
	{
	    p = (MYSAVEPARA*)&st_Save[i];
	    //存储地址为空则结束初始化
	    if(st_Save[i].ul_SaveAddr == 0)
	    {
	        break;
	    }
	    
	    //存储变量地址为空时不进行操作
	    if(st_Save[i].ul_VariableAddr == 0 || p->uin_Len > N_MAX_EEPROM_ARR)//不支持长度大于
	    {
	        continue;
	    }
	    
	    //进行赋值合法性并存储到存储单元
    	ul_SaveAddr = p->ul_SaveAddr;
    	switch(p->uch_TypeFlag)
    	{
    	  //INT8U类型
    	  case KIND_BOOL:
    	  case KIND_INT8U:
    	  case KIND_INT8S:
    	    for(k=0; k < p->uin_Len; k++)
        	{
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        uch_Temp = (INT8U)p->f_Default;
        	        WriteCharToEeprom(ul_SaveAddr+k*MW1,uch_Temp);
        	    }
        	    else     
        	    {
        	        uch_Temp = ReadCharFromEeprom(ul_SaveAddr+k*MW1);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(uch_Temp >= (INT8U)p->f_Min && uch_Temp <= (INT8U)p->f_Max))
        	        {
        	            uch_Temp = (INT8U)p->f_Default;
        	            WriteCharToEeprom(ul_SaveAddr+k*MW1,uch_Temp);
        	        } 
        	    }
        	    *(INT8U*)(p->ul_VariableAddr+k*MW1) = uch_Temp;
    	    }
    	    break;
    	    
    	    
    	  //INT16U类型
    	  case KIND_INT16U:
    	  case KIND_INT16S:
    	    for(k=0; k < p->uin_Len; k++)
        	{
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        uin_Temp = (INT16U)p->f_Default;
        	        WriteIntToEeprom(ul_SaveAddr+k*MW2,uin_Temp);
        	    }
        	    else
        	    {
        	        uin_Temp = ReadIntFromEeprom(ul_SaveAddr+k*MW2);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(uin_Temp >= (INT16U)p->f_Min && uin_Temp <= (INT16U)p->f_Max))
        	        {
        	            uin_Temp = (INT16U)p->f_Default;
        	            WriteIntToEeprom(ul_SaveAddr+k*MW2,uin_Temp);
        	        }
        	    }
        	    *(INT16U*)(p->ul_VariableAddr+k*MW2) = uin_Temp;
        	}
    	    break;
    	  
    	
    	  //INT32U类型
    	  case KIND_INT32U:
    	  case KIND_INT32S:
    	    for(k=0; k < p->uin_Len; k++)
            {
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        ul_Temp = (INT32U)p->f_Default;
        	        WriteLongToEeprom(ul_SaveAddr+k*MW4,uch_Temp);
        	    }
        	    else
        	    {
        	        ul_Temp = ReadLongFromEeprom(ul_SaveAddr+k*MW4);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(ul_Temp >= (INT32U)p->f_Min && ul_Temp <= (INT32U)p->f_Max))
        	        {
        	            ul_Temp = (INT32U)p->f_Default;
        	            WriteLongToEeprom(ul_SaveAddr+k*MW4,ul_Temp);
        	        }
        	    }
        	    *(INT32U*)(p->ul_VariableAddr+k*MW4) = ul_Temp;
            }
    	    break;
    	  
    	    
    	  //FP32类型
    	  case KIND_FP32:
    	    for(k=0; k < p->uin_Len; k++)
            {
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        f_Temp = (FP32)p->f_Default;
        	        WriteFloatToEeprom(ul_SaveAddr+k*MW4,f_Temp);
        	    }
        	    else
        	    {
        	        f_Temp = ReadFloatFromEeprom(ul_SaveAddr+k*MW4);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(f_Temp >= (FP32)p->f_Min && f_Temp <= (FP32)p->f_Max))
        	        {
        	            f_Temp = (FP32)p->f_Default;
        	            WriteFloatToEeprom(ul_SaveAddr+k*MW4,f_Temp);
        	        }
        	    }
        	    *(FP32*)(p->ul_VariableAddr+k*MW4) = f_Temp;
            }
    	    break;
    	  case KIND_FP64:
    	    for(k=0; k < p->uin_Len; k++)
            {
        	    if(uch_InitFlag == INIT_DEFAULT)
        	    {
        	        lf_Temp = (FP64)p->f_Default;
        	        WriteFloatToEeprom(ul_SaveAddr+k*MW8,lf_Temp);
        	    }
        	    else
        	    {
        	        lf_Temp = ReadFloatFromEeprom(ul_SaveAddr+k*MW8);
        	        if(p->uch_LimitFlag == LIMIT_RANGE && !(lf_Temp >= (FP64)p->f_Min && lf_Temp <= (FP64)p->f_Max))
        	        {
        	            lf_Temp = (FP64)p->f_Default;
        	            WriteFloatToEeprom(ul_SaveAddr+k*MW8,lf_Temp);
        	        }
        	    }
        	    *(FP64*)(p->ul_VariableAddr+k*MW8) = lf_Temp;
            }
    	    break;
    	  default:
    	    break;
    	}
	}	
}
//==================================================================================================
//| 函数名称 | SaveToEeprom()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 数据存储函数(单变量类型)(适用于数据存储宽度为8位的单片机比如DT512/DG128)
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | ul_VariableAddr: 要存储变量的首地址
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | 
//==================================================================================================
BOOL SaveToEeprom(INT32U ul_VariableAddr)
{
    INT16U i;
	INT8U  uch_Temp;
	INT16U uin_Temp;
	INT32U ul_Temp;
	FP32   f_Temp;
    FP64   lf_Temp;
	INT32U ul_SaveAddr;
	MYSAVEPARA *p=0;
	
	//找出对应地址
	for(i=0; i<N_MAX_SAVE; i++)
	{
	    if(st_Save[i].ul_VariableAddr == ul_VariableAddr)
	    {
	        p = (MYSAVEPARA*)&st_Save[i];
	        break;
	    }
	    else if(st_Save[i].ul_SaveAddr == 0)
	    {
	        return(FALSE);
	    }
	}
	
	//进行赋值合法性并存储到存储单元
	ul_SaveAddr = p->ul_SaveAddr;
	switch(p->uch_TypeFlag)
	{
	  //INT8U类型
	  case KIND_BOOL:
	  case KIND_INT8U:
	  case KIND_INT8S:
	    uch_Temp = *(INT8U*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(uch_Temp >= (INT8U)p->f_Min && uch_Temp <= (INT8U)p->f_Max)
	        {
	            WriteCharToEeprom(ul_SaveAddr,uch_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {
	         WriteCharToEeprom(ul_SaveAddr,uch_Temp);
	    }
	    break;
	    
	  //INT16U类型
	  case KIND_INT16U:
	  case KIND_INT16S:
	    uin_Temp = *(INT16U*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(uin_Temp >= (INT16U)p->f_Min && uin_Temp <= (INT16U)p->f_Max)
	        {
	            WriteIntToEeprom(ul_SaveAddr,uin_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {					 
	         WriteIntToEeprom(ul_SaveAddr,uin_Temp);
	    }
	    break;
	
	  //INT32U类型
	  case KIND_INT32U:
	  case KIND_INT32S:
	    ul_Temp = *(INT32U*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(ul_Temp >= (INT32U)p->f_Min && ul_Temp <= (INT32U)p->f_Max)
	        {
	            WriteLongToEeprom(ul_SaveAddr,ul_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {
	         WriteLongToEeprom(ul_SaveAddr,ul_Temp);
	    }
	    break;
	    
	  //FP32类型
	  case KIND_FP32:
	    f_Temp = *(FP32*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(f_Temp >= (FP32)p->f_Min && f_Temp <= (FP32)p->f_Max)
	        {
	            WriteFloatToEeprom(ul_SaveAddr,f_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {
	         WriteFloatToEeprom(ul_SaveAddr,f_Temp);
	    }
	    break;
	  case KIND_FP64:
	    lf_Temp = *(FP64*)ul_VariableAddr;
	    if(p->uch_LimitFlag == LIMIT_RANGE)
	    {
	        if(lf_Temp >= (FP64)p->f_Min && lf_Temp <= (FP64)p->f_Max)
	        {
	            WriteDoubleToEeprom(ul_SaveAddr,lf_Temp);
	        }
	        else
	        {
	            return(FALSE);
	        }
	    }
	    else
	    {
	         WriteDoubleToEeprom(ul_SaveAddr,lf_Temp);
	    }
	    break;
	  default:
	    break;
	}
	return(TRUE);
}



//==================================================================================================
//| 函数名称 | SaveToEeprom()
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 数据存储函数(数据组类型)
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | ul_VariableAddr: 要存储变量的首地址
//|          | uin_Len: 数据长度
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 |  
//==================================================================================================
BOOL SaveToEepromExt(INT32U ul_VariableAddr,INT16U uin_Len)
{
	INT16U i,k;
	INT8U  uch_Temp;
	INT16U uin_Temp;
	INT32U ul_Temp;
	FP32   f_Temp;
	INT32U ul_SaveAddr;
	MYSAVEPARA *p=0;
	
	//找出对应地址
	for(i=0; i<N_MAX_SAVE; i++)
	{
	    if(st_Save[i].ul_VariableAddr == ul_VariableAddr)
	    {
	        p = (MYSAVEPARA*)&st_Save[i];
	        break;
	    }
	    else if(st_Save[i].ul_SaveAddr == 0)
	    {
	        return(FALSE);
	    }
	}
	
	//长度合法性判定
	if(uin_Len > p->uin_Len || uin_Len > N_MAX_EEPROM_ARR)
	{
	    return(FALSE);
	}
	
	
	//进行赋值合法性并存储到存储单元
	ul_SaveAddr = p->ul_SaveAddr;
	switch(p->uch_TypeFlag)
	{
	  //INT8U类型
	  case KIND_BOOL:
	  case KIND_INT8U:
	  case KIND_INT8S:
	    for(k=0; k < uin_Len; k++)
	    {
            uch_Temp = *(INT8U*)(ul_VariableAddr+k*MW1);
            if(p->uch_LimitFlag == LIMIT_RANGE)
            {
                if(uch_Temp >= (INT8U)p->f_Min && uch_Temp <= (INT8U)p->f_Max)
                {
                    WriteCharToEeprom(ul_SaveAddr+k*MW1,uch_Temp);
                }
                else
                {
                    return(FALSE);
                }
            }
            else
            {
                 WriteCharToEeprom(ul_SaveAddr+k*MW1,uch_Temp);
            }
	    }
	    break;
	    
	  //INT16U类型
	  case KIND_INT16U:
	  case KIND_INT16S:
	    for(k=0; k < uin_Len; k++)
	    {
    	    uin_Temp = *(INT16U*)(ul_VariableAddr+k*MW2);
    	    if(p->uch_LimitFlag == LIMIT_RANGE)
    	    {
    	        if(uin_Temp >= (INT16U)p->f_Min && uin_Temp <= (INT16U)p->f_Max)
    	        {
    	            WriteIntToEeprom(ul_SaveAddr+k*MW2,uin_Temp);
    	        }
    	        else
    	        {
    	            return(FALSE);
    	        }
    	    }
    	    else
    	    {
    	         WriteIntToEeprom(ul_SaveAddr+k*MW2,uin_Temp);
    	    }
	    }
	    break;
	
	  //INT32U类型
	  case KIND_INT32U:
	  case KIND_INT32S:
	    for(k=0; k < p->uin_Len; k++)
	    {
    	    ul_Temp = *(INT32U*)(ul_VariableAddr+k*MW4);
    	    if(p->uch_LimitFlag == LIMIT_RANGE)
    	    {
    	        if(ul_Temp >= (INT32U)p->f_Min && ul_Temp <= (INT32U)p->f_Max)
    	        {
    	            WriteLongToEeprom(ul_SaveAddr+k*MW4,ul_Temp);
    	        }
    	        else
    	        {
    	            return(FALSE);
    	        }
    	    }
    	    else
    	    {
    	         WriteLongToEeprom(ul_SaveAddr+k*MW4,ul_Temp);
    	    }
	    }
	    break;
	    
	  //FP32类型
	  case KIND_FP32:
	    for(k=0; k < p->uin_Len; k++)
	    {
    	    f_Temp = *(FP32*)(ul_VariableAddr+k*MW4);
    	    if(p->uch_LimitFlag == LIMIT_RANGE)
    	    {
    	        if(f_Temp >= (FP32)p->f_Min && f_Temp <= (FP32)p->f_Max)
    	        {
    	            WriteFloatToEeprom(ul_SaveAddr+k*MW4,f_Temp);
    	        }
    	        else
    	        {
    	            return(FALSE);
    	        }
    	    }
    	    else
    	    {
    	         WriteFloatToEeprom(ul_SaveAddr+k*MW4,f_Temp);
    	    }
	    }
	    break;
	  case KIND_FP64:
	    for(k=0; k < p->uin_Len; k++)
	    {
    	    f_Temp = *(FP64*)(ul_VariableAddr+k*MW8);
    	    if(p->uch_LimitFlag == LIMIT_RANGE)
    	    {
    	        if(f_Temp >= (FP64)p->f_Min && f_Temp <= (FP64)p->f_Max)
    	        {
    	            WriteDoubleToEeprom(ul_SaveAddr+k*MW8,f_Temp);
    	        }
    	        else
    	        {
    	            return(FALSE);
    	        }
    	    }
    	    else
    	    {
    	         WriteDoubleToEeprom(ul_SaveAddr+k*MW8,f_Temp);
    	    }
	    }
	    break;
	    
	  default:
	    break;
	}
	return(TRUE);	
}
//------------------------------------------- Eeprom(写)-------------------------------------//
void WriteCharToEeprom(INT32U uin_Addr,INT8U uch_Data)
{
	Bsp_At24c512Write((INT8U*)&uch_Data,uin_Addr,1);
}   
void WriteIntToEeprom(INT32U uin_Addr,INT16U uin_Data)
{
	Bsp_At24c512Write((INT8U*)&uin_Data,uin_Addr,2);
}   
void WriteLongToEeprom(INT32U uin_Addr,INT32U ul_Data)
{
	Bsp_At24c512Write((INT8U*)&ul_Data,uin_Addr,4);
} 
void WriteFloatToEeprom(INT32U uin_Addr,FP32 f_Data)
{
	Bsp_At24c512Write((INT8U*)&f_Data,uin_Addr,4);
}
void WriteDoubleToEeprom(INT32U uin_Addr,FP64 f_Data)
{
	Bsp_At24c512Write((INT8U*)&f_Data,uin_Addr,8);
}

//------------------------------------------- Eeprom(读)-------------------------------------//
INT8U ReadCharFromEeprom(INT32U uin_Addr)
{
    INT8U auch_Data[4]={0};   
    Bsp_At24c512Read(auch_Data,uin_Addr,1);
    return(*(INT8U*)(auch_Data));
} 
INT16U ReadIntFromEeprom(INT32U uin_Addr)
{
    INT8U auch_Data[4]={0};
    Bsp_At24c512Read(auch_Data,uin_Addr,2);
    return(*(INT16U*)auch_Data);
} 
INT32U ReadLongFromEeprom(INT32U uin_Addr)
{
    INT8U auch_Data[4]={0};
    Bsp_At24c512Read(auch_Data,uin_Addr,4);
    return(*(INT32U*)auch_Data);
}   
FP32 ReadFloatFromEeprom(INT32U uin_Addr)
{
    INT8U auch_Data[4]={0};
    Bsp_At24c512Read(auch_Data,uin_Addr,4);
    return(*(FP32*)auch_Data);
}
FP64 ReadDouleFromEeprom(INT32U uin_Addr)
{
    INT8U auch_Data[8]={0};
    Bsp_At24c512Read(auch_Data,uin_Addr,8);
    return(*(FP64*)auch_Data);
}
