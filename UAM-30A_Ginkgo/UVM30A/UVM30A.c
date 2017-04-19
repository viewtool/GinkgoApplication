  /*
  ******************************************************************************
  * @file     : UVM30A.c
  * @Copyright: ViewTool 
  * @Revision : ver 1.1
  * @Date     : 2015/06/23 13:47
  * @brief    : UVM30A DRIVER
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2015, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

/* Private macro -------------------------------------------------------------*/
/* Includes ------------------------------------------------------------------*/
#include "ControlADC.h"
#include "UVM30A.h"
#include <stdio.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

/*
    指数值0到2，一般为阴或雨天，此时紫外线强度最弱，预报等级为一级；
    指数值3到4，一般为多云天气，此时紫外线强度较弱，预报等级为二级；
    指数值5到6，一般为少云天气，此时紫外线强度较强，预报等级为三级；
    指数值7到9，一般为晴天无云，此时紫外线强度很强，预报等级为四级；
    指数值达到或超过10，多为夏季晴日，紫外线强度特别强，预报等级为五级。
*/

/**
  * @brief  UVM30A 传感器初始化
  * @param  CHx 通道号  x取值区间为0~7 ,有定义宏定义 CH(x)或者  1<<x
  * @retval SUCCESS-读取数据成功，FAILED-读取数据失败
  */
enum Status UVM30A_Init(uint8_t CHx){
    int ret;
    ret = VAI_ScanDevice(1);
    if (ret <= 0)
    {
#ifdef _DEBUG
        printf("No device connect!\n");
#endif // _DEBUG
        return FAILED;
    }
    ret = VAI_OpenDevice(VAI_USBADC, 0, 0);
    if (ret != ERR_SUCCESS)
    {
#ifdef _DEBUG
        printf("Open device error!\n");
#endif // _DEBUG        
        return FAILED;
    }
    // Initialize ADC_CHx channel
    ret = VAI_InitADC(VAI_USBADC, 0, CHx,0);
    if (ret != ERR_SUCCESS)
    {
#ifdef _DEBUG
        printf("Initialize ADC error!\n");
#endif // _DEBUG
        return FAILED;
    }
    return SUCCESS;
}

/**
  * @brief  UVM30A 获取紫外线指数
  * @param  pIndex 获取紫外线指数的指针
  * @retval SUCCESS-读取数据成功，FAILED-读取数据失败
  */
enum Status UVM30A_GetIndex(uint8_t *pIndex){
    uint8_t ret = 0,i = 0;
    int8_t uvIndex = -1; // 紫外线指数
    uint16_t adc_datas[8] = {0};
    uint16_t uvIndexValue[12] = {50, 227,318,408,503,606,696,795,881,976,1079,1170};
    uint16_t mv = 0;

    ret = VAI_ReadDatas(VAI_USBADC, 0, 1, adc_datas);
    if (ret != ERR_SUCCESS)
    {
#ifdef _DEBUG
        printf("Read ADC data error!\n");
#endif // _DEBUG
        return FAILED;
    }
    else
    {
        //mv = (adc_datas[0] * 3.3 * 1000/ 4095);//把获取到的电压转换成mv
        //mv = (adc_datas[0] * 3300/ 4096);
        mv = (adc_datas[0] * 3300)>>12;//把获取到的电压转换成mv
        if (mv > 1170)
        {
            mv = 1170;
        }
#ifdef _DEBUG
        printf("sampling data is %dmv\n",mv);
#endif // _DEBUG
        for (i = 0; i < 12; i++)
        {
            if (mv <= uvIndexValue[i]) 
            {
                uvIndex = i;
                break;
            }
        }
        *pIndex = uvIndex;
    return SUCCESS;
    }
}