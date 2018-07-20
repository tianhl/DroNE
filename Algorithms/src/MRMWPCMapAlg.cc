// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MRMWPCMapAlg.cc
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//
#include "MRMWPCMapAlg.h"
#include "DroNECore/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "tinyxml.h"
#include <math.h>
#include "boost/lexical_cast.hpp"
#include <map>

DECLARE_ALGORITHM(MRMWPCMapAlg);

MRMWPCMapAlg::MRMWPCMapAlg(const std::string& name)
    : AlgBase(name)
{
	//declProp("ConfigFileName",m_configfile);
}

MRMWPCMapAlg::~MRMWPCMapAlg()
{
}

bool
MRMWPCMapAlg::initialize()
{
    LogInfo << " initialized successfully" << std::endl;

    SniperPtr<DataSvc> pSvc("DataSvc");
    if ( pSvc.invalid()) {
      return false;
    }

    //TiXmlDocument doc;
    //doc.LoadFile(m_configfile.c_str());
    ////doc.Print();
    //TiXmlElement* root = doc.FirstChildElement();
    //for(TiXmlElement* bank = root->FirstChildElement("type")     ; bank != NULL; bank = bank->NextSiblingElement("type"))
    //for(TiXmlElement* mdul = bank->FirstChildElement("component"); mdul != NULL; mdul = mdul->NextSiblingElement("component"))
    //m_m2p.insert(MODULE2PID::value_type(boost::lexical_cast<int>(mdul->Attribute("moduleNum")),
    //                                    boost::lexical_cast<int>(mdul->Attribute("idstart"))));

    //for(MODULE2PID::iterator it=m_m2p.begin(); it!=m_m2p.end(); it++) 
    //    std::cout << "module: " << it->first << " pid: " << it->second << std::endl;



    m_svc = pSvc.data();

    m_pulse = m_svc->getObj<NeutronPulse>("/pulse");
    m_evtcol = m_svc->getObj<EvtDList>("/pulse/evts");

    return true;
}

bool MRMWPCMapAlg::execute()
{

	//uint32_t x, y, module;
	float x, y;
    uint32_t module;
	uint64_t pixelID;
	uint32_t size = m_evtcol->size();
    std::cout<<"mapping evt size: "<<size<<std::endl;
	for(uint32_t i = 0; i < size; i++){
		EvtD* evt = m_evtcol->at(i);

		x = evt->getX();
		y = evt->getY();
		module = m_pulse->getModule();
		pixelID = getPixelID(module, x, y);
		evt->setPixelID(pixelID);

	}

	return true;
}

bool MRMWPCMapAlg::finalize()
{
	//EventList* evtcol = m_svc->getObj<EventList >("/pulse/event");
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//evtcol->clear();
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//LogInfo << " finalized successfully" << std::endl;

	return true;
}

uint64_t MRMWPCMapAlg::getPixelID(uint32_t& module, float& x, float& y)
{
    //float xpos=(x+0.5)*4.0;
    //float ypos=(y-50+0.5)*2.2;
    //cout<<"pid:"<<(100001+uint64_t(uint64_t(y/2)*80+uint64_t(x/2)))<<endl;
	return uint64_t(100001+uint64_t(y/2)*100+uint64_t(x/2));	
}
