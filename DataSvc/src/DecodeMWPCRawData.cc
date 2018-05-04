// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2018
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2018 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DecodeMWPCRawData.cc
//  
//
//  Created by Haolai TIAN 4th May 2018.
//
//
#include "DataSvc/DecodeMWPCRawData.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;


DecodeMWPCRawData::DecodeMWPCRawData()
: m_bcount(0), m_status(PExecReady)
{
};

//====================
void DecodeMWPCRawData::U32toU14U12U6(uint32_t *Input, uint32_t *Output_1, uint32_t *Output_2, uint32_t *Output_3){
	*Output_1 = (((((uint32_t)(*Input))&0xFFFFFFFF)>>18)&0x3FFF);
	*Output_2 = (((((uint32_t)(*Input))&0x3FFFF)>>6)&0xFFF);
	*Output_3 =   (((uint32_t)(*Input))&0x3F);
};

void DecodeMWPCRawData::U24to2U12(uint32_t *Input, uint32_t *Output_1, uint32_t *Output_2){
	*Output_1 = (((((uint32_t)(*Input))&0xFFFFFF)>>12)&0xFFF);
	*Output_2 =   (((uint32_t)(*Input))&0xFFF);
};

void DecodeMWPCRawData::U8to2U4(uint32_t *Input, uint32_t *Output_1, uint32_t *Output_2){
	*Output_1 = (((((uint32_t)(*Input))&0xFF)>>4)&0xF);
	*Output_2 =   (((uint32_t)(*Input))&0xF);
};

void DecodeMWPCRawData::U32toU12U20(uint32_t *Input, uint32_t *Output_1, uint32_t *Output_2){
	*Output_1 = ((((uint32_t)(*Input))>>20)&0xFFF);
	*Output_2 =  (((uint32_t)(*Input))&0xFFFFF);
};

//====================
void DecodeMWPCRawData::U32_1st(uint8_t *Input, uint32_t *Output){
	*Output = (((((uint32_t)(*Input))&0xFF)<<24)&0xFF000000);
};

void DecodeMWPCRawData::U32_2ed(uint8_t *Input, uint32_t *Output){
	*Output |= (((((uint32_t)(*Input)&0xFF))<<16)&0xFF0000);
};

void DecodeMWPCRawData::U32_3rd(uint8_t *Input, uint32_t *Output){
	*Output |= (((((uint32_t)(*Input)&0xFF))<<8)&0xFF00);
};

void DecodeMWPCRawData::U32_4th(uint8_t *Input, uint32_t *Output){
	*Output |= ((((uint32_t)(*Input)&0xFF))&0xFF);
};

//====================
void DecodeMWPCRawData::Decode_PulseHdr00(uint8_t *Input){
	if(0xFA == *Input) m_status = PulseHdr00;
	else m_status = PExecReady;
};

uint32_t DecodeMWPCRawData::Decode_PulseHit00(uint8_t *Input){
	m_return = 0x0;
	if(0xFB == *Input) m_status = PulseEnd00;
	else{
		m_return = (uint32_t)(*Input);
                //uint32_t value1, value2;
                //U8to2U4(&m_return, &value1, &value2);
                m_status = PulseHit00;
                //else m_status = PExecError; 
	}
	return m_return;
};

void DecodeMWPCRawData::Decode_PulseEnd00(uint8_t *Input){
	if(0xFB == *Input)m_status = PulseEnd00;
	else m_status = PExecError;
	//std::cout << "Cannot find End: " << std::endl;
	//printf("     Input %x\n",*Input);
};

uint32_t DecodeMWPCRawData::Decode_RawDataSegment(uint8_t *ReadRawData){
	//printf("%x\n", *ReadRawData);

	switch(m_status){
		//================ Hdr ================
		case PExecReady:
			Decode_PulseHdr00(ReadRawData);
			break;
		case PulseEnd07:
			Decode_PulseHdr00(ReadRawData);
			break;
		case PulseHdr00:
			// instrument id
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr01;
			break;
		case PulseHdr01:
			// detector id
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr02;
			break;
		case PulseHdr02:
			// module id
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr03;
			break;
		case PulseHdr03:
			// run
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr04;
			break;
		case PulseHdr04:
			// frame 
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr05;
			break;
		case PulseHdr05:
			// version
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr06;
			break;
		case PulseHdr06:
			// data type 
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr07;
			break;
			//================ Byte Count ================
		case PulseHdr07:
			m_return = 0x0;
			U32_1st(ReadRawData, &m_return);
			m_status = PulseHdr08;
			break;
		case PulseHdr08:
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHdr09;
			break;
		case PulseHdr09:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHdr10;
			break;
		case PulseHdr10:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHdr11;
			break;
			//================ T0 ================
		case PulseHdr11:
			m_return = 0x0;
			U32_1st(ReadRawData, &m_return);
			m_status = PulseHdr12;
			break;
		case PulseHdr12:
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHdr13;
			break;
		case PulseHdr13:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHdr14;
			break;
		case PulseHdr14:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHdr15;
			m_bcount = 16;
			break;
			//================ Hit 0================
		case PulseHdr15:
                        // channel
			m_return = Decode_PulseHit00(ReadRawData);
			break;
		case PulseHit07:
                        // channel
			m_return = Decode_PulseHit00(ReadRawData);
			break;
		case PulseHit00:
			// TOF
			m_return = 0x0;
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHit01;
			break;
		case PulseHit01:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHit02;
			break;
		case PulseHit02:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHit03;
			m_bcount += 4;
			break;
			//================ Hit 1================
		case PulseHit03:
                        // Channel
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHit04;
			break;
		case PulseHit04:
                        // Baseline(12bit) & Q(12bit)
			m_return = 0x0;
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHit05;
			break;
		case PulseHit05:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHit06;
			break;
		case PulseHit06:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHit07;
			m_bcount += 4;
			break;
			//================ End ================
		case PulseEnd00:
			// status
			m_return = 0x0;
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseEnd01;
			break;
		case PulseEnd01:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseEnd02;
			break;
		case PulseEnd02:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseEnd03;
			m_bcount += 4;
			break;
			//================ Filling 0x0 ================
		case PulseEnd03:
			U32_1st(ReadRawData, &m_return);
			m_status = PulseEnd04;
			break;
		case PulseEnd04:
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseEnd05;
			break;
		case PulseEnd05:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseEnd06;
			break;
		case PulseEnd06:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseEnd07;
			break;
		default:
			m_status = PExecReady;
	}

	return m_return;
}
