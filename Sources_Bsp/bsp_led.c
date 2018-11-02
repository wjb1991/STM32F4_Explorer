
#include  "bsp_led.h"



/* NUCLEO-F767ZI��PB7 PB14 β��������PE12  
   ̽����STM32F4��PF9 PF10 */
#define  BSP_LED1_GPIO_PIN                      GPIO_PIN_9
#define  BSP_LED1_GPIO_PORT                     GPIOF
#define  BSP_LED1_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOF_CLK_ENABLE(); 

#define  BSP_LED2_GPIO_PIN                      GPIO_PIN_10
#define  BSP_LED2_GPIO_PORT                     GPIOF
#define  BSP_LED2_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOF_CLK_ENABLE(); 

#define  BSP_RS485DE_GPIO_PIN                   GPIO_PIN_4
#define  BSP_RS485DE_GPIO_PORT                  GPIOA
#define  BSP_RS485DE_GPIO_CLK_ENABLE()          __HAL_RCC_GPIOA_CLK_ENABLE(); 

#define  BSP_LTC1867CS0_GPIO_PIN                GPIO_PIN_9
#define  BSP_LTC1867CS0_GPIO_PORT               GPIOB
#define  BSP_LTC1867CS0_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOB_CLK_ENABLE(); 

#define  BSP_LTC1867CS1_GPIO_PIN                GPIO_PIN_0
#define  BSP_LTC1867CS1_GPIO_PORT               GPIOI
#define  BSP_LTC1867CS1_GPIO_CLK_ENABLE()       __HAL_RCC_GPIOI_CLK_ENABLE(); 

void  BSP_LedInit (void)
{
    GPIO_InitTypeDef  gpio_init;


    BSP_LED1_GPIO_CLK_ENABLE();                                 /* Enable GPIO clock for LED1(PF10)                     */
    BSP_LED2_GPIO_CLK_ENABLE();
    BSP_LTC1867CS0_GPIO_CLK_ENABLE();
    BSP_LTC1867CS1_GPIO_CLK_ENABLE();
    
                                                                /* Configure the GPIOF for LED1(PF10)                   */
    gpio_init.Pin   = BSP_LED1_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(BSP_LED1_GPIO_PORT, &gpio_init);

    gpio_init.Pin   = BSP_LED2_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(BSP_LED2_GPIO_PORT, &gpio_init); 
    
    gpio_init.Pin   = BSP_LTC1867CS0_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(BSP_LTC1867CS0_GPIO_PORT, &gpio_init); 
    
    gpio_init.Pin   = BSP_LTC1867CS1_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(BSP_LTC1867CS1_GPIO_PORT, &gpio_init); 
    
    
    BSP_Led1Set(eLedOff);
    BSP_Led2Set(eLedOff);
    Bsp_Rs485de(eRs485Recv);
    Bsp_Ltc1867CS0(1);
    Bsp_Ltc1867CS1(1);
}

void Bsp_Ltc1867CS0(BOOL status)
{
    HAL_GPIO_WritePin(BSP_LTC1867CS0_GPIO_PORT, BSP_LTC1867CS0_GPIO_PIN, (GPIO_PinState)status);
}

void Bsp_Ltc1867CS1(BOOL status)
{
    HAL_GPIO_WritePin(BSP_LTC1867CS1_GPIO_PORT, BSP_LTC1867CS1_GPIO_PIN, (GPIO_PinState)status);
}

void Bsp_Rs485de(Rs485de_t status)
{
    HAL_GPIO_WritePin(BSP_RS485DE_GPIO_PORT, BSP_RS485DE_GPIO_PIN, (GPIO_PinState)status);
}

void  BSP_Led1Set (LedStatus_t status)
{
    HAL_GPIO_WritePin(BSP_LED1_GPIO_PORT, BSP_LED1_GPIO_PIN, (GPIO_PinState)status);
}

void  BSP_Led1Toggle(void)
{
    HAL_GPIO_TogglePin(BSP_LED1_GPIO_PORT, BSP_LED1_GPIO_PIN);
}

void  BSP_Led2Set (LedStatus_t status)
{
    HAL_GPIO_WritePin(BSP_LED2_GPIO_PORT, BSP_LED2_GPIO_PIN, (GPIO_PinState)status);
}

void  BSP_Led2Toggle(void)
{
    HAL_GPIO_TogglePin(BSP_LED2_GPIO_PORT, BSP_LED2_GPIO_PIN);
}
