//==================================================================================================
//| 文件名称 | Bsp_Spi.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Bsp_Spi.c 对应的头文件 STM32版本
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | 初版 从串口更改过来
//==================================================================================================
#ifndef __BSP_SPI_H__
#define __BSP_SPI_H__

#include "bsp.h"

typedef struct _DEV_SPI  Dev_SPI;

extern Dev_SPI st_SPI1;
extern Dev_SPI st_SPI2;
extern Dev_SPI st_SPI3;

struct _DEV_SPI{
    /*  配置参数  */
    const INT8U *       puch_Name;  
    INT32U              ul_Direction;
    INT32U              ul_DataSize;
    INT32U              ul_CLKPolarity;
    INT32U              ul_CLKPhase;
    INT32U              ul_NSS;
    INT32U              ul_BaudRate;
    INT32U              ul_FirstBit;
    INT32U              ul_Mode;
    
    /*  发送缓冲区相关设置 */
    INT8U*              puch_TxBuff;
    INT16U              uin_TxLen;
    INT16U              uin_TxCount;
    
    /* 接受缓冲区相关设置 */
    INT8U*              puch_RxBuff;
    INT16U              uin_RxLen;
    INT16U              uin_RxCount;

    /*  硬件回调函数  */
    void (*cb_SendStart)(void * pv_dev);
    void (*cb_SendComplete)(void * pv_dev);
    void (*cb_RecvReady)(void * pv_dev, INT8U* puch_Buff, INT16U uin_Len);
    void (*cb_ErrHandle)(void * pv_dev);
    
    /*  底层句柄      */
    void * pv_SpiHandle; //句柄
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
BOOL Bsp_SpiInit(Dev_SPI* pst_Dev);


//==================================================================================
//| 函数名称 | Bsp_SpiDeInit
//|----------|----------------------------------------------------------------------
//| 函数功能 | 配置Spi接口
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:设备句柄
//|----------|----------------------------------------------------------------------
//| 返回参数 | -1 关闭失败, 0 关闭成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_SpiDeInit(Dev_SPI* pst_Dev);


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
INT8U Bsp_SpiTransByteBlock(Dev_SPI* pst_Dev,INT8U uch_Byte);


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
BOOL Bsp_SpiTransBuff(Dev_SPI* pst_Dev,INT8U* uch_TxBuff,INT8U* uch_RxBuff,INT8U uch_Len);


#endif
