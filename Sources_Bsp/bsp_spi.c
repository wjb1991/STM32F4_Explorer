//==================================================================================================
//| �ļ����� | Bsp_Spi.h
//|--------- |--------------------------------------------------------------------------------------
//| �ļ����� | Bsp_Spi.c �弶SPI���� STM32�汾
//|--------- |--------------------------------------------------------------------------------------
//| ��Ȩ���� | 
//|----------|--------------------------------------------------------------------------------------
//|  �汾    |  ʱ��       |  ����     | ����
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | ���� �Ӵ��ڸ��Ĺ���
//==================================================================================================
#include "bsp_spi.h"

#define     DEF_UART_CONFIG         SPI_DIRECTION_2LINES,SPI_DATASIZE_8BIT,SPI_POLARITY_LOW,SPI_PHASE_1EDGE,\
                                    SPI_NSS_HARD_OUTPUT,SPI_BAUDRATEPRESCALER_2,SPI_FIRSTBIT_MSB,SPI_TIMODE_DISABLE\
                                    SPI_MODE_MASTER\
#define     DEF_UART_HOOK           0,0,0,0

SPI_HandleTypeDef stm32f7xx_spi1 = {SPI1};
SPI_HandleTypeDef stm32f7xx_spi2 = {SPI2};
SPI_HandleTypeDef stm32f7xx_spi3 = {SPI3};

Dev_SPI Dev_SPI1 = {"SPI1",                              //�˿���
                    DEF_SPI_CONFIG,                      //Ĭ������

                    NULL,                                //���ͻ���������
                    0,
                    0,

                    NULL,                                //���ջ���������
                    0,
                    0,                     

                    DEF_SPI_HOOK,                        //�ص�����
                    &stm32f7xx_spi1,                     //�ײ���
};                    

Dev_SPI Dev_SPI1 = {"SPI2",                              //�˿���
                    DEF_SPI_CONFIG,                      //Ĭ������

                    NULL,                                //���ͻ���������
                    0,
                    0,

                    NULL,                                //���ջ���������
                    0,
                    0,                     

                    DEF_SPI_HOOK,                        //�ص�����
                    &stm32f7xx_spi2,                     //�ײ���
};

Dev_SPI Dev_SPI1 = {"SPI3",                              //�˿���
                    DEF_SPI_CONFIG,                      //Ĭ������

                    NULL,                                //���ͻ���������
                    0,
                    0,

                    NULL,                                //���ջ���������
                    0,
                    0,                     

                    DEF_SPI_HOOK,                        //�ص�����
                    &stm32f7xx_spi3,                     //�ײ���
};

//==================================================================================
//| �������� | Bsp_SpiInit
//|----------|----------------------------------------------------------------------
//| �������� | ����Spi�ӿ�
//|----------|----------------------------------------------------------------------
//| ������� | pst_Dev:�豸���
//|----------|----------------------------------------------------------------------
//| ���ز��� | -1 ��ʧ��, 0 �򿪳ɹ�
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
BOOL Bsp_SpiInit(Dev_SPI* pst_Dev)
{
    SPI_HandleTypeDef* SpiHandle = pst_Dev->pv_SpiHandle;

    SpiHandle->Init.Mode = pst_Dev->ul_Mode;
    SpiHandle->Init.Direction = pst_Dev->ul_Direction;
    SpiHandle->Init.DataSize =  pst_Dev->ul_DataSize;
    SpiHandle->Init.CLKPolarity =  pst_Dev->ul_CLKPolarity;
    SpiHandle->Init.CLKPhase =  pst_Dev->ul_CLKPhase;
    SpiHandle->Init.NSS =  pst_Dev->ul_NSS;
    SpiHandle->Init.BaudRatePrescaler =  pst_Dev->ul_BaudRate;
    SpiHandle->Init.FirstBit =  pst_Dev->ul_FirstBit;
    SpiHandle->Init.TIMode =  SPI_TIMODE_DISABLE;
    SpiHandle->Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
    SpiHandle->Init.CRCPolynomial = 10;
    if (HAL_SPI_Init(SpiHandle) != HAL_OK)
    {
        //_Error_Handler(__FILE__, __LINE__);
    }

    return TRUE;
}

//==================================================================================
//| �������� | Bsp_SpiTransByte
//|----------|----------------------------------------------------------------------
//| �������� | ˫�������� Spi�շ�һ���ֽ� �ڲ���������
//|----------|----------------------------------------------------------------------
//| ������� | pst_Dev:�豸��� uch_Byte: ������ֽ� 
//|----------|----------------------------------------------------------------------
//| ���ز��� | -1 ��ʧ��, 0 �򿪳ɹ�
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
INT8U Bsp_SpiTransByteBlock(Dev_SPI* pst_Dev,INT8U uch_Byte)
{
    SPI_HandleTypeDef* SpiHandle = pst_Dev->pv_SpiHandle;
    INT8U RecvByte = 0;
    
    while( ! __HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_TXE)){}
    SpiHandle->Instance->DR = uch_Byte;
    while( ! __HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_RXNE)){}
    RecvByte= SpiHandle->Instance->DR;
    return RecvByte;
}

//==================================================================================
//| �������� | Bsp_SpiTransByte
//|----------|----------------------------------------------------------------------
//| �������� | ˫�������� Spi�շ�n���ֽ�
//|----------|----------------------------------------------------------------------
//| ������� | pst_Dev:�豸��� uch_Byte: ������ֽ� 
//|----------|----------------------------------------------------------------------
//| ���ز��� | -1 ��ʧ��, 0 �򿪳ɹ�
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
INT8U Bsp_SpiTransBuff(Dev_SPI* pst_Dev,INT8U* uch_TxBuff,INT8U* uch_RxBuff,INT8U uch_Len)
{
    SPI_HandleTypeDef* SpiHandle = pst_Dev->pv_SpiHandle;
    
    pst_Dev->puch_TxBuff = uch_TxBuff;
    pst_Dev->uin_TxLen = uch_Len;
    pst_Dev->uin_TxCount = 0;
    
    pst_Dev->puch_RxBuff = uch_TxBuff;
    pst_Dev->uin_RxLen = uch_Len;
    pst_Dev->uin_RxCount = 0;
    
    while( ! __HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_TXE)){}
    pst_Dev->puch_TxBuff[pst_Dev->uin_TxCount++];
}

