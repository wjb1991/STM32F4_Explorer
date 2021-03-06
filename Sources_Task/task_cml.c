#include "task_cml.h"

static Dev_SerialPort *pst_Dev = &COM1;

static void Send_Complete(void* pst_Dev);


//==================================================================================================
//| 函数名称 | Task_CmlSend
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 命令行发送任务 主体函数
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Task_CmlSend (void *p_arg)
{
    OS_ERR  os_err;
    (void)p_arg;
    
    pst_Dev->cb_ErrHandle = NULL;
    pst_Dev->cb_SendComplete = Send_Complete;
    
    OSTaskQFlush(&TaskCmlSendTCB, &os_err);
    OSTaskSemSet(&TaskCmlSendTCB,0,&os_err);
    
    while(1)
    {
        INT16U uin_Len;
        INT8U* puch_Msg;
        
        puch_Msg = (INT8U*)OSTaskQPend(0,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
        
        if(os_err != OS_ERR_NONE)
            continue;
        /** mark*/
        Bsp_UartSend(pst_Dev, puch_Msg, uin_Len);
        
        OSTaskSemPend(0,OS_OPT_PEND_BLOCKING,NULL,&os_err);
        
        //BSP_Led2Toggle();
    }
}

//==================================================================================================
//| 函数名称 | Send_Complete
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 串口发送完成中断
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
static void Send_Complete(void* pst_Dev)
{
    OS_ERR  os_err;
    
    OSTaskSemPost(&TaskCmlSendTCB, OS_OPT_POST_NONE, &os_err);     
}

//==================================================================================================
//| 函数名称 | App_CmlSendMsg
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 发送一条信息
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Task_CmlSendMsg(INT8U* puch_Msg, INT16U uin_Len)
{
    OS_ERR  os_err;

    OSTaskQPost(&TaskCmlSendTCB,puch_Msg,uin_Len,OS_OPT_POST_FIFO,&os_err); 
}

//==================================================================================================
//| 函数名称 | Task_CmlRecv
//|----------|--------------------------------------------------------------------------------------
//| 函数功能 | 开始任务
//|----------|--------------------------------------------------------------------------------------
//| 输入参数 | 无 
//|----------|--------------------------------------------------------------------------------------       
//| 返回参数 | 无
//|----------|-------------------------------------------------------------------------------------- 
//| 函数设计 | wjb
//==================================================================================================
void Task_CmlRecv (void *p_arg)
{
    OS_ERR  os_err;
    (void)p_arg;

    while(1)
    {

    }
}