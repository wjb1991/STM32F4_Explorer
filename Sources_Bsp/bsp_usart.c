//==================================================================================================
//| �ļ����� | Bsp_Usart.c
//|--------- |--------------------------------------------------------------------------------------
//| �ļ����� | Bsp.c �弶���ڹ��ܵ�ʵ�� STM32�汾
//|--------- |--------------------------------------------------------------------------------------
//| ��Ȩ���� | 
//|----------|--------------------------------------------------------------------------------------
//|  �汾    |  ʱ��       |  ����     | ����
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | ����
//|  V1.01   | 2018.10.31  |  wjb      | ��Ӵ��ڴ�ӡ�����ʹ��ڴ�ӡ������
//==================================================================================================
#include "bsp_usart.h"

#define     DEF_UART_CONFIG         115200,UART_WORDLENGTH_8B,UART_STOPBITS_1,UART_PARITY_NONE,UART_HWCONTROL_NONE,UART_MODE_TX_RX
#define     DEF_UART_HOOK           0,0,0,0
#define     DEF_UART_BUFF_SIZE      100



UART_HandleTypeDef stm32f7xx_usart1 = {USART1};
UART_HandleTypeDef stm32f7xx_usart2 = {USART2};
UART_HandleTypeDef stm32f7xx_usart3 = {USART3};
UART_HandleTypeDef stm32f7xx_uart4 = {UART4};

static INT8U    auch_RxBuff[4][DEF_UART_BUFF_SIZE] = {0};
static INT8U    auch_PrintfBuff[100] = {0};
/**/
Dev_SerialPort COM1 = {"COM1",                              //�˿���
                        DEF_UART_CONFIG,                    //Ĭ������
                        
                        NULL,                               //���ͻ���������
                        0,
                        0,
                        
                        &auch_RxBuff[0][0],                 //���ջ���������
                        DEF_UART_BUFF_SIZE,
                        0,                     
                        
                        DEF_UART_HOOK,                      //�ص�����
                        &stm32f7xx_usart1};                 //�ײ���

Dev_SerialPort COM2 = {"COM2",                              //�˿���
                        DEF_UART_CONFIG,                    //Ĭ������
                        
                        NULL,                               //���ͻ���������
                        0,
                        0,
                        
                        &auch_RxBuff[1][0],                 //���ջ���������
                        DEF_UART_BUFF_SIZE,
                        0,                     
                        
                        DEF_UART_HOOK,                      //�ص�����
                        &stm32f7xx_usart2};                 //�ײ���

Dev_SerialPort COM3 = {"COM3",                              //�˿���
                        DEF_UART_CONFIG,                    //Ĭ������
                        
                        NULL,                               //���ͻ���������
                        0,
                        0,
                        
                        &auch_RxBuff[2][0],                 //���ջ���������
                        DEF_UART_BUFF_SIZE,
                        0,                     
                        
                        DEF_UART_HOOK,                      //�ص�����
                        &stm32f7xx_usart3};                 //�ײ���

Dev_SerialPort COM4 = {"COM4",                              //�˿���
                        DEF_UART_CONFIG,                    //Ĭ������
                        
                        NULL,                               //���ͻ���������
                        0,
                        0,
                        
                        &auch_RxBuff[3][0],                 //���ջ���������
                        DEF_UART_BUFF_SIZE,
                        0,                     
                        
                        DEF_UART_HOOK,                      //�ص�����
                        &stm32f7xx_uart4};                  //�ײ���


void Bsp_UsartRxEnable(Dev_SerialPort* pst_Dev)
{
    UART_HandleTypeDef* UartHandle = pst_Dev->pv_UartHandle;
    /* ������������ж� */  
    LL_USART_EnableIT_RXNE(UartHandle->Instance);
}

void Bsp_UsartRxDisable(Dev_SerialPort* pst_Dev)
{
    UART_HandleTypeDef* UartHandle = pst_Dev->pv_UartHandle;
    /* �رս�������ж� */  
    LL_USART_DisableIT_RXNE(UartHandle->Instance);
}

void Bsp_UsartTxEnable(Dev_SerialPort* pst_Dev)
{
    UART_HandleTypeDef* UartHandle = pst_Dev->pv_UartHandle;
    /* �����������ж� */ 
    LL_USART_ClearFlag_TC(UartHandle->Instance);
    /* ������������ж� */ 
    LL_USART_EnableIT_TC(UartHandle->Instance);
}

