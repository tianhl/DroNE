// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  SaveDataAlg.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "SaveDataAlg.h"
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

DECLARE_ALGORITHM(SaveDataAlg);

SaveDataAlg::SaveDataAlg(const std::string& name)
    : AlgBase(name)
{
	declProp("OutputFileName1",m_outputfile1);
	declProp("OutputFileName2",m_outputfile2);

	m_count = 0;
	m_evtnum = 0;
	m_hitnum = 0;
	m_xvect.clear();
	m_yvect.clear();
}

SaveDataAlg::~SaveDataAlg()
{
}

	bool
SaveDataAlg::initialize()
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
SaveDataAlg::execute()
{
	++m_count;

	NeutronPulse* pulse  = m_svc->getObj<NeutronPulse>("/pulse");
	//SNDHitList*   hitcol = m_svc->getObj<SNDHitList>("/pulse/hits");
	EvtList*      evtcol = m_svc->getObj<EvtList>("/pulse/evts");
	//PixelCount*   pc     = m_svc->getObj<PixelCount>("/statistic/pixel_count");


	for(uint32_t i = 0; i < evtcol->size(); i++){
                m_ofstream1.open(m_outputfile1.c_str(),ios::app);
                m_ofstream2.open(m_outputfile2.c_str(),ios::app);
                Evt* evt = evtcol->at(i);
                int evtPid=evt->getPixelID();
		m_ofstream1 <<  evtPid << ";" << float((evt->getTOF())/40) <<";"<<pulse->getT0()<< std::endl;
               // m_ofstream2 <<  evtPid << ";" << pulse->getT0() << std::endl;
                m_ofstream1.close();
               // m_ofstream2.close();

	}

        //uint32_t pidstart = 0;
        //uint32_t pidstop  = 9999;
	//uint32_t c = 0;

	//for(uint32_t i = pidstart; i<pidstop; i++){
	//	c = c+ pc->getCount(i);
	//}
        //std::cout << " counts: " << c << std::endl;


	return true;
}

	bool
SaveDataAlg::finalize()
{
	return true;
}
