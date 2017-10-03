// @(#)DroNE/DataSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  SimNeutronGunInputSvc.cc
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/Incident.h"
#include "SniperKernel/Task.h"

#include "Data/Evt.h"
#include "DataSvc/SimNeutronGunInputSvc.h"
#include "DroNECore/DataSvc.h"

#include "stdlib.h"
#include <vector>
#include <boost/algorithm/string/classification.hpp>
#include <boost/algorithm/string/split.hpp>

DECLARE_SERVICE(SimNeutronGunInputSvc);

SimNeutronGunInputSvc::SimNeutronGunInputSvc(const std::string& name)
: DataInputSvc(name)
{
	declProp("InputFileName",m_inputfilename);
	declProp("MaxTof",m_maxtof);
	declProp("MaxDet",m_maxdet);
	declProp("MaxEvt",m_maxevt);
	m_count = 0;
}

SimNeutronGunInputSvc::~SimNeutronGunInputSvc()
{
}

bool SimNeutronGunInputSvc::initialize()
{
	LogInfo << " initialized successfully" << std::endl;

	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) throw SniperException("DataSvc is invalid!");
	m_svc = pSvc.data();

	m_ifstream.open(m_inputfilename.c_str());
	string samplebuff;
	vector<string> substring;

	m_totcnt = 0;
	m_curcnt = 0;
	m_cmap = new uint32_t[m_maxtof*m_maxdet];
	m_dmap = new uint32_t[m_maxdet];
	m_tmap = new uint32_t[m_maxtof];

	getline(m_ifstream, samplebuff);
	boost::split( substring, samplebuff, boost::is_any_of( ";" ), boost::token_compress_on );

	for(int detidx = 0; detidx < m_maxdet; detidx++){
               m_dmap[detidx] = (int)(atoi(substring[detidx+1].c_str()));
	}

	for (int tofidx=0;tofidx<m_maxtof ;tofidx++){
		substring.clear();
		getline(m_ifstream, samplebuff);
		boost::split( substring, samplebuff, boost::is_any_of( ";" ), boost::token_compress_on );
		m_tmap[tofidx] = atoi(substring[0].c_str());
		for(int detidx = 0; detidx < m_maxdet  ; detidx++){
			m_cmap[mapIdx(tofidx, detidx)] =(int)(atoi(substring[detidx+1].c_str()));
			m_totcnt += m_cmap[mapIdx(tofidx, detidx)];
		}
	}
	LogInfo << "total neutron hit count: " << m_totcnt << std::endl;
	m_ifstream.close();

	return true;
}

bool SimNeutronGunInputSvc::next()
{
	++m_count;
	Evt* evt;
	//if((m_count%100000)==0) LogInfo << "Hello world: count: " << m_count << std::endl;

	EvtList* evtcol     = m_svc->getObj<EvtList>("/simulation/evts");
	evtcol->clear();

	uint32_t evtcount = 0;
	uint32_t maxevt   = m_maxevt;//4000;
	if(m_curcnt == m_totcnt) Incident::fire("StopRun");

	for(int i = 0; i < m_maxdet; i++){
		for(int j = 0; j < m_maxtof; j++){
			uint32_t detid = mapIdx(j,i);
			double rdn = (rand()%100)/100.0;
                        uint32_t r = m_totcnt-m_curcnt;
                        double psb = m_cmap[detid]/double(r);
                        if(r<(maxevt*10))psb=1.0;
			if((0<m_cmap[detid]) and (rdn<psb)){
				m_cmap[detid] = m_cmap[detid] - 1;
				evtcount++;
				m_curcnt++;
				//std::cout << "add evt tof: [" << i << "]" << m_tmap[i] 
                                //          << "; det: [" << j << "]" << m_dmap[j] << std::endl;
				evt = evtcol->add_item();
				evt->setPixelID(m_dmap[i]);
				evt->setTOF(m_tmap[j]);
				if(evtcount >= maxevt)goto ENDPULSE;
                                break;
			}
		}

	}

ENDPULSE:
	LogInfo << "END NEW PULSE tot: " << m_totcnt << " provided evts: " << m_curcnt 
		<< " evts in this pulse:"<< evtcount << std::endl;

	return true;
}

	bool
SimNeutronGunInputSvc::finalize()
{
	LogInfo << " simulation evt: " << m_count << std::endl;
	delete []m_cmap;
	delete []m_dmap;
	delete []m_tmap;
	m_cmap = NULL;
	m_dmap = NULL;
	m_tmap = NULL;
	return true;
}

uint32_t SimNeutronGunInputSvc::mapIdx(uint32_t tofidx, uint32_t detidx){
	return tofidx*m_maxdet+detidx;
}

