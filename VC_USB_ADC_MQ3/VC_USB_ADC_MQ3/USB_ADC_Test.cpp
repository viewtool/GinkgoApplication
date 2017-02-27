  /*
  ******************************************************************************
  * @file     : USB_ADC_Test.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/29 9:23
  * @brief    : USB_ADC_Test demo
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
#include "ControlADC.h"


int _tmain(int argc, _TCHAR* argv[])
{
    int ret;
	float c;
	uint8_t SamplingCnt = 0;
    //Scan connected device
    ret = VAI_ScanDevice(1);
    if (ret <= 0)
    {
        printf("No device connect!\n");
        return ret;
    }
    // Open device
    ret = VAI_OpenDevice(VAI_USBADC, 0, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Open device error!\n");
        return ret;
    }
    // Initialize ADC_CH0 channel
    ret = VAI_InitADC(VAI_USBADC, 0, VAI_ADC_CH0,1000);// Cycle can be set to 0 if each channel transfer a byte of data at a time.
    if (ret != ERR_SUCCESS)
    {
        printf("Initialize ADC error!\n");
        return ret;
    }
    // Get voltage values of ADC_CH0 
	uint16_t adc_datas[4096] = {0};
    
    while(1)
	{
		SamplingCnt++;
		ret = VAI_ReadDatas(VAI_USBADC, 0, 1, adc_datas);
		if (ret != ERR_SUCCESS)
		{
			printf("Read ADC data error!\n");
			return ret;
		}
		// 10ms * 100 =1s in theory,but there is other time-consuming in practice
		if( SamplingCnt==250 )
		{
			SamplingCnt = 0;
			c=(adc_datas[0]*3.3)/4095;
			if(c<0.3)
				printf("C=0ppm\n");
			else
			{
				c=(c-0.3)*20+25.0;
				printf("C = %.3fppm\n" ,c);
			}
		}
	}
 

    // close device
    ret = VAI_CloseDevice(VAI_USBADC, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Close device error!\n");
        return ret;
    }
	return 0;
}

