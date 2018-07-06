#ifndef DECODE_MWPC_RAWDATA_H
#define DECODE_MWPC_RAWDATA_H
// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2018
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2018 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DecodeMWPCRawData.h
//  
//
//  Created by Haolai TIAN 4th May 2018.
//
//

#include "DecodeRawDataBase.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



class DecodeMWPCRawData : public DecodeRawDataBase{
	private:
		uint32_t  m_bcount;
		uint32_t  m_return;
	public:
		enum ProStatus{
			PExecReady = 0,
			PExecError = 1,
			PulsePre00 = 3,
			PulsePre01 = 4,
			PulseHdr00 = 10,
			PulseHdr01 = 11,
			PulseHdr02 = 12,
			PulseHdr03 = 13,
			PulseHdr04 = 14,
			PulseHdr05 = 15,
			PulseHdr06 = 16,
			PulseHdr07 = 17,
			PulseHdr08 = 18,
			PulseHdr09 = 19,
			PulseHdr10 = 20,
			PulseHdr11 = 21,
			PulseHdr12 = 22,
			PulseHdr13 = 23,
			PulseHdr14 = 24,
			PulseHdr15 = 25,
			PulseHdr16 = 26,
			PulseHdr17 = 27,
			PulseHdr18 = 28,
			PulseHdr19 = 29,
			PulseHdr20 = 30,
			PulseHdr21 = 31,
			PulseHdr22 = 32,
			PulseHdr23 = 33,
			PulseHdr24 = 34,
			PulseHdr25 = 35,
			PulseHdr26 = 36,
			PulseHdr27 = 37,
			PulseHdr28 = 38,
			PulseHdr29 = 39,
			PulseHdr30 = 40,
			PulseHdr31 = 41,
			PulseHdr32 = 42,
			PulseHdr33 = 43,
			PulseHdr34 = 44,
			PulseHdr35 = 45,
			PulseHdr36 = 46,
			PulseHdr37 = 47,
			PulseHdr38 = 48,
			PulseHdr39 = 49,
			PulseHdr40 = 50,
			PulseHdr41 = 51,
			PulseHdr42 = 52,
			PulseHdr43 = 53,
			PulseHdr44 = 54,
			PulseHdr45 = 55,
			PulseHdr46 = 56,
			PulseHdr47 = 57,
			PulseHdr48 = 58,
			PulseHdr49 = 59,
			PulseHdr50 = 60,
			PulseHdr51 = 61,
			PulseHdr52 = 62,
			PulseHdr53 = 63,
			PulseHdr54 = 64,
			PulseHdr55 = 65,
			PulseHdr56 = 66,
			PulseHdr57 = 67,
			PulseHdr58 = 68,
			PulseHdr59 = 69,
			PulseHdr60 = 70,
			PulseHdr61 = 71,
			PulseHdr62 = 72,
			PulseHdr63 = 73,
			FrameHdr00 = 80,
			FrameHdr01 = 81,
			FrameHdr02 = 82,
			FrameHdr03 = 83,
			FrameHdr04 = 84,
			FrameHdr05 = 85,
			FrameHdr06 = 86,
			FrameHdr07 = 87,
			PulseHit00 = 90,
			PulseHit01 = 91,
			PulseHit02 = 92,
			PulseHit03 = 93,
			PulseHit04 = 94,
			PulseHit05 = 95,
			PulseHit06 = 96,
			PulseHit07 = 97,
		} m_status;
	private:
                
		void     Decode_PulsePre00(uint8_t* Input);
		void     Decode_PulsePre01(uint8_t* Input);
		void     Decode_PulseHdr00(uint8_t* Input);
		void     Decode_PulseHdr01(uint8_t* Input);
		void     Decode_PulseHdr02(uint8_t* Input);
		void     Decode_PulseHdr03(uint8_t* Input);
		void     Decode_FrameHdr00(uint8_t* Input);
		void     Decode_PulseHit00(uint8_t* Input);

	public:
		DecodeMWPCRawData();
		~DecodeMWPCRawData(){};

		ProStatus Get_ProStatus(){return m_status;}
		void Set_ProStatus(ProStatus status){m_status = status;}

		uint32_t Decode_RawDataSegment(uint8_t *buff);

};


#endif
