// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  DumpAlg.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "DumpAlg.h"
#include "DroNECore/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "stdlib.h"
#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/SNDHit.h"
#include "Data/GPPDStatistic.h"


#include <time.h>

DECLARE_ALGORITHM(DumpAlg);

DumpAlg::DumpAlg(const std::string& name)
    : AlgBase(name)
{
	declProp("OutputFileName",m_outputfile);

	m_count = 0;
	m_evtnum = 0;
	m_hitnum = 0;
	m_xvect.clear();
	m_yvect.clear();
}

DumpAlg::~DumpAlg()
{
}

	bool
DumpAlg::initialize()
{
	LogInfo << " initialized successfully" << std::endl;

	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) {
		return false;
	}

	m_svc = pSvc.data();
	std::cout << "get DataSvc " << m_svc->objName() << std::endl;

	m_start = clock();
	return true;
}

	bool
DumpAlg::execute()
{
	++m_count;

	//NeutronPulse* pulse  = m_svc->getObj<NeutronPulse>("/pulse");
	//SNDHitList*   hitcol = m_svc->getObj<SNDHitList>("/pulse/hits");
	EvtList*      evtcol = m_svc->getObj<EvtList>("/pulse/evts");
	PixelCount*   pc     = m_svc->getObj<PixelCount>("/statistic/pixel_count");

        uint32_t pidstart = 0;
        uint32_t pidstop  = 9999;
	uint32_t c = 0;

	for(uint32_t i = pidstart; i<pidstop; i++){
		c = c+ pc->getCount(i);
	}
        std::cout << " counts: " << c << std::endl;


	return true;
}

	bool
DumpAlg::finalize()
{
	return true;
}
