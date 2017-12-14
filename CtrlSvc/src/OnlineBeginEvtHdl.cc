// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  OnlineBeginEvtHdl.cc
//  
//
//  Created by Haolai TIAN 10th Dec 2017.
//
//
#include "CtrlSvc/OnlineBeginEvtHdl.h"
#include "DroNECore/DataInputSvc.h"

#include "SniperKernel/Incident.h"
#include "SniperKernel/SniperPtr.h"
#include "SniperKernel/SniperDataPtr.h"
#include "SniperKernel/SniperLog.h"
#include "SniperKernel/SniperException.h"


	OnlineBeginEvtHdl::OnlineBeginEvtHdl(Task* par)
: m_par(par)
{

	SniperPtr<DataInputSvc> pSvc("DataInputSvc");
	if ( pSvc.invalid()) {
		throw SniperException("DataInputSvc is invalid!");
	}
	m_iSvc = pSvc.data();

}

bool OnlineBeginEvtHdl::handle(Incident& incident)
{
	if ( m_iSvc->next() ) return true;
        else return false;
}
