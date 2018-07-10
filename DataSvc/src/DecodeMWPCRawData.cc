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
void DecodeMWPCRawData::Decode_PulseHdr00(uint8_t *Input){
        // END
	if(0xEE == *Input) m_status = PulseHdr00;
	else m_status = PExecError;
	//std::cout << "Pulse HDR00 " << std::endl;
};

void DecodeMWPCRawData::Decode_PulseHdr02(uint8_t *Input){
	// BEGIN
	if(0x34 == *Input) m_status = PulseHdr02;
	else m_status = PExecError;
};

void DecodeMWPCRawData::Decode_FrameHdr00(uint8_t *Input){
	if(0xAA == *Input)m_status = FrameHdr00;
	else m_status = PExecError;
	//std::cout << "Frame HDR " << std::endl;
};

void DecodeMWPCRawData::Decode_PulsePre00(uint8_t *Input){
        // PRE
	if(0xEE == *Input) m_status = PulsePre00;
	else m_status = PExecReady;
};

void DecodeMWPCRawData::Decode_PulsePre01(uint8_t *Input){
	if(0x12 == *Input) m_status = PulsePre01;
	else m_status = PExecReady;
	//std::cout << "Frame HDR " << std::endl;
};

void DecodeMWPCRawData::Decode_PulseHdr01(uint8_t *Input){
	//std::cout << "HDR " << std::endl;
	if(0x12 == *Input) m_status = PulseHdr01;
	else m_status = PExecError;
};

void DecodeMWPCRawData::Decode_PulseHdr03(uint8_t *Input){
	if(0xEE == *Input) m_status = PulseHdr03;
	else m_status = PExecError;
};

void DecodeMWPCRawData::Decode_PulseHit00(uint8_t *Input){
	//std::cout << "Hit " << std::endl;
	if(0xAA == *Input)      m_status = FrameHdr00;
	else if(0xEE == *Input) m_status = PulseHdr00;
	else if(0x00 == *Input) m_status = PulseEnd00;
	else                    m_status = PulseHit00;
	m_return = (uint32_t)(*Input);
};


