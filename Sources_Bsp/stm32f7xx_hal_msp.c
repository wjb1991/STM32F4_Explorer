/**
  ******************************************************************************
  * @file    Templates/Src/stm32f7xx_hal_msp.c
  * @author  MCD Application Team
  * @brief   HAL MSP module.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx_hal.h"

/** @addtogroup STM32F7xx_HAL_Examples
  * @{
  */

/** @addtogroup Templates
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

/**
  * @brief  Initializes the Global MSP.
  * @param  None
  * @retval None
  */
void HAL_MspInit(void)
{
}

/**
  * @brief  DeInitializes the Global MSP.
  * @param  None  
  * @retval None
  */
void HAL_MspDeInit(void)
{
}

/**
  * @brief  Initializes the PPP MSP.
  * @param  None
  * @retval None
  */
/*void HAL_PPP_MspInit(void)
{*/
/*}*/

/**
  * @brief  DeInitializes the PPP MSP.
  * @param  None  
  * @retval None
  */
/*void HAL_PPP_MspDeInit(void)
{*/
/*}*/


/** @defgroup HAL_MSP_Private_Functions
  * @{
  */

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
        __HAL_RCC_GPIOD_CLK_ENABLE();        
        __HAL_RCC_GPIOD_CLK_ENABLE();
          
        /* Enable USARTx clock */
        __HAL_RCC_USART2_CLK_ENABLE();       
          
        /*##-2- Configure peripheral GPIO ##########################################*/
        /* UART TX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_5;
        GPIO_InitStruct.Mode      = GPIO_MODE_AF_PP;
        GPIO_InitStruct.Pull      = GPIO_PULLUP;
        GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

        /* UART RX GPIO pin configuration  */
        GPIO_InitStruct.Pin       = GPIO_PIN_6;
        GPIO_InitStruct.Alternate = GPIO_AF7_USART2;

        HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

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



/**
  * @}
  */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
