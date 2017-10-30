// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  PushMatrixHdl.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include <stdio.h> 
#include <sstream>
#include <iostream>
#include <Python.h>

#include "SniperKernel/Task.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperLog.h"

#include "CtrlSvc/PushMatrixHdl.h"
#include "DroNECore/DataSvc.h"
#include "DroNECore/DroNEIncident.h"
#include "Data/GPPDStatistic.h"

PushMatrixHdl::PushMatrixHdl(Task* par)
: DroNEIncidentHandler(par)
{
	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) throw SniperException("DataInputSvc is invalid!");
	m_svc = pSvc.data();
}


bool PushMatrixHdl::handle(Incident& incident)
{

	PixelCountList* pcs  = m_svc->getObj<PixelCountList>("/statistic/pixel_counts");
        uint32_t   size  = pcs->size();

	DroNEIncident* pIncident = getDroNEIncident(incident);
	if(pIncident == NULL) return false;

	std::stringstream retVal;
        int pidstart(getInt("pidstart"));
        int pidsize(getInt("pidsize"));
	//KEYS& pids = pc->getPixelIDs();
        retVal << "[";
	for(uint32_t i = 0; i < uint32_t(pidsize); i++){
		if(0<i)retVal << ","; 
		retVal << "[";
		for(uint32_t j = 0; j < size; j++){
                        if(0<j)retVal << ",";
			retVal << pcs->at(j)->getCount(i+pidstart);
		}
		retVal << "]";
	}
	retVal << "]";

	std::string ret;
	ret = retVal.str();
	//std::cout << ret << std::endl;
	pIncident->setRetVal(ret);
	return true;
}
