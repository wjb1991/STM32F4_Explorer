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

static USBH_StatusTypeDef USBH_USB4000_QueryInformation(USBH_HandleTypeDef *phost);

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
    
    
    USBH_UsrLog ("ClassProcess");

    USBH_USB4000_SetIntegralTime(phost,USB4000_Handle->ul_SetIntegralTime);
    
    USBH_USB4000_QueryInformation(phost);
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
    
    while(phost->gState == HOST_CLASS)
    {
        OS_ERR os_err;
        
        uint32_t delay_time = USB4000_Handle->ul_IntegralTime / 1000;
        
        /* ��ȡ������Ҫ20ms���� ���50ms��ȡһ�Ź��� �����20+ms ������������ */
        if(delay_time < 50)
        {
            delay_time = 50;
        }
        
        OSTimeDlyHMSM(0u, 0u, 0u, delay_time,
                      OS_OPT_TIME_HMSM_STRICT | OS_OPT_TIME_PERIODIC,/* ����ģʽ */
                      &os_err);
        
        if(USB4000_Handle->b_SetFlag == TRUE)
        {
            USBH_USB4000_SetIntegralTime(phost,USB4000_Handle->ul_SetIntegralTime);
            USB4000_Handle->b_SetFlag = FALSE;
        }
        else
        {
            USBH_USB4000_GetSpectrum(phost); 
            USBH_USB4000_GetStatus(phost);        
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
    INT16U uin_Len;
    OS_ERR os_err;

    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 
    uint8_t buff[64] = {0};
    
    /* ��ʱ1ms �ȵ��¸�sof �ڲ��� */
    //USB4000_Handle->sof_signal = FALSE;
    //while(USB4000_Handle->sof_signal == FALSE){}
    HAL_Delay(1);
    
    /* ���Ͳ�ѯ����ȴ� */
    buff[0] = 0xFE;
    USBH_BulkSendData(phost,buff,1,USB4000_Handle->OutPipe1,0);
    OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
    if(os_err != OS_ERR_NONE)
    {
        USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
        USBH_UsrLog ("����ʧ��");
        return USBH_FAIL;
    }

    /* ��ʱ1ms �ȵ��¸�sof �ڶ�ȡ */
    //USB4000_Handle->sof_signal = FALSE;
    //while(USB4000_Handle->sof_signal == FALSE){}
    HAL_Delay(1);
    
    
    /* ����������� */ 
    USBH_BulkReceiveData(phost, buff, 64, USB4000_Handle->InPipe1); 
    OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);    
    
    if(os_err != OS_ERR_NONE)
    {
        USBH_ClrFeature(phost,USB4000_Handle->InEp1);
        USBH_UsrLog ("���ճ�ʱ");
        return USBH_FAIL;
    }
    
    /* ���ճɹ� */

    INT32U t;
    Bsp_Uint8ToUint32(&buff[2],&t,eLeToLe);
    USB4000_Handle->ul_IntegralTime = t;
    
    if( buff[14] !=0 )
    {
        USB4000_Handle->b_HighSpeed = TRUE;
    }
    else
    {
        USB4000_Handle->b_HighSpeed = FALSE;        //FullSpeed
    }    

    return USBH_OK;
}

