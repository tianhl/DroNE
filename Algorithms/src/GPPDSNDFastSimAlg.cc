// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  GPPDSNDFastSimAlg.cc
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//
#include "GPPDSNDFastSimAlg.h"
#include "DataSvc/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include <math.h>
#include "stdlib.h"

DECLARE_ALGORITHM(GPPDSNDFastSimAlg);

GPPDSNDFastSimAlg::GPPDSNDFastSimAlg(const std::string& name)
    : AlgBase(name)
{
    m_count = 0;
}

GPPDSNDFastSimAlg::~GPPDSNDFastSimAlg()
{
}

bool
GPPDSNDFastSimAlg::initialize()
{
    LogInfo << " initialized successfully" << std::endl;

    SniperPtr<DataSvc> pSvc("DataSvc");
    if ( pSvc.invalid()) {
      return false;
    }

    m_svc = pSvc.data();
    std::cout << "get DataSvc " << m_svc->objName() << std::endl;

    m_pulse = m_svc->getObj<NeutronPulse>("/pulse");
    m_hitcol = m_svc->getObj<HitList>("/pulse/hits");
    m_evtcol = m_svc->getObj<EvtList>("/simulation/evts");

    return true;
}

bool
GPPDSNDFastSimAlg::execute()
{
    ++m_count;
    m_hitcol->clear();
    m_svc->getObj<EvtList>("/pulse/evts")->clear();

    uint32_t size = m_evtcol->size();

    for(uint32_t i = 0; i < size; i++){
	    Evt* evt = m_evtcol->at(i);
	    uint32_t tof = evt->getTOF();
	    uint32_t pid = evt->getPixelID();
	    uint32_t tdata = tof*10; // 1us => 0.1us
	    uint32_t xchan = 0;
	    uint32_t ychan = 0;

	    getXYChannel(pid, xchan, ychan);
	    simulation(xchan, ychan, tdata);
	    //std::cout << "FastSim pid: " << pid << " tof: " << tof << " x channel: " << xchan
	    //          << " y channel: " << ychan << std::endl;
    }

    LogInfo << m_count << " pulse, including " << size << " evts " << m_hitcol->size() << " hits" << std::endl; 
    return true;
}

bool GPPDSNDFastSimAlg::finalize()
{
	return true;
}

void GPPDSNDFastSimAlg::getXYChannel(uint32_t pid, uint32_t& xchannel, uint32_t& ychannel){
	xchannel = pid%111;
	ychannel = pid/111;
}

void GPPDSNDFastSimAlg::simulation(uint32_t xchan, uint32_t ychan, uint32_t tdata){
	int position;
	uint32_t xsize = uint32_t(rand()%5);
	for(uint32_t i = 0; i < xsize; i++){
		position = xchan + i;
		if(position<=111)calculation(position, tdata);
		position = xchan - i;
		if(position>=1  )calculation(position, tdata);
	}
	uint32_t ysize = uint32_t(rand()%5);
	for(uint32_t i = 0; i < ysize; i++){
		position = 127 + ychan + i;
		if(position<=175)calculation(position, tdata);
		position = 127 + ychan - i;
		if(position>=128)calculation(position, tdata);
	}
}

void GPPDSNDFastSimAlg::calculation(uint32_t position, uint32_t timedata){
	uint32_t time_offset = uint32_t(rand()%70);
	Hit* hit = m_hitcol->add_item();
	hit->setChannel(position);
	hit->setTOF(timedata+time_offset);
}

