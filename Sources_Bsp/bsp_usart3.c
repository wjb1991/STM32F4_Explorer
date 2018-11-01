/*
    ����UART_HandleTypeDef�ṹ�� ʵ�� ��������
*/

#include "bsp_usart.h"

#define DEF_UART_CONFIG 115200,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_NONE,UART_HWCONTROL_NONE,UART_MODE_TX_RX
#define DEF_UART_HOOK   0,0,0,0

UART_HandleTypeDef stm32f7xx_usart1 = {USART1};
UART_HandleTypeDef stm32f7xx_usart2 = {USART2};
UART_HandleTypeDef stm32f7xx_usart3 = {USART3};
UART_HandleTypeDef stm32f7xx_uart4 = {UART4};
/**/
Dev_SerialPort_t COM1 = {"COM1",DEF_UART_CONFIG,DEF_UART_HOOK,&stm32f7xx_usart1};
Dev_SerialPort_t COM2 = {"COM2",DEF_UART_CONFIG,DEF_UART_HOOK,&stm32f7xx_usart2};
Dev_SerialPort_t COM3 = {"COM3",DEF_UART_CONFIG,DEF_UART_HOOK,&stm32f7xx_usart3};
Dev_SerialPort_t COM4 = {"COM4",DEF_UART_CONFIG,DEF_UART_HOOK,&stm32f7xx_uart4};
//Dev_SerialPort_t COM5 = {"USART5",DEF_UART_CONFIG,DEF_UART_OPS,DEF_UART_HOOK,USART5};


void Bsp_UsartRxEnable(UART_HandleTypeDef* UartHandle)
{
    /* ������������ж� */  
    LL_USART_EnableIT_RXNE(UartHandle->Instance);
}

void Bsp_UsartRxDisable(UART_HandleTypeDef* UartHandle)
{
    /* �رս�������ж� */  
    LL_USART_DisableIT_RXNE(UartHandle->Instance);
}

void Bsp_UsartTxEnable(UART_HandleTypeDef* UartHandle)
{
    /* �����������ж� */ 
    LL_USART_ClearFlag_TC(UartHandle->Instance);
    /* ������������ж� */ 
    LL_USART_EnableIT_TC(UartHandle->Instance);
}

void Bsp_UsartTxDisable(UART_HandleTypeDef* UartHandle)
{
    /* �رշ�������ж� */  
    LL_USART_DisableIT_TC(UartHandle->Instance);
}

int32_t Bsp_UsartTransStart(UART_HandleTypeDef* UartHandle, uint8_t *pTxPayload, uint16_t uiLenth)
{
    /* ��������Ϣ��� */
    if (UartHandle->TxXferSize < uiLenth && pTxPayload != NULL)
        return -1;
    
    /* ����æ��� */
    if (UartHandle->TxXferCount != 0)
        return -2;
    
    /* �������ݵ��ڲ����ͻ����� */
    memcpy( UartHandle->pTxBuffPtr, pTxPayload, uiLenth);
    
    /* ���ͼ�����0 */  
    UartHandle->TxXferCount = 0;
    
    /* ���͵�һ������ */
    LL_USART_TransmitData8(UartHandle->Instance, UartHandle->pTxBuffPtr[UartHandle->TxXferCount++]); 

    return 0;
}

