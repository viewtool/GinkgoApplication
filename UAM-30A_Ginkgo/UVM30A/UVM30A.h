  /*
  ******************************************************************************
  * @file     : UVM30A.h
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/06/23 13:51
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
#include <stdint.h>
/* Private typedef -----------------------------------------------------------*/
enum Status
{
	FAILED = 0,
	SUCCESS,
};
/* Private define ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/

#ifndef __UVM30A_H
#define __UVM30A_H

#define CH(x) (((x) < 8)&&((x) > 0)? (1<<(x)):(1<<0))

enum Status UVM30A_Init(uint8_t CHx);
enum Status UVM30A_GetIndex(uint8_t *pIndex);

#endif