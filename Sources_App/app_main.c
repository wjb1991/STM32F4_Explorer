#include  "bsp.h"

#include  <app_cfg.h>

int main(void)
{
    OS_ERR   err;

    HAL_Init();                                                 /* See Note 1.                                          */
  
    BSP_SystemClkCfg();                                         /* ��ʼ��ʱ��Ƶ�� 168Mhz                                */

    /* ����Flash�е��ж�������ram�� ����ж���ת���ٶ�? 
    for(int i = 0; i < 0x200; i++)
    {
        *((uint8_t*)(0x20000000+i)) = *((uint8_t*)(0x08000000+i));
    }
    */
    
    CPU_Init();                                                 /* ��ʼ�� uC/CPU ����                                   */

    Mem_Init();                                                 /* ��ʼ���ڴ����ģ��                                   */
    Math_Init();                                                /* ��ʼ������ģ��                                       */

    CPU_IntDis();                                               /* �ر������ж�                                         */

    OSInit(&err);                                               /* ��ʼ�� uC/OS-III.                                    */
    App_OS_SetAllHooks();

    OSTaskCreate((OS_TCB       *)&TaskStartTCB,                                 /* ����������ƿ� */
                 (CPU_CHAR     *)"Task Start",                                  /* �������� */
                 (OS_TASK_PTR   )Task_Start,                                    /* ������ */    
                 (void         *)0u,                                            /* ������� */
                 (OS_PRIO       )TASK_START_PRIO,                               /* �������ȼ� */
                 (CPU_STK      *)&TaskStartStk[0u],                             /* ������ص�ַ */    
                 (CPU_STK_SIZE  )TASK_START_STK_SIZE / 10u,                     /* ����ջ������ */        
                 (CPU_STK_SIZE  )TASK_START_STK_SIZE,                           /* �����ջ��С */ 
                 (OS_MSG_QTY    )0u,                                            /* �ڲ���Ϣ���е������Ϣ��Ŀ */
                 (OS_TICK       )0u,                                            /* ʱ��Ƭ��ѯ��ʱ��Ƭ�� */
                 (void         *)0u,                                            /* �û�����洢�� */
                 (OS_OPT        )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR| OS_OPT_TASK_SAVE_FP),
                 (OS_ERR       *)&err);                                         /* ��Ŵ���ֵ */    

    
    OSStart(&err);                                              /* Start multitasking (i.e. give control to uC/OS-III). */

    while (DEF_ON) {                                            /* Should Never Get Here.                               */
        ;
    }
}
