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

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



class DecodeMWPCRawData{
	public:
		enum ProStatus{
			PExecReady = 0,
			PExecError = 1,
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
			PulseEnd00 = 40,
			PulseEnd01 = 41,
			PulseEnd02 = 42,
			PulseEnd03 = 43,
			PulseEnd04 = 44,
			PulseEnd05 = 45,
			PulseEnd06 = 46,
			PulseEnd07 = 47,
			PulseHit00 = 70,
			PulseHit01 = 71,
			PulseHit02 = 72,
			PulseHit03 = 73,
			PulseHit04 = 74,
			PulseHit05 = 75,
			PulseHit06 = 76,
			PulseHit07 = 77,
		};
	private:
		uint32_t  m_bcount;
		ProStatus m_status;
		uint32_t  m_return;
	private:
		void U32_1st(uint8_t* Input, uint32_t* Output);
		void U32_2ed(uint8_t* Input, uint32_t* Output);
		void U32_3rd(uint8_t* Input, uint32_t* Output);
		void U32_4th(uint8_t* Input, uint32_t* Output);
                
		void     Decode_PulseHdr00(uint8_t* Input);
		uint32_t Decode_PulseHit00(uint8_t* Input);
		void     Decode_PulseEnd00(uint8_t* Input);

	public:
		DecodeMWPCRawData();
		~DecodeMWPCRawData(){};

		uint32_t Decode_RawDataSegment(uint8_t *buff);
		DecodeMWPCRawData::ProStatus Get_ProStatus(){return m_status;}
		void Set_ProStatus(ProStatus status){m_status = status;}

                void U24to2U12(uint32_t* Input, uint32_t* Output_1, uint32_t* Ouput_2);
                void U8to2U4(uint32_t* Input, uint32_t* Output_1, uint32_t* Ouput_2);
                void U32toU12U20(uint32_t* Input, uint32_t* Output_1, uint32_t* Ouput_2);
                void U32toU14U12U6(uint32_t* Input, uint32_t* Output_1, uint32_t* Ouput_2, uint32_t* Ouput_3);
};


#endif
