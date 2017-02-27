/*
MS5611-01BA.cpp - Interfaces a Measurement Specialities MS5611-01BA with Arduino
See http://www.meas-spec.com/downloads/MS5611-01BA01.pdf for the device datasheet

Copyright (C) 2011 Fabio Varesano <fvaresano@yahoo.it>

Development of this code has been supported by the Department of Computer Science,
Universita' degli Studi di Torino, Italy within the Piemonte Project
http://www.piemonte.di.unito.it/


This program is free software: you can redistribute it and/or modify
it under the terms of the version 3 GNU General Public License as
published by the Free Software Foundation.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.

*/

#include "MS561101BA.h"
#include "stdio.h"
#define EXTRA_PRECISION 5 // trick to add more precision to the pressure and temp readings

MS561101BA::MS561101BA() {
	;
}

void MS561101BA::init(uint8_t address) {  
	_addr =  address;
	reset(); // reset the device to populate its internal PROM registers
	Sleep(1000); // some safety time 
	readPROM(); // reads the PROM into object variables for later use
}

float MS561101BA::getPressure(uint8_t OSR) {
	// see datasheet page 7 for formulas
	int64_t dT   = getDeltaTemp(OSR);
	int64_t off  = (((int64_t)_C[1]) << 16) + ((_C[3] * dT) >> 7);
	int64_t sens = (((int64_t)_C[0]) << 15) + ((_C[2] * dT) >> 8);
	return ((((rawPressure(OSR) * sens) >> 21) - off) >> (15-EXTRA_PRECISION)) / ((1<<EXTRA_PRECISION) * 100.0);
}

float MS561101BA::getTemperature(uint8_t OSR) {
	// see datasheet page 7 for formulas
	return ((1<<EXTRA_PRECISION)*2000l + ((getDeltaTemp(OSR) * _C[5]) >> (23-EXTRA_PRECISION))) / ((1<<EXTRA_PRECISION) * 100.0);
}

int64_t MS561101BA::getDeltaTemp(uint8_t OSR) {
	return rawTemperature(OSR) - (((int32_t)_C[4]) << 8);
}

int32_t MS561101BA::rawPressure(uint8_t OSR) {
	return doConversion(MS561101BA_D1 + OSR);
}

int32_t MS561101BA::rawTemperature(uint8_t OSR) {
	return doConversion(MS561101BA_D2 + OSR);
}


unsigned long MS561101BA::doConversion(uint8_t command) {
	unsigned long conver=0;
	uint8_t conversion[3] = {0};
	int ret;
	uint8_t readCommand=0;
	// see page 11 of the datasheet 
	// initialize pressure conversion
	ret=VII_WriteBytes(VII_USBI2C, 0, 0, _addr, 0x00, &command, 1);
	if (ret != ERR_SUCCESS){
		printf("Conversion Write command error!\n");
		return ret;
	}
	Sleep(10); // the conversion will take a time <= 9.04 ms to have the output ready
			  // TODO: make the delay dependant on the OSR requested in the command  
	ret=VII_WriteBytes(VII_USBI2C, 0, 0, _addr, 0x00, &readCommand, 1);
	if (ret != ERR_SUCCESS){
		printf("Send read command error!\n");
		return ret;
	}

	ret = VII_ReadBytes(VII_USBI2C, 0, 0, _addr, 0x00, conversion, MS561101BA_D1D2_SIZE);
	if (ret != ERR_SUCCESS){
		printf("Conversion Read data error!\n");
		return ret;
	}else{		
		conver= conversion[0] * 65536 + conversion[1] * 256 + conversion[2];
		//printf("Read Data:%d\n",conver);
	}  
	return conver;
}


/**
* Reads factory calibration and store it into object variables.
*/
int MS561101BA::readPROM() {
	int ret;
	uint8_t command,receive[2];
	for (int i=0;i<MS561101BA_PROM_REG_COUNT;i++) {
		command=MS561101BA_PROM_BASE_ADDR + (i * MS561101BA_PROM_REG_SIZE);
		ret=VII_WriteBytes(VII_USBI2C, 0, 0, _addr, 0x00, &command, 1);
		if (ret != ERR_SUCCESS){
			printf("Prom Write command error!\n");
			return ret;
		}
		ret = VII_ReadBytes(VII_USBI2C, 0, 0, _addr, 0x00, receive, MS561101BA_PROM_REG_SIZE);
		if (ret != ERR_SUCCESS){
			printf("Prom Read data error!\n");
			return ret;
		}else{			
			_C[i] = receive[0] << 8 | receive[1];
			//printf("Read Data:%d\n",_C[i]);
		}
	}
	return 0;
}


/**
* Send a reset command to the device. With the reset command the device
* populates its internal registers with the values read from the PROM.
*/
void MS561101BA::reset() {
	int ret;
	uint8_t resetCommand = MS561101BA_RESET;
	ret=VII_WriteBytes(VII_USBI2C, 0, 0, _addr, 0x00, &resetCommand, 1);
	if (ret != ERR_SUCCESS){
		printf("Reset error! ret=%d\n",ret);
	}
}