void USARTx_IRQHandler(void)
{
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
    
    /* ��������ж� */
    if(LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
    {
        if(COM1.pRecvReady != NULL)
        {
            uint8_t tmp = LL_USART_ReceiveData8(USART1);
            COM1.pRecvReady(&tmp, 1);
        }
    }
    
    /* ����Ϊ���ж� */
    if(LL_USART_IsEnabledIT_TXE(USART1) && LL_USART_IsActiveFlag_TXE(USART1))
    {

    }

    /* ��������ж� */
    if(LL_USART_IsEnabledIT_TC(USART1) && LL_USART_IsActiveFlag_TC(USART1))
    {
        LL_USART_ClearFlag_TC(USART1);
        
        if(COM1.UartHandle->TxXferCount < COM1.UartHandle->TxXferSize)
        {            
            LL_USART_TransmitData8((void*)COM1.UartHandle->Instance, 
                                   COM1.UartHandle->pTxBuffPtr[COM1.UartHandle->TxXferCount++]);  
      
        }
        else
        {
            if(COM1.pTransComplete != NULL)
            {
                COM1.pTransComplete();
            }  
        }
    }

    /* �����ж� */
    if(LL_USART_IsEnabledIT_ERROR(USART1) && LL_USART_IsActiveFlag_NE(USART1))
    {
        __IO uint32_t isr_reg;

        NVIC_DisableIRQ(USART1_IRQn);
        
        if(COM1.pErrHandle != NULL)
        {
            COM1.pErrHandle();
        }
    
        /*  */
    }
    
    if(LL_USART_IsActiveFlag_ORE(USART1))
    {
        LL_USART_ClearFlag_ORE(USART1);
    }
    
    if(LL_USART_IsActiveFlag_FE(USART1))
    {
        LL_USART_ClearFlag_FE(USART1);
    }
    
    
    
    OSIntExit();
}

//==================================================================================
//| �������� | USART1_IRQHandler
//|----------|----------------------------------------------------------------------
//| �������� | USART1�����жϴ�����
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void USART1_IRQHandler(void)
{
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();

    /* ��������ж� */
    if(LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
    {
        if(COM1.pRecvReady != NULL)
        {
            uint8_t tmp = LL_USART_ReceiveData8(USART1);
            COM1.pRecvReady(&tmp, 1);
        }
    }
    
    /* ����Ϊ���ж� */
    if(LL_USART_IsEnabledIT_TXE(USART1) && LL_USART_IsActiveFlag_TXE(USART1))
    {

    }

    /* ��������ж� */
    if(LL_USART_IsEnabledIT_TC(USART1) && LL_USART_IsActiveFlag_TC(USART1))
    {
        LL_USART_ClearFlag_TC(USART1);
        
        if(COM1.UartHandle->TxXferCount < COM1.UartHandle->TxXferSize)
        {            
            LL_USART_TransmitData8((void*)COM1.UartHandle->Instance, 
                                   COM1.UartHandle->pTxBuffPtr[COM1.UartHandle->TxXferCount++]);  
      
        }
        else
        {
            if(COM1.pTransComplete != NULL)
            {
                COM1.pTransComplete();
            }  
        }
    }

    /* �����ж� */
    if(LL_USART_IsEnabledIT_ERROR(USART1) && LL_USART_IsActiveFlag_NE(USART1))
    {
        __IO uint32_t isr_reg;

        NVIC_DisableIRQ(USART1_IRQn);
        
        if(COM1.pErrHandle != NULL)
        {
            COM1.pErrHandle();
        }
    
        /*  */
    }
    
    if(LL_USART_IsActiveFlag_ORE(USART1))
    {
        LL_USART_ClearFlag_ORE(USART1);
    }
    
    if(LL_USART_IsActiveFlag_FE(USART1))
    {
        LL_USART_ClearFlag_FE(USART1);
    }
    
    
    OSIntExit();
}

//==================================================================================
//| �������� | USART2_IRQHandler
//|----------|----------------------------------------------------------------------
//| �������� | USART2�����жϴ�����
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void USART2_IRQHandler(void)
{
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();

    /* ��������ж� */
    if(LL_USART_IsActiveFlag_RXNE(USART2) && LL_USART_IsEnabledIT_RXNE(USART2))
    {
        if(COM2.pRecvReady != NULL)
        {
            uint8_t tmp = LL_USART_ReceiveData8(USART2);
            COM2.pRecvReady(&tmp, 1);
        }
    }
    
    /* ����Ϊ���ж� */
    if(LL_USART_IsEnabledIT_TXE(USART2) && LL_USART_IsActiveFlag_TXE(USART2))
    {

    }

    /* ��������ж� */
    if(LL_USART_IsEnabledIT_TC(USART2) && LL_USART_IsActiveFlag_TC(USART2))
    {
        LL_USART_ClearFlag_TC(USART2);
        if(COM2.pTransComplete != NULL)
        {
            COM2.pTransComplete();
        }
    }

    /* �����ж� */
    if(LL_USART_IsEnabledIT_ERROR(USART2) && LL_USART_IsActiveFlag_NE(USART2))
    {
        __IO uint32_t isr_reg;

        NVIC_DisableIRQ(USART2_IRQn);
    
        if(COM2.pErrHandle != NULL)
        {
            COM2.pErrHandle();
        }
        
        /*  */
    } 
    
    if(LL_USART_IsActiveFlag_ORE(USART2))
    {
        LL_USART_ClearFlag_ORE(USART2);
    }
    
    if(LL_USART_IsActiveFlag_FE(USART2))
    {
        LL_USART_ClearFlag_FE(USART2);
    }
    
    OSIntExit();
}

//==================================================================================
//| �������� | USART3_IRQHandler
//|----------|----------------------------------------------------------------------
//| �������� | USART3�����жϴ�����
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void USART3_IRQHandler(void)
{
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();

    /* ��������ж� */
    if(LL_USART_IsActiveFlag_RXNE(USART3) && LL_USART_IsEnabledIT_RXNE(USART3))
    {
        if(COM3.pRecvReady != NULL)
        {
            uint8_t tmp = LL_USART_ReceiveData8(USART3);
            COM3.pRecvReady(&tmp, 1);
        }
    }
    
    /* ����Ϊ���ж� */
    if(LL_USART_IsEnabledIT_TXE(USART3) && LL_USART_IsActiveFlag_TXE(USART3))
    {

    }

    /* ��������ж� */
    if(LL_USART_IsEnabledIT_TC(USART3) && LL_USART_IsActiveFlag_TC(USART3))
    {
        LL_USART_ClearFlag_TC(USART3);
        if(COM3.pTransComplete != NULL)
        {
            COM3.pTransComplete();
        }
    }

    /* �����ж� */
    if(LL_USART_IsEnabledIT_ERROR(USART3) && LL_USART_IsActiveFlag_NE(USART3))
    {
        __IO uint32_t isr_reg;

        NVIC_DisableIRQ(USART3_IRQn);
    
        if(COM3.pErrHandle != NULL)
        {
            COM3.pErrHandle();
        }
        
        /*  */
    }
    
    if(LL_USART_IsActiveFlag_ORE(USART3))
    {
        LL_USART_ClearFlag_ORE(USART3);
    }
    
    if(LL_USART_IsActiveFlag_FE(USART3))
    {
        LL_USART_ClearFlag_FE(USART3);
    }
    
    OSIntExit();
}

//==================================================================================
//| �������� | UART4_IRQHandler
//|----------|----------------------------------------------------------------------
//| �������� | UART4�����жϴ�����
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void UART4_IRQHandler(void)
{
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();

    /* ��������ж� */
    if(LL_USART_IsActiveFlag_RXNE(UART4) && LL_USART_IsEnabledIT_RXNE(UART4))
    {
        if(COM4.pRecvReady != NULL)
        {
            uint8_t tmp = LL_USART_ReceiveData8(UART4);
            COM4.pRecvReady(&tmp, 1);
        }
    }
    
    /* ����Ϊ���ж� */
    if(LL_USART_IsEnabledIT_TXE(UART4) && LL_USART_IsActiveFlag_TXE(UART4))
    {

    }

    /* ��������ж� */
    if(LL_USART_IsEnabledIT_TC(UART4) && LL_USART_IsActiveFlag_TC(UART4))
    {
        LL_USART_ClearFlag_TC(UART4);
        if(COM4.pTransComplete != NULL)
        {
            COM4.pTransComplete();
        }
    }

    /* �����ж� */
    if(LL_USART_IsEnabledIT_ERROR(UART4) && LL_USART_IsActiveFlag_NE(UART4))
    {
        __IO uint32_t isr_reg;

        NVIC_DisableIRQ(UART4_IRQn);
    
        if(COM4.pErrHandle != NULL)
        {
            COM4.pErrHandle();
        }
        
        /*  */
    }
    
    if(LL_USART_IsActiveFlag_ORE(UART4))
    {
        LL_USART_ClearFlag_ORE(UART4);
    }
    
    if(LL_USART_IsActiveFlag_FE(UART4))
    {
        LL_USART_ClearFlag_FE(UART4);
    }
    
    OSIntExit();
}


//==================================================================================
//| �������� | Bsp_UartOpen
//|----------|----------------------------------------------------------------------
//| �������� | ���ò��򿪴���
//|----------|----------------------------------------------------------------------
//| ������� | pstPort  �������ýṹ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | -1 ��ʧ��, 0 �򿪳ɹ�
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
int8_t Bsp_UartOpen(Dev_SerialPort_t* pstPort)
{

    if(HAL_UART_Init(pstPort->UartHandle) != HAL_OK)
    {
        /* Initialization Error */
        return -1;
    }
    
    /* ���� �����ж� */
    LL_USART_EnableIT_ERROR(pstPort->UartHandle->Instance);
    
    if(pstPort->UartHandle->Init.Mode  == UART_MODE_TX_RX)
    {
        Bsp_UsartTxEnable(pstPort->UartHandle);
        Bsp_UsartRxEnable(pstPort->UartHandle);
    }
    else if(pstPort->UartHandle->Init.Mode  == UART_MODE_TX)
    {
        Bsp_UsartTxEnable(pstPort->UartHandle);
        Bsp_UsartRxDisable(pstPort->UartHandle);       
    }
    else if(pstPort->UartHandle->Init.Mode  == UART_MODE_RX)
    {
        Bsp_UsartTxDisable(pstPort->UartHandle);
        Bsp_UsartRxEnable(pstPort->UartHandle);       
    } 
    return 0;
}

//==================================================================================
//| �������� | Bsp_UartClose
//|----------|----------------------------------------------------------------------
//| �������� | �رմ���
//|----------|----------------------------------------------------------------------
//| ������� | pstPort  �������ýṹ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | -1 ��ʧ��, 0 �򿪳ɹ�
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
int8_t Bsp_UartClose(Dev_SerialPort_t *pstPort)
{
    HAL_UART_DeInit(pstPort->UartHandle);
    return 0;
}


//==================================================================================
//| �������� | Bsp_UartTrans
//|----------|----------------------------------------------------------------------
//| �������� | ���ڷ���(������������)
//|----------|----------------------------------------------------------------------
//| ������� | pstPort  ���ڽṹ�� pucBuff ���ͻ����� uiLenth ���ͻ���������
//|----------|----------------------------------------------------------------------
//| ���ز��� | -1 ��ʧ��, 0 �򿪳ɹ�
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
int8_t Bsp_UartTrans(Dev_SerialPort_t *pstPort, uint8_t* pucBuff, uint16_t uiLenth)
{
    uint16_t i = 0;
    
    if( pstPort == NULL || pucBuff == NULL || uiLenth == 0 )
        return -1;
    
    pstPort->UartHandle->pTxBuffPtr = pucBuff;
    pstPort->UartHandle->TxXferSize = uiLenth;
    pstPort->UartHandle->TxXferCount = 0;
    

    while (!LL_USART_IsActiveFlag_TXE((void *)pstPort->UartHandle->Instance)){}
    
    LL_USART_TransmitData8((void*)pstPort->UartHandle->Instance, 
                           pstPort->UartHandle->pTxBuffPtr[pstPort->UartHandle->TxXferCount++]);  
    return 0;
}

void Bsp_UartTransOneByte(Dev_SerialPort_t *pstPort, uint8_t uc_Data)
{
    while (!LL_USART_IsActiveFlag_TXE((void *)pstPort->UartHandle->Instance)){}
    LL_USART_TransmitData8((void*)pstPort->UartHandle->Instance, 
                           uc_Data);   
}

int fputc(int ch, FILE *f)
{
    //while (!LL_USART_IsActiveFlag_TXE(UART4)){}
    //LL_USART_TransmitData8(UART4, ch); 
    //while (!LL_USART_IsActiveFlag_TC(USART2)){}
    //LL_USART_ClearFlag_TC(USART2);
    return ch;
}

