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
	uint32_t value   = 0x0;
	uint32_t value1  = 0x0;
	uint32_t value2  = 0x0;
        uint32_t tdataid = 0;
	MWPCHit* hit;
	enum Status{
		hdr0,
		hdr1,
                fam0,
                fam1,
		hit0,
		hit1,
                end0,
                end1,
		redy,
	}status;

	//std::cout << "=========== new pulse ready =============" << std::endl;
	NeutronPulse* npulse = m_dataSvc->getObj<NeutronPulse>("/pulse");
	MWPCHitList*  hitcol = m_dataSvc->getObj<MWPCHitList>("/pulse/hits");
	EvtList*      evtcol = m_dataSvc->getObj<EvtList>("/pulse/evts");
STARTNEXT:
        tdataid = 0;
	hitcol->clear();
	evtcol->clear();
	if(m_decoder->Get_ProStatus() != DecodeMWPCRawData::PulseHdr01)
		m_decoder->Set_ProStatus(DecodeMWPCRawData::PExecReady);

	while(true){
		ReadRawData = readByte();
		if(NULL != ReadRawData) {
			value = m_decoder->Decode_RawDataSegment(ReadRawData);
			//printf("MWPCRawDataInputSvc ReadRawData %x\n",*ReadRawData);
			//LogInfo << "Pro Status: " << m_decoder->Get_ProStatus() << std::endl;
			//if(0xEE == *ReadRawData)std::cout << "NEXT PULSE" << std::endl;

			switch(m_decoder->Get_ProStatus()){
				//================ Hdr line 00 ================
				case DecodeMWPCRawData::PulsePre00:
					status = hdr0;
					break;
				case DecodeMWPCRawData::PulsePre01:
					break;
				case DecodeMWPCRawData::PulseHdr00:
					status = end0;
					break;
				case DecodeMWPCRawData::PulseHdr01:
					status = end1;
					break;
				case DecodeMWPCRawData::PulseEnd00:
					status = end1;
					break;
				case DecodeMWPCRawData::PulseHdr02:
					//std::cout << "HDR02" << std::endl;
					status = hdr0;
					break;
				case DecodeMWPCRawData::PulseHdr03:
					status = hdr1;
					break;
					//================ Hdr line 01 ================
				case DecodeMWPCRawData::PulseHdr04:
					npulse->setInstrument(value);
					break;
				case DecodeMWPCRawData::PulseHdr05:
					npulse->setDetector(value);
					break;
				case DecodeMWPCRawData::PulseHdr06:
					break;
				case DecodeMWPCRawData::PulseHdr07:
					break;
					//================ Hdr line 02 ================
				case DecodeMWPCRawData::PulseHdr08:
					npulse->setRunMode(value);
					break;
				case DecodeMWPCRawData::PulseHdr09:
					npulse->setFrame(value);
					break;
				case DecodeMWPCRawData::PulseHdr10:
					npulse->setVersion(value);
					break;
				case DecodeMWPCRawData::PulseHdr11:
					//npulse->setDataType(value);
					break;
					//================ Hdr line 03 ================
				case DecodeMWPCRawData::PulseHdr12:
					m_decoder->U8to2U4(&value, &value1, &value2);
					break;
				case DecodeMWPCRawData::PulseHdr13:
					m_decoder->U8to2U4(&value, &value1, &value2);
					break;
				case DecodeMWPCRawData::PulseHdr14:
					m_decoder->U8to2U4(&value, &value1, &value2);
					break;
				case DecodeMWPCRawData::PulseHdr15:
					break;
					//================ Hdr line 04 ================
				case DecodeMWPCRawData::PulseHdr16:
					break;
				case DecodeMWPCRawData::PulseHdr17:
					break;
				case DecodeMWPCRawData::PulseHdr18:
					break;
				case DecodeMWPCRawData::PulseHdr19:
					break;
					//================ Hdr line 05 ================
				case DecodeMWPCRawData::PulseHdr20:
					break;
				case DecodeMWPCRawData::PulseHdr21:
					break;
				case DecodeMWPCRawData::PulseHdr22:
					break;
				case DecodeMWPCRawData::PulseHdr23:
					break;
					//================ Hdr line 06 ================
				case DecodeMWPCRawData::PulseHdr24:
					break;
				case DecodeMWPCRawData::PulseHdr25:
					break;
				case DecodeMWPCRawData::PulseHdr26:
					break;
				case DecodeMWPCRawData::PulseHdr27:
					break;
					//================ Hdr line 07 ================
				case DecodeMWPCRawData::PulseHdr28:
					break;
				case DecodeMWPCRawData::PulseHdr29:
					break;
				case DecodeMWPCRawData::PulseHdr30:
					break;
				case DecodeMWPCRawData::PulseHdr31:
					break;
					//================ Hdr line 08 ================
				case DecodeMWPCRawData::PulseHdr32:
					break;
				case DecodeMWPCRawData::PulseHdr33:
					break;
				case DecodeMWPCRawData::PulseHdr34:
					break;
				case DecodeMWPCRawData::PulseHdr35:
					break;
					//================ Hdr line 09 ================
				case DecodeMWPCRawData::PulseHdr36:
					break;
				case DecodeMWPCRawData::PulseHdr37:
					break;
				case DecodeMWPCRawData::PulseHdr38:
					break;
				case DecodeMWPCRawData::PulseHdr39:
					break;
					//================ Hdr line 10 ================
					//================ T0          ================
				case DecodeMWPCRawData::PulseHdr40:
					break;
				case DecodeMWPCRawData::PulseHdr41:
					break;
				case DecodeMWPCRawData::PulseHdr42:
					break;
				case DecodeMWPCRawData::PulseHdr43:
					npulse->setT0(value);
					//std::cout << std::dec << "bitcount " << value << std::endl;
					//std::cout << std::hex << "T0 " << value << std::endl;
					break;
					//================ Hdr line 11 ================
					//================ Count       ================
				case DecodeMWPCRawData::PulseHdr44:
					break;
				case DecodeMWPCRawData::PulseHdr45:
					break;
				case DecodeMWPCRawData::PulseHdr46:
					break;
				case DecodeMWPCRawData::PulseHdr47:
					npulse->setCount(value);
					break;
					//================ Hdr line 12 ================
					//================ Number of Frame ============
				case DecodeMWPCRawData::PulseHdr48:
					break;
				case DecodeMWPCRawData::PulseHdr49:
					break;
				case DecodeMWPCRawData::PulseHdr50:
					break;
				case DecodeMWPCRawData::PulseHdr51:
					break;
					//================ Hdr line 13 ================
					//================ Reserve 3   ================
				case DecodeMWPCRawData::PulseHdr52:
					break;
				case DecodeMWPCRawData::PulseHdr53:
					break;
				case DecodeMWPCRawData::PulseHdr54:
					break;
				case DecodeMWPCRawData::PulseHdr55:
					break;
					//================ Hdr line 14 ================
					//================ Reserve 4   ================
				case DecodeMWPCRawData::PulseHdr56:
					break;
				case DecodeMWPCRawData::PulseHdr57:
					break;
				case DecodeMWPCRawData::PulseHdr58:
					break;
				case DecodeMWPCRawData::PulseHdr59:
					break;
					//================ Hdr line 15 ================
					//================ Reserve 5   ================
				case DecodeMWPCRawData::PulseHdr60:
					break;
				case DecodeMWPCRawData::PulseHdr61:
					break;
				case DecodeMWPCRawData::PulseHdr62:
					break;
				case DecodeMWPCRawData::PulseHdr63:
					status = hdr1;
					break;
					//================ Frame Hdr 1 ================
					//================ TData ID    ================
				case DecodeMWPCRawData::FrameHdr00:
					//std::cout << "Frame HDR" << std::endl;
					status = fam0;
					break;
				case DecodeMWPCRawData::FrameHdr01:
					break;
				case DecodeMWPCRawData::FrameHdr02:
					break;
				case DecodeMWPCRawData::FrameHdr03:
                                        tdataid = value;
                                        //std::cout << "Get TData ID: " << tdataid << std::endl;
					break;
					//================ Frame Hdr 2 ================
					//================ Number of Event ============
				case DecodeMWPCRawData::FrameHdr04:
					break;
				case DecodeMWPCRawData::FrameHdr05:
					break;
				case DecodeMWPCRawData::FrameHdr06:
					break;
				case DecodeMWPCRawData::FrameHdr07:
					status = fam1;
					break;
					//================ Hit ================
				case DecodeMWPCRawData::PulseHit00:
					status = hit0;
					hit = hitcol->add_item();
					hit->setChannel(value);
					hit->setTDataID(tdataid);
                                        //std::cout << "Set TData ID: " << hit->getTDataID() << std::endl;
					//std::cout << "Channel: " << hit->getChannel() << std::endl;
					break;
				case DecodeMWPCRawData::PulseHit01:
					break;
				case DecodeMWPCRawData::PulseHit02:
					break;
				case DecodeMWPCRawData::PulseHit03:
					hit = hitcol->back();
					hit->setTOF(value);
					//std::cout << "TOF: " << hit->getTOF() << std::endl;
					break;
				case DecodeMWPCRawData::PulseHit04:
					//hit = hitcol->back();
					//if(value != hit->getChannel()) std::cout << "Decodor ERROR!" << std::endl;
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
					//std::cout << "Baseline: " << hit->getBaseline() << std::endl;
					//std::cout << "Charge:   " << hit->getCharge() << std::endl;
					status = hit1;
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
