  /*
  ******************************************************************************
  * @file     : main.c
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/06/24 12:43
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

#include "MS1100.h"
#include "ControlADC.h"
#include <stdint.h>
#include <stdio.h>

#if defined(__GNUC__)
#include <unistd.h>
#endif
#if defined(_MSC_VER)
#include <Windows.h>
#endif

int main(void)
{
    uint8_t paIndex = 0; // ¼×È©Ö¸Êý

    MS1100_Init(CH(0));
    while(1){
        MS1100_GetIndex(PA,&paIndex);
        printf("%d\n",paIndex);
#if defined(__GNUC__)
        sleep(1);
#endif
#if defined(_MSC_VER)
        Sleep(1000);
#endif
    }

}
