// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MRHe3TRecAlg.cc
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//
#include "MRHe3TRecAlg.h"
#include "DroNECore/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"

#include <math.h>

DECLARE_ALGORITHM(MRHe3TRecAlg);

MRHe3TRecAlg::MRHe3TRecAlg(const std::string& name)
    : AlgBase(name)
{
    m_count = 0;
}

MRHe3TRecAlg::~MRHe3TRecAlg()
{
}

bool
MRHe3TRecAlg::initialize()
{
    LogInfo << " initialized successfully" << std::endl;

    SniperPtr<DataSvc> pSvc("DataSvc");
    if ( pSvc.invalid()) {
      return false;
    }

    m_svc = pSvc.data();
    std::cout << "get DataSvc " << m_svc->objName() << std::endl;

    m_pulse  = m_svc->getObj<NeutronPulse>("/pulse");
    m_ecal   = m_svc->getObj<ECal>("/pulse/ecal");
    m_hitcol = m_svc->getObj<He3THitList>("/pulse/hits");
    m_evtcol = m_svc->getObj<EvtList>("/pulse/evts");

    return true;
}

bool
MRHe3TRecAlg::execute()
{
    ++m_count;
    uint32_t ntube = m_ecal->getNTube();
    uint32_t nsamp = m_ecal->getNSample();
    uint32_t nstag = m_ecal->getNStage();

    uint32_t size = m_hitcol->size();

    for(uint32_t i = 0; i < size; i++){
	    He3THit* hit = m_hitcol->at(i);
            uint32_t qa   = hit->getQu();
            uint32_t qb   = hit->getQl();
            uint32_t su   = hit->getSu();
            uint32_t sl   = hit->getSl();
            uint32_t time = hit->getTOF();
            uint32_t Y    = hit->getTube();
            uint32_t X    = uint32_t(double(qa-qb)/(qa+qb)+500);
            
	    Evt* evt = m_evtcol->add_item();
	    evt->setX(X);
	    evt->setY(Y);
	    evt->setTOF(time);
    }

        LogInfo << "Num of Pulse: " << m_count << " , includeing " 
    	    << m_hitcol->size() << " hits and " << m_evtcol->size() << " evts reconstruced" << std::endl;
    return true;
}

bool MRHe3TRecAlg::finalize()
{
	//EventList* evtcol = m_svc->getObj<EventList >("/pulse/event");
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//evtcol->clear();
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//LogInfo << " finalized successfully" << std::endl;

	return true;
}
