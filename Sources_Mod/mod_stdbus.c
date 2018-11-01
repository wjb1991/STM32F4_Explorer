//==================================================================================
//| �ļ����� | STDBUS����
//|----------|----------------------------------------------------------------------
//| �ļ����� | 
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//==================================================================================
#include "mod_stdbus.h"
#include "mod_crc16.h"
#include "app_stdbus.h"

StdBus_t    StdBus_Port0;
StdBus_t    StdBus_Port1;

void StdBus_Init(uint8_t uch_Address);
void Send_ComPack(StdBus_t * pst_Fram);
void Send_OneByte(StdBus_t * pst_Fram);
void Deal_RecvByte(StdBus_t * pst_Fram, uint8_t* puc_Data, uint16_t uin_Lenth);
void StdbusPoll(void);
void Rsc_ComPack(StdBus_t * pst_Fram );
void Deal_ComPack(StdBus_t * pst_Fram);
void Make_ComPack(StdBus_t * pst_Fram);
void Make_ComPack(StdBus_t * pst_Fram);
void Send_Other(StdBus_t * pst_Fram);

uint8_t Deal_CmdPack(StdBus_t* pst_Fram);

//==================================================================================
//| �������� | s_Port0TransComplet
//|----------|----------------------------------------------------------------------
//| �������� | �˿�0�������
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void s_Port0TransComplet(void * pv_dev)
{
    Send_OneByte(&StdBus_Port0);
}

//==================================================================================
//| �������� | s_Port0RecvReady
//|----------|----------------------------------------------------------------------
//| �������� | �˿�0 ���ܵ�����
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void s_Port0RecvReady(void * pv_dev,uint8_t *puc_Buff,uint16_t uin_Lenth)
{
    Deal_RecvByte(&StdBus_Port0, puc_Buff, uin_Lenth);
}

void ErrHandle(void * pv_dev)
{
    TRACE_DBG(">>DBG:       �˿ڴ���\r\n");
}

void StdBus_Init(uint8_t uch_Address)
{
    Rsc_ComPack(&StdBus_Port0);
    StdBus_Port0.pv_PortHandle = &COM2;
    StdBus_Port0.uch_Address = uch_Address;
    
    ((Dev_SerialPort*)StdBus_Port0.pv_PortHandle)->cb_SendComplete = s_Port0TransComplet;
    ((Dev_SerialPort*)StdBus_Port0.pv_PortHandle)->cb_RecvReady = s_Port0RecvReady;
    ((Dev_SerialPort*)StdBus_Port0.pv_PortHandle)->cb_ErrHandle = ErrHandle;
}



void Send_ComPack(StdBus_t * pst_Fram)
{
    if(pst_Fram->uch_State == STATE_SEND)
    {
        if( StdBus_Port0.pv_PortHandle == (void *)&COM4)
            Bsp_Rs485de(eRs485Trans);
        
        pst_Fram->uin_BuffIndex = 0;
        Bsp_UartSend(pst_Fram->pv_PortHandle,
                      &pst_Fram->auc_Buff[pst_Fram->uin_BuffIndex++],
                      1);
    }
}

void Send_OneByte(StdBus_t * pst_Fram)
{
    if(pst_Fram->uch_State == STATE_SEND)
    {
        uint8_t uch_data = 0;

        if (pst_Fram->uin_BuffIndex < pst_Fram->uin_BuffLenth - 1 )
        {
            uch_data = pst_Fram->auc_Buff[pst_Fram->uin_BuffIndex++];
            if (pst_Fram->uch_LastByte == 0x7c)
            {
                uch_data ^= 0x7c;
            }
            else if(uch_data == 0x7b || uch_data == 0x7c|| uch_data == 0x7d)
            {
                uch_data = 0x7c;
                pst_Fram->uin_BuffIndex--;
    
            }
            pst_Fram->uch_LastByte = uch_data;
            
            Bsp_UartSend(pst_Fram->pv_PortHandle,
                          &uch_data,
                          1);        
        
        }
        else if (pst_Fram->uin_BuffIndex < pst_Fram->uin_BuffLenth )
        {
            uch_data = pst_Fram->auc_Buff[pst_Fram->uin_BuffIndex++];
            Bsp_UartSend(pst_Fram->pv_PortHandle,
                          &uch_data,
                          1);    
        }
        else 
        {
            if( StdBus_Port0.pv_PortHandle == (void *)&COM4)
                Bsp_Rs485de(eRs485Recv);
            
            Rsc_ComPack(pst_Fram);
            TRACE_DBG(">>DBG:       �������\r\n");
        }
    }
}


//==================================================================================
//| �������� | PostMsg
//|----------|----------------------------------------------------------------------
//| �������� | ������Ϣ(����ֲ)
//|----------|----------------------------------------------------------------------
//| ������� | ��Ϣ�ľ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void PostMsg(void* pst_Hardware)
{
    OS_ERR  os_err;
    OSTaskQPost(&TaskStdBusTCB,pst_Hardware,1,OS_OPT_POST_FIFO ,&os_err);
    if(os_err != OS_ERR_NONE)
    {
    
    }
}

