//==================================================================================
//| �ļ����� | usbh_usb4000.c
//|----------|----------------------------------------------------------------------
//| �ļ����� | USB4000�����ǲ���
//|----------|----------------------------------------------------------------------
//| ������� | ��
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//==================================================================================

/* Includes ------------------------------------------------------------------*/
#include "usbh_template.h"

static USBH_StatusTypeDef USBH_USB4000_InterfaceInit  (USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_USB4000_InterfaceDeInit  (USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_USB4000_Process(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_USB4000_ClassRequest (USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_USB4000_SOFProcess(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_USB4000_GetStatus(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_USB4000_GetSpectrum(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_USB4000_SetIntegralTime(USBH_HandleTypeDef *phost,INT32U ITime);

static USBH_StatusTypeDef USBH_USB4000_Init(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_USB4000_QueryInformation(USBH_HandleTypeDef *phost,INT8U cmd,INT8U* auc_Buff);

static USBH_StatusTypeDef USBH_USB4000_QueryAllInformation(USBH_HandleTypeDef *phost);

static USBH_StatusTypeDef USBH_USB4000_ProcessSpectrum(USBH_HandleTypeDef *phost);

USBH_ClassTypeDef  USB4000_Class = 
{
  "USB4000",
  USB_USB4000_CLASS,
  USBH_USB4000_InterfaceInit,
  USBH_USB4000_InterfaceDeInit,
  USBH_USB4000_ClassRequest,
  USBH_USB4000_Process,
  USBH_USB4000_SOFProcess,
};

#pragma location = (0x68000000)        
static int16_t spa_buff[4096]={0};

#pragma location = (0x68002000)
static int32_t  sum_spa[4096] = {0};

#pragma location = (0x68006000)
static double   wavelenth_buff[4096]={0};

#pragma location = (0x6800E000 + 0x8000)    //0x6800A000
static double   process_spa[4096] = {0};
    
USB4000_HandleTypeDef   USB4000 = {

    0,//              OutPipe1;
    0,//              OutEp1; 
    0,//              InPipe1; 
    0,//              InEp1;
    0,//              InPipe2; 
    0,//              InEp2;
    0,//              InPipe6; 
    0,//              InEp6;
    FALSE,//          sof_signal;
    0,//              rx_count;
    FALSE,//          b_SetFlag;
    0,//              ul_SetIntegralTime;
    {0,0,0,0,0,0,0,0,0,0,0},//auin_EdcIndexs[11];             
    {0,0,0,0},              //alf_WlcCoeff[4];
    {0,0,0,0,0,0,0,0},      //alf_NlcCoeff[8];  
    0,//              uch_NlcOrder;
    spa_buff,//       pin_Spectrum;           
    sum_spa,//        pl_SumSpectrum;             
    process_spa,//    plf_ProcessSpectrum;    
    wavelenth_buff,// plf_WaveLenth;          
    0,//              uin_Pixels;             
    0,//              uch_ScansToAverage;     
    0,//              uch_ScansConut;         
    0,//              uch_Boxcar;             
    0,//              ul_IntegralTime;               
    FALSE,//          b_EdcEnable;            
    FALSE,//          b_NlcEnable;            
         
    FALSE,//          b_Open;
    FALSE,//          b_IsConnect;
    FALSE,//          b_HighSpeed;  
};
    
//==================================================================================
//| �������� | USBH_USB4000_InterfaceInit
//|----------|----------------------------------------------------------------------
//| �������� | USB4000�����ǽӿڳ�ʼ�� �����ڴ� ����˵�
//|----------|----------------------------------------------------------------------
//| ������� | phost�� �������
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
static USBH_StatusTypeDef USBH_USB4000_InterfaceInit (USBH_HandleTypeDef *phost)
{	  
    USB4000_HandleTypeDef* USB4000_Handle = NULL;
    uint8_t max_ep = 0;
    uint8_t num = 0;
    
    max_ep = ( (phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].bNumEndpoints <= USBH_MAX_NUM_ENDPOINTS) ? 
              phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].bNumEndpoints :
                  USBH_MAX_NUM_ENDPOINTS); 
    
    USBH_UsrLog ("EndPoint Num = %x.", max_ep);

    //ע�������ݵ�����
    phost->pActiveClass->pData = &USB4000;//(USB4000_HandleTypeDef *)USBH_malloc (sizeof(USB4000_HandleTypeDef));  
    USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 
    
    /*�����õĶ˵�����*/
    /*�ڽӿ��������в���EPs������*/
    /*ѡ��ϵ͵����֣����ⳬ���ѷ���Ļ�����*/
    /*����˵������������� �ӽӿ��������ж˵��ַ */
    for ( ;num < max_ep; num++)
    {
        uint8_t bEndpointAddress = phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[num].bEndpointAddress;            
        uint16_t len;
        uint8_t poll;
        uint8_t* ref_ep;
        uint8_t* ref_pipe;
            
        //USBH_UsrLog ("bEndpointAddress = %x.", bEndpointAddress);

        if(bEndpointAddress & 0x80)
        {
            switch (bEndpointAddress)
            {
            case 0x81:  /* �˵�1��������˵� */
                ref_ep = &USB4000_Handle->InEp1;
                ref_pipe = &USB4000_Handle->InPipe1;
                break;  
            case 0x82:  /* �˵�2��������˵� */
                ref_ep = &USB4000_Handle->InEp2;
                ref_pipe = &USB4000_Handle->InPipe2;
                break;
            case 0x86:  /* �˵�6��������˵� */
                ref_ep = &USB4000_Handle->InEp6;
                ref_pipe = &USB4000_Handle->InPipe6;
                break;
            default:
                break;
            }
           
            *ref_ep = phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[num].bEndpointAddress;
            *ref_pipe = USBH_AllocPipe(phost, *ref_ep);
            
            len     = phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[num].wMaxPacketSize;
            poll    = phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[num].bInterval ;
            
            /* Open pipe for IN endpoint */
            USBH_OpenPipe  (phost,
                            *ref_pipe,
                            *ref_ep,
                            phost->device.address,
                            phost->device.speed,
                            USB_EP_TYPE_BULK,
                            len); 

            USBH_LL_SetToggle (phost, *ref_pipe, 1);

            USBH_UsrLog ("InEp addr = %x pipe = %x len = %x poll = %x", *ref_ep,*ref_pipe,len,poll);
            
        }
        else
        {
            ref_ep = &USB4000_Handle->OutEp1;
            ref_pipe = &USB4000_Handle->OutPipe1;
          
            *ref_ep = (phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[num].bEndpointAddress);
            *ref_pipe = USBH_AllocPipe(phost, *ref_ep);

            len     = phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[num].wMaxPacketSize;
            poll    = phost->device.CfgDesc.Itf_Desc[phost->device.current_interface].Ep_Desc[num].bInterval ;          
            
            /* Open pipe for OUT endpoint  */
            USBH_OpenPipe  (phost,
                *ref_pipe,
                *ref_ep,                            
                phost->device.address,
                phost->device.speed,
                USB_EP_TYPE_BULK,
                len); 
           
            USBH_LL_SetToggle (phost,*ref_pipe, 1);
            
            USBH_UsrLog ("OutEp addr = %x pipe = %x len = %x poll = %x", *ref_ep,*ref_pipe,len,poll);
        }

    }
    
    return USBH_OK;
}

//==================================================================================
//| �������� | USBH_USB4000_InterfaceDeInit
//|----------|----------------------------------------------------------------------
//| �������� | USB4000�����ǽӿڷ���ʼ�� �ͷ��ڴ� ����˵�
//|----------|----------------------------------------------------------------------
//| ������� | phost�� �������
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
USBH_StatusTypeDef USBH_USB4000_InterfaceDeInit (USBH_HandleTypeDef *phost)
{
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 

    // �ͷŸ��ֹܵ� 
    if(USB4000_Handle->OutPipe1 != 0)
    {
        USBH_ClosePipe  (phost, USB4000_Handle->OutPipe1);
        USBH_FreePipe  (phost, USB4000_Handle->OutPipe1);
        USB4000_Handle->OutPipe1 = 0;     /* Reset the pipe as Free */  
    }

    if(USB4000_Handle->InPipe1 != 0)
    {
        USBH_ClosePipe  (phost, USB4000_Handle->InPipe1);
        USBH_FreePipe  (phost, USB4000_Handle->InPipe1);
        USB4000_Handle->InPipe1 = 0;     /* Reset the pipe as Free */  
    }

    if(USB4000_Handle->InPipe2 != 0)
    {
        USBH_ClosePipe  (phost, USB4000_Handle->InPipe2);
        USBH_FreePipe  (phost, USB4000_Handle->InPipe2);
        USB4000_Handle->InPipe2 = 0;     /* Reset the pipe as Free */  
    }

    if(USB4000_Handle->InPipe6 != 0)
    {
        USBH_ClosePipe  (phost, USB4000_Handle->InPipe6);
        USBH_FreePipe  (phost, USB4000_Handle->InPipe6);
        USB4000_Handle->InPipe6 = 0;     /* Reset the pipe as Free */  
    }

    /* �ͷ������� */
    if(phost->pActiveClass->pData)
    {
        //USBH_free (phost->pActiveClass->pData);
        phost->pActiveClass->pData = NULL;
    }
    return USBH_OK;
}

//==================================================================================
//| �������� | USBH_USB4000_ClassRequest
//|----------|----------------------------------------------------------------------
//| �������� | ��׼Э���ʼ�� USB4000���Զ����豸����Ҫ
//|----------|----------------------------------------------------------------------
//| ������� | phost�� �������
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
static USBH_StatusTypeDef USBH_USB4000_ClassRequest (USBH_HandleTypeDef *phost)
{   
    /* ��׼��Э������ ��ȡ����������  �Զ�����豸Ӧ�ò��ð� */
    USBH_UsrLog ("ClassRequest");
    return USBH_OK; 
}


//==================================================================================
//| �������� | USBH_USB4000_Process
//|----------|----------------------------------------------------------------------
//| �������� | USB4000����
//|----------|----------------------------------------------------------------------
//| ������� | phost�� �������
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
static USBH_StatusTypeDef USBH_USB4000_Process (USBH_HandleTypeDef *phost)
{
    OS_ERR os_err;
  
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 
    uint16_t i = 0;   
    
    USB4000_Handle->rx_count = 0;

    USB4000_Handle->pin_Spectrum = spa_buff;
    USB4000_Handle->pl_SumSpectrum = sum_spa;
    USB4000_Handle->plf_ProcessSpectrum = process_spa;
    USB4000_Handle->plf_WaveLenth = wavelenth_buff;
    USB4000_Handle->uin_Pixels = 3648;     /* USB�ӿ�3648������ ʵ�ʴ��� 3840������*/
  
    /*  EEPROM ��ʼ��
    USB4000_Handle->uch_ScansToAverage = 5;
    USB4000_Handle->uch_ScansConut = 0;
    USB4000_Handle->uch_Boxcar = 32;
    USB4000_Handle->ul_IntegralTime = 100000;
    
    USB4000_Handle->b_EdcEnable = TRUE;
    USB4000_Handle->b_NlcEnable = FALSE; 
    */
    
    USB4000_Handle->b_Open = TRUE;
    
    /*
    while(phost->gState == HOST_CLASS)
    {
        USBH_USB4000_GetSpectrum(phost);
        OSTimeDlyHMSM(0u, 0u, 0u, 200,
                      //OS_OPT_TIME_HMSM_STRICT,
                      OS_OPT_TIME_HMSM_STRICT | OS_OPT_TIME_PERIODIC,
                      &os_err);
    }
    */
    
    
    USBH_UsrLog ("==================USB4000Init==================");  
    USBH_USB4000_Init(phost);
    //USBH_USB4000_GetStatus(phost);

    USBH_UsrLog ("==================USB4000WaitInit==================");   
    OSTimeDlyHMSM(0u, 0u, 5u, 0u,
                  OS_OPT_TIME_HMSM_STRICT,
                    &os_err);
    
    while(phost->gState == HOST_CLASS)
    {
        if(USBH_USB4000_GetStatus(phost) == USBH_OK)
            break;
        OSTimeDlyHMSM(0u, 0u, 0u, 10u,
                      OS_OPT_TIME_HMSM_STRICT,
                        &os_err);
    }
    
    USBH_USB4000_QueryAllInformation(phost);
    
    USBH_USB4000_SetIntegralTime(phost,USB4000_Handle->ul_SetIntegralTime);
    
    USBH_USB4000_GetStatus(phost);

    /* ��ϼ��㲨�� */
    for(i = 0;i < USB4000_Handle->uin_Pixels; i++)
    {
        USB4000_Handle->plf_WaveLenth[i] = USB4000_Handle->alf_WlcCoeff[0] + 
                                           USB4000_Handle->alf_WlcCoeff[1] * i +
                                           USB4000_Handle->alf_WlcCoeff[2] * i * i + 
                                           USB4000_Handle->alf_WlcCoeff[3] * i * i * i;    
        USB4000_Handle->pl_SumSpectrum[i] = 0;
    }

    for(i = 0;i < 11; i++)
        USB4000_Handle->auin_EdcIndexs[i] = i+5;
    
    USB4000_Handle->b_IsConnect = TRUE;

    
    USBH_UsrLog ("==================USB4000Process==================");
    
    while(phost->gState == HOST_CLASS)
    {
        uint32_t delay_time = USB4000_Handle->ul_IntegralTime / 1000;
        
        /* ��ȡ������Ҫ20ms���� ���50ms��ȡһ�Ź��� �����20+ms ������������ */
        if(delay_time < 200)
        {
            delay_time = 200;
        }
        
        OSTimeDlyHMSM(0u, 0u, 0u, delay_time,
                      //OS_OPT_TIME_HMSM_STRICT,
                      OS_OPT_TIME_HMSM_STRICT | OS_OPT_TIME_PERIODIC,/* ����ģʽ */
                      &os_err);
        
        OSTaskQFlush(&TaskUsbHostTCB,&os_err);
        
        if(USB4000_Handle->b_SetFlag == TRUE)
        {
            USBH_USB4000_SetIntegralTime(phost,USB4000_Handle->ul_SetIntegralTime);
            USB4000_Handle->b_SetFlag = FALSE;
        }
        else
        {    
            if( USBH_USB4000_GetSpectrum(phost)==USBH_FAIL );
            {

            }
            //USBH_USB4000_GetStatus(phost);        
        }
    }
    
    USB4000_Handle->b_IsConnect = FALSE;
    
    USBH_UsrLog ("Exit ClassProcess");

    return USBH_OK;
}

//==================================================================================
//| �������� | USBH_USB4000_SOFProcess
//|----------|----------------------------------------------------------------------
//| �������� | USB4000֡��ʼ�źŴ���
//|----------|----------------------------------------------------------------------
//| ������� | phost�� �������
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
static USBH_StatusTypeDef USBH_USB4000_SOFProcess(USBH_HandleTypeDef *phost)
{
    //USBH_UsrLog("USBH_USB4000_SOFProcess\r\n");
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 
    
    USB4000_Handle->sof_signal = TRUE;
    
    return USBH_OK;
}

//==================================================================================
//| �������� | USBH_USB4000_GetStatus
//|----------|----------------------------------------------------------------------
//| �������� | USB4000 ����״̬
//|----------|----------------------------------------------------------------------
//| ������� | phost�� �������
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
static USBH_StatusTypeDef USBH_USB4000_GetStatus(USBH_HandleTypeDef *phost)
{
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    OS_ERR os_err;
    INT8U uc_SendBuff[1];
    INT8U auc_Buff[64]={0};
    INT8U uc_ErrCnt = 0; 
    void* pv_Msg = 0;
    INT16U uin_Len = 0;
    USB4000_Handle->e_State = USB4000_SEND_DATA;
    HAL_Delay(1);  
    
    while(phost->gState == HOST_CLASS)
    {
        switch(USB4000_Handle->e_State)
        {
        case USB4000_SEND_DATA:     //��������
            uc_SendBuff[0] = 0xFE;
            USBH_BulkSendData(phost,uc_SendBuff,1,USB4000_Handle->OutPipe1,0);
            USB4000_Handle->e_State = USB4000_WAIT_SEND;
            break;
                
        case USB4000_WAIT_SEND:     //�ȴ��������
          
            pv_Msg = OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
            if(os_err != OS_ERR_NONE)
            {
                USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                USBH_ClrFeature(phost,USB4000_Handle->InEp1);
                USBH_UsrLog ("��ʱ");
                return USBH_FAIL;
            }
            
            if((INT32U)pv_Msg != USBH_URB_EVENT)
                continue;
          
            URB_Status = USBH_LL_GetURBState(phost, USB4000_Handle->OutPipe1);
            switch (URB_Status)
            {
            case USBH_URB_DONE:
                USB4000_Handle->e_State = USB4000_GET_DATA;
                break;
            case USBH_URB_NOTREADY:
                /* ���·��� */
                USB4000_Handle->e_State = USB4000_SEND_DATA;
                break;
            case USBH_URB_STALL:
                return USBH_FAIL;
                break;
            default:
                break;
            }
        case USB4000_GET_DATA:
            HAL_Delay(1);
            USBH_BulkReceiveData(phost, auc_Buff, 64, USB4000_Handle->InPipe1);
            USB4000_Handle->e_State = USB4000_WAIT_GET;
            break;
        case USB4000_WAIT_GET:

            pv_Msg = OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
            if(os_err != OS_ERR_NONE)
            {                USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                USBH_ClrFeature(phost,USB4000_Handle->InEp1);
                USBH_UsrLog("������Ϣʧ�� ��ʱ");
                USB4000_Handle->e_State = USB4000_IDLE;
                return USBH_FAIL;
            }
            
            if((INT32U)pv_Msg != USBH_URB_EVENT)
                continue;
            
            URB_Status = USBH_LL_GetURBState(phost, USB4000_Handle->InPipe1);
      
            if(URB_Status == USBH_URB_DONE)
            {
                USBH_UsrLog ("\r\nUSBH_URB_DONE");
  
                INT32U t = Bsp_CnvArrToINT32U(&auc_Buff[2],FALSE);

                USB4000_Handle->ul_IntegralTime = t;
                
                if( auc_Buff[14] !=0 )
                {
                    USB4000_Handle->b_HighSpeed = TRUE;
                }
                else
                {
                    USB4000_Handle->b_HighSpeed = FALSE;        //FullSpeed
                }
              
                USBH_UsrLog ("����״̬���");
                USB4000_Handle->e_State = USB4000_IDLE;  
                return USBH_OK;

            }
            else if(URB_Status == USBH_URB_IDLE)
            {
                if(++uc_ErrCnt>=10)
                {
                    USBH_UsrLog ("\r\nUSBH_URB_IDLE");
                
                    USBH_UsrLog ("����״̬ʧ��");
                    USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                    USBH_ClrFeature(phost,USB4000_Handle->InEp1);
                    USB4000_Handle->e_State = USB4000_IDLE;
                    return USBH_FAIL;                
                }
            }
            else if(URB_Status == USBH_URB_NOTREADY)
            {
                if(++uc_ErrCnt>=10)
                {
                    USBH_UsrLog ("\r\nUSBH_URB_NOTREADY");
                
                    USBH_UsrLog ("����״̬ʧ��");
                    USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                    USBH_ClrFeature(phost,USB4000_Handle->InEp1);
                    USB4000_Handle->e_State = USB4000_IDLE;
                    return USBH_FAIL;                
                }
            }
            else
                return USBH_FAIL;
            break;  
        default:
            break;
        }
    }
    return USBH_OK;
}

//==================================================================================
//| �������� | USBH_USB4000_Init
//|----------|----------------------------------------------------------------------
//| �������� | USB4000 ��ʼ��
//|----------|----------------------------------------------------------------------
//| ������� | phost�� �������
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
static USBH_StatusTypeDef USBH_USB4000_Init(USBH_HandleTypeDef *phost)
{
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    OS_ERR os_err;
    INT8U uc_SendBuff[1];
    INT8U uc_ErrCnt = 0; 
    void* pv_Msg = 0;
    INT16U uin_Len = 0;
    USB4000_Handle->e_State = USB4000_SEND_DATA;
    HAL_Delay(1);  
    while(phost->gState == HOST_CLASS)
    {
        switch(USB4000_Handle->e_State)
        {
        case USB4000_SEND_DATA:     //��������
            uc_SendBuff[0] = 0x01;
            USBH_BulkSendData(phost,uc_SendBuff,1,USB4000_Handle->OutPipe1,0);
            USB4000_Handle->e_State = USB4000_WAIT_SEND;
            break;
                
        case USB4000_WAIT_SEND:     //�ȴ��������
          
            pv_Msg = OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
            if(os_err != OS_ERR_NONE)
            {
                USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                USBH_UsrLog ("��ʱ");
                return USBH_FAIL;
            }
            
            if((INT32U)pv_Msg != USBH_URB_EVENT)
                continue;
          
            URB_Status = USBH_LL_GetURBState(phost, USB4000_Handle->OutPipe1);
            switch (URB_Status)
            {
            case USBH_URB_DONE:
                USBH_UsrLog ("��ʼ���ɹ�");
                USB4000_Handle->e_State = USB4000_IDLE;
                return USBH_OK;
                break;
            case USBH_URB_NOTREADY:
                /* ���·��� */
                USB4000_Handle->e_State = USB4000_SEND_DATA;
                break;
            case USBH_URB_STALL:
                USBH_UsrLog ("��ʼ��ʧ��");
                return USBH_FAIL;
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
    return USBH_OK;
}

//==================================================================================
//| �������� | USBH_USB4000_SetIntegralTime
//|----------|----------------------------------------------------------------------
//| �������� | USB4000 ���û���ʱ��
//|----------|----------------------------------------------------------------------
//| ������� | phost�� �������
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
static USBH_StatusTypeDef USBH_USB4000_SetIntegralTime(USBH_HandleTypeDef *phost,INT32U IntegTime)
{
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    OS_ERR os_err;
    INT8U uc_SendBuff[5];
    INT8U uc_ErrCnt = 0; 
    void* pv_Msg = 0;
    INT16U uin_Len = 0;
    USB4000_Handle->e_State = USB4000_SEND_DATA;
    HAL_Delay(1);  
    while(phost->gState == HOST_CLASS)
    {
        switch(USB4000_Handle->e_State)
        {
        case USB4000_SEND_DATA:     //��������
            uc_SendBuff[0] = 0x02;
            Bsp_CnvINT32UToArr(&uc_SendBuff[1],IntegTime,FALSE);
            
            USBH_BulkSendData(phost,uc_SendBuff,5,USB4000_Handle->OutPipe1,0);
            USB4000_Handle->e_State = USB4000_WAIT_SEND;
            break;
                
        case USB4000_WAIT_SEND:     //�ȴ��������
          
            pv_Msg = OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
            if(os_err != OS_ERR_NONE)
            {
                USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                USBH_UsrLog ("��ʱ");
                return USBH_FAIL;
            }
            
            if((INT32U)pv_Msg != USBH_URB_EVENT)
                continue;
          
            URB_Status = USBH_LL_GetURBState(phost, USB4000_Handle->OutPipe1);
            switch (URB_Status)
            {
            case USBH_URB_DONE:
                USBH_UsrLog ("���û���ʱ��ɹ�");
                USB4000_Handle->e_State = USB4000_IDLE;
                return USBH_OK;
                break;
            case USBH_URB_NOTREADY:
                /* ���·��� */
                USB4000_Handle->e_State = USB4000_SEND_DATA;
                break;
            case USBH_URB_STALL:
                USBH_UsrLog ("���û���ʱ��ʧ��");
                return USBH_FAIL;
                break;
            default:
                break;
            }
        default:
            break;
        }
    }
    return USBH_OK;
}

//==================================================================================
//| �������� | USBH_USB4000_GetSpectrum
//|----------|----------------------------------------------------------------------
//| �������� | USB4000 ��ȡһ�Ź��ײ�����
//|----------|----------------------------------------------------------------------
//| ������� | phost�� �������
//|----------|----------------------------------------------------------------------
//| ���ز��� | ��
//|----------|----------------------------------------------------------------------
//| ������� | wjb
//==================================================================================
static USBH_StatusTypeDef USBH_USB4000_GetSpectrum(USBH_HandleTypeDef *phost)
{  
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    OS_ERR os_err;
    
    INT8U auc_Buff[64] = {0};
    INT8U uc_ErrCnt = 0; 
    void* pv_Msg = 0;
    INT16U uin_Len = 0;
    USB4000_Handle->e_State = USB4000_SEND_DATA;
    
    while(phost->gState == HOST_CLASS)
    {
        switch(USB4000_Handle->e_State)
        {
        case USB4000_SEND_DATA:     //��������
            auc_Buff[0] = 0x09;
            USBH_BulkSendData(phost,auc_Buff,1,USB4000_Handle->OutPipe1,0);
            HAL_Delay(1);
            USB4000_Handle->e_State = USB4000_WAIT_SEND;
            break;
                
        case USB4000_WAIT_SEND:     //�ȴ��������
          
            pv_Msg = OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
            if(os_err != OS_ERR_NONE)
            {
                USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                USBH_UsrLog ("��ʱ");
                return USBH_FAIL;
            }
            
            if((INT32U)pv_Msg != USBH_URB_EVENT)
                continue;
          
            URB_Status = USBH_LL_GetURBState(phost, USB4000_Handle->OutPipe1);
            switch (URB_Status)
            {
            case USBH_URB_DONE:
                USB4000_Handle->rx_count = 0;
                USB4000_Handle->e_State = USB4000_GET_DATA;
                break;
            case USBH_URB_NOTREADY:
                /* ���·��� */
                USB4000_Handle->e_State = USB4000_SEND_DATA;
                break;
            case USBH_URB_STALL:
                return USBH_FAIL;
                break;
            default:
                break;
            }
        case USB4000_GET_DATA:
            USBH_BulkReceiveData(phost, auc_Buff, 64, USB4000_Handle->InPipe2);
            USB4000_Handle->e_State = USB4000_WAIT_GET;
            break;
        case USB4000_WAIT_GET:

            pv_Msg = OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
            if(os_err != OS_ERR_NONE)
            {
                USBH_ClrFeature(phost,USB4000_Handle->InEp2);
                USBH_UsrLog("�������ʧ�� ��ʱ");
                USB4000_Handle->rx_count = 0;   
                USB4000_Handle->e_State = USB4000_IDLE;
                return USBH_FAIL;
            }
            
            if((INT32U)pv_Msg != USBH_URB_EVENT)
                continue;
            
            URB_Status = USBH_LL_GetURBState(phost, USB4000_Handle->InPipe2);

            if(URB_Status == USBH_URB_DONE)
            {
                //USBH_UsrLog("Recv USBH_URB_DONE %u",USB4000_Handle->rx_count);
                if(USB4000_Handle->rx_count >= 120)
                {
                    /* ���ܳɹ� pack0~ pack120 rx = 121*/
                    //USBH_USB4000_ProcessSpectrum(phost);
                    if(auc_Buff[0] == 0x69)
                    {
                        USBH_UsrLog ("����������");
                        USB4000_Handle->rx_count = 0;   
                        USB4000_Handle->e_State = USB4000_IDLE;                         
                    }
                    else
                    {
                        USBH_UsrLog("�������ʧ��");
                        USB4000_Handle->rx_count = 0;   
                        USB4000_Handle->e_State = USB4000_IDLE;  
                    }
                    return USBH_OK;
                }
                else
                {
                    uint16_t offset = (USB4000_Handle->rx_count) * 64;
                    uint8_t* puc_buff = (uint8_t*)USB4000_Handle->pin_Spectrum;
                    /**/
                    for(int i = 0 ; i < 64; i++)
                    {
                        puc_buff[offset+i] = auc_Buff[i];
                    } 
                    
                    USBH_BulkReceiveData(phost, auc_Buff, 64, USB4000_Handle->InPipe2);
                    USB4000_Handle->rx_count++; 
                    uc_ErrCnt = 0;
                }
            }
            else if(URB_Status == USBH_URB_STALL)
            {
                USBH_UsrLog("Recv USBH_URB_STALL");
                USBH_UsrLog("�������ʧ��");
                USB4000_Handle->rx_count = 0;   
                USB4000_Handle->e_State = USB4000_IDLE;
                return USBH_FAIL;
            }
            else if(URB_Status == USBH_URB_NOTREADY)
            {
                //USBH_UsrLog("Recv USBH_URB_NOTREADY");
                if(++uc_ErrCnt >= 100)
                {                
                    USBH_UsrLog("Recv USBH_URB_NOTREADY");
                    USBH_UsrLog("�������ʧ��");
                    USB4000_Handle->rx_count = 0;   
                    USB4000_Handle->e_State = USB4000_IDLE;
                    return USBH_FAIL;
                }
            }
            else if(URB_Status == USBH_URB_ERROR)
            {
                USBH_UsrLog("Recv USBH_URB_ERROR");
                USBH_UsrLog("�������ʧ��");
                USB4000_Handle->rx_count = 0;   
                USB4000_Handle->e_State = USB4000_IDLE; 
                return USBH_FAIL;
            }
            else if(URB_Status == USBH_URB_IDLE)
            {
                if(++uc_ErrCnt >= 100)
                {
                    USBH_UsrLog("Recv USBH_URB_IDLE");
                    USBH_UsrLog("�������ʧ��");
                    USB4000_Handle->rx_count = 0;   
                    USB4000_Handle->e_State = USB4000_IDLE;
                    return USBH_FAIL;
                }
            }

            break;  
        default:
            break;
        }
    }
    return USBH_OK;
}

static USBH_StatusTypeDef USBH_USB4000_QueryInformation(USBH_HandleTypeDef *phost,INT8U cmd,INT8U* auc_Buff)
{
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 
    USBH_URBStateTypeDef URB_Status = USBH_URB_IDLE;
    OS_ERR os_err;
    INT8U uc_SendBuff[2];
    INT8U uc_ErrCnt = 0; 
    void* pv_Msg = 0;
    INT16U uin_Len = 0;
    USB4000_Handle->e_State = USB4000_SEND_DATA;
    HAL_Delay(1);  
    while(phost->gState == HOST_CLASS)
    {
        switch(USB4000_Handle->e_State)
        {
        case USB4000_SEND_DATA:     //��������
            uc_SendBuff[0] = 0x05;
            uc_SendBuff[1] = cmd;
            USBH_BulkSendData(phost,uc_SendBuff,2,USB4000_Handle->OutPipe1,0);
            USB4000_Handle->e_State = USB4000_WAIT_SEND;
            break;
                
        case USB4000_WAIT_SEND:     //�ȴ��������
          
            pv_Msg = OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
            if(os_err != OS_ERR_NONE)
            {
                USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                USBH_ClrFeature(phost,USB4000_Handle->InEp1);
                USBH_UsrLog ("��ʱ");
                return USBH_FAIL;
            }
            
            if((INT32U)pv_Msg != USBH_URB_EVENT)
                continue;
          
            URB_Status = USBH_LL_GetURBState(phost, USB4000_Handle->OutPipe1);
            switch (URB_Status)
            {
            case USBH_URB_DONE:
                USB4000_Handle->e_State = USB4000_GET_DATA;
                break;
            case USBH_URB_NOTREADY:
                /* ���·��� */
                USB4000_Handle->e_State = USB4000_SEND_DATA;
                break;
            case USBH_URB_STALL:
                return USBH_FAIL;
                break;
            default:
                break;
            }
        case USB4000_GET_DATA:
            HAL_Delay(1);
            USBH_BulkReceiveData(phost, auc_Buff, 64, USB4000_Handle->InPipe1);
            USB4000_Handle->e_State = USB4000_WAIT_GET;
            break;
        case USB4000_WAIT_GET:

            pv_Msg = OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
            if(os_err != OS_ERR_NONE)
            {                USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                USBH_ClrFeature(phost,USB4000_Handle->InEp1);
                USBH_UsrLog("������Ϣʧ�� ��ʱ");
                USB4000_Handle->e_State = USB4000_IDLE;
                return USBH_FAIL;
            }
            
            if((INT32U)pv_Msg != USBH_URB_EVENT)
                continue;
            
            URB_Status = USBH_LL_GetURBState(phost, USB4000_Handle->InPipe1);
      
            if(URB_Status == USBH_URB_DONE)
            {
                USBH_UsrLog ("\r\nUSBH_URB_DONE");
                if(auc_Buff[0] == uc_SendBuff[0] && auc_Buff[1] == uc_SendBuff[1])
                {
                    USBH_UsrLog ("������Ϣ��� %u",cmd);
                    USB4000_Handle->e_State = USB4000_IDLE;  
                    return USBH_OK;
                }
                else
                {
                    USBH_UsrLog ("������Ϣʧ�� %u",cmd);
                    USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                    USBH_ClrFeature(phost,USB4000_Handle->InEp1);

                    USB4000_Handle->e_State = USB4000_IDLE;
                    return USBH_FAIL;
                }
            }
            else if(URB_Status == USBH_URB_IDLE)
            {
                if(++uc_ErrCnt>=10)
                {
                    USBH_UsrLog ("\r\nUSBH_URB_IDLE");
                
                    USBH_UsrLog ("������Ϣʧ��");
                    USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                    USBH_ClrFeature(phost,USB4000_Handle->InEp1);
                    USB4000_Handle->e_State = USB4000_IDLE;
                    return USBH_FAIL;                
                }
            }
            else if(URB_Status == USBH_URB_NOTREADY)
            {
                if(++uc_ErrCnt>=10)
                {
                    USBH_UsrLog ("\r\nUSBH_URB_NOTREADY");
                
                    USBH_UsrLog ("������Ϣʧ��");
                    USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
                    USBH_ClrFeature(phost,USB4000_Handle->InEp1);
                    USB4000_Handle->e_State = USB4000_IDLE;
                    return USBH_FAIL;                
                }
            }
            else
              return USBH_FAIL;
            break;  
        default:
            break;
        }
    }
    return USBH_OK;
}

static USBH_StatusTypeDef USBH_USB4000_QueryAllInformation(USBH_HandleTypeDef *phost)
{
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 
    INT8U   aauc_Str[31][64] = {0};
      
    for(int i = 0; i <= 18; )
    {
        if(USBH_OK == USBH_USB4000_QueryInformation(phost,i,&aauc_Str[i][0]))
        {
            printf("��Ϣ%u = ",i);
            printf((const char*)&aauc_Str[i][2]);
            printf("\r\n");
            i++;
        }
        else
        {
            OS_ERR os_err;
            OSTimeDlyHMSM(0u, 0u, 0u, 100,
                        OS_OPT_TIME_HMSM_STRICT ,/* ����ģʽ */
                        &os_err);
            //USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
            //USBH_ClrFeature(phost,USB4000_Handle->InEp1);
        }
        
        if(phost->gState != HOST_CLASS)
            break;
    } 
}

static USBH_StatusTypeDef USBH_USB4000_ProcessSpectrum(USBH_HandleTypeDef *phost)
{
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData;
    uint16_t i = 0;
    uint8_t ready = 0;
    
    /* ���ɨ��ƽ�� */
    if (USB4000_Handle->uch_ScansToAverage > 1)
    {
        if(++USB4000_Handle->uch_ScansConut >= USB4000_Handle->uch_ScansToAverage)
        {
            USB4000_Handle->uch_ScansConut = 0;
            
            /* ��ƽ�� */
            for(i = 0; i < USB4000_Handle->uin_Pixels; i++)
            {
                USB4000_Handle->plf_ProcessSpectrum[i] = ((double)USB4000_Handle->pl_SumSpectrum[i]) / 
                                                          USB4000_Handle->uch_ScansToAverage;
            }
            
            /* ������� */
            for(i = 0; i < USB4000_Handle->uin_Pixels; i++)
            {
                USB4000_Handle->pl_SumSpectrum[i] = 0;
            }
            
            ready = 1;
        }
        else
        {
            /* �ۼƹ��� */
            for(i = 0; i < USB4000_Handle->uin_Pixels; i++)
            {
                USB4000_Handle->pl_SumSpectrum[i] += USB4000_Handle->pin_Spectrum[i];
            }
        }
      
    }
    else 
    {
        USB4000_Handle->uch_ScansConut = 0;

        for(i = 0; i < USB4000_Handle->uin_Pixels; i++)
        {
            USB4000_Handle->pl_SumSpectrum[i] = 0;
            USB4000_Handle->plf_ProcessSpectrum[i] = USB4000_Handle->pin_Spectrum[i];
        }
        
        ready = 1;
    }
    
    if( ready != 0 )
    {
        if(USB4000_Handle->b_EdcEnable == TRUE)
        {
            double value = 0.0;
            int usableDarkCount = 0;
            
            for (int i = 0; i < 11; i++)
            {
                int edcPixelIndex = USB4000_Handle->auin_EdcIndexs[i];
                if (edcPixelIndex <  USB4000_Handle->uin_Pixels)
                {
                    value += USB4000_Handle->plf_ProcessSpectrum[edcPixelIndex];
                    usableDarkCount++;
                }
            }
            
            if (usableDarkCount > 0)
            {
                value /= usableDarkCount;
                for (int i = 0; i < USB4000_Handle->uin_Pixels; i++)
                    USB4000_Handle->plf_ProcessSpectrum[i] -= value;
            }
        }
        
        /* ������У�� */
        if(USB4000_Handle->b_NlcEnable == TRUE)
        {
            
        }

        // perform boxcar if requested (should be done after NLC)
        if (USB4000_Handle->uch_Boxcar > 0)
        {
            int boxcar = USB4000_Handle->uch_Boxcar;
            double * procSpectrum = USB4000_Handle->plf_ProcessSpectrum;
            double smoothed[200] = {0};
            int boxcarLimit = USB4000_Handle->uin_Pixels - boxcar - 1;  /* ���һ���������*/
            int boxcarRange = 2 * boxcar + 1;                                               /* �˲���Χ */
            double sum; 
            int i,j,k; 
            
            /* �������ݵ��ڴ��� */         
            for( k = 0; k < boxcarRange; k++ )
            {
                smoothed[k] = procSpectrum[k];
            }

            for (i = boxcar; i <= boxcarLimit; i++)
            {
                sum = 0;
                for(j = 0; j < boxcarRange; j++)
                {
                    sum += smoothed[j];
                }
                procSpectrum[i] = sum / boxcarRange;
              
                /* ������һ���㵽������ ѭ��*/
                if(k >= boxcarRange)
                    k = 0;
                else
                    k++;
                
                smoothed[k] = procSpectrum[i+boxcar];     
            }
        }  
    
        /* ֪ͨ�ϲ� ����ģ��
        OS_ERR  os_err;
        OSTaskQPost (&TaskGasProcTCB,
                     (void*)USB4000_Handle,
                     sizeof(USB4000_Handle),
                     OS_OPT_POST_FIFO,
                     &os_err);*/
    }
    
    return USBH_OK;
}

void USB4000_SetIntegTime(USB4000_HandleTypeDef *USB4000_Handle, INT32U IntegTime)
{
    USB4000_Handle->b_SetFlag = TRUE;
    USB4000_Handle->ul_SetIntegralTime = IntegTime;
}

/**
* @}
*/

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
