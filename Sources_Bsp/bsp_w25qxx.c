//==================================================================================================
//| �ļ����� | Bsp_W25Qxx.c
//|--------- |--------------------------------------------------------------------------------------
//| �ļ����� | Bsp_W25Qxx.c W25QxxоƬ�弶����
//|--------- |--------------------------------------------------------------------------------------
//| ��Ȩ���� | 
//|----------|--------------------------------------------------------------------------------------
//|  �汾    |  ʱ��       |  ����     | ����
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | ����
//==================================================================================================
#include "Bsp_W25Qxx.h"

/* W25Qxx Ƭѡ��λ���� */
#define  BSP_W25Qxx_GPIO_PIN                      GPIO_PIN_14
#define  BSP_W25Qxx_GPIO_PORT                     GPIOB
#define  BSP_W25Qxx_GPIO_CLK_ENABLE()             __HAL_RCC_GPIOB_CLK_ENABLE(); 

void  Bsp_W25QxxCS (BOOL b_Status);

Dev_W25QXX st_W25Q128 = {
    0,
    &st_SPI1,           //�ײ�SPI���
    Bsp_W25QxxCS,       //CS�����ص�����
};

INT16U Bsp_W25QxxReadID(void* pv_Dev);

void  Bsp_W25QxxCS (BOOL b_Status)
{
    HAL_GPIO_WritePin(BSP_W25Qxx_GPIO_PORT, BSP_W25Qxx_GPIO_PIN, (GPIO_PinState)b_Status);
}

BOOL Bsp_W25QxxInit(void* pv_Dev)
{
    Dev_W25QXX* pst_Dev = pv_Dev;
    GPIO_InitTypeDef  gpio_init;
    
    BSP_W25Qxx_GPIO_CLK_ENABLE();
    
    gpio_init.Pin   = BSP_W25Qxx_GPIO_PIN;
    gpio_init.Mode  = GPIO_MODE_OUTPUT_PP;
    gpio_init.Pull  = GPIO_PULLUP;
    gpio_init.Speed = GPIO_SPEED_HIGH;
    HAL_GPIO_Init(BSP_W25Qxx_GPIO_PORT, &gpio_init);
    Bsp_W25QxxCS(1);
    
    Bsp_SpiInit(pst_Dev->pv_DevSpiHandle);
    
    Bsp_W25QxxReadID(pv_Dev);
}


//��ȡоƬID
//����ֵ����:				   
//0XEF13,��ʾоƬ�ͺ�ΪW25Q80  
//0XEF14,��ʾоƬ�ͺ�ΪW25Q16    
//0XEF15,��ʾоƬ�ͺ�ΪW25Q32  
//0XEF16,��ʾоƬ�ͺ�ΪW25Q64 
//0XEF17,��ʾоƬ�ͺ�ΪW25Q128 	  
INT16U Bsp_W25QxxReadID(void* pv_Dev)
{
    Dev_W25QXX* pst_Dev = pv_Dev;
	INT16U Temp = 0;	  
    static INT8U TxBuff[6] = {0x90,0x00,0x00,0x00,0xFF,0xFF};
    static INT8U RxBuff[6] = {0};
    pst_Dev->CS(0);	
#if 0
	Bsp_SpiTransByteBlock(&st_SPI1,0x90);//���Ͷ�ȡID����	    
	Bsp_SpiTransByteBlock(&st_SPI1,0x00);
	Bsp_SpiTransByteBlock(&st_SPI1,0x00);    
	Bsp_SpiTransByteBlock(&st_SPI1,0x00);			   
	Temp |= Bsp_SpiTransByteBlock(&st_SPI1,0xFF)<<8;  
	Temp |= Bsp_SpiTransByteBlock(&st_SPI1,0xFF);
#else  
    
    Bsp_SpiTransDMA(&st_SPI1,TxBuff,RxBuff,6);
#endif    
    while(Bsp_SpiIsBusy(&st_SPI1) == TRUE){}
    
    pst_Dev->CS(1);
    
    pst_Dev->uin_ID = Temp;
	return Temp;
} 
