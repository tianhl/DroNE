// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  He3TRawDataInputSvcV02.cc
//  
//
//  Created by Haolai TIAN 10th Oct. 2017.
//
//
#include "DataSvc/He3TRawDataInputSvcV02.h"
#include "DataSvc/DecodeHe3TRawDataV02.h"

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


DECLARE_SERVICE(He3TRawDataInputSvcV02);

He3TRawDataInputSvcV02::He3TRawDataInputSvcV02(const std::string& name)
: DataInputSvc(name) {
	m_decoder = new DecodeHe3TRawDataV02();
}

He3TRawDataInputSvcV02::~He3TRawDataInputSvcV02() {
}

bool He3TRawDataInputSvcV02::initialize() {

	LogInfo << "InputSvc initialize " << std::endl;

	SniperPtr<DataSvc> pDSvc("DataSvc");
	if ( pDSvc.invalid()) throw SniperException("DataSvc is invalid!");
	m_dataSvc = pDSvc.data();

	SniperPtr<DataProvideSvc> pPSvc("DataProvideSvc");
	if ( pPSvc.invalid()) throw SniperException("DataProvideSvc is invalid!");
	m_dataPvdSvc = pPSvc.data();

	//m_dataPvdSvc->open();

	return true;
}



bool He3TRawDataInputSvcV02::finalize() {
	return true;
}

