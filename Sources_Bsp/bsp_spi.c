//==================================================================================================
//| 文件名称 | Bsp_Spi.c
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Bsp_Spi.c 板级SPI驱动 STM32版本
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | 初版 从串口更改过来
//==================================================================================================
#include "bsp_spi.h"

#define     DEF_SPI_CONFIG          SPI_DIRECTION_2LINES,\
                                    SPI_DATASIZE_8BIT,\
                                    SPI_POLARITY_LOW,\
                                    SPI_PHASE_1EDGE,\
                                    SPI_NSS_SOFT,\
                                    SPI_BAUDRATEPRESCALER_4,\
                                    SPI_FIRSTBIT_MSB,\
                                    SPI_MODE_MASTER
#define     DEF_SPI_HOOK            0,0,0,0

SPI_HandleTypeDef stm32f7xx_spi1 = {SPI1};
SPI_HandleTypeDef stm32f7xx_spi2 = {SPI2};
SPI_HandleTypeDef stm32f7xx_spi3 = {SPI3};

Dev_SPI st_SPI1 = {"SPI1",                              //端口名
                    DEF_SPI_CONFIG,                      //默认配置

                    NULL,                                //发送缓冲区配置
                    0,
                    0,

                    NULL,                                //接收缓冲区配置
                    0,
                    0,                     

                    DEF_SPI_HOOK,                        //回调函数
                    &stm32f7xx_spi1,                     //底层句柄
};                    

Dev_SPI st_SPI2 = {"SPI2",                              //端口名
                    DEF_SPI_CONFIG,                      //默认配置

                    NULL,                                //发送缓冲区配置
                    0,
                    0,

                    NULL,                                //接收缓冲区配置
                    0,
                    0,                     

                    DEF_SPI_HOOK,                        //回调函数
                    &stm32f7xx_spi2,                     //底层句柄
};

Dev_SPI st_SPI3 = {"SPI3",                              //端口名
                    DEF_SPI_CONFIG,                      //默认配置

                    NULL,                                //发送缓冲区配置
                    0,
                    0,

                    NULL,                                //接收缓冲区配置
                    0,
                    0,                     

                    DEF_SPI_HOOK,                        //回调函数
                    &stm32f7xx_spi3,                     //底层句柄
};

//==================================================================================
//| 函数名称 | Bsp_SpiInit
//|----------|----------------------------------------------------------------------
//| 函数功能 | 配置Spi接口
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:设备句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | -1 打开失败, 0 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
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
    
    __HAL_SPI_ENABLE(SpiHandle);

    return TRUE;
}

//==================================================================================
//| 函数名称 | Bsp_SpiTransByte
//|----------|----------------------------------------------------------------------
//| 函数功能 | 双相四线制 Spi收发一个字节 内部是阻塞的
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:设备句柄 uch_Byte: 传输的字节 
//|----------|----------------------------------------------------------------------
//| 返回参数 | -1 打开失败, 0 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
INT8U Bsp_SpiTransByteBlock(Dev_SPI* pst_Dev,INT8U uch_Byte)
{
    SPI_HandleTypeDef* SpiHandle = pst_Dev->pv_SpiHandle;
    INT8U RecvByte = 0;
    
    while( ! __HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_TXE)){}
    SpiHandle->Instance->DR = uch_Byte;
    while( ! __HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_RXNE)){}
    while( ! __HAL_SPI_GET_FLAG(SpiHandle, SPI_FLAG_TXE)){}
    RecvByte= SpiHandle->Instance->DR;
    return RecvByte;
}

//==================================================================================
//| 函数名称 | Bsp_SpiTransByte
//|----------|----------------------------------------------------------------------
//| 函数功能 | 双相四线制 Spi收发n个字节
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:设备句柄 uch_Byte: 传输的字节 
//|----------|----------------------------------------------------------------------
//| 返回参数 | FALSE 传输失败, TRUE:传输成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_SpiTransBuff(Dev_SPI* pst_Dev,INT8U* uch_TxBuff,INT8U* uch_RxBuff,INT8U uch_Len)
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
    return TRUE;
}

//==================================================================================
//| 函数名称 | SPIx_IRQHandler
//|----------|----------------------------------------------------------------------
//| 函数功能 | SPIx中断函数处理
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:设备句柄 
//|----------|----------------------------------------------------------------------
//| 返回参数 | -1 打开失败, 0 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void SPIx_IRQHandler(Dev_SPI* pst_Dev)
{
    SPI_HandleTypeDef *SpiHandle = pst_Dev->pv_SpiHandle;
    INT32U itsource = SpiHandle->Instance->CR2;
    INT32U itflag   = SpiHandle->Instance->SR;

    /* SPI in mode Receiver ----------------------------------------------------*/
    if(((itflag & SPI_FLAG_OVR) == RESET) && ((itflag & SPI_FLAG_RXNE) != RESET) && ((itsource & SPI_IT_RXNE) != RESET))
    {
        if( pst_Dev->uin_RxCount < pst_Dev->uin_RxLen)
        {
            //继续接收
            pst_Dev->puch_RxBuff[pst_Dev->uin_RxCount++] = SpiHandle->Instance->DR;
        }
        else
        {
            //接受完成
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
            //继续发送
            SpiHandle->Instance->DR = pst_Dev->puch_TxBuff[pst_Dev->uin_TxCount++];
        }
        else
        {
            //发送完成
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
            __HAL_SPI_CLEAR_OVRFLAG(SpiHandle);
        }

        /* SPI Mode Fault error interrupt occurred -------------------------------*/
        if((itflag & SPI_FLAG_MODF) != RESET)
        {
            __HAL_SPI_CLEAR_MODFFLAG(SpiHandle);
        }

        /* SPI Frame error interrupt occurred ------------------------------------*/
        if((itflag & SPI_FLAG_FRE) != RESET)
        {
            __HAL_SPI_CLEAR_FREFLAG(SpiHandle);
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
        PB3     ------> SPI1_SCK
        PB4     ------> SPI1_MISO
        PB5     ------> SPI1_MOSI 
        */
        GPIO_InitStruct.Pin = GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5;
        GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull = GPIO_NOPULL;
        GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
        HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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
    PB3     ------> SPI1_SCK
    PB4     ------> SPI1_MISO
    PB5     ------> SPI1_MOSI 
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_3|GPIO_PIN_4|GPIO_PIN_5);

    /* USER CODE BEGIN SPI1_MspDeInit 1 */

    /* USER CODE END SPI1_MspDeInit 1 */
    }
} 

