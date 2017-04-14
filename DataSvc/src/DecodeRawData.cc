#include "DataSvc/DecodeRawData.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <vector>
using namespace std;


DecodeRawData::DecodeRawData()
: m_bcount(0), m_status(PExecReady)
{
};


void DecodeRawData::U32_1st(uint8_t *Input, uint32_t *Output){
	*Output = (((((uint32_t)(*Input))&0xFF)<<24)&0xFF000000);
};

void DecodeRawData::U32_2ed(uint8_t *Input, uint32_t *Output){
	*Output |= (((((uint32_t)(*Input)&0xFF))<<16)&0xFF0000);
};

void DecodeRawData::U32_3rd(uint8_t *Input, uint32_t *Output){
	*Output |= (((((uint32_t)(*Input)&0xFF))<<8)&0xFF00);
};

void DecodeRawData::U32_4th(uint8_t *Input, uint32_t *Output){
	*Output |= ((((uint32_t)(*Input)&0xFF))&0xFF);
};

void DecodeRawData::Decode_PulseHdr00(uint8_t *Input){
	if(0xFA == *Input) m_status = PulseHdr00;
	else m_status = PExecReady;
};

uint32_t DecodeRawData::Decode_PulseHit00(uint8_t *Input){
	m_return = 0x0;
	if(0xF0 ==  *Input) m_status = PulsePop00;
	else if(0xFB == *Input) m_status = PulseEnd00;
	else{
		m_return = (uint32_t)(*Input);
		m_status = PulseHit00;
	}
	return m_return;
};

void DecodeRawData::Decode_PulseEnd00(uint8_t *Input){
	if(0xFB == *Input)m_status = PulseEnd00;
	else {
		m_status = PExecError;
		//std::cout << "Cannot find End: " << std::endl;
		//printf("     Input %x\n",*Input);
	}
};

uint32_t DecodeRawData::Decode_RawDataSegment(uint8_t *ReadRawData){
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
			// reserve 1
			m_return = (uint32_t)(*ReadRawData);
			m_status = PulseHdr07;
			break;
			//================ T0 ================
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
			//================ Rs1 ================
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
			//================ Hit ================
		case PulseHdr15:
			m_return = Decode_PulseHit00(ReadRawData);
			break;
		case PulseHit03:
			m_return = Decode_PulseHit00(ReadRawData);
			break;
		case PulseHit00:
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
			//================ Pop ================
		case PulsePop00:
			m_status = PulsePop01;
			break;
		case PulsePop01:
			m_status = PulsePop02;
			break;
		case PulsePop02:
			m_status = PulsePop03;
			m_bcount += 4;
			break;
			//================ End ================
		case PulsePop03:
			Decode_PulseEnd00(ReadRawData);
			break;
		case PulseEnd00:
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
			//================ Cnt ================
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
			//if(16>abs(m_bcount - m_return)){
			//	m_status = PExecError;
		        //	std::cout << "Count Error: bcount/" << m_bcount << " rcount/" << m_return << std::endl;
			//	m_return = (uint32_t)NULL;
			//}
			break;
		default:
			m_status = PExecReady;
	}

	return m_return;
}
