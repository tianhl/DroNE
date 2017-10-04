// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  REFLMWPCFastSimAlg.cc
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//
#include "REFLMWPCFastSimAlg.h"
#include "DroNECore/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include <math.h>
#include "stdlib.h"

DECLARE_ALGORITHM(REFLMWPCFastSimAlg);

REFLMWPCFastSimAlg::REFLMWPCFastSimAlg(const std::string& name)
    : AlgBase(name)
{
    m_count = 0;
}

REFLMWPCFastSimAlg::~REFLMWPCFastSimAlg()
{
}

bool
REFLMWPCFastSimAlg::initialize()
{
    LogInfo << " initialized successfully" << std::endl;

    SniperPtr<DataSvc> pSvc("DataSvc");
    if ( pSvc.invalid()) {
      return false;
    }

    m_svc = pSvc.data();
    std::cout << "get DataSvc " << m_svc->objName() << std::endl;

    m_pulse = m_svc->getObj<NeutronPulse>("/pulse");
    m_hitcol = m_svc->getObj<MWPCHitList>("/pulse/hits");
    m_evtcol = m_svc->getObj<EvtList>("/simulation/evts");

    return true;
}

bool
REFLMWPCFastSimAlg::execute()
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

bool REFLMWPCFastSimAlg::finalize()
{
	return true;
}

void REFLMWPCFastSimAlg::getXYChannel(uint32_t pid, uint32_t& xchannel, uint32_t& ychannel){
	xchannel = pid%80;
	ychannel = pid/80;
}

void REFLMWPCFastSimAlg::simulation(uint32_t xchan, uint32_t ychan, uint32_t tdata){
        double oddpositive[3]  = {0.186568195575, 0.0370793937337, 0.00289134596959};
        double oddnegative[3]  = {0.295643900541, 0.0934253011426, 0.01164744766670};
        double evenpositive[3] = {0.295643900541, 0.0934253011426, 0.01164744766670};
        double evennegative[3] = {0.186568195575, 0.0370793937337, 0.00289134596959};
        double zero = 0.372078973037;

        double* xpositive;
	double* xnegative;
	if((xchan%2)==0){ // even
		xpositive = evenpositive;
		xnegative = evennegative;
	}
	if((xchan%2)==1){ // odd
		xpositive = oddpositive;
		xnegative = oddnegative;
	}

        double* ypositive;
	double* ynegative;
	if((ychan%2)==0){ // even
		ypositive = evenpositive;
		ynegative = evennegative;
	}
	if((ychan%2)==1){ // odd
		ypositive = oddpositive;
		ynegative = oddnegative;
	}

	uint32_t xcharge = uint32_t(rand()%40000)+50000;
	uint32_t ycharge = uint32_t(rand()%40000)+50000;
	int position;
	uint32_t size = 3;

	ychan = ychan + 100;
	if(rand()>0.5){
		calculation(xchan, tdata, uint32_t(xcharge*zero), false);
		calculation(ychan, tdata, uint32_t(ycharge*zero), true);
	}
	else{
		calculation(xchan, tdata, uint32_t(xcharge*zero), true);
		calculation(ychan, tdata, uint32_t(ycharge*zero), false);
	}

	for(uint32_t i = 0; i < size; i++){
		position = xchan + i;
		if(position<=80)calculation(position, tdata, uint32_t(xcharge*xpositive[i]), true);
		position = xchan - i;
		if(position>=1 )calculation(position, tdata, uint32_t(xcharge*xnegative[i]), true);
	}
	for(uint32_t i = 0; i < size; i++){
		position = ychan + i;
		if(position<=180)calculation(position, tdata, uint32_t(ycharge*ypositive[i]), true);
		position = ychan - i;
		if(position>=101)calculation(position, tdata, uint32_t(ycharge*ynegative[i]), true);
	}
}

void REFLMWPCFastSimAlg::calculation(uint32_t position, uint32_t timedata, uint32_t qdata, bool addoffset){
	uint32_t time_offset = uint32_t(rand()%10);
	MWPCHit* hit = m_hitcol->add_item();
	hit->setChannel(position);
	if(addoffset)timedata=timedata+time_offset;
	hit->setTOF(timedata);
	hit->setCharge(qdata);
}