uint32_t DecodeMWPCRawData::Decode_RawDataSegment(uint8_t *ReadRawData){
	//printf("%x\n", *ReadRawData);

	switch(m_status){
		//================ Hdr ================
                case PulseEnd00:
			Decode_PulsePre00(ReadRawData);
                        break;
		case PExecReady:
                        //std::cout << "Ready" << std::endl;
			Decode_PulsePre00(ReadRawData);
			break;
		case PulsePre00:
			Decode_PulsePre01(ReadRawData);
			break;
		case PulsePre01:
			Decode_PulseHdr02(ReadRawData);
			break;
		case PulseHit07:
			Decode_PulseHit00(ReadRawData);
			break;
		case PulseHdr00:
			// PulseHdr 1
			Decode_PulseHdr01(ReadRawData);
			break;
		case PulseHdr01:
			// PulseHdr 2
			Decode_PulseHdr02(ReadRawData);
			break;
		case PulseHdr02:
			// PulseHdr 3
			Decode_PulseHdr03(ReadRawData);
			break;
			//================ 01 line of hdr ================
		case PulseHdr03:
			// instrument ID
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr04;
			break;
		case PulseHdr04:
			// detector ID 
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr05;
			break;
		case PulseHdr05:
			// reserve1[0]
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr06;
			break;
		case PulseHdr06:
			// reserve1[1]
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr07;
			break;
			//================ 02 line of hdr ================
		case PulseHdr07:
			// run mode
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr08;
			break;
		case PulseHdr08:
			// frame
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr09;
			break;
		case PulseHdr09:
			// version
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr10;
			break;
		case PulseHdr10:
			// data type
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr11;
			break;
			//================ 03 line of hdr ================
		case PulseHdr11:
			// m1 & m2 status
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr12;
			break;
		case PulseHdr12:
			// m3 & m4 status
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr13;
			break;
		case PulseHdr13:
			// m5 & m6 status
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr14;
			break;
		case PulseHdr14:
			// reserve2
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr15;
			m_bcount = 16;
			break;
			//================ 04 line of hdr ================
			//================ Module 1 Byte count ===========
		case PulseHdr15:
			m_return = 0x0;
			U32_1st(ReadRawData, &m_return);
			m_status = PulseHdr16;
			break;
		case PulseHdr16:
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHdr17;
			break;
		case PulseHdr17:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHdr18;
			break;
		case PulseHdr18:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHdr19;
			m_bcount += 4;
			break;
			//================ 05 line of hdr ================
			//================ Module 2 Byte count ===========
		case PulseHdr19:
			m_return = 0x0;
			U32_1st(ReadRawData, &m_return);
			m_status = PulseHdr20;
			break;
		case PulseHdr20:
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHdr21;
			break;
		case PulseHdr21:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHdr22;
			break;
		case PulseHdr22:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHdr23;
			m_bcount += 4;
			break;
			//================ 06 line of hdr ================
			//================ Module 3 Byte count ===========
		case PulseHdr23:
			m_return = 0x0;
			U32_1st(ReadRawData, &m_return);
			m_status = PulseHdr24;
			break;
		case PulseHdr24:
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHdr25;
			break;
		case PulseHdr25:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHdr26;
			break;
		case PulseHdr26:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHdr27;
			m_bcount += 4;
			break;
			//================ 07 line of hdr ================
			//================ Module 4 Byte count ===========
		case PulseHdr27:
			m_return = 0x0;
			U32_1st(ReadRawData, &m_return);
			m_status = PulseHdr28;
			break;
		case PulseHdr28:
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHdr29;
			break;
		case PulseHdr29:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHdr30;
			break;
		case PulseHdr30:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHdr31;
			m_bcount += 4;
			break;
			//================ 08 line of hdr ================
			//================ Module 5 Byte count ===========
		case PulseHdr31:
			m_return = 0x0;
			U32_1st(ReadRawData, &m_return);
			m_status = PulseHdr32;
			break;
		case PulseHdr32:
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHdr33;
			break;
		case PulseHdr33:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHdr34;
			break;
		case PulseHdr34:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHdr35;
			m_bcount += 4;
			break;
			//================ 09 line of hdr ================
			//================ Module 6 Byte count ===========
		case PulseHdr35:
			m_return = 0x0;
			U32_1st(ReadRawData, &m_return);
			m_status = PulseHdr36;
			break;
		case PulseHdr36:
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHdr37;
			break;
		case PulseHdr37:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHdr38;
			break;
		case PulseHdr38:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHdr39;
			m_bcount += 4;
			break;
			//================ 10 line of hdr ================
			//================ T0 count ======================
		case PulseHdr39:
			m_return = 0x0;
			U32_1st(ReadRawData, &m_return);
			m_status = PulseHdr40;
			break;
		case PulseHdr40:
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHdr41;
			break;
		case PulseHdr41:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHdr42;
			break;
		case PulseHdr42:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHdr43;
			m_bcount += 4;
			break;
			//================ 11 line of hdr ================
			//================ Byte count ====================
		case PulseHdr43:
			m_return = 0x0;
			U32_1st(ReadRawData, &m_return);
			m_status = PulseHdr44;
			break;
		case PulseHdr44:
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHdr45;
			break;
		case PulseHdr45:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHdr46;
			break;
		case PulseHdr46:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHdr47;
			m_bcount += 4;
			break;
			//================ 12 line of hdr ================
			//================ Number of Frame ===============
		case PulseHdr47:
			m_return = 0x0;
			U32_1st(ReadRawData, &m_return);
			m_status = PulseHdr48;
			break;
		case PulseHdr48:
			U32_2ed(ReadRawData, &m_return);
			m_status = PulseHdr49;
			break;
		case PulseHdr49:
			U32_3rd(ReadRawData, &m_return);
			m_status = PulseHdr50;
			break;
		case PulseHdr50:
			U32_4th(ReadRawData, &m_return);
			m_status = PulseHdr51;
			m_bcount += 4;
			break;
			//================ 13 line of hdr ================
			//================ reserve3       ===============
		case PulseHdr51:
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr52;
			break;
		case PulseHdr52:
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr53;
			break;
		case PulseHdr53:
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr54;
			break;
		case PulseHdr54:
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr55;
			m_bcount += 4;
			break;
			//================ 14 line of hdr ================
			//================ reserve4       ===============
		case PulseHdr55:
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr56;
			break;
		case PulseHdr56:
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr57;
			break;
		case PulseHdr57:
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr58;
			break;
		case PulseHdr58:
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr59;
			m_bcount += 4;
			break;
			//================ 15 line of hdr ================
			//================ reserve5       ===============
		case PulseHdr59:
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr60;
			break;
		case PulseHdr60:
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr61;
			break;
		case PulseHdr61:
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr62;
			break;
		case PulseHdr62:
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr63;
			m_bcount += 4;
			break;
			//================ Frame Hdr       ===============
		case PulseHdr63:
			Decode_FrameHdr00(ReadRawData);
			break;
		case FrameHdr00:
			// TDataID
			m_return = 0x0;
			U32_2ed(ReadRawData, &m_return);
			m_status = FrameHdr01;
			break;
		case FrameHdr01:
			U32_3rd(ReadRawData, &m_return);
			m_status = FrameHdr02;
			break;
		case FrameHdr02:
			U32_4th(ReadRawData, &m_return);
			m_status = FrameHdr03;
			m_bcount += 4;
			break;
			//================ Number of Event ===============
		case FrameHdr03:
			m_return = 0x0;
			U32_1st(ReadRawData, &m_return);
			m_status = FrameHdr04;
			break;
		case FrameHdr04:
			U32_2ed(ReadRawData, &m_return);
			m_status = FrameHdr05;
			break;
		case FrameHdr05:
			U32_3rd(ReadRawData, &m_return);
			m_status = FrameHdr06;
			break;
		case FrameHdr06:
			U32_4th(ReadRawData, &m_return);
			m_status = FrameHdr07;
			m_bcount += 4;
			break;
			//================ Hit 1================
		case FrameHdr07:
			// Channel
			Decode_PulseHit00(ReadRawData);
			break;
		case PulseHit00:
			// TData
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
			//================ Hit 2================
		case PulseHit03:
			// Channel
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHit04;
			break;
		case PulseHit04:
			// Baseline & QData
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
		default:
			m_status = PExecReady;
	}

	return m_return;
}
