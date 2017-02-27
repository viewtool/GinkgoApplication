  /*
  ******************************************************************************
  * @file     : USB_GPIO_Test.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/19 11:53
  * @brief    : USB_GPIO_Test demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */
#include "stdafx.h"
#include "ControlGPIO.h"

void sleep(int i){
	int j;
	for (j=0;j<i;j++);
}

int _tmain(int argc, _TCHAR* argv[])
{
    int ret;
    // Scan connected device
    ret = VGI_ScanDevice(1);
    if (ret <= 0)
    {
        printf("No device connect!\n");
        return ret;
    }
    // Open device 
    ret = VGI_OpenDevice(VGI_USBGPIO, 0, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Open device error!\n");
        return ret;
    }
    
    // Set GPIO_0 to input 
    ret = VGI_SetInput(VGI_USBGPIO, 0, VGI_GPIO_PIN0);
    if (ret != ERR_SUCCESS)
    {
        printf("Set pin input error!\n");
        return ret;
    }
    // Get GPIO_0 status 
    uint16_t pin_value = 0;
	uint16_t pin_last_value = 1;
	while(1)
	{
		ret = VGI_ReadDatas(VGI_USBGPIO, 0, VGI_GPIO_PIN0, &pin_value);
		if (ret != ERR_SUCCESS)
		{
			printf("Get pin data error!\n");
			return ret;
		}
		else//有振动输出高电平，无振动输出低电平
		{
			if(pin_value==1&&pin_last_value==0)
			{
				printf("************\n");
			}				
			if(pin_value==0&&pin_last_value==1)
			{
				printf("No vibration\n");
			}
			pin_last_value=pin_value;					
		}
		
	}
    //关闭设备
    ret = VGI_CloseDevice(VGI_USBGPIO, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Close device error!\n");
        return ret;
    }
	return 0;
}
