  /*
  ******************************************************************************
  * @file     : USB_I2C_AT24C02.cpp
  * @Copyright: ViewTool 
  * @Revision : ver 1.0
  * @Date     : 2014/12/19 10:42
  * @brief    : USB_I2C_AT24C02 demo
  ******************************************************************************
  * @attention
  *
  * Copyright 2009-2014, ViewTool
  * http://www.viewtool.com/
  * All Rights Reserved
  * 
  ******************************************************************************
  */

  /*
  Hardware Connection  (This is for your reference only)
  AT24C02        Ginkgo USB-I2C Adapter
  1.A0      <-->  GND(Pin19/Pin20)
  2.A1	    <-->  GND(Pin19/Pin20)
  3.A2      <-->  GND(Pin19/Pin20)
  4.GND     <-->  GND(Pin19/Pin20)
  5.SDA	    <-->  HI2C_SDA0(Pin8)
  6.SCL	    <-->  HI2C_SCL0 (Pin6)
  7.WP	    <-->  GND(Pin19/Pin20)
  8.VCC	    <-->  VCC(Pin1/Pin2)
  */

#include "stdafx.h"
#include "ControlI2C.h"
#include "MS561101BA.h"
#include "ControlGPIO.h"
#include "math.h"

#define MOVAVG_SIZE 32
MS561101BA baro = MS561101BA();

float movavg_buff[MOVAVG_SIZE];
int movavg_i=0;

const float sea_press = 1013.25;
float press, temp;

float getAltitude(float press, float temp) {
	float x=sea_press/press;
	float y=1/5.257;
	return ((pow(x, y) - 1.0) * (temp + 273.15)) / 0.0065;
}

void pushAvg(float val) {
	movavg_buff[movavg_i] = val;
	movavg_i = (movavg_i + 1) % MOVAVG_SIZE;
}

float getAvg(float * buff, int size) {
	float sum = 0.0;
	for(int i=0; i<size; i++) {
		sum += buff[i];
	}
	return sum / size;
}
int _tmain(int argc, _TCHAR* argv[])
{
	int ret,i;
	VII_INIT_CONFIG I2C_Config;
	VII_BOARD_INFO BoardInfo;
	uint8_t write_buffer[8]={0};
	uint8_t	read_buffer[8]={0};
	//Scan device
	ret = VII_ScanDevice(1);
	if(ret <= 0)
	{
		printf("No device connect!\n");
		return ret;
	}
    //Open device
    ret = VII_OpenDevice(VII_USBI2C, 0, 0);
    if (ret != ERR_SUCCESS)
    {
        printf("Open device error!\n");
        return ret;
    }
	//Get product information
	ret = VII_ReadBoardInfo(0,&BoardInfo);
    if (ret != ERR_SUCCESS){
        printf("Read board information error!\n");
        return ret;
    }else{
		printf("Product Name:%s\n",BoardInfo.ProductName);
		printf("Firmware Version:V%d.%d.%d\n",BoardInfo.FirmwareVersion[1],BoardInfo.FirmwareVersion[2],BoardInfo.FirmwareVersion[3]);
		printf("Hardware Version:V%d.%d.%d\n",BoardInfo.HardwareVersion[1],BoardInfo.HardwareVersion[2],BoardInfo.HardwareVersion[3]);
		printf("Serial Number:");
		for(i=0;i<12;i++){
			printf("%02X",BoardInfo.SerialNumber[i]);
		}
		printf("\n");
	}
    //Initializes the device
    I2C_Config.AddrType = VII_ADDR_7BIT;
    I2C_Config.ClockSpeed = 400000;
    I2C_Config.ControlMode = VII_HCTL_MODE;
    I2C_Config.MasterMode = VII_MASTER;
    I2C_Config.SubAddrWidth = VII_SUB_ADDR_NONE;
    ret = VII_InitI2C(VII_USBI2C, 0, 0, &I2C_Config);
    if (ret != ERR_SUCCESS)
    {
        printf("Initialize device error!\n");
        return ret;
    }
	
	ret = VGI_SetOutput(VGI_USBGPIO, 0, VGI_GPIO_PIN7 | VGI_GPIO_PIN8);
    if (ret != ERR_SUCCESS)
    {
        printf("Set pin output error!\n");
        return ret;
    }
    // Set GPIO_7 and GPIO_8 
    ret = VGI_SetPins(VGI_USBGPIO, 0, VGI_GPIO_PIN7 | VGI_GPIO_PIN8);
    if (ret != ERR_SUCCESS)
    {
        printf("Set pin high error!\n");
        return ret;
    }
    
	// Embedded adventures module CSB is VCC so HIGH
	baro.init(MS561101BA_ADDR_CSB_HIGH); 
	//Sleep(100);
	// populate movavg_buff before starting loop
	for(int i=0; i<MOVAVG_SIZE; i++) {
		movavg_buff[i] = baro.getPressure(MS561101BA_OSR_4096);
	}

	while(1){
		temp = baro.getTemperature(MS561101BA_OSR_4096);
		printf("temp: %.2f\n",temp);
		pushAvg(baro.getPressure(MS561101BA_OSR_4096));
		press = getAvg(movavg_buff, MOVAVG_SIZE);
		printf("degC pres: %.2f\n",press);
		printf("mbar altitude: %.2f\n",getAltitude(press, temp));
		Sleep(1000);
	}
	return 0;
}