void Bsp_UsartTxDisable(Dev_SerialPort* pst_Dev)
{
    UART_HandleTypeDef* UartHandle = pst_Dev->pv_UartHandle;
    /* �رշ�������ж� */  
    LL_USART_DisableIT_TC(UartHandle->Instance);
}

 __STATIC_INLINE void USARTx_IRQHandler(Dev_SerialPort* pst_Dev)
{
    USART_TypeDef *USARTx = ((UART_HandleTypeDef*)pst_Dev->pv_UartHandle)->Instance;

    /* ��������ж� */
    if(LL_USART_IsActiveFlag_RXNE(USARTx) && LL_USART_IsEnabledIT_RXNE(USARTx))
    {
        if(pst_Dev->cb_RecvReady != NULL)
        {
            uint8_t tmp = LL_USART_ReceiveData8(USARTx);
            pst_Dev->cb_RecvReady(pst_Dev, &tmp, 1);
        }
    }
    
    /* ����Ϊ���ж� */
    if(LL_USART_IsEnabledIT_TXE(USARTx) && LL_USART_IsActiveFlag_TXE(USARTx))
    {

    }

    /* ��������ж� */
    if(LL_USART_IsEnabledIT_TC(USARTx) && LL_USART_IsActiveFlag_TC(USARTx))
    {
        LL_USART_ClearFlag_TC(USARTx);
        
        if( pst_Dev->uin_TxCount < pst_Dev->uin_TxLen)
        {
            LL_USART_TransmitData8(USARTx, pst_Dev->puch_TxBuff[pst_Dev->uin_TxCount++]);
        }
        else
        {
            pst_Dev->uin_TxCount = 0;
            pst_Dev->uin_TxLen = 0;
            if(pst_Dev->cb_SendComplete != NULL)
            {
                pst_Dev->cb_SendComplete(pst_Dev);
            }        
        }
    }

    /* �����ж� */
    if(LL_USART_IsEnabledIT_ERROR(USARTx) && LL_USART_IsActiveFlag_NE(USARTx))
    {
        if(pst_Dev->cb_ErrHandle != NULL)
        {
            pst_Dev->cb_ErrHandle(pst_Dev);
        }
    }
    
    if(LL_USART_IsActiveFlag_ORE(USARTx))
    {
        LL_USART_ClearFlag_ORE(USARTx);
    }
    
    if(LL_USART_IsActiveFlag_FE(USARTx))
    {
        LL_USART_ClearFlag_FE(USARTx);
    }
}
#if 0
 __STATIC_INLINE void UARTx_IRQHandler(Dev_SerialPort* pst_Dev)
{
    USART_TypeDef *UARTx = ((UART_HandleTypeDef*)pst_Dev->pv_UartHandle)->Instance; 
    
    /* ��������ж� */
    if(LL_USART_IsActiveFlag_RXNE(UARTx) && LL_USART_IsEnabledIT_RXNE(UARTx))
    {
        if(pst_Dev->cb_RecvReady != NULL)
        {
            uint8_t tmp = LL_USART_ReceiveData8(UARTx);
            pst_Dev->cb_RecvReady(pst_Dev, &tmp, 1);
        }
    }
    
    /* ����Ϊ���ж� */
    if(LL_USART_IsEnabledIT_TXE(UARTx) && LL_USART_IsActiveFlag_TXE(UARTx))
    {

    }

    /* ��������ж� */
    if(LL_USART_IsEnabledIT_TC(UARTx) && LL_USART_IsActiveFlag_TC(UARTx))
    {
        LL_USART_ClearFlag_TC(UARTx);       
        if( pst_Dev->uin_TxCount < pst_Dev->uin_TxLen)
        {
            LL_USART_TransmitData8(UARTx, pst_Dev->puch_TxBuff[pst_Dev->uin_TxCount++]);
        }
        else
        {
            pst_Dev->uin_TxCount = 0;
            pst_Dev->uin_TxLen = 0;
            if(pst_Dev->cb_SendComplete != NULL)
            {
                pst_Dev->cb_SendComplete(pst_Dev);
            }        
        }      
    }

    /* �����ж� */
    if(LL_USART_IsEnabledIT_ERROR(UARTx) && LL_USART_IsActiveFlag_NE(UARTx))
    {
        //NVIC_DisableIRQ(UART4_IRQn);
    
        if(pst_Dev->cb_ErrHandle != NULL)
        {
            pst_Dev->cb_ErrHandle(pst_Dev);
        }
    }
    
    if(LL_USART_IsActiveFlag_ORE(UARTx))
    {
        LL_USART_ClearFlag_ORE(UARTx);
    }
    
    if(LL_USART_IsActiveFlag_FE(UARTx))
    {
        LL_USART_ClearFlag_FE(UARTx);
    }
}
#endif 

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
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    USARTx_IRQHandler(&COM1);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
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
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    USARTx_IRQHandler(&COM2);
    