//==================================================================================
//| �������� | PendMsg
//|----------|----------------------------------------------------------------------
//| �������� | �ȴ���Ϣ (����ֲ) �ڲ���������
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��Ϣ�ľ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void* PendMsg(void)
{
    OS_ERR  os_err;
    uint16_t ui_MsgSize = 0;
    void * pv_Msg;
    pv_Msg = OSTaskQPend(500,OS_OPT_PEND_BLOCKING,&ui_MsgSize,0,&os_err);
    
    if(os_err == OS_ERR_NONE)
        return pv_Msg;
    else
        return 0;
}

//==================================================================================
//| �������� | Deal_RecvByte
//|----------|----------------------------------------------------------------------
//| �������� | ����һ���ֽڵĺ���
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void Deal_RecvByte(StdBus_t * pst_Fram, uint8_t* puc_Data, uint16_t uin_Lenth)
{
    uint16_t i = 0;
    for(i = 0; i<uin_Lenth; i++)
    {
        if (pst_Fram->uch_State == STATE_IDLE)
        {            
            if (puc_Data[i] == 0x7b)
            {
                Rsc_ComPack((void*)pst_Fram);                  //�ͷű��˿ڵ�����
                pst_Fram->uch_State = STATE_RECV;
                pst_Fram->auc_Buff[pst_Fram->uin_BuffLenth++] = puc_Data[i];
            }
        }
        else if (pst_Fram->uch_State == STATE_RECV)
        {
            if(pst_Fram->uin_BuffLenth <= STDBUS_BUFF_MAX)
            {
                if (puc_Data[i] == 0x7d)
                {
                    pst_Fram->uch_State = STATE_RECVED;
                    pst_Fram->auc_Buff[pst_Fram->uin_BuffLenth++] = puc_Data[i];
                    PostMsg((void*)pst_Fram);
                    //Rsc_ComPack((void*)pst_Fram);                  //�ͷű��˿ڵ�����
                    TRACE_DBG(">>DBG:       ���յ�һ������\r\n");
                }
                /***/
                else if (puc_Data[i] == 0x7b)
                {
                    Rsc_ComPack((void*)pst_Fram);                  //�ͷű��˿ڵ�����
                    pst_Fram->uch_State = STATE_RECV;
                    pst_Fram->auc_Buff[pst_Fram->uin_BuffLenth++] = puc_Data[i];
                }
                else
                {
                    if(pst_Fram->uch_LastByte == 0x7c)
                        pst_Fram->auc_Buff[pst_Fram->uin_BuffLenth-1] ^= puc_Data[i];
                    else
                        pst_Fram->auc_Buff[pst_Fram->uin_BuffLenth++] = puc_Data[i];
                    pst_Fram->uch_LastByte = puc_Data[i];            
                }
                
            }
        }
    }
}

//==================================================================================
//| �������� | StdbusPoll
//|----------|----------------------------------------------------------------------
//| �������� | ������ѯ���� �ڲ���������
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void StdbusPoll(void)
{
    StdBus_t * pst_Fram = (StdBus_t *)PendMsg();
    
    if(pst_Fram != 0)
    {
        INT16U  uin_crc16;
        
        GetCrc16Bit(pst_Fram->auc_Buff + 1,pst_Fram->uin_BuffLenth - 2,&uin_crc16);
        if (uin_crc16 == 0)
        {
            TRACE_DBG(">>DBG:       CRCУ��ͨ��\r\n");
            //CRCУ��ͨ��
            Deal_ComPack(pst_Fram);   
            
            //Send_ComPack(pst_Fram);
        }
        else
        {
            TRACE_DBG(">>DBG:       CRCУ�鲻ͨ��\r\n");
            Rsc_ComPack((void*)pst_Fram);                  //�ͷű��˿ڵ�����
        }
    }
}

//==================================================================================
//| �������� | Rsc_ComPack
//|----------|----------------------------------------------------------------------
//| �������� | ���һ������
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void Rsc_ComPack(StdBus_t * pst_Fram )
{
    pst_Fram->uch_Resv[0] = 0;
    pst_Fram->uch_Resv[1] = 0;
    pst_Fram->uch_Resv[2] = 0;
    pst_Fram->uch_Resv[3] = 0;
    
    pst_Fram->uch_LinkLenth = 0;
    pst_Fram->uch_Location = 0;
    pst_Fram->puc_AddrList = 0;
    
    pst_Fram->uch_Cmd = 0;
    pst_Fram->uch_SubCmd = 0;
    pst_Fram->uin_PayLoadLenth =  0;
    pst_Fram->puc_PayLoad = 0;
    
    pst_Fram->uch_State = STATE_IDLE;
    pst_Fram->uch_LastByte = 0;
    pst_Fram->uin_BuffLenth = 0;
}

