// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  He3TRawDataInputSvc.cc
//  
//
//  Created by Haolai TIAN 10th Oct. 2017.
//
//
#include "DataSvc/He3TRawDataInputSvc.h"
#include "DataSvc/DecodeHe3TRawData.h"

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
#include "Data/He3THit.h"


DECLARE_SERVICE(He3TRawDataInputSvc);

He3TRawDataInputSvc::He3TRawDataInputSvc(const std::string& name)
: DataInputSvc(name) {
	m_decoder = new DecodeHe3TRawData();
}

He3TRawDataInputSvc::~He3TRawDataInputSvc() {
}

bool He3TRawDataInputSvc::initialize() {

	LogInfo << "InputSvc initialize " << std::endl;
        DataInputSvc::initialize();

	SniperPtr<DataSvc> pDSvc("DataSvc");
	if ( pDSvc.invalid()) throw SniperException("DataSvc is invalid!");
	m_dataSvc = pDSvc.data();

	SniperPtr<DataProvideSvc> pPSvc("DataProvideSvc");
	if ( pPSvc.invalid()) throw SniperException("DataProvideSvc is invalid!");
	m_dataPvdSvc = pPSvc.data();

	//m_dataPvdSvc->open();

	return true;
}



bool He3TRawDataInputSvc::finalize() {
	return true;
}

