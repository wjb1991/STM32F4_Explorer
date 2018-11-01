#include "bsp.h"

#include "usbh_core.h"
#include "usbh_hid.h"
#include "usbh_hid_parser.h"
#include "usbh_template.h"

USBH_HandleTypeDef hUSBHost;

static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id);
#ifdef  OS_SUPPORT
int standalone(void)
#else
int main(void)
#endif
{
#ifndef  OS_SUPPORT

    HAL_Init();

    BSP_SystemClkCfg();
    
    HAL_Delay(100);
#endif  
    
    Bsp_Init();

    printf(">>DBG:       �������\r\n");

    /* Init HID Application */
    //HID_InitApplication();

    /* Init Host Library */
    USBH_Init(&hUSBHost, USBH_UserProcess, 0);

    /* ���֧���� */
    USBH_RegisterClass(&hUSBHost, USBH_USB4000_CLASS);
    //USBH_RegisterClass(&hUSBHost, USBH_HID_CLASS);
    
    /* Start Host Process */
    USBH_Start(&hUSBHost);

    __enable_irq();  
    
    /* Run Application (Blocking mode) */
    while (1)
    {
        BSP_Led1Toggle();
      
        /* USB Host Background task */

        USBH_Process(&hUSBHost);


        /* HID Menu Process */
        //HID_MenuProcess();

        /* HID Joystick */
        //HID_Joysticky();
    }
}

/**
  * @brief  User Process
  * @param  phost: Host Handle
  * @param  id: Host Library user message ID
  * @retval None
  */
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8_t id)
{
  switch(id)
  { 
  case HOST_USER_SELECT_CONFIGURATION:
    break;
    
  case HOST_USER_DISCONNECTION:

    break;
    
  case HOST_USER_CLASS_ACTIVE:

    break;
    
  case HOST_USER_CONNECTION:
    /* ��ʱ��� USB4000ʶ���� */
    printf("HOST_USER_CONNECTION\r\n");
    int i = 1000;
    while(i--){
        int j = 60000;
        while(j--);
    } 
    break;
    
  default:
    break; 
  }
}



