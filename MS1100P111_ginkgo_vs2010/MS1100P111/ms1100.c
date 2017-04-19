  /*
  ******************************************************************************
  * @file     : ms1100.c
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/06/24 12:44
  * @brief    : MS1100 DRIVER
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
#include "MS1100.h"
#include <stdio.h>
#include <stdint.h>
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
static uint8_t Voltage2PPM(uint8_t airSensor, uint16_t voltage);

/**
  * @brief  MS1100 传感器初始化
  * @param  CHx 通道号  x取值区间为0~7 ,有定义宏定义 CH(x)或者  1<<x
  * @retval SUCCESS-读取数据成功，FAILED-读取数据失败
  */
enum Status MS1100_Init(uint8_t CHx){
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
  * @brief  获取 ms1100采集到的参数 
  * @param  airSensor:PA为甲醛，TL为甲苯  ,pIndex: 测试的气体指数
  * @retval SUCCESS-读取数据成功，FAILED-读取数据失败
  */
enum Status MS1100_GetIndex(uint8_t airSensor, uint8_t *pIndex){
    int ret;
    uint16_t adc_datas[8] = {0};
    float datax = 0;
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
        datax = (adc_datas[0] * 3.3 / 4095);
#ifdef _DEBUG
        printf("sampling data is %fmv\n",adc_datas[0]*3.3/4095);
        printf("sampling data is %fmv\n",adc_datas[0]*3.3/4095*1000);
#endif // _DEBUG
        // Voltage2PPM函数的表里面的数据统一除以10，so下面是 *100 不是*1000
        *pIndex = Voltage2PPM(airSensor, adc_datas[0]*3.3/4095*100);
    }
    return SUCCESS;
}

/**
  * @brief  电压转换为ppm
  * @param  airSensor:PA为甲醛，TL为甲苯  ,voltage: 要转换的电压
  * @retval ppm 浓度
  */
uint8_t Voltage2PPM(uint8_t airSensor, uint16_t voltage){
    uint8_t i;// 用于循环计数
    uint8_t ppm = 255;
    uint16_t ppm2VoltageValueLow[2][11] = {
        {64,210,238,257,271,282,292,301,309,316,322},
        {60,327,347,360,369,377,383,388,393,398,401}
    };
    uint16_t ppm2VoltageValueHigh[2][14] ={
        {322,349,369,387,402,415,427,438,448,457,466,474,482,490},
        {401,417,429 ,438,446,453,459,465,470,475,479,483,486,490}
    };
    uint16_t vRange = 0;//差值：范围  
    uint16_t vCalc = 0; //差值：要计算的数

    if (voltage <= ppm2VoltageValueLow[airSensor][10])
    {
        for (i = 0; i < 11; i++)
        {
            if (voltage <= ppm2VoltageValueLow[airSensor][i]) 
            {
                ppm = i;
#ifdef _DEBUG
                printf("ppm is %d\n",ppm);
#endif // _DEBUG
                break;
            }
        }
    }else if (voltage > ppm2VoltageValueLow[airSensor][10])
    {
        for (i = 0; i < 14; i++)
        {
            if (voltage <= ppm2VoltageValueHigh[airSensor][i]) 
            {
                //ppm =10 + (i-1) * 5;
                ppm = 5 + i * 5;
                vCalc  = voltage - ppm2VoltageValueHigh[airSensor][i-1];
                if (vCalc)
                {
                    vRange = ppm2VoltageValueHigh[airSensor][i] - ppm2VoltageValueHigh[airSensor][i-1];
                    ppm   += (vCalc*5)/vRange;

                }
#ifdef _DEBUG
                    printf("PPM is %d\n",ppm);
#endif // _DEBUG
                break;
            }

        }
    }
    return ppm;
}




