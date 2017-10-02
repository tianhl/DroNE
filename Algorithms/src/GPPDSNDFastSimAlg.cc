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

    uint32_t size = m_evtcol->size();
    LogInfo << m_count << " pulse, including " << size << " evts needed to be simulated" << std::endl; 

    for(uint32_t i = 0; i < size; i++){
	    Evt* evt = m_evtcol->at(i);
	    uint32_t tof = evt->getTOF();
	    uint32_t pid = evt->getPixelID();
            uint32_t tdata = tof*80;
            uint32_t xchan = 0;
            uint32_t ychan = 0;
       
            getXYChannel(pid, xchan, ychan);
            //std::cout << "FastSim pid: " << pid << " tof: " << tof << " x channel: " << xchan
            //          << " y channel: " << ychan << std::endl;
            simulation(xchan, ychan, tdata);
    }

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
	m_hitcol->clear();

}

