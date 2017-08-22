// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  RunningInfAlg.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "RunningInfAlg.h"
#include "DataSvc/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "stdlib.h"
#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/Hit.h"
#include "Data/GPPDStatistic.h"


DECLARE_ALGORITHM(RunningInfAlg);

RunningInfAlg::RunningInfAlg(const std::string& name)
    : AlgBase(name)
{
        declProp("TofBins", m_tofbins);
        declProp("TofStep", m_tofstep);
}

RunningInfAlg::~RunningInfAlg()
{
}

	bool
RunningInfAlg::initialize()
{
	LogInfo << " initialized successfully" << std::endl;

	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) return false;
	m_svc = pSvc.data();

	PixelCountList* pcs = m_svc->getObj<PixelCountList>("/statistic/pixel_counts");
        for(uint32_t i = 0; i < m_tofbins; i++) pcs->add_item();

	return true;
}

	bool
RunningInfAlg::execute()
{
	NeutronPulse* pulse = m_svc->getObj<NeutronPulse>("/pulse");
	HitList* hitcol = m_svc->getObj<HitList>("/pulse/hits");
	EvtList* evtcol = m_svc->getObj<EvtList>("/pulse/evts");

	RunningInf* ri  = m_svc->getObj<RunningInf>("/statistic/running_inf");
        ri->addHitCnt(hitcol->size());
        ri->addEvtCnt(evtcol->size());
        ri->addPulseCnt(1);

	PixelCount*     pc  = m_svc->getObj<PixelCount>("/statistic/pixel_count");
	PixelCountList* pcs = m_svc->getObj<PixelCountList>("/statistic/pixel_counts");
	for(uint32_t i = 0; i < evtcol->size(); i++){
		Evt* evt = evtcol->at(i);
		pc->addCount(evt->getPixelID(), 1);
		uint32_t time = evt->getTOF();
                uint32_t chan = time/m_tofstep;
                if(chan>m_tofbins)continue; 
                pcs->at(chan)->addCount(evt->getPixelID(), 1);
	}


	return true;
}

	bool
RunningInfAlg::finalize()
{
	return true;
}
