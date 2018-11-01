//==================================================================================
//| �ļ����� | STDBUS����
//|----------|----------------------------------------------------------------------
//| �ļ����� | 
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//==================================================================================
#ifndef __MOD_STDBUS_H__
#define __MOD_STDBUS_H__

#include "app_cfg.h"


#define     STDBUS_BUFF_MAX  1024

#define     STATE_IDLE     0x01
#define     STATE_RECV     0x02
#define     STATE_RECVED   0x04
#define     STATE_SEND     0x03

typedef struct __Stdbus {
    
    uint8_t     uch_Resv[4];
    uint8_t     uch_LinkLenth;
    uint8_t     uch_Location;
    uint8_t*    puc_AddrList;       /*������*/
    uint8_t     uch_Cmd;
    uint8_t     uch_SubCmd;
    uint16_t    uin_PayLoadLenth;
    uint8_t*    puc_PayLoad;        /*������*/
    
    
    void*       pv_PortHandle;
    uint8_t     uch_Address;
    
    uint8_t     uch_State;
    uint8_t     uch_LastByte;
    
    uint8_t     auc_Buff[STDBUS_BUFF_MAX];
    uint16_t    uin_BuffSize;                   //��������С
    uint16_t    uin_BuffLenth;                  //��Ч���ݵĳ���
    uint16_t    uin_BuffIndex;                  //��ǰ����
    
}StdBus_t;


extern StdBus_t    StdBus_Port0;
extern StdBus_t    StdBus_Port1;

void StdBus_Init(uint8_t uch_Address);
void StdbusPoll(void);


#endif
