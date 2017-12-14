// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  HeartBeatHdl.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include <Python.h>
#include <stdio.h> 
#include <sstream>
#include <iostream>

#include "SniperKernel/Task.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperLog.h"

#include "CtrlSvc/HeartBeatHdl.h"
#include "DroNECore/DataSvc.h"
#include "DroNECore/DroNEIncident.h"
#include "Data/GPPDStatistic.h"

HeartBeatHdl::HeartBeatHdl(Task* par)
: DroNEIncidentHandler(par)
{
	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) throw SniperException("DataInputSvc is invalid!");
	m_svc = pSvc.data();
}


bool HeartBeatHdl::handle(Incident& incident)
{

	RunningInf* ri  = m_svc->getObj<RunningInf>("/statistic/running_inf");
	LogInfo << "hitcnt " << ri->getHitCnt() << " evtcnt " <<
		ri->getEvtCnt() << " pulsecnt " <<
		ri->getPulseCnt() << std::endl;

	DroNEIncident* pIncident = getDroNEIncident(incident);
	if(pIncident == NULL) return false;

	std::string ret;
	std::stringstream retVal;

	retVal << ri->getHitCnt() << ":" << ri->getEvtCnt() << ":" << ri->getPulseCnt();
	ret = retVal.str();
	pIncident->setRetVal(ret);
	return true;
}
