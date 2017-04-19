  /*
  ******************************************************************************
  * @file     : MS1100.h
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
#include <stdint.h>
/* Private typedef -----------------------------------------------------------*/
enum Status
{
	FAILED = 0,
	SUCCESS,
};
enum AirSensor
{
    PA = 0,  //  ¼×È© 
    TL,      //  ¼×±½
};
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

#ifndef __MS1100_H
#define __MS1100_H

#define CH(x) (((x) < 8)&&((x) > 0)? (1<<(x)):(1<<0))

enum Status MS1100_Init(uint8_t CHx);
enum Status MS1100_GetIndex(uint8_t index, uint8_t *pIndex);

#endif