#include "bsp_i2c.h"

#define  BSP_EESCK_GPIO_PIN                      GPIO_PIN_8
#define  BSP_EESCK_GPIO_PORT                     GPIOB
#define  BSP_EESCK_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE(); 

#define  BSP_EESDA_GPIO_PIN                      GPIO_PIN_9
#define  BSP_EESDA_GPIO_PORT                     GPIOB
#define  BSP_EESDA_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE(); 

I2cHandle_t Bsp_At24c512;

static void Bsp_E2pomSDAH(void) 
{
    HAL_GPIO_WritePin(BSP_EESDA_GPIO_PORT, BSP_EESDA_GPIO_PIN, (GPIO_PinState)1);
}

static void Bsp_E2pomSDAL(void) 
{
    HAL_GPIO_WritePin(BSP_EESDA_GPIO_PORT, BSP_EESDA_GPIO_PIN, (GPIO_PinState)0);
}

static void Bsp_E2pomSCKH(void) 
{
    HAL_GPIO_WritePin(BSP_EESCK_GPIO_PORT, BSP_EESCK_GPIO_PIN, (GPIO_PinState)1);
}

static void Bsp_E2pomSCKL(void) 
{
    HAL_GPIO_WritePin(BSP_EESCK_GPIO_PORT, BSP_EESCK_GPIO_PIN, (GPIO_PinState)0);
}

static uint8_t Bsp_E2pomSDAValue(void) 
{
    return (uint8_t)(HAL_GPIO_ReadPin(BSP_EESDA_GPIO_PORT, BSP_EESDA_GPIO_PIN));
}


void  Bsp_I2cInit(I2cHandle_t* pst_Dev)
{
    GPIO_InitTypeDef  gpio_init;
  
    if( pst_Dev == 0)
        return;
    
    if( pst_Dev == &Bsp_At24c512 )
    {
        /* IO��ʼ�� */
        BSP_EESDA_GPIO_CLK_ENABLE();
        BSP_EESCK_GPIO_CLK_ENABLE();
        
        gpio_init.Pin   = BSP_EESCK_GPIO_PIN;
        gpio_init.Mode  = GPIO_MODE_OUTPUT_OD;
        gpio_init.Pull  = GPIO_PULLUP;
        gpio_init.Speed = GPIO_SPEED_HIGH;
        HAL_GPIO_Init(BSP_EESCK_GPIO_PORT, &gpio_init);

        gpio_init.Pin   = BSP_EESDA_GPIO_PIN;
        gpio_init.Mode  = GPIO_MODE_OUTPUT_OD;
        gpio_init.Pull  = GPIO_PULLUP;
        gpio_init.Speed = GPIO_SPEED_HIGH;
        HAL_GPIO_Init(BSP_EESDA_GPIO_PORT, &gpio_init); 
        
        /* ע��IO�������� */
        pst_Dev->SDA_H = Bsp_E2pomSDAH;
        pst_Dev->SDA_L = Bsp_E2pomSDAL;
        pst_Dev->SCK_H = Bsp_E2pomSCKH;
        pst_Dev->SCK_L = Bsp_E2pomSCKL; 
        pst_Dev->SDA_Read = Bsp_E2pomSDAValue; 
        
        Bsp_I2cStop(pst_Dev);
    }

}

//==================================================================================================
//| �������� | Bsp_I2cDelay
//|----------|--------------------------------------------------------------------------------------
//| �������� | I2C��ʱ
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
void Bsp_I2cDelay(void) 
{ 
    uint16_t i=50;  
    while (i)
    {
        i--;  
    }  
}

//==================================================================================================
//| �������� | Bsp_I2cStart
//|----------|--------------------------------------------------------------------------------------
//| �������� | I2C����
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
uint8_t Bsp_I2cStart(I2cHandle_t* pst_Dev) 
{ 
    (pst_Dev->SDA_H)(); 
    (pst_Dev->SCK_H)(); 
    
    Bsp_I2cDelay();
    if ( (pst_Dev->SDA_Read)() == 0 )
        return 0; //SDA��Ϊ�͵�ƽ������æ,�˳� 
    
    
    (pst_Dev->SDA_L)(); 
    Bsp_I2cDelay(); 
    if ( (pst_Dev->SDA_Read)() != 0 )
        return 0; //SDA��Ϊ�ߵ�ƽ�����߳���,�˳� 
    
    (pst_Dev->SCK_L)(); 
    Bsp_I2cDelay(); 
    return 1; 
} 

//==================================================================================================
//| �������� | Bsp_I2cStop
//|----------|--------------------------------------------------------------------------------------
//| �������� | I2C1ֹͣλ
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
void Bsp_I2cStop(I2cHandle_t* pst_Dev) 
{ 
    (pst_Dev->SCK_L)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_L)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SCK_H)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_H)();
    Bsp_I2cDelay(); 
} 

