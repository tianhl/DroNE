// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2018
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2018 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MWPCRawDataInputSvc.cc
//  
//
//  Created by Haolai TIAN 4th May 2018.
//
//
#include "DataSvc/MWPCRawDataInputSvc.h"
#include "DataSvc/DecodeMWPCRawData.h"

#include "DroNECore/DataProvideSvc.h"
#include "DroNECore/DataSvc.h"

#include "SniperKernel/Incident.h"
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
#include "Data/ECal.h"
#include "Data/MWPCHit.h"


DECLARE_SERVICE(MWPCRawDataInputSvc);

MWPCRawDataInputSvc::MWPCRawDataInputSvc(const std::string& name)
: DataInputSvc(name) {
	declProp("BuffSize",  m_buffsize);
	m_decoder = new DecodeMWPCRawData();

	m_isLastSegment = false;
	m_offset = 0;
	m_currbuffsize = 0;
}

MWPCRawDataInputSvc::~MWPCRawDataInputSvc() {
}

bool MWPCRawDataInputSvc::initialize() {

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



bool MWPCRawDataInputSvc::finalize() {
	return true;
}

bool MWPCRawDataInputSvc::next() {
	uint8_t *ReadRawData = NULL;
	uint32_t value  = 0x0;
	uint32_t value1 = 0x0;
	uint32_t value2 = 0x0;
	MWPCHit* hit;
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
	MWPCHitList*  hitcol = m_dataSvc->getObj<MWPCHitList>("/pulse/hits");
	EvtList*      evtcol = m_dataSvc->getObj<EvtList>("/pulse/evts");
	hitcol->clear();
	evtcol->clear();
	m_decoder->Set_ProStatus(DecodeMWPCRawData::PExecReady);

	while(true){
		ReadRawData = readByte();
		if(NULL != ReadRawData) {
			value = m_decoder->Decode_RawDataSegment(ReadRawData);
			//printf("MWPCRawDataInputSvc ReadRawData %x\n",*ReadRawData);
			//LogInfo << "Pro Status: " << m_decoder->Get_ProStatus() << std::endl;

			switch(m_decoder->Get_ProStatus()){
				//================ Hdr ================
				case DecodeMWPCRawData::PulseHdr00:
					status = hdr0;
					//LogInfo << "HDR" << std::endl;
					break;
				case DecodeMWPCRawData::PulseHdr01:
					npulse->setInstrument(value);
					break;
				case DecodeMWPCRawData::PulseHdr02:
					npulse->setDetector(value);
					break;
				case DecodeMWPCRawData::PulseHdr03:
					npulse->setModule(value);
					break;
				case DecodeMWPCRawData::PulseHdr04:
					npulse->setRunMode(value);
					break;
				case DecodeMWPCRawData::PulseHdr05:
					npulse->setFrame(value);
					break;
				case DecodeMWPCRawData::PulseHdr06:
					npulse->setVersion(value);
					break;
				case DecodeMWPCRawData::PulseHdr07:
					//npulse->setDataType(value);
					break;
					//================ Byte Cnt ================
				case DecodeMWPCRawData::PulseHdr08:
					break;
				case DecodeMWPCRawData::PulseHdr09:
					break;
				case DecodeMWPCRawData::PulseHdr10:
					break;
				case DecodeMWPCRawData::PulseHdr11:
					npulse->setCount(value);
					//std::cout << std::dec << "T0 " << value << std::endl;
					//std::cout << std::hex << "T0 " << value << std::endl;
					break;
					//================ T0 ================
				case DecodeMWPCRawData::PulseHdr12:
					break;
				case DecodeMWPCRawData::PulseHdr13:
					break;
				case DecodeMWPCRawData::PulseHdr14:
					break;
				case DecodeMWPCRawData::PulseHdr15:
					npulse->setT0(value);
					status = hdr1;
					break;
					//================ Hit ================
				case DecodeMWPCRawData::PulseHit00:
					status = hit0;
					hit = hitcol->add_item();
					hit->setCharge(value);
					break;
				case DecodeMWPCRawData::PulseHit01:
					break;
				case DecodeMWPCRawData::PulseHit02:
					break;
				case DecodeMWPCRawData::PulseHit03:
					hit = hitcol->back();
					hit->setTOF(value);
					break;
				case DecodeMWPCRawData::PulseHit04:
					hit = hitcol->back();
					if(value != hit->getChannel()) std::cout << "Decodor ERROR!" << std::endl;
                                        break;
				case DecodeMWPCRawData::PulseHit05:
                                        break;
				case DecodeMWPCRawData::PulseHit06:
                                        break;
				case DecodeMWPCRawData::PulseHit07:
                                        m_decoder->U24to2U12(&value, &value1, &value2);
					hit = hitcol->back();
					hit->setBaseline(value1);
					hit->setCharge(value2);
                                        status = hit1;
                                        break;
					//================ End ================
				case DecodeMWPCRawData::PulseEnd00:
					status = end0;
					break;
				case DecodeMWPCRawData::PulseEnd01:
					break;
				case DecodeMWPCRawData::PulseEnd02:
					break;
				case DecodeMWPCRawData::PulseEnd03:
					npulse->setStatus(value);
					break;
				case DecodeMWPCRawData::PulseEnd04:
					break;
				case DecodeMWPCRawData::PulseEnd05:
					break;
				case DecodeMWPCRawData::PulseEnd06:
					break;
				case DecodeMWPCRawData::PulseEnd07:
					status = end1;
					break;
					//================ Exe ================
				case DecodeMWPCRawData::PExecReady:
					//std::cout << "Ready: " << std::endl;
					status = redy;
					break;
				case DecodeMWPCRawData::PExecError:
					// if error, discards the old and find the new PULSE
					//LogInfo << "IO Decode Error " << std::endl;
					status = redy;
					m_decoder->Set_ProStatus(DecodeMWPCRawData::PExecReady);
					goto STARTNEXT;
					break;
			}
		}

		if(status == end1)   break;
		if(m_isLastSegment && (m_offset == m_currbuffsize)) return false;

	}

	//if(DecodeMWPCRawData::isPulseHeader(ReadRawData))  
	//else throw SniperException("Pulse Header NOT FOUND!");

	return true;

}

//=====================================================================
// Private Functions
// ====================================================================

uint8_t* MWPCRawDataInputSvc::readByte(){
	if(m_offset == m_currbuffsize) m_currbuffsize = nextSegment();
	//std::cout << "raw data input svc m_offset: " << m_offset << std::endl;
	//printf("ReadByte %x\n",*(m_dataBuff+m_offset));
	if(0 == m_currbuffsize)return (uint8_t*)NULL;
	else return (uint8_t*)(m_dataBuff+(m_offset++));
}

size_t MWPCRawDataInputSvc::nextSegment() {
	//std::cout << "nextSegment" << std::endl;
	m_offset = 0;
	if (not m_dataPvdSvc->read(m_dataBuff, m_buffsize)) m_isLastSegment = true;
	//std::cout << "nSegment get count: " << m_dataPvdSvc->count() << std::endl;
	return m_dataPvdSvc->count();
}
