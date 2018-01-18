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
#include <Python.h>
#include <stdio.h> 
#include <sstream>
#include <iostream>

#include "SniperKernel/Task.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperException.h"
#include "SniperKernel/SniperLog.h"

#include "CtrlSvc/ClearDataSvcHdl.h"
#include "DroNECore/DataSvc.h"
#include "DroNECore/DroNEIncident.h"
#include "Data/GPPDStatistic.h"

ClearDataSvcHdl::ClearDataSvcHdl(Task* par)
: DroNEIncidentHandler(par)
{
	SniperPtr<DataSvc> pSvc("DataSvc");
	if ( pSvc.invalid()) throw SniperException("DataInputSvc is invalid!");
	m_svc = pSvc.data();
}


bool ClearDataSvcHdl::handle(Incident& incident)
{
	PixelCount*     pc   = m_svc->getObj<PixelCount>("/statistic/pixel_count");
	PixelCountList* pcs  = m_svc->getObj<PixelCountList>("/statistic/pixel_counts");
        RunningInf*     ri   = m_svc->getObj<RunningInf>("/statistic/running_inf");

        pc->clear();

        uint32_t        size = pcs->size();
	for(uint32_t i = 0; i < size; i++){
           pcs->at(i)->clear();
	}

        ri->setEvtCnt(0);
        ri->setPulseCnt(0);

	return true;
}