bool He3TRawDataInputSvcV02::next() {
	uint8_t *ReadRawData = NULL;
	uint32_t value  = 0x0;
	uint32_t value1 = 0x0;
	uint32_t value2 = 0x0;
	uint32_t value3 = 0x0;
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
	m_decoder->Set_ProStatus(DecodeHe3TRawDataV02::PExecReady);

	while(true){
		ReadRawData = readByte();
		if(NULL != ReadRawData) {
			value = m_decoder->Decode_RawDataSegment(ReadRawData);
			//printf("He3TRawDataInputSvcV02 ReadRawData %x\n",*ReadRawData);
			//LogInfo << "Pro Status: " << m_decoder->Get_ProStatus() << std::endl;

			switch(m_decoder->Get_ProStatus()){
				//================ Hdr ================
				case DecodeHe3TRawDataV02::PulseHdr00:
					status = hdr0;
					//LogInfo << "HDR" << std::endl;
					break;
				case DecodeHe3TRawDataV02::PulseHdr01:
					npulse->setInstrument(value);
					break;
				case DecodeHe3TRawDataV02::PulseHdr02:
					npulse->setDetector(value);
					break;
				case DecodeHe3TRawDataV02::PulseHdr03:
					npulse->setModule(value);
					break;
				case DecodeHe3TRawDataV02::PulseHdr04:
					npulse->setRunMode(value);
					break;
				case DecodeHe3TRawDataV02::PulseHdr05:
					npulse->setFrame(value);
					break;
				case DecodeHe3TRawDataV02::PulseHdr06:
					npulse->setVersion(value);
					break;
				case DecodeHe3TRawDataV02::PulseHdr07:
					//npulse->setDataType(value);
					break;
					//================ T0 ================
				case DecodeHe3TRawDataV02::PulseHdr08:
					break;
				case DecodeHe3TRawDataV02::PulseHdr09:
					break;
				case DecodeHe3TRawDataV02::PulseHdr10:
					break;
				case DecodeHe3TRawDataV02::PulseHdr11:
					npulse->setT0(value);
					//std::cout << std::dec << "T0 " << value << std::endl;
					//std::cout << std::hex << "T0 " << value << std::endl;
					break;
					//================ Rs1 ================
				case DecodeHe3TRawDataV02::PulseHdr12:
					break;
				case DecodeHe3TRawDataV02::PulseHdr13:
					break;
				case DecodeHe3TRawDataV02::PulseHdr14:
					break;
				case DecodeHe3TRawDataV02::PulseHdr15:
                                        m_decoder->U32toU14U12U6(&value, &value1, &value2, &value3);
                                        ecal->setNTube(value1);
                                        ecal->setNSample(value2);
                                        ecal->setNStage(value3);
					status = hdr1;
					break;
					//================ Hit ================
				case DecodeHe3TRawDataV02::PulseHit00:
					status = hit0;
                                        m_decoder->U8to2U4(&value, &value1, &value2);
					hit = hitcol->add_item();
					hit->setTube(value2);
					//std::cout << std::dec << " TUBE  " << value2 << std::endl;
					//std::cout << std::hex << " TUBE  " << value2 << std::endl;
					break;
				case DecodeHe3TRawDataV02::PulseHit01:
					break;
				case DecodeHe3TRawDataV02::PulseHit02:
					break;
				case DecodeHe3TRawDataV02::PulseHit03:
					hit = hitcol->back();
					hit->setTOF(value);
					//std::cout << std::dec << " TOF  " << value << std::endl;
					//std::cout << std::hex << " TOF  " << value << std::endl;
					break;
				case DecodeHe3TRawDataV02::PulseHit04:
                                        break;
				case DecodeHe3TRawDataV02::PulseHit05:
                                        break;
				case DecodeHe3TRawDataV02::PulseHit06:
                                        break;
				case DecodeHe3TRawDataV02::PulseHit07:
                                        m_decoder->U32toU12U20(&value, &value1, &value2);
					hit = hitcol->back();
					hit->setSu(value2);
                                        break;
				case DecodeHe3TRawDataV02::PulseHit08:
                                        break;
				case DecodeHe3TRawDataV02::PulseHit09:
                                        break;
				case DecodeHe3TRawDataV02::PulseHit10:
                                        break;
				case DecodeHe3TRawDataV02::PulseHit11:
                                        m_decoder->U32toU12U20(&value, &value1, &value2);
					hit = hitcol->back();
					hit->setSl(value2);
                                        break;
				case DecodeHe3TRawDataV02::PulseHit12:
                                        break;
				case DecodeHe3TRawDataV02::PulseHit13:
					hit = hitcol->back();
					hit->setQu(value);
					//std::cout << std::dec << " QU  " << value << std::endl;
					//std::cout << std::hex << " QU  " << value << std::endl;
                                        break;
				case DecodeHe3TRawDataV02::PulseHit14:
                                        break;
				case DecodeHe3TRawDataV02::PulseHit15:
					hit = hitcol->back();
					hit->setQl(value);
					//std::cout << std::dec << " QL  " << value << std::endl;
					//std::cout << std::hex << " QL  " << value << std::endl;
                                        status = hit1;
                                        break;
					//================ End ================
				case DecodeHe3TRawDataV02::PulseEnd00:
					status = end0;
					break;
				case DecodeHe3TRawDataV02::PulseEnd01:
					break;
				case DecodeHe3TRawDataV02::PulseEnd02:
					break;
				case DecodeHe3TRawDataV02::PulseEnd03:
					npulse->setStatus(value);
					break;
				case DecodeHe3TRawDataV02::PulseEnd04:
					break;
				case DecodeHe3TRawDataV02::PulseEnd05:
					break;
				case DecodeHe3TRawDataV02::PulseEnd06:
					break;
				case DecodeHe3TRawDataV02::PulseEnd07:
					npulse->setCount(value);
					//LogInfo << "END" << std::endl;
					status = end1;
					break;
					//================ Exe ================
				case DecodeHe3TRawDataV02::PExecReady:
					//std::cout << "Ready: " << std::endl;
					status = redy;
					break;
				case DecodeHe3TRawDataV02::PExecError:
					// if error, discards the old and find the new PULSE
					//LogInfo << "IO Decode Error " << std::endl;
					status = redy;
					m_decoder->Set_ProStatus(DecodeHe3TRawDataV02::PExecReady);
					goto STARTNEXT;
					break;
			}
		}

		if(status == end1)   break;
		if(m_isLastSegment && (m_offset == m_currbuffsize)) return false;

	}

	//if(DecodeHe3TRawDataV02::isPulseHeader(ReadRawData))  
	//else throw SniperException("Pulse Header NOT FOUND!");

	return true;

}
