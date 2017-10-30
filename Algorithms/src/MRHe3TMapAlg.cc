// @(#)DroNE/Algorithms:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  MRHe3TMapAlg.cc
//  
//
//  Created by Haolai TIAN 1st Oct. 2017.
//
//
#include "MRHe3TMapAlg.h"
#include "DroNECore/DataSvc.h"
#include "SniperKernel/AlgFactory.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperPtr.h"
#include "tinyxml.h"
#include <math.h>
#include "boost/lexical_cast.hpp"
#include <map>

DECLARE_ALGORITHM(MRHe3TMapAlg);

MRHe3TMapAlg::MRHe3TMapAlg(const std::string& name)
    : AlgBase(name)
{
	//declProp("ConfigFileName",m_configfile);
}

MRHe3TMapAlg::~MRHe3TMapAlg()
{
}

bool
MRHe3TMapAlg::initialize()
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
    m_evtcol = m_svc->getObj<EvtList>("/pulse/evts");

    return true;
}

bool MRHe3TMapAlg::execute()
{

	uint32_t x, y, module;
	uint64_t pixelID;
	uint32_t size = m_evtcol->size();

	for(uint32_t i = 0; i < size; i++){
		Evt* evt = m_evtcol->at(i);

		x = evt->getX();
		y = evt->getY();
		module = m_pulse->getModule();
		pixelID = getPixelID(module, x, y);
		evt->setPixelID(pixelID);

	}

	return true;
}

bool MRHe3TMapAlg::finalize()
{
	//EventList* evtcol = m_svc->getObj<EventList >("/pulse/event");
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//evtcol->clear();
	//LogInfo << "Event Collection Size: " << evtcol->size() << std::endl;
	//LogInfo << " finalized successfully" << std::endl;

	return true;
}

uint64_t MRHe3TMapAlg::getPixelID(uint32_t& module, uint32_t& x, uint32_t& y)
{
        return uint64_t(y*1000+uint64_t(x/8)+1);
}
