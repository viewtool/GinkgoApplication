  /*
  ******************************************************************************
  * @file     : main.c
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2015/06/23 13:47
  * @brief    : UVM30A demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2015, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

#include "uvm30a.h"
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
    //float pIx[1]={0};
    uint8_t uvIndex = 0;

    UVM30A_Init(CH(0));
    while(1){
        UVM30A_GetIndex(&uvIndex);
        printf("%d UV Index\n",uvIndex);
#if defined(__GNUC__)
        sleep(1);
#endif
#if defined(_MSC_VER)
        Sleep(1000);
#endif
        
    }

}
