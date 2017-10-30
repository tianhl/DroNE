// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  HelloDroNEHdl.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include <time.h>
#include <stdio.h> 
#include <sstream>
#include <iostream>
#include <Python.h>

#include "SniperKernel/Task.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"

#include "CtrlSvc/HelloDroNEHdl.h"
#include "DroNECore/DataSvc.h"
#include "DroNECore/DroNEIncident.h"
#include "Data/GPPDStatistic.h"

HelloDroNEHdl::HelloDroNEHdl(Task* par)
: DroNEIncidentHandler(par)
{
	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) throw SniperException("DataInputSvc is invalid!");
	m_svc = pSvc.data();
}


bool HelloDroNEHdl::handle(Incident& incident)
{
        
	RunningInf* ri  = m_svc->getObj<RunningInf>("/statistic/running_inf");
        std::cout << "hitcnt " << ri->getHitCnt() << " evtcnt " <<
        ri->getEvtCnt() << " pulsecnt " <<
        ri->getPulseCnt() << std::endl;

        DroNEIncident* pIncident = getDroNEIncident(incident);
        if(pIncident == NULL) return false;

	std::string ret;
	time_t t = time(NULL);

	std::string vstringstring(getChar("string"));
	std::cout << "string: " << vstringstring << std::endl;

	int vintint(getInt("times"));
	std::cout << "times: " << vintint << std::endl;

	for (int i = 0 ; i < vintint; i++){
		ret += vstringstring;
	}


	std::stringstream retVal;
	retVal << t;
	ret += retVal.str();
	pIncident->setRetVal(ret);
	return true;
}