static USBH_StatusTypeDef USBH_USB4000_SetIntegralTime(USBH_HandleTypeDef *phost,INT32U IntegTime)
{
    INT16U uin_Len;
    INT32U ul_IntegTime;
    OS_ERR os_err;
    
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 

    /* ��ʱ1ms �ȵ��¸�sof �ڲ��� */
    //USB4000_Handle->sof_signal = FALSE;
    //while(USB4000_Handle->sof_signal == FALSE){}
    HAL_Delay(1);
    
    do
    {
        ul_IntegTime = IntegTime;
        
        uint8_t buff[64] = {0};
        buff[0] = 0x02;
        Bsp_Uint32ToUint8(&ul_IntegTime,&buff[1],eLeToLe);
        buff[4] = 0x00;
        USBH_BulkSendData(phost,buff,5,USB4000_Handle->OutPipe1,0);
        
        OSTaskQPend(0,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
        if(os_err != OS_ERR_NONE)
        {
            USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
            USBH_UsrLog ("����ʧ��");
            //return USBH_FAIL;
        }
        
        USBH_USB4000_GetStatus(phost);
        
        HAL_Delay(1);
        
    }while(USB4000_Handle->ul_IntegralTime != ul_IntegTime);
    
    return USBH_OK;
}

static USBH_StatusTypeDef USBH_USB4000_GetSpectrum(USBH_HandleTypeDef *phost)
{
    INT16U uin_Len;
    OS_ERR os_err;
  
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 
    uint8_t buff[64] = {0};

    /* ��ʱ1ms �ȵ��¸�sof �ٲ��� */
    //USB4000_Handle->sof_signal = FALSE;
    //while(USB4000_Handle->sof_signal == FALSE){}
    HAL_Delay(1);
    
    
    buff[0] = 0x09;
    USBH_BulkSendData(phost,buff,1,USB4000_Handle->OutPipe1,0);
    USBH_UsrLog ("�������");
    
    OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
    if(os_err != OS_ERR_NONE)
    {
        USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
        USBH_UsrLog ("����ʧ��");
        return USBH_FAIL;
    }
    
    /* ��ʱ1ms �ȵ��¸�sof �ٶ�ȡ */
    //USB4000_Handle->sof_signal = FALSE;
    //while(USB4000_Handle->sof_signal == FALSE){}
    HAL_Delay(1);
    
    /* ������ȡ */
    USB4000_Handle->rx_count = 0;
    while(1)
    {
        USBH_BulkReceiveData(phost, buff, 64, USB4000_Handle->InPipe2);
        
        OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
        if(os_err != OS_ERR_NONE)
        {
            USBH_ClrFeature(phost,USB4000_Handle->InEp2);
            USBH_UsrLog ("������׳�ʱ");
            return USBH_FAIL;        
        }

        if(buff[0] == 0x69 && USB4000_Handle->rx_count == 120)
        {
            /* ���ܳɹ� pack0~ pack120 rx = 121*/
            USBH_USB4000_ProcessSpectrum(phost);
            
            USBH_UsrLog ("���������� PackCount = %d", USB4000_Handle->rx_count);
            USB4000_Handle->rx_count = 0; 
            break;
        }
        else if (USB4000_Handle->rx_count >= 121)
        {
            /* 1000�� 1s���ܳ�ʱ */
            USBH_UsrLog ("������׳�������");
            return USBH_FAIL;
        }
        else
        {
            uint16_t offset = (USB4000_Handle->rx_count) * 64;
            uint8_t* puc_buff = (uint8_t*)USB4000_Handle->pin_Spectrum;
              
            for(int i = 0 ; i < 64; i++)
            {
                puc_buff[offset+i] = buff[i];
            }
            
            USB4000_Handle->rx_count++; 
        }
    }
    return USBH_OK;
}

static USBH_StatusTypeDef USBH_USB4000_QueryInformation(USBH_HandleTypeDef *phost)
{
    INT16U uin_Len;
    OS_ERR os_err;
  
    USB4000_HandleTypeDef *USB4000_Handle =  (USB4000_HandleTypeDef *) phost->pActiveClass->pData; 
    uint8_t buff[64] = {0};
    
    for(uint8_t i = 0; i < 31; i++)
    {
        buff[0] = 0x05;
        buff[1] = i;
        
        /* ��ʱ1ms �ȵ��¸�sof �ڲ��� */
        //USB4000_Handle->sof_signal = FALSE;
        //while(USB4000_Handle->sof_signal == FALSE){}
        HAL_Delay(1);
    
        
        /* ���Ͳ�ѯ���� */
        USBH_BulkSendData(phost,buff,2,USB4000_Handle->OutPipe1,0);
        OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);
        if(os_err != OS_ERR_NONE)
        {
            USBH_ClrFeature(phost,USB4000_Handle->OutEp1);
            USBH_UsrLog ("����ʧ��");
            return USBH_FAIL;
        }
        
        /* ��ʱ1ms �ȵ��¸�sof �ڶ�ȡ */
        //USB4000_Handle->sof_signal = FALSE;
        //while(USB4000_Handle->sof_signal == FALSE){}
        HAL_Delay(1);
    
        
        /* ����������� */ 
        USBH_BulkReceiveData(phost, buff, 64, USB4000_Handle->InPipe1); 
        OSTaskQPend(100,OS_OPT_PEND_BLOCKING,&uin_Len,NULL,&os_err);    
      
        if(os_err != OS_ERR_NONE)
        {
            USBH_ClrFeature(phost,USB4000_Handle->InEp1);
            USBH_UsrLog ("���ճ�ʱ");
            return USBH_FAIL;
        }

        if(buff[1] >=1 && buff[1] <= 4)
        {
            uint8_t j = buff[1]-1;
            USB4000_Handle->alf_WlcCoeff[j] = atof((const char *)&buff[2]);
            USBH_UsrLog ("%d�ײ���У������ = %04f",j,USB4000_Handle->alf_WlcCoeff[j] );
        }
       
        if(buff[1] >=6 && buff[1] <= 13)
        {
            uint8_t j = buff[1]-6;
            USB4000_Handle->alf_NlcCoeff[j] = atof((const char *)&buff[2]);
            USBH_UsrLog ("%d�׷�����У������ = %04f",j,USB4000_Handle->alf_NlcCoeff[j] );
        }
        
        if(buff[1] == 14)
        {
            USBH_UsrLog ("������ ����");
        }
    }
    
    return USBH_OK;
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
    
        /* ֪ͨ�ϲ� ����ģ��*/
        OS_ERR  os_err;
        OSTaskQPost (&TaskGasProcTCB,
                     (void*)USB4000_Handle,
                     sizeof(USB4000_Handle),
                     OS_OPT_POST_FIFO,
                     &os_err);
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
