// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  ClearDataSvcHdl.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "CtrlSvc/ClearDataSvcHdl.h"
#include "DroNECore/DataSvc.h"
#include "DroNECore/DroNEIncident.h"
#include "Data/GPPDStatistic.h"

#include "SniperKernel/Task.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperLog.h"

#include <stdio.h> 
#include <sstream>
#include <iostream>
#include <Python.h>

ClearDataSvcHdl::ClearDataSvcHdl(Task* par)
: DroNEIncidentHandler(par)
{
	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) throw SniperException("DataInputSvc is invalid!");
	m_svc = pSvc.data();
}


bool ClearDataSvcHdl::handle(Incident& incident)
{
	DroNEIncident* pIncident = getDroNEIncident(incident);
	if(pIncident == NULL) return false;

	PixelCount*     pc   = m_svc->getObj<PixelCount>("/statistic/pixel_count");
	PixelCountList* pcs  = m_svc->getObj<PixelCountList>("/statistic/pixel_counts");
        pc->clear();

        uint32_t        size = pcs->size();
	for(uint32_t i = 0; i < size; i++){
           pcs->at(i)->clear();
	}

	//int pidstart(getInt("pidstart"));
	//int pidsize(getInt("pidsize"));
        std::string retval("true");
	pIncident->setRetVal(retval);
	return true;
}
