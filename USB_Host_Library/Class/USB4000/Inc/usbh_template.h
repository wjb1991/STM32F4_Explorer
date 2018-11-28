/**
  ******************************************************************************
  * @file    usbh_template.h
  * @author  MCD Application Team
  * @version V3.2.2
  * @date    07-July-2015
  * @brief   This file contains all the prototypes for the usbh_template.c
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */ 

/* Define to prevent recursive  ----------------------------------------------*/
#ifndef __USBH_USB4000_H
#define __USBH_USB4000_H

#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "usbh_core.h"


/* States for USB4000 State Machine */
typedef enum
{
  USB4000_INIT= 0,  
  USB4000_IDLE,
  USB4000_SEND_DATA,
  USB4000_WAIT_SEND,
  USB4000_GET_DATA,  
  USB4000_WAIT_GET,
  USB4000_SYNC,     
  USB4000_POLL,
  USB4000_ERROR,
}
USB4000_StateTypeDef;  
/* Structure for USB4000 process */
typedef struct _USB4000_Process
{
    /* ����������� �˵�*/
    uint8_t              OutPipe1;
    uint8_t              OutEp1; 
    /* ������������ �˵�*/
    uint8_t              InPipe1; 
    uint8_t              InEp1;
    /* ��������˵� */
    uint8_t              InPipe2; 
    uint8_t              InEp2;
    /* ���ٴ����ʹ�õĶ˵� */
    uint8_t              InPipe6; 
    uint8_t              InEp6;
   
    /* ����״̬ */
    __IO uint8_t         sof_signal;
    uint32_t             rx_count;

    /* ���Ʋ���-����ʱ�� us */
    uint8_t              b_SetFlag;
    uint32_t             ul_SetIntegralTime;
    
    /* 5-15����*/
    uint16_t             auin_EdcIndexs[11];        
    /* Wavelength Calibration Coefficient */
    double               alf_WlcCoeff[4];
    /* non-linearity correction coefficient */
    double               alf_NlcCoeff[8];
    
    uint8_t              uch_NlcOrder;
    
    /* �������� */
    int16_t*             pin_Spectrum;              //��������
    int32_t*             pl_SumSpectrum;            //��͹���
    
    double*              plf_ProcessSpectrum;       //�����Ĺ���
    double*              plf_WaveLenth;             //��������
    uint16_t             uin_Pixels;                //���ظ���
    
    uint8_t              uch_ScansToAverage;        //���ɨ��ƽ��
    uint8_t              uch_ScansConut;            //ɨ�����      
    uint8_t              uch_Boxcar;                //����ƽ��
    
    uint32_t             ul_IntegralTime;           //����ʱ��

    uint8_t              b_EdcEnable;               //�Ƿ���EDC 
    uint8_t              b_NlcEnable;               //�Ƿ���NLC �����Բ���
    
    uint8_t              b_Open;
    uint8_t              b_IsConnect;
    uint8_t              b_HighSpeed;               
    USB4000_StateTypeDef e_State;   
}
USB4000_HandleTypeDef;


#define USB_USB4000_CLASS              0xFF
   

extern USBH_ClassTypeDef  USB4000_Class;
#define USBH_USB4000_CLASS    &USB4000_Class

extern USB4000_HandleTypeDef   USB4000;

void USB4000_SetIntegTime(USB4000_HandleTypeDef *USB4000_Handle, INT32U IntegTime);

/**
* @}
*/ 

#ifdef __cplusplus
}
#endif

#endif /* __USBH_USB4000_H */

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 

/**
* @}
*/ 
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/

