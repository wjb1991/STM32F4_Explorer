//==================================================================================
//| �ļ����� | task_gasproc
//|----------|----------------------------------------------------------------------
//| �ļ����� | �����Ƿ�����������Ũ��
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//==================================================================================
#include "app_cfg.h"
void Task_GasProc (void  *p_arg)
{
    OS_ERR  os_err;
    (void)p_arg;

    Mod_GasAnalysisInit(&GasAnalysis);
    
    OSTaskQFlush(&TaskGasProcTCB, &os_err);
    while(TRUE)
    {
        INT16U uin_Len;
        INT8U* puch_Msg;

        puch_Msg = (INT8U*)OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
        
        if(os_err != OS_ERR_NONE || puch_Msg == NULL || uin_Len == 0)
            continue;
        GasAnalysis.pst_Dev = puch_Msg;
        Mod_GasAnalysisPoll(&GasAnalysis);
    }
}