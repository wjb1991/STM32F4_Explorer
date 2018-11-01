#include "stm32f7xx_hal.h"
#include "stdint.h"


int main(void)
{
    GPIO_InitTypeDef    GPIO_InitStruct;
    
    /*�����༶��ˮ��? Systick��ʱ���ж� Ĭ���ж����ȼ�����4 ��ʼ��MSP*/
    HAL_Init();
    
    
    
    __HAL_RCC_GPIOB_CLK_ENABLE();
    
    /*ѡ��Ҫ���Ƶ�GPIO����*/															   
    GPIO_InitStruct.Pin = GPIO_PIN_0;	

    /*�������ŵ��������*/
    GPIO_InitStruct.Mode  = GPIO_MODE_OUTPUT_PP;  

    /*��������Ϊ����ģʽ*/
    GPIO_InitStruct.Pull  = GPIO_PULLUP;

    /*������������Ϊ���� */   
    GPIO_InitStruct.Speed = GPIO_SPEED_HIGH; 

    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
    
    while(1)
    {
        HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_0);
        for(int32_t i = 0; i < 1000000; i++)
        {
        
        }
    }
}