#ifdef  OS_SUPPORT   
    OSIntExit();
#endif
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
#ifdef  OS_SUPPORT
    CPU_SR_ALLOC();

    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    USARTx_IRQHandler(&COM3);
    
#ifdef  OS_SUPPORT
    OSIntExit();
#endif
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
#ifdef  OS_SUPPORT
    CPU_CRITICAL_ENTER();
    OSIntEnter();
    CPU_CRITICAL_EXIT();
#endif
    
    USARTx_IRQHandler(&COM4);

#ifdef  OS_SUPPORT
    OSIntExit();
#endif
}


//==================================================================================
//| �������� | Bsp_UartOpen
//|----------|----------------------------------------------------------------------
//| �������� | ���ò��򿪴���
//|----------|----------------------------------------------------------------------
//| ������� | pst_Dev:�����豸�ṹ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | FALSE ��ʧ��, TRUE �򿪳ɹ�
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
BOOL Bsp_UartOpen(Dev_SerialPort* pst_Dev)
{
    UART_HandleTypeDef* UartHandle = pst_Dev->pv_UartHandle;
    
    UartHandle->Init.BaudRate   = pst_Dev->ul_BaudRate;
    UartHandle->Init.WordLength = pst_Dev->ul_WordLength;
    UartHandle->Init.StopBits   = pst_Dev->ul_StopBits;
    UartHandle->Init.Parity     = pst_Dev->ul_Parity;
    UartHandle->Init.HwFlowCtl  = pst_Dev->ul_HwFlowCtl;
    UartHandle->Init.Mode       = pst_Dev->ul_Mode;


    if(HAL_UART_Init(UartHandle) != HAL_OK)
    {
        /* ��ʼ������ �����Ӧ���� */
        Bsp_UartClose(pst_Dev);
        return FALSE;
    }
    
    /* ���� �����ж� */
    LL_USART_EnableIT_ERROR(UartHandle->Instance);
    
    if(UartHandle->Init.Mode  == UART_MODE_TX_RX)
    {
        Bsp_UsartTxEnable(pst_Dev);
        Bsp_UsartRxEnable(pst_Dev);
    }
    else if(UartHandle->Init.Mode  == UART_MODE_TX)
    {
        Bsp_UsartTxEnable(pst_Dev);
        Bsp_UsartRxDisable(pst_Dev);       
    }
    else if(UartHandle->Init.Mode  == UART_MODE_RX)
    {
        Bsp_UsartTxDisable(pst_Dev);
        Bsp_UsartRxEnable(pst_Dev);       
    }  
    return TRUE;
}

//==================================================================================
//| �������� | Bsp_UartClose
//|----------|----------------------------------------------------------------------
//| �������� | �رմ���
//|----------|----------------------------------------------------------------------
//| ������� | pst_Dev:�������ýṹ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | FALSE �ر�ʧ��, TRUE �رճɹ�
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
BOOL Bsp_UartClose(Dev_SerialPort* pst_Dev)
{
    HAL_UART_DeInit(pst_Dev->pv_UartHandle);
    return TRUE;
}


