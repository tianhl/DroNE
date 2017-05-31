// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  REFLMAPMTSimAlg.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "REFLMAPMTSimAlg.h"
#include "DataSvc/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/Incident.h"

#include "stdlib.h"
#include "Data/Pulse.h"
#include "Data/Hit.h"
#include "Data/Evt.h"

#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

DECLARE_ALGORITHM(REFLMAPMTSimAlg);

REFLMAPMTSimAlg::REFLMAPMTSimAlg(const std::string& name)
: AlgBase(name)
{
	declProp("InputFileName",m_inputfilename);
	declProp("MaxTof",m_maxtof);
	declProp("MaxDet",m_maxdet);
	//declProp("DetXMax",m_detXMax);
	m_count = 0;
}

REFLMAPMTSimAlg::~REFLMAPMTSimAlg()
{
}

bool REFLMAPMTSimAlg::initialize()
{
	LogInfo << " initialized successfully" << std::endl;

	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) return false;
	m_svc = pSvc.data();

	m_ifstream.open(m_inputfilename.c_str());
	string samplebuff;
	getline(m_ifstream, samplebuff);
	m_totcnt = 0;
	m_curcnt = 0;
	m_cmap = new uint32_t[m_maxtof*m_maxdet];

	for (int tofidx=0;tofidx<m_maxtof ;tofidx++){
		getline(m_ifstream, samplebuff);
		vector<string> substring;
		boost::split( substring, samplebuff, boost::is_any_of( ";" ), boost::token_compress_on );
		for(uint32_t detidx = 0; detidx < m_maxdet  ; detidx++){
			m_cmap[mapIdx(tofidx, detidx)] =atoi(substring[detidx+1].c_str());
			m_cmap[mapIdx(tofidx, detidx)] =(int)(atoi(substring[detidx+1].c_str()));
			m_totcnt += (int)(atoi(substring[detidx+1].c_str()));
		}
	}
	std::cout << "total neutron hit count: " << m_totcnt << std::endl;
	m_ifstream.close();

	return true;
}

	bool
REFLMAPMTSimAlg::execute()
{
	++m_count;
	Hit* hit;
	//if((m_count%100000)==0) LogInfo << "Hello world: count: " << m_count << std::endl;

	NeutronPulse* pulse = m_svc->getObj<NeutronPulse>("/pulse");
	HitList* hitcol     = m_svc->getObj<HitList>("/pulse/hits");
	EvtList* evtcol     = m_svc->getObj<EvtList>("/pulse/evts");
	hitcol->clear();
	evtcol->clear();

	uint32_t evtcount = 0;
	uint32_t maxevt   = 4000;
	if(m_curcnt == m_totcnt) Incident::fire("StopRun");

	for(int i = 0; i < m_maxtof; i++){
		for(int j = 0; j < m_maxdet; j++){
			uint32_t detid = mapIdx(i,j);
			double rdn = (rand()%100)/100.0;
			if((0<m_cmap[detid]) and (rdn<(m_cmap[detid]/double(m_totcnt-m_curcnt)))){
				m_cmap[detid] = m_cmap[detid] - 1;
				evtcount++;
				m_curcnt++;
				if(evtcount >= maxevt)goto ENDPULSE;
				//std::cout << "add evt tof: " << i << "; det: " << j << std::endl;
				//hit = hitcol->add_item();
				//hit->setChannel(value);
				//hit->setTOF(value);
			}
		}

	}
	//pulse->setInstrument();
	//pulse->setDetector();
	//pulse->setModule(value);
	//pulse->setRunMode(value);
	//pulse->setFrame(value);
	//pulse->setVersion(value);
	//pulse->setT0(value);
ENDPULSE:
	std::cout << "END NEW PULSE tot: " << m_totcnt << " cur: " << m_curcnt << std::endl;

	//pulse->setStatus(value);
	//pulse->setCount();

	return true;
}

	bool
REFLMAPMTSimAlg::finalize()
{
	std::cout << " simulation evt: " << m_count << std::endl;
	return true;
}

uint32_t REFLMAPMTSimAlg::mapIdx(uint32_t tofidx, uint32_t detidx){
	return tofidx*m_maxdet+detidx;
}

