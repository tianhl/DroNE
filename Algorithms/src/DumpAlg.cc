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
#include "DataSvc/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include "stdlib.h"
#include "Data/Pulse.h"
#include "Data/Evt.h"
#include "Data/Hit.h"
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

	NeutronPulse* pulse = m_svc->getObj<NeutronPulse>("/pulse");
	//LogInfo << "Instrument: " << pulse->getInstrument() << std::endl;
	//LogInfo << "Detector:   " << pulse->getDetector() << std::endl;
	//LogInfo << "Count:      " << pulse->getCount() << std::endl;


	HitList* hitcol = m_svc->getObj<HitList>("/pulse/hits");
	//std::cout << hitcol->size() << " hits are collected!" << std::endl;
	m_hitnum += hitcol->size();
	//for(uint32_t i = 0; i < hitcol->size(); i++){
	//  Hit* hit = hitcol->at(i);
	//  std::cout << "iterator " << std::dec << i << " channel " << hit->getChannel() << " tof " << hit->getTOF() << std::endl;
	//}

	EvtList* evtcol = m_svc->getObj<EvtList>("/pulse/evts");
	for(uint32_t i = 0; i < evtcol->size(); i++){
		Evt* evt = evtcol->at(i);
		LogInfo << "Module:      " << pulse->getModule() << std::endl;
		std::cout << hitcol->size() << " hits are collected!" << std::endl;
		std::cout << evtcol->size() << " evts are collected!" << std::endl;
		std::cout << std::dec << " TOF  "<<evt->getTOF() << " PID " << evt->getPixelID() << std::endl;
		//if(1100000==evt->getPixelID()) std::cout << "ERROR: " << std::endl;
	}

	m_evtnum += evtcol->size();

	PixelCount* pc  = m_svc->getObj<PixelCount>("/statistic/pixel_count");
	for(uint32_t i = 0; i < evtcol->size(); i++){
		Evt* evt = evtcol->at(i);
		//std::cout << std::dec <<  evt->getPixelID() << " " << evt->getX() << ", " << evt->getY() << ",  " << evt->getTOF() << "," << evt->getPixelID() << std::endl;
		//if(1100000==evt->getPixelID()) std::cout << "ERROR: " << std::endl;
	}

	//LogInfo << "Pulse T0:  " << pulse->getT0()  << ". " << hitcol->size() << " hits are collected, " << evtcol->size() << " evts are reconstructed in this pulse"<< std::endl;



	//if((m_count%200)==0){
	//	m_ofstream.open(m_outputfile.c_str());
	//	time_t now;
	//	time(&now);
	//	m_ofstream << now << " " << m_count << " " << m_evtnum << " " << m_hitnum << std::endl;
	//	KEYS& pids = pc->getPixelIDs();
	//	for(KEYS::iterator it = pids.begin(); it < pids.end(); it++){
	//		int p = (*it)%10000;
	//		m_ofstream <<  p  << " " << (p%111)*4 <<" " << ceil(p/111)*4	
	//			<< " " << pc->getCount(*it) << std::endl;
	//	}
	//	m_ofstream.close();
	//}


	return true;
}

	bool
DumpAlg::finalize()
{
	//EventList* evtcol = m_svc->getObj<EventList >("/pulse/event");
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//evtcol->clear();
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//LogInfo << " finalized successfully" << std::endl;

	PixelCount* pc  = m_svc->getObj<PixelCount>("/statistic/pixel_count");
	m_ofstream.open(m_outputfile.c_str());
	time_t now;
	time(&now);
	m_ofstream << now << " " << m_count << " " << m_evtnum << " " << m_hitnum << std::endl;
	KEYS& pids = pc->getPixelIDs();
	//	for(KEYS::iterator it = pids.begin(); it < pids.end(); it++){

	for(int i = 0; i < 111*48; i++){
		m_ofstream <<  i  << " " << pc->getCount(i+1100000) << std::endl;
	}
	for(int i = 111*48; i < 111*48*2; i++){
		m_ofstream <<  i  << " " <<rand()/10000000 << std::endl;
	}
	for(int i = 111*48*2; i < 111*48*3; i++){
		m_ofstream <<  i  << " " <<rand()/10000000 << std::endl;
	}

	/*
	   int p = (*it)%10000;
	   m_ofstream <<  *it  << " " << (p%111)*4 <<" " << ceil(p/111)*4	
	   << " " << pc->getCount(*it) << std::endl;
	   }
	   */
	m_ofstream.close();

	m_finish = clock();
	clock_t duration = (m_finish-m_start)/CLOCKS_PER_SEC;
	LogInfo << m_count  << " pulses are collected" << std::endl;
	LogInfo << m_hitnum << " hits   are collected" << std::endl;
	LogInfo << m_evtnum << " evts   are collected" << std::endl;
	LogInfo << duration << " secs are used!" << std::endl;
	return true;
}
