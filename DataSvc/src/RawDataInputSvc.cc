#include "DataSvc/RawDataInputSvc.h"
#include "DataSvc/DataSvc.h"
#include "DataSvc/BeginEvtHdl.h"
#include "DataSvc/DataProvideSvc.h"

#include "SniperKernel/Incident.h"
#include "DataSvc/DecodeRawData.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"


#include <iostream>
#include <list>
#include <string>
#include <ctime>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/Hit.h"


DECLARE_SERVICE(RawDataInputSvc);

RawDataInputSvc::RawDataInputSvc(const std::string& name)
: SvcBase(name) {
	declProp("BuffSize",  m_buffsize);
	m_decoder = new DecodeRawData();

	m_isLastSegment = false;
	m_offset = 0;
	m_currbuffsize = 0;
}

RawDataInputSvc::~RawDataInputSvc() {
}

bool RawDataInputSvc::initialize() {

	LogInfo << "InputSvc initialize " << std::endl;

	m_dataBuff = new uint8_t[m_buffsize];
	for(uint32_t i =0; i < m_buffsize; i++) m_dataBuff[i] = 0xFF;

	SniperPtr<DataSvc> pDSvc("DataSvc");
	if ( pDSvc.invalid()) throw SniperException("DataSvc is invalid!");
	m_dataSvc = pDSvc.data();

	SniperPtr<DataProvideSvc> pPSvc("DataProvideSvc");
	if ( pPSvc.invalid()) throw SniperException("DataProvideSvc is invalid!");
	m_dataPvdSvc = pPSvc.data();

	//m_dataPvdSvc->open();

	return true;
}



bool RawDataInputSvc::finalize() {
	return true;
}

bool RawDataInputSvc::next() {
	uint8_t *ReadRawData = NULL;
	uint32_t value = 0;
	Hit* hit;
	enum Status{
		hdr0,
		hdr1,
		hit0,
		hit1,
		end0,
		end1,
		redy,
	}status;

STARTNEXT:
	//std::cout << "=========== new pulse ready =============" << std::endl;
	NeutronPulse* npulse = m_dataSvc->getObj<NeutronPulse>("/pulse");
	HitList* hitcol      = m_dataSvc->getObj<HitList>("/pulse/hits");
	EvtList* evtcol      = m_dataSvc->getObj<EvtList>("/pulse/evts");
	hitcol->clear();
	evtcol->clear();
	m_decoder->Set_ProStatus(DecodeRawData::PExecReady);

	while(true){
		ReadRawData = readByte();
		if(NULL != ReadRawData) {
			value = m_decoder->Decode_RawDataSegment(ReadRawData);
			//printf("RawDataInputSvc ReadRawData %x\n",*ReadRawData);
			//LogInfo << "Pro Status: " << m_decoder->Get_ProStatus() << std::endl;

			switch(m_decoder->Get_ProStatus()){
				//================ Hdr ================
				case DecodeRawData::PulseHdr00:
					status = hdr0;
					//LogInfo << "HDR" << std::endl;
					break;
				case DecodeRawData::PulseHdr01:
					npulse->setInstrument(value);
					break;
				case DecodeRawData::PulseHdr02:
					npulse->setDetector(value);
					break;
				case DecodeRawData::PulseHdr03:
					npulse->setModule(value);
					break;
				case DecodeRawData::PulseHdr04:
					npulse->setRunMode(value);
					break;
				case DecodeRawData::PulseHdr05:
					npulse->setFrame(value);
					break;
				case DecodeRawData::PulseHdr06:
					npulse->setVersion(value);
					break;
					//================ T0 ================
				case DecodeRawData::PulseHdr07:
					break;
				case DecodeRawData::PulseHdr08:
					break;
				case DecodeRawData::PulseHdr09:
					break;
				case DecodeRawData::PulseHdr10:
					break;
				case DecodeRawData::PulseHdr11:
					npulse->setT0(value);
					break;
					//================ Rs1 ================
				case DecodeRawData::PulseHdr12:
					break;
				case DecodeRawData::PulseHdr13:
					break;
				case DecodeRawData::PulseHdr14:
					break;
				case DecodeRawData::PulseHdr15:
					status = hdr1;
					break;
					//================ Hit ================
				case DecodeRawData::PulseHit00:
					status = hit0;
					hit = hitcol->add_item();
					hit->setChannel(value);
					//std::cout << std::dec << m_decoder->Get_ProStatus() << " CHN  " << value << std::endl;
					break;
				case DecodeRawData::PulseHit01:
					break;
				case DecodeRawData::PulseHit02:
					break;
				case DecodeRawData::PulseHit03:
					status = hit1;
					hit = hitcol->back();
					hit->setTOF(value);
					//std::cout << std::dec << m_decoder->Get_ProStatus() << " TOF  " << value << std::endl;
					break;
					//================ Pop ================
				case DecodeRawData::PulsePop00:
					status = hit0;
					break;
				case DecodeRawData::PulsePop01:
					break;
				case DecodeRawData::PulsePop02:
					break;
				case DecodeRawData::PulsePop03:
					status = hit1;
					break;
					//================ End ================
				case DecodeRawData::PulseEnd00:
					status = end0;
					break;
				case DecodeRawData::PulseEnd01:
					break;
				case DecodeRawData::PulseEnd02:
					break;
				case DecodeRawData::PulseEnd03:
					npulse->setStatus(value);
					break;
				case DecodeRawData::PulseEnd04:
					break;
				case DecodeRawData::PulseEnd05:
					break;
				case DecodeRawData::PulseEnd06:
					break;
				case DecodeRawData::PulseEnd07:
					npulse->setCount(value);
					//LogInfo << "END" << std::endl;
					status = end1;
					break;
					//================ Exe ================
				case DecodeRawData::PExecReady:
					//std::cout << "Ready: " << std::endl;
					status = redy;
					break;
				case DecodeRawData::PExecError:
					// if error, discards the old and find the new PULSE
					//LogInfo << "IO Decode Error " << std::endl;
					status = redy;
					m_decoder->Set_ProStatus(DecodeRawData::PExecReady);
					goto STARTNEXT;
					break;
			}
		}

		if(status == end1)   break;
		if(m_isLastSegment && (m_offset == m_currbuffsize)) return false;

	}

	//if(DecodeRawData::isPulseHeader(ReadRawData))  
	//else throw SniperException("Pulse Header NOT FOUND!");

	return true;

}

//=====================================================================
// Private Functions
// ====================================================================

uint8_t* RawDataInputSvc::readByte(){
	if(m_offset == m_currbuffsize) m_currbuffsize = nextSegment();
	if(0 == m_currbuffsize)return (uint8_t*)NULL;
	else return (uint8_t*)(m_dataBuff+(m_offset++));
}

size_t RawDataInputSvc::nextSegment() {
	m_offset = 0;
	if (not m_dataPvdSvc->read(m_dataBuff, m_buffsize)) m_isLastSegment = true;
	return m_dataPvdSvc->count();
}
