#include "bsp_at24c02.h"
#include "bsp_i2c.h"


uint8_t Bsp_AT24C02Read(uint8_t *pBuffer,uint16_t uin_Addr, uint16_t uin_Size)
{
    /* ���Ϳ�ʼλ */
	if (!Bsp_I2cStart(&Bsp_At24c02))                   
	    return 0;
    
    /* �����豸��ַ д���� */
    Bsp_I2cSendByte(&Bsp_At24c02,AT24C02_ADDR);
	if (!Bsp_I2cWaitAck(&Bsp_At24c02))
    {
        Bsp_I2cStop(&Bsp_At24c02); 
		return 0;
    }
    
    /* �����ڴ��ַ */
	Bsp_I2cSendByte(&Bsp_At24c02,(uint8_t)(uin_Addr));      
    Bsp_I2cWaitAck(&Bsp_At24c02);

	Bsp_I2cDelay();
	
    /* ���Ϳ�ʼλ */
    if (!Bsp_I2cStart(&Bsp_At24c02))
	    return 0;
    
    /* �����豸��ַ ������ */
	Bsp_I2cSendByte(&Bsp_At24c02,AT24C02_ADDR|0x01);   //���ö���ַ      
    Bsp_I2cWaitAck(&Bsp_At24c02);

    /* ��ȡ���� */
    while (uin_Size)
    {
        *pBuffer = Bsp_I2cReceiveByte(&Bsp_At24c02);
        if (uin_Size == 1)      
		    Bsp_I2cNoAck(&Bsp_At24c02);                //���һ���ֽڷ���NACK������ȡ
        else 
		    Bsp_I2cAck(&Bsp_At24c02);                  //����ACK�ӻ�������������
        pBuffer++;
        uin_Size--;
    }
    Bsp_I2cStop(&Bsp_At24c02);

    return 1;
}


uint8_t Bsp_AT24C02Write(uint8_t *pBuffer,uint16_t uin_Addr, uint16_t uin_Size)
{
    OS_ERR                os_err;
    
    //���ж�
	__disable_irq();
    
    /* ���Ϳ�ʼλ */
	if(!Bsp_I2cStart(&Bsp_At24c02))
	{
	    //���ж�
	    __enable_irq();
		return 0;
	}
    
    /* �����豸��ַ д���� */
    Bsp_I2cSendByte(&Bsp_At24c02,AT24C02_ADDR);        
    if(!Bsp_I2cWaitAck(&Bsp_At24c02))
	{
		Bsp_I2cStop(&Bsp_At24c02); 
		//���ж�
	    __enable_irq();
		return 0;
	}
    
    /* �����ڴ��ַ */
	Bsp_I2cSendByte(&Bsp_At24c02,(uint8_t)(uin_Addr));       
    Bsp_I2cWaitAck(&Bsp_At24c02);

    /* д������ */
    while(uin_Size--)
    {
      Bsp_I2cSendByte(&Bsp_At24c02,*pBuffer);
      Bsp_I2cWaitAck(&Bsp_At24c02);
      pBuffer++;
    }
    Bsp_I2cStop(&Bsp_At24c02);
    
	//���ж�
	__enable_irq();  

    OSTimeDlyHMSM(0u, 0u, 0u, 15u,
                  OS_OPT_TIME_HMSM_STRICT,
                  &os_err);

    return 1;   
}
//==================================================================================================
//| �������� | Bsp_AT24C02Init
//|----------|--------------------------------------------------------------------------------------
//| �������� | I2C��ʼ��
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
void Bsp_AT24C02Init(void) 
{
    Bsp_I2cInit(&Bsp_At24c02);
    Bsp_AT24C02SelfTest();
} 
//==================================================================================================
//| �������� | Bsp_AT24C02SelfTest
//|----------|--------------------------------------------------------------------------------------
//| �������� | AT24C512�Լ�
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
uint8_t Bsp_AT24C02SelfTest(void) 
{
	uint16_t i;
	uint8_t Test[10];
	uint8_t Check[10];
    
    //�Լ�EEPROM
	for(i=0; i<10;i++) 
	{
		Test[i] = (uint8_t)i+5;
	}

	Bsp_AT24C02Write(Test,0x0,8);

	Bsp_AT24C02Read(Check,0x0,8);

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