//==================================================================================
//| �������� | Bsp_UartSendBlock
//|----------|----------------------------------------------------------------------
//| �������� | ���ڷ���(������)
//|----------|----------------------------------------------------------------------
//| ������� | pst_Dev:���ڽṹ�� puch_Buff:���ͻ����� uin_Len:���ͻ���������
//|----------|----------------------------------------------------------------------
//| ���ز��� | FALSE:��������, TRUE:���ͳɹ�
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
BOOL Bsp_UartSendBlock(Dev_SerialPort *pst_Dev, INT8U* puch_Buff, INT16U uin_Len)
{
    INT16U i = 0;
    USART_TypeDef *USARTx = ((UART_HandleTypeDef*)pst_Dev->pv_UartHandle)->Instance;
    
    if( pst_Dev == NULL || puch_Buff == NULL || uin_Len == 0 )
        return FALSE;
    
    /* ������������� ����ʹ�������ķ��� */
    if( pst_Dev->uin_TxCount != 0 || pst_Dev->uin_TxLen != 0)
        return FALSE;

    
    /* ��Ҫ�رմ��ڷ�������ж� */
    while(uin_Len--)
    {
        while (!LL_USART_IsActiveFlag_TXE(USARTx)){}
        LL_USART_TransmitData8(USARTx, puch_Buff[i++]);  
    }
    return TRUE;
}

//==================================================================================
//| �������� | Bsp_UartSend
//|----------|----------------------------------------------------------------------
//| �������� | ���ڷ���(������)
//|----------|----------------------------------------------------------------------
//| ������� | pst_Dev:���ڽṹ�� puch_Buff:���ͻ����� uin_Len:���ͻ���������
//|----------|----------------------------------------------------------------------
//| ���ز��� | FALSE:��������, TRUE:���ͳɹ�
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
BOOL Bsp_UartSend(Dev_SerialPort *pst_Dev, INT8U* puch_Buff, INT16U uin_Len)
{
    INT16U i = 0;
    USART_TypeDef *USARTx = ((UART_HandleTypeDef*)pst_Dev->pv_UartHandle)->Instance;
    
    if( pst_Dev == NULL || puch_Buff == NULL || uin_Len == 0 )
        return FALSE;
    
    if( pst_Dev->uin_TxCount < pst_Dev->uin_TxLen)
        return FALSE;

    pst_Dev->puch_TxBuff    = puch_Buff;
    pst_Dev->uin_TxLen      = uin_Len;
    pst_Dev->uin_TxCount    = 0;
    
    while (!LL_USART_IsActiveFlag_TXE(USARTx)){}
    LL_USART_TransmitData8(USARTx, puch_Buff[pst_Dev->uin_TxCount++]);  

    return TRUE;
}

/**
  * @brief UART MSP Initialization
  *        This function configures the hardware resources used in this example:
  *           - Peripheral's clock enable
  *           - Peripheral's GPIO Configuration
  *           - Peripheral's GPIO Configuration  
  *           - NVIC configuration for UART interrupt request enable
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspInit(UART_HandleTypeDef *huart)
{
    GPIO_InitTypeDef  GPIO_InitStruct;

    if(huart->Instance == USART1)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();        
        __HAL_RCC_GPIOA_CLK_ENABLE();
          
        /* Enable USARTx clock */
        __HAL_RCC_USART1_CLK_ENABLE();       
          
        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_9;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_10;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART1;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /*##-3- Configure the NVIC for UART ########################################*/   
        /* NVIC for USARTx */
        HAL_NVIC_SetPriority(USART1_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(USART1_IRQn); 

    }
    else if(huart->Instance == USART2)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();        
        __HAL_RCC_GPIOA_CLK_ENABLE();
          
        /* Enable USARTx clock */
        __HAL_RCC_USART2_CLK_ENABLE();       
          
        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_2;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_3;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /*##-3- Configure the NVIC for UART ########################################*/   
        /* NVIC for USARTx */
        HAL_NVIC_SetPriority(USART2_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(USART2_IRQn); 

    }
    else if(huart->Instance == USART3)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOB_CLK_ENABLE();        
        __HAL_RCC_GPIOB_CLK_ENABLE();
          
        /* Enable USARTx clock */
        __HAL_RCC_USART3_CLK_ENABLE();       
          
        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_10;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_11;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART3;

        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

        /*##-3- Configure the NVIC for UART ########################################*/   
        /* NVIC for USARTx */
        HAL_NVIC_SetPriority(USART3_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(USART3_IRQn); 

    }
    else if(huart->Instance == UART4)
    {
        /*##-1- Enable peripherals and GPIO Clocks #################################*/
        /* Enable GPIO TX/RX clock */
        __HAL_RCC_GPIOA_CLK_ENABLE();        
        __HAL_RCC_GPIOA_CLK_ENABLE();
          
        /* Enable USARTx clock */
        __HAL_RCC_UART4_CLK_ENABLE();       
          
        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_0;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_1;
        GPIO_InitStruct.Alternate = GPIO_AF8_UART4;

        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /*##-3- Configure the NVIC for UART ########################################*/   
        /* NVIC for USARTx */
        HAL_NVIC_SetPriority(UART4_IRQn, 0, 1);
        HAL_NVIC_EnableIRQ(UART4_IRQn); 

    }

}