//==================================================================================
//| �������� | Deal_ComPack
//|----------|----------------------------------------------------------------------
//| �������� | ����һ������
//|----------|----------------------------------------------------------------------
//| ������� | Stdbus���ݽṹ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void Deal_ComPack(StdBus_t * pst_Fram)
{
    uint16_t i = 1;
    
    pst_Fram->uch_Resv[0] = pst_Fram->auc_Buff[i++]+1;
    pst_Fram->uch_Resv[1] = pst_Fram->auc_Buff[i++]+2;
    pst_Fram->uch_Resv[2] = pst_Fram->auc_Buff[i++]+3;
    pst_Fram->uch_Resv[3] = pst_Fram->auc_Buff[i++]+4;
    
    pst_Fram->uch_LinkLenth = pst_Fram->auc_Buff[i++];
    pst_Fram->uch_Location = pst_Fram->auc_Buff[i++];
    
    pst_Fram->puc_AddrList = &pst_Fram->auc_Buff[i];
    

    i += pst_Fram->uch_LinkLenth;
    pst_Fram->uch_Cmd = pst_Fram->auc_Buff[i++];
    pst_Fram->uch_SubCmd = pst_Fram->auc_Buff[i++];
    pst_Fram->uin_PayLoadLenth =  (uint16_t)(pst_Fram->auc_Buff[i++]<<8);
    pst_Fram->uin_PayLoadLenth +=  pst_Fram->auc_Buff[i++];

    pst_Fram->puc_PayLoad = &pst_Fram->auc_Buff[i];

    //�ж��Ƿ�����ĩ�ڵ�
    if(pst_Fram->puc_AddrList[pst_Fram->uch_Location] != pst_Fram->uch_Address)
    {
        //ת���������˿�
        TRACE_DBG(">>DBG:       ת���������˿�\r\n");
        Send_Other(pst_Fram);
        return;
    }
        
    //�����豸���ʱ��豸 ��App��ȥ����
    TRACE_DBG(">>DBG:       �����豸���ʱ��豸 ��App��ȥ����\r\n");
    if(0 != Deal_CmdPack(pst_Fram))
    {
        int i = 0;
        //��ת��ַ�б� ԭ·����
        for(i = 0 ; i < pst_Fram->uch_LinkLenth/2; i++)
        {
            uint8_t uch_temp = pst_Fram->puc_AddrList[pst_Fram->uch_LinkLenth -1 -i];
            pst_Fram->puc_AddrList[pst_Fram->uch_LinkLenth -1 -i] = pst_Fram->puc_AddrList[i];    //len = 4 0<>3 1<>2 // len = 5  0<>4 1<>3
            pst_Fram->puc_AddrList[i] = uch_temp;
        }
        pst_Fram->uch_Location = 1;
        pst_Fram->uch_SubCmd ^= 0xff; 
        Make_ComPack(pst_Fram);
    }
    else
    {
        //����Ҫ�ظ�
        Rsc_ComPack(pst_Fram);              //�ͷű��˿ڵ�����
    }
    
}

//==================================================================================
//| �������� | Make_ComPack
//|----------|----------------------------------------------------------------------
//| �������� | ��֡������
//|----------|----------------------------------------------------------------------
//| ������� | Stdbus���ݽṹ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
void Make_ComPack(StdBus_t * pst_Fram)
{
    /* �ı�puc_AddrList�ĳ��ȿ��ܳ������ݸ�����ע�⣡��*/
    uint16_t i = 0,j = 0 ,crc16 = 0;
    
    pst_Fram->auc_Buff[i++] = 0x7b;
    
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_Resv[0];
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_Resv[1];
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_Resv[2];
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_Resv[3];
    
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_LinkLenth;
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_Location;
    
    for( j = 0; j < pst_Fram->uch_LinkLenth; j++)
        pst_Fram->auc_Buff[i++] = pst_Fram->puc_AddrList[j];
    
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_Cmd;
    pst_Fram->auc_Buff[i++] = pst_Fram->uch_SubCmd;
    pst_Fram->auc_Buff[i++] = (uint8_t)(pst_Fram->uin_PayLoadLenth>>8);
    pst_Fram->auc_Buff[i++] = (uint8_t)(pst_Fram->uin_PayLoadLenth&0xff);
    
    for( j = 0; j < pst_Fram->uin_PayLoadLenth; j++)
        pst_Fram->auc_Buff[i++] = pst_Fram->puc_PayLoad[j];
    
    GetCrc16Bit(pst_Fram->auc_Buff + 1,i-1, &crc16);  
    pst_Fram->auc_Buff[i++] = (uint8_t)(crc16 >> 8);
    pst_Fram->auc_Buff[i++] = (uint8_t)(crc16 );
    
    pst_Fram->auc_Buff[i++] = 0x7d;
    pst_Fram->uin_BuffLenth = i;
    
    pst_Fram->uch_State = STATE_SEND; 
    Send_ComPack(pst_Fram);
}

uint8_t Deal_CmdPack(StdBus_t* pst_Fram)
{   
    if(pst_Fram->uch_SubCmd == 0x55 || pst_Fram->uch_SubCmd == 0x66 )
    {
        return Deal_SlavePack(pst_Fram);
    }
    else if(pst_Fram->uch_SubCmd == 0xaa || pst_Fram->uch_SubCmd == 0x99 )
    {
        return Deal_MasterPack(pst_Fram);
    }    
    return 0;
}

void Send_Other(StdBus_t * pst_Fram)
{
    
}