//==================================================================================
//| �������� | SPIx_IRQHandler
//|----------|----------------------------------------------------------------------
//| �������� | SPIx�жϺ�������
//|----------|----------------------------------------------------------------------
//| ������� | pst_Dev:�豸��� 
//|----------|----------------------------------------------------------------------
//| ���ز��� | -1 ��ʧ��, 0 �򿪳ɹ�
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void SPIx_IRQHandler(Dev_SPI* pst_Dev)
{
    SPI_HandleTypeDef *hspi = pst_Dev->pv_SpiHandle;
    INT32U itsource = hspi->Instance->CR2;
    INT32U itflag   = hspi->Instance->SR;

    /* SPI in mode Receiver ----------------------------------------------------*/
    if(((itflag & SPI_FLAG_OVR) == RESET) && ((itflag & SPI_FLAG_RXNE) != RESET) && ((itsource & SPI_IT_RXNE) != RESET))
    {
        if( pst_Dev->uin_RxCount < pst_Dev->uin_RxLen)
        {
            //��������
            pst_Dev->puch_RxBuff[pst_Dev->uin_RxCount++] = SpiHandle->Instance->DR;
        }
        else
        {
            //�������
            pst_Dev->uin_RxCount = 0;
            pst_Dev->uin_RxLen = 0;
            if(pst_Dev->cb_SendComplete != NULL)
            {
                pst_Dev->cb_SendComplete(pst_Dev);
            }        
        }
        return;
    }

    /* SPI in mode Transmitter -------------------------------------------------*/
    if(((itflag & SPI_FLAG_TXE) != RESET) && ((itsource & SPI_IT_TXE) != RESET))
    {
        if( pst_Dev->uin_TxCount < pst_Dev->uin_TxLen)
        {
            //��������
            SpiHandle->Instance->DR = pst_Dev->puch_TxBuff[pst_Dev->uin_TxCount++];
        }
        else
        {
            //�������
            pst_Dev->uin_TxCount = 0;
            pst_Dev->uin_TxLen = 0;
            if(pst_Dev->cb_SendComplete != NULL)
            {
                pst_Dev->cb_SendComplete(pst_Dev);
            }        
        }
        return;
    }

    /* SPI in Error Treatment --------------------------------------------------*/
    if(((itflag & (SPI_FLAG_MODF | SPI_FLAG_OVR | SPI_FLAG_FRE)) != RESET) && ((itsource & SPI_IT_ERR) != RESET))
    {
        /* SPI Overrun error interrupt occurred ----------------------------------*/
        if((itflag & SPI_FLAG_OVR) != RESET)
        {
            __HAL_SPI_CLEAR_OVRFLAG(hspi);
        }

        /* SPI Mode Fault error interrupt occurred -------------------------------*/
        if((itflag & SPI_FLAG_MODF) != RESET)
        {
            __HAL_SPI_CLEAR_MODFFLAG(hspi);
        }

        /* SPI Frame error interrupt occurred ------------------------------------*/
        if((itflag & SPI_FLAG_FRE) != RESET)
        {
            __HAL_SPI_CLEAR_FREFLAG(hspi);
        }
        
        if(pst_Dev->cb_ErrHandle != NULL)
        {
            pst_Dev->cb_ErrHandle(pst_Dev);
        }
    }
}

void HAL_SPI_MspInit(SPI_HandleTypeDef* spiHandle)
{
    GPIO_InitTypeDef GPIO_InitStruct;
    if(spiHandle->Instance==SPI1)
    {
        /* USER CODE BEGIN SPI1_MspInit 0 */

        /* USER CODE END SPI1_MspInit 0 */
        /* SPI1 clock enable */
        __HAL_RCC_SPI1_CLK_ENABLE();

        /**SPI1 GPIO Configuration    
        PA4     ------> SPI1_NSS
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

        /* SPI1 interrupt Init */
        HAL_NVIC_SetPriority(SPI1_IRQn, 0, 0);
        HAL_NVIC_EnableIRQ(SPI1_IRQn);
        /* USER CODE BEGIN SPI1_MspInit 1 */

        /* USER CODE END SPI1_MspInit 1 */
    }
}

void HAL_SPI_MspDeInit(SPI_HandleTypeDef* spiHandle)
{
    if(spiHandle->Instance==SPI1)
    {
        /* USER CODE BEGIN SPI1_MspDeInit 0 */

        /* USER CODE END SPI1_MspDeInit 0 */
        /* Peripheral clock disable */
        __HAL_RCC_SPI1_CLK_DISABLE();

        /**SPI1 GPIO Configuration    
        PA4     ------> SPI1_NSS
        PA5     ------> SPI1_SCK
        PA6     ------> SPI1_MISO
        PA7     ------> SPI1_MOSI 
        */
        HAL_GPIO_DeInit(GPIOA, GPIO_PIN_4|GPIO_PIN_5|GPIO_PIN_6|GPIO_PIN_7);

        /* SPI1 interrupt Deinit */
        HAL_NVIC_DisableIRQ(SPI1_IRQn);
        /* USER CODE BEGIN SPI1_MspDeInit 1 */

        /* USER CODE END SPI1_MspDeInit 1 */
    }
}


