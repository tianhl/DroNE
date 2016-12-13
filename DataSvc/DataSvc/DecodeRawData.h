#ifndef DECODE_RAWDATA_H
#define DECODE_RAWDATA_H

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>



class DecodeRawData{
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
			PulseHit00 = 30,
			PulseHit01 = 31,
			PulseHit02 = 32,
			PulseHit03 = 33,
			PulseEnd00 = 40,
			PulseEnd01 = 41,
			PulseEnd02 = 42,
			PulseEnd03 = 43,
			PulseEnd04 = 44,
			PulseEnd05 = 45,
			PulseEnd06 = 46,
			PulseEnd07 = 47,
			PulsePop00 = 50,
			PulsePop01 = 51,
			PulsePop02 = 52,
			PulsePop03 = 53,
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
		DecodeRawData();
		~DecodeRawData(){};

		uint32_t Decode_RawDataSegment(uint8_t *buff);
		DecodeRawData::ProStatus Get_ProStatus(){return m_status;}
		void Set_ProStatus(ProStatus status){m_status = status;}
		
};


#endif
