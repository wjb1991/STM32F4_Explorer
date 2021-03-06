//==================================================================================
//| 文件名称 | 
//|----------|----------------------------------------------------------------------
//| 文件功能 | USB4000光谱仪USB通讯任务
//|----------|----------------------------------------------------------------------
//| 输入参数 | 无
//|----------|----------------------------------------------------------------------
//| 返回参数 | 无
//==================================================================================
#include "app_cfg.h"
void Task_UsbHost (void  *p_arg)
{
    OS_ERR  os_err;
    (void)p_arg;

    CPU_IntDis();
    
    USB_HOST_Init();
    OSTaskQFlush(&TaskUsbHostTCB, &os_err);

    CPU_IntEn();

    while(TRUE)
    {
        INT16U uin_Len;
        INT8U* puch_Msg;

        puch_Msg = (INT8U*)OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
        
        USB_HOST_Process();
    }
}