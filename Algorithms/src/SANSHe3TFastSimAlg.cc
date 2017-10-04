// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  SANSHe3TFastSimAlg.cc
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//
#include "SANSHe3TFastSimAlg.h"
#include "DroNECore/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include <math.h>
#include "stdlib.h"

DECLARE_ALGORITHM(SANSHe3TFastSimAlg);

SANSHe3TFastSimAlg::SANSHe3TFastSimAlg(const std::string& name)
    : AlgBase(name)
{
    m_count = 0;
}

SANSHe3TFastSimAlg::~SANSHe3TFastSimAlg()
{
}

bool
SANSHe3TFastSimAlg::initialize()
{
    LogInfo << " initialized successfully" << std::endl;

    SniperPtr<DataSvc> pSvc("DataSvc");
    if ( pSvc.invalid()) {
      return false;
    }

    m_svc = pSvc.data();
    std::cout << "get DataSvc " << m_svc->objName() << std::endl;

    m_pulse = m_svc->getObj<NeutronPulse>("/pulse");
    m_hitcol = m_svc->getObj<He3THitList>("/pulse/hits");
    m_evtcol = m_svc->getObj<EvtList>("/simulation/evts");

    return true;
}

bool
SANSHe3TFastSimAlg::execute()
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

bool SANSHe3TFastSimAlg::finalize()
{
	return true;
}

void SANSHe3TFastSimAlg::getXYChannel(uint32_t pid, uint32_t& xchannel, uint32_t& ychannel){
	xchannel = pid%80;
	ychannel = pid/80;
}

void SANSHe3TFastSimAlg::simulation(uint32_t xchan, uint32_t ychan, uint32_t tdata){
       double R = xchan/80.0;
       uint32_t QA = 0;
       uint32_t QB = 0;

       if(R<0.5){
	       QB = (rand()%1024+1);
	       QA = R*QB/(1-R);
       }
       else{
	       QA = (rand()%1024+1);
	       QB = QA*(1-R)/R;
       }

       calculation(ychan, tdata, QA, QB);
}

void SANSHe3TFastSimAlg::calculation(uint32_t tub, uint32_t tof, uint32_t qu, uint32_t ql){
	He3THit* hit = m_hitcol->add_item();
	hit->setTube(tub);
	hit->setTOF(tof);
	hit->setQu(qu);
	hit->setQl(ql);
}

