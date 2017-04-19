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
    ָ��ֵ0��2��һ��Ϊ�������죬��ʱ������ǿ��������Ԥ���ȼ�Ϊһ����
    ָ��ֵ3��4��һ��Ϊ������������ʱ������ǿ�Ƚ�����Ԥ���ȼ�Ϊ������
    ָ��ֵ5��6��һ��Ϊ������������ʱ������ǿ�Ƚ�ǿ��Ԥ���ȼ�Ϊ������
    ָ��ֵ7��9��һ��Ϊ�������ƣ���ʱ������ǿ�Ⱥ�ǿ��Ԥ���ȼ�Ϊ�ļ���
    ָ��ֵ�ﵽ�򳬹�10����Ϊ�ļ����գ�������ǿ���ر�ǿ��Ԥ���ȼ�Ϊ�弶��
*/

/**
  * @brief  UVM30A ��������ʼ��
  * @param  CHx ͨ����  xȡֵ����Ϊ0~7 ,�ж���궨�� CH(x)����  1<<x
  * @retval SUCCESS-��ȡ���ݳɹ���FAILED-��ȡ����ʧ��
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
  * @brief  UVM30A ��ȡ������ָ��
  * @param  pIndex ��ȡ������ָ����ָ��
  * @retval SUCCESS-��ȡ���ݳɹ���FAILED-��ȡ����ʧ��
  */
enum Status UVM30A_GetIndex(uint8_t *pIndex){
    uint8_t ret = 0,i = 0;
    int8_t uvIndex = -1; // ������ָ��
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
        //mv = (adc_datas[0] * 3.3 * 1000/ 4095);//�ѻ�ȡ���ĵ�ѹת����mv
        //mv = (adc_datas[0] * 3300/ 4096);
        mv = (adc_datas[0] * 3300)>>12;//�ѻ�ȡ���ĵ�ѹת����mv
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