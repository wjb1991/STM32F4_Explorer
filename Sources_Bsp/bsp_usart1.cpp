/*
    ����UART_HandleTypeDef�ṹ�� ʵ�� ��������
*/

#include "bsp_usart.h"

UART_HandleTypeDef COM1;
UART_HandleTypeDef COM2;
UART_HandleTypeDef COM3;
UART_HandleTypeDef COM4;
UART_HandleTypeDef COM5;

int32_t Bsp_UsartInit(UART_HandleTypeDef* UartHandle)
{
/*
    UartHandle->Instance        = USART1;

    UartHandle->Init.BaudRate   = 9600;
    UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
    UartHandle->Init.StopBits   = UART_STOPBITS_1;
    UartHandle->Init.Parity     = UART_PARITY_ODD;
    UartHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    UartHandle->Init.Mode       = UART_MODE_TX_RX;
*/
    if(HAL_UART_Init(UartHandle) != HAL_OK)
    {
        /* Initialization Error */
        return -1;
    }

    /* ������������ж� �����ж� */  
    LL_USART_EnableIT_RXNE(UartHandle->Instance);
    LL_USART_EnableIT_ERROR(UartHandle->Instance);
 
    return 0;
}

int32_t Bsp_UsartTransStart(UART_HandleTypeDef* UartHandle, uint8_t *pTxPaload, uint16_t uiLenth)
{
    if (UartHandle->TxXferSize < uiLenth)
        return -1;
      
    /* �������ݵ��ڲ����ͻ����� */
    memcpy( UartHandle->pTxBuffPtr, pTxPaload, uiLenth);
    
    /* ���ͼ�����0 */  
    UartHandle->TxXferCount = 0;
    
    /* ���͵�һ������ */
    LL_USART_TransmitData8(UartHandle->Instance, UartHandle->pTxBuffPtr[UartHandle->TxXferCount++]); 

    /* ��������Ϊ���ж� */
    LL_USART_EnableIT_TXE(UartHandle->Instance);
    return 0;
}

/**
  * @brief  This function handles UART interrupt request.
  * @param  None
  * @retval None
  */
void USART1_IRQHandler(void)
{
    /* ��������ж� */
    if(LL_USART_IsActiveFlag_RXNE(USART1) && LL_USART_IsEnabledIT_RXNE(USART1))
    {
        if (COM1.RxXferCount < COM1.RxXferSize )
        {
            COM1.pRxBuffPtr[COM1.RxXferCount++] = LL_USART_ReceiveData8(USART1);
        }
        else
        {
            /* ���ܻ������� */
        }
    }
    
    /* ����Ϊ���ж� */
    if(LL_USART_IsEnabledIT_TXE(USART1) && LL_USART_IsActiveFlag_TXE(USART1))
    {
        if(COM1.pTxBuffPtr != NULL)
        {
            if(COM1.TxXferCount == COM1.TxXferSize)
            {
                LL_USART_DisableIT_TXE(USART1);
                COM1.TxXferCount = 0;
            }
            else
                LL_USART_TransmitData8(COM1.Instance, COM1.pTxBuffPtr[COM1.TxXferCount++]); 
        }
    }

    /* ��������ж� ����δʹ�� */
    if(LL_USART_IsEnabledIT_TC(USART1) && LL_USART_IsActiveFlag_TC(USART1))
    {
        /* �����������ж� */
        LL_USART_ClearFlag_TC(USART1);
    }

    /* �����ж� */
    if(LL_USART_IsEnabledIT_ERROR(USART1) && LL_USART_IsActiveFlag_NE(USART1))
    {
        __IO uint32_t isr_reg;

        NVIC_DisableIRQ(USART1_IRQn);

        isr_reg = LL_USART_ReadReg(USART1, ISR);
        
        /*  */
    }
}



void Bsp_DevUartOpen(Dev_SerialPort_t *port)
{
    //UART_HandleTypeDef *UartHandle = 0;
    if(memcmp(port->Name,"COM1",sizeof("COM1")) == 0)
    {
        port->UartHandle              = &COM1;
        port->UartHandle->Instance    = USART1;  
    }

    port->UartHandle->Init.BaudRate   = 9600;
    port->UartHandle->Init.WordLength = UART_WORDLENGTH_8B;
    port->UartHandle->Init.StopBits   = UART_STOPBITS_1;
    port->UartHandle->Init.Parity     = UART_PARITY_ODD;
    port->UartHandle->Init.HwFlowCtl  = UART_HWCONTROL_NONE;
    port->UartHandle->Init.Mode       = UART_MODE_TX_RX;

    Bsp_UsartInit(port->UartHandle);
}

void Bsp_DevUartClose(Dev_SerialPort_t *port)
{
    HAL_UART_DeInit(port->UartHandle);
}

int fputc(int ch, FILE *f)
{
    while (!LL_USART_IsActiveFlag_TXE(USART1)){}
    LL_USART_TransmitData8(USART1, ch); 
    return ch;
}