//==================================================================================================
//| �������� | Bsp_I2cAck
//|----------|--------------------------------------------------------------------------------------
//| �������� | I2C1Ӧ��
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
void Bsp_I2cAck(I2cHandle_t* pst_Dev) 
{ 
    (pst_Dev->SCK_L)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_L)();
    Bsp_I2cDelay(); 
    (pst_Dev->SCK_H)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SCK_L)(); 
    Bsp_I2cDelay(); 
} 

//==================================================================================================
//| �������� | Bsp_I2cNoAck
//|----------|--------------------------------------------------------------------------------------
//| �������� | I2C1��Ӧ��
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
void Bsp_I2cNoAck(I2cHandle_t* pst_Dev) 
{ 
    (pst_Dev->SCK_L)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_H)();
    Bsp_I2cDelay(); 
    (pst_Dev->SCK_H)(); 
    Bsp_I2cDelay(); 
    (pst_Dev->SCK_L)(); 
    Bsp_I2cDelay(); 
}

//==================================================================================================
//| �������� | Bsp_I2cWaitAck
//|----------|--------------------------------------------------------------------------------------
//| �������� | I2C1�ȴ�Ӧ��
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
uint8_t Bsp_I2cWaitAck(I2cHandle_t* pst_Dev)  
{ 
    (pst_Dev->SCK_L)();
    Bsp_I2cDelay(); 
    (pst_Dev->SDA_H)();
    Bsp_I2cDelay(); 
    (pst_Dev->SCK_H)();
    Bsp_I2cDelay(); 
    if ((pst_Dev->SDA_Read)())
    {
        (pst_Dev->SCK_L)();
        return 0; 
    }
    else
    {
        (pst_Dev->SCK_L)();
        return 1;     
    }
} 

//==================================================================================================
//| �������� | Bsp_I2cSendByte
//|----------|--------------------------------------------------------------------------------------
//| �������� | I2C��������
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
void Bsp_I2cSendByte(I2cHandle_t* pst_Dev,uint8_t SendByte)  
{ 
    uint8_t i = 8; 
    while (i--)
    {
        (pst_Dev->SCK_L)();
        Bsp_I2cDelay(); 
        
        if ( SendByte & 0x80 )
            (pst_Dev->SDA_H)();
        else
            (pst_Dev->SDA_L)(); 
        
        SendByte<<=1;
        
        Bsp_I2cDelay(); 
        (pst_Dev->SCK_H)();
        Bsp_I2cDelay(); 
    } 
    (pst_Dev->SCK_L)();
} 

//==================================================================================================
//| �������� | Bsp_I2cReceiveByte
//|----------|--------------------------------------------------------------------------------------
//| �������� | I2C1��������
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
uint8_t Bsp_I2cReceiveByte(I2cHandle_t* pst_Dev)  
{  
    uint8_t i=8; 
    uint8_t ReceiveByte=0; 

    (pst_Dev->SDA_H)();
    while (i--)
    {
        ReceiveByte<<=1;       
        (pst_Dev->SCK_L)(); 
        Bsp_I2cDelay(); 
        (pst_Dev->SCK_H)(); 
        Bsp_I2cDelay(); 
        if ((pst_Dev->SDA_Read)())
        {
            ReceiveByte|=0x01; 
        }
    } 
    (pst_Dev->SCK_L)(); 
    return ReceiveByte; 
} 
#if 0
//==================================================================================================
//| �������� | I2C1_WriteByte
//|----------|--------------------------------------------------------------------------------------
//| �������� | д��1�ֽ�����
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================
uint8_t I2C1_BufferWrite(I2cHandle_t* pst_Dev, uint8_t* pBuffer, uint16_t length,uint16_t WriteAddress, uint8_t DeviceAddress)
{
    //���ж�
	__disable_irq();
	
	if(!Bsp_I2cStart(pst_Dev))
	{
	    //���ж�
	    __enable_irq();
		return 0;
	}
    Bsp_I2cSendByte(pst_Dev,DeviceAddress);           //����������ַ
    if(!Bsp_I2cWaitAck(pst_Dev))
	{
		Bsp_I2cStop(pst_Dev); 
		//���ж�
	    __enable_irq();
		return 0;
	}

	Bsp_I2cSendByte(pst_Dev,(uint8_t)(WriteAddress>>8));       //������ʼ��ַ(��)
    Bsp_I2cWaitAck(pst_Dev);
	
	Bsp_I2cSendByte(pst_Dev,(uint8_t)(WriteAddress));         //������ʼ��ַ(��)
    Bsp_I2cWaitAck(pst_Dev);
	
    while(length--)
    {
      Bsp_I2cSendByte(pst_Dev,*pBuffer);
      Bsp_I2cWaitAck(pst_Dev);
      pBuffer++;
    }
    Bsp_I2cStop(pst_Dev);

	AT24C512_WRITE_PAGE_DLY();
	
	//���ж�
	__enable_irq();

    return 1;
} 

