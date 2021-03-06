//==================================================================================================
//| 文件名称 | Bsp_Usart.h
//|--------- |--------------------------------------------------------------------------------------
//| 文件描述 | Bsp_Usart.c 对应的头文件 STM32版本
//|--------- |--------------------------------------------------------------------------------------
//| 版权声明 | 
//|----------|--------------------------------------------------------------------------------------
//|  版本    |  时间       |  作者     | 描述
//|--------- |-------------|-----------|------------------------------------------------------------
//|  V1.0    | 2018.10.31  |  wjb      | 初版
//|  V1.01   | 2018.10.31  |  wjb      | 添加串口打印函数和串口打印缓冲区
//==================================================================================================
#ifndef __BSP_USART_H__
#define __BSP_USART_H__

#include "bsp.h"

typedef struct _DEV_SERIALPORT  Dev_SerialPort;

extern Dev_SerialPort COM1;
extern Dev_SerialPort COM2;
extern Dev_SerialPort COM3;
extern Dev_SerialPort COM4;

struct _DEV_SERIALPORT{
    /*  配置参数  */
    const INT8U *       puch_Name;    
    INT32U              ul_BaudRate;
    INT32U              ul_WordLength;
    INT32U              ul_StopBits;
    INT32U              ul_Parity;
    INT32U              ul_HwFlowCtl;
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
    void * pv_UartHandle; //句柄
};


//==================================================================================
//| 函数名称 | Bsp_UartOpen
//|----------|----------------------------------------------------------------------
//| 函数功能 | 配置并打开串口
//|----------|----------------------------------------------------------------------
//| 输入参数 | pstPort  串口配置结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | -1 打开失败, 0 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_UartOpen(Dev_SerialPort* pst_Dev);


//==================================================================================
//| 函数名称 | Bsp_UartClose
//|----------|----------------------------------------------------------------------
//| 函数功能 | 关闭串口
//|----------|----------------------------------------------------------------------
//| 输入参数 | pstPort  串口配置结构体
//|----------|----------------------------------------------------------------------
//| 返回参数 | -1 打开失败, 0 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_UartClose(Dev_SerialPort* pst_Dev);


//==================================================================================
//| 函数名称 | Bsp_UartSendBlock
//|----------|----------------------------------------------------------------------
//| 函数功能 | 串口发送(阻塞的)
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:串口控制句柄 puch_Buff: 发送缓冲区 uin_Len:发送缓冲区长度
//|----------|----------------------------------------------------------------------
//| 返回参数 | -1 打开失败, 0 打开成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_UartSendBlock(Dev_SerialPort *pst_Dev, INT8U* puch_Buff, INT16U uin_Len);


//==================================================================================
//| 函数名称 | Bsp_UartSend
//|----------|----------------------------------------------------------------------
//| 函数功能 | 串口发送(非阻塞)
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:串口结构体 puch_Buff:发送缓冲区 uin_Len:发送缓冲区长度
//|----------|----------------------------------------------------------------------
//| 返回参数 | FALSE:参数有误, TRUE:发送成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
BOOL Bsp_UartSend(Dev_SerialPort *pst_Dev, INT8U* puch_Buff, INT16U uin_Len);

//==================================================================================
//| 函数名称 | Bsp_Printf
//|----------|----------------------------------------------------------------------
//| 函数功能 | 串口打印(非阻塞)
//|----------|----------------------------------------------------------------------
//| 输入参数 | pst_Dev:串口结构体 puch_Buff:发送缓冲区 uin_Len:发送缓冲区长度
//|----------|----------------------------------------------------------------------
//| 返回参数 | FALSE:参数有误, TRUE:发送成功
//|----------|----------------------------------------------------------------------
//| 函数设计 | wjb
//==================================================================================
void Bsp_UartPrintf(const char * Format,...);

#endif
