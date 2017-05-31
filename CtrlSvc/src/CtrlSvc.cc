// @(#)DroNE/CtrlSvc:$Id$
// Authors: H.L. TIAN 2017
/**********************************************************************
 *                                                                    *
 * Copyright (c) 2017 Data Analysis & Instrument Software, CSNS       *
 *                                                                    *
 *                                                                    *
 **********************************************************************/
//
//  CtrlSvc.cc
//  
//
//  Created by Haolai TIAN 31th May 2017.
//
//
#include "CtrlSvc/CtrlSvc.h"
#include "CtrlSvc/HeartBeatHdl.h"
#include "CtrlSvc/HelloDroNEHdl.h"
#include "CtrlSvc/PushHistHdl.h"
#include "CtrlSvc/BeginEvtHdl.h"
#include "CtrlSvc/EndEvtHdl.h"
#include "SniperKernel/SvcFactory.h"
#include "SniperKernel/Task.h"
#include <iostream>
#include <list>

DECLARE_SERVICE(CtrlSvc);

CtrlSvc::CtrlSvc(const std::string& name)
    : SvcBase(name)
{
}

CtrlSvc::~CtrlSvc()
{
}

bool CtrlSvc::initialize()
{

	Task* par = getScope();

	IIncidentHandler* hi = new HeartBeatHdl(par);
	if ( par->isTop() ) hi->regist("HeartBeat");
	else hi->regist(par->scope() + par->objName() + ":HeartBeat");
	hi->listening();

	IIncidentHandler* di = new HelloDroNEHdl(par);
	if ( par->isTop() ) di->regist("HelloDroNE");
	else di->regist(par->scope() + par->objName() + ":HelloDroNE");
	di->listening();

	IIncidentHandler* pi = new PushHistHdl(par);
	if ( par->isTop() ) pi->regist("PushHist");
	else pi->regist(par->scope() + par->objName() + ":PushHist");
	pi->listening();

	IIncidentHandler* bi = new BeginEvtHdl(par);
	bi->regist("BeginEvent");
	bi->listening();

	IIncidentHandler* ei = new EndEvtHdl(par);
	ei->regist("EndEvent");
	ei->listening();

	m_icdts.push_back(hi);
	m_icdts.push_back(di);
	m_icdts.push_back(pi);
	m_icdts.push_back(bi);
	m_icdts.push_back(ei);

	std::cout << "ctrlsvc incidents size: " << m_icdts.size() << std::endl;

	return true;
}

bool CtrlSvc::finalize()
{
	return true;
}