//==================================================================================================
//| �������� | I2C1_ReadByte
//|----------|--------------------------------------------------------------------------------------
//| �������� | ����1������ 
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================	
void I2C1_PageWrite(I2cHandle_t* pst_Dev,uint8_t* pBuffer,uint16_t length,uint16_t WriteAddress,uint8_t DeviceAddress)
{
	/*
    uint8_t NumOfPage = 0, NumOfSingle = 0, Addr = 0, count = 0;
    Addr  = WriteAddress % AT24C512_PGE_SZE;  //д���ַ�ǿ�ʼҳ�ĵڼ�λ
    count = AT24C512_PGE_SZE - Addr;          //�ڿ�ʼҳҪд��ĸ���
    NumOfPage   =  length / AT24C512_PGE_SZE; //Ҫд���ҳ��
    NumOfSingle =  length % AT24C512_PGE_SZE; //����һҳ�ĸ���

    if(Addr == 0)         //д���ַ��ҳ�Ŀ�ʼ
    {
      if(NumOfPage == 0)  //����С��һҳ
      {
        I2C1_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress);   //д����һҳ������
      }
      else                //���ݴ��ڵ���һҳ
      {
        while(NumOfPage)//Ҫд���ҳ��
        {
          I2C1_BufferWrite(pBuffer,AT24C512_PGE_SZE,WriteAddress,DeviceAddress);//дһҳ������
          WriteAddress +=  AT24C512_PGE_SZE;
          pBuffer      +=  AT24C512_PGE_SZE;
          NumOfPage--;
          AT24C512_WRITE_PAGE_DLY();
         }
        if(NumOfSingle!=0)//ʣ������С��һҳ
        {
          I2C1_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress); //д����һҳ������
          AT24C512_WRITE_PAGE_DLY();
        }
       }
    }
    else                  //д���ַ����ҳ�Ŀ�ʼ
    {
      if(NumOfPage== 0)   //����С��һҳ
      {
        I2C1_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress);   //д����һҳ������
      }
      else                //���ݴ��ڵ���һҳ
      {
        length       -= count;
        NumOfPage     = length / AT24C512_PGE_SZE;  //���¼���Ҫд���ҳ��
        NumOfSingle   = length % AT24C512_PGE_SZE;  //���¼��㲻��һҳ�ĸ���

        if(count != 0)
        {
          I2C1_BufferWrite(pBuffer,count,WriteAddress,DeviceAddress);      //����ʼ�Ŀռ�д��һҳ
          WriteAddress += count;
          pBuffer      += count;
         }

        while(NumOfPage--)  //Ҫд���ҳ��
        {
          I2C1_BufferWrite(pBuffer,AT24C512_PGE_SZE,WriteAddress,DeviceAddress);//дһҳ������
          WriteAddress +=  AT24C512_PGE_SZE;
          pBuffer      +=  AT24C512_PGE_SZE;
        }
        if(NumOfSingle != 0)//ʣ������С��һҳ
        {
          I2C1_BufferWrite(pBuffer,NumOfSingle,WriteAddress,DeviceAddress); //д����һҳ������
        }
      }
    }
	*/
	I2C1_BufferWrite(pst_Dev, pBuffer,length,WriteAddress,DeviceAddress);
} 

//==================================================================================================
//| �������� | I2C1_ReadByte
//|----------|--------------------------------------------------------------------------------------
//| �������� | ����1������ 
//|----------|--------------------------------------------------------------------------------------
//| ������� | �� 
//|----------|--------------------------------------------------------------------------------------       
//| ���ز��� | ��
//==================================================================================================	
uint8_t I2C1_ReadByte(I2cHandle_t* pst_Dev,uint8_t* pBuffer,uint16_t length,uint16_t ReadAddress,uint8_t DeviceAddress)
{       
    
	if (!Bsp_I2cStart(pst_Dev))
	    return 0;

    Bsp_I2cSendByte(pst_Dev,DeviceAddress);
	if (!Bsp_I2cWaitAck(pst_Dev))
    {
        Bsp_I2cStop(pst_Dev); 
		return 0;
    }
	Bsp_I2cSendByte(pst_Dev,(uint8_t)(ReadAddress>>8));   //�������ݵ�ַ(��)      
    Bsp_I2cWaitAck(pst_Dev);
	
	Bsp_I2cSendByte(pst_Dev,(uint8_t)(ReadAddress));   //�������ݵ�ַ(��)      
    Bsp_I2cWaitAck(pst_Dev);

	Bsp_I2cDelay();
	
    if (!Bsp_I2cStart(pst_Dev))
	    return 0;

	Bsp_I2cSendByte(pst_Dev,DeviceAddress|0x01);   //���ö���ַ      
    Bsp_I2cWaitAck(pst_Dev);

    while (length)
    {
        *pBuffer = Bsp_I2cReceiveByte(pst_Dev);
        if (length == 1)
		    Bsp_I2cNoAck(pst_Dev);
        else 
		    Bsp_I2cAck(pst_Dev); 
        pBuffer++;
        length--;
    }
    Bsp_I2cStop(pst_Dev);

    return 1;
}

#endif
