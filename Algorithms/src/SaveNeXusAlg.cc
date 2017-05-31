// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  SaveNeXusAlg.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "SaveNeXusAlg.h"
#include "DataSvc/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include <math.h>

DECLARE_ALGORITHM(SaveNeXusAlg);

SaveNeXusAlg::SaveNeXusAlg(const std::string& name)
    : AlgBase(name)
{
}

SaveNeXusAlg::~SaveNeXusAlg()
{
}

bool
SaveNeXusAlg::initialize()
{
    LogInfo << " initialized successfully" << std::endl;

    SniperPtr<DataSvc> pSvc("DataSvc");
    if ( pSvc.invalid()) {
      return false;
    }

    m_svc = pSvc.data();

    m_pulse = m_svc->getObj<NeutronPulse>("/pulse");
    m_evtcol = m_svc->getObj<EvtList>("/pulse/evts");

    return true;
}

bool SaveNeXusAlg::execute()
{

	uint64_t pixelID;
	uint32_t tof;
	uint32_t t0 = m_pulse->getT0();

	uint32_t size = m_evtcol->size();
	for(uint32_t i = 0; i < size; i++){
		Evt* evt = m_evtcol->at(i);
		pixelID = evt->getPixelID();
		tof     = evt->getTOF();

	}

	return true;
}

bool SaveNeXusAlg::finalize()
{
	//EventList* evtcol = m_svc->getObj<EventList >("/pulse/event");
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//evtcol->clear();
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	LogInfo << " finalized successfully max size of vector is " << m_nexusEvt.max_size() << std::endl;

	return true;
}

