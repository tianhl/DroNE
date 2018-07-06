// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2018
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2018 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DecodeRawDataBase.cc
//  
//
//  Created by Haolai TIAN 6th July 2018.
//
//
#include "DataSvc/DecodeRawDataBase.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;


//====================
void DecodeRawDataBase::U32toU14U12U6(uint32_t *Input, uint32_t *Output_1, uint32_t *Output_2, uint32_t *Output_3){
	*Output_1 = (((((uint32_t)(*Input))&0xFFFFFFFF)>>18)&0x3FFF);
	*Output_2 = (((((uint32_t)(*Input))&0x3FFFF)>>6)&0xFFF);
	*Output_3 =   (((uint32_t)(*Input))&0x3F);
};

void DecodeRawDataBase::U24to2U12(uint32_t *Input, uint32_t *Output_1, uint32_t *Output_2){
	*Output_1 = (((((uint32_t)(*Input))&0xFFFFFF)>>12)&0xFFF);
	*Output_2 =   (((uint32_t)(*Input))&0xFFF);
};

void DecodeRawDataBase::U8to2U4(uint32_t *Input, uint32_t *Output_1, uint32_t *Output_2){
	*Output_1 = (((((uint32_t)(*Input))&0xFF)>>4)&0xF);
	*Output_2 =   (((uint32_t)(*Input))&0xF);
};

void DecodeRawDataBase::U32toU12U20(uint32_t *Input, uint32_t *Output_1, uint32_t *Output_2){
	*Output_1 = ((((uint32_t)(*Input))>>20)&0xFFF);
	*Output_2 =  (((uint32_t)(*Input))&0xFFFFF);
};

//====================
void DecodeRawDataBase::U32_1st(uint8_t *Input, uint32_t *Output){
	*Output = (((((uint32_t)(*Input))&0xFF)<<24)&0xFF000000);
};

void DecodeRawDataBase::U32_2ed(uint8_t *Input, uint32_t *Output){
	*Output |= (((((uint32_t)(*Input)&0xFF))<<16)&0xFF0000);
};

void DecodeRawDataBase::U32_3rd(uint8_t *Input, uint32_t *Output){
	*Output |= (((((uint32_t)(*Input)&0xFF))<<8)&0xFF00);
};

void DecodeRawDataBase::U32_4th(uint8_t *Input, uint32_t *Output){
	*Output |= ((((uint32_t)(*Input)&0xFF))&0xFF);
};

//====================