/**
  * @brief UART MSP De-Initialization
  *        This function frees the hardware resources used in this example:
  *          - Disable the Peripheral's clock
  *          - Revert GPIO and NVIC configuration to their default state
  * @param huart: UART handle pointer
  * @retval None
  */
void HAL_UART_MspDeInit(UART_HandleTypeDef *huart)
{
    if(huart->Instance == USART1)
    {
        /*##-1- Reset peripherals ##################################################*/
        __HAL_RCC_USART1_FORCE_RESET();
        __HAL_RCC_USART1_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* Configure UART Tx as alternate function  */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_9);
        /* Configure UART Rx as alternate function  */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_10);

        /*##-3- Disable the NVIC for UART ##########################################*/
        HAL_NVIC_DisableIRQ(USART1_IRQn);
    }
    else if(huart->Instance == USART2)
    {
        /*##-1- Reset peripherals ##################################################*/
        __HAL_RCC_USART2_FORCE_RESET();
        __HAL_RCC_USART2_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* Configure UART Tx as alternate function  */
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_5);
        /* Configure UART Rx as alternate function  */
        HAL_GPIO_DeInit(GPIOD, GPIO_PIN_6);

        /*##-3- Disable the NVIC for UART ##########################################*/
        HAL_NVIC_DisableIRQ(USART2_IRQn);
    }
    else if(huart->Instance == USART3)
    {
        /*##-1- Reset peripherals ##################################################*/
        __HAL_RCC_USART3_FORCE_RESET();
        __HAL_RCC_USART3_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* Configure UART Tx as alternate function  */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_10);
        /* Configure UART Rx as alternate function  */
        HAL_GPIO_DeInit(GPIOB, GPIO_PIN_11);

        /*##-3- Disable the NVIC for UART ##########################################*/
        HAL_NVIC_DisableIRQ(USART3_IRQn);
    }
    else if(huart->Instance == UART4)
    {
        /*##-1- Reset peripherals ##################################################*/
        __HAL_RCC_UART4_FORCE_RESET();
        __HAL_RCC_UART4_RELEASE_RESET();

        /*##-2- Disable peripherals and GPIO Clocks ################################*/
        /* Configure UART Tx as alternate function  */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_0);
        /* Configure UART Rx as alternate function  */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_1);

        /*##-3- Disable the NVIC for UART ##########################################*/
        HAL_NVIC_DisableIRQ(UART4_IRQn);
    }
}

//==================================================================================
//| �������� | Bsp_UartPrintf
//|----------|----------------------------------------------------------------------
//| �������� | ���ڴ�ӡ
//|----------|----------------------------------------------------------------------
//| ������� | Format: ��ӡ�ַ���
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void Bsp_UartPrintf(const char * Format,...)
{
	Dev_SerialPort* p = &COM1;      //��ӡ���ڸ��Ĵ˴�
	while(p->uin_TxLen != 0){}
	va_list pArgs;
	va_start(pArgs,Format);
	vsprintf((char *)auch_PrintfBuff,Format,pArgs);
	va_end(pArgs);

	/* scia �� 485�ӿ�
	if(p == &COM1)
		Bsp_Rs485de(eRs485Trans);*/
	Bsp_UartSendBlock(p,auch_PrintfBuff,strlen((const char*)auch_PrintfBuff));
}


int fputc(int ch, FILE *f)
{
    //while (!LL_USART_IsActiveFlag_TXE(USART1)){}
    //LL_USART_TransmitData8(USART1, ch); 
    
    //while (!LL_USART_IsActiveFlag_TC(USART2)){}
    //LL_USART_ClearFlag_TC(USART2);
    return ch;
}

