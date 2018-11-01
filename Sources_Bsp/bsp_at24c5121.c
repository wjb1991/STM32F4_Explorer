#include "bsp_at24c512.h"
#include "bsp_i2c.h"

#define  BSP_EEWP_GPIO_PIN                      GPIO_PIN_2
#define  BSP_EEWP_GPIO_PORT                     GPIOF
#define  BSP_EEWP_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOF_CLK_ENABLE(); 

static void Bsp_AT24C512WP_ON(void) 
{
    HAL_GPIO_WritePin(BSP_EEWP_GPIO_PORT, BSP_EEWP_GPIO_PIN, (GPIO_PinState)1);
}

static void Bsp_AT24C512WP_OFF(void) 
{
    HAL_GPIO_WritePin(BSP_EEWP_GPIO_PORT, BSP_EEWP_GPIO_PIN, (GPIO_PinState)0);
}


uint8_t Bsp_AT24C512Read(uint8_t *pBuffer,uint32_t uin_Addr, uint16_t uin_Size)
{
	if (!Bsp_I2cStart(&Bsp_At24c512))
	    return 0;

    Bsp_I2cSendByte(&Bsp_At24c512,AT24C512_I2C_ADDR);
	if (!Bsp_I2cWaitAck(&Bsp_At24c512))
    {
        Bsp_I2cStop(&Bsp_At24c512); 
		return 0;
    }
	Bsp_I2cSendByte(&Bsp_At24c512,(uint8_t)(uin_Addr>>8));   //�������ݵ�ַ(��)      
    Bsp_I2cWaitAck(&Bsp_At24c512);
	
	Bsp_I2cSendByte(&Bsp_At24c512,(uint8_t)(uin_Addr));   //�������ݵ�ַ(��)      
    Bsp_I2cWaitAck(&Bsp_At24c512);

	Bsp_I2cDelay();
	
    if (!Bsp_I2cStart(&Bsp_At24c512))
	    return 0;

	Bsp_I2cSendByte(&Bsp_At24c512,AT24C512_I2C_ADDR|0x01);   //���ö���ַ      
    Bsp_I2cWaitAck(&Bsp_At24c512);

    while (uin_Size)
    {
        *pBuffer = Bsp_I2cReceiveByte(&Bsp_At24c512);
        if (uin_Size == 1)
		    Bsp_I2cNoAck(&Bsp_At24c512);
        else 
		    Bsp_I2cAck(&Bsp_At24c512); 
        pBuffer++;
        uin_Size--;
    }
    Bsp_I2cStop(&Bsp_At24c512);

    return 1;
}


uint8_t Bsp_AT24C512Write(uint8_t *pBuffer,uint32_t uin_Addr, uint16_t uin_Size)
{
    OS_ERR                os_err;
    uint16_t            s = uin_Size;
    
    //���ж�
	__disable_irq();
	
	if(!Bsp_I2cStart(&Bsp_At24c512))
	{
	    //���ж�
	    __enable_irq();
		return 0;
	}
    Bsp_I2cSendByte(&Bsp_At24c512,AT24C512_I2C_ADDR);           //����������ַ
    if(!Bsp_I2cWaitAck(&Bsp_At24c512))
	{
		Bsp_I2cStop(&Bsp_At24c512); 
		//���ж�
	    __enable_irq();
		return 0;
	}

	Bsp_I2cSendByte(&Bsp_At24c512,(uint8_t)(uin_Addr>>8));       //������ʼ��ַ(��)
    Bsp_I2cWaitAck(&Bsp_At24c512);
	
	Bsp_I2cSendByte(&Bsp_At24c512,(uint8_t)(uin_Addr));         //������ʼ��ַ(��)
    Bsp_I2cWaitAck(&Bsp_At24c512);
	
    while(uin_Size--)
    {
      Bsp_I2cSendByte(&Bsp_At24c512,*pBuffer);
      Bsp_I2cWaitAck(&Bsp_At24c512);
      pBuffer++;
    }
    Bsp_I2cStop(&Bsp_At24c512);
    
    

    
    {
        INT32U  i;
        for(i = 0;i<100000;i++){}       //300000 ��12ms����
    } 
	//���ж�
	__enable_irq(); 
    

        
    return 1;   
}
//==================================================================================================
//| �������� | I2C_Init
//|----------|--------------------------------------------------------------------------------------
//| �������� | I2C��ʼ��
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
void AT24C512_Init(void) 
{
    GPIO_InitTypeDef  gpio_init;
  
    BSP_EEWP_GPIO_CLK_ENABLE();
    
    gpio_init.Pin   = BSP_EEWP_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(BSP_EEWP_GPIO_PORT, &gpio_init);
    
    Bsp_AT24C512WP_OFF();
    
  
    Bsp_I2cInit(&Bsp_At24c512);
    AT24C512_Test();
} 
//==================================================================================================
//| �������� | AT24C512_Test
//|----------|--------------------------------------------------------------------------------------
//| �������� | AT24C512�Լ�
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
uint8_t AT24C512_Test(void) 
{
	uint16_t i;
	uint8_t Test[10];
	uint8_t Check[10];
	
    //�Լ�EEPROM
	for(i=0; i<10;i++) 
	{
		Test[i] = (uint8_t)i;
	}

	AT24C512_WriteStr(Test,0,10);

	AT24C512_ReadStr(Check,0x10,10);

	for(i=0;i<10;i++)
	{
		if (Test[i] != Check[i]) 
		{
			break;
		}
	}
	
	if( i<10 ) 
	{
	    return 0;
	} 
	return 1;
} 