bool He3TRawDataInputSvc::next() {
	uint8_t *ReadRawData = NULL;
	uint32_t value  = 0x0;
	uint32_t value1 = 0x0;
	uint32_t value2 = 0x0;
	He3THit* hit;
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
	ECal*         ecal   = m_dataSvc->getObj<ECal>("/pulse/ecal");
	He3THitList*  hitcol = m_dataSvc->getObj<He3THitList>("/pulse/hits");
	EvtList*      evtcol = m_dataSvc->getObj<EvtList>("/pulse/evts");
	hitcol->clear();
	evtcol->clear();
	m_decoder->Set_ProStatus(DecodeHe3TRawData::PExecReady);

	while(true){
		ReadRawData = readByte();
		if(NULL != ReadRawData) {
			value = m_decoder->Decode_RawDataSegment(ReadRawData);
			//printf("He3TRawDataInputSvc ReadRawData %x\n",*ReadRawData);
			//LogInfo << "Pro Status: " << m_decoder->Get_ProStatus() << std::endl;

			switch(m_decoder->Get_ProStatus()){
				//================ Hdr ================
				case DecodeHe3TRawData::PulseHdr00:
					status = hdr0;
					//LogInfo << "HDR" << std::endl;
					break;
				case DecodeHe3TRawData::PulseHdr01:
					npulse->setInstrument(value);
					break;
				case DecodeHe3TRawData::PulseHdr02:
					npulse->setDetector(value);
					break;
				case DecodeHe3TRawData::PulseHdr03:
					npulse->setModule(value);
					break;
				case DecodeHe3TRawData::PulseHdr04:
					npulse->setRunMode(value);
					break;
				case DecodeHe3TRawData::PulseHdr05:
					npulse->setFrame(value);
					break;
				case DecodeHe3TRawData::PulseHdr06:
					npulse->setVersion(value);
					break;
				case DecodeHe3TRawData::PulseHdr07:
					//npulse->setDataType(value);
					break;
					//================ T0 ================
				case DecodeHe3TRawData::PulseHdr08:
					break;
				case DecodeHe3TRawData::PulseHdr09:
					break;
				case DecodeHe3TRawData::PulseHdr10:
					break;
				case DecodeHe3TRawData::PulseHdr11:
					npulse->setT0(value);
					//std::cout << std::dec << "T0 " << value << std::endl;
					//std::cout << std::hex << "T0 " << value << std::endl;
					break;
					//================ Rs1 ================
				case DecodeHe3TRawData::PulseHdr12:
					break;
				case DecodeHe3TRawData::PulseHdr13:
					break;
				case DecodeHe3TRawData::PulseHdr14:
                                        m_decoder->U24to2U12(&value, &value1, &value2);
                                        ecal->setNTube(value1);
                                        ecal->setNSample(value2);
					break;
				case DecodeHe3TRawData::PulseHdr15:
                                        ecal->setNStage(value);
					status = hdr1;
					break;
					//================ Hit ================
				case DecodeHe3TRawData::PulseHit00:
					status = hit0;
                                        m_decoder->U8to2U4(&value, &value1, &value2);
					hit = hitcol->add_item();
					hit->setTube(value2);
					//std::cout << std::dec << " TUBE  " << value2 << std::endl;
					//std::cout << std::hex << " TUBE  " << value2 << std::endl;
					break;
				case DecodeHe3TRawData::PulseHit01:
					break;
				case DecodeHe3TRawData::PulseHit02:
					break;
				case DecodeHe3TRawData::PulseHit03:
					hit = hitcol->back();
					hit->setTOF(value);
					//std::cout << std::dec << " TOF  " << value << std::endl;
					//std::cout << std::hex << " TOF  " << value << std::endl;
					break;
				case DecodeHe3TRawData::PulseHit04:
                                        break;
				case DecodeHe3TRawData::PulseHit05:
                                        break;
				case DecodeHe3TRawData::PulseHit06:
                                        break;
				case DecodeHe3TRawData::PulseHit07:
                                        m_decoder->U32toU12U20(&value, &value1, &value2);
					hit = hitcol->back();
					hit->setSu(value2);
                                        break;
				case DecodeHe3TRawData::PulseHit08:
                                        break;
				case DecodeHe3TRawData::PulseHit09:
                                        break;
				case DecodeHe3TRawData::PulseHit10:
                                        break;
				case DecodeHe3TRawData::PulseHit11:
                                        m_decoder->U32toU12U20(&value, &value1, &value2);
					hit = hitcol->back();
					hit->setSl(value2);
                                        break;
				case DecodeHe3TRawData::PulseHit12:
                                        break;
				case DecodeHe3TRawData::PulseHit13:
					hit = hitcol->back();
					hit->setQu(value);
					//std::cout << std::dec << " QU  " << value << std::endl;
					//std::cout << std::hex << " QU  " << value << std::endl;
                                        break;
				case DecodeHe3TRawData::PulseHit14:
                                        break;
				case DecodeHe3TRawData::PulseHit15:
					hit = hitcol->back();
					hit->setQl(value);
					//std::cout << std::dec << " QL  " << value << std::endl;
					//std::cout << std::hex << " QL  " << value << std::endl;
                                        status = hit1;
                                        break;
					//================ End ================
				case DecodeHe3TRawData::PulseEnd00:
					status = end0;
					break;
				case DecodeHe3TRawData::PulseEnd01:
					break;
				case DecodeHe3TRawData::PulseEnd02:
					break;
				case DecodeHe3TRawData::PulseEnd03:
					npulse->setStatus(value);
					break;
				case DecodeHe3TRawData::PulseEnd04:
					break;
				case DecodeHe3TRawData::PulseEnd05:
					break;
				case DecodeHe3TRawData::PulseEnd06:
					break;
				case DecodeHe3TRawData::PulseEnd07:
					npulse->setCount(value);
					//LogInfo << "END" << std::endl;
					status = end1;
					break;
					//================ Exe ================
				case DecodeHe3TRawData::PExecReady:
					//std::cout << "Ready: " << std::endl;
					status = redy;
					break;
				case DecodeHe3TRawData::PExecError:
					// if error, discards the old and find the new PULSE
					//LogInfo << "IO Decode Error " << std::endl;
					status = redy;
					m_decoder->Set_ProStatus(DecodeHe3TRawData::PExecReady);
					goto STARTNEXT;
					break;
			}
		}

		if(status == end1)   break;
		if(m_isLastSegment && (m_offset == m_currbuffsize)) return false;

	}

	//if(DecodeHe3TRawData::isPulseHeader(ReadRawData))  
	//else throw SniperException("Pulse Header NOT FOUND!");

	return true;

}